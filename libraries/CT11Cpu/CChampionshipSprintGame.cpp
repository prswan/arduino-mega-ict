//
// Copyright (c) 2016, Paul R. Swan
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
#include "CChampionshipSprintGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Base set from MAME (csprint) - Rev 3
//                                           01   02   04   08   10   20   40   80  100  200  400  800 1000 2000
static const UINT16 s_romData2n_c7L[]    = {0x7e,0xdf,0x04,0xdf,0x5f,0xe6,0xdf,0x00,0x00,0xd4,0xc6,0x1f,0x32,0x00};
static const UINT16 s_romData2n_c7N[]    = {0x01,0x17,0x00,0x0b,0x00,0x10,0x15,0x18,0x0a,0x04,0xbb,0xa0,0x0a,0x0c};

//
// Base set from MAME (csprint) - Rev 3
//
static const ROM_REGION s_romRegion[] PROGMEM = { //
                                                 {NO_BANK_SWITCH, 0x8000, 0x4000, s_romData2n_c7L, 0x1dcf8b98, "c7L"}, // Fixed Prog. Mem Lo
                                                 {NO_BANK_SWITCH, 0x8001, 0x4000, s_romData2n_c7N, 0xbdcbe42c, "c7N"}, // Fixed Prog. Mem Hi
                                                 {0} }; // end of list


IGame*
CChampionshipSprintGame::createInstance(
)
{
    return (new CChampionshipSprintGame(s_romRegion));
}


CChampionshipSprintGame::CChampionshipSprintGame(
    const ROM_REGION *romRegion
) : CSystem2BaseGame( romRegion )
{
}


