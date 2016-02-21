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
#include "CDefenderGame.h"


//
// Defender Red ROMs
// ROM Type: 2532 x2 & 2716/2516 x9  (Note there is no ROM 5)
// ROM Board Jumpers: W2, W4, W6, W7, W9, W11, W13, W16, W17, W20, W21, W24
//
// Interface Board Jumpers: 
// W1 should be removed with a later series interface board when using in a tabletop game
// W2 should be fitted with a later series interface board
//
// Can be used in a cocktail game with a later series CPU and interface board
//
//                                                    01   02   04   08   10   20   40   80  100  200  400, 800
static const UINT8 s_romData2nDefenderRed_IC03[] = {0x29,0x81,0x26,0x84,0x91,0x78,0xed,0x81,0x4d,0x4d,0xbd,0x34};
static const UINT8 s_romData2nDefenderRed_IC02[] = {0xe3,0x9f,0xa7,0xa3,0xc3,0x36,0xa2,0x85,0x84,0xff,0x83,0xef};
static const UINT8 s_romData2nDefenderRed_IC01[] = {0xde,0x63,0x44,0xe8,0x7e,0xed,0xde,0x5f,0x31,0xa7,0xbe};
static const UINT8 s_romData2nDefenderRed_IC04[] = {0x5a,0x26,0x39,0xff,0x17,0x65,0x37,0xc2,0x90,0x1e,0x02};
static const UINT8 s_romData2nDefenderRed_IC09[] = {0xc0,0x06,0xcc,0x7c,0x96,0x06,0x7c,0x96,0x7d,0x80,0x33};
static const UINT8 s_romData2nDefenderRed_IC12[] = {0x30,0x89,0xf8,0x61,0xf8,0x86,0xc8,0x44,0x7c,0x18,0x5c};
static const UINT8 s_romData2nDefenderRed_IC08[] = {0xfb,0x7e,0xa7,0x7e,0xc1,0x7e,0x00,0x4d,0x01,0xc3,0x58};
static const UINT8 s_romData2nDefenderRed_IC11[] = {0x11,0x11,0x11,0x01,0x10,0x01,0x10,0x01,0x10,0x10,0xff};
static const UINT8 s_romData2nDefenderRed_IC07[] = {0xc0,0x76,0xc0,0xb8,0xcb,0xaf,0x02,0x69,0x7f,0x80,0x26};
static const UINT8 s_romData2nDefenderRed_IC10[] = {0x23,0xc5,0x26,0x0a,0x20,0xca,0xcb,0xfe,0xc0,0x28,0x89};
static const UINT8 s_romData2nDefenderRed_IC06[] = {0xc0,0x15,0xc0,0x62,0xd0,0x0f,0xc2,0x97,0xb1,0x01,0x04};

static const ROM_REGION s_romRegionDefenderRed[] PROGMEM = { //                                    "012"
    {CDefenderBaseGame::onROMBankSelect0, 0xf000, 0x1000, s_romData2nDefenderRed_IC03, 0x94f51e9b, "U03"},
    {CDefenderBaseGame::onROMBankSelect0, 0xe000, 0x1000, s_romData2nDefenderRed_IC02, 0x89b75984, "U02"},
    {CDefenderBaseGame::onROMBankSelect0, 0xd000, 0x0800, s_romData2nDefenderRed_IC01, 0xc3e52d7e, "U01"},
    {CDefenderBaseGame::onROMBankSelect0, 0xd800, 0x0800, s_romData2nDefenderRed_IC04, 0x9a72348b, "U04"},
    {CDefenderBaseGame::onROMBankSelect1, 0xc000, 0x0800, s_romData2nDefenderRed_IC09, 0x6870e8a5, "U09"},
    {CDefenderBaseGame::onROMBankSelect1, 0xc800, 0x0800, s_romData2nDefenderRed_IC12, 0xf1f88938, "U12"},
    {CDefenderBaseGame::onROMBankSelect2, 0xc000, 0x0800, s_romData2nDefenderRed_IC08, 0xb649e306, "U08"},
    {CDefenderBaseGame::onROMBankSelect2, 0xc800, 0x0800, s_romData2nDefenderRed_IC11, 0x9deaf6d9, "U11"},
    {CDefenderBaseGame::onROMBankSelect3, 0xc000, 0x0800, s_romData2nDefenderRed_IC07, 0x339e092e, "U07"},
    {CDefenderBaseGame::onROMBankSelect3, 0xc800, 0x0800, s_romData2nDefenderRed_IC10, 0xa543b167, "U10"},
    {CDefenderBaseGame::onROMBankSelect7, 0xc000, 0x0800, s_romData2nDefenderRed_IC06, 0x65f4efd1, "U06"},
    {0} 
};


