//
// Copyright (c) 2021, Marc Deslauriers
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
#include "CSprintGame.h"


//                                       01   02   04   08   10   20   40   80  100  200  400  800
static const UINT16 s_romData2n_1[] = {0x20,0x26,0x20,0xcc,0xcc,0x22,0x12,0x11,0x66,0x66,0x66}; // 6290-01.b1
static const UINT16 s_romData2n_2[] = {0xcc,0xcc,0xcc,0x44,0x3f,0xfb,0xa2,0xa1,0xa0,0x4a,0x44}; // 6291-01.c1
static const UINT16 s_romData2n_3[] = {0x8a,0x48,0xa5,0x8d,0xa5,0xc9,0x9a,0xa2,0x38,0xa9,0xff}; // 6442-01.d1
static const UINT16 s_romData2n_4[] = {0x38,0xa9,0x85,0x08,0x51,0x85,0x09,0x22,0x69,0x10,0x08}; // 6443-01.e1
static const UINT16 s_romData2n_5[] = {0x8a,0x48,0xa5,0x8d,0xa5,0xc9,0x9a,0xa9,0xd0,0x3d,0x36}; // 6404.d1
static const UINT16 s_romData2n_6[] = {0x10,0x0a,0x22,0x22,0x10,0xe6,0x3d,0x3d,0xa9,0xe6,0x30}; // 6405.e1
static const UINT16 s_romData2n_7[] = {0x10,0x0a,0x22,0x22,0x10,0xe6,0x3d,0xbd,0xa9,0xe6,0x30}; // 6405-02.e1

//
// Sprint1
//
static const ROM_REGION s_romRegionSprint1[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2n_1, 0x41fc985e, "A1 "}, // 6290-01.b1
                                                            {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2n_2, 0x07f7a920, "C1 "}, // 6291-01.c1
                                                            {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2n_3, 0xe9ff0124, "D1 "}, // 6442-01.d1
                                                            {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2n_4, 0xd6bb00d0, "E1 "}, // 6443-01.e1
                                                            {0} }; // end of list

//
// Sprint2
//
static const ROM_REGION s_romRegionSprint2[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2n_1, 0x41fc985e, "A1 "}, // 6290-01.b1
                                                            {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2n_2, 0x07f7a920, "C1 "}, // 6291-01.c1
                                                            {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2n_5, 0xd2878ff6, "D1 "}, // 6404.d1
                                                            {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2n_6, 0x6c991c80, "E1 "}, // 6405.e1
                                                            {0} }; // end of list

//
// Sprint2a
//
static const ROM_REGION s_romRegionSprint2a[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2n_1, 0x41fc985e, "A1 "}, // 6290-01.b1
                                                            {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2n_2, 0x07f7a920, "C1 "}, // 6291-01.c1
                                                            {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2n_5, 0xd2878ff6, "D1 "}, // 6404.d1
                                                            {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2n_7, 0xe80fd249, "E1 "}, // 6405-02.e1
                                                            {0} }; // end of list


//
// Input region for Sprint 1
//
static const INPUT_REGION s_inputRegionS1[] PROGMEM = { //                              "012", "012345"
                                                        {NO_BANK_SWITCH, 0x0828, 0x80,  " F9", "GEAR 1"}, // Gear 1
                                                        {NO_BANK_SWITCH, 0x0829, 0x80,  " F9", "GEAR 2"}, // Gear 2
                                                        {NO_BANK_SWITCH, 0x082A, 0x80,  " F9", "GEAR 3"}, // Gear 3
                                                        {NO_BANK_SWITCH, 0x082B, 0x80,  " F9", "GAS   "}, // Gas
                                                        {NO_BANK_SWITCH, 0x082C, 0x80,  " F9", "SERVIC"}, // Service
                                                        {NO_BANK_SWITCH, 0x082D, 0x80,  " F9", "P1STRT"}, // Start
                                                        {NO_BANK_SWITCH, 0x0830, 0x80,  " M9", "DIP 1 "}, // DIP1
                                                        {NO_BANK_SWITCH, 0x0830, 0x40,  " M9", "DIP 2 "}, // DIP2
                                                        {NO_BANK_SWITCH, 0x0831, 0x80,  " M9", "DIP 3 "}, // DIP3
                                                        {NO_BANK_SWITCH, 0x0831, 0x40,  " M9", "DIP 4 "}, // DIP4
                                                        {NO_BANK_SWITCH, 0x0832, 0x80,  " M9", "DIP 5 "}, // DIP5
                                                        {NO_BANK_SWITCH, 0x0832, 0x40,  " M9", "DIP 6 "}, // DIP6
                                                        {NO_BANK_SWITCH, 0x0833, 0x80,  " M9", "DIP 7 "}, // DIP7
                                                        {NO_BANK_SWITCH, 0x0833, 0x40,  " M9", "DIP 8 "}, // DIP8
                                                        {NO_BANK_SWITCH, 0x0840, 0x40,  " M8", "COIN 1"}, // Coin 1
                                                        {NO_BANK_SWITCH, 0x0840, 0x80,  " M8", "COIN 2"}, // Coin 2
                                                        {NO_BANK_SWITCH, 0x0880, 0xFF,  " M8", "Steer "}, // Steering
                                                        {NO_BANK_SWITCH, 0x0C00, 0xFF,  " E6", "Sync  "}, // Sync
                                                        {NO_BANK_SWITCH, 0x1000, 0xFF,  " D2", "Collis"}, // Collision
                                                        {0}
                                                      }; // end of list

