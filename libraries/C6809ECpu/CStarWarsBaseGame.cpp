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
static const UINT32 c_MPAGE_A  = 0x4684;
static const UINT8  c_MPAGE_D  = 0x80;


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
                                                      {NO_BANK_SWITCH, 0x4700L, 0xFF,  "4J ", "REH   "}, // REH
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                            "012", "012345"
                                                        {NO_BANK_SWITCH, c_MPAGE_A, c_MPAGE_D, 0x00,  "9LM", "MPAGE"},  // MPAGE ROM bank switch
                                                        {NO_BANK_SWITCH, 0x4700L,   0xFF,      0x00,  "   ", "MW0-PA"}, // MW0 - MP Address MPA2-MPA9, run
                                                        {NO_BANK_SWITCH, 0x4701L,   0x01,      0x00,  "3D ", "MW1-BI"}, // MW1 - MP Block Index BIC8
                                                        {NO_BANK_SWITCH, 0x4702L,   0xFF,      0x00,  "   ", "MW2-BI"}, // MW2 - MP Block Index BIC0-BIC7
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


