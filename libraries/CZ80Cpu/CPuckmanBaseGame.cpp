//
// Copyright (c) 2019, Paul R. Swan
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
#include "CPuckmanBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   Tie CPU board IC 5C (74LS00) pin 13 to GND.
//
// Board Designations:
//   This is the CPU/main board base.
//
// Z80 Compatibility Notes
//
//


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x004000, 0x0043FF, 1, 0x0F, " 4H", "VidRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x004000, 0x0043FF, 1, 0xF0, " 4L", "VidRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x004400, 0x0047FF, 1, 0x0F, " 4J", "ClrRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x004400, 0x0047FF, 1, 0xF0, " 4M", "ClrRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x004C00, 0x004FFF, 1, 0x0F, " 4K", "SprRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x004C00, 0x004FFF, 1, 0xF0, " 4N", "SprRam"}, // 2114
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                                                      "012", "012345"
                                                          {NO_BANK_SWITCH, 0x004000, 0x0043FF, 1, 0xFF, "4HL", "VidRam"}, // 2114
                                                          {NO_BANK_SWITCH, 0x004400, 0x0047FF, 1, 0xFF, "4JM", "ClrRam"}, // 2114
                                                          {NO_BANK_SWITCH, 0x004C00, 0x004FFF, 1, 0xFF, "4KN", "SprRam"}, // 2114
                                                          {0}
                                                        }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                                                                    "012", "012345"
                                                           {NO_BANK_SWITCH, 0x005040, 0x00505F, 1, 0xFF, " 2JH", "Sound"},
                                                           {NO_BANK_SWITCH, 0x005060, 0x00506F, 1, 0xFF, " 2JH", "SprXY"},
                                                           {NO_BANK_SWITCH, 0x005070, 0x00507F, 1, 0xFF, " 2JH", "Ram  "},
                                                           {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                             "012", "012345"
                                                      {NO_BANK_SWITCH, 0x5000, 0xFF, "9ML", "IN0   "},
                                                      {NO_BANK_SWITCH, 0x5040, 0xFF, "9HM", "IN1   "},
                                                      {NO_BANK_SWITCH, 0x5080, 0xFF, "9FL", "DSW1  "},
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                   "012", "012345"
                                                        {NO_BANK_SWITCH, 0x05000, 0x01, 0x00, " 9N", "IRQ En"}, //
                                                        {NO_BANK_SWITCH, 0x05003, 0x01, 0x00, " 9N", "ScFlip"}, //
                                                        {NO_BANK_SWITCH, 0x050c0, 0x01, 0x00, " 9N", "WdRes "}, //
                                                        {NO_BANK_SWITCH, 0x10000, 0x55, 0x00, " ??", "Int-55"}, //
                                                        {NO_BANK_SWITCH, 0x10000, 0x55, 0x00, " ??", "Int-AA"}, //
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CPuckmanBaseGame::CPuckmanBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction )
{
    m_cpu = new CZ80ACpu(0,
                         NO_ADDRESS_REMAP,
                         NULL,
                         NO_DATA_REMAP,
                         NULL,
                         CZ80ACpu::CYCLE_TYPE_PUCKMAN);

    m_cpu->idle();

    // The VBLANK interrupt is on the IRQ pin.
    m_interrupt = ICpu::IRQ0;

    // MAME states the vector is set by an OUT at IO address 0
    m_interruptAutoVector = false;
}


CPuckmanBaseGame::~CPuckmanBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// The original game PCB & ROM set uses Interrupt Mode 2 with a programmable vector
// latch (in the Sync Bus Controller) that's set by output port 0.
// The bootleg game PCB & ROM set uses Interrupt Mode 1 with no vector latch hardware
// that thus means the original ROM test won't run on the bootleg game PCB.
//
PERROR
CPuckmanBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}

