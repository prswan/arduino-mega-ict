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
#include "CAsteroidsDlxGame.h"

//
// Asteroids Deluxe Set 1
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0x00,0x02,0x3c,0xbd,0x05,0x62,0x10,0x86,0x13,0x05,0xc9}, // 036800-01.r2
                                                       {0x76,0x84,0x9e,0x06,0xce,0x0b,0xa8,0xa3,0x80,0x00,0x40}, // 036799-01.np2
                                                       {0xf5,0x7b,0xd4,0xd3,0xd4,0xa5,0x1f,0x03,0x60,0x69,0xce}, // 036430-01.d1
                                                       {0xc9,0x18,0x08,0x17,0x85,0x1e,0x8d,0xb0,0xa5,0x07,0xd0}, // 036431-01.ef1
                                                       {0xa0,0x00,0x23,0xb9,0xf5,0xa5,0x55,0x11,0x71,0x30,0x9a}, // 036432-01.fh1
                                                       {0x85,0x04,0x02,0xc9,0x8a,0x20,0x00,0xd0,0x9a,0x69,0x03}, // 036433-01.j1
                                                     {0} };  // end of list

static const ROM_REGION s_romRegionAstDlxSet1[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x4800, 0x0800, s_romData2nSet1[0].data2n, 0x3b597407, "R2 "}, // r2
                                                            {NO_BANK_SWITCH, 0x5000, 0x0800, s_romData2nSet1[1].data2n, 0x7d511572, "NP2"}, // np2
                                                            {NO_BANK_SWITCH, 0x6000, 0x0800, s_romData2nSet1[2].data2n, 0x8f5dabc6, "D1 "}, // d1
                                                            {NO_BANK_SWITCH, 0x6800, 0x0800, s_romData2nSet1[3].data2n, 0x157a8516, "EF1"}, // ef1
                                                            {NO_BANK_SWITCH, 0x7000, 0x0800, s_romData2nSet1[4].data2n, 0xfdea913c, "FH1"}, // fh1
                                                            {NO_BANK_SWITCH, 0x7800, 0x0800, s_romData2nSet1[5].data2n, 0xef09bac7, "J1 "}, // j1
                                                            {0} }; // end of list

//
// Asteroids Deluxe Set 2
//
static const ROM_DATA2N s_romData2nSet2[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0x00,0x02,0x3c,0xbd,0x05,0x62,0x10,0x86,0x13,0x05,0xc9}, // 036800-01.r2
                                                       {0x76,0x84,0x9e,0x06,0xce,0x0b,0xa8,0xa3,0x80,0x00,0x40}, // 036799-01.np2
                                                       {0xf5,0x7b,0xd4,0xd3,0xd4,0xa5,0x1f,0x03,0x60,0x69,0xce}, // 036430-01.d1
                                                       {0xc9,0x18,0x08,0x17,0x85,0x1e,0x8d,0xb0,0xa5,0x07,0xd0}, // 036431-01.ef1
                                                       {0xa0,0x00,0x23,0xb9,0xf5,0xa5,0x55,0x11,0x71,0x30,0x9a}, // 036432-01.fh1
                                                       {0x85,0x04,0x02,0xc9,0x8a,0x20,0x00,0xd0,0x9a,0x69,0x03}, // 036433-02.j1
                                                     {0} };  // end of list

static const ROM_REGION s_romRegionAstDlxSet2[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x4800, 0x0800, s_romData2nSet2[0].data2n, 0x3b597407, "R2 "}, // r2
                                                            {NO_BANK_SWITCH, 0x5000, 0x0800, s_romData2nSet2[1].data2n, 0x7d511572, "NP2"}, // np2
                                                            {NO_BANK_SWITCH, 0x6000, 0x0800, s_romData2nSet2[2].data2n, 0x8f5dabc6, "D1 "}, // d1
                                                            {NO_BANK_SWITCH, 0x6800, 0x0800, s_romData2nSet2[3].data2n, 0x157a8516, "EF1"}, // ef1
                                                            {NO_BANK_SWITCH, 0x7000, 0x0800, s_romData2nSet2[4].data2n, 0xfdea913c, "FH1"}, // fh1
                                                            {NO_BANK_SWITCH, 0x7800, 0x0800, s_romData2nSet2[5].data2n, 0xd8db74e3, "J1 "}, // j1
                                                            {0} }; // end of list

