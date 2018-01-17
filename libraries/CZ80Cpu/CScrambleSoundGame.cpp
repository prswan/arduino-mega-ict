//
// Copyright (c) 2018, Paul R. Swan
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
#include "CScrambleSoundGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Stern Set 1 from MAME.
//                                               01   02   04   08   10   20   40   80  100  200  400
static const UINT16 s_romData2nSet1_5C[]    = {0x72,0x02,0xff,0xff,0xff,0xff,0xb7,0xfe,0x02,0xbe,0x00};
static const UINT16 s_romData2nSet1_5D[]    = {0x80,0x77,0xb7,0x08,0x08,0xfe,0xaf,0x00,0x21,0x80,0x0d};
static const UINT16 s_romData2nSet1_5E[]    = {0x8c,0x8f,0x88,0x8f,0x09,0x83,0x0d,0x85,0xcd,0x04,0x1c};

//
// Stern Set 1 from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet1_5C, 0xbcd297f0, " 5C"}, // ot1.5c (scrambles)
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet1_5D, 0xde7912da, " 5D"}, // ot2.5d (scrambles)
                                                      {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSet1_5E, 0xba2fa933, " 5E"}, // ot3.5e (scrambles)
                                                      {0} }; // end of list

//
// Zaccaria Set 1 from PCB.
//
static const ROM_REGION s_romRegionZacSet1[] PROGMEM = { //
                                                         {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet1_5C, 0xbe037cf6, " 5C"}, // 5c.cpu (scramrf)
                                                         {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet1_5D, 0xde7912da, " 5D"}, // ot2.5d (scrambles)
                                                         {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSet1_5E, 0xba2fa933, " 5E"}, // ot3.5e (scrambles)
                                                         {0} }; // end of list

IGame*
CScrambleSoundGame::createInstanceSet1(
)
{
    return (new CScrambleSoundGame(s_romRegionSet1));
}


IGame*
CScrambleSoundGame::createInstanceZacSet1(
)
{
    return (new CScrambleSoundGame(s_romRegionZacSet1));
}

CScrambleSoundGame::CScrambleSoundGame(
    const ROM_REGION *romRegion
) : CScrambleSoundBaseGame( romRegion )
{
}


