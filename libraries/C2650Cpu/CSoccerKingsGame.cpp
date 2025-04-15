//
// Copyright (c) 2025, Paul R. Swan
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
#include "CSoccerKingsGame.h"

static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0x20,0x92,0x02,0xf0,0x59,0x7e,0xc3,0xb0,0x0d,0xe4,0x88},//
                                                       {0x38,0xcc,0x7a,0xcc,0x26,0xf1,0xe4,0x05,0x1a,0x05,0xf0},//
                                                       {0xce,0x1c,0x04,0x79,0x1c,0x3f,0x09,0x0f,0xee,0x14,0x18},//
                                                       {0x4f,0x49,0x53,0x50,0x57,0x49,0x3b,0x20,0x18,0x03,0x21},//
                                                       {0x3c,0x49,0x31,0x48,0x47,0x0c,0x18,0x0c,0x3e,0x20,0x5e},//
                                                       {0x59,0x75,0x02,0x02,0x05,0x05,0x05,0x24,0xc2,0x0f,0x40},//
                                                       {0} };  // end of list


//
// From IPDB, Doug Cope
//
// The ROMs are not addressed in order so we need to split hi and lo
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet1[0].data2n, 0xaae982f1, "1lo"}, // IC 1
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet1[1].data2n, 0x81882178, "2lo"}, // IC 2
                                                      {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSet1[2].data2n, 0xdbac52be, "3lo"}, // IC 3
                                                      {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nSet1[3].data2n, 0xbc4eea6d, "1hi"}, // IC 1
                                                      {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nSet1[4].data2n, 0xdebbd604, "2hi"}, // IC 2
                                                      {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2nSet1[5].data2n, 0xde1621f2, "3hi"}, // IC 3
                                                      {0} }; // end of list


static const ROM_DATA2N s_romData2nSetA[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0x20,0x92,0x02,0xf0,0x59,0x7e,0xc3,0xb0,0x0d,0xe4,0x88},//
                                                       {0x38,0xcc,0x7a,0xcc,0x26,0xf1,0xe4,0x05,0x1a,0x05,0xf0},//
                                                       {0xce,0x1c,0x04,0x79,0x1c,0x3f,0x09,0x0f,0xee,0x14,0xec},//
                                                       {0x4f,0x49,0x53,0x50,0x57,0x49,0x3b,0x20,0x31,0x03,0x1e},//
                                                       {0x48,0x46,0x33,0x31,0x48,0x5d,0x3b,0xe6,0x2c,0x98,0x3b},//
                                                       {0x46,0xc2,0x5c,0x59,0x18,0x1a,0x98,0xcc,0xce,0x0d,0x18},//
                                                       {0} };  // end of list


//
// Dump from my own PCB I found in a box. DIfferent version, 02 & AL
//
// The ROMs are not addressed in order so we need to split hi and lo
//
static const ROM_REGION s_romRegionSetA[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nSet1[0].data2n, 0x8e57a616, "1lo"}, // IC 1 - 02
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nSet1[1].data2n, 0xded3d380, "2lo"}, // IC 2 - 02
                                                      {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nSet1[2].data2n, 0xf67100dc, "3lo"}, // IC 3 - AL
                                                      {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nSet1[3].data2n, 0xbb3761b6, "1hi"}, // IC 1 - 02
                                                      {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nSet1[4].data2n, 0x0ec6cfb9, "2hi"}, // IC 2 - 02
                                                      {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2nSet1[5].data2n, 0x365faf6c, "3hi"}, // IC 3 - AL
                                                      {0} }; // end of list


IGame*
CSoccerKingsGame::createInstanceSet1(
)
{
    return (new CSoccerKingsGame(s_romData2nSet1, s_romRegionSet1));
}


IGame*
CSoccerKingsGame::createInstanceSetA(
)
{
    return (new CSoccerKingsGame(s_romData2nSetA, s_romRegionSetA));
}


CSoccerKingsGame::CSoccerKingsGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CPinball1B1165BaseGame( romData2n, romRegion )
{
}


