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
#include "CStarWarsAvgBaseGame.h"
#include "C6809EClockMasterCpu.h"

//
// Notes
// -----
//
// * The Clock output from the ICT should be connected to Main PCB IC 1N (S04) pin 1
//   that's been disconnected from the 12MHz clock source (e.g. socket 1N and insert
//   the IC with pin 1 bent out and connect the ICT clock to the bent out pin).
//

//
// Programmatically used definitions
//

// Vector control addresses
static const UINT32 c_AVG_GO_A    = 0x4600;
static const UINT32 c_AVG_RST_A   = 0x4620;

static const UINT32 c_AVG_HALT_A  = 0x4320;
static const UINT32 c_AVG_HALT_D  = 0x40;

static const UINT32 c_VRAM_BASE   = 0x0000;
static const UINT32 c_VRAM_LEN    = 0x3000;
//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                  "012", "012345"
                                                  {NO_BANK_SWITCH, 0x0000,      0x07FF,      1, 0xFF, "3L ", "VRAM0 "}, // "Vectror RAM, 6116"
                                                  {NO_BANK_SWITCH, 0x0800,      0x0FFF,      1, 0xFF, "3M ", "VRAM1 "}, // "Vectror RAM, 6116"
                                                  {NO_BANK_SWITCH, 0x1000,      0x17FF,      1, 0xFF, "3P ", "VRAM2 "}, // "Vectror RAM, 6116"
                                                  {NO_BANK_SWITCH, 0x1800,      0x1FFF,      1, 0xFF, "4L ", "VRAM3 "}, // "Vectror RAM, 6116"
                                                  {NO_BANK_SWITCH, 0x2000,      0x27FF,      1, 0xFF, "4M ", "VRAM4 "}, // "Vectror RAM, 6116"
                                                  {NO_BANK_SWITCH, 0x2800,      0x2FFF,      1, 0xFF, "4P ", "VRAM5 "}, // "Vectror RAM, 6116"
                                                  {0}
                                                }; // end of list

//
// Input region is the same for all versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                           "012", "012345"
                                                      {NO_BANK_SWITCH, c_AVG_HALT_A, c_AVG_HALT_D, "1P ", "IN1   "}, // Input port 1, Halt/Run
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                "012", "012345"
                                                        {NO_BANK_SWITCH, c_AVG_GO_A,    0xFF,      0x00,  "1P ", "VG Go "}, // Vector Go
                                                        {NO_BANK_SWITCH, c_AVG_RST_A,   0xFF,      0x00,  "1P ", "VG Rst"}, // Vector Reset
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
                                                            {CStarWarsAvgBaseGame::vgRst,                    "VG Rst    "},
                                                            {CStarWarsAvgBaseGame::loadHALT,                 "Load HALT "},
                                                            {CStarWarsAvgBaseGame::loadVCTR,                 "Load VCTR "},
                                                            {CStarWarsAvgBaseGame::loadSTAT,                 "Load STAT "},
                                                            {CStarWarsAvgBaseGame::loadSCAL,                 "Load SCAL "},
                                                            {CStarWarsAvgBaseGame::loadCNTR,                 "Load CNTR "},
                                                            {CStarWarsAvgBaseGame::loadAll,                  "Load All  "},
                                                            {CStarWarsAvgBaseGame::vgGo,                     "VG Go     "},
                                                            {CStarWarsAvgBaseGame::capture32,                "Capture 32"},
                                                            {CStarWarsAvgBaseGame::rstGoCap32,               "RstGoCap32"},
                                                            {CStarWarsAvgBaseGame::runToHalt,                "Run-Halt  "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CStarWarsAvgBaseGame::CStarWarsAvgBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegion, // It's all Byte wide
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction )
{
    m_cpu = new C6809EClockMasterCpu();
    m_cpu->idle();

    // A timer is on the INT pin (vector game thus no VBALNK).
    m_interrupt = ICpu::IRQ0;

    // The interrupt uses an external ROM vector.
    m_interruptAutoVector = false;

    // Signal capture extension
    m_capture = new CCapture(m_cpu);

}


