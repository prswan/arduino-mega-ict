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
#include "CZaxxonGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Set 1 from MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800 1000
                                                       {0xed,0x56,0x89,0x3a,0x3a,0x21,0xf3,0x3a,0xd1,0x02,0x05,0xfa,0x21}, // 3 - U27
                                                       {0xaf,0x77,0x03,0x28,0x02,0x07,0x21,0xff,0xe6,0x28,0x28,0x01,0x3c}, // 2 - U28
                                                       {0xef,0x0a,0x2e,0x3a,0x7e,0x32,0x00,0x00,0xc0,0x18,0x09,0x47},      // 1 - U29
                                                       {0} };  // end of list

//
// Set 1 from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSet1[0].data2n, 0x6e2b4a30, "c27"}, // zaxxon3.u27
                                                      {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSet1[1].data2n, 0x1c9ea398, "c28"}, // zaxxon2.u28
                                                      {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nSet1[2].data2n, 0x1c123ef9, "c29"}, // zaxxon1.u29
                                                      {0} }; // end of list

//
// Set 2 from MAME.
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSet1[0].data2n, 0xb18e428a, "c27"}, // zaxxon3.u27
                                                      {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSet1[1].data2n, 0x1c9ea398, "c28"}, // zaxxon2.u28
                                                      {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nSet1[2].data2n, 0x1977d933, "c29"}, // zaxxon1.u29
                                                      {0} }; // end of list

//
// Set 3 from MAME.
//
static const ROM_DATA2N s_romData2nSet3[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800 1000
                                                       {0xed,0x56,0x89,0x3a,0x3a,0x21,0xf3,0x3a,0xd1,0x02,0x05,0xfa,0x46}, // 3 - U27
                                                       {0x03,0xdd,0x11,0x1f,0x18,0xe1,0x00,0xfe,0x10,0x3c,0x0e,0xc0,0x3d}, // 2 - U28
                                                       {0x23,0x3a,0x62,0xe7,0x40,0x28,0x23,0x60,0x00,0x78,0x22,0xee},      // 1 - U29
                                                       {0} };  // end of list
//
// Set 3 from MAME.
//
static const ROM_REGION s_romRegionSet3[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSet3[0].data2n, 0x2f2f2b7c, "c27"}, // zaxxon3.u27
                                                      {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nSet3[1].data2n, 0xae7e1c38, "c28"}, // zaxxon2.u28
                                                      {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nSet3[2].data2n, 0xcc67c097, "c29"}, // zaxxon1.u29
                                                      {0} }; // end of list

IGame*
CZaxxonGame::createInstanceSet1(
)
{
    return (new CZaxxonGame(s_romData2nSet1, s_romRegionSet1));
}


IGame*
CZaxxonGame::createInstanceSet2(
)
{
    return (new CZaxxonGame(s_romData2nSet1, s_romRegionSet2));
}


IGame*
CZaxxonGame::createInstanceSet3(
)
{
    return (new CZaxxonGame(s_romData2nSet3, s_romRegionSet3));
}


CZaxxonGame::CZaxxonGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CZaxxonBaseGame(romData2n, romRegion)
{
}


