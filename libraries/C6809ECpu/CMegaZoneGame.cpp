//
// Copyright (c) 2019, Paul R. Swan
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
#include "CMegaZoneGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Set 1 "megazone" from MAME
//                                               01   02   04   08   10   20   40   80  100  200  400  800 1000
static const UINT16 s_romData2nSet1_H11[]   = {0x38,0x00,0x05,0x1c,0xa4,0x02,0x04,0x0f,0xb4,0xe8,0x68,0x04,0x12};
static const UINT16 s_romData2nSet1_H10[]   = {0x45,0xe8,0x48,0x95,0x03,0x88,0x05,0xff,0xa1,0xa3,0x62,0xa2,0x1b};
static const UINT16 s_romData2nSet1_H8[]    = {0xfe,0x86,0x00,0xff,0x50,0x94,0xb5,0x8f,0x28,0x05,0xcc,0x85,0x00};
static const UINT16 s_romData2nSet1_H7[]    = {0xff,0x54,0x88,0x40,0xff,0x53,0xff,0x03,0xff,0x43,0xff,0x66,0xaa};
static const UINT16 s_romData2nSet1_H6[]    = {0x41,0x8a,0x04,0xff,0x45,0xee,0x52,0xc8,0x04,0xff,0x21,0x2e,0x00};

//
// Set 1 "megazone" from MAME
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                     {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSet1_H11, 0x94b22ea8, "H11"}, // IC59
                                                     {NO_BANK_SWITCH, 0x8000, 0x2000, s_romData2nSet1_H10, 0x0468b619, "H10"}, // IC58
                                                     {NO_BANK_SWITCH, 0xA000, 0x2000, s_romData2nSet1_H8,  0xac59000c, "H8 "}, // IC57
                                                     {NO_BANK_SWITCH, 0xC000, 0x2000, s_romData2nSet1_H7,  0x1e968603, "H7 "}, // IC56
                                                     {NO_BANK_SWITCH, 0xE000, 0x2000, s_romData2nSet1_H6,  0x0888b803, "H6 "}, // IC55
                                                     {0} }; // end of list


//
// Set I "megazonei" from MAME
//                                               01   02   04   08   10   20   40   80  100  200  400  800 1000
static const UINT16 s_romData2nSetI_H11[]   = {0x38,0x00,0x05,0x1c,0xa4,0x02,0x04,0x96,0xdd,0x96,0x9f,0x02,0x09};
static const UINT16 s_romData2nSetI_H10[]   = {0x0a,0xe8,0x08,0x94,0x88,0x89,0x0b,0x2d,0x7d,0x06,0x08,0x07,0x01};
static const UINT16 s_romData2nSetI_H8[]    = {0x85,0x03,0x04,0xf4,0x8c,0x02,0x1f,0xfc,0x05,0x00,0x07,0x06,0x12};
static const UINT16 s_romData2nSetI_H7[]    = {0x04,0xff,0x45,0xc8,0x42,0xff,0x03,0xa8,0x41,0xff,0x57,0xff,0x40};
static const UINT16 s_romData2nSetI_H6[]    = {0x03,0x04,0xfb,0xfe,0x03,0xff,0x02,0x03,0x03,0x50,0x30,0x40,0x60};

//
// Set I "megazonei" from MAME
//
static const ROM_REGION s_romRegionSetI[] PROGMEM = { //
                                                     {NO_BANK_SWITCH, 0x6000, 0x2000, s_romData2nSetI_H11, 0xf41922a0, "H11"}, // IC59
                                                     {NO_BANK_SWITCH, 0x8000, 0x2000, s_romData2nSetI_H10, 0x7fd7277b, "H10"}, // IC58
                                                     {NO_BANK_SWITCH, 0xA000, 0x2000, s_romData2nSetI_H8,  0xa4b33b51, "H8 "}, // IC57
                                                     {NO_BANK_SWITCH, 0xC000, 0x2000, s_romData2nSetI_H7,  0x2aabcfbf, "H7 "}, // IC56
                                                     {NO_BANK_SWITCH, 0xE000, 0x2000, s_romData2nSetI_H6,  0xb33a3c37, "H6 "}, // IC55
                                                     {0} }; // end of list

IGame*
CMegaZoneGame::createInstanceSet1(
)
{
    return (new CMegaZoneGame(s_romRegionSet1));
}


IGame*
CMegaZoneGame::createInstanceSetI(
)
{
    return (new CMegaZoneGame(s_romRegionSetI));
}


CMegaZoneGame::CMegaZoneGame(
    const ROM_REGION *romRegion
) : CMegaZoneBaseGame( romRegion )
{
}


