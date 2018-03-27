//
// Copyright (c) 2015, Paul R. Swan
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include "CStarWarsBaseGame.h"
#include "C6809ECpu.h"

//
// Notes
// -----
//
// * The Clock output from the ICT should be connected to IC 1N (S04) pin 1 that's
//   been disconnected from the 12MHz clock source (e.g. socket 1N and insert the IC
//   with pin 1 bent out and connect the ICT clock to the bent out pin).
//
// * E is not used to enable the bus (E, Q are unused).
//   If RW is high memory is read (there is no idle bus state).
//
// * BS, BA, AVMA, LIC, BUSY, TSC, HALT, FIRQ & NMI are all unused on this platform.
//   It's the simplest bus implementation possible.
//
// * The CPU board can be worked on alone - the AVG & interconnect PCB's are not required.
//

//
// Programmatically used definitions
//

// ROM bank switching
static const UINT32 c_MPAGE_A  = 0x4684;
static const UINT8  c_MPAGE_D  = 0x80;

// Divider
static const UINT32 c_REH_A    = 0x4700; // Quotient Hi
static const UINT32 c_REL_A    = 0x4701; // Quotient Lo

static const UINT32 c_DVSRH_A  = 0x4704; // Divisor Hi, also clears quotient, loads dividend shift register
static const UINT32 c_DVSRL_A  = 0x4705; // Divisor Lo, also starts the divider

static const UINT32 c_DVDDH_A  = 0x4706; // Dividend Hi
static const UINT32 c_DVDDL_A  = 0x4707; // Dividend Lo

// Matrix
static const UINT32 c_MBRAM_A  = 0x5000; // Matrix RAM base address

static const UINT32 c_MBRUN_A  = 0x4320; // Matrix MATH RUN
static const UINT32 c_MBRUN_D  = 0x80;   // Matrix MATH RUN bit

static const UINT32 c_MW0_A    = 0x4700; // Matrix Write 0 (Program Address MPA2-MPA9)
static const UINT32 c_MW1_A    = 0x4701; // Matrix Write 1 (Block Index Hi BIC8)
static const UINT32 c_MW2_A    = 0x4702; // Matrix Write 2 (Block Index Lo BIC0-BIC7)

// ADC
static const UINT32 c_ADC_A       = 0x4380; // ADC read address

static const UINT32 c_ADCSTART0_A = 0x46C0; // ADC start channel 0
static const UINT32 c_ADCSTART1_A = 0x46C1; // ADC start channel 1
static const UINT32 c_ADCSTART2_A = 0x46C2; // ADC start channel 2

//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                  "012", "012345"
                                                  {NO_BANK_SWITCH, 0x4800,      0x4FFF,      1, 0xFF, "2FH", "Prog. "}, // "Program RAM, 6116, CPU"
                                                  {NO_BANK_SWITCH, c_MBRAM_A,   0x5FFF,      1, 0xFF, "5FH", "MB AB "}, // "Matrix RAM 2x6116, 16-bit, CPU"
                                                  {0}
                                                }; // end of list

