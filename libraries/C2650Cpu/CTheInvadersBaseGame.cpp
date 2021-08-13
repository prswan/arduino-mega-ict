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
#include "CTheInvadersBaseGame.h"
#include "C2650Cpu.h"
#include <DFR_Key.h>


static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                  "012", "012345"
                                                  {NO_BANK_SWITCH, 0x1800,      0x1BFF,      1, 0x01, " 10", "VRAM  "}, // "Video Ram 2102"
                                                  {NO_BANK_SWITCH, 0x1800,      0x1BFF,      1, 0x02, "  9", "VRAM  "}, // "Video Ram 2102"
                                                  {NO_BANK_SWITCH, 0x1800,      0x1BFF,      1, 0x04, "  8", "VRAM  "}, // "Video Ram 2102"
                                                  {NO_BANK_SWITCH, 0x1800,      0x1BFF,      1, 0x08, "  7", "VRAM  "}, // "Video Ram 2102"
                                                  {NO_BANK_SWITCH, 0x1800,      0x1BFF,      1, 0x10, "  4", "VRAM  "}, // "Video Ram 2102"
                                                  {NO_BANK_SWITCH, 0x1800,      0x1BFF,      1, 0x20, "  3", "VRAM  "}, // "Video Ram 2102"
                                                  {NO_BANK_SWITCH, 0x1800,      0x1BFF,      1, 0x40, "  2", "VRAM  "}, // "Video Ram 2102"
                                                  {NO_BANK_SWITCH, 0x1800,      0x1BFF,      1, 0x80, "  1", "VRAM  "}, // "Video Ram 2102"
                                                  {NO_BANK_SWITCH, 0x1C00,      0x1CFF,      1, 0x0F, " 48", "Prog. "}, // "Program RAM, 2101"
                                                  {NO_BANK_SWITCH, 0x1C00,      0x1CFF,      1, 0xF0, " 28", "Prog. "}, // "Program RAM, 2101"
                                                  {NO_BANK_SWITCH, 0x1F00+0x00, 0x1F00+0x2D, 1, 0xFF, " 33", "2636  "}, // "2636 PVI"
                                                  {NO_BANK_SWITCH, 0x1F00+0x40, 0x1F00+0x6D, 1, 0xFF, " 33", "2636  "}, // "2636 PVI"
                                                  {NO_BANK_SWITCH, 0x1F00+0x80, 0x1F00+0xAD, 1, 0xFF, " 33", "2636  "}, // "2636 PVI"
                                                  {0}
                                                }; // end of list

static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                  "012", "012345"
                                                          {NO_BANK_SWITCH, 0x1800,      0x1BFF,      1, 0xFF, " - ", "VRAM  "}, // "Video Ram 2102"
                                                          {NO_BANK_SWITCH, 0x1C00,      0x1CFF,      1, 0xFF, " - ", "Prog. "}, // "Program RAM, 2101"
                                                          {NO_BANK_SWITCH, 0x1F00+0x00, 0x1F00+0x2D, 1, 0xFF, " 33", "2636  "}, // "2636 PVI"
                                                          {NO_BANK_SWITCH, 0x1F00+0x40, 0x1F00+0x6D, 1, 0xFF, " 33", "2636  "}, // "2636 PVI"
                                                          {NO_BANK_SWITCH, 0x1F00+0x80, 0x1F00+0xAD, 1, 0xFF, " 33", "2636  "}, // "2636 PVI"
                                                          {0}
                                                        }; // end of list

//
// This region allows the PVI to be initialized.
// If the PVI is not initialized, the screen ends up all white such that no
// other graphics can be seen.
//

static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                        "012", "012345"
                                                           {NO_BANK_SWITCH, 0x1F00, 0x1FFF, 1, 0xFF, " 33", "2636  "}, // "2636 PVI"
                                                           {0}
                                                         }; // end of list

//
// Input region is the same for all versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                              "012", "012345"
                                                      {NO_BANK_SWITCH, 0x1E80L, 0xFF, " 38", "IN0   "}, // Row 0 - Coin, Start, Service, Fire, Collision
                                                      {NO_BANK_SWITCH, 0x1E81L, 0xFF, " 38", "DIP   "}, // Row 1 - Dip Switches
                                                      {NO_BANK_SWITCH, 0x1E82L, 0xFF, " 38", "IN1   "}, // Row 2 - Joystick
                                                      {NO_BANK_SWITCH, 0x1E85L, 0xFF, " - ", "ExtDip"}, // Row 5 - External DIP (Dodgem)
                                                      {NO_BANK_SWITCH, 0x1E87L, 0xFF, " 13", "ColRst"}, // Row 7 - Collision Reset Pulse
                                                      {0}
                                                    }; // end of list

//
// Output region goes to the sound daughter PCB
//
// From MAME
/* 08 = hit invader */
/* 20 = bonus (extra base) */
/* 40 = saucer */
/* 84 = fire */
/* 90 = die */
/* c4 = hit saucer */
//
// ...and it appears 0x1 is the sound enable bit
//

static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x01, 0x00, "CI6", "Snd  1"}, //
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x02, 0x00, "CI6", "Snd  2"}, //
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x04, 0x00, "CI6", "Snd  3"}, //
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x08, 0x00, "CI6", "Snd  4"}, //
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x10, 0x00, "CI6", "Snd  5"}, //
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x20, 0x00, "CI6", "Snd  6"}, //
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x08, 0x00, " 19", "SndL 7"}, //
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x10, 0x00, " 19", "SndL 8"}, //
                                                        // MAME game sound mappping
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x08+0x01, 0x00, "CI6", "InvHit"}, //
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x20+0x01, 0x00, "CI6", "ExtrBa"}, //
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x40+0x01, 0x00, "CI6", "Saucer"}, //
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x84+0x01, 0x00, "CI6", "Fire  "}, //
                                                        {NO_BANK_SWITCH, 0x1E80L, 0x90+0x01, 0x00, "CI6", "BseHit"}, //
                                                        {NO_BANK_SWITCH, 0x1E80L, 0xC4+0x01, 0x00, "CI6", "ScrHit"}, //
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CTheInvadersBaseGame::CTheInvadersBaseGame(
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
    m_cpu = new C2650Cpu();
    m_cpu->idle();

    // There is only a single interrupt pin on the 2650.
    m_interrupt = ICpu::IRQ0;

    // The interrupt uses an external hardware vector.
    m_interruptAutoVector = false;

    // The vector is hard coded to the same value on both games.
    m_interruptResponse = 0x03;

    // Clear the sound generator
    m_cpu->memoryWrite(0x1FC7, 0x00);
}


CTheInvadersBaseGame::~CTheInvadersBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}

