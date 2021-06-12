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
#include "CAsteroidsGame.h"

//
// Asteroids Set 1
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0xa0,0x00,0x00,0x00,0x00,0xfe,0x00,0x3b,0x81,0x0a,0xc0}, // 035127-01.np3
                                                       {0xee,0x6e,0xcc,0x71,0x90,0x00,0x76,0xf0,0x02,0x8a,0x1f}, // 035145-01.ef2
                                                       {0x8c,0x02,0x18,0x04,0x8c,0x02,0x1b,0x58,0x02,0x29,0x31}, // 035144-01.h2
                                                       {0xa2,0x00,0x08,0x4a,0x0a,0x46,0x91,0x1a,0x64,0xec,0x0f}, // 035143-01.j2
                                                     {0} };  // end of list

static const ROM_REGION s_romRegionAsteroidsSet1[] PROGMEM = { //
                                                               {NO_BANK_SWITCH, 0x5000, 0x0800, s_romData2nSet1[0].data2n, 0x99699366, "NP3"}, // 127
                                                               {NO_BANK_SWITCH, 0x6800, 0x0800, s_romData2nSet1[1].data2n, 0xe9bfda64, "EF2"}, // 145
                                                               {NO_BANK_SWITCH, 0x7000, 0x0800, s_romData2nSet1[2].data2n, 0xe53c28a9, "H2 "}, // 144
                                                               {NO_BANK_SWITCH, 0x7800, 0x0800, s_romData2nSet1[3].data2n, 0x7d4e3d05, "J2 "}, // 143
                                                               {0} }; // end of list

//
// Asteroids Set 2
//
static const ROM_DATA2N s_romData2nSet2[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0xa0,0x00,0x00,0x00,0x00,0xfe,0x00,0x3b,0x0d,0x7a,0x80}, // 035127-02.np3
                                                       {0xf3,0x7c,0xfa,0x6e,0xfe,0x01,0xc2,0x02,0x8d,0x04,0x60}, // 035145-02.ef2
                                                       {0x85,0x06,0x7d,0x18,0x02,0xa0,0x1c,0x02,0x3e,0xa7,0x0f}, // 035144-02.h2
                                                       {0xbd,0x88,0x85,0x78,0x02,0x70,0x85,0x9d,0x67,0x9d,0xc0}, // 035143-02.j2
                                                     {0} };  // end of list

static const ROM_REGION s_romRegionAsteroidsSet2[] PROGMEM = { //
                                                               {NO_BANK_SWITCH, 0x5000, 0x0800, s_romData2nSet2[0].data2n, 0x8b71fd9e, "NP3"}, // 127
                                                               {NO_BANK_SWITCH, 0x6800, 0x0800, s_romData2nSet2[1].data2n, 0x0cc75459, "EF2"}, // 145
                                                               {NO_BANK_SWITCH, 0x7000, 0x0800, s_romData2nSet2[2].data2n, 0x096ed35c, "H2 "}, // 144
                                                               {NO_BANK_SWITCH, 0x7800, 0x0800, s_romData2nSet2[3].data2n, 0x312caa02, "J2 "}, // 143
                                                               {0} }; // end of list

//
// Asteroids Set 4
//
static const ROM_DATA2N s_romData2nSet4[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400
                                                       {0xa0,0x00,0x00,0x00,0x00,0xfe,0x00,0x3b,0x0d,0x7a,0x80}, // 035127-02.np3
                                                       {0xf3,0x7c,0xfa,0x6e,0xfe,0x01,0xc2,0x02,0x8d,0x04,0x60}, // 035145-04e.ef2
                                                       {0x85,0x06,0x7d,0x18,0x02,0xa0,0x1c,0x02,0x3e,0xa7,0x0f}, // 035144-04e.h2
                                                       {0xbd,0x88,0x85,0x78,0x02,0x70,0x85,0x9d,0x67,0x9d,0xc0}, // 035143-02.j2
                                                     {0} };  // end of list

static const ROM_REGION s_romRegionAsteroidsSet4[] PROGMEM = { //
                                                               {NO_BANK_SWITCH, 0x5000, 0x0800, s_romData2nSet4[0].data2n, 0x8b71fd9e, "NP3"}, // 127
                                                               {NO_BANK_SWITCH, 0x6800, 0x0800, s_romData2nSet4[1].data2n, 0xb503eaf7, "EF2"}, // 145
                                                               {NO_BANK_SWITCH, 0x7000, 0x0800, s_romData2nSet4[2].data2n, 0x25233192, "H2 "}, // 144
                                                               {NO_BANK_SWITCH, 0x7800, 0x0800, s_romData2nSet4[3].data2n, 0x312caa02, "J2 "}, // 143
                                                               {0} }; // end of list


//
// Asteroids Output region
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x3000, 0xFF, 0x00,  " --", "DMA GO"}, // DMA GO
                                                        {NO_BANK_SWITCH, 0x3200, 0x01, 0x00,  " --", "P2 LED"}, // player 2 LED
                                                        {NO_BANK_SWITCH, 0x3200, 0x02, 0x00,  " --", "P1 LED"}, // player 1 LED
                                                        {NO_BANK_SWITCH, 0x3200, 0x04, 0x00,  " --", "RAMSEL"}, // RAMSEL
                                                        {NO_BANK_SWITCH, 0x3200, 0x08, 0x00,  " --", "CNCTRL"}, // coin counter L
                                                        {NO_BANK_SWITCH, 0x3200, 0x10, 0x00,  " --", "CNCTRC"}, // coin counter C
                                                        {NO_BANK_SWITCH, 0x3200, 0x20, 0x00,  " --", "CNCTRR"}, // coin counter R
                                                        {NO_BANK_SWITCH, 0x3400, 0xFF, 0x00,  " --", "WD CLR"}, // watchdog clear
                                                        {NO_BANK_SWITCH, 0x3600, 0xFF, 0x00,  " --", "EXPLOS"}, // Explosion Sound
                                                        {NO_BANK_SWITCH, 0x3A00, 0xFF, 0x00,  " --", "THUMP "}, // Thump Sound
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
CAsteroidsGame::createInstanceAsteroidsSet1(
)
{
    return (new CAsteroidsGame(s_romData2nSet1, s_romRegionAsteroidsSet1, s_outputRegion));
}

IGame*
CAsteroidsGame::createInstanceAsteroidsSet2(
)
{
    return (new CAsteroidsGame(s_romData2nSet2, s_romRegionAsteroidsSet2, s_outputRegion));
}

IGame*
CAsteroidsGame::createInstanceAsteroidsSet4(
)
{
    return (new CAsteroidsGame(s_romData2nSet4, s_romRegionAsteroidsSet4, s_outputRegion));
}

CAsteroidsGame::CAsteroidsGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion,
    const OUTPUT_REGION *s_outputRegion
) : CAsteroidsBaseGame(romData2n, romRegion, s_outputRegion)
{
}