//
// Input region is the same for all versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                      "012", "012345"
                                                      {NO_BANK_SWITCH, 0x4300,    0xFF,       "9J ", "IN0   "}, // Input port 0
                                                      {NO_BANK_SWITCH, 0x4320,    0xFF,       "9H ", "IN1   "}, // Input port 1
                                                      {NO_BANK_SWITCH, 0x4340,    0xFF,       "9E ", "OPT0  "}, // DIP Switch 10D
                                                      {NO_BANK_SWITCH, 0x4360,    0xFF,       "9F ", "OPT1  "}, // DIP Switch 10E/F
                                                      {NO_BANK_SWITCH, c_ADC_A,   0xFF,       "9K ", "ADC   "}, // Yoke analog inputs
                                                      {NO_BANK_SWITCH, c_MBRUN_A, c_MBRUN_D,  "9H ", "MB RUN"}, // MATHRUN
                                                      {NO_BANK_SWITCH, c_REH_A,   0xFF,       "4J ", "REH   "}, // REH Hi - Quotient Hi
                                                      {NO_BANK_SWITCH, c_REL_A,   0xFF,       "4K ", "REL   "}, // REH Lo - Quotient Lo
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                "012", "012345"
                                                        {NO_BANK_SWITCH, c_ADCSTART0_A, 0xFF,      0x00,  "9K ", "ADCS0 "}, // ADC start channel 0 (pitch,  J)
                                                        {NO_BANK_SWITCH, c_ADCSTART1_A, 0xFF,      0x00,  "9K ", "ADCS1 "}, // ADC start channel 0 (yaw,    K)
                                                        {NO_BANK_SWITCH, c_ADCSTART2_A, 0xFF,      0x00,  "9K ", "ADCS2 "}, // ADC start channel 0 (thrust, 9)
                                                        {NO_BANK_SWITCH, c_MPAGE_A,     c_MPAGE_D, 0x00,  "9LM", "MPAGE "}, // MPAGE ROM bank switch
                                                        {NO_BANK_SWITCH, c_MW0_A,       0xFF,      0x00,  "   ", "MW0-PA"}, // MW0 - MP Address MPA2-MPA9, run
                                                        {NO_BANK_SWITCH, c_MW1_A,       0x01,      0x00,  "3D ", "MW1-BI"}, // MW1 - MP Block Index BIC8
                                                        {NO_BANK_SWITCH, c_MW2_A,       0xFF,      0x00,  "   ", "MW2-BI"}, // MW2 - MP Block Index BIC0-BIC7
                                                        {NO_BANK_SWITCH, c_DVSRH_A,     0xFF,      0x00,  "45P", "DVSRH "}, // DVSRH - Divisor Hi, Q clear, load div.
                                                        {NO_BANK_SWITCH, c_DVSRL_A,     0xFF,      0x00,  "6PL", "DVSRL "}, // DVSRL - Divisor Lo, start
                                                        {NO_BANK_SWITCH, c_DVDDH_A,     0xFF,      0x00,  "4L ", "DVDDH "}, // DVDDH - Dividend Hi
                                                        {NO_BANK_SWITCH, c_DVDDL_A,     0xFF,      0x00,  "5L ", "DVDDL "}, // DVDDL - Dividend Lo
                                                        {0}
                                                      }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                               "0123456789"
                                                            {CStarWarsBaseGame::testADC,                     "Test ADC  "},
                                                            {CStarWarsBaseGame::test10,                      "MX Test 10"},
                                                            {CStarWarsBaseGame::test11,                      "MX Test 11"},
                                                            {CStarWarsBaseGame::test12,                      "MX Test 12"},
                                                            {CStarWarsBaseGame::test13,                      "MX Test 13"},
                                                            {CStarWarsBaseGame::test14,                      "MX Test 14"},
                                                            {CStarWarsBaseGame::test15,                      "MX Test 15"},
                                                            {CStarWarsBaseGame::test16,                      "MX Test 16"},
                                                            {CStarWarsBaseGame::test17,                      "MX Test 17"},
                                                            {CStarWarsBaseGame::test18,                      "MX Test 18"},
                                                            {CStarWarsBaseGame::test19,                      "MX Test 19"},
                                                            {CStarWarsBaseGame::test20,                      "MX Test 20"},
                                                            {CStarWarsBaseGame::test21,                      "DV Test 21"},
                                                            {CStarWarsBaseGame::test22,                      "DV Test 22"},
                                                            {CStarWarsBaseGame::test23,                      "DV Test 23"},
                                                            {CStarWarsBaseGame::test24,                      "DV Test 24"},
                                                            {CStarWarsBaseGame::test25,                      "DV Test 25"},
                                                            {CStarWarsBaseGame::testRepeatLastMatrixProgram, "Repeat MX "},
                                                            {CStarWarsBaseGame::testClockPulse,              "Clk Pulse "},
                                                            {CStarWarsBaseGame::testCapture,                 "Capture   "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CStarWarsBaseGame::CStarWarsBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegion, // It's all Byte wide
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction ),
    m_clockPulseCount(0),
    m_lastMatrixProgramAddress(0)
{
    m_cpu = new C6809ECpu(0);
    m_cpu->idle();

    // A timer is on the INT pin (vector game thus no VBALNK).
    m_interrupt = ICpu::IRQ0;

    // The interrupt uses an external ROM vector.
    m_interruptAutoVector = false;
}


CStarWarsBaseGame::~CStarWarsBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// The program memory is banked.
//
PERROR
CStarWarsBaseGame::onBankSwitchMPAGE0(
    void *cStarWarsBaseGame
)
{
    CStarWarsBaseGame *thisGame  = (CStarWarsBaseGame *) cStarWarsBaseGame;
    ICpu              *cpu       = thisGame->m_cpu;

    return cpu->memoryWrite(c_MPAGE_A, 0x00);
}


