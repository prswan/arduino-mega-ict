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
#include "CWmsSoundGame.h"



//
// Video Sound ROM 1 (Defender)
// ROM Type: 2716/2516
// Jumpers: W1, W2, W5, W7, W9, W10 & W15

//                                              01   02   04   08   10   20   40   80   100  200  400
static const UINT8 s_romData2nSet1_IC12[]    = {0x0f,0x8e,0x7f,0x6f,0x6f,0x04,0x1b,0xcb,0x5a,0x97,0xce};
static const ROM_REGION s_romRegionSet1[] PROGMEM = {
                                                  {NO_BANK_SWITCH, 0xf800, 0x0800, s_romData2nSet1_IC12, 0xfefd5b48, "U12"},
                                                  {0} }; // end of list


//
// Video Sound ROM 2 (Stargate)
// ROM Type: 2716/2516
// Jumpers: W1, W2, W5, W7, W9, W10 & W15
//                                              01   02   04   08   10   20   40   80   100  200  400
static const UINT8 s_romData2nSet2_IC12[]    = {0x0f,0x8e,0x7f,0x6f,0x6f,0x04,0x04,0x1b,0x5a,0x97,0x96};
static const ROM_REGION s_romRegionSet2[] PROGMEM = {
                                                  {NO_BANK_SWITCH, 0xf800, 0x0800, s_romData2nSet2_IC12, 0x2fcf6c4d, " 12"},
                                                  {0} }; // end of list

//
// Video Sound ROM 3 (Robotron)
// ROM Type: 2532
// Jumpers: W1, W3, W4, W5, W7, W10 & W15
//                                              01   02   04   08   10   20   40   80   100  200  400  800
static const UINT8 s_romData2nSet3_IC12[]    = {0x28,0x43,0x31,0x20,0x53,0x7f,0x02,0x06,0xce,0x25,0x00,0x06};
static const ROM_REGION s_romRegionSet3[] PROGMEM = {
                                                  {NO_BANK_SWITCH, 0xf000, 0x1000, s_romData2nSet3_IC12, 0xc56c1d28, " 12"},
                                                  {0} }; // end of list

//
// Video Sound ROM 4 (Joust)
// ROM Type: 2532
// Jumpers: W1, W3, W4, W5, W7, W10 & W15
//                                              01   02   04   08   10   20   40   80   100  200  400  800
static const UINT8 s_romData2nSet4_IC12[]    = {0x0f,0x8e,0x7f,0x6f,0x6f,0x07,0x00,0xde,0xb7,0x97,0x16,0x24};
static const ROM_REGION s_romRegionSet4[] PROGMEM = {
                                                  {NO_BANK_SWITCH, 0xf000, 0x1000, s_romData2nSet4_IC12, 0xf1835bdd, " 12"},
                                                  {0} }; // end of list

//
// Video Sound ROM 9 (Sinistar Main and Speech)
// ROM Type: 2532 x5
// Jumpers: W3, W4, W5, W7, W10 & W15 (W1 fitted if speech board not present)
//                                              01   02   04   08   10   20   40   80   100  200  400  800
static const UINT8 s_romData2nSet9_IC12[]    = {0x0f,0x8e,0x7f,0x6f,0xc6,0x7f,0xbd,0xe7,0x03,0x00,0x2a,0x06};
static const UINT8 s_romData2nSet9_IC04[]    = {0x41,0x5d,0xb5,0x69,0x55,0xaf,0x02,0x94,0xaa,0xa8,0xbd,0x56};
static const UINT8 s_romData2nSet9_IC06[]    = {0x07,0x89,0xbe,0xa2,0x67,0x5c,0xe0,0x38,0xdb,0x41,0xbf,0x2a};
static const UINT8 s_romData2nSet9_IC05[]    = {0x00,0xfe,0xfd,0x01,0xff,0xf8,0x81,0x00,0x0f,0xea,0x03,0xa8};
static const UINT8 s_romData2nSet9_IC07[]    = {0x00,0x00,0x00,0x00,0x00,0x42,0x03,0x87,0x00,0x00,0x00,0x00};
static const ROM_REGION s_romRegionSet9[] PROGMEM = {
                                                  {NO_BANK_SWITCH, 0xf000, 0x1000, s_romData2nSet9_IC12, 0xb82f4ddb, " 12"},
                                                  {NO_BANK_SWITCH, 0xe000, 0x1000, s_romData2nSet9_IC04, 0x4b56a626, " 04"},
                                                  {NO_BANK_SWITCH, 0xd000, 0x1000, s_romData2nSet9_IC06, 0xff8d2645, " 06"},
                                                  {NO_BANK_SWITCH, 0xc000, 0x1000, s_romData2nSet9_IC05, 0xcf3b5ffd, " 05"},
                                                  {NO_BANK_SWITCH, 0xb000, 0x1000, s_romData2nSet9_IC07, 0xe1019568, " 07"},
                                                  {0} }; // end of list

//
// Video Sound ROM 10 (Sinistar Aux)
// ROM Type: 2532
// Jumpers: W1, W3, W4, W5, W7, W10 & W15
//                                              01   02   04   08   10   20   40   80   100  200  400  800
// static const UINT8 s_romData2nSet10_IC12[]    = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
// static const ROM_REGION s_romRegionSet10[] PROGMEM = {
//                                                   {NO_BANK_SWITCH, 0xf000, 0x1000, s_romData2nSet10_IC12, 0x00000000, " 12"},
//                                                   {0} }; // end of list


IGame*
CWmsSoundGame::createInstanceVideoSoundROM1(
)
{
    return (new CWmsSoundGame(s_romRegionSet1));
}

IGame*
CWmsSoundGame::createInstanceVideoSoundROM2(
)
{
    return (new CWmsSoundGame(s_romRegionSet2));
}

IGame*
CWmsSoundGame::createInstanceVideoSoundROM3(
)
{
    return (new CWmsSoundGame(s_romRegionSet3));
}

IGame*
CWmsSoundGame::createInstanceVideoSoundROM4(
)
{
    return (new CWmsSoundGame(s_romRegionSet4));
}

IGame*
CWmsSoundGame::createInstanceVideoSoundROM9(
)
{
    return (new CWmsSoundGame(s_romRegionSet9));
}

// IGame*
// CWmsSoundGame::createInstanceVideoSoundROM10(
// )
// {
//     return (new CWmsSoundGame(s_romRegionSet10));
// }


CWmsSoundGame::CWmsSoundGame(
    const ROM_REGION *romRegion
) : CWmsSoundBaseGame( romRegion )
{
}

