//
// Copyright (c) 2020, Paul R. Swan
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
#include "CFitterBaseGame.h"
#include "CZ80ACpu.h"

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   Tie CPU board IC 44 (74LS00) pin 4 to GND. This is under the security module, but the resistor
//   for it, R62, and jumper "J" are just about accessible underneath.
//
// Board Designations:
//   "c" - Main CPU & sound PCB HT-01A
//   "v" - Video PCB HT-01B
//   "s" - Security module (metal can epoxy)
//
// Z80 Compatibility Notes
//   The schematics indicate there is WAIT hold-off on the CRAM & VRAM access for video blanking.
//


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x008000, 0x0083FF, 1, 0x0F, "c21", "WRAM  "}, // 2114
                                                  {NO_BANK_SWITCH, 0x008000, 0x0083FF, 1, 0xF0, "c22", "WRAM  "}, // 2114
                                                  // MAME shows 0x800 of WRAM but I suspect this is for other games (e.g. Portman)
                                                  {NO_BANK_SWITCH, 0x108800, 0x108BFF, 1, 0x0F, "s??", "CRAM  "}, // 2114, in the security module?
                                                  {NO_BANK_SWITCH, 0x108800, 0x108BFF, 1, 0xF0, "s??", "CRAM  "}, // 2114, in the security module?
                                                  {NO_BANK_SWITCH, 0x109000, 0x1093FF, 1, 0x0F, "v02", "VRAM  "}, // 2114
                                                  {NO_BANK_SWITCH, 0x109000, 0x1093FF, 1, 0xF0, "v03", "VRAM  "}, // 2114
                                                  {NO_BANK_SWITCH, 0x009800, 0x0098FF, 1, 0x0F, "v07", "OBJRAM"}, // 2114, 256b used
                                                  {NO_BANK_SWITCH, 0x009800, 0x0098FF, 1, 0xF0, "v06", "OBJRAM"}, // 2114, 256b used
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x008000, 0x0083FF, 1, 0xFF, "c2x", "WRAM  "}, //
                                                          {NO_BANK_SWITCH, 0x108800, 0x108BFF, 1, 0xFF, "s??", "CRAM  "}, //
                                                          {NO_BANK_SWITCH, 0x109000, 0x1093FF, 1, 0xFF, "v0x", "VRAM  "}, //
                                                          {NO_BANK_SWITCH, 0x009800, 0x0098FF, 1, 0xFF, "v0x", "OBJRAM"}, //
                                                          {0}
                                                        }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //  "012", "012345"
                                                           {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                             "012", "012345"
                                                      {NO_BANK_SWITCH, 0xa800, 0xFF, "c05", "IN1   "}, // Inputs, c05 p1 & c06 p2 by HFLIP
                                                      {NO_BANK_SWITCH, 0xb000, 0xFF, "c12", "DSW1  "}, // DIP switches, c13 also
                                                      {NO_BANK_SWITCH, 0xb800, 0xFF, "c44", "Wd Res"}, // Watchdog Reset
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x0b000, 0x01, 0x00, "c42", "NMI En"}, // NMION
                                                        {NO_BANK_SWITCH, 0x0b002, 0x01, 0x00, "c42", "CoinLO"}, // LOCKOUT
                                                        {NO_BANK_SWITCH, 0x0b003, 0x01, 0x00, "c42", "Snd En"}, // GON
                                                        {NO_BANK_SWITCH, 0x0b006, 0x01, 0x00, "c42", "H Flip"}, // HFLIP
                                                        {NO_BANK_SWITCH, 0x0b007, 0x01, 0x00, "c42", "V Flip"}, // VFLIP
                                                        {NO_BANK_SWITCH, 0x0b800, 0xFF, 0x00, "c09", "SndCmd"}, // PITCH
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CFitterBaseGame::CFitterBaseGame(
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
                         NULL);

    m_cpu->idle();

    // The VBLANK interrupt is on the NMI pin.
    m_interrupt = ICpu::NMI;

    // Internal vector
    m_interruptAutoVector = true;
}


CFitterBaseGame::~CFitterBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


PERROR
CFitterBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}