CStarWarsAvgBaseGame::~CStarWarsAvgBaseGame(
)
{
    delete m_capture;
    m_capture = (CCapture *) NULL;

    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// Load a pattern into the vector RAM terminated by HALT
//
PERROR
CStarWarsAvgBaseGame::load(
    void   *context,
    UINT8   dataLen,
    UINT16 *data
)
{
    CStarWarsAvgBaseGame *thisGame = (CStarWarsAvgBaseGame *) context;
    C6809EClockMasterCpu *cpu = (C6809EClockMasterCpu *) thisGame->m_cpu;
    PERROR error = errorSuccess;

    // Load the vector RAM with test pattern
    for (UINT32 address = c_VRAM_BASE ; address < (c_VRAM_BASE + c_VRAM_LEN) ; address++)
    {
        CHECK_CPU_WRITE_EXIT(error, cpu, address, data[address % dataLen]);
    }

    // Set the last two bytes to be HALT so we can use runToHalt
    {
        CHECK_CPU_WRITE_EXIT(error, cpu, ((c_VRAM_BASE + c_VRAM_LEN) - 2), 0x20);
        CHECK_CPU_WRITE_EXIT(error, cpu, ((c_VRAM_BASE + c_VRAM_LEN) - 1), 0x20);
    }

Exit:

    return error;
}


//
// Pattern test for the 2-byte HALT vector instruction.
//
// "6809 style"
//
//   0x00     0x01
// 76543210 76543210
//
// 001----- --------
//
//
PERROR
CStarWarsAvgBaseGame::loadHALT(
    void   *context
)
{
    static const UINT8 dataLen = 4;
    static const UINT16 data[dataLen] = {0x35,0xAA, 0x2A,0x55};

    return CStarWarsAvgBaseGame::load(context, dataLen, data);
}


//
// Pattern test for the 4-byte VCTR vector instruction.
//
// "6809 style"
//
//   0x00     0x01     0x02     0x03
// 76543210 76543210 76543210 76543210
//
// 000YYYYY YYYYYYYY ZZZXXXXX XXXXXXXX
//
//
PERROR
CStarWarsAvgBaseGame::loadVCTR(
    void   *context
)
{
    static const UINT8 dataLen = 8;
    static const UINT16 data[dataLen] = {0x15,0xAA,0x55,0xAA, 0x0A,0x55,0xAA,0x55};

    return CStarWarsAvgBaseGame::load(context, dataLen, data);
}


//
// Pattern test for the 2-byte STAT colour instruction.
//
// "6809 style"
//
//   0x00     0x01
// 76543210 76543210
//
// 0110xRGB ZZZZZZZZ
//
//
PERROR
CStarWarsAvgBaseGame::loadSTAT(
    void   *context
)
{
    static const UINT8 dataLen = 4;
    static const UINT16 data[dataLen] = {0x65,0xAA,0x6A,0x55};

    return CStarWarsAvgBaseGame::load(context, dataLen, data);
}


//
// Pattern test for the 2-byte SCAL Binary & Linear scale instruction.
//
// "6809 style"
//
//   0x00     0x01
// 76543210 76543210
//
// 0111xBBB LLLLLLLL
//
//
PERROR
CStarWarsAvgBaseGame::loadSCAL(
    void   *context
)
{
    static const UINT8 dataLen = 4;
    static const UINT16 data[dataLen] = {0x75,0xAA, 0x7A,0x55};

    return CStarWarsAvgBaseGame::load(context, dataLen, data);
}


//
// Pattern test for the 2-byte CNTR beam centre instruction.
//
// "6809 style"
//
//   0x00     0x01
// 76543210 76543210
//
// 10000000 01xxxxxx
//
//
PERROR
CStarWarsAvgBaseGame::loadCNTR(
    void   *context
)
{
    static const UINT8 dataLen = 2;
    static const UINT16 data[dataLen] = {0x80,0x40};

    return CStarWarsAvgBaseGame::load(context, dataLen, data);
}


//
// Pattern test for all the display instructions
// Values taken from individual instruction tests above.
//
PERROR
CStarWarsAvgBaseGame::loadAll(
    void   *context
)
{
    static const UINT8 dataLen = 20;
    static const UINT16 data[dataLen] = {
                                         0x15,0xAA,0x55,0xAA,   // VCTR 0
                                         0x65,0xAA,             // STAT 0
                                         0x75,0xAA,             // SCAL 0
                                         0x80,0x40,             // CNTR
                                         0x0A,0x55,0xAA,0x55,   // VCTR 1
                                         0x6A,0x55,             // STAT 1
                                         0x7A,0x55,             // SCAL 1
                                         0x80,0x40              // CNTR
                                        };

    return CStarWarsAvgBaseGame::load(context, dataLen, data);
}


// Reset the vector generator to idle
PERROR
CStarWarsAvgBaseGame::vgRst(
    void   *context
)
{
    CStarWarsAvgBaseGame *thisGame = (CStarWarsAvgBaseGame *) context;
    C6809EClockMasterCpu *cpu = (C6809EClockMasterCpu *) thisGame->m_cpu;
    PERROR error = errorSuccess;

    // Reset the vector generator
    CHECK_CPU_WRITE_EXIT(error, cpu, c_AVG_RST_A, 0);

Exit:

    return error;
}


// Run the vector generator
PERROR
CStarWarsAvgBaseGame::vgGo(
    void   *context
)
{
    CStarWarsAvgBaseGame *thisGame = (CStarWarsAvgBaseGame *) context;
    C6809EClockMasterCpu *cpu = (C6809EClockMasterCpu *) thisGame->m_cpu;
    PERROR error = errorSuccess;

    // Poke the vector generator to run
    CHECK_CPU_WRITE_EXIT(error, cpu, c_AVG_GO_A, 0);

Exit:

    return error;
}


// Run 32 clocks and signal capture
PERROR
CStarWarsAvgBaseGame::capture32(
    void   *context
)
{
    CStarWarsAvgBaseGame *thisGame = (CStarWarsAvgBaseGame *) context;

    return thisGame->m_capture->capture32();
}


// VG Rst then VG Go then Capture 32
PERROR
CStarWarsAvgBaseGame::rstGoCap32(
    void   *context
)
{
    CStarWarsAvgBaseGame *thisGame = (CStarWarsAvgBaseGame *) context;
    C6809EClockMasterCpu *cpu = (C6809EClockMasterCpu *) thisGame->m_cpu;
    PERROR error = errorSuccess;

    // Reset the vector generator
    CHECK_CPU_WRITE_EXIT(error, cpu, c_AVG_RST_A, 0);

    // Poke the vector generator to run
    CHECK_CPU_WRITE_EXIT(error, cpu, c_AVG_GO_A, 0);

    error = thisGame->m_capture->capture32();

Exit:

    return error;
}


// Run the vector generator until it halts
PERROR
CStarWarsAvgBaseGame::runToHalt(
    void   *context
)
{
    CStarWarsAvgBaseGame *thisGame = (CStarWarsAvgBaseGame *) context;
    C6809EClockMasterCpu *cpu = (C6809EClockMasterCpu *) thisGame->m_cpu;
    UINT16 recData;
    UINT32 cycleCount = 0;
    PERROR error = errorSuccess;

    // The CPU reads for HALT status will run the system clock
    do
    {
        CHECK_CPU_READ_EXIT(error, cpu, c_AVG_HALT_A, &recData);
        cycleCount++;
    }
    while (!(recData & c_AVG_HALT_D));

    // Print out how long it took
    error = errorCustom;
    error->code = ERROR_SUCCESS;
    error->description = "OK: ";
    STRING_UINT32_HEX(error->description, cycleCount);

Exit:

    return error;
}

