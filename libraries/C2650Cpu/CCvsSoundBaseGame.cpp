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
#include "CCvsSoundBaseGame.h"
#include "C2650Cpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   2650 GND Pin 21
//
// Watchdog Disable:
//   I didn't see a watchdog implemented, RESET wasn't pulsing.
//

//
// RAM region is the same for all versions.
// Taken from MAME Cvs.cpp
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                         "012", "012345"
                                                  {NO_BANK_SWITCH,  0x1000, 0x107F, 1, 0xFF, "  ?", "Prog. "}, // RAM, 6810, 128b
                                                  {0}
                                                }; // end of list


//
// No write only memory.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //  "012", "012345"
                                                           {0}
                                                         }; // end of list

//
// Input region is the same for all versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                              "012", "012345"
                                                      {NO_BANK_SWITCH, 0x1800L, 0xFF, "  ?", "SndCmd"}, // Sound command
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x1840L, 0x55, 0x00, "  ?", "Dac8-5"}, // Byte wide DAC & tone frequency
                                                        {NO_BANK_SWITCH, 0x1840L, 0xAA, 0x00, "  ?", "Dac8-A"}, // Byte wide DAC
                                                        {NO_BANK_SWITCH, 0x1840L, 0xFF, 0x00, "  ?", "Dac8-F"}, // Byte wide DAC
                                                        {NO_BANK_SWITCH, 0x1880L, 0x80, 0x00, "  ?", "Dac4-0"}, // 4-Bit DAC bit 0
                                                        {NO_BANK_SWITCH, 0x1881L, 0x80, 0x00, "  ?", "Dac4-1"}, // 4-Bit DAC bit 1
                                                        {NO_BANK_SWITCH, 0x1882L, 0x80, 0x00, "  ?", "Dac4-2"}, // 4-Bit DAC bit 2
                                                        {NO_BANK_SWITCH, 0x1883L, 0x80, 0x00, "  ?", "Dac4-3"}, // 4-Bit DAC bit 3
                                                        {NO_BANK_SWITCH, 0x1884L, 0x01, 0x00, "  ?", "ToneOn"}, // Tone on-off (freq set by DAC8 value)
                                                        {NO_BANK_SWITCH, 0x1885L, 0x01, 0x00, "  ?", "ToneAm"}, // Tone amplitude hi-lo
                                                        {NO_BANK_SWITCH, 0x1886L, 0x01, 0x00, "  ?", "Fire  "}, // Galaxia-like ship fire sound
                                                        {NO_BANK_SWITCH, 0x1887L, 0x01, 0x00, "  ?", "ToneEv"}, // Tone envelope attack-decay
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CCvsSoundBaseGame::CCvsSoundBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction )
{
    m_cpu = new C2650Cpu();
    m_cpu->idle();

    // There is only a single interrupt pin on the 2650.
    m_interrupt = ICpu::IRQ0;

    // The interrupt uses an external hardware vector.
    m_interruptAutoVector = false;

    // The vector is hard coded.
    m_interruptResponse = 0x03;

}


CCvsSoundBaseGame::~CCvsSoundBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}

