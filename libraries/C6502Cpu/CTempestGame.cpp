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
#include "CTempestGame.h"


//                                       01   02   04   08   10   20   40   80  100  200  400
static const UINT16 s_romData2n_1[] = {0xbb,0x5a,0x50,0x4d,0x2b,0xa9,0x24,0xa5,0x05,0x04,0x15}; // 136002-113.d1
static const UINT16 s_romData2n_2[] = {0x30,0x01,0xad,0x10,0x01,0x02,0xbd,0x0f,0x01,0x3d,0xd0}; // 136002-114.e1
static const UINT16 s_romData2n_3[] = {0x20,0x28,0xa9,0x02,0x02,0x83,0xb9,0xf0,0x06,0xf0,0x2c}; // 136002-115.f1
static const UINT16 s_romData2n_4[] = {0x10,0x1b,0x18,0x08,0x03,0x03,0x01,0x85,0xf0,0x01,0xc9}; // 136002-116.h1
static const UINT16 s_romData2n_5[] = {0xdf,0xa6,0xbc,0x63,0xa0,0x69,0xe1,0x37,0x01,0xad,0x02}; // 136002-117.j1
static const UINT16 s_romData2n_6[] = {0x01,0xce,0x01,0x8e,0x01,0xa8,0x02,0x09,0x44,0x4b,0x00}; // 136002-118.k1
static const UINT16 s_romData2n_7[] = {0x01,0x92,0x16,0x8a,0xcb,0xcb,0xcb,0x89,0x30,0x0c,0x68}; // 136002-119.lm1
static const UINT16 s_romData2n_8[] = {0x03,0x2d,0x01,0x04,0x02,0x02,0x05,0x04,0xf8,0x26,0x0c}; // 136002-120.mn1
static const UINT16 s_romData2n_9[] = {0x06,0x85,0xa5,0x27,0x09,0xa2,0xd2,0xd3,0xd4,0x22,0x1e}; // 136002-121.p1
static const UINT16 s_romData2n_A[] = {0xac,0xab,0x20,0xa8,0xad,0x32,0x8d,0x34,0xfb,0x20,0xa9}; // 136002-122.r1
static const UINT16 s_romData2n_B[] = {0x48,0xc4,0xc4,0x18,0xc0,0xa0,0xdc,0x04,0xc6,0x1b,0xc8}; // 136002-123.np3
static const UINT16 s_romData2n_C[] = {0x1f,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0xc0,0x34}; // 136002-124.r3
static const UINT16 s_romData2n_D[] = {0xac,0xab,0x20,0xa8,0xad,0x32,0x8d,0x34,0xfb,0xe0,0x60}; // 136002-222.r1

//
// Tempest Set 1
//
static const ROM_REGION s_romRegionTempestSet1[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2n_B, 0x29f7e937, "NP3"}, // 123
                                                            {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2n_C, 0xc16ec351, "R3 "}, // 124
                                                            {NO_BANK_SWITCH, 0x9000, 0x0800, s_romData2n_1, 0x65d61fe7, "D1 "}, // 113
                                                            {NO_BANK_SWITCH, 0x9800, 0x0800, s_romData2n_2, 0x11077375, "E1 "}, // 114
                                                            {NO_BANK_SWITCH, 0xA000, 0x0800, s_romData2n_3, 0xf3e2827a, "F1 "}, // 115
                                                            {NO_BANK_SWITCH, 0xA800, 0x0800, s_romData2n_4, 0x7356896c, "H1 "}, // 116
                                                            {NO_BANK_SWITCH, 0xB000, 0x0800, s_romData2n_5, 0x55952119, "J1 "}, // 117
                                                            {NO_BANK_SWITCH, 0xB800, 0x0800, s_romData2n_6, 0xbeb352ab, "K1 "}, // 118
                                                            {NO_BANK_SWITCH, 0xC000, 0x0800, s_romData2n_7, 0xa4de050f, "LM1"}, // 119
                                                            {NO_BANK_SWITCH, 0xC800, 0x0800, s_romData2n_8, 0x35619648, "MN1"}, // 120
                                                            {NO_BANK_SWITCH, 0xD000, 0x0800, s_romData2n_9, 0x73d38e47, "P1 "}, // 121
                                                            {NO_BANK_SWITCH, 0xD800, 0x0800, s_romData2n_A, 0x796a9918, "R1 "}, // 122
                                                            {NO_BANK_SWITCH, 0xF800, 0x0800, s_romData2n_A, 0x796a9918, "P1M"}, // 122 mirror
                                                            {0} }; // end of list