//
// Defender Green ROMs
// ROM Type: 2532 x2 & 2716/2516 x9  (Note there is no ROM 5)
// ROM Board Jumpers: W2, W4, W6, W7, W9, W11, W13, W16, W17, W20, W21, W24
//
// Interface Board Jumpers: 
// W1 should be fited with a later series interface board
// W2 should be removed with a later series interface board
//
// Cannot be used in a cocktail game
//
//                                                      01   02   04   08   10   20   40   80  100  200  400, 800
static const UINT8 s_romData2nDefenderGreen_IC03[] = {0xef,0x06,0xfc,0x8d,0x35,0x26,0x14,0x5f,0xae,0x86,0x1f,0xd0};
static const UINT8 s_romData2nDefenderGreen_IC02[] = {0xc4,0x80,0x4c,0x4c,0x01,0xa7,0x38,0xce,0xa6,0x0f,0xee,0x08};
static const UINT8 s_romData2nDefenderGreen_IC01[] = {0xde,0x40,0x44,0xe7,0x7e,0xed,0xde,0x3c,0x31,0xa7,0xbe};
static const UINT8 s_romData2nDefenderGreen_IC04[] = {0xf8,0x42,0x26,0x97,0xec,0x96,0x7c,0xa3,0xad,0xd9,0xc2};
static const UINT8 s_romData2nDefenderGreen_IC09[] = {0xd0,0x7c,0xff,0xdf,0xb7,0x00,0xc6,0x58,0xa8,0x53,0x10};
static const UINT8 s_romData2nDefenderGreen_IC12[] = {0x8b,0x86,0x97,0xd6,0xb7,0x0a,0x03,0x34,0x7e,0x24,0x06};
static const UINT8 s_romData2nDefenderGreen_IC08[] = {0xff,0x7e,0xb4,0x7e,0xfa,0x7e,0x00,0x4d,0x01,0xc3,0x58};
static const UINT8 s_romData2nDefenderGreen_IC11[] = {0x11,0x00,0x11,0x11,0x01,0x00,0x01,0x01,0x10,0x10,0xce};
static const UINT8 s_romData2nDefenderGreen_IC07[] = {0xc0,0x76,0xc0,0xb8,0xcb,0x7f,0x02,0x69,0x7f,0xbd,0xc0};
static const UINT8 s_romData2nDefenderGreen_IC10[] = {0x01,0x26,0xc5,0x7c,0xc6,0xc1,0xbd,0xf6,0x8b,0x8d,0x8e};
static const UINT8 s_romData2nDefenderGreen_IC06[] = {0xc0,0x11,0xc0,0x5e,0xcc,0x4b,0x20,0xbe,0x35,0xc9,0x1d};

static const ROM_REGION s_romRegionDefenderGreen[] PROGMEM = { //                                    "012"
    {CDefenderBaseGame::onROMBankSelect0, 0xf000, 0x1000, s_romData2nDefenderGreen_IC03, 0x788b76d7, "U03"},
    {CDefenderBaseGame::onROMBankSelect0, 0xe000, 0x1000, s_romData2nDefenderGreen_IC02, 0xd184ab6b, "U02"},
    {CDefenderBaseGame::onROMBankSelect0, 0xd000, 0x0800, s_romData2nDefenderGreen_IC01, 0x6111d74d, "U01"},
    {CDefenderBaseGame::onROMBankSelect0, 0xd800, 0x0800, s_romData2nDefenderGreen_IC04, 0x3cfc04ce, "U04"},
    {CDefenderBaseGame::onROMBankSelect1, 0xc000, 0x0800, s_romData2nDefenderGreen_IC09, 0xf57caa62, "U09"},
    {CDefenderBaseGame::onROMBankSelect1, 0xc800, 0x0800, s_romData2nDefenderGreen_IC12, 0x33db686f, "U12"},
    {CDefenderBaseGame::onROMBankSelect2, 0xc000, 0x0800, s_romData2nDefenderGreen_IC08, 0x9a9eb3d2, "U08"},
    {CDefenderBaseGame::onROMBankSelect2, 0xc800, 0x0800, s_romData2nDefenderGreen_IC11, 0x5ca4e860, "U11"},
    {CDefenderBaseGame::onROMBankSelect3, 0xc000, 0x0800, s_romData2nDefenderGreen_IC07, 0x545c3326, "U07"},
    {CDefenderBaseGame::onROMBankSelect3, 0xc800, 0x0800, s_romData2nDefenderGreen_IC10, 0x941cf34e, "U10"},
    {CDefenderBaseGame::onROMBankSelect7, 0xc000, 0x0800, s_romData2nDefenderGreen_IC06, 0x3af34c05, "U06"},
    {0} 
};


