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
#include "CCatnMouseSoundGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Set 1 from MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x00,0x00,0x00,0x8c,0x00,0x81,0x00,0x82,0x81,0x00,0x81,0x10}, // 1F
                                                       {0x00,0x00,0x03,0x5a,0x00,0x63,0x00,0x5b,0x5b,0x00,0x46,0x10}, // 1D
                                                       {0x8e,0x00,0xbd,0x55,0x07,0xbd,0xc6,0x5a,0x00,0x02,0x20,0x05}, // 1E
                                                       {0} };  // end of list

//
// Set 1 from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0xc000, 0x1000, s_romData2nSet1[0].data2n, 0x473c44de, "1F "}, // 1F
                                                      {NO_BANK_SWITCH, 0xe000, 0x1000, s_romData2nSet1[1].data2n, 0xf65cb9d0, "1D "}, // 1D
                                                      {NO_BANK_SWITCH, 0xf000, 0x1000, s_romData2nSet1[2].data2n, 0x1bd90c93, "1E "}, // 1E
                                                      {0} }; // end of list

IGame*
CCatnMouseSoundGame::createInstanceSet1(
)
{
    return (new CCatnMouseSoundGame(s_romData2nSet1, s_romRegionSet1));
}


CCatnMouseSoundGame::CCatnMouseSoundGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CCatnMouseSoundBaseGame( romData2n, romRegion )
{
}


