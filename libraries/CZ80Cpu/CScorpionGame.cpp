//
// Copyright (c) 2021, Paul R. Swan
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
#include "CScorpionGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// "scorpion" Set 1 from MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x32,0x01,0x00,0x77,0x7b,0x77,0xcb,0x32,0x81,0x11,0xef,0xb5}, // 1 - 2D
                                                       {0x0f,0xd5,0xa7,0x28,0x10,0x32,0xf8,0xcb,0xcb,0x77,0x23,0xfe}, // 2 - 2F
                                                       {0xdc,0xcb,0x20,0x00,0xcb,0xd0,0x03,0xfd,0x3a,0xdd,0x10,0xfe}, // 3 - 2G
                                                       {0xc9,0xdd,0x03,0x7e,0xdd,0x47,0x2f,0xb3,0x41,0x49,0x02,0xd3}, // 4 - 2H
                                                       {0xc5,0xd5,0x08,0xd5,0x01,0x5b,0x06,0x2a,0xcb,0x32,0xa7},      // 5 - 2K - hi
                                                       {0xb0,0xfd,0x00,0xfd,0x00,0x3d,0x29,0x00,0xc3,0xed,0x40},      // 5 - 2K - lo
                                                       {0} };  // end of list

//
// "scorpion" Set 1 from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSet1[0].data2n, 0xba1219b4, " 2D"}, // 1.2d
                                                      {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSet1[1].data2n, 0xc3909ab6, " 2F"}, // 2.2f
                                                      {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSet1[2].data2n, 0x43261352, " 2G"}, // 3.2g
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nSet1[3].data2n, 0xaba2276a, " 2H"}, // 4.2h
                                                      {NO_BANK_SWITCH, 0x5800, 0x0800, s_romData2nSet1[4].data2n, 0xe7a572d7, " 2K"}, // 5.2k - hi
                                                      {NO_BANK_SWITCH, 0x6000, 0x0800, s_romData2nSet1[5].data2n, 0x22e2d15a, " 2K"}, // 5.2k - lo
                                                      {0} }; // end of list

//
// "scorpiona" Set 2 from MAME.
//
static const ROM_DATA2N s_romData2nSet2[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x32,0x01,0x00,0x77,0x7b,0x77,0xcb,0x32,0x81,0x11,0xef,0xb5}, // 1 - 2D
                                                       {0x30,0x09,0xc9,0xfd,0xfd,0x10,0xfd,0x4f,0xcb,0xf0,0x9c,0x00}, // 2 - 2E
                                                       {0xcb,0x61,0x23,0x0c,0xfe,0x2c,0xc1,0x12,0xa5,0xcb,0xc8,0x7e}, // 3 - 2G
                                                       {0x47,0xe6,0xdd,0x20,0x87,0x2f,0x03,0x20,0x80,0x54,0x7a,0x01}, // 4 - 2H
                                                       {0xc5,0xd5,0x08,0xd5,0x01,0x5b,0x01,0x32,0xf4,0x20,0x18},      // 5 - 2K
                                                       {0x42,0xed,0xcb,0x22,0x28,0x53,0xe6,0x70,0x00,0x21,0x10},      // 5 - 2L
                                                       {0} };  // end of list

//
// "scorpiona" Set 2 from MAME.
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSet2[0].data2n, 0xc5b9daeb, " 2D"}, // scor_d2.bin
                                                      {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSet2[1].data2n, 0x82308d05, " 2E"}, // scor_e2.bin
                                                      {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSet2[2].data2n, 0x756b09cd, " 2G"}, // scor_g2.bin
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nSet2[3].data2n, 0x667ad8be, " 2H"}, // igr_scorpion_32_h2.ic112
                                                      {NO_BANK_SWITCH, 0x5800, 0x0800, s_romData2nSet2[4].data2n, 0x42ec34d8, " 2K"}, // scor_k2.bin
                                                      {NO_BANK_SWITCH, 0x6000, 0x0800, s_romData2nSet2[5].data2n, 0x6623da33, " 2L"}, // scor_l2.bin
                                                      {0} }; // end of list

//
// Single ROM difference found on PCB
//
static const ROM_REGION s_romRegionSet2A[] PROGMEM = { //
                                                       {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSet2[0].data2n, 0xc5b9daeb, " 2D"}, // scor_d2.bin
                                                       {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSet2[1].data2n, 0x82308d05, " 2E"}, // scor_e2.bin
                                                       {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSet2[2].data2n, 0x756b09cd, " 2G"}, // scor_g2.bin
                                                       {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nSet2[3].data2n, 0x667ad8be, " 2H"}, // igr_scorpion_32_h2.ic112
                                                       {NO_BANK_SWITCH, 0x5800, 0x0800, s_romData2nSet2[4].data2n, 0x42ec34d8, " 2K"}, // scor_k2.bin
                                                       {NO_BANK_SWITCH, 0x6000, 0x0800, s_romData2nSet2[5].data2n, 0x8b9bfa2d, " 2L"}, // SCO_C2A on PCB
                                                       {0} }; // end of list

IGame*
CScorpionGame::createInstanceSet1(
)
{
    return (new CScorpionGame(s_romData2nSet1, s_romRegionSet1));
}

IGame*
CScorpionGame::createInstanceSet2(
)
{
    return (new CScorpionGame(s_romData2nSet2, s_romRegionSet2));
}

IGame*
CScorpionGame::createInstanceSet2A(
)
{
    return (new CScorpionGame(s_romData2nSet2, s_romRegionSet2A));
}

CScorpionGame::CScorpionGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CScrambleBaseGame( SCRAMBLE, romData2n, romRegion )
{
}


