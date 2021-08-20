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
#include "CScorpionSoundGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Set 1 from MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x00,0x21,0x80,0xfe,0x40,0xff,0xb7,0xfe,0x02,0xbe,0x00,0x0c}, //  A4 - 12
                                                       {0x8c,0x8f,0x88,0x8f,0x09,0x83,0x0d,0x85,0xcd,0x04,0x1c,0x20}, //  A5 - 13
                                                       {0} };  // end of list

//
// Set 1 from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSet1[0].data2n, 0x361b8a36, " 12"}, // ot1.5c (Scorpions)
                                                      {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSet1[1].data2n, 0xaddecdd4, " 13"}, // ot2.5d (Scorpions)
                                                      {0} }; // end of list


IGame*
CScorpionSoundGame::createInstanceSet1(
)
{
    return (new CScorpionSoundGame(s_romData2nSet1, s_romRegionSet1));
}


CScorpionSoundGame::CScorpionSoundGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CScorpionSoundBaseGame( romData2n, romRegion )
{
}


