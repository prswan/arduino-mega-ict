//
// Copyright (c) 2026, patzik
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
#include "CTimePilotGame.h"

//
// ROM sets, addresses, lengths & CRCs are from MAME (src/mame/konami/timeplt.cpp).
// The data2n samples were generated from the MAME ROM images.
//
// MAME doesn't give the PCB locations of the program ROMs so the location
// shown is the Konami ROM number of the parent set (tm1, tm2, tm3).
//

//
// Konami (timeplt)
//
static const ROM_DATA2N s_romData2nKonamiSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800 1000
                                                             {0xb1,0x07,0xff,0x85,0x87,0x7b,0x7e,0x23,0xa7,0xc9,0x0e,0x3d,0x00}, // tm1
                                                             {0xf1,0xf1,0xf1,0xf1,0xdd,0x04,0xdd,0xb2,0xc0,0x7c,0xfe,0x33,0xdc}, // tm2
                                                             {0x20,0x05,0x17,0xcd,0xfd,0xa7,0x19,0x1c,0xc2,0xc9,0xc2,0x09,0x3c}, // tm3
                                                             {0} }; // end of list

static const ROM_REGION s_romRegionKonamiSet1[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nKonamiSet1[0].data2n, 0x1551f1b9, "tm1"}, // tm1
                                                            {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nKonamiSet1[1].data2n, 0x58636cb5, "tm2"}, // tm2
                                                            {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nKonamiSet1[2].data2n, 0xff4e0d83, "tm3"}, // tm3
                                                            {0} }; // end of list

//
// Atari license (timeplta)
//
static const ROM_DATA2N s_romData2nAtariSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800 1000
                                                            {0xc8,0x07,0xff,0x85,0x87,0x7b,0x7e,0x23,0x3a,0x04,0x2f,0x3a,0x39}, // cd_e1.bin
                                                            {0xef,0xee,0xec,0xe8,0x40,0xc0,0x2c,0xa8,0x03,0x4b,0xed,0x80,0xfd}, // cd_e2.bin
                                                            {0xfd,0x77,0xfd,0xc9,0x9a,0xdd,0xa8,0x11,0xcd,0x00,0x77,0x40,0xc3}, // cd_e3.bin
                                                            {0} }; // end of list

static const ROM_REGION s_romRegionAtariSet1[] PROGMEM = { //
                                                           {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nAtariSet1[0].data2n, 0xa4513b35, "tm1"}, // cd_e1.bin
                                                           {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nAtariSet1[1].data2n, 0x38b0c72a, "tm2"}, // cd_e2.bin
                                                           {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nAtariSet1[2].data2n, 0x83846870, "tm3"}, // cd_e3.bin
                                                           {0} }; // end of list

//
// Centuri license (timepltc)
//
static const ROM_DATA2N s_romData2nCenturiSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800 1000
                                                              {0x00,0x60,0xfe,0x85,0x87,0x7b,0x7e,0x23,0xc2,0xa5,0xaa,0x11,0xc6}, // cd1y
                                                              {0x92,0x98,0xf1,0xf1,0xf1,0xf1,0x56,0x18,0x40,0x8b,0x95,0x84,0xff}, // cd2y
                                                              {0x49,0x4a,0x4c,0x48,0xf4,0x3a,0x40,0xdd,0xd0,0x79,0xdd,0x00,0x28}, // cd3y
                                                              {0} }; // end of list

static const ROM_REGION s_romRegionCenturiSet1[] PROGMEM = { //
                                                             {NO_BANK_SWITCH, 0x0000, 0x2000, s_romData2nCenturiSet1[0].data2n, 0x83ec72c2, "tm1"}, // cd1y
                                                             {NO_BANK_SWITCH, 0x2000, 0x2000, s_romData2nCenturiSet1[1].data2n, 0x0dcf5287, "tm2"}, // cd2y
                                                             {NO_BANK_SWITCH, 0x4000, 0x2000, s_romData2nCenturiSet1[2].data2n, 0xc789b912, "tm3"}, // cd3y
                                                             {0} }; // end of list


IGame*
CTimePilotGame::createInstanceKonamiSet1(
)
{
    return (new CTimePilotGame(s_romData2nKonamiSet1, s_romRegionKonamiSet1));
}


IGame*
CTimePilotGame::createInstanceAtariSet1(
)
{
    return (new CTimePilotGame(s_romData2nAtariSet1, s_romRegionAtariSet1));
}


IGame*
CTimePilotGame::createInstanceCenturiSet1(
)
{
    return (new CTimePilotGame(s_romData2nCenturiSet1, s_romRegionCenturiSet1));
}


CTimePilotGame::CTimePilotGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CTimePilotBaseGame( romData2n, romRegion )
{
}

