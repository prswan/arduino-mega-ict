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
#include "CGravitarGame.h"

//
// Gravitar Set 1
//
static const ROM_DATA2N s_romData2nGSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                        {0x16,0x85,0x85,0x85,0x85,0x85,0x7a,0x57,0xab,0x20,0xbb,0x00},// 136010-101.d1
                                                        {0x24,0x85,0xa5,0x84,0x38,0xa5,0xa0,0x85,0x85,0x29,0xff,0x81}, // 136010-102.ef1
                                                        {0x00,0x81,0x00,0x00,0x00,0x01,0x00,0xff,0x01,0xfe,0x00,0x00}, // 136010-103.h1
                                                        {0xfa,0xfa,0xfa,0xfb,0x08,0xc1,0xc3,0xca,0xa8,0xf7,0x20,0x20}, // 136010-104.j1
                                                        {0x3d,0x5c,0x64,0x74,0x62,0xf8,0xf8,0x10,0x96,0xcf,0x40,0x00}, // 136010-105.kl1
                                                        {0x03,0x80,0x00,0xff,0xff,0xfe,0xa1,0xf8,0x26,0x10,0x20,0xa2}, // 136010-106.m1
                                                        {0x00,0xff,0x01,0xff,0xff,0xff,0x00,0x6e,0xff,0x00,0x00,0x00}, // 136010-107.mn7
                                                        {0xef,0x40,0x84,0x82,0x26,0x3a,0x28,0x30,0x9c,0x21,0x54,0x04}, // 136010-108.np7
                                                        {0x9b,0x50,0xd6,0x52,0x01,0x04,0xf6,0x85,0xa9,0xa8,0x00,0x20}, // 136010-109.r7
                                                        {0x00,0xff,0x00,0x40,0x60,0xc0,0xc0,0x00,0x00,0x60,0x00},      // 136010-110.l7
                                                        {0} };  // end of list

static const ROM_REGION s_romRegionGSet1[] PROGMEM = { //
                                                       {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nGSet1[9].data2n, 0x1da0d845, "L7 "}, // 110
                                                       {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nGSet1[6].data2n, 0x650ba31e, "MN7"}, // 107
                                                       {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nGSet1[7].data2n, 0x5119c0b2, "NP7"}, // 108
                                                       {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nGSet1[8].data2n, 0xdefa8cbc, "R7 "}, // 109
                                                       {NO_BANK_SWITCH, 0x9000, 0x1000, s_romData2nGSet1[0].data2n, 0xacbc0e2c, "D1 "}, // 101
                                                       {NO_BANK_SWITCH, 0xA000, 0x1000, s_romData2nGSet1[1].data2n, 0x88f98f8f, "EF1"}, // 102
                                                       {NO_BANK_SWITCH, 0xB000, 0x1000, s_romData2nGSet1[2].data2n, 0x68a85703, "H1 "}, // 103
                                                       {NO_BANK_SWITCH, 0xC000, 0x1000, s_romData2nGSet1[3].data2n, 0x33d19ef6, "J1 "}, // 104
                                                       {NO_BANK_SWITCH, 0xD000, 0x1000, s_romData2nGSet1[4].data2n, 0x032b5806, "KL1"}, // 105
                                                       {NO_BANK_SWITCH, 0xE000, 0x1000, s_romData2nGSet1[5].data2n, 0x47fe97a0, "M1 "}, // 106
                                                       {NO_BANK_SWITCH, 0xF000, 0x1000, s_romData2nGSet1[5].data2n, 0x47fe97a0, "M1 "}, // 106 mirror
                                                       {0} }; // end of list

//
// Gravitar Set 2
//
static const ROM_DATA2N s_romData2nGSet2[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                        {0x16,0x85,0x85,0x85,0x85,0x85,0x7a,0x57,0xb4,0x20,0xbb,0x00}, // 136010.201
                                                        {0x24,0x85,0xa5,0x84,0x38,0xa5,0xa0,0x85,0x85,0x29,0xff,0x81}, // 136010.202
                                                        {0x00,0x81,0x00,0x00,0x00,0x01,0x00,0xff,0x01,0xfe,0x00,0x00}, // 136010.203
                                                        {0xfa,0xfa,0xfa,0xfb,0x08,0xc1,0xc3,0xca,0xa8,0xf7,0x20,0x20}, // 136010.204
                                                        {0x20,0xb6,0xc6,0xcf,0x3d,0x3e,0x42,0x0b,0x85,0xf0,0x00,0x00}, // 136010.205
                                                        {0x00,0x00,0x02,0x08,0x20,0x00,0x01,0x00,0x00,0x0a,0x00,0x4a}, // 136010.206
                                                        {0x00,0xff,0x01,0xff,0xff,0xff,0x00,0x6e,0xff,0xff,0xff,0x01}, // 136010.207
                                                        {0xef,0x40,0x84,0x82,0x26,0x3a,0x28,0x30,0x9c,0x21,0x54,0x04}, // 136010.208
                                                        {0x9b,0x50,0xd6,0x52,0x01,0x04,0xf6,0x85,0xa9,0xa8,0x00,0x20}, // 136010.209
                                                        {0x00,0xff,0x00,0x40,0x60,0xc0,0xc0,0x00,0x00,0x60,0x00},      // 136010.210
                                                        {0} };  // end of list

