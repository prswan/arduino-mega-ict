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
#include <DFR_Key.h>

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
// * The CPU bord can be worked on alone - the AVG & interconnect PCB's are not required.
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


//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                               "012", "012345"
                                                  {NO_BANK_SWITCH, 0x4800,      0x4FFF,      0xFF, "2FH", "Prog. "}, // "Program RAM, 6116, CPU"
                                                  {NO_BANK_SWITCH, 0x5000,      0x5FFF,      0xFF, "5FH", "MB AB "}, // "Mathbox RAM 2x6116, 16-bit, CPU"
                                                  {0}
                                                }; // end of list

//
// Input region is the same for all versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                               "012", "012345"
                                                      {NO_BANK_SWITCH, 0x4320L, 0x80,  "9H ", "MB RUN"}, // MATHRUN
                                                      {NO_BANK_SWITCH, c_REH_A, 0xFF,  "4J ", "REH   "}, // REH Hi - Quotient Hi
                                                      {NO_BANK_SWITCH, c_REL_A, 0xFF,  "4K ", "REL   "}, // REH Lo - Quotient Lo
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                            "012", "012345"
                                                        {NO_BANK_SWITCH, c_MPAGE_A, c_MPAGE_D, 0x00,  "9LM", "MPAGE "},  // MPAGE ROM bank switch
                                                        {NO_BANK_SWITCH, 0x4700L,   0xFF,      0x00,  "   ", "MW0-PA"}, // MW0 - MP Address MPA2-MPA9, run
                                                        {NO_BANK_SWITCH, 0x4701L,   0x01,      0x00,  "3D ", "MW1-BI"}, // MW1 - MP Block Index BIC8
                                                        {NO_BANK_SWITCH, 0x4702L,   0xFF,      0x00,  "   ", "MW2-BI"}, // MW2 - MP Block Index BIC0-BIC7
                                                        {NO_BANK_SWITCH, c_DVSRH_A, 0xFF,      0x00,  "45P", "DVSRH "}, // DVSRH - Divisor Hi, Q clear, load div.
                                                        {NO_BANK_SWITCH, c_DVSRL_A, 0xFF,      0x00,  "6PL", "DVSRL "}, // DVSRL - Divisor Lo, start
                                                        {NO_BANK_SWITCH, c_DVDDH_A, 0xFF,      0x00,  "4L ", "DVDDH "}, // DVDDH - Dividend Hi
                                                        {NO_BANK_SWITCH, c_DVDDL_A, 0xFF,      0x00,  "5L ", "DVDDL "}, // DVDDL - Dividend Lo
                                                        {0}
                                                      }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                    "0123456789"
                                                            {CStarWarsBaseGame::test21,           "DV Test 21"},
                                                            {CStarWarsBaseGame::test22,           "DV Test 22"},
                                                            {CStarWarsBaseGame::test23,           "DV Test 23"},
                                                            {CStarWarsBaseGame::test24,           "DV Test 24"},
                                                            {CStarWarsBaseGame::test25,           "DV Test 25"},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CStarWarsBaseGame::CStarWarsBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction )
{
    m_cpu = new C6809ECpu();
    m_cpu->idle();

    // A timer is on the INT pin (vector game thus no VBALNK).
    m_interrupt = ICpu::INT;

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
    UINT8  recData;
    UINT16 recQuotient = 0;

    // Load dividend
    CHECK_CPU_WRITE_EXIT(error, cpu, c_DVDDH_A, (dividend >> 8) & 0xFF);
    CHECK_CPU_WRITE_EXIT(error, cpu, c_DVDDL_A, (dividend >> 0) & 0xFF);

    // Load divisor
    CHECK_CPU_WRITE_EXIT(error, cpu, c_DVSRH_A, (divisor >> 8) & 0xFF);
    CHECK_CPU_WRITE_EXIT(error, cpu, c_DVSRL_A, (divisor >> 0) & 0xFF);

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
    recQuotient |= ((UINT16) recData) << 8;
    CHECK_CPU_READ_EXIT(error, cpu, c_REL_A, &recData);
    recQuotient |= ((UINT16) recData) << 0;

    // Check the result is what we expect
    CHECK_UINT16_VALUE_EXIT(error, "DV", recQuotient, quotient);

Exit:
    return error;
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