//
// Defender Blue ROMs
// ROM Type: 2532 x3 & 2716/2516 x7  (Note there is no ROM 4 or ROM 5)
// ROM Board Jumpers: W2, W3, W6, W7, W10, W11, W13, W16, W17, W20, W21, W24
//
// Interface Board Jumpers: 
// W1 should be fited with a later series interface board
// W2 should be removed with a later series interface board
//
// Cannot be used in a cocktail game
//
//                                                     01   02   04   08   10   20   40   80  100  200  400, 800
static const UINT8 s_romData2nDefenderBlue_IC03[] = {0xef,0x06,0xfc,0x8d,0x35,0x26,0x14,0x5f,0xae,0x86,0x1f,0xd0};
static const UINT8 s_romData2nDefenderBlue_IC02[] = {0xc4,0x80,0x4c,0x4c,0x01,0xa7,0x38,0xce,0xa6,0x0f,0xee,0x08};
static const UINT8 s_romData2nDefenderBlue_IC01[] = {0xde,0x40,0x44,0xe7,0x7e,0xed,0xde,0x3c,0x31,0xa7,0xbe,0xbd};
static const UINT8 s_romData2nDefenderBlue_IC09[] = {0xd0,0x7c,0xff,0xdf,0xb7,0x00,0xc6,0x58,0xa8,0x53,0x10};
static const UINT8 s_romData2nDefenderBlue_IC12[] = {0x8b,0x86,0x97,0xd6,0xb7,0x0a,0x03,0x34,0x7e,0x24,0x06};
static const UINT8 s_romData2nDefenderBlue_IC08[] = {0xff,0x7e,0xb4,0x7e,0xfa,0x7e,0x00,0x4d,0x01,0xc3,0x58};
static const UINT8 s_romData2nDefenderBlue_IC11[] = {0x11,0x00,0x11,0x11,0x01,0x00,0x01,0x01,0x10,0x10,0xce};
static const UINT8 s_romData2nDefenderBlue_IC07[] = {0xc0,0x76,0xc0,0xb8,0xcb,0x7f,0x02,0x69,0x7f,0xbd,0xc0};
static const UINT8 s_romData2nDefenderBlue_IC10[] = {0x01,0x26,0xc5,0x7c,0xc6,0xc1,0xbd,0xf6,0x8b,0x8d,0x8e};
static const UINT8 s_romData2nDefenderBlue_IC06[] = {0xc0,0x11,0xc0,0x5e,0xcc,0x4b,0x20,0xbe,0x35,0xc9,0x1d};

static const ROM_REGION s_romRegionDefenderBlue[] PROGMEM = { //                                    "012"
    {CDefenderBaseGame::onROMBankSelect0, 0xf000, 0x1000, s_romData2nDefenderBlue_IC03, 0xa732d649, "U03"},
    {CDefenderBaseGame::onROMBankSelect0, 0xe000, 0x1000, s_romData2nDefenderBlue_IC02, 0xd184ab6b, "U02"},
    {CDefenderBaseGame::onROMBankSelect0, 0xd000, 0x1000, s_romData2nDefenderBlue_IC01, 0x0ee1019d, "U01"},
    {CDefenderBaseGame::onROMBankSelect1, 0xc000, 0x0800, s_romData2nDefenderBlue_IC09, 0xf57caa62, "U09"},
    {CDefenderBaseGame::onROMBankSelect1, 0xc800, 0x0800, s_romData2nDefenderBlue_IC12, 0x33db686f, "U12"},
    {CDefenderBaseGame::onROMBankSelect2, 0xc000, 0x0800, s_romData2nDefenderBlue_IC08, 0x9a9eb3d2, "U08"},
    {CDefenderBaseGame::onROMBankSelect2, 0xc800, 0x0800, s_romData2nDefenderBlue_IC11, 0x5ca4e860, "U11"},
    {CDefenderBaseGame::onROMBankSelect3, 0xc000, 0x0800, s_romData2nDefenderBlue_IC07, 0x545c3326, "U07"},
    {CDefenderBaseGame::onROMBankSelect3, 0xc800, 0x0800, s_romData2nDefenderBlue_IC10, 0x941cf34e, "U10"},
    {CDefenderBaseGame::onROMBankSelect7, 0xc000, 0x0800, s_romData2nDefenderBlue_IC06, 0x3af34c05, "U06"},
    {0} 
};


