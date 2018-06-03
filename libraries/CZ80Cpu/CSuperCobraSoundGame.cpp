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
#include "CSuperCobraSoundGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Stern Set 1 from MAME.
//
static const ROM_DATA2N s_romData2nSternSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                            {0xa4,0x02,0xff,0xff,0xff,0xff,0xb7,0xe1,0x16,0xc9,0x38}, //  0 - 5C
                                                            {0x28,0x20,0x04,0x05,0x0b,0x03,0x09,0x16,0x00,0x3b,0x00}, //  1 - 5D
                                                            {0x06,0x05,0x3b,0x06,0x80,0x32,0xcd,0xa5,0x40,0xe7,0x3e}, //  2 - 5E
                                                            {0} };  // end of list

//
// Stern Set 1 from MAME.
//
static const ROM_REGION s_romRegionSternSet1[] PROGMEM = { //
                                                           {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSternSet1[0].data2n, 0xdeeb0dd3, " 5C"}, // epr1275.5c (scobras)
                                                           {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSternSet1[1].data2n, 0x872c1a74, " 5D"}, // epr1276.5d (scobras)
                                                           {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSternSet1[2].data2n, 0xccd7a110, " 5E"}, // epr1277.5e (scobras)
                                                           {0} }; // end of list


IGame*
CSuperCobraSoundGame::createInstanceSternSet1(
)
{
    return (new CSuperCobraSoundGame(s_romData2nSternSet1, s_romRegionSternSet1));
}


CSuperCobraSoundGame::CSuperCobraSoundGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CScrambleSoundBaseGame( romData2n, romRegion )
{
}


