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
#include "CCatnMouseGame.h"


//
// ROM Memory Map
//   The ROM memory map is a bit strange in that the 2Kb ROMS are split addressed.
//   This causes some complications with MAME because MAME calculates CRC's accross the image and
//   the tester calculates the CRC based on contigiously addressed block. Therefore, we need to split
//   the images and recalculate the individual CRC's of the two halves. HxD can be used to do this.
//

//
// Set 01 - MAME catnmousa
//
static const ROM_DATA2N s_romData2nSet01[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200
                                                        {0x01,0xf1,0x91,0x87,0xea,0xe0,0x15,0x24,0x0f,0x01}, //  7C_0
                                                        {0x1a,0xe2,0x04,0x1d,0xe0,0x08,0xe2,0x1c,0x1c,0x5e}, //  6C_0
                                                        {0x6e,0x0f,0x55,0x09,0x64,0x06,0x07,0x0e,0xe2,0x01}, //  5C_0
                                                        {0x1a,0x13,0x0c,0x19,0x85,0x0f,0x7c,0x7c,0x0e,0x0e}, //  3C_0
                                                        {0xe4,0x01,0x3b,0x46,0xad,0xbb,0x15,0xaf,0x98,0x15}, //  2C_0
                                                        {0x1c,0x22,0x25,0xc1,0x09,0x05,0x04,0x06,0x02,0x15}, //  7B_0
                                                        {0xbb,0xab,0x02,0x0f,0x5f,0x1c,0xcd,0xc3,0x86,0x1c}, //  6B_0
                                                        {0x28,0xe9,0xf3,0xf3,0x43,0xa5,0xd9,0x05,0x00,0x00}, //  5B_0
                                                        {0x7e,0x2c,0x2c,0x2d,0x2d,0x6d,0x6e,0x2d,0x3c,0x00}, //  3B_0
                                                        {0xc1,0xd4,0xd3,0x01,0x01,0x01,0x01,0x7b,0x3b,0x00}, //  2B_2
                                                        {0x1d,0x1d,0x1d,0x1e,0xc3,0x79,0x7c,0x1d,0x04,0x82}, //  2B_0
                                                        {0x10,0xcc,0x4e,0x4f,0x41,0x6c,0xa4,0xc8,0xec,0x15}, //  7C_1
                                                        {0x1c,0xe7,0x1c,0x44,0x25,0x61,0x7c,0x33,0x40,0x7c}, //  6C_1
                                                        {0x00,0xfd,0xfd,0x03,0xba,0xfc,0xfe,0x17,0x58,0xcc}, //  5C_1
                                                        {0xad,0x00,0x31,0x55,0x05,0x0f,0x08,0x4b,0x20,0x04}, //  3C_1
                                                        {0x19,0x1e,0x17,0x45,0x17,0x61,0x17,0x2a,0x1d,0x39}, //  2C_1
                                                        {0x15,0xad,0x01,0x02,0x1c,0x1c,0x1c,0x1c,0x0f,0x9a}, //  7B_1
                                                        {0xfa,0x65,0x65,0x65,0x66,0x2d,0x2d,0x29,0x10,0x18}, //  6B_1
                                                        {0xbb,0xcc,0xbf,0xa4,0xcc,0xcc,0x1a,0x17,0xc0,0x1e}, //  5B_1
                                                        {0x1b,0x76,0x6c,0xbc,0xb9,0xcc,0x18,0x6a,0x18,0x00}, //  3B_1
                                                        {0x0c,0x15,0x84,0x5a,0x0d,0x1c,0xcc,0x20,0xf5,0x81}, //  2B_3
                                                        {0x05,0x0e,0x1b,0x1a,0x0e,0x84,0xf4,0x0e,0xc1,0x7a}, //  2B_1
                                                        {0} };  // end of list

