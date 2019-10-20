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
#include "CMegaZoneSoundGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// From MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800 1000
                                                       {0x32,0x01,0xc3,0xd3,0x78,0x19,0x21,0x02,0x11,0x7a,0x10,0x4e,0x10}, //
                                                       {0} };  // end of list

//
// From MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nSet1[0].data2n, 0xd5d45edb, "DE6"},
                                                      {0} }; // end of list


IGame*
CMegaZoneSoundGame::createInstanceSet1(
)
{
    return (new CMegaZoneSoundGame(s_romData2nSet1, s_romRegionSet1));
}


CMegaZoneSoundGame::CMegaZoneSoundGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CMegaZoneSoundBaseGame( romData2n, romRegion )
{
}


