//
// Copyright (c) 2021, Marc Deslauriers
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
#include "CSpaceDuelGame.h"


//                                       01   02   04   08   10   20   40   80  100  200  400  800
static const UINT16 s_romData2n_1[] = {0x28,0x7a,0xb0,0x36,0x36,0x36,0x96,0x06,0x10,0x08,0xf6};      // 136006-106.r7
static const UINT16 s_romData2n_2[] = {0xaf,0xf8,0x02,0xfe,0x02,0xfc,0x00,0x00,0x3a,0x20,0xb8,0x06}; // 136006-107.np7
static const UINT16 s_romData2n_3[] = {0x02,0xbb,0x30,0xa8,0xc0,0x00,0x02,0x64,0x5b,0xa3,0x4a,0x03}; // 136006-101.r1
static const UINT16 s_romData2n_4[] = {0x07,0x09,0x08,0x03,0xb0,0x04,0xf0,0x7d,0xf0,0x4a,0x02,0x18}; // 136006-102.np1
static const UINT16 s_romData2n_5[] = {0x22,0x18,0xa9,0xa9,0x02,0x01,0xc2,0xa9,0x06,0x91,0xb9,0x4d}; // 136006-103.m1
static const UINT16 s_romData2n_6[] = {0xbd,0xdd,0xa6,0x60,0xe6,0xd6,0x41,0x68,0x00,0x10,0x03,0xa2}; // 136006-104.kl1
static const UINT16 s_romData2n_7[] = {0x00,0x00,0x00,0x00,0x00,0x00,0xa2,0x8d,0xd7,0x08,0xd2,0x91}; // 136006-105.j1

//
// SpaceDuel Set 1
//
static const ROM_REGION s_romRegionSpaceDuelSet1[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2n_1, 0x691122fe, "R7 "}, // 106
                                                            {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2n_2, 0xd8dd0461, "NP7"}, // 107
                                                            {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2n_3, 0xcd239e6c, "R1 "}, // 101
                                                            {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2n_4, 0x4c451e8a, "NP1"}, // 102
                                                            {NO_BANK_SWITCH, 0x6000, 0x1000, s_romData2n_5, 0xee72da63, "M1 "}, // 103
                                                            {NO_BANK_SWITCH, 0x7000, 0x1000, s_romData2n_6, 0xe41b38a3, "KL1"}, // 104
                                                            {NO_BANK_SWITCH, 0x8000, 0x1000, s_romData2n_7, 0x5652710f, "J1 "}, // 105
                                                            {NO_BANK_SWITCH, 0xF000, 0x1000, s_romData2n_7, 0x5652710f, "J1 "}, // 105 mirror
                                                            {0} }; // end of list

//
// SpaceDuel Set 2
//
static const ROM_REGION s_romRegionSpaceDuelSet2[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2n_1, 0x691122fe, "R7 "}, // 106
                                                            {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2n_2, 0xd8dd0461, "NP7"}, // 107
                                                            {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2n_3, 0xf4037b6e, "R1 "}, // 201
                                                            {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2n_4, 0x4c451e8a, "NP1"}, // 102
                                                            {NO_BANK_SWITCH, 0x6000, 0x1000, s_romData2n_5, 0xee72da63, "M1 "}, // 103
                                                            {NO_BANK_SWITCH, 0x7000, 0x1000, s_romData2n_6, 0xe41b38a3, "KL1"}, // 104
                                                            {NO_BANK_SWITCH, 0x8000, 0x1000, s_romData2n_7, 0x5652710f, "J1 "}, // 105
                                                            {NO_BANK_SWITCH, 0xF000, 0x1000, s_romData2n_7, 0x5652710f, "J1 "}, // 105 mirror
                                                            {0} }; // end of list



IGame*
CSpaceDuelGame::createInstanceSpaceDuelSet1(
)
{
    return (new CSpaceDuelGame(s_romRegionSpaceDuelSet1));
}

IGame*
CSpaceDuelGame::createInstanceSpaceDuelSet2(
)
{
    return (new CSpaceDuelGame(s_romRegionSpaceDuelSet2));
}

CSpaceDuelGame::CSpaceDuelGame(
    const ROM_REGION *romRegion
) : CSpaceDuelBaseGame( romRegion )
{
}


