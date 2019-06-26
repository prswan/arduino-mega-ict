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
                                                        {NO_BANK_SWITCH, c_AVG_GO_A,    0xFF,      0x00,  "1P ", "VG Go  "}, // Vector Go
                                                        {NO_BANK_SWITCH, c_AVG_RST_A,   0xFF,      0x00,  "1P ", "VG Rst "}, // Vector Reset
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
}


CStarWarsAvgBaseGame::~CStarWarsAvgBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


