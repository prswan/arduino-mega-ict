//
// Copyright (c) 2020, Paul R. Swan
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
#include "CTheInvadersGame.h"

static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0x00,0xe7,0x13,0x00,0x00,0x00,0xcd,0xf9,0xc1,0x0e,0x9c},//
                                                       {0xe4,0x20,0x21,0x1d,0xcc,0x1d,0x07,0x0c,0x75,0x45,0x01},//
                                                       {0x30,0x31,0x30,0x20,0xa7,0x00,0x90,0x69,0xf4,0x20,0x03},//
                                                       {0} };  // end of list


//
// tinv2650 in MAME
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet1[0].data2n, 0xa85550a9, " 42"}, // 42_1
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet1[1].data2n, 0x083c8621, " 44"}, // 44t_2
                                                      {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSet1[2].data2n, 0x12c0934f, " 46"}, // 46t_3
                                                      {0} }; // end of list


IGame*
CTheInvadersGame::createInstanceSet1(
)
{
    return (new CTheInvadersGame(s_romData2nSet1, s_romRegionSet1));
}


CTheInvadersGame::CTheInvadersGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CTheInvadersBaseGame( romData2n, romRegion )
{
}


