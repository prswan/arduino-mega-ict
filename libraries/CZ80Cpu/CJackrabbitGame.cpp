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
#include "CJackrabbitGame.h"

//
// See the base game cpp comments for details about this platform.
//
// ROM Memory Map
//   The ROM memory map is a bit strange in that the 8Kb ROMS are split addressed:
//      Lo 4Kb - 0x0000
//      Hi 4Kb - 0x8000
//   This causes some complications with MAME because MAME calculates CRC's accross the image and
//   the tester calculates the CRC based on contigiously addressed block. Therefore, we need to split
//   the images and recalculate the individual CRC's of the two halves. HxD can be used to do this.
//

//
// Set 1 from MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x07,0x6c,0x00,0x04,0xf9,0xfa,0x08,0x20,0x73,0x48,0x8b,0x8b},// 1A Lo
                                                       {0x02,0x02,0x02,0x02,0x08,0x02,0x08,0x08,0x08,0x02,0x02,0x02},// 2L
                                                       {0x3f,0x5e,0x43,0xa9,0xab,0xb1,0xa8,0xef,0xd4,0xa7,0xb7,0x08},// 3L
                                                       {0x00,0x0e,0x06,0x19,0x23,0x69,0x73,0xd2,0x73,0x06,0x62,0x32},// 4L
                                                       {0x21,0x2b,0x34,0x23,0x74,0x3a,0xcb,0x23,0x72,0xcd,0xaf,0xbe},// 5L
                                                       {0xcd,0x84,0x3e,0x72,0x47,0x72,0x3a,0x84,0xbe,0x31,0x93,0xa9},// 6L
                                                       {0x9f,0x76,0xc2,0xf9,0xd2,0x00,0xf2,0x32,0xca,0x3a,0x04,0x0c},// 1A Hi
                                                       {0x10,0x09,0x04,0x07,0x1a,0x0b,0x1d,0x17,0x18,0x37,0x00,0xb8},// 2H
                                                       {0x28,0x31,0x47,0x33,0x34,0x36,0x3b,0x42,0x53,0x72,0x07,0x4f},// 3H
                                                       {0x0a,0xb0,0x80,0x0e,0x08,0x02,0xcd,0x02,0x0b,0xff,0xa8,0xe1},// 4H
                                                       {0xc1,0x29,0x4a,0x08,0x08,0x90,0x94,0x70,0x02,0xfe,0xfe,0xda},// 5H
                                                       {0xff,0xff,0xff,0xf9,0x40,0x1c,0x3a,0x46,0x9a,0x70,0x86,0x32},// 6H
                                                       {0} };  // end of list

//
// Set 1 from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSet1[ 0].data2n, 0x6072B10B, "c1A"},
                                                      {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSet1[ 1].data2n, 0x4772e557, "r1B"},
                                                      {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSet1[ 2].data2n, 0x1e844228, "r1C"},
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nSet1[ 3].data2n, 0xebffcc38, "r1D"},
                                                      {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nSet1[ 4].data2n, 0x275e0ed6, "r2A"},
                                                      {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nSet1[ 5].data2n, 0x8a20977a, "r2C"},
                                                      {NO_BANK_SWITCH, 0x8000, 0x1000, s_romData2nSet1[ 6].data2n, 0xE0BD39F3, "c1A"},
                                                      {NO_BANK_SWITCH, 0x9000, 0x1000, s_romData2nSet1[ 7].data2n, 0x21f2be2a, "r1B"},
                                                      {NO_BANK_SWITCH, 0xa000, 0x1000, s_romData2nSet1[ 8].data2n, 0x59077027, "r1C"},
                                                      {NO_BANK_SWITCH, 0xb000, 0x1000, s_romData2nSet1[ 9].data2n, 0x0b9db007, "r1D"},
                                                      {NO_BANK_SWITCH, 0xc000, 0x1000, s_romData2nSet1[10].data2n, 0x785e1a01, "r2A"},
                                                      {NO_BANK_SWITCH, 0xd000, 0x1000, s_romData2nSet1[11].data2n, 0xdd5979cf, "r2C"},
                                                      {0} }; // end of list

