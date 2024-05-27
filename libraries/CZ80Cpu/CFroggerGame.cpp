//
// Copyright (c) 2021, Paul R. Swan
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
#include "CFroggerGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Sega Set 1 from MAME.
//
static const ROM_DATA2N s_romData2nSegaSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                           {0x00,0x40,0x55,0x3a,0x02,0x00,0x73,0x2c,0xca,0x81,0x00,0x77}, // 0 - epr-26.ic5
                                                           {0x77,0x01,0x23,0xd1,0x77,0x81,0x3a,0x46,0x2a,0x12,0xa8,0x83}, // 1 - epr-27.ic6
                                                           {0x69,0x82,0xdd,0x82,0x10,0x7e,0xca,0xcc,0x67,0x32,0x81,0x81}, // 2 - epr-34.ic7
                                                           {0} };  // end of list

//
// Sega Set 1 from MAME.
//
static const ROM_REGION s_romRegionSegaSet1[] PROGMEM = { //
                                                          {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSegaSet1[0].data2n, 0x597696d6, "  5"}, // 0 - epr-26.ic5
                                                          {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSegaSet1[1].data2n, 0xb6e6fcc3, "  6"}, // 1 - epr-27.ic6
                                                          {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSegaSet1[2].data2n, 0xed866bab, "  7"}, // 2 - epr-34.ic7
                                                          {0} }; // end of list




IGame*
CFroggerGame::createInstanceSegaSet1(
)
{
    return (new CFroggerGame(s_romData2nSegaSet1, s_romRegionSegaSet1));
}


CFroggerGame::CFroggerGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CFroggerBaseGame( romData2n, romRegion )
{
}


