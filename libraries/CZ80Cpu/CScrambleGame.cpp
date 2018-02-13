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
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0x32,0x01,0xc3,0x77,0x77,0x85,0x7e,0x4e,0x82,0x70,0x2b}, // 0 - 2C
                                                       {0x78,0xd1,0xd1,0xd1,0xd1,0xd1,0xd1,0xd1,0xd1,0xd1,0x1f}, // 1 - 2E
                                                       {0x06,0xff,0x0c,0x40,0x0f,0x11,0x40,0xc9,0x14,0x1d,0x39}, // 2 - 2F
                                                       {0x40,0x35,0x36,0x3e,0x22,0x07,0x23,0x19,0x06,0x05,0x40}, // 3 - 2H
                                                       {0x2d,0x05,0xf7,0x10,0x3d,0x10,0xcd,0x07,0x05,0xdd,0x0f}, // 4 - 2J
                                                       {0x03,0xc6,0xfe,0x7e,0x09,0x00,0x3e,0x03,0xdd,0xfd,0x00}, // 5 - 2L
                                                       {0xc9,0x3a,0x42,0x1b,0x41,0x11,0x01,0x5e,0xc3,0x30,0x00}, // 6 - 2M
                                                       {0xd8,0xd1,0xd1,0xd1,0xc7,0xc7,0xd8,0xd1,0xd1,0xd1,0xff}, // 7 - 2P
                                                       {0} };  // end of list

//
// Stern Set 1 from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet1[0].data2n, 0xb89207a1, " 2C"}, // 2d(__scrambles)
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet1[1].data2n, 0xe9b4b9eb, " 2E"}, // 2e(__scrambles)
                                                      {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSet1[2].data2n, 0xa1f14f4c, " 2F"}, // 2f(__scrambles)"
                                                      {NO_BANK_SWITCH, 0x1800, 0x0800, s_romData2nSet1[3].data2n, 0x591bc0d9, " 2H"}, // 2h(__scrambles)"
                                                      {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nSet1[4].data2n, 0x22f11b6b, " 2J"}, // 2j(__scrambles)
                                                      {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nSet1[5].data2n, 0x705ffe49, " 2L"}, // 2l(__scrambles)
                                                      {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2nSet1[6].data2n, 0xea26c35c, " 2M"}, // 2m(__scrambles)
                                                      {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2nSet1[7].data2n, 0x94d8f5e3, " 2P"}, // 2p(__scrambles)
                                                      {0} }; // end of list


//
// Found on a Zaccaria Scramble PCB set that uses 2708 EPROMS.
// Maps to "scramblebb" in MAME.
//
static const ROM_DATA2N s_romData2nZacSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200
                                                          {0x32,0x01,0xc3,0x77,0x77,0x85,0x7e,0x32,0x32,0x02}, //  0 - 2XC
                                                          {0x18,0x04,0xfe,0x30,0x3f,0x7b,0x11,0x06,0x4e,0x31}, //  1 - 2P
                                                          {0x2b,0x71,0xfc,0xdd,0xe6,0x0e,0x00,0x07,0x3a,0xc4}, //  2 - 2XE
                                                          {0x40,0x36,0x21,0xaf,0x06,0xc0,0x2f,0x06,0x2c,0x05}, //  3 - 2M
                                                          {0x32,0x07,0x32,0x0a,0x40,0x11,0x32,0x22,0x41,0x3a}, //  4 - 2XF
                                                          {0x7e,0xdd,0x0d,0xe5,0x0a,0x06,0x28,0x2c,0xdd,0x21}, //  5 - 2L
                                                          {0x8c,0x43,0x36,0x36,0x43,0xdd,0x01,0x04,0x19,0x02}, //  6 - 2XH
                                                          {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x88,0x0f,0x02}, //  7 - 2J
                                                          {0x20,0xcd,0x20,0xcd,0x21,0xcd,0x10,0x17,0xbb,0x05}, //  8 - 2XJ
                                                          {0x36,0x00,0xa0,0x21,0x3a,0x40,0x46,0x02,0x3a,0x7f}, //  9 - 2H
                                                          {0xef,0x32,0x42,0xc9,0xf7,0xc9,0x8d,0xc3,0x0d,0x2e}, // 10 - 2XL
                                                          {0xd0,0x36,0x36,0x36,0x33,0x30,0x36,0x36,0x35,0x30}, // 11 - 2F
                                                          {0x36,0x48,0x50,0x36,0xd0,0x60,0x36,0x28,0x28,0x36}, // 12 - 2XM
                                                          {0x01,0x60,0x60,0x48,0x58,0x58,0x88,0x70,0x80,0x48}, // 13 - 2E
                                                          {0x00,0x60,0x60,0x37,0xd1,0xd1,0x02,0x40,0xa0,0xc7}, // 14 - 2XP
                                                          {0x00,0x48,0x48,0xc8,0x98,0xc8,0xc8,0xff,0xff,0xff}, // 15 - 2C
                                                          {0} };  // end of list


