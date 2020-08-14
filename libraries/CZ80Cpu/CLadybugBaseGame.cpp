//
// Copyright (c) 2020, Marc Deslauriers
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
#include "CLadybugBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   No watchdog
//
// Board Designations:
//   This is the CPU/main board base.
//
// Z80 Compatibility Notes
//   There is no VBLANK interrupt, it is read from IN1
//   Left coin generates INT, right coin generates IRQ


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x006000, 0x0063FF, 1, 0x0F, " F3", "CpuRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x006000, 0x0063FF, 1, 0xF0, " H3", "CpuRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x006400, 0x0067FF, 1, 0x0F, " J3", "CpuRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x006400, 0x0067FF, 1, 0xF0, " K3", "CpuRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x006800, 0x006BFF, 1, 0x0F, " L3", "CpuRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x006800, 0x006BFF, 1, 0xF0, " M3", "CpuRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x006C00, 0x006FFF, 1, 0x0F, " N3", "CpuRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x006C00, 0x006FFF, 1, 0xF0, " P3", "CpuRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x00D000, 0x00D3FF, 1, 0x0F, " K6", "VidRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x00D000, 0x00D3FF, 1, 0xF0, " J6", "VidRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x00D400, 0x00D8FF, 1, 0x0F, " L6", "ColRam"}, // 2114
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                                                                    "012", "012345"
                                                          {NO_BANK_SWITCH, 0x006000, 0x0063FF, 1, 0xFF, "FH3", "CpuRam"}, // 2114
                                                          {NO_BANK_SWITCH, 0x006400, 0x0067FF, 1, 0xFF, "JK3", "CpuRam"}, // 2114
                                                          {NO_BANK_SWITCH, 0x006800, 0x006BFF, 1, 0xFF, "LM3", "CpuRam"}, // 2114
                                                          {NO_BANK_SWITCH, 0x006C00, 0x006FFF, 1, 0xFF, "NP3", "CpuRam"}, // 2114
                                                          {NO_BANK_SWITCH, 0x00D000, 0x00D3FF, 1, 0xFF, "JK6", "VidRam"}, // 2114
                                                          {NO_BANK_SWITCH, 0x00D400, 0x00D8FF, 1, 0x0F, " L6", "ColRam"}, // 2114
                                                          {0}
                                                        }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                                                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x007000, 0x0073FF, 1, 0x0F, " N5", "SprRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x007000, 0x0073FF, 1, 0xF0, " P5", "SprRam"}, // 2114
                                                  {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                             "012", "012345"
                                                      {NO_BANK_SWITCH, 0x9000, 0xFF, "   ", "IN0   "},
                                                      {NO_BANK_SWITCH, 0x9001, 0xFF, "   ", "IN1   "},
                                                      {NO_BANK_SWITCH, 0x9002, 0xFF, "   ", "DSW0  "},
                                                      {NO_BANK_SWITCH, 0x9003, 0xFF, "   ", "DSW1  "},
                                                      {NO_BANK_SWITCH, 0xE000, 0xFF, "   ", "IN2   "},
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x0A000, 0x01, 0x00, " L5", "ScFlip"}, //
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CLadybugBaseGame::CLadybugBaseGame(
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
                         CZ80ACpu::CYCLE_TYPE_LADYBUG);
    m_cpu->idle();
}


CLadybugBaseGame::~CLadybugBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// Interrups are generated by the coin slots, there's nothing much to test
// in an automated fashion.
//
PERROR
CLadybugBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}

