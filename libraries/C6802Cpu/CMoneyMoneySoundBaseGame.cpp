//
// Copyright (c) 2024, Paul R. Swan
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
#include "CMoneyMoneySoundBaseGame.h"
#include "C6802Cpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   6802 GND Pins 1 & 21
//
// Watchdog Disable:
//   Sound CPU has none.
//
// This is the primary sound CPU that:
// 1) Handles the main CPU sound commands
// 2) Sends music commands to the music CPU
// 3) Generates game sounds using the DAC08
// 4) Plays speech via the TI speech IC attached to the 6821.
//


//
// RAM region is internal to the 6802 (no external RAM)
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {0}
                                                }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                "012", "012345"
                                                      {NO_BANK_SWITCH,  0x0090L, 0xFF,  " 1I", "6821 0"}, // 6821
                                                      {NO_BANK_SWITCH,  0x1800L, 0xFF,  " 2E", "SndCmd"}, // Host sound command
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x0090L, 0xFF, 0x00,  " 1I", "6821 0"}, // 6821
                                                        {NO_BANK_SWITCH, 0x1000L, 0xFF, 0x00,  " 1F", "DAC08 "}, // 1408 DAC
                                                        {NO_BANK_SWITCH, 0x1400L, 0xFF, 0x00,  " 2G", "Mus-FF"}, // Music command
                                                        {NO_BANK_SWITCH, 0x1400L, 0x55, 0x00,  " 2G", "Mus-55"}, // Music command
                                                        {NO_BANK_SWITCH, 0x1400L, 0xAA, 0x00,  " 2G", "Mus-AA"}, // Music command
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CMoneyMoneySoundBaseGame::CMoneyMoneySoundBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegion,
           s_ramRegion,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new C6802Cpu();
    m_cpu->idle();

    // The interrupt is on the IRQ pin.
    m_interrupt = ICpu::IRQ0;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;
}


CMoneyMoneySoundBaseGame::~CMoneyMoneySoundBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


PERROR
CMoneyMoneySoundBaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}