PERROR
CStarWarsBaseGame::onBankSwitchMPAGE1(
    void *cStarWarsBaseGame
)
{
    CStarWarsBaseGame *thisGame  = (CStarWarsBaseGame *) cStarWarsBaseGame;
    ICpu              *cpu       = thisGame->m_cpu;

    return cpu->memoryWrite(c_MPAGE_A, c_MPAGE_D);
}


PERROR
CStarWarsBaseGame::waitForMathRunLo(
)
{
    PERROR error = errorSuccess;
    UINT16 recData;

    // Poll the math run flag
    for (int x = 0; x < 128 ; x++)
    {
        CHECK_CPU_READ_EXIT(error, m_cpu, c_MBRUN_A, &recData);

        if ((recData & c_MBRUN_D) == 0x00)
        {
            break;
        }
    }

    // Math Run is still active
    if (!((recData & c_MBRUN_D) == 0x00))
    {
        error = errorCustom;
        error->code = ERROR_FAILED;
        error->description = "E: MATH RUN Hi";
    }

Exit:
    return error;
}


PERROR
CStarWarsBaseGame::testMatrix(
    void   *context,
    int    srcDataLength,
    UINT32 *srcDataAddress,
    UINT16 *srcData,
    int    expDataLength,
    UINT32 *expDataAddress,
    UINT16 *expData,
    UINT16 programAddress
)
{
    CStarWarsBaseGame *thisGame = (CStarWarsBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT16 recData;
    UINT16 recResult = 0;

    // Make sure the matrix processor is idle
    error = thisGame->waitForMathRunLo();
    if (FAILED(error))
    {
        goto Exit;
    }

    // Load the source data words
    for (int x = 0 ; x < srcDataLength ; x++)
    {
        CHECK_CPU_WRITE_EXIT(error, cpu, c_MBRAM_A + (srcDataAddress[x] << 1) | 0, (srcData[x] >> 8) & 0xFF);
        CHECK_CPU_WRITE_EXIT(error, cpu, c_MBRAM_A + (srcDataAddress[x] << 1) | 1, (srcData[x] >> 0) & 0xFF);
    }

    // Make sure the result words are set to something so we know something happened
    for (int x = 0 ; x < expDataLength ; x++)
    {
        CHECK_CPU_WRITE_EXIT(error, cpu, c_MBRAM_A + (expDataAddress[x] << 1) | 0, 0x12);
        CHECK_CPU_WRITE_EXIT(error, cpu, c_MBRAM_A + (expDataAddress[x] << 1) | 1, 0x34);
    }

    // Write the program address to start
    CHECK_CPU_WRITE_EXIT(error, cpu, c_MW0_A, (programAddress >> 2) & 0xFF);
    thisGame->m_lastMatrixProgramAddress = programAddress;
    thisGame->m_clockPulseCount = 0;

    // Wait for the matrix processor to become idle again.
    error = thisGame->waitForMathRunLo();
    if (FAILED(error))
    {
        goto Exit;
    }

    // Check all the result words
    for (int x = 0 ; x < expDataLength ; x++)
    {
        recResult = 0;

        CHECK_CPU_READ_EXIT(error, cpu, c_MBRAM_A + (expDataAddress[x] << 1) | 0, &recData);
        recResult |= (recData << 8);
        CHECK_CPU_READ_EXIT(error, cpu, c_MBRAM_A + (expDataAddress[x] << 1) | 1, &recData);
        recResult |= (recData << 0);

        CHECK_UINT16_VALUE_EXIT(error, "MX", recResult, expData[x]);
    }

Exit:
    return error;
}


PERROR
CStarWarsBaseGame::testDivider(
    void   *context,
    UINT16 dividend,
    UINT16 divisor,
    UINT16 quotient
)
{
    CStarWarsBaseGame *thisGame = (CStarWarsBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT16 recData;
    UINT16 recQuotient = 0;

    // Load dividend
    CHECK_CPU_WRITE_EXIT(error, cpu, c_DVDDH_A, (dividend >> 8) & 0xFF);
    CHECK_CPU_WRITE_EXIT(error, cpu, c_DVDDL_A, (dividend >> 0) & 0xFF);

    // Load divisor
    CHECK_CPU_WRITE_EXIT(error, cpu, c_DVSRH_A, (divisor >> 8) & 0xFF);
    CHECK_CPU_WRITE_EXIT(error, cpu, c_DVSRL_A, (divisor >> 0) & 0xFF);
    thisGame->m_clockPulseCount = 0;

    // Wait for a few clocks.
    // There is no indication to the CPU that the divide is actually complete,
    // therefore we'll just execute a dummy read to run the master clock.
    // There's a counter at 8R loaded with 0001 that counts up at 3MHz and pops
    // out a ripple signal, thus ~16 3MHz clocks maybe?
    // 16 CPU reads (at 1 x 1.5MHz cycle each) should be more than enough to
    // complete the operation.
    //
    for (int x = 0 ; x < 16 ; x++)
    {
        CHECK_CPU_READ_EXIT(error, cpu, 0xFFFF, &recData);
    }

    // Read quotient
    CHECK_CPU_READ_EXIT(error, cpu, c_REH_A, &recData);
    recQuotient |= (recData << 8);
    CHECK_CPU_READ_EXIT(error, cpu, c_REL_A, &recData);
    recQuotient |= (recData << 0);

    // Check the result is what we expect
    CHECK_UINT16_VALUE_EXIT(error, "DV", recQuotient, quotient);

Exit:
    return error;
}


PERROR
CStarWarsBaseGame::testADC(
    void   *context
)
{
    CStarWarsBaseGame *thisGame = (CStarWarsBaseGame *) context;
    C6809ECpu *cpu = (C6809ECpu *) thisGame->m_cpu;
    PERROR error = errorCustom;
    UINT16 data[4] = {0,0,0,0};

    for (UINT32 channel = 0 ; channel < 4 ; channel++)
    {
        // Start the conversion
        CHECK_CPU_WRITE_EXIT(error, cpu, (c_ADCSTART0_A + channel), 0);

        // Wait for a few clocks.
        // There is no indication to the CPU when the conversion is actually complete.
        // The datasheet states that the maximum conversion time is 116us @ 640KHz so
        // we'll convert 116uS into the equivalent number of 12MHz clock pulses:
        //  12MHz === 83.33ns
        //  116us/83.33ns = 1,392 clock pulses.
        //
        for (int x = 0 ; x < 1392 ; x++)
        {
            cpu->clockPulse();
        }

        CHECK_CPU_READ_EXIT(error, cpu, c_ADC_A, &data[channel]);
    }

    error->code = ERROR_SUCCESS;
    error->description = "OK: ";

    for (int byte = 0 ; byte < 4 ; byte++)
    {
        STRING_UINT8_HEX(error->description, data[byte]);
    }

Exit:

    return error;
}


PERROR
CStarWarsBaseGame::test10(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0F  };
    UINT16 srcData[]    = {0x5555};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               0, (UINT32*) NULL, (UINT16*) NULL,
                               0x15C);
}


