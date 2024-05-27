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
#include "CCrazyKongGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// "ckongpt2" from MAME set 1
//
static const ROM_DATA2N s_romData2nPt2Set1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                          {0x00,0x32,0xa0,0x3a,0x3a,0x21,0x19,0x3a,0x14,0x11,0xc2,0x56}, //  7 5D
                                                          {0x0c,0x3e,0xa6,0x10,0x2a,0x21,0x69,0xb0,0x80,0x6a,0x1a,0xcd}, //  8 5E
                                                          {0xca,0x5f,0x2d,0x33,0x05,0xd4,0xdd,0xc3,0x21,0xc0,0x1e,0xdd}, //  9 5H
                                                          {0xcb,0x13,0xc6,0xc9,0x0f,0x31,0xfe,0x70,0x02,0x60,0xc9,0x1f}, // 10 5K
                                                          {0x27,0x62,0x49,0x92,0xa4,0x00,0x3e,0xcd,0x06,0x36,0x89,0xf4}, // 11 5L
                                                          {0x5c,0x38,0x32,0x32,0xff,0xc4,0x22,0xfd,0x00,0x0e,0xd1,0x4a}, // 12 5N
                                                          {0} };  // end of list

//
// "ckongpt2" from MAME set 1
//
static const ROM_REGION s_romRegionPt2Set1[] PROGMEM = { //
                                                        {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nPt2Set1[0].data2n, 0xb27df032, " 5D"}, //
                                                        {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nPt2Set1[1].data2n, 0x5dc1aaba, " 5E"}, //
                                                        {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nPt2Set1[2].data2n, 0xc9054c94, " 5H"}, //
                                                        {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nPt2Set1[3].data2n, 0x069c4797, " 5K"}, //
                                                        {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nPt2Set1[4].data2n, 0xae159192, " 5L"}, //
                                                        {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nPt2Set1[5].data2n, 0x966bc9ab, " 5N"}, //
                                                        {0} }; // end of list

//
// "ckongpt2z"
//
static const ROM_REGION s_romRegionPt2ZacSet1[] PROGMEM = { //
                                                           {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nPt2Set1[0].data2n, 0x9b5e7592, " 5D"}, // diff
                                                           {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nPt2Set1[1].data2n, 0x5dc1aaba, " 5E"}, //
                                                           {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nPt2Set1[2].data2n, 0xc9054c94, " 5H"}, //
                                                           {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nPt2Set1[3].data2n, 0xc3beb501, " 5K"}, // diff
                                                           {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nPt2Set1[4].data2n, 0xae159192, " 5L"}, //
                                                           {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nPt2Set1[5].data2n, 0x966bc9ab, " 5N"}, //
                                                           {0} }; // end of list

IGame*
CCrazyKongGame::createInstancePt2Set1(
)
{
    return (new CCrazyKongGame(s_romData2nPt2Set1, s_romRegionPt2Set1));
}


IGame*
CCrazyKongGame::createInstancePt2ZacSet1(
)
{
    return (new CCrazyKongGame(s_romData2nPt2Set1, s_romRegionPt2ZacSet1));
}


CCrazyKongGame::CCrazyKongGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CCrazyKongBaseGame(  romData2n, romRegion )
{
}


