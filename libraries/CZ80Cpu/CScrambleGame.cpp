//
// Copyright (c) 2015, Paul R. Swan
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
#include "CScrambleGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Stern Set 1 from MAME.
//                                              01   02   04   08   10   20   40   80  100  200  400
static const UINT16 s_romData2nSet1_2C[]    = {0x32,0x01,0xc3,0x77,0x77,0x85,0x7e,0x4e,0x82,0x70,0x2b};
static const UINT16 s_romData2nSet1_2E[]    = {0x78,0xd1,0xd1,0xd1,0xd1,0xd1,0xd1,0xd1,0xd1,0xd1,0x1f};
static const UINT16 s_romData2nSet1_2F[]    = {0x06,0xff,0x0c,0x40,0x0f,0x11,0x40,0xc9,0x14,0x1d,0x39};
static const UINT16 s_romData2nSet1_2H[]    = {0x40,0x35,0x36,0x3e,0x22,0x07,0x23,0x19,0x06,0x05,0x40};
static const UINT16 s_romData2nSet1_2J[]    = {0x2d,0x05,0xf7,0x10,0x3d,0x10,0xcd,0x07,0x05,0xdd,0x0f};
static const UINT16 s_romData2nSet1_2L[]    = {0x03,0xc6,0xfe,0x7e,0x09,0x00,0x3e,0x03,0xdd,0xfd,0x00};
static const UINT16 s_romData2nSet1_2M[]    = {0xc9,0x3a,0x42,0x1b,0x41,0x11,0x01,0x5e,0xc3,0x30,0x00};
static const UINT16 s_romData2nSet1_2P[]    = {0xd8,0xd1,0xd1,0xd1,0xc7,0xc7,0xd8,0xd1,0xd1,0xd1,0xff};

//
// Stern Set 1 from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet1_2C, 0xb89207a1, " 2C"}, // 2d(__scrambles)
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet1_2E, 0xe9b4b9eb, " 2E"}, // 2e(__scrambles)
                                                      {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSet1_2F, 0xa1f14f4c, " 2F"}, // 2f(__scrambles)"
                                                      {NO_BANK_SWITCH, 0x1800, 0x0800, s_romData2nSet1_2H, 0x591bc0d9, " 2H"}, // 2h(__scrambles)"
                                                      {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nSet1_2J, 0x22f11b6b, " 2J"}, // 2j(__scrambles)
                                                      {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nSet1_2L, 0x705ffe49, " 2L"}, // 2l(__scrambles)
                                                      {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2nSet1_2M, 0xea26c35c, " 2M"}, // 2m(__scrambles)
                                                      {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2nSet1_2P, 0x94d8f5e3, " 2P"}, // 2p(__scrambles)
                                                      {0} }; // end of list


//
// TBD
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { //
                                                      {0} }; // end of list

IGame*
CScrambleGame::createInstanceSet1(
)
{
    return (new CScrambleGame(s_romRegionSet1));
}


IGame*
CScrambleGame::createInstanceSet2(
)
{
    return (new CScrambleGame(s_romRegionSet2));
}


CScrambleGame::CScrambleGame(
    const ROM_REGION *romRegion
) : CScrambleBaseGame( romRegion )
{
}