static const ROM_REGION s_romRegionGSet2[] PROGMEM = { //
                                                       {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nGSet2[9].data2n, 0xdebcb243, "L7 "}, // 210
                                                       {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nGSet2[6].data2n, 0x4135629a, "MN7"}, // 207
                                                       {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nGSet2[7].data2n, 0x358f25d9, "NP7"}, // 208
                                                       {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nGSet2[8].data2n, 0x37034287, "R7 "}, // 209
                                                       {NO_BANK_SWITCH, 0x9000, 0x1000, s_romData2nGSet2[0].data2n, 0x167315e4, "D1 "}, // 201
                                                       {NO_BANK_SWITCH, 0xA000, 0x1000, s_romData2nGSet2[1].data2n, 0xaaa9e62c, "EF1"}, // 202
                                                       {NO_BANK_SWITCH, 0xB000, 0x1000, s_romData2nGSet2[2].data2n, 0xae437253, "H1 "}, // 203
                                                       {NO_BANK_SWITCH, 0xC000, 0x1000, s_romData2nGSet2[3].data2n, 0x5d6bc29e, "J1 "}, // 204
                                                       {NO_BANK_SWITCH, 0xD000, 0x1000, s_romData2nGSet2[4].data2n, 0x0db1ff34, "KL1"}, // 205
                                                       {NO_BANK_SWITCH, 0xE000, 0x1000, s_romData2nGSet2[5].data2n, 0x4521ca48, "M1 "}, // 206
                                                       {NO_BANK_SWITCH, 0xF000, 0x1000, s_romData2nGSet2[5].data2n, 0x4521ca48, "M1 "}, // 206 mirror
                                                       {0} }; // end of list

//
// Gravitar Set 3
//
static const ROM_DATA2N s_romData2nGSet3[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                        {0x16,0x85,0x85,0x85,0x85,0x85,0x7a,0x57,0xc2,0x20,0xbb,0x00}, // 136010-301.d1
                                                        {0x24,0x85,0xa5,0x84,0x38,0xa5,0xa0,0x85,0x85,0x29,0xff,0x81}, // 136010-302.ef1
                                                        {0x00,0x01,0xff,0x01,0x00,0x00,0x0a,0xff,0x01,0x00,0x00,0x01}, // 136010-303.h1
                                                        {0x03,0x05,0x04,0xfb,0xfa,0x08,0xb5,0x00,0xa8,0x86,0xb9,0xb0}, // 136010-304.j1
                                                        {0x01,0xd0,0xa0,0xd0,0xc6,0x3d,0x41,0x03,0xde,0x4b,0x00,0x00}, // 136010-305.kl1
                                                        {0x00,0x00,0x01,0x01,0x00,0x01,0x00,0x03,0xaf,0xa0,0x12,0xc0}, // 136010-306.m1
                                                        {0x00,0xff,0x01,0xff,0xff,0xff,0x00,0x6e,0xff,0xff,0xff,0x01}, // 136010-207.mn7
                                                        {0xef,0x40,0x84,0x82,0x26,0x3a,0x28,0x30,0x9c,0x21,0x54,0x04}, // 136010-208.np7
                                                        {0x9b,0x50,0xd6,0x52,0x01,0x04,0xf6,0x85,0xa9,0xa8,0x00,0x20}, // 136010-309.r7
                                                        {0x00,0xff,0x00,0x40,0x60,0xc0,0xc0,0x00,0x00,0x60,0x00},      // 136010-210.l7
                                                        {0} };  // end of list

static const ROM_REGION s_romRegionGSet3[] PROGMEM = { //
                                                       {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nGSet3[9].data2n, 0xdebcb243, "L7 "}, // 210
                                                       {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nGSet3[6].data2n, 0x4135629a, "MN7"}, // 207
                                                       {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nGSet3[7].data2n, 0x358f25d9, "NP7"}, // 208
                                                       {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nGSet3[8].data2n, 0x4ac78df4, "R7 "}, // 309
                                                       {NO_BANK_SWITCH, 0x9000, 0x1000, s_romData2nGSet3[0].data2n, 0xa2a55013, "D1 "}, // 301
                                                       {NO_BANK_SWITCH, 0xA000, 0x1000, s_romData2nGSet3[1].data2n, 0xd3700b3c, "EF1"}, // 302
                                                       {NO_BANK_SWITCH, 0xB000, 0x1000, s_romData2nGSet3[2].data2n, 0x8e12e3e0, "H1 "}, // 303
                                                       {NO_BANK_SWITCH, 0xC000, 0x1000, s_romData2nGSet3[3].data2n, 0x467ad5da, "J1 "}, // 304
                                                       {NO_BANK_SWITCH, 0xD000, 0x1000, s_romData2nGSet3[4].data2n, 0x840603af, "KL1"}, // 305
                                                       {NO_BANK_SWITCH, 0xE000, 0x1000, s_romData2nGSet3[5].data2n, 0x3f3805ad, "M1 "}, // 306
                                                       {NO_BANK_SWITCH, 0xF000, 0x1000, s_romData2nGSet3[5].data2n, 0x3f3805ad, "M1 "}, // 306 mirror
                                                       {0} }; // end of list

