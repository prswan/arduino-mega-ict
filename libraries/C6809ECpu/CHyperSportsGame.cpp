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
#include "CHyperSportsGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Set 1 "hyperspt" from MAME
//                                               01   02   04   08   10   20   40   80  100  200  400  800 1000
static const UINT16 s_romData2nSet1_G5[]    = {0x00,0x00,0xd4,0xf5,0xf5,0xdb,0xea,0x09,0x21,0xcd,0x00,0x00,0x00};
static const UINT16 s_romData2nSet1_G7[]    = {0x32,0x2a,0xcd,0x00,0xcd,0x14,0x25,0x7d,0xc2,0x3a,0x00,0x00,0x00};
static const UINT16 s_romData2nSet1_G9[]    = {0x32,0xc1,0xcd,0x67,0x02,0x18,0x0a,0xc3,0x00,0xf1,0x00,0x00,0x00};
static const UINT16 s_romData2nSet1_G11[]   = {0x00,0x00,0xd4,0xf5,0xf5,0xdb,0xea,0x09,0x21,0xcd,0x00,0x00,0x00};
static const UINT16 s_romData2nSet1_G13[]   = {0x32,0x2a,0xcd,0x00,0xcd,0x14,0x25,0x7d,0xc2,0x3a,0x00,0x00,0x00};
static const UINT16 s_romData2nSet1_G15[]   = {0x32,0xc1,0xcd,0x67,0x02,0x18,0x0a,0xc3,0x00,0xf1,0x00,0x00,0x00};

//
// Set 1 "hyperspt" from MAME
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                     {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nSet1_G5,  0x0c720eeb, " G5"},
                                                     {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSet1_G7,  0x560258e0, " G7"},
                                                     {NO_BANK_SWITCH, 0x8000, 0x2000, s_romData2nSet1_G9,  0x9b01c7e6, " G9"},
                                                     {NO_BANK_SWITCH, 0xA000, 0x2000, s_romData2nSet1_G11, 0x10d7e9a2, "G11"},
                                                     {NO_BANK_SWITCH, 0xC000, 0x2000, s_romData2nSet1_G13, 0xb105a8cd, "G13"},
                                                     {NO_BANK_SWITCH, 0xE000, 0x2000, s_romData2nSet1_G15, 0x1a34a849, "G15"},
                                                     {0} }; // end of list


IGame*
CHyperSportsGame::createInstanceSet1(
)
{
    return (new CHyperSportsGame(s_romRegionSet1));
}

CHyperSportsGame::CHyperSportsGame(
    const ROM_REGION *romRegion
) : CHyperSportsBaseGame( romRegion )
{
}


