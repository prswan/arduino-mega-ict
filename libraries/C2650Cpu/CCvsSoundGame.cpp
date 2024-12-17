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
#include "CCvsSoundGame.h"

//
// TODO: Set 1 from MAME.
//                                          01   02   04   08   10   20   40   80  100  200
static const UINT16 s_romData2n_SDP1[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//
// Set 1 from MAME
//
static const ROM_REGION s_romRegionHunchbackSet1[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2n_SDP1, 0xf9ba2854, "SD1"},
                                                               {0} }; // end of list

IGame*
CCvsSoundGame::createInstanceHunchbackSet1(
)
{
    return (new CCvsSoundGame(s_romRegionHunchbackSet1));
}

CCvsSoundGame::CCvsSoundGame(
    const ROM_REGION *romRegion
) : CCvsSoundBaseGame( romRegion )
{
}


