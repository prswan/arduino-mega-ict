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
#include "CStarWarsAvgGame.h"

//
// Default set from MAME.
//
//                                           01   02   04   08   10   20   40   80  100  200  400  800
static const UINT16 s_romData2n_1L[]    = {0xec,0xb8,0xb8,0xb8,0xb8,0xb8,0xb8,0xf8,0x40,0x45,0xb8,0xbc};

static const ROM_REGION s_romRegion[] PROGMEM =     { //
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2n_1L, 0x538e7d2f, "1L "}, // 105
                                                      {0} }; // end of list

IGame*
CStarWarsAvgGame::createInstance(
)
{
    return (new CStarWarsAvgGame(s_romRegion));
}


CStarWarsAvgGame::CStarWarsAvgGame(
    const ROM_REGION *romRegion
) : CStarWarsAvgBaseGame( romRegion )
{
}