//
// Set 01
//
static const ROM_REGION s_romRegionSet01[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2nSet01[ 0].data2n, 0x9181eb19, "7C0"},//
                                                       {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2nSet01[ 1].data2n, 0xd7608c38, "6C0"},//
                                                       {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2nSet01[ 2].data2n, 0x56b6857d, "5C0"},//
                                                       {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2nSet01[ 3].data2n, 0x008d4eb5, "3C0"},//
                                                       {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2nSet01[ 4].data2n, 0x93715efd, "2C0"},//
                                                       {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2nSet01[ 5].data2n, 0xaca82a94, "7B0"},//
                                                       {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2nSet01[ 6].data2n, 0xfda562ac, "6B0"},//
                                                       {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2nSet01[ 7].data2n, 0xf5629561, "5B0"},//
                                                       {NO_BANK_SWITCH, 0x2C00, 0x0400, s_romData2nSet01[ 8].data2n, 0xf9fb88d3, "3B0"},//
                                                       {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2nSet01[ 9].data2n, 0xe4e07329, "2B2"},//
                                                       {NO_BANK_SWITCH, 0x3800, 0x0400, s_romData2nSet01[10].data2n, 0x00a7b3a1, "2B0"},//
                                                       {NO_BANK_SWITCH, 0x4000, 0x0400, s_romData2nSet01[11].data2n, 0xc1c0bedc, "7C1"},//
                                                       {NO_BANK_SWITCH, 0x4400, 0x0400, s_romData2nSet01[12].data2n, 0x34060de2, "6C1"},//
                                                       {NO_BANK_SWITCH, 0x4800, 0x0400, s_romData2nSet01[13].data2n, 0x9030dbc2, "5C1"},//
                                                       {NO_BANK_SWITCH, 0x4c00, 0x0400, s_romData2nSet01[14].data2n, 0x6f8f29c6, "3C1"},//
                                                       {NO_BANK_SWITCH, 0x5000, 0x0400, s_romData2nSet01[15].data2n, 0x28809997, "2C1"},//
                                                       {NO_BANK_SWITCH, 0x6000, 0x0400, s_romData2nSet01[16].data2n, 0x4fae93ae, "7B1"},//
                                                       {NO_BANK_SWITCH, 0x6400, 0x0400, s_romData2nSet01[17].data2n, 0xb6b06595, "6B1"},//
                                                       {NO_BANK_SWITCH, 0x6800, 0x0400, s_romData2nSet01[18].data2n, 0xce7fae3b, "5B1"},//
                                                       {NO_BANK_SWITCH, 0x6C00, 0x0400, s_romData2nSet01[19].data2n, 0xa73c239c, "3B1"},//
                                                       {NO_BANK_SWITCH, 0x7000, 0x0400, s_romData2nSet01[20].data2n, 0x7e76abd0, "2B3"},//
                                                       {NO_BANK_SWITCH, 0x7800, 0x0400, s_romData2nSet01[21].data2n, 0x676e80c8, "2B1"},//
                                                       {0} }; // end of list


//
// Set 02 - MAME catnmous
//
static const ROM_DATA2N s_romData2nSet02[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200
                                                        {0x02,0x00,0xba,0xb0,0xff,0xee,0x15,0x24,0x0f,0x04}, //  7C_0
                                                        {0x0c,0x1c,0xe2,0x08,0x1f,0x0f,0x1a,0x84,0x17,0x1f}, //  6C_0
                                                        {0x74,0x17,0x09,0x18,0x9c,0x07,0x20,0x20,0x0d,0x09}, //  5C_0
                                                        {0xbb,0xe4,0x9c,0x7c,0x01,0x06,0x0c,0x65,0x0f,0x04}, //  3C_0
                                                        {0xcc,0x1c,0xbb,0x8a,0x7c,0x33,0x0c,0xaf,0x70,0x02}, //  2C_0
                                                        {0x1c,0x22,0x25,0xc1,0x09,0x05,0x04,0x06,0x02,0x5d}, //  7B_0
                                                        {0x1f,0x24,0x47,0x05,0x17,0x1c,0x5e,0x5e,0x08,0x1c}, //  6B_0
                                                        {0x0d,0xce,0xa0,0xce,0x28,0x29,0x29,0x2b,0x00,0x00}, //  5B_0
                                                        {0x2c,0xcb,0xd5,0xe9,0x7f,0x25,0xc8,0xcb,0x00,0x00}, //  3B_0
                                                        {0xcb,0xc0,0x1c,0xb0,0xc8,0xb0,0xce,0xa4,0xb0,0x00}, //  2B_2
                                                        {0x1d,0x1d,0x1d,0x1e,0xc3,0x7a,0x7c,0xce,0x0d,0x26}, //  2B_0
                                                        {0x10,0xcc,0x4e,0x4f,0x42,0x7c,0xa4,0xc8,0xeb,0x41}, //  7C_1
                                                        {0x05,0x48,0x06,0x02,0xcc,0x03,0xf2,0x13,0x0c,0x7c}, //  6C_1
                                                        {0x00,0x03,0x03,0xfd,0xfd,0x0f,0x1c,0x0d,0x81,0x1a}, //  5C_1
                                                        {0x1c,0x4b,0xc8,0x0c,0xc8,0x04,0xc8,0x0f,0x98,0x04}, //  3C_1
                                                        {0x19,0x1e,0x17,0x45,0x17,0x61,0x17,0x3b,0xad,0xc8}, //  2C_1
                                                        {0x15,0xad,0x01,0x02,0x1c,0x1c,0x1c,0x1c,0x0f,0x7c}, //  7B_1
                                                        {0x64,0xdb,0xe5,0xf9,0x21,0x4d,0x9d,0x08,0xc3,0x04}, //  6B_1
                                                        {0x26,0x9a,0x04,0xcc,0xc3,0xd2,0x00,0x1a,0x7c,0x84}, //  5B_1
                                                        {0x4d,0x1f,0xb8,0x08,0x16,0x1b,0x00,0x78,0xa8,0x00}, //  3B_1
                                                        {0x02,0xcc,0x5a,0xe7,0x08,0xd0,0x9d,0xbb,0x97,0x00}, //  2B_3
                                                        {0x7b,0xa0,0x08,0x18,0x5c,0x84,0x12,0xd2,0x36,0x80}, //  2B_1
                                                        {0} };  // end of list

