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
#include "CRallyXBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   TBD - Probably doesn't need to be disabled
//
// Board Designations:
//   c - CPU board
//   v - Video board
//
// Z80 Compatibility Notes
//   VBLANK interrupt is only activated with a write to INT ON at 0x0a181


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x008000, 0x0083FF, 1, 0x0F, " 6C", "RadRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x008000, 0x0083FF, 1, 0xF0, " 6A", "RadRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x008400, 0x0087FF, 1, 0x0F, " 6D", "TilRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x008400, 0x0087FF, 1, 0xF0, " 6B", "TilRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x008800, 0x008BFF, 1, 0x0F, " 6K", "RadRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x008800, 0x008BFF, 1, 0xF0, " 6J", "RadRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x008C00, 0x008FFF, 1, 0x0F, " 6L", "TilRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x008C00, 0x008FFF, 1, 0xF0, " 6H", "TilRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x009800, 0x009BFF, 1, 0x0F, " 6M", "WorRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x009800, 0x009BFF, 1, 0xF0, " 6F", "WorRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x009C00, 0x009FFF, 1, 0x0F, " 6N", "WorRam"}, // 2114
                                                  {NO_BANK_SWITCH, 0x009C00, 0x009FFF, 1, 0xF0, " 6E", "WorRam"}, // 2114
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x008000, 0x0083FF, 1, 0xFF, "6AC", "RadRam"}, // 2114
                                                          {NO_BANK_SWITCH, 0x008400, 0x0087FF, 1, 0xFF, "6BD", "TilRam"}, // 2114
                                                          {NO_BANK_SWITCH, 0x008800, 0x008BFF, 1, 0xFF, "6JK", "RadRam"}, // 2114
                                                          {NO_BANK_SWITCH, 0x008C00, 0x008FFF, 1, 0xFF, "6HL", "TilRam"}, // 2114
                                                          {NO_BANK_SWITCH, 0x009800, 0x009BFF, 1, 0xFF, "6FM", "WorRam"}, // 2114
                                                          {NO_BANK_SWITCH, 0x009C00, 0x009FFF, 1, 0xFF, "6EN", "WorRam"}, // 2114
                                                          {0}
                                                        }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { // "012", "012345"
                                                           {0}
                                                         }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                             "012", "012345"
                                                      {NO_BANK_SWITCH, 0xa000, 0xFF, "   ", "P1    "},
                                                      {NO_BANK_SWITCH, 0xa080, 0xFF, "   ", "P2    "},
                                                      {NO_BANK_SWITCH, 0xa100, 0xFF, "   ", "DSW   "},
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x0a080, 0x01, 0x00, "12P", "WatchD"}, //
                                                        {NO_BANK_SWITCH, 0x0a130, 0x01, 0x00, "   ", "ScrolX"}, //
                                                        {NO_BANK_SWITCH, 0x0a140, 0x01, 0x00, "   ", "ScrolY"}, //
                                                        {NO_BANK_SWITCH, 0x0a180, 0x01, 0x00, "12M", "Bang  "}, //
                                                        {NO_BANK_SWITCH, 0x0a181, 0x01, 0x00, "12M", "Int On"}, //
                                                        {NO_BANK_SWITCH, 0x0a182, 0x01, 0x00, "12M", "Snd On"}, //
                                                        {NO_BANK_SWITCH, 0x0a183, 0x01, 0x00, "12M", "FlipSc"}, //
                                                        {NO_BANK_SWITCH, 0x0a184, 0x01, 0x00, "12M", "Led 0 "}, //
                                                        {NO_BANK_SWITCH, 0x0a185, 0x01, 0x00, "12M", "Led 1 "}, //
                                                        {NO_BANK_SWITCH, 0x0a186, 0x01, 0x00, "12M", "CoinLo"}, //
                                                        {NO_BANK_SWITCH, 0x0a187, 0x01, 0x00, "12M", "CoinCo"}, //
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CRallyXBaseGame::CRallyXBaseGame(
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
    m_cpu = new CZ80ACpu();
    m_cpu = new CZ80ACpu(0,
                         NO_ADDRESS_REMAP,
                         NULL,
                         NO_DATA_REMAP,
                         NULL,
                         CZ80ACpu::CYCLE_TYPE_LADYBUG);
    m_cpu->idle();
}


CRallyXBaseGame::~CRallyXBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// TODO: test interrupt
//
PERROR
CRallyXBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}

