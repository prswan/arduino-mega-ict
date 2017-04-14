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
//                                           01   02   04   08   10   20   40   80  100  200  400  800 1000 2000 4000
static const UINT16 s_romData2n_c7L[]    = {0x7e,0xdf,0x04,0xdf,0x5f,0xe6,0xdf,0x00,0x00,0x99,0xd7,0x40,0x5f,0x80};
static const UINT16 s_romData2n_c7N[]    = {0x01,0x17,0x00,0x0b,0x00,0x10,0x15,0x18,0x0a,0x01,0x20,0xdc,0x08,0x1d};

static const UINT16 s_romData2n_c6F[]    = {0x6e,0x8d,0x98,0x82,0x98,0x93,0x9b,0xd1,0xea,0x6e,0x6e,0x00,0x0f,0x00,0x00};
static const UINT16 s_romData2n_c6N[]    = {0x05,0x05,0x00,0x06,0x00,0x06,0x06,0x06,0x06,0x05,0x05,0x00,0x0f,0x00,0x00};
static const UINT16 s_romData2n_c6K[]    = {0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x01,0x01,0x01,0x01,0x58,0x0f,0x0f,0x01};
static const UINT16 s_romData2n_c6R[]    = {0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x01,0x01,0x01,0x04,0x18,0x0f,0x0f,0x01};
static const UINT16 s_romData2n_c6L[]    = {0x02,0x02,0x02,0x02,0x4b,0x41,0x61,0x7d,0x03,0x1f,0x00,0x00,0xe6,0x08,0x00};
static const UINT16 s_romData2n_c6S[]    = {0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x06,0x07,0x00};

//
// Base set from MAME (ssprint) - Rev 4
//
static const ROM_REGION s_romRegion[] PROGMEM = { //
                                                 {NO_BANK_SWITCH, 0x00008000, 0x4000, s_romData2n_c7L, 0xee312027, "c7L"}, // Fixed Prog. Mem Lo
                                                 {NO_BANK_SWITCH, 0x00008001, 0x4000, s_romData2n_c7N, 0x2ef15354, "c7N"}, // Fixed Prog. Mem Hi
                                                 {NO_BANK_SWITCH, 0x00800000, 0x8000, s_romData2n_c6F, 0xed1d6205, "c6F"}, // Paged Prog. Mem Lo
                                                 {NO_BANK_SWITCH, 0x00800001, 0x8000, s_romData2n_c6N, 0xaecaa2bf, "c6N"}, // Paged Prog. Mem Hi
                                                 // 6J not used
                                                 // 6P not used
                                                 {NO_BANK_SWITCH, 0x00A00000, 0x8000, s_romData2n_c6K, 0xde6c4db9, "c6K"}, // Paged Prog. Mem Lo
                                                 {NO_BANK_SWITCH, 0x00A00001, 0x8000, s_romData2n_c6R, 0xaff23b5a, "c6R"}, // Paged Prog. Mem Hi
                                                 {NO_BANK_SWITCH, 0x00B00000, 0x8000, s_romData2n_c6L, 0x92f5392c, "c6L"}, // Paged Prog. Mem Lo
                                                 {NO_BANK_SWITCH, 0x00B00001, 0x8000, s_romData2n_c6S, 0x0381f362, "c6S"}, // Paged Prog. Mem Hi
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


