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
#include "CNichibutsuMoonBaseGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Set 1 from MAME (moonbase)
//                                               01   02   04   08   10   20   40   80  100  200
static const UINT16 s_romData2nSet1_4A[]    = {0x00,0x00,0x8f,0xf5,0xf5,0xdb,0x27,0x20,0x05,0xe1}; //
static const UINT16 s_romData2nSet1_4C[]    = {0x2a,0x20,0x86,0x23,0x04,0x7e,0x77,0x11,0xc3,0x06}; //
static const UINT16 s_romData2nSet1_4E[]    = {0x3a,0xef,0xa7,0x22,0xc9,0x06,0x83,0xaf,0x08,0x3a}; //
static const UINT16 s_romData2nSet1_4F[]    = {0xe1,0xfe,0xc2,0x00,0xda,0x09,0x09,0x01,0xcd,0x06}; //
static const UINT16 s_romData2nSet1_4H[]    = {0x4e,0x23,0x0d,0x3a,0xc3,0x97,0x12,0x3e,0x1b,0x61}; //
static const UINT16 s_romData2nSet1_4L[]    = {0xfe,0xff,0x21,0x23,0xc0,0xe8,0x23,0x14,0xcd,0x50}; //
static const UINT16 s_romData2nSet1_5A[]    = {0x0b,0xa7,0xf5,0x66,0xc3,0x10,0x00,0x00,0xcd,0x17}; //
static const UINT16 s_romData2nSet1_5C[]    = {0x00,0x00,0x79,0xfa,0x00,0x00,0x00,0x00,0x0c,0x0e}; //
static const UINT16 s_romData2nSet1_5E[]    = {0x01,0x01,0x01,0x01,0x01,0x03,0x01,0x00,0x01,0x79}; //
static const UINT16 s_romData2nSet1_5F[]    = {0x20,0xe6,0x32,0xb7,0x3a,0x32,0x44,0x34,0x07,0x01}; //

//
// Set 1 from MAME (moonbase)
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2nSet1_4A, 0x82dbf2c7, "r4A"}, //
                                                      {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2nSet1_4C, 0xc867f5b4, "r4C"}, //
                                                      {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2nSet1_4E, 0xcb23ccc1, "r4E"}, //
                                                      {NO_BANK_SWITCH, 0x0C00, 0x0400, s_romData2nSet1_4F, 0x9a11abe2, "r4F"}, //
                                                      {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2nSet1_4H, 0x2b105ed3, "r4H"}, //
                                                      {NO_BANK_SWITCH, 0x1400, 0x0400, s_romData2nSet1_4L, 0xcb3d6dcb, "r4L"}, //
                                                      {NO_BANK_SWITCH, 0x1800, 0x0400, s_romData2nSet1_5A, 0x774b52c9, "r5A"}, //
                                                      {NO_BANK_SWITCH, 0x1C00, 0x0400, s_romData2nSet1_5C, 0xe88ea83b, "r5C"}, //
                                                      {NO_BANK_SWITCH, 0x4000, 0x0400, s_romData2nSet1_5E, 0x2dd5adfa, "r5E"}, //
                                                      {NO_BANK_SWITCH, 0x4400, 0x0400, s_romData2nSet1_5F, 0x1e7c22a4, "r5F"}, //
                                                      {0} }; // end of list

//
// Set 2 from MAME (moonbasea)
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2nSet1_4A, 0x82dbf2c7, "r4A"}, //
                                                      {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2nSet1_4C, 0xc867f5b4, "r4C"}, //
                                                      {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2nSet1_4E, 0xcb23ccc1, "r4E"}, //
                                                      {NO_BANK_SWITCH, 0x0C00, 0x0400, s_romData2nSet1_4F, 0x86a00411, "r4F"}, //
                                                      {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2nSet1_4H, 0x2b105ed3, "r4H"}, //
                                                      {NO_BANK_SWITCH, 0x1400, 0x0400, s_romData2nSet1_4L, 0xcb3d6dcb, "r4L"}, //
                                                      {NO_BANK_SWITCH, 0x1800, 0x0400, s_romData2nSet1_5A, 0x774b52c9, "r5A"}, //
                                                      {NO_BANK_SWITCH, 0x1C00, 0x0400, s_romData2nSet1_5C, 0xe88ea83b, "r5C"}, //
                                                      {NO_BANK_SWITCH, 0x4000, 0x0400, s_romData2nSet1_5E, 0x2dd5adfa, "r5E"}, //
                                                      {NO_BANK_SWITCH, 0x4400, 0x0400, s_romData2nSet1_5F, 0x1e7c22a4, "r5F"}, //
                                                      {0} }; // end of list


IGame*
CNichibutsuMoonBaseGame::createInstanceSet1(
)
{
    return (new CNichibutsuMoonBaseGame(s_romRegionSet1));
}


IGame*
CNichibutsuMoonBaseGame::createInstanceSet2(
)
{
    return (new CNichibutsuMoonBaseGame(s_romRegionSet2));
}


CNichibutsuMoonBaseGame::CNichibutsuMoonBaseGame(
    const ROM_REGION *romRegion
) : CSpaceInvadersBaseGame( romRegion )
{
}


