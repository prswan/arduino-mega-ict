//
// Copyright (c) 2020, Marc Deslauriers
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
#include "CLadybugGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// "ladybug" from MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x18,0x50,0x30,0x14,0xc0,0x00,0x14,0x3a,0x31,0x77,0x10,0x14}, // C4
                                                       {0xe0,0x29,0x2b,0x2c,0x2e,0x2e,0xe2,0x18,0x0f,0x08,0x68,0xcb}, // D4
                                                       {0xca,0x03,0x11,0x19,0x4e,0x79,0xcb,0x06,0xc3,0xdd,0x3e,0x20}, // E4
                                                       {0x17,0x0e,0x0e,0x0b,0xff,0x0e,0xff,0xdd,0x3a,0xfe,0x0a,0x3a}, // H4
                                                       {0xd1,0xc1,0xc9,0x21,0x28,0x21,0x00,0x61,0xda,0x02,0x12,0x77}, // J4
                                                       {0x60,0x50,0xff,0xc3,0xc3,0xc3,0x21,0x03,0x01,0xcb,0x44,0xd9}, // K4
                                                       {0} };  // end of list

//
// "ladybug" from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSet1[0].data2n, 0xd09e0adb, " C4"}, // l1.c4
                                                      {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSet1[1].data2n, 0x88bc4a0a, " D4"}, // l2.d4
                                                      {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSet1[2].data2n, 0x53e9efce, " E4"}, // l3.e4
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nSet1[3].data2n, 0xffc424d7, " H4"}, // l4.h4
                                                      {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nSet1[3].data2n, 0xad6af809, " J4"}, // l5.j4
                                                      {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nSet1[3].data2n, 0xcf1acca4, " K4"}, // l6.k4
                                                      {0} }; // end of list

//
// "cavenger" from MAME.
//
static const ROM_DATA2N s_romData2nSet2[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x00,0x6c,0x1e,0x5e,0x77,0xff,0xff,0x0f,0x01,0x17,0xc3,0xd0}, // C4
                                                       {0x7e,0xee,0x77,0xc9,0x06,0xc1,0xb0,0x19,0x28,0x05,0xc6,0xcb}, // D4
                                                       {0x04,0x4b,0x2c,0x2e,0x2a,0x2b,0x16,0x16,0x12,0x08,0x00,0xfe}, // E4
                                                       {0x0d,0x00,0x5f,0x4f,0x80,0xd0,0x50,0x20,0x00,0x86,0xdd,0x52}, // H4
                                                       {0xfd,0x77,0xfd,0xdd,0x06,0x7e,0x06,0x06,0xcb,0x04,0x0e,0x75}, // J4
                                                       {0x50,0x39,0x3e,0x48,0x52,0x67,0x30,0x0c,0xcf,0xdd,0xfd,0x00}, // K4
                                                        {0} };  // end of list

//
// "cavenger" from MAME.
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSet2[0].data2n, 0x9e0cc781, " C4"}, // 1.c4
                                                      {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSet2[1].data2n, 0x5ce5b950, " D4"}, // 2.d4
                                                      {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSet2[2].data2n, 0xbc28218d, " E4"}, // 3.e4
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nSet2[3].data2n, 0x2b32e9f5, " H4"}, // 4.h4
                                                      {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nSet2[3].data2n, 0xd117153e, " J4"}, // 5.j4
                                                      {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nSet2[3].data2n, 0xc7d366cb, " K4"}, // 6.k4
                                                       {0} }; // end of list

IGame*
CLadybugGame::createInstanceSet1(
)
{
    return (new CLadybugGame(s_romData2nSet1, s_romRegionSet1));
}


IGame*
CLadybugGame::createInstanceSet2(
)
{
    return (new CLadybugGame(s_romData2nSet2, s_romRegionSet2));
}


CLadybugGame::CLadybugGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CLadybugBaseGame(  romData2n, romRegion )
{
}


