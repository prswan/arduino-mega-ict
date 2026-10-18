//
// Copyright (c) 2026, patzik
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
#include "CTimePilotSoundGame.h"

//
// ROM set, address, length & CRC are from MAME (src/mame/konami/timeplt.cpp).
// The same sound ROM is used by all the Time Pilot sets.
// The data2n samples were generated from the MAME ROM image.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x00,0x30,0x00,0x32,0x32,0x32,0x3e,0x21,0x32,0xdd,0xaf,0xae}, // tm7
                                                       {0} }; // end of list

static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSet1[0].data2n, 0xd66da813, "tm7"}, // tm7
                                                      {0} }; // end of list

IGame*
CTimePilotSoundGame::createInstanceSet1(
)
{
    return (new CTimePilotSoundGame(s_romData2nSet1, s_romRegionSet1));
}


CTimePilotSoundGame::CTimePilotSoundGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CTimePilotSoundBaseGame( romData2n, romRegion )
{
}

