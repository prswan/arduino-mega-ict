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
#include "CWmsSoundBaseGame.h"
#include "C6802Cpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   6802 GND Pin 1 and Pin 21
//
// Watchdog Disable:
//   This board does not have a watchdog circuit.
//
//
// 6802 Compatibility Notes
//   * When RE is enabled by W14 the 6802's internal RAM is enabled.  This cannot be used with a 6808.

//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                     "012", "012345"
                                                  {NO_BANK_SWITCH, 0x0000, 0x007F, 0xFF, "U11", "RAM   "}, // 6810 RAM
                                                  {0}
                                                }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} };

//
// Input region is the same for all versions - IC10 6821 PIA Inputs from ROM board
//
// Sound board Inputs
// 4J3-1 - Key                                              PB0 - 4J3-3
// 4J3-2 - PB1                                              PB1 - 4J3-2
// 4J3-3 - PB0                                              PB2 - 4J3-5
// 4J3-4 - PB3                                              PB3 - 4J3-4
// 4J3-5 - PB2                                              PB4 - 4J3-7
// 4J3-6 - PB5 (W4 Installed & W9 Removed)                  PB5 - 4J3-6 (W4) / DSW-1 (W9)
// 4J3-7 - PB4                                              PB6 - DSW-1
// 4J3-8 - PB7 (W12 Installed)                              PB7 - 4J3-8 (W12)
// 4J3-9 - No Connection
// 
// DSW-1 - PB6
// DSW-2 - PB5 (W4 Removed & W9 Installed)

static const INPUT_REGION s_inputRegion[] PROGMEM = { //                             "012", "012345"
                                                      {NO_BANK_SWITCH, 0x0402, 0xFF, "U10", "PB0-7 "},
                                                      // {NO_BANK_SWITCH, 0x0402, 0x01, "U10", "PB0   "},
                                                      // {NO_BANK_SWITCH, 0x0402, 0x02, "U10", "PB1   "},
                                                      // {NO_BANK_SWITCH, 0x0402, 0x04, "U10", "PB2   "},
                                                      // {NO_BANK_SWITCH, 0x0402, 0x08, "U10", "PB3   "},
                                                      // {NO_BANK_SWITCH, 0x0402, 0x10, "U10", "PB4   "},
                                                      // {NO_BANK_SWITCH, 0x0402, 0x20, "U10", "PB5   "},
                                                      // {NO_BANK_SWITCH, 0x0402, 0x40, "U10", "PB6   "},
                                                      // {NO_BANK_SWITCH, 0x0402, 0x80, "U10", "PB7   "},
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions - IC10 6821 PIA Outputs to DAC
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x0400, 0xFF, 0x00,  "U10", "CA0-7 "},
                                                        // {NO_BANK_SWITCH, 0x0400, 0x01, 0x00,  "U10", "CA0   "},
                                                        // {NO_BANK_SWITCH, 0x0400, 0x02, 0x00,  "U10", "CA1   "},
                                                        // {NO_BANK_SWITCH, 0x0400, 0x04, 0x00,  "U10", "CA2   "},
                                                        // {NO_BANK_SWITCH, 0x0400, 0x08, 0x00,  "U10", "CA3   "},
                                                        // {NO_BANK_SWITCH, 0x0400, 0x10, 0x00,  "U10", "CA4   "},
                                                        // {NO_BANK_SWITCH, 0x0400, 0x20, 0x00,  "U10", "CA5   "},
                                                        // {NO_BANK_SWITCH, 0x0400, 0x40, 0x00,  "U10", "CA6   "},
                                                        // {NO_BANK_SWITCH, 0x0400, 0x80, 0x00,  "U10", "CA7   "},
                                                        {NO_BANK_SWITCH, 0x0401, 0x38, 0x00,  "U10", "CA2   "},
                                                        {NO_BANK_SWITCH, 0x0403, 0x38, 0x00,  "U10", "CB2   "},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { {NO_CUSTOM_FUNCTION} };



CWmsSoundBaseGame::CWmsSoundBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new C6802Cpu();
    m_cpu->idle();

    // The sound test interrupt is on the NMI pin.
    // The sound request interrupt is on the /IRQ pin.
    m_interrupt = ICpu::NMI;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;

}


CWmsSoundBaseGame::~CWmsSoundBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


