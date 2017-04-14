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
#include "CTomahawk777Game.h"


//                                       01   02   04   08   10   20   40   80  100  200
static const UINT16 s_romData2n_C5[] = {0xA2,0xFF,0xD8,0x20,0xDC,0x16,0x06,0xA9,0x08,0x20}; // 8
static const UINT16 s_romData2n_D4[] = {0xA0,0xFF,0x13,0xA9,0x08,0x3F,0xA0,0xF8,0x85,0xE2}; // 7
static const UINT16 s_romData2n_F4[] = {0xB5,0xB8,0x69,0x90,0xA9,0x73,0xA0,0xE4,0xFF,0x42}; // 6
static const UINT16 s_romData2n_D5[] = {0xFB,0x04,0x2C,0x7C,0xE7,0x85,0x3B,0x29,0x04,0x85}; // 5
static const UINT16 s_romData2n_F5[] = {0x60,0xA9,0x85,0xBD,0x1D,0x40,0xA5,0xA6,0x90,0x85}; // 4
static const UINT16 s_romData2n_H4[] = {0xA2,0x0E,0x86,0xEF,0x4D,0xFF,0xDC,0xE8,0x85,0x8D}; // 3
static const UINT16 s_romData2n_K4[] = {0xB1,0xB9,0xC9,0x57,0x97,0x94,0x03,0x10,0x02,0x00}; // 2
static const UINT16 s_romData2n_H5[] = {0x38,0x68,0x20,0x7C,0x54,0x48,0x00,0x05,0x06,0x00}; // 1
static const UINT16 s_romData2n_K5[] = {0x20,0x10,0x20,0x6D,0x60,0x02,0x06,0x00,0x40,0x18}; // 0

//
// Later Set 5 from MAME.
//
static const ROM_REGION s_romRegionSet5[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0xDC00, 0x0400, s_romData2n_C5, 0xb01dab4b, " C5"}, // 8
                                                      {NO_BANK_SWITCH, 0xE000, 0x0400, s_romData2n_D4, 0x3a6549e8, " D4"}, // 7
                                                      {NO_BANK_SWITCH, 0xE400, 0x0400, s_romData2n_F4, 0x863e47f7, " F4"}, // 6
                                                      {NO_BANK_SWITCH, 0xE800, 0x0400, s_romData2n_D5, 0xde0183bc, " D5"}, // 5
                                                      {NO_BANK_SWITCH, 0xEC00, 0x0400, s_romData2n_F5, 0x11e9c7ea, " F5"}, // 4
                                                      {NO_BANK_SWITCH, 0xF000, 0x0400, s_romData2n_H4, 0xec44d388, " H4"}, // 3
                                                      {NO_BANK_SWITCH, 0xF400, 0x0400, s_romData2n_K4, 0xdc0a0f54, " K4"}, // 2
                                                      {NO_BANK_SWITCH, 0xF800, 0x0400, s_romData2n_H5, 0x1d9dab9c, " H5"}, // 1
                                                      {NO_BANK_SWITCH, 0xFC00, 0x0400, s_romData2n_K5, 0xd21a1eba, " K5"}, // 0
                                                      {0} }; // end of list

//
// Input region is the same for all ROM versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                               "012", "012345"
                                                      {NO_BANK_SWITCH, 0xA000L, 0xFF,  "???", "IN    "}, // Inputs
                                                      {NO_BANK_SWITCH, 0xA001L, 0xFF,  "???", "DSW   "}, // Dip switch
                                                      {NO_BANK_SWITCH, 0xA002L, 0xFF,  "???", "IrqClr"}, // Interupt clear
                                                      {NO_BANK_SWITCH, 0xA003L, 0xFF,  "???", "Prot. "}, // Protection read
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all ROM versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x8003L, 0xFF, 0x00,  "???", "Colour"}, // Colour control
                                                        {NO_BANK_SWITCH, 0x8004L, 0x01, 0x00,  "c6J", "Flip  "}, // Video control #1 - D0 - Flip
                                                        {NO_BANK_SWITCH, 0x8004L, 0x02, 0x00,  "c6J", "VidOff"}, // Video control #1 - D1 - Screen off
                                                        {NO_BANK_SWITCH, 0x8005L, 0x01, 0x00,  "c2C", "Out0  "}, // Video control #2 - D0 - Out 0.
                                                        {NO_BANK_SWITCH, 0x8005L, 0x02, 0x00,  "c2C", "Out1  "}, // Video control #2 - D1 - Out 1.
                                                        {NO_BANK_SWITCH, 0x8005L, 0x04, 0x00,  "c2C", "ClrSel"}, // Video control #2 - D2 - Colour select.
                                                        {NO_BANK_SWITCH, 0x8005L, 0x08, 0x00,  "c2C", "Red on"}, // Video control #2 - D3 - Red screen
                                                        {NO_BANK_SWITCH, 0x8006L, 0xFF, 0x00,  "???", "Sound "}, // Sound control
                                                        {NO_BANK_SWITCH, 0x8007L, 0xFF, 0x00,  "???", "Prot. "}, // Protection write
                                                        {0}
                                                      }; // end of list

IGame*
CTomahawk777Game::createInstanceSet5(
)
{
    return (new CTomahawk777Game(false,
                                 s_romRegionSet5));
}

IGame*
CTomahawk777Game::createInstanceClockMasterSet5(
)
{
    return (new CTomahawk777Game(true,
                                 s_romRegionSet5));
}

CTomahawk777Game::CTomahawk777Game(
    const bool       clockMaster,
    const ROM_REGION *romRegion
) : CAstroFighterBaseGame( clockMaster,
                           romRegion,
                           s_inputRegion,
                           s_outputRegion )
{
}


