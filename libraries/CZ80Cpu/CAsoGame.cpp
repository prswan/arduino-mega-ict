//
// Copyright (c) 2016, Paul R. Swan
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
#include "CAsoGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Set 1 from MAME.
//                                                   01   02   04   08   10   20   40   80  100  200  400  800 1000 2000
static const UINT16 s_romData2nAsoASet1_cD8[]    = {0xc0,0xfe,0xc3,0xc3,0xc3,0xc3,0xff,0x00,0xcb,0xc9,0xdd,0x20,0x10,0xa8};
static const UINT16 s_romData2nAsoASet1_cD7[]    = {0x40,0xdd,0x15,0xdd,0x77,0xed,0x4e,0x18,0x19,0xdd,0x7e,0x48,0x50,0xe8};
static const UINT16 s_romData2nAsoASet1_cD5[]    = {0x45,0x45,0x65,0x85,0xc4,0x46,0x46,0x67,0x67,0xa7,0x4a,0xff,0x06,0x07};

//
// Set 1 from MAME.
//
static const ROM_REGION s_romRegionAsoASet1[] PROGMEM = { //
                                                         {NO_BANK_SWITCH, 0x0000, 0x4000, s_romData2nAsoASet1_cD8, 0x84981f3c, "cD8"}, // P1
                                                         {NO_BANK_SWITCH, 0x4000, 0x4000, s_romData2nAsoASet1_cD7, 0xcfe912a6, "cD7"}, // P2
                                                         {NO_BANK_SWITCH, 0x8000, 0x4000, s_romData2nAsoASet1_cD5, 0x39a666d2, "cD5"}, // P3
                                                         {0} }; // end of list

//
// Set 1 from MAME.
//                                                      01   02   04   08   10   20   40   80  100  200  400  800 1000 2000
static const UINT16 s_romData2nArianASet1_cD8[]    = {0xc0,0xfe,0xc3,0xc3,0xc3,0xc3,0xff,0x00,0xcb,0xc9,0xdd,0xf9,0xdd,0xc9};
static const UINT16 s_romData2nArianASet1_cD7[]    = {0x00,0x10,0x1b,0x03,0xf1,0x0f,0xd1,0x76,0x41,0x34,0x21,0x01,0x7e,0xeb};
static const UINT16 s_romData2nArianASet1_cD5[]    = {0x46,0x46,0x65,0x85,0xc5,0x46,0x47,0x67,0x87,0xc8,0x4a,0x67,0x88,0x07};

//
// Set 1 from MAME.
//
static const ROM_REGION s_romRegionArianASet1[] PROGMEM = { //
                                                           {NO_BANK_SWITCH, 0x0000, 0x4000, s_romData2nArianASet1_cD8, 0x0ca89307, "cD8"}, // P1
                                                           {NO_BANK_SWITCH, 0x4000, 0x4000, s_romData2nArianASet1_cD7, 0x724518c3, "cD7"}, // P2
                                                           {NO_BANK_SWITCH, 0x8000, 0x4000, s_romData2nArianASet1_cD5, 0x4d8db650, "cD5"}, // P3
                                                           {0} }; // end of list

//
// Set 1 from MAME.
//                                                      01   02   04   08   10   20   40   80  100  200  400  800 1000 2000
static const UINT16 s_romData2nArianBSet1_cD3[]    = {0x31,0xc0,0xc3,0xc3,0xc3,0xc3,0xff,0x48,0xc3,0xc1,0xb7,0xfd,0x22,0x00};
static const UINT16 s_romData2nArianBSet1_cD2[]    = {0xff,0xfd,0xff,0xff,0xff,0xff,0x0f,0x0e,0xa0,0x9b,0xbd,0x8f,0xff,0xfc};
static const UINT16 s_romData2nArianBSet1_cD1[]    = {0x76,0x68,0x31,0x3a,0x73,0x6b,0x99,0xff,0xff,0xff,0xfd,0xfb,0xfc,0x5d};

//
// Set 1 from MAME.
//
static const ROM_REGION s_romRegionArianBSet1[] PROGMEM = { //
                                                           {NO_BANK_SWITCH, 0x0000, 0x4000, s_romData2nArianBSet1_cD3, 0x47baf1db, "cD3"}, // P4
                                                           {NO_BANK_SWITCH, 0x4000, 0x4000, s_romData2nArianBSet1_cD2, 0x9879e506, "cD2"}, // P5
                                                           {NO_BANK_SWITCH, 0x8000, 0x4000, s_romData2nArianBSet1_cD1, 0xc0bfdf1f, "cD1"}, // P6
                                                           {0} }; // end of list

IGame*
CAsoAGame::createInstanceAsoSet1(
)
{
    return (new CAsoAGame(s_romRegionAsoASet1));
}


IGame*
CAsoAGame::createInstanceArianSet1(
)
{
    return (new CAsoAGame(s_romRegionArianASet1));
}


CAsoAGame::CAsoAGame(
    const ROM_REGION *romRegion
) : CAsoABaseGame( romRegion )
{
}


IGame*
CAsoBGame::createInstanceArianSet1(
)
{
    return (new CAsoBGame(s_romRegionArianBSet1));
}


CAsoBGame::CAsoBGame(
    const ROM_REGION *romRegion
) : CAsoBBaseGame( romRegion )
{
}

