//
// Copyright (c) 2018, Paul R. Swan
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
#include "CAstroFighterGame.h"


//                                       01   02   04   08   10   20   40   80  100  200
static const UINT16 s_romData2n_i2[] = {0x36,0xd0,0xa2,0xa9,0xa9,0xf2,0x6f,0x01,0xad,0x54}; // 11
static const UINT16 s_romData2n_i1[] = {0xc8,0xc8,0xa5,0xc9,0xc4,0xa9,0xa5,0x90,0xcc,0x1b}; // 10
static const UINT16 s_romData2n_i0[] = {0x4c,0xee,0x41,0x20,0x20,0xee,0xb1,0xd0,0x05,0x0d}; //  9
static const UINT16 s_romData2n_C5[] = {0xa9,0x00,0x63,0x4c,0xe6,0x60,0x69,0xd0,0x20,0x85}; //  8
static const UINT16 s_romData2n_D4[] = {0x09,0x10,0x01,0x69,0xde,0x01,0x14,0x91,0x60,0xa5}; //  7
static const UINT16 s_romData2n_F4[] = {0x03,0x4c,0xe4,0x85,0x6c,0xe4,0xfa,0x03,0xdd,0x60}; //  6
static const UINT16 s_romData2n_D5[] = {0xa2,0x5d,0xf9,0xe8,0x20,0x56,0x8d,0x03,0x85,0x91}; //  5
static const UINT16 s_romData2n_F5[] = {0xf0,0xe0,0x80,0xf8,0xf8,0x3f,0xf9,0x10,0xa0,0xbd}; //  4
static const UINT16 s_romData2n_H4[] = {0x4c,0xeb,0xad,0x29,0x29,0x60,0x26,0x00,0x41,0x80}; //  3
static const UINT16 s_romData2n_K4[] = {0x08,0xf4,0x02,0x85,0xa5,0x38,0x04,0xf4,0xa5,0x70}; //  2
static const UINT16 s_romData2n_H5[] = {0x00,0x00,0x00,0x00,0xfc,0x03,0x0f,0x00,0x30,0xf0}; //  1
static const UINT16 s_romData2n_K5[] = {0xe7,0xc3,0xbd,0x81,0xb1,0xea,0x1b,0x18,0xff,0x00}; //  0

//
// Set 2 from MAME.
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0xD000, 0x0400, s_romData2n_i2, 0x9f0bd355, " i2"}, // 11
                                                      {NO_BANK_SWITCH, 0xD400, 0x0400, s_romData2n_i1, 0x71f229f0, " i1"}, // 10
                                                      {NO_BANK_SWITCH, 0xD800, 0x0400, s_romData2n_i0, 0x88114f7c, " i0"}, //  9
                                                      {NO_BANK_SWITCH, 0xDC00, 0x0400, s_romData2n_C5, 0x9793c124, " C5"}, //  8
                                                      {NO_BANK_SWITCH, 0xE000, 0x0400, s_romData2n_D4, 0x08e44b12, " D4"}, //  7
                                                      {NO_BANK_SWITCH, 0xE400, 0x0400, s_romData2n_F4, 0x8a42d62c, " F4"}, //  6
                                                      {NO_BANK_SWITCH, 0xE800, 0x0400, s_romData2n_D5, 0x3e9aa743, " D5"}, //  5
                                                      {NO_BANK_SWITCH, 0xEC00, 0x0400, s_romData2n_F5, 0x712a4557, " F5"}, //  4
                                                      {NO_BANK_SWITCH, 0xF000, 0x0400, s_romData2n_H4, 0xad06f306, " H4"}, //  3
                                                      {NO_BANK_SWITCH, 0xF400, 0x0400, s_romData2n_K4, 0x680b91b4, " K4"}, //  2
                                                      {NO_BANK_SWITCH, 0xF800, 0x0400, s_romData2n_H5, 0x2c4cab1a, " H5"}, //  1
                                                      {NO_BANK_SWITCH, 0xFC00, 0x0400, s_romData2n_K5, 0xf699dda3, " K5"}, //  0
                                                      {0} }; // end of list

//
// Input region is the same for all ROM versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                               "012", "012345"
                                                      {NO_BANK_SWITCH, 0xA000L, 0xFF,  "???", "IN    "}, // Inputs
                                                      {NO_BANK_SWITCH, 0xA001L, 0xFF,  "???", "DSW   "}, // Dip switch
                                                      {NO_BANK_SWITCH, 0xA002L, 0xFF,  "???", "IrqClr"}, // Interrupt clear
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
                                                        {NO_BANK_SWITCH, 0x8006L, 0xFF, 0x00,  "???", "Sound1"}, // Sound control #1
                                                        {NO_BANK_SWITCH, 0x8007L, 0xFF, 0x00,  "???", "Sound2"}, // Sound control #2
                                                        {0}
                                                      }; // end of list

IGame*
CAstroFighterGame::createInstanceSet2(
)
{
    return (new CAstroFighterGame(false,
                                 s_romRegionSet2));
}

IGame*
CAstroFighterGame::createInstanceClockMasterSet2(
)
{
    return (new CAstroFighterGame(true,
                                 s_romRegionSet2));
}

CAstroFighterGame::CAstroFighterGame(
    const bool       clockMaster,
    const ROM_REGION *romRegion
) : CAstroFighterBaseGame( clockMaster,
                           romRegion,
                           s_inputRegion,
                           s_outputRegion )
{
}


