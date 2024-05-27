//
// Copyright (c) 2019, Paul R. Swan
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
#include "CSuperBasketballGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Set G from MAME
//                                                01   02   04   08   10   20   40   80  100  200  400  800 1000
static const UINT16 s_romData2nSetG_14J[]    = {0x00,0x00,0x3c,0x3c,0x3d,0xf6,0xb5,0x80,0x03,0xac,0xb4,0x2d,0x3a};
static const UINT16 s_romData2nSetG_13J[]    = {0xa1,0x10,0x07,0x57,0xa4,0xd5,0x32,0x32,0x08,0x74,0x32,0x20,0x6d};
static const UINT16 s_romData2nSetG_11J[]    = {0xa8,0x36,0xae,0x0f,0xd4,0x01,0x40,0xec,0xa3,0x94,0x22,0x06,0x9f};
static const UINT16 s_romData2nSetG_10J[]    = {0x5c,0xc2,0x58,0x0e,0x02,0xe3,0x94,0x21,0x80,0x25,0xc8,0xc9,0x05};
static const UINT16 s_romData2nSetG_09J[]    = {0xc7,0xd7,0x37,0x37,0x38,0xee,0x1e,0x0f,0x1f,0xff,0x37,0x01,0x99};

//
// Set G from MAME
//
static const ROM_REGION s_romRegionSetG[] PROGMEM = { //
                                                     {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetG_14J, 0x336dc0ab, "14J"},
                                                     {NO_BANK_SWITCH, 0x8000, 0x2000, s_romData2nSetG_13J, 0xf064a9bc, "13J"},
                                                     {NO_BANK_SWITCH, 0xA000, 0x2000, s_romData2nSetG_11J, 0xb9de7d53, "11J"},
                                                     {NO_BANK_SWITCH, 0xC000, 0x2000, s_romData2nSetG_10J, 0xe98470a0, "10J"},
                                                     {NO_BANK_SWITCH, 0xE000, 0x2000, s_romData2nSetG_09J, 0x1bd0cd2e, "09J"},
                                                     {0} }; // end of list


IGame*
CSuperBasketballGame::createInstanceSetG_09(
)
{
    return (new CSuperBasketballGame(false, s_romRegionSetG));
}

IGame*
CSuperBasketballGame::createInstanceSetG_K1(
)
{
    return (new CSuperBasketballGame(true, s_romRegionSetG));
}


CSuperBasketballGame::CSuperBasketballGame(
    bool              konami1,
    const ROM_REGION *romRegion
) : CSuperBasketballBaseGame( konami1, romRegion )
{
}


