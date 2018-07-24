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
#include "CBuckRogersGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Encrypted Set 1 from MAME.
//
static const ROM_DATA2N s_romData2nEncSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800 1000 2000
                                                          {0x4d,0x5e,0x88,0x8e,0x50,0x00,0x5b,0x2c,0x45,0x17,0x17,0xc1,0x9a,0xdf}, // IC3
                                                          {0x68,0xba,0x70,0x1e,0x51,0x87,0xc0,0xbc,0xde,0xc6,0x9e,0x8c,0x7d,0xa8}, // IC4
                                                          {0} };  // end of list
//
// Encrypted Set 1 from MAME.
//
static const ROM_REGION s_romRegionEncSet1[] PROGMEM = { //
                                                         {NO_BANK_SWITCH, 0x0000, 0x4000, s_romData2nEncSet1[0].data2n, 0xf0055e97, "c3 "}, // epr-5265.cpu-ic3
                                                         {NO_BANK_SWITCH, 0x4000, 0x4000, s_romData2nEncSet1[1].data2n, 0x7d084c39, "c4 "}, // epr-5266.cpu-ic4
                                                         {0} }; // end of list

//
// Set 1 from MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800 1000 2000
                                                       {0xed,0x56,0x00,0x06,0xf8,0xff,0xff,0xff,0xcd,0x97,0x97,0xe1,0x32,0x77}, // IC3
                                                       {0x40,0x3a,0xf8,0x16,0x79,0x0f,0x48,0x3c,0x7e,0x4e,0x16,0x04,0xdd,0x80}, // IC4
                                                       {0} };  // end of list
//
// Set 1 from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x4000, s_romData2nSet1[0].data2n, 0x7f1910af, "c3 "}, // cpu-ic3.bin
                                                      {NO_BANK_SWITCH, 0x4000, 0x4000, s_romData2nSet1[1].data2n, 0x5ecd393b, "c4 "}, // cpu-ic4.bin
                                                      {0} }; // end of list

//
// Set 2 from MAME.
//
static const ROM_DATA2N s_romData2nSet2[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800 1000 2000
                                                       {0xed,0x56,0x00,0x05,0xf8,0xff,0xff,0xff,0xcd,0x06,0x01,0x30,0xd9,0xb9}, // IC3
                                                       {0x28,0x01,0x32,0xcb,0x20,0x3a,0x06,0x01,0x23,0x49,0x45,0xc2,0x41,0x80}, // IC4
                                                       {0} };  // end of list
//
// Set 2 from MAME.
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x4000, s_romData2nSet2[0].data2n, 0xb18e428a, "c3 "}, // epr-5204.cpu-ic3
                                                      {NO_BANK_SWITCH, 0x4000, 0x4000, s_romData2nSet2[1].data2n, 0x1c9ea398, "c4 "}, // epr-5205.cpu-ic4
                                                      {0} }; // end of list


IGame*
CBuckRogersGame::createInstanceEncSet1(
)
{
    return (new CBuckRogersGame(s_romData2nEncSet1, s_romRegionEncSet1));
}


IGame*
CBuckRogersGame::createInstanceSet1(
)
{
    return (new CBuckRogersGame(s_romData2nSet1, s_romRegionSet1));
}


IGame*
CBuckRogersGame::createInstanceSet2(
)
{
    return (new CBuckRogersGame(s_romData2nSet2, s_romRegionSet2));
}


CBuckRogersGame::CBuckRogersGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CBuckRogersBaseGame(romData2n, romRegion)
{
}