//
// Set 02
//
static const ROM_REGION s_romRegionSet02[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2nSet02[ 0].data2n, 0x92df3655, "7C0"},//
                                                       {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2nSet02[ 1].data2n, 0x8f413318, "6C0"},//
                                                       {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2nSet02[ 2].data2n, 0x9d95c66b, "5C0"},//
                                                       {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2nSet02[ 3].data2n, 0x09161951, "3C0"},//
                                                       {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2nSet02[ 4].data2n, 0x76cc583e, "2C0"},//
                                                       {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2nSet02[ 5].data2n, 0x058d7e55, "7B0"},//
                                                       {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2nSet02[ 6].data2n, 0x1a2349b7, "6B0"},//
                                                       {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2nSet02[ 7].data2n, 0x55eb7a5a, "5B0"},//
                                                       {NO_BANK_SWITCH, 0x2C00, 0x0400, s_romData2nSet02[ 8].data2n, 0x9fb238f0, "3B0"},//
                                                       {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2nSet02[ 9].data2n, 0x4a86d4a6, "2B2"},//
                                                       {NO_BANK_SWITCH, 0x3800, 0x0400, s_romData2nSet02[10].data2n, 0x91742b83, "2B0"},//
                                                       {NO_BANK_SWITCH, 0x4000, 0x0400, s_romData2nSet02[11].data2n, 0x3b7e82cb, "7C1"},//
                                                       {NO_BANK_SWITCH, 0x4400, 0x0400, s_romData2nSet02[12].data2n, 0x67b0c5ce, "6C1"},//
                                                       {NO_BANK_SWITCH, 0x4800, 0x0400, s_romData2nSet02[13].data2n, 0x76eab655, "5C1"},//
                                                       {NO_BANK_SWITCH, 0x4c00, 0x0400, s_romData2nSet02[14].data2n, 0x4d7dc710, "3C1"},//
                                                       {NO_BANK_SWITCH, 0x5000, 0x0400, s_romData2nSet02[15].data2n, 0x2bb74c69, "2C1"},//
                                                       {NO_BANK_SWITCH, 0x6000, 0x0400, s_romData2nSet02[16].data2n, 0x406848b0, "7B1"},//
                                                       {NO_BANK_SWITCH, 0x6400, 0x0400, s_romData2nSet02[17].data2n, 0x671f0fbc, "6B1"},//
                                                       {NO_BANK_SWITCH, 0x6800, 0x0400, s_romData2nSet02[18].data2n, 0x507c1077, "5B1"},//
                                                       {NO_BANK_SWITCH, 0x6C00, 0x0400, s_romData2nSet02[19].data2n, 0x2613ce75, "3B1"},//
                                                       {NO_BANK_SWITCH, 0x7000, 0x0400, s_romData2nSet02[20].data2n, 0xb1bb6a67, "2B3"},//
                                                       {NO_BANK_SWITCH, 0x7800, 0x0400, s_romData2nSet02[21].data2n, 0xc46c87d9, "2B1"},//
                                                       {0} }; // end of list

IGame*
CCatnMouseGame::createInstanceSet01(
)
{
    return (new CCatnMouseGame(s_romData2nSet01, s_romRegionSet01));
}

IGame*
CCatnMouseGame::createInstanceSet02(
)
{
    return (new CCatnMouseGame(s_romData2nSet02, s_romRegionSet02));
}

CCatnMouseGame::CCatnMouseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CLaserBattleBaseGame( romData2n,
                          romRegion )
{
}


