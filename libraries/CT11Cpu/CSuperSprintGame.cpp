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
#include "CSuperSprintGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Base set from MAME (ssprint) - Rev 4
//                                           01   02   04   08   10   20   40   80  100  200  400  800 1000 2000
static const UINT8 s_romData2n_c7L[]    = {0x7e,0xdf,0x04,0xdf,0x5f,0xe6,0xdf,0x00,0x00,0x99,0xd7,0x40,0x5f,0x80};
static const UINT8 s_romData2n_c7N[]    = {0x01,0x17,0x00,0x0b,0x00,0x10,0x15,0x18,0x0a,0x01,0x20,0xdc,0x08,0x1d};

static const UINT8 s_romData2n_c00[]    = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//
// Base set from MAME (ssprint) - Rev 4
//
static const ROM_REGION s_romRegion[] PROGMEM = { //
                                                 {NO_BANK_SWITCH, 0x8000, 0x4000, s_romData2n_c7L, 0xee312027, "c7L"}, // Fixed Prog. Mem Lo
                                                 {NO_BANK_SWITCH, 0x8001, 0x4000, s_romData2n_c7N, 0x2ef15354, "c7N"}, // Fixed Prog. Mem Hi
                                                 {NO_BANK_SWITCH, 0x6000, 0x1000, s_romData2n_c00, 0x00000000, "c??"}, // Page 1 Prog. Mem Lo
                                                 {NO_BANK_SWITCH, 0x6001, 0x1000, s_romData2n_c00, 0x00000000, "c??"}, // Page 1 Prog. Mem Hi
                                                 {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2n_c00, 0x00000000, "c??"}, // Page 0 Prog. Mem Lo
                                                 {NO_BANK_SWITCH, 0x4001, 0x1000, s_romData2n_c00, 0x00000000, "c??"}, // Page 0 Prog. Mem Hi
                                                 {0} }; // end of list


IGame*
CSuperSprintGame::createInstance(
)
{
    return (new CSuperSprintGame(s_romRegion));
}


CSuperSprintGame::CSuperSprintGame(
    const ROM_REGION *romRegion
) : CSystem2BaseGame( romRegion )
{
}


