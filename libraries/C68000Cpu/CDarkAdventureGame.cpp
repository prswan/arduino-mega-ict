//
// Copyright (c) 2017, Paul R. Swan
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
#include "CDarkAdventureGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// From MAME (darkadv)
//                                           01   02   04   08   10   20   40   80  100  200  400  800 1000 2000 4000 8000
static const UINT16 s_romData2n_6N[]    = {0x0f,0x00,0x00,0x00,0x00,0x00,0x48,0xff,0x51,0x00,0x13,0x4e,0x41,0x10,0x00,0x00}; //
static const UINT16 s_romData2n_4N[]    = {0x00,0x00,0x00,0x00,0x00,0x00,0xe7,0xff,0xc9,0x10,0xfc,0xf9,0x52,0x84,0x01,0x00}; //
static const UINT16 s_romData2n_6R[]    = {0x93,0x00,0x4e,0x4e,0x67,0x00,0x00,0x00,0x00,0x0c,0x36,0xe5,0x00,0x00,0x00,0x10}; //
static const UINT16 s_romData2n_4R[]    = {0xbe,0x02,0x75,0xf9,0x32,0x18,0x00,0x02,0x03,0x28,0xa8,0x49,0x08,0x02,0x20,0x00}; //

//
// From MAME (darkadv)
//
static const ROM_REGION s_romRegion[] PROGMEM = { //
                                                  {NO_BANK_SWITCH, 0x00000000, 0x00010000, s_romData2n_6N, 0xa9195b0b, " 6N"}, //
                                                  {NO_BANK_SWITCH, 0x00000001, 0x00010000, s_romData2n_4N, 0x65b55105, " 4N"}, //
                                                  {NO_BANK_SWITCH, 0x00020000, 0x00010000, s_romData2n_6R, 0x1c6b594c, " 6R"}, //
                                                  {NO_BANK_SWITCH, 0x00020001, 0x00010000, s_romData2n_4R, 0xa9603196, " 4R"}, //
                                                  {0} }; // end of list


IGame*
CDarkAdventureGame::createInstance(
)
{
    return (new CDarkAdventureGame(s_romRegion));
}


CDarkAdventureGame::CDarkAdventureGame(
    const ROM_REGION *romRegion
) : CKonamiTwin16BaseGame( romRegion )
{
}


