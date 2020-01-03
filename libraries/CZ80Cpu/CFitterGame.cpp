//
// Copyright (c) 2020, Paul R. Swan
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
#include "CFitterGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Taito "fitter" Set 1 from MAME
//
static const ROM_DATA2N s_romData2nTaitoSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                            {0x00,0x00,0x32,0x02,0x00,0x84,0x00,0x32,0xcd,0x70,0x21,0xfe}, // ic38.bin
                                                            {0x04,0x05,0x08,0x18,0x26,0x2b,0xcd,0xe6,0x00,0x00,0xcd,0x3a}, // roundup.u39
                                                            {0x7e,0x06,0x70,0xfe,0x00,0x21,0x21,0xdd,0xdd,0x3e,0x21,0x01}, // ic40.bin
                                                            {0x8d,0x80,0xca,0x21,0xe1,0xaf,0xdd,0xc3,0x21,0xdd,0xcd,0x32}, // roundup.u41
                                                            {0x70,0x80,0x20,0x32,0x42,0x40,0x6b,0x3d,0xfe,0x01,0x00,0x00}, // ic33.bin
                                                            {0} };  // end of list

//
// Taito "fitter" Set 1 from MAME
//
static const ROM_REGION s_romRegionTaitoSet1[] PROGMEM = { //
                                                           {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nTaitoSet1[0].data2n, 0x6bf6cca4, "c38"}, //
                                                           {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nTaitoSet1[1].data2n, 0x37bf554b, "c39"}, //
                                                           {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nTaitoSet1[2].data2n, 0x572e2157, "c40"}, //
                                                           {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nTaitoSet1[3].data2n, 0x1c5ed660, "c41"}, //
                                                           {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nTaitoSet1[4].data2n, 0xab47c6c2, "c33"}, //
                                                           {0} }; // end of list

//
// Kaga Denshi Fitter Set 1, not in MAME at time of writing
//
static const ROM_REGION s_romRegionKagaDenshiSet1[] PROGMEM = { //
                                                                {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nTaitoSet1[0].data2n, 0xb980fecd, "c38"}, //
                                                                {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nTaitoSet1[1].data2n, 0x37bf554b, "c39"}, //
                                                                {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nTaitoSet1[2].data2n, 0x572e2157, "c40"}, //
                                                                {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nTaitoSet1[3].data2n, 0xa67d5bda, "c41"}, //
                                                                {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nTaitoSet1[4].data2n, 0xbc4ab3b6, "c33"}, //
                                                                {0} }; // end of list

IGame*
CFitterGame::createInstanceTaitoSet1(
)
{
    return (new CFitterGame(s_romData2nTaitoSet1, s_romRegionTaitoSet1));
}


IGame*
CFitterGame::createInstanceKagaDenshiSet1(
)
{
    return (new CFitterGame(s_romData2nTaitoSet1, s_romRegionKagaDenshiSet1));
}


CFitterGame::CFitterGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CFitterBaseGame( romData2n, romRegion )
{
}