//
// Input region for Sprint 2
//
static const INPUT_REGION s_inputRegionS2[] PROGMEM = { //                              "012", "012345"
                                                        {NO_BANK_SWITCH, 0x0818, 0x80,  " F9", "S2P1G1"}, // Sprint 2 P1 Gear 1
                                                        {NO_BANK_SWITCH, 0x0819, 0x80,  " F9", "S2P2G1"}, // Sprint 2 P2 Gear 1
                                                        {NO_BANK_SWITCH, 0x081A, 0x80,  " F9", "S2P1G2"}, // Sprint 2 P1 Gear 2
                                                        {NO_BANK_SWITCH, 0x081B, 0x80,  " F9", "S2P2G2"}, // Sprint 2 P2 Gear 2
                                                        {NO_BANK_SWITCH, 0x081C, 0x80,  " F9", "S2P1G3"}, // Sprint 2 P1 Gear 3
                                                        {NO_BANK_SWITCH, 0x081D, 0x80,  " F9", "S2P2G3"}, // Sprint 2 P2 Gear 3
                                                        {NO_BANK_SWITCH, 0x0828, 0x80,  " F9", "GAS 1 "}, // Gas 1
                                                        {NO_BANK_SWITCH, 0x0829, 0x80,  " F9", "GAS 2 "}, // Gas 2
                                                        {NO_BANK_SWITCH, 0x082A, 0x80,  " F9", "SERVIC"}, // Service
                                                        {NO_BANK_SWITCH, 0x082B, 0x80,  " F9", "P1STRT"}, // P1 Start
                                                        {NO_BANK_SWITCH, 0x082C, 0x80,  " F9", "P2STRT"}, // P2 Start
                                                        {NO_BANK_SWITCH, 0x082D, 0x80,  " F9", "TRACKS"}, // Track Select
                                                        {NO_BANK_SWITCH, 0x0830, 0x80,  " M9", "DIP 1 "}, // DIP1
                                                        {NO_BANK_SWITCH, 0x0830, 0x40,  " M9", "DIP 2 "}, // DIP2
                                                        {NO_BANK_SWITCH, 0x0831, 0x80,  " M9", "DIP 3 "}, // DIP3
                                                        {NO_BANK_SWITCH, 0x0831, 0x40,  " M9", "DIP 4 "}, // DIP4
                                                        {NO_BANK_SWITCH, 0x0832, 0x80,  " M9", "DIP 5 "}, // DIP5
                                                        {NO_BANK_SWITCH, 0x0832, 0x40,  " M9", "DIP 6 "}, // DIP6
                                                        {NO_BANK_SWITCH, 0x0833, 0x80,  " M9", "DIP 7 "}, // DIP7
                                                        {NO_BANK_SWITCH, 0x0833, 0x40,  " M9", "DIP 8 "}, // DIP8
                                                        {NO_BANK_SWITCH, 0x0840, 0x40,  " M8", "COIN 1"}, // Coin 1
                                                        {NO_BANK_SWITCH, 0x0840, 0x80,  " M8", "COIN 2"}, // Coin 2
                                                        {NO_BANK_SWITCH, 0x0880, 0xFF,  " M8", "Steer1"}, // Steering 1
                                                        {NO_BANK_SWITCH, 0x08C0, 0xFF,  " M8", "Steer2"}, // Steering 2
                                                        {NO_BANK_SWITCH, 0x0C00, 0xFF,  " E6", "Sync  "}, // Sync
                                                        {NO_BANK_SWITCH, 0x1000, 0xFF,  " D2", "Colli1"}, // Collision 1
                                                        {NO_BANK_SWITCH, 0x1400, 0xFF,  " D2", "Colli2"}, // Collision 2
                                                        {0}
                                                      }; // end of list


IGame*
CSprintGame::createInstanceSprint1(
)
{
    return (new CSprintGame(s_romRegionSprint1, s_inputRegionS1));
}

IGame*
CSprintGame::createInstanceSprint2(
)
{
    return (new CSprintGame(s_romRegionSprint2, s_inputRegionS2));
}

IGame*
CSprintGame::createInstanceSprint2a(
)
{
    return (new CSprintGame(s_romRegionSprint2a, s_inputRegionS2));
}

CSprintGame::CSprintGame(
    const ROM_REGION *romRegion,
    const INPUT_REGION *s_inputRegion
) : CSprintBaseGame( romRegion, s_inputRegion )
{
}


