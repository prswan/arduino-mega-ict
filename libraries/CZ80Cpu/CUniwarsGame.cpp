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
#include "CUniwarsGame.h"

//
// See the base game cpp comments for details about this platform.
//
// Watchdog Disable:
//   Tie CPU board IC D5 (74LS00) pin 9 or R113 to GND.
//

//
// Set 1 "uniwars" from MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0x32,0x01,0xc3,0x3a,0x77,0x85,0xe5,0x80,0x32,0x21,0x08},//
                                                       {0x11,0x03,0xcd,0x00,0xca,0x00,0x3a,0x65,0x2c,0x04,0x04},//
                                                       {0x35,0x09,0xa5,0x13,0x77,0x2c,0x05,0x62,0x04,0xe8,0x0f},//
                                                       {0xc9,0xdd,0x04,0x07,0x18,0x0f,0xc0,0x8a,0x2c,0xc6,0x21},//
                                                       {0xa2,0x40,0x1e,0x10,0x7e,0x2c,0x20,0xcb,0x50,0xad,0x45},//
                                                       {0x35,0x7e,0xff,0x01,0x1a,0x3e,0x62,0x04,0xc3,0xfa,0xb7},//
                                                       {0xdd,0x36,0x00,0xc9,0x00,0xed,0xc9,0x77,0x78,0x05,0x77},//
                                                       {0x00,0x00,0xaf,0x43,0x21,0x3a,0x41,0x00,0x20,0x00,0x02},//
                                                       {0} };  // end of list

//
// Set 1 "galaxianm" from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet1[0].data2n, 0xd975af10, "GG1"}, // f07_1a.bin
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet1[1].data2n, 0xb2ed14c3, "GG2"}, // h07_2a.bin
                                                      {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSet1[2].data2n, 0x945f4160, "GG3"}, // k07_3a.bin
                                                      {NO_BANK_SWITCH, 0x1800, 0x0800, s_romData2nSet1[3].data2n, 0xddc80bc5, "GG4"}, // m07_4a.bin
                                                      {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nSet1[4].data2n, 0x62354351, "GG5"}, // d08p_5a.bin
                                                      {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nSet1[5].data2n, 0x270a3f4d, "GG6"}, // gg6
                                                      {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2nSet1[6].data2n, 0xc9245346, "GG7"}, // m08p_7a.bin
                                                      {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2nSet1[7].data2n, 0x797d45c7, "GG8"}, // n08p_8a.bin
                                                      {0} }; // end of list


IGame*
CUniwarsGame::createInstanceSet1(
)
{
    return (new CUniwarsGame(s_romData2nSet1, s_romRegionSet1));
}


CUniwarsGame::CUniwarsGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGalaxianBaseGame(romData2n, romRegion)
{
}


