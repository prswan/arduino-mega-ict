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
#include "CMarbleMadnessSoundGame.h"

//
// Marble Madness Rev 4.2 (presumably)
//
static const ROM_DATA2N s_romData2nRev42[] PROGMEM = { // 01   02   04   08   10   20   40   80  100  200  400  800 1000 2000
                                                       {0xa0,0x00,0x00,0x00,0x00,0xfe,0x00,0x3b,0x0d,0x7a,0x80,0x00,0x00,0x00}, // TBD
                                                       {0xf3,0x7c,0xfa,0x6e,0xfe,0x01,0xc2,0x02,0x8d,0x04,0x60,0x00,0x00,0x00}, // TBD
                                                     {0} };  // end of list

static const ROM_REGION s_romRegionRev42[] PROGMEM = { //
                                                       {NO_BANK_SWITCH, 0x8000, 0x4000, s_romData2nRev42[0].data2n, 0x00000000, "D14"}, // TBD
                                                       {NO_BANK_SWITCH, 0xC000, 0x4000, s_romData2nRev42[1].data2n, 0x00000000, "D16"}, // TBD
                                                     {0} }; // end of list

IGame*
CMarbleMadnessSoundGame::createInstanceRev42(
)
{
    return (new CMarbleMadnessSoundGame(s_romData2nRev42, s_romRegionRev42));
}

CMarbleMadnessSoundGame::CMarbleMadnessSoundGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CAtariSystem1SoundBaseGame(romData2n, romRegion)
{
}