//
// Asteroids Deluxe Set 3
//
static const ROM_DATA2N s_romData2nSet3[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0x00,0x02,0x3c,0xbd,0x05,0x61,0x10,0x86,0x13,0xfb,0x56}, // 036800-02.r2
                                                       {0x76,0x84,0x9e,0x06,0xce,0x0b,0xa8,0xa3,0x80,0x00,0x40}, // 036799-01.np2
                                                       {0xe3,0x7b,0xcd,0xcc,0xcd,0xa5,0x1f,0xff,0x10,0xa5,0x5b}, // 036430-02.d1
                                                       {0x02,0x8d,0x02,0x8d,0xa5,0x20,0xfd,0x5b,0x29,0x29,0x9d}, // 036431-02.ef1
                                                       {0xca,0x10,0xa5,0xc5,0x90,0x00,0x11,0xa9,0x40,0x7e,0xb6}, // 036432-02.fh1
                                                       {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x25,0x00,0x56,0xfa}, // 036433-03.j1
                                                     {0} };  // end of list

static const ROM_REGION s_romRegionAstDlxSet3[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x4800, 0x0800, s_romData2nSet3[0].data2n, 0xbb8cabe1, "R2 "}, // r2
                                                            {NO_BANK_SWITCH, 0x5000, 0x0800, s_romData2nSet3[1].data2n, 0x7d511572, "NP2"}, // np2
                                                            {NO_BANK_SWITCH, 0x6000, 0x0800, s_romData2nSet3[2].data2n, 0xa4d7a525, "D1 "}, // d1
                                                            {NO_BANK_SWITCH, 0x6800, 0x0800, s_romData2nSet3[3].data2n, 0xd4004aae, "EF1"}, // ef1
                                                            {NO_BANK_SWITCH, 0x7000, 0x0800, s_romData2nSet3[4].data2n, 0x6d720c41, "FH1"}, // fh1
                                                            {NO_BANK_SWITCH, 0x7800, 0x0800, s_romData2nSet3[5].data2n, 0x0dcc0be6, "J1 "}, // j1
                                                            {0} }; // end of list


//
// Asteroids Deluxe Output region
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x3000, 0xFF, 0x00,  " --", "DMA GO"}, // DMA GO
                                                        {NO_BANK_SWITCH, 0x3400, 0xFF, 0x00,  " --", "WD CLR"}, // watchdog clear
                                                        {NO_BANK_SWITCH, 0x3600, 0xFF, 0x00,  " --", "EXPLOS"}, // Explosion Sound
                                                        {NO_BANK_SWITCH, 0x3C00, 0xFF, 0x00,  " --", "SAUCER"}, // Saucer Sound
                                                        {NO_BANK_SWITCH, 0x3C01, 0xFF, 0x00,  " --", "SAFIRE"}, // Saucer Fire Sound
                                                        {NO_BANK_SWITCH, 0x3C02, 0xFF, 0x00,  " --", "SASEL "}, // Saucer Sound Select
                                                        {NO_BANK_SWITCH, 0x3C03, 0xFF, 0x00,  " --", "THRUST"}, // Ship Thrust Sound
                                                        {NO_BANK_SWITCH, 0x3C04, 0xFF, 0x00,  " --", "FIRE  "}, // Ship Fire Sound
                                                        {NO_BANK_SWITCH, 0x3C05, 0xFF, 0x00,  " --", "LIFE  "}, // Life Sound
                                                        {NO_BANK_SWITCH, 0x3E00, 0xFF, 0x00,  " --", "SNDRST"}, // Noise Reset
                                                        {0}
                                                      }; // end of list


IGame*
CAsteroidsDlxGame::createInstanceAstDlxSet1(
)
{
    return (new CAsteroidsDlxGame(s_romData2nSet1, s_romRegionAstDlxSet1,s_outputRegion));
}

IGame*
CAsteroidsDlxGame::createInstanceAstDlxSet2(
)
{
    return (new CAsteroidsDlxGame(s_romData2nSet2, s_romRegionAstDlxSet2,s_outputRegion));
}

IGame*
CAsteroidsDlxGame::createInstanceAstDlxSet3(
)
{
    return (new CAsteroidsDlxGame(s_romData2nSet3, s_romRegionAstDlxSet3,s_outputRegion));
}

CAsteroidsDlxGame::CAsteroidsDlxGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion,
    const OUTPUT_REGION *s_outputRegion
) : CAsteroidsBaseGame(romData2n, romRegion, s_outputRegion)
{
}


