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
#include "CStarWarsGame.h"

//
// Default set from MAME.
//
//                                          01   02   04   08   10   20   40   80  100  200  400  800 1000
static const UINT16 s_romData2n_1F_Lo[] = {0xf2,0x61,0x56,0xcc,0x3d,0x24,0xad,0x68,0x02,0x02,0x20,0x41,0xfc};
static const UINT16 s_romData2n_1F_Hi[] = {0xf2,0x61,0x56,0x00,0xea,0x30,0x00,0xd0,0xd9,0x00,0x3c,0xfd,0x28};
static const UINT16 s_romData2n_1HJ[]   = {0x00,0x05,0xb0,0x2c,0x0a,0x2c,0x01,0x00,0x02,0x05,0x60,0x7c,0xa7};
static const UINT16 s_romData2n_1JK[]   = {0x38,0x00,0x00,0x38,0x00,0x00,0x50,0x00,0x00,0xb7,0xbd,0xbd,0xa0};
static const UINT16 s_romData2n_1KL[]   = {0xfa,0xa6,0xbd,0x30,0xd7,0x85,0x19,0xfe,0x02,0xfd,0x4c,0x2b,0xbb};
static const UINT16 s_romData2n_1M[]    = {0x42,0x45,0x20,0x43,0x20,0xd3,0x46,0x41,0x41,0x50,0x20,0xea,0x01};

static const ROM_REGION s_romRegion[] PROGMEM =     { //
                                                      {CStarWarsBaseGame::onBankSwitchMPAGE0, 0x6000, 0x2000, s_romData2n_1F_Lo,  0xA7ED5F8A, "1F0"}, // 214 - Lo
                                                      {CStarWarsBaseGame::onBankSwitchMPAGE1, 0x6000, 0x2000, s_romData2n_1F_Hi,  0xB8123618, "1F1"}, // 214 - Hi
                                                      {NO_BANK_SWITCH,                        0x8000, 0x2000, s_romData2n_1HJ,    0xf725e344, "1HJ"}, // 102
                                                      {NO_BANK_SWITCH,                        0xA000, 0x2000, s_romData2n_1JK,    0xf6da0a00, "1JK"}, // 203
                                                      {NO_BANK_SWITCH,                        0xC000, 0x2000, s_romData2n_1KL,    0x7e406703, "1KL"}, // 104
                                                      {NO_BANK_SWITCH,                        0xE000, 0x2000, s_romData2n_1M,     0xc7e51237, "1M "}, // 206
                                                      {0} }; // end of list

//
// Set "O" from MAME.
//
static const ROM_REGION s_romRegionSetO[] PROGMEM = { //
                                                      {CStarWarsBaseGame::onBankSwitchMPAGE0, 0x6000, 0x2000, s_romData2n_1F_Lo,  0x67CBA67C, "1F0"}, // 114 - Lo
                                                      {CStarWarsBaseGame::onBankSwitchMPAGE1, 0x6000, 0x2000, s_romData2n_1F_Hi,  0xB8123618, "1F1"}, // 114 - Hi
                                                      {NO_BANK_SWITCH,                        0x8000, 0x2000, s_romData2n_1HJ,    0xf725e344, "1HJ"}, // 102
                                                      {NO_BANK_SWITCH,                        0xA000, 0x2000, s_romData2n_1JK,    0x3fde9ccb, "1JK"}, // 103
                                                      {NO_BANK_SWITCH,                        0xC000, 0x2000, s_romData2n_1KL,    0x7e406703, "1KL"}, // 104
                                                      {NO_BANK_SWITCH,                        0xE000, 0x2000, s_romData2n_1M,     0xc7e51237, "1M "}, // 206
                                                      {0} }; // end of list

IGame*
CStarWarsGame::createInstance(
)
{
    return (new CStarWarsGame(s_romRegion));
}


IGame*
CStarWarsGame::createInstanceSetO(
)
{
    return (new CStarWarsGame(s_romRegionSetO));
}

CStarWarsGame::CStarWarsGame(
    const ROM_REGION *romRegion
) : CStarWarsBaseGame( romRegion )
{
}

