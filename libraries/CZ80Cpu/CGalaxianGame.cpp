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
#include "CGalaxianGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Set 1 "galaxianm" from MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0x32,0x01,0xc3,0x3a,0x77,0x85,0x87,0x80,0x32,0x21,0x08},//
                                                       {0x11,0x03,0xcd,0x00,0xca,0x32,0x3a,0x65,0x2c,0x04,0x0c},//
                                                       {0x35,0x02,0x3a,0x01,0x09,0x6e,0xc0,0x03,0x7e,0x77,0x15},//
                                                       {0x7e,0xe6,0xc2,0x60,0x30,0x41,0x41,0x20,0x2f,0x1a,0xc0},//
                                                       {0xa2,0x40,0x1e,0x10,0x7e,0x2c,0x20,0xcb,0x50,0x30,0x3f},//
                                                       {0} };  // end of list

//
// Set 1 "galaxianm" from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet1[0].data2n, 0x745e2d61, " rU"}, // galmidw.u
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet1[1].data2n, 0x9c999a40, " rV"}, // galmidw.v
                                                      {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSet1[2].data2n, 0xb5894925, " rW"}, // galmidw.w
                                                      {NO_BANK_SWITCH, 0x1800, 0x0800, s_romData2nSet1[3].data2n, 0x6b3ca10b, " rY"}, // galmidw.y
                                                      {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nSet1[4].data2n, 0xcb24f797, " rZ"}, // galmidw.z
                                                      {0} }; // end of list


//
// Set 2 "galaxianmo" from MAME.
//
static const ROM_DATA2N s_romData2nSet2[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0x32,0x01,0xc3,0x3a,0x77,0x85,0xFF,0x80,0x32,0x21,0x08},//
                                                       {0x11,0x03,0xcd,0x00,0xca,0x32,0x3a,0x65,0x2c,0x04,0x04},//
                                                       {0x35,0x09,0xa5,0x13,0x77,0x2c,0x05,0x62,0x04,0xe8,0x0F},//
                                                       {0x3d,0x32,0x41,0xc2,0x0f,0x0f,0xc0,0x8a,0x2c,0xc6,0x21},//
                                                       {0xa2,0x40,0x1e,0x10,0x7e,0x2c,0x20,0xcb,0x50,0xad,0x52},//
                                                       {0} };  // end of list

//
// Set 2 "galaxianmo" from MAME.
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet2[0].data2n, 0xfac42d34, " rU"}, // galaxian.u
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet2[1].data2n, 0xf58283e3, " rV"}, // galaxian.v
                                                      {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSet2[2].data2n, 0x4c7031c0, " rW"}, // galaxian.w
                                                      {NO_BANK_SWITCH, 0x1800, 0x0800, s_romData2nSet2[3].data2n, 0x96a7ac94, " rY"}, // galaxian.y
                                                      {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nSet2[4].data2n, 0x5341d75a, " rZ"}, // galaxian.z
                                                      {0} }; // end of list

IGame*
CGalaxianGame::createInstanceSet1(
)
{
    return (new CGalaxianGame(s_romData2nSet1, s_romRegionSet1));
}


IGame*
CGalaxianGame::createInstanceSet2(
)
{
    return (new CGalaxianGame(s_romData2nSet2, s_romRegionSet2));
}


CGalaxianGame::CGalaxianGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGalaxianBaseGame(romData2n, romRegion)
{
}