PERROR
CStarWarsBaseGame::test11(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0F  };
    UINT16 srcData[]    = {0xAAAA};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               0, (UINT32*) NULL, (UINT16*) NULL,
                               0x160);
}


PERROR
CStarWarsBaseGame::test12(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0F  };
    UINT16 srcData[]    = {0x5555};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               0, (UINT32*) NULL, (UINT16*) NULL,
                               0x164);
}


PERROR
CStarWarsBaseGame::test13(
    void   *context
)
{
    return testMatrix(context, 0, (UINT32*) NULL, (UINT16*) NULL,
                               0, (UINT32*) NULL, (UINT16*) NULL,
                               0x168);
}


PERROR
CStarWarsBaseGame::test14(
    void   *context
)
{
    return testMatrix(context, 0, (UINT32*) NULL, (UINT16*) NULL,
                               0, (UINT32*) NULL, (UINT16*) NULL,
                               0x16C);
}


PERROR
CStarWarsBaseGame::test15(
    void   *context
)
{
    UINT32 srcAddress[] = {0x00  };
    UINT16 srcData[]    = {0x5555};
    UINT32 expAddress[] = {0x01  };
    UINT16 expData[]    = {0x5555};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               ARRAYSIZE(expAddress), expAddress, expData,
                               0x170); // -> 171
}


PERROR
CStarWarsBaseGame::test16(
    void   *context
)
{
    UINT32 srcAddress[] = {0x00  };
    UINT16 srcData[]    = {0xAAAA};
    UINT32 expAddress[] = {0x01  };
    UINT16 expData[]    = {0xAAAA};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               ARRAYSIZE(expAddress), expAddress, expData,
                               0x170); // -> 171
}


PERROR
CStarWarsBaseGame::test17(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0C,   0x0D,   0x0E  };
    UINT16 srcData[]    = {0x5555, 0x0000, 0x4000};
    UINT32 expAddress[] = {0x00  };
    UINT16 expData[]    = {0x5555};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               ARRAYSIZE(expAddress), expAddress, expData,
                               0x174); // -> 177
}