//
// Black Widow Set
//
static const ROM_DATA2N s_romData2nBSet[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x4f,0x50,0x52,0x54,0x41,0x60,0x01,0x9d,0xa2,0xd8,0xef,0xdc}, // 136017-101.d1
                                                       {0x01,0x01,0x11,0x01,0x01,0x11,0x01,0x02,0x4c,0xe6,0x9d,0x29}, // 136017-102.ef1
                                                       {0x10,0xb0,0x9d,0x31,0x4c,0x80,0xd0,0x69,0xb1,0xb0,0x98,0xd8}, // 136017-103.h1
                                                       {0x03,0x3c,0x84,0xde,0x92,0x5a,0x2a,0xf4,0xbe,0x1c,0xbd,0x73}, // 136017-104.j1
                                                       {0x74,0xd3,0xd2,0xd2,0xcf,0xcf,0xcf,0xd4,0x00,0x99,0x3f,0x11}, // 136017-105.kl1
                                                       {0xe5,0xcf,0x00,0x13,0x49,0xc9,0xc1,0x71,0x2c,0x3a,0x04,0x88}, // 136017-106.m1
                                                       {0x00,0x00,0x25,0x28,0x2a,0x28,0x0a,0x5a,0x3d,0x20,0x3f,0x23}, // 136017-108.mn7
                                                       {0x44,0x23,0x35,0x00,0x22,0x3c,0x00,0x39,0x34,0x3b,0x20,0x23}, // 136017-109.np7
                                                       {0x5e,0x3a,0x3c,0x05,0x22,0x3d,0x23,0x3e,0x21,0x06,0x29,0x25}, // 136017-110.r7
                                                       {0xe8,0xb9,0xc4,0xda,0x06,0x5e,0x6e,0x75,0x6f,0x30,0x30},      // 136017-107.l7
                                                       {0} };  // end of list

static const ROM_REGION s_romRegionBSet[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nBSet[9].data2n, 0x97f6000c, "L7 "}, // 107
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nBSet[6].data2n, 0x3da354ed, "MN7"}, // 108
                                                      {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nBSet[7].data2n, 0x2fc4ce79, "NP7"}, // 109
                                                      {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nBSet[8].data2n, 0x0dd52987, "R7 "}, // 110
                                                      {NO_BANK_SWITCH, 0x9000, 0x1000, s_romData2nBSet[0].data2n, 0xfe3febb7, "D1 "}, // 101
                                                      {NO_BANK_SWITCH, 0xA000, 0x1000, s_romData2nBSet[1].data2n, 0x10ad0376, "EF1"}, // 102
                                                      {NO_BANK_SWITCH, 0xB000, 0x1000, s_romData2nBSet[2].data2n, 0x8a1430ee, "H1 "}, // 103
                                                      {NO_BANK_SWITCH, 0xC000, 0x1000, s_romData2nBSet[3].data2n, 0x44f9943f, "J1 "}, // 104
                                                      {NO_BANK_SWITCH, 0xD000, 0x1000, s_romData2nBSet[4].data2n, 0x1fdf801c, "KL1"}, // 105
                                                      {NO_BANK_SWITCH, 0xE000, 0x1000, s_romData2nBSet[5].data2n, 0xccc9b26c, "M1 "}, // 106
                                                      {NO_BANK_SWITCH, 0xF000, 0x1000, s_romData2nBSet[5].data2n, 0xccc9b26c, "M1 "}, // 106 mirror
                                                      {0} }; // end of list


IGame*
CGravitarGame::createInstanceGravitarSet1(
)
{
    return (new CGravitarGame(s_romData2nGSet1, s_romRegionGSet1));
}

IGame*
CGravitarGame::createInstanceGravitarSet2(
)
{
    return (new CGravitarGame(s_romData2nGSet2, s_romRegionGSet2));
}

IGame*
CGravitarGame::createInstanceGravitarSet3(
)
{
    return (new CGravitarGame(s_romData2nGSet3, s_romRegionGSet3));
}

IGame*
CGravitarGame::createInstanceBlackWidowSet(
)
{
    return (new CGravitarGame(s_romData2nBSet, s_romRegionBSet));
}

CGravitarGame::CGravitarGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGravitarBaseGame(romData2n, romRegion)
{
}