//
// Tempest Set 2
//
static const ROM_REGION s_romRegionTempestSet2[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2n_B, 0x29f7e937, "NP3"}, // 123
                                                            {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2n_C, 0xc16ec351, "R3 "}, // 124
                                                            {NO_BANK_SWITCH, 0x9000, 0x0800, s_romData2n_1, 0x65d61fe7, "D1 "}, // 113
                                                            {NO_BANK_SWITCH, 0x9800, 0x0800, s_romData2n_2, 0x11077375, "E1 "}, // 114
                                                            {NO_BANK_SWITCH, 0xA000, 0x0800, s_romData2n_3, 0xf3e2827a, "F1 "}, // 115
                                                            {NO_BANK_SWITCH, 0xA800, 0x0800, s_romData2n_4, 0x7356896c, "H1 "}, // 116
                                                            {NO_BANK_SWITCH, 0xB000, 0x0800, s_romData2n_5, 0xef2eb645, "J1 "}, // 217
                                                            {NO_BANK_SWITCH, 0xB800, 0x0800, s_romData2n_6, 0xbeb352ab, "K1 "}, // 118
                                                            {NO_BANK_SWITCH, 0xC000, 0x0800, s_romData2n_7, 0xa4de050f, "LM1"}, // 119
                                                            {NO_BANK_SWITCH, 0xC800, 0x0800, s_romData2n_8, 0x35619648, "MN1"}, // 120
                                                            {NO_BANK_SWITCH, 0xD000, 0x0800, s_romData2n_9, 0x73d38e47, "P1 "}, // 121
                                                            {NO_BANK_SWITCH, 0xD800, 0x0800, s_romData2n_D, 0x707bd5c3, "R1 "}, // 222
                                                            {NO_BANK_SWITCH, 0xF800, 0x0800, s_romData2n_D, 0x707bd5c3, "P1M"}, // 222 mirror
                                                            {0} }; // end of list

//
// Tempest Set 3
//
static const ROM_REGION s_romRegionTempestSet3[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2n_B, 0x29f7e937, "NP3"}, // 123
                                                            {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2n_C, 0xc16ec351, "R3 "}, // 124
                                                            {NO_BANK_SWITCH, 0x9000, 0x0800, s_romData2n_1, 0x65d61fe7, "D1 "}, // 113
                                                            {NO_BANK_SWITCH, 0x9800, 0x0800, s_romData2n_2, 0x11077375, "E1 "}, // 114
                                                            {NO_BANK_SWITCH, 0xA000, 0x0800, s_romData2n_3, 0xf3e2827a, "F1 "}, // 115
                                                            {NO_BANK_SWITCH, 0xA800, 0x0800, s_romData2n_4, 0xaeb0f7e9, "H1 "}, // 316
                                                            {NO_BANK_SWITCH, 0xB000, 0x0800, s_romData2n_5, 0xef2eb645, "J1 "}, // 217
                                                            {NO_BANK_SWITCH, 0xB800, 0x0800, s_romData2n_6, 0xbeb352ab, "K1 "}, // 118
                                                            {NO_BANK_SWITCH, 0xC000, 0x0800, s_romData2n_7, 0xa4de050f, "LM1"}, // 119
                                                            {NO_BANK_SWITCH, 0xC800, 0x0800, s_romData2n_8, 0x35619648, "MN1"}, // 120
                                                            {NO_BANK_SWITCH, 0xD000, 0x0800, s_romData2n_9, 0x73d38e47, "P1 "}, // 121
                                                            {NO_BANK_SWITCH, 0xD800, 0x0800, s_romData2n_D, 0x707bd5c3, "R1 "}, // 222
                                                            {NO_BANK_SWITCH, 0xF800, 0x0800, s_romData2n_D, 0x707bd5c3, "P1M"}, // 222 mirror
                                                            {0} }; // end of list


IGame*
CTempestGame::createInstanceTempestSet1(
)
{
    return (new CTempestGame(s_romRegionTempestSet1));
}

IGame*
CTempestGame::createInstanceTempestSet2(
)
{
    return (new CTempestGame(s_romRegionTempestSet2));
}

IGame*
CTempestGame::createInstanceTempestSet3(
)
{
    return (new CTempestGame(s_romRegionTempestSet3));
}

CTempestGame::CTempestGame(
    const ROM_REGION *romRegion
) : CTempestBaseGame( romRegion )
{
}


