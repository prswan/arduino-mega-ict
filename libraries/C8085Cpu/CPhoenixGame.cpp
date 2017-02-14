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
#include "CPhoenixGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Centuri Set 1 from MAME (phoenixa)
//                                                      01   02   04   08   10   20   40   80  100  200  400
static const UINT16 s_romData2nCenturiSet1_r45[]    = {0x00,0x00,0x00,0x31,0x50,0xa7,0xc3,0x26,0x01,0x34,0x21}; //
static const UINT16 s_romData2nCenturiSet1_r46[]    = {0x14,0x08,0xb8,0x1e,0xff,0x22,0x5a,0xa0,0x3a,0x43,0xe5}; //
static const UINT16 s_romData2nCenturiSet1_r47[]    = {0x01,0x01,0x02,0x02,0x00,0x10,0x1d,0x04,0x0b,0x1c,0x30}; //
static const UINT16 s_romData2nCenturiSet1_r48[]    = {0x20,0xff,0xff,0x03,0x09,0x43,0x43,0x00,0x43,0x43,0x00}; //
static const UINT16 s_romData2nCenturiSet1_r49[]    = {0x76,0x08,0xf0,0x24,0x34,0xc2,0x21,0x29,0x0e,0xff,0xcd}; //
static const UINT16 s_romData2nCenturiSet1_r50[]    = {0x32,0x00,0x00,0x00,0x00,0x32,0x00,0xe0,0x00,0x00,0x0b}; //
static const UINT16 s_romData2nCenturiSet1_r51[]    = {0x93,0x43,0x34,0x18,0x67,0x31,0x2e,0x4f,0x12,0x33,0xcd}; //
static const UINT16 s_romData2nCenturiSet1_r52[]    = {0xc4,0x43,0x08,0x43,0x3a,0xd6,0x38,0x38,0x2c,0x3a,0xe8}; //

//
// GGI Set 1 from MAME (phoenixc)
//                                                      01   02   04   08   10   20   40   80  100  200  400
static const UINT16 s_romData2nGGISet1_r45[]        = {0x00,0x00,0x00,0x31,0x50,0xa7,0xcd,0x26,0x01,0x34,0x21}; //
static const UINT16 s_romData2nGGISet1_r48[]        = {0x20,0xff,0xff,0x03,0x09,0x43,0x43,0x43,0x43,0x43,0x00}; //

//
// Centuri Set 1 from MAME (phoenixa)
//
static const ROM_REGION s_romRegionCenturiSet1[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nCenturiSet1_r45, 0xc7a9b499, "r45"}, //
                                                            {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nCenturiSet1_r46, 0xd0e6ae1b, "r46"}, //
                                                            {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nCenturiSet1_r47, 0x64bf463a, "r47"}, //
                                                            {NO_BANK_SWITCH, 0x1800, 0x0800, s_romData2nCenturiSet1_r48, 0x1b20fe62, "r48"}, //
                                                            {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nCenturiSet1_r49, 0xa105e4e7, "r49"}, //
                                                            {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nCenturiSet1_r50, 0xac5e9ec1, "r50"}, //
                                                            {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2nCenturiSet1_r51, 0x2eab35b4, "r51"}, //
                                                            {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2nCenturiSet1_r52, 0xaff8e9c5, "r52"}, //
                                                            {0} }; // end of list

//
// GGI Set 1 from MAME (phoenixc)
//
static const ROM_REGION s_romRegionGGISet1[] PROGMEM     = { //
                                                            {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nGGISet1_r45,     0x5b8c55a8, "r45"}, //
                                                            {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nCenturiSet1_r46, 0xdbc942fa, "r46"}, //
                                                            {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nCenturiSet1_r47, 0xcbbb8839, "r47"}, //
                                                            {NO_BANK_SWITCH, 0x1800, 0x0800, s_romData2nGGISet1_r48,     0x5ae0b215, "r48"}, //
                                                            {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nCenturiSet1_r49, 0x1a1ce0d0, "r49"}, //
                                                            {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nCenturiSet1_r50, 0xac5e9ec1, "r50"}, //
                                                            {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2nCenturiSet1_r51, 0x2eab35b4, "r51"}, //
                                                            {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2nCenturiSet1_r52, 0x8424d7c4, "r52"}, //
                                                            {0} }; // end of list

//
// GGI Set 2 from MAME (phoenixc2)
//
static const ROM_REGION s_romRegionGGISet2[] PROGMEM     = { //
                                                            {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nGGISet1_r45,     0x5b8c55a8, "r45"}, //
                                                            {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nCenturiSet1_r46, 0xdbc942fa, "r46"}, //
                                                            {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nCenturiSet1_r47, 0xcbbb8839, "r47"}, //
                                                            {NO_BANK_SWITCH, 0x1800, 0x0800, s_romData2nGGISet1_r48,     0xf28e16d8, "r48"}, //
                                                            {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nCenturiSet1_r49, 0x1a1ce0d0, "r49"}, //
                                                            {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nCenturiSet1_r50, 0xac5e9ec1, "r50"}, //
                                                            {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2nCenturiSet1_r51, 0x2eab35b4, "r51"}, //
                                                            {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2nCenturiSet1_r52, 0x8424d7c4, "r52"}, //
                                                            {0} }; // end of list

//
// GGI Set 5
//
static const ROM_REGION s_romRegionGGISet5[] PROGMEM     = { //
                                                            {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2nGGISet1_r45,     0x5b8c55a8, "r45"}, //
                                                            {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2nCenturiSet1_r46, 0xdbc942fa, "r46"}, //
                                                            {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2nCenturiSet1_r47, 0xcbbb8839, "r47"}, //
                                                            {NO_BANK_SWITCH, 0x1800, 0x0800, s_romData2nGGISet1_r48,     0xe4aa0b0b, "r48"}, //
                                                            {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2nCenturiSet1_r49, 0x1a1ce0d0, "r49"}, //
                                                            {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2nCenturiSet1_r50, 0xac5e9ec1, "r50"}, //
                                                            {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2nCenturiSet1_r51, 0x2eab35b4, "r51"}, //
                                                            {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2nCenturiSet1_r52, 0x8424d7c4, "r52"}, //
                                                            {0} }; // end of list

IGame*
CPhoenixGame::createInstanceCenturiSet1(
)
{
    return (new CPhoenixGame(s_romRegionCenturiSet1));
}


IGame*
CPhoenixGame::createInstanceGGISet1(
)
{
    return (new CPhoenixGame(s_romRegionGGISet1));
}


IGame*
CPhoenixGame::createInstanceGGISet2(
)
{
    return (new CPhoenixGame(s_romRegionGGISet2));
}


IGame*
CPhoenixGame::createInstanceGGISet5(
)
{
    return (new CPhoenixGame(s_romRegionGGISet5));
}


CPhoenixGame::CPhoenixGame(
    const ROM_REGION *romRegion
) : CPhoenixBaseGame( romRegion )
{
}