PERROR
CStarWarsBaseGame::test18(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0C,   0x0D,   0x0E  };
    UINT16 srcData[]    = {0x0000, 0x5555, 0xC000};
    UINT32 expAddress[] = {0x00  };
    UINT16 expData[]    = {0x5555};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               ARRAYSIZE(expAddress), expAddress, expData,
                               0x174); // -> 177
}


PERROR
CStarWarsBaseGame::test19(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0C,   0x0D,   0x0E  };
    UINT16 srcData[]    = {0x2AAA, 0x0000, 0x4000};
    UINT32 expAddress[] = {0x00  };
    UINT16 expData[]    = {0x2AAA};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               ARRAYSIZE(expAddress), expAddress, expData,
                               0x174); // -> 177
}


PERROR
CStarWarsBaseGame::test20(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0C,   0x0D,   0x0E  };
    UINT16 srcData[]    = {0x0000, 0x2AAA, 0xC000};
    UINT32 expAddress[] = {0x00  };
    UINT16 expData[]    = {0x2AAA};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               ARRAYSIZE(expAddress), expAddress, expData,
                               0x174); // -> 177
}


PERROR
CStarWarsBaseGame::test21(
    void   *context
)
{
    return testDivider(context, 0x4000, 0x4000, 0x4000);
}


PERROR
CStarWarsBaseGame::test22(
    void   *context
)
{
    return testDivider(context, 0x5555, 0x4000, 0x5555);
}


PERROR
CStarWarsBaseGame::test23(
    void   *context
)
{
    return testDivider(context, 0x2AAA, 0x4000, 0x2AAA);
}


PERROR
CStarWarsBaseGame::test24(
    void   *context
)
{
    return testDivider(context, 0x2AAA, 0x2AAA, 0x4000);
}


PERROR
CStarWarsBaseGame::test25(
    void   *context
)
{
    return testDivider(context, 0x5555, 0x5555, 0x4000);
}


PERROR
CStarWarsBaseGame::testRepeatLastMatrixProgram(
    void   *context
)
{
    CStarWarsBaseGame *thisGame = (CStarWarsBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;

    if (thisGame->m_lastMatrixProgramAddress == 0)
    {
        error = errorUnexpected;
        goto Exit;
    }

    // Write the program address to start
    CHECK_CPU_WRITE_EXIT(error, cpu, c_MW0_A, (thisGame->m_lastMatrixProgramAddress >> 2) & 0xFF);
    thisGame->m_clockPulseCount = 0;

Exit:
    return error;
}


PERROR
CStarWarsBaseGame::testClockPulse(
    void   *context
)
{
    CStarWarsBaseGame *thisGame = (CStarWarsBaseGame *) context;
    C6809ECpu *cpu = (C6809ECpu *) thisGame->m_cpu;
    PERROR error = errorCustom;

    cpu->clockPulse();
    thisGame->m_clockPulseCount++;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "OK: Count ";
    errorCustom->description += String(thisGame->m_clockPulseCount, DEC);

    return error;
}

//
// Prototype signal capture
//
#include "PinMap.h"

//
// External capture input on J14 AUX pin 1.
//
static const CONNECTION s_AUX1_i = {1, "AUX1"};

//
// Prototype Signal Capture
// ------------------------
// Capture the clocked input of aux pin 1 and display the hex result.
// The 4 hex digits covers 32 clocks.
// See StarWarsCaptures.txt for reference results.
//

PERROR
CStarWarsBaseGame::testCapture(
    void   *context
)
{
    CStarWarsBaseGame *thisGame = (CStarWarsBaseGame *) context;
    C6809ECpu *cpu = (C6809ECpu *) thisGame->m_cpu;
    PERROR error = errorCustom;
    UINT8 capture[4] = {0,0,0,0};

    ::pinMode(g_pinMap8Aux[s_AUX1_i.pin], INPUT);

    for (int byte = 0 ; byte < 4 ; byte++)
    {
        for (int bit = 0 ; bit < 8 ; bit++)
        {
            cpu->clockPulse();
            thisGame->m_clockPulseCount++;

            int value = ::digitalRead(g_pinMap8Aux[s_AUX1_i.pin]);

            capture[byte] = capture[byte] << 1;
            if (value == HIGH)
            {
                capture[byte] |= 1;
            }
        }
    }

    error->code = ERROR_SUCCESS;
    error->description = "OK: ";

    for (int byte = 0 ; byte < 4 ; byte++)
    {
        STRING_UINT8_HEX(error->description, capture[byte]);
    }

    return error;
}


