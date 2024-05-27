//
// Copyright (c) 2022, Paul R. Swan
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
#include "CFitterSoundGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// "fitter" from MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0x00,0x00,0x31,0xbf,0x06,0x00,0x8e,0xcd,0xdd,0xf6,0x0d}, //  0 - 30
                                                       {0x02,0x0d,0x87,0x02,0x00,0x0d,0x87,0x02,0x81,0x00,0x02}, //  1 - 31
                                                       {0} };  // end of list

static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet1[0].data2n, 0x4055b5ca, " 30"}, // ic30.bin
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet1[1].data2n, 0xc9d8c1cc, " 31"}, // ic31.bin
                                                      {0} }; // end of list


IGame*
CFitterSoundGame::createInstanceSet1(
)
{
    return (new CFitterSoundGame(s_romData2nSet1, s_romRegionSet1));
}


CFitterSoundGame::CFitterSoundGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CFitterSoundBaseGame( romData2n, romRegion )
{
}


