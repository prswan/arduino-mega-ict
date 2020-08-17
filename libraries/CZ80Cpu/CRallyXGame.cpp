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
#include "CRallyXGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// "rallyx" from MAME.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x00,0x38,0x00,0xc3,0x00,0x00,0x00,0x47,0x4f,0xc5,0x7e,0x82}, // 1B
                                                       {0x1e,0xc3,0x0c,0xe4,0x28,0x80,0x32,0x08,0x48,0x5b,0x91,0x38}, // 1E
                                                       {0x53,0x28,0x14,0x44,0x52,0x52,0x45,0x30,0x90,0x22,0x00,0x2f}, // 1H
                                                       {0x01,0xfe,0x77,0x77,0x7e,0xaf,0x60,0x80,0xff,0x00,0xff,0x31}, // 1K
                                                       {0} };  // end of list

//
// "rallyx" from MAME.
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSet1[0].data2n, 0x5882700d, " 1B"}, // 1b
                                                      {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSet1[1].data2n, 0xed1eba2b, " 1E"}, // rallyxn.1e
                                                      {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSet1[2].data2n, 0x4f98dd1c, " 1H"}, // rallyxn.1h
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nSet1[3].data2n, 0x9aacccf0, " 1K"}, // rallyxn.1k
                                                      {0} }; // end of list

//
// "rallyxm" from MAME.
//
static const ROM_DATA2N s_romData2nSet2[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x00,0x38,0x00,0xc3,0x00,0x00,0x00,0x47,0x4f,0xc5,0x7e,0x82}, // 1B
                                                       {0x1e,0xc3,0x0c,0xe4,0x28,0x80,0x32,0x08,0x48,0x5b,0x91,0x38}, // 1E
                                                       {0x53,0x28,0x14,0x44,0x52,0x52,0x45,0x30,0x90,0x22,0x00,0x2f}, // 1H
                                                       {0x01,0xfe,0x77,0x77,0x7e,0xaf,0x60,0x80,0xff,0x00,0xff,0x31}, // 1K
                                                       {0} };  // end of list

//
// "rallyxm" from MAME.
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSet1[0].data2n, 0x5882700d, " 1B"}, // 1b
                                                      {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSet1[1].data2n, 0x786585ec, " 1E"}, // 1e
                                                      {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSet1[2].data2n, 0x110d7dcd, " 1H"}, // 1h
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nSet1[3].data2n, 0x473ab447, " 1K"}, // 1k
                                                      {0} }; // end of list

IGame*
CRallyXGame::createInstanceSet1(
)
{
    return (new CRallyXGame(s_romData2nSet1, s_romRegionSet1));
}


IGame*
CRallyXGame::createInstanceSet2(
)
{
    return (new CRallyXGame(s_romData2nSet2, s_romRegionSet2));
}


CRallyXGame::CRallyXGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CRallyXBaseGame(  romData2n, romRegion )
{
}


