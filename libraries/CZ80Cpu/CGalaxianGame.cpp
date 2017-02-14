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
// Set 1 from MAME.
//                                              01   02   04   08   10   20   40   80  100  200  400
static const UINT16 s_romData2nSet1_rU[]    = {0x32,0x01,0xc3,0x3a,0x77,0x85,0x87,0x80,0x32,0x21,0x08};//
static const UINT16 s_romData2nSet1_rV[]    = {0x11,0x03,0xcd,0x00,0xca,0x32,0x3a,0x65,0x2c,0x04,0x0c};//
static const UINT16 s_romData2nSet1_rW[]    = {0x35,0x02,0x3a,0x01,0x09,0x6e,0xc0,0x03,0x7e,0x77,0x15};//
static const UINT16 s_romData2nSet1_rY[]    = {0x7e,0xe6,0xc2,0x60,0x30,0x41,0x41,0x20,0x2f,0x1a,0xc0};//
static const UINT16 s_romData2nSet1_rZ[]    = {0xa2,0x40,0x1e,0x10,0x7e,0x2c,0x20,0xcb,0x50,0x30,0x3f};//

//
// Set 1 from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet1_rU, 0x745e2d61, " rU"}, // galmidw.u
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet1_rV, 0x9c999a40, " rV"}, // galmidw.v
                                                      {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSet1_rW, 0xb5894925, " rW"}, // galmidw.w
                                                      {NO_BANK_SWITCH, 0x1800, 0x0800, s_romData2nSet1_rY, 0x6b3ca10b, " rY"}, // galmidw.y
                                                      {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nSet1_rZ, 0xcb24f797, " rZ"}, // galmidw.z
                                                      {0} }; // end of list


//
// Set 2 from MAME.
//                                              01   02   04   08   10   20   40   80  100  200  400
static const UINT16 s_romData2nSet2_rU[]    = {0x32,0x01,0xc3,0x3a,0x77,0x85,0xFF,0x80,0x32,0x21,0x08};//
static const UINT16 s_romData2nSet2_rV[]    = {0x11,0x03,0xcd,0x00,0xca,0x32,0x3a,0x65,0x2c,0x04,0x04};//
static const UINT16 s_romData2nSet2_rW[]    = {0x35,0x09,0xa5,0x13,0x77,0x2c,0x05,0x62,0x04,0xe8,0x0F};//
static const UINT16 s_romData2nSet2_rY[]    = {0x3d,0x32,0x41,0xc2,0x0f,0x0f,0xc0,0x8a,0x2c,0xc6,0x21};//
static const UINT16 s_romData2nSet2_rZ[]    = {0xa2,0x40,0x1e,0x10,0x7e,0x2c,0x20,0xcb,0x50,0xad,0x52};//

//
// Set 2 from MAME.
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet2_rU, 0xfac42d34, " rU"}, // galaxian.u
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet2_rV, 0xf58283e3, " rV"}, // galaxian.v
                                                      {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSet2_rW, 0x4c7031c0, " rW"}, // galaxian.w
                                                      {NO_BANK_SWITCH, 0x1800, 0x0800, s_romData2nSet2_rY, 0x96a7ac94, " rY"}, // galaxian.y
                                                      {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nSet2_rZ, 0x5341d75a, " rZ"}, // galaxian.z
                                                      {0} }; // end of list

IGame*
CGalaxianGame::createInstanceSet1(
)
{
    return (new CGalaxianGame(s_romRegionSet1));
}


IGame*
CGalaxianGame::createInstanceSet2(
)
{
    return (new CGalaxianGame(s_romRegionSet2));
}


CGalaxianGame::CGalaxianGame(
    const ROM_REGION *romRegion
) : CGalaxianBaseGame( romRegion )
{
}


