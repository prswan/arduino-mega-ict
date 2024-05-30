//
// Copyright (c) 2024, Paul R. Swan
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
#include "CMoneyMoneySoundGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Set 1 from MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0xa2,0x2d,0xdc,0x6a,0xc3,0xb0,0xa2,0xd6,0x21,0x70,0x26,0x28}, // ROM 8 with A12 low
                                                       {0xf1,0x91,0xcb,0x3b,0xa2,0xdf,0xed,0xd9,0x3e,0x72,0x34,0xa8}, // ROM 7 with A12 low
                                                       {0xc8,0x26,0x00,0xa0,0x4d,0xd5,0x63,0x1d,0xc6,0x69,0x00,0x01}, // ROM 8 with A12 high
                                                       {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x8e,0xec,0x00,0x83}, // ROM 7 with A12 high
                                                       {0} };  // end of list

//
// Set 1 from MAME.
// The ROMs are split into halves so the image ranges are split.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSet1[0].data2n, 0x87c24aec, "1Hl"}, // ROM 8 with A12 low
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nSet1[1].data2n, 0xba89dfbe, "1Gl"}, // ROM 7 with A12 low
                                                      {NO_BANK_SWITCH, 0x6000, 0x1000, s_romData2nSet1[2].data2n, 0xf6b5f19e, "1Hh"}, // ROM 8 with A12 high
                                                      {NO_BANK_SWITCH, 0x7000, 0x1000, s_romData2nSet1[3].data2n, 0xe7e4f19d, "1Gh"}, // ROM 7 with A12 high
                                                      {0} }; // end of list

IGame*
CMoneyMoneySoundGame::createInstanceSet1(
)
{
    return (new CMoneyMoneySoundGame(s_romData2nSet1, s_romRegionSet1));
}


CMoneyMoneySoundGame::CMoneyMoneySoundGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CMoneyMoneySoundBaseGame( romData2n, romRegion )
{
}


