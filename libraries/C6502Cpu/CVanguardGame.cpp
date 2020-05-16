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
#include "CVanguardGame.h"


//                                       01   02   04   08   10   20   40   80  100  200  400  800
static const UINT16 s_romData2n_1[] = {0x6f,0xa9,0x9d,0x10,0x8e,0x8a,0x02,0x02,0xba,0x02,0x60,0xc9}; // 1
static const UINT16 s_romData2n_2[] = {0xc9,0x06,0x60,0x34,0x00,0x48,0x32,0x85,0x38,0x68,0x54,0xc9}; // 2
static const UINT16 s_romData2n_3[] = {0x29,0x03,0xbd,0x12,0x10,0x60,0xb0,0x90,0x51,0x51,0x51,0x4c}; // 3
static const UINT16 s_romData2n_4[] = {0x48,0x8a,0x98,0xb5,0xe6,0x50,0x20,0x68,0x5f,0xed,0x48,0x3b}; // 4
static const UINT16 s_romData2n_5[] = {0x7c,0xc2,0x86,0x02,0x62,0x08,0x0c,0x9e,0xf8,0x00,0x88,0x38}; // 5
static const UINT16 s_romData2n_6[] = {0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x32}; // 6
static const UINT16 s_romData2n_7[] = {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xd4}; // 7
static const UINT16 s_romData2n_8[] = {0x01,0xd1,0xd1,0xd1,0xd7,0xd2,0xc9,0x30,0xc4,0x01,0x20,0x14}; // 8

//
// Centuri Set 1
//
static const ROM_REGION s_romRegionCenturiSet1[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2n_1, 0x6a29e354, " 1 "}, // 1
                                                            {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2n_2, 0x302bba54, " 2 "}, // 2
                                                            {NO_BANK_SWITCH, 0x6000, 0x1000, s_romData2n_3, 0x424755f6, " 3 "}, // 3
                                                            {NO_BANK_SWITCH, 0x7000, 0x1000, s_romData2n_4, 0x770f9714, " 4 "}, // 4CN
                                                            {NO_BANK_SWITCH, 0x8000, 0x1000, s_romData2n_5, 0x3445cba6, " 5 "}, // 5C
                                                            {NO_BANK_SWITCH, 0x9000, 0x1000, s_romData2n_6, 0x0d5b47d0, " 6 "}, // 6
                                                            {NO_BANK_SWITCH, 0xA000, 0x1000, s_romData2n_7, 0x8549b8f8, " 7 "}, // 7
                                                            {NO_BANK_SWITCH, 0xB000, 0x1000, s_romData2n_8, 0x4b825bc8, " 8 "}, // 8CS
                                                            {NO_BANK_SWITCH, 0xF000, 0x1000, s_romData2n_5, 0x3445cba6, " 5 "}, // 5C, mirror
                                                            {0} }; // end of list

//
// German Set 1
//
static const ROM_REGION s_romRegionGermanSet1[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2n_1, 0x6a29e354, " 1 "}, // 1
                                                            {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2n_2, 0x302bba54, " 2 "}, // 2
                                                            {NO_BANK_SWITCH, 0x6000, 0x1000, s_romData2n_3, 0x424755f6, " 3 "}, // 3
                                                            {NO_BANK_SWITCH, 0x7000, 0x1000, s_romData2n_4, 0x4a82306a, " 4 "}, // 4G
                                                            {NO_BANK_SWITCH, 0x8000, 0x1000, s_romData2n_5, 0xfde157d0, " 5 "}, // 5
                                                            {NO_BANK_SWITCH, 0x9000, 0x1000, s_romData2n_6, 0x0d5b47d0, " 6 "}, // 6
                                                            {NO_BANK_SWITCH, 0xA000, 0x1000, s_romData2n_7, 0x8549b8f8, " 7 "}, // 7
                                                            {NO_BANK_SWITCH, 0xB000, 0x1000, s_romData2n_8, 0xabe5fa3f, " 8 "}, // 8S
                                                            {NO_BANK_SWITCH, 0xF000, 0x1000, s_romData2n_5, 0xfde157d0, " 5 "}, // 5, mirror
                                                            {0} }; // end of list


IGame*
CVanguardGame::createInstanceCenturiSet1(
)
{
    return (new CVanguardGame(s_romRegionCenturiSet1));
}

IGame*
CVanguardGame::createInstanceGermanSet1(
)
{
    return (new CVanguardGame(s_romRegionGermanSet1));
}

CVanguardGame::CVanguardGame(
    const ROM_REGION *romRegion
) : CVanguardBaseGame( romRegion )
{
}