//
// Set 2 from MAME.
// Some of the regions are the same as Set 1 but repeated here to reflect the different naming convention.
//
static const ROM_DATA2N s_romData2nSet2[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x07,0x6c,0x00,0x04,0xf9,0xfa,0x08,0x20,0x73,0x48,0x8b,0x8b},// 1A Lo == Set 1
                                                       {0x02,0x02,0x02,0x02,0x08,0x02,0x08,0x08,0x08,0x02,0x02,0x02},// 2L    == Set 1
                                                       {0x3f,0x5e,0x43,0xa9,0xab,0xb1,0xa8,0xef,0xd4,0xa7,0xb7,0x08},// 3L    == Set 1
                                                       {0x00,0x0e,0x06,0x19,0x23,0x69,0x73,0xd2,0x73,0x06,0x62,0x32},// 4L    == Set 1
                                                       {0x21,0x2b,0x34,0x23,0x74,0x3a,0xcb,0x23,0x72,0xcd,0xaf,0xbe},// 5L    == Set 1
                                                       {0xcd,0x84,0x3e,0x72,0x47,0x72,0x3a,0x84,0xbe,0x31,0x93,0xca},// 6L
                                                       {0x9f,0x76,0xc2,0xf9,0xd2,0x00,0x13,0x76,0xcd,0xc2,0x32,0x70},// 1A Hi
                                                       {0x00,0x0b,0x12,0x1a,0x09,0x00,0x0a,0x07,0x01,0x23,0x00,0x98},// 2H
                                                       {0x2e,0x24,0x22,0x50,0x51,0x53,0x57,0x5f,0x6f,0x0b,0x00,0x47},// 3H
                                                       {0xb0,0x67,0x78,0xf1,0x70,0x7e,0xfe,0xb0,0x2f,0x02,0x04,0x28},// 4H
                                                       {0xcd,0xc0,0xc0,0xc0,0xc0,0xc1,0xc1,0xc2,0x03,0x01,0x01,0x42},// 5H
                                                       {0x09,0xd1,0xd1,0xd1,0xff,0xff,0xff,0x08,0x4f,0xd2,0x0e,0x02},// 6H
                                                       {0} };  // end of list

//
// Set 2 from MAME.
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSet2[ 0].data2n, 0x6072B10B, "c1A"},// == Set 1
                                                      {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSet2[ 1].data2n, 0x4772e557, "r1B"},// == Set 1
                                                      {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSet2[ 2].data2n, 0x1e844228, "r1C"},// == Set 1
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nSet2[ 3].data2n, 0x40BC1D5E, "r1D"},//
                                                      {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nSet2[ 4].data2n, 0xB5BDE0FA, "r2A"},//
                                                      {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nSet2[ 5].data2n, 0x76DF1C96, "r2C"},//
                                                      {NO_BANK_SWITCH, 0x8000, 0x1000, s_romData2nSet2[ 6].data2n, 0x50148677, "r1A"},//
                                                      {NO_BANK_SWITCH, 0x9000, 0x1000, s_romData2nSet2[ 7].data2n, 0x310BC6F9, "r1B"},//
                                                      {NO_BANK_SWITCH, 0xa000, 0x1000, s_romData2nSet2[ 8].data2n, 0xD0357C92, "r1C"},//
                                                      {NO_BANK_SWITCH, 0xb000, 0x1000, s_romData2nSet2[ 9].data2n, 0x5E5D06AA, "r1D"},//
                                                      {NO_BANK_SWITCH, 0xc000, 0x1000, s_romData2nSet2[10].data2n, 0xBEE5AED2, "r2A"},//
                                                      {NO_BANK_SWITCH, 0xd000, 0x1000, s_romData2nSet2[11].data2n, 0xA27A1F88, "r2C"},//
                                                      {0} }; // end of list

IGame*
CJackrabbitGame::createInstanceSet1(
)
{
    return (new CJackrabbitGame(s_romData2nSet1, s_romRegionSet1));
}

IGame*
CJackrabbitGame::createInstanceSet2(
)
{
    return (new CJackrabbitGame(s_romData2nSet2, s_romRegionSet2));
}

CJackrabbitGame::CJackrabbitGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CMoneyMoneyBaseGame(romData2n, romRegion)
{
}