//
// Defender White ROMs
// ROM Type: 2532 x3 & 2716/2516 x7  (Note there is no ROM 4 or ROM 5)
// ROM Board Jumpers: W2, W3, W6, W7, W10, W11, W13, W16, W17, W20, W21, W24
//
// Interface Board Jumpers: 
// W1 should be fited with a later series interface board
// W2 should be removed with a later series interface board
//
// Cannot be used in a cocktail game
//
//                                                      01   02   04   08   10   20   40   80  100  200  400, 800
static const UINT8 s_romData2nDefenderWhite_IC03[] = {0x96,0xf8,0xf0,0xbd,0xdd,0x52,0xd5,0x8e,0x0c,0x7e,0xe3,0xff};
static const UINT8 s_romData2nDefenderWhite_IC02[] = {0x25,0x32,0x6c,0x6c,0x8c,0x7d,0x0c,0xbd,0x98,0x08,0xae,0x01};
static const UINT8 s_romData2nDefenderWhite_IC01[] = {0xff,0xde,0xa7,0x7e,0x84,0x84,0x62,0xa0,0x34,0x08,0x34,0x06};
static const UINT8 s_romData2nDefenderWhite_IC09[] = {0xd0,0x7d,0xff,0xbd,0x59,0x10,0xa0,0xbd,0x7d,0x01,0x86};
static const UINT8 s_romData2nDefenderWhite_IC12[] = {0xf8,0x10,0xcc,0x8e,0x86,0x08,0x8e,0x4c,0x8e,0x81,0x00};
static const UINT8 s_romData2nDefenderWhite_IC08[] = {0x27,0x7e,0xd4,0x7e,0xfc,0x7e,0x00,0x4d,0x01,0xc3,0x58};
static const UINT8 s_romData2nDefenderWhite_IC11[] = {0x01,0x00,0x00,0x00,0x11,0x00,0x00,0x10,0x01,0x10,0xff};
static const UINT8 s_romData2nDefenderWhite_IC07[] = {0xc0,0x76,0xc0,0xbe,0xcb,0xa5,0x02,0x94,0xbd,0x39,0x83};
static const UINT8 s_romData2nDefenderWhite_IC10[] = {0xbd,0xca,0x7d,0x1c,0xb2,0xc1,0xbd,0xbd,0xf0,0xca,0xb3};
static const UINT8 s_romData2nDefenderWhite_IC06[] = {0xc0,0x11,0xc0,0x5e,0xcc,0x4b,0x20,0xbe,0x35,0xc9,0x1d};

static const ROM_REGION s_romRegionDefenderWhite[] PROGMEM = { //                                    "012"
    {CDefenderBaseGame::onROMBankSelect0, 0xf000, 0x1000, s_romData2nDefenderWhite_IC03, 0x4097b46b, "U03"},
    {CDefenderBaseGame::onROMBankSelect0, 0xe000, 0x1000, s_romData2nDefenderWhite_IC02, 0x1126adc9, "U02"},
    {CDefenderBaseGame::onROMBankSelect0, 0xd000, 0x1000, s_romData2nDefenderWhite_IC01, 0x5af871e3, "U01"},
    {CDefenderBaseGame::onROMBankSelect1, 0xc000, 0x0800, s_romData2nDefenderWhite_IC09, 0x93012991, "U09"},
    {CDefenderBaseGame::onROMBankSelect1, 0xc800, 0x0800, s_romData2nDefenderWhite_IC12, 0x4bdd8dc4, "U12"},
    {CDefenderBaseGame::onROMBankSelect2, 0xc000, 0x0800, s_romData2nDefenderWhite_IC08, 0x5227fc0b, "U08"},
    {CDefenderBaseGame::onROMBankSelect2, 0xc800, 0x0800, s_romData2nDefenderWhite_IC11, 0xd068f0c5, "U11"},
    {CDefenderBaseGame::onROMBankSelect3, 0xc000, 0x0800, s_romData2nDefenderWhite_IC07, 0xfef4cb77, "U07"},
    {CDefenderBaseGame::onROMBankSelect3, 0xc800, 0x0800, s_romData2nDefenderWhite_IC10, 0x49b50b40, "U10"},
    {CDefenderBaseGame::onROMBankSelect7, 0xc000, 0x0800, s_romData2nDefenderWhite_IC06, 0x43d42a1b, "U06"},
    {0} 
};


IGame*
CDefenderGame::createInstanceDefenderRed(
)
{
    return (new CDefenderGame(s_romRegionDefenderRed));
}

IGame*
CDefenderGame::createInstanceDefenderGreen(
)
{
    return (new CDefenderGame(s_romRegionDefenderGreen));
}

IGame*
CDefenderGame::createInstanceDefenderBlue(
)
{
    return (new CDefenderGame(s_romRegionDefenderBlue));
}

IGame*
CDefenderGame::createInstanceDefenderWhite(
)
{
    return (new CDefenderGame(s_romRegionDefenderWhite));
}


CDefenderGame::CDefenderGame(
    const ROM_REGION *romRegion
) : CDefenderBaseGame( romRegion )
{
}