static const ROM_REGION s_romRegionZacSet1[] PROGMEM = { //
                                                         {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2nZacSet1[ 0].data2n, 0x3fb6cb36, "2XC"}, // 1 - Lo
                                                         {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2nZacSet1[ 1].data2n, 0x40c5d8de, " 2P"}, // 1 - Hi
                                                         {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2nZacSet1[ 2].data2n, 0x02170b0e, "2XE"}, // 2 - Lo
                                                         {NO_BANK_SWITCH, 0x0C00, 0x0400, s_romData2nZacSet1[ 3].data2n, 0xe66d79b8, " 2M"}, // 2 - Hi
                                                         {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2nZacSet1[ 4].data2n, 0x4f12e016, "2XF"}, // 3 - Lo
                                                         {NO_BANK_SWITCH, 0x1400, 0x0400, s_romData2nZacSet1[ 5].data2n, 0x859418f6, " 2L"}, // 3 - Hi
                                                         {NO_BANK_SWITCH, 0x1800, 0x0400, s_romData2nZacSet1[ 6].data2n, 0x84cb9f1b, "2XH"}, // 4 - Lo
                                                         {NO_BANK_SWITCH, 0x1C00, 0x0400, s_romData2nZacSet1[ 7].data2n, 0xca4e400e, " 2J"}, // 4 - Hi
                                                         {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2nZacSet1[ 8].data2n, 0xa3262038, "2XJ"}, // 5 - Lo
                                                         {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2nZacSet1[ 9].data2n, 0x27662cf4, " 2H"}, // 5 - Hi
                                                         {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2nZacSet1[10].data2n, 0xdb342d11, "2XL"}, // 6 - Lo
                                                         {NO_BANK_SWITCH, 0x2C00, 0x0400, s_romData2nZacSet1[11].data2n, 0x90d81bb5, " 2F"}, // 6 - Hi
                                                         {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2nZacSet1[12].data2n, 0x19aabef4, "2XM"}, // 7 - Lo
                                                         {NO_BANK_SWITCH, 0x3400, 0x0400, s_romData2nZacSet1[13].data2n, 0xf7cbd05f, " 2E"}, // 7 - Hi
                                                         {NO_BANK_SWITCH, 0x3800, 0x0400, s_romData2nZacSet1[14].data2n, 0x5c878c22, "2XP"}, // 8 - Lo
                                                         {NO_BANK_SWITCH, 0x3C00, 0x0400, s_romData2nZacSet1[15].data2n, 0x01c3b5fe, " 2C"}, // 8 - Hi
                                                         {0} }; // end of list


IGame*
CScrambleGame::createInstanceSet1(
)
{
    return (new CScrambleGame(s_romData2nSet1, s_romRegionSet1));
}


IGame*
CScrambleGame::createInstanceZacSet1(
)
{
    return (new CScrambleGame(s_romData2nZacSet1, s_romRegionZacSet1));
}


CScrambleGame::CScrambleGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CScrambleBaseGame( SCRAMBLE, romData2n, romRegion )
{
}


