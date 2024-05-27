//
// Copyright (c) 2015, Paul R. Swan
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
#include "CAstroWarsGame.h"


//                                         01   02   04   08   10   20   40   80  100  200
static const UINT16 s_romData2n__8H[] = {0x01,0x20,0x05,0x00,0x00,0x00,0x59,0x4F,0xC2,0x25};
static const UINT16 s_romData2n_10H[] = {0xCD,0x14,0xE6,0x2E,0x1C,0x45,0x44,0x63,0x00,0x08};
static const UINT16 s_romData2n_11H[] = {0x7D,0x7C,0xEB,0xEB,0xEB,0x04,0x06,0x38,0xFF,0xC2};
static const UINT16 s_romData2n_13H[] = {0x02,0x98,0x06,0xE4,0x1F,0xC1,0xD2,0x14,0x14,0x48};
static const UINT16 s_romData2n__8I[] = {0x63,0x9A,0x04,0x42,0x7A,0x1F,0x01,0x07,0x1B,0x77};
static const UINT16 s_romData2n_10I[] = {0x00,0x00,0xFC,0x00,0x54,0x5E,0x05,0x60,0x8C,0x04};
static const UINT16 s_romData2n_11I[] = {0x14,0xF4,0x98,0xB1,0x06,0x1A,0x80,0x14,0xE4,0x9E};
static const UINT16 s_romData2n_13I[] = {0xFB,0xFB,0xFB,0xF9,0xF2,0xF6,0xF9,0x8A,0x0F,0xFB};
static const UINT16 s_romData2n_11L[] = {0xE5,0x0B,0x02,0x28,0xEE,0xDE,0xE4,0xE4,0x02,0xD0};
static const UINT16 s_romData2n_13L[] = {0x14,0x9F,0x03,0x08,0x6D,0x04,0xFF,0xF4,0x75,0xA9};

//
// Not yet found a board that matches this set from MAME.
//
static const ROM_REGION s_romRegion[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2n__8H, 0xb0ec246c, " 8H"},
                                                  {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2n_10H, 0x090d360f, "10H"},
                                                  {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2n_11H, 0x72ab1378, "11H"},
                                                  {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2n_13H, 0x2dc4c895, "13H"},
                                                  {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2n__8I, 0xab87fbfc, " 8I"},
                                                  {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2n_10I, 0x533675c1, "10I"},
                                                  {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2n_11I, 0x59cf8901, "11I"},
                                                  {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2n_13I, 0x5149c121, "13I"},
                                                  {NO_BANK_SWITCH, 0x2c00, 0x0400, s_romData2n_11L, 0x29f52f57, "11L"},
                                                  {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2n_13L, 0x882cdb87, "13L"},
                                                  {0} }; // end of list

//
// Found on one board, a two byte difference in ROM 13I at offset 0x0200->0x201
// "03" designation.
//
static const UINT16 s_romData2nSet2_13I[] = {0xFB,0xFB,0xFB,0xF9,0xF2,0xF6,0xF9,0x8A,0x0F,0xE4};

static const ROM_REGION s_romRegionSet2[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2n__8H,     0xb0ec246c, " 8H"},
                                                      {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2n_10H,     0x090d360f, "10H"},
                                                      {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2n_11H,     0x72ab1378, "11H"},
                                                      {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2n_13H,     0x2dc4c895, "13H"},
                                                      {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2n__8I,     0xab87fbfc, " 8I"},
                                                      {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2n_10I,     0x533675c1, "10I"},
                                                      {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2n_11I,     0x59cf8901, "11I"},
                                                      {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2nSet2_13I, 0xF0D75AAA, "13I"}, // Set 2
                                                      {NO_BANK_SWITCH, 0x2c00, 0x0400, s_romData2n_11L,     0x29f52f57, "11L"},
                                                      {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2n_13L,     0x882cdb87, "13L"},
                                                      {0} }; // end of list

//
// Found on at least two boards, a single bit difference in ROM 8I at offset 0x3B1 is 27 (versus 67 in Set 2).
// "03" designation
//
static const ROM_REGION s_romRegionSet3[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2n__8H,     0xb0ec246c, " 8H"},
                                                      {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2n_10H,     0x090d360f, "10H"},
                                                      {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2n_11H,     0x72ab1378, "11H"},
                                                      {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2n_13H,     0x2dc4c895, "13H"},
                                                      {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2n__8I,     0xE2337FE5, " 8I"}, // Set 3
                                                      {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2n_10I,     0x533675c1, "10I"},
                                                      {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2n_11I,     0x59cf8901, "11I"},
                                                      {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2nSet2_13I, 0xF0D75AAA, "13I"}, // Set 2
                                                      {NO_BANK_SWITCH, 0x2c00, 0x0400, s_romData2n_11L,     0x29f52f57, "11L"},
                                                      {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2n_13L,     0x882cdb87, "13L"},
                                                      {0} }; // end of list

//
// Found on one board sent in for repair, a pristine board with all ROMS factory hand labelled "02"
// The set is mostly completely different from all the prior known sets.
//
//                                             01   02   04   08   10   20   40   80  100  200
static const UINT16 s_romData2nSet02__8H[] = {0x00,0xf1,0x05,0x00,0x00,0x00,0x0a,0xd1,0xbb,0x1c};//
static const UINT16 s_romData2nSet02_10H[] = {0x54,0xe6,0x1c,0x01,0xf1,0xe5,0xcf,0x03,0xcf,0x0f};//
static const UINT16 s_romData2nSet02_11H[] = {0xe4,0xe3,0xf2,0xe5,0xeb,0xeb,0x03,0x12,0xcf,0x5a};//
static const UINT16 s_romData2nSet02_13H[] = {0x14,0x54,0xd9,0x17,0x14,0x01,0x54,0xdf,0x0d,0x14};//
static const UINT16 s_romData2nSet02__8I[] = {0x0f,0x74,0x14,0x44,0xe3,0x01,0x1c,0x20,0x99,0xd0};//
static const UINT16 s_romData2nSet02_10I[] = {0x00,0x00,0xFC,0x00,0x54,0x5E,0x05,0x60,0x8C,0x04};//
static const UINT16 s_romData2nSet02_11I[] = {0x1b,0x1a,0x08,0x9a,0x10,0xb3,0x04,0xb2,0x1a,0x23};//
static const UINT16 s_romData2nSet02_13I[] = {0xf2,0xf2,0xf2,0x6c,0xf6,0x69,0x00,0x3b,0xf8,0x08};//
static const UINT16 s_romData2nSet02_11L[] = {0x4d,0xcf,0xae,0x09,0x53,0xbb,0x94,0x1a,0x24,0xab};//
static const UINT16 s_romData2nSet02_13L[] = {0x14,0x5f,0x04,0x94,0x99,0xff,0x00,0x03,0x66,0xcc};//

static const ROM_REGION s_romRegionSet02[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2nSet02__8H, 0x73D45B59, " 8H"},
                                                       {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2nSet02_10H, 0x01C471CF, "10H"},
                                                       {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2nSet02_11H, 0x2CE443B1, "11H"},
                                                       {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2nSet02_13H, 0xE085C293, "13H"},
                                                       {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2nSet02__8I, 0x80078A6C, " 8I"},
                                                       {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2nSet02_10I, 0x9535D0A9, "10I"},
                                                       {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2nSet02_11I, 0xF3BEA168, "11I"},
                                                       {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2nSet02_13I, 0xC2A07996, "13I"},
                                                       {NO_BANK_SWITCH, 0x2c00, 0x0400, s_romData2nSet02_11L, 0xB59423CB, "11L"},
                                                       {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2nSet02_13L, 0xBE0356C2, "13L"},
                                                       {0} }; // end of list

static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                                          "012", "012345"
                                                  {NO_BANK_SWITCH,                         0x1400,      0x14FF,      1, 0x0F, "13F", "Prog. "}, // "Program RAM, 2112, 13F"
                                                  {NO_BANK_SWITCH,                         0x1400,      0x14FF,      1, 0xF0, "13G", "Prog. "}, // "Program RAM, 2112, 13G"
                                                  {NO_BANK_SWITCH,                         0x1C00,      0x1CFF,      1, 0x0F, " 3F", "Shell "}, // "Bullet (SHELL) RAM, 2101, 3F"
                                                  {NO_BANK_SWITCH,                         0x1C00,      0x1CFF,      1, 0xF0, " 2F", "Shell "}, // "Bullet (SHELL) RAM, 2101, 2F"
                                                  {CAstroWarsBaseGame::onBankSwitchFlagHi, 0x1800,      0x1BFF,      1, 0x0F, " 2C", "Char. "}, // "Video Character RAM, 2114, FLAG=Hi"
                                                  {CAstroWarsBaseGame::onBankSwitchFlagHi, 0x1800,      0x1BFF,      1, 0xF0, " 1C", "Char. "}, // "Video Character RAM, 2114, FLAG=Hi"
                                                  {CAstroWarsBaseGame::onBankSwitchFlagLo, 0x1800,      0x1BFF,      1, 0x0F, " 3C", "Colour"}, // "Colour RAM, 2114, FLAG=Lo"
                                                  {NO_BANK_SWITCH,                         0x1500+0x00, 0x1500+0x2D, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                  {NO_BANK_SWITCH,                         0x1500+0x40, 0x1500+0x6D, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                  {NO_BANK_SWITCH,                         0x1500+0x80, 0x1500+0xAD, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                  {0}
                                                }; // end of list

static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                                          "012", "012345"
                                                          {NO_BANK_SWITCH,                         0x1400,      0x14FF,      1, 0xFF, "13?", "Prog. "}, // "Program RAM, 2112, 13F/13G"
                                                          {NO_BANK_SWITCH,                         0x1C00,      0x1CFF,      1, 0xFF, "23F", "Shell "}, // "Bullet (SHELL) RAM, 2101, 2F/3F"
                                                          {CAstroWarsBaseGame::onBankSwitchFlagHi, 0x1800,      0x1BFF,      1, 0xFF, "12C", "Char. "}, // "Video Character RAM, 2114, FLAG=Hi, 2C/1C"
                                                          {NO_BANK_SWITCH,                         0x1500+0x00, 0x1500+0x2D, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                          {NO_BANK_SWITCH,                         0x1500+0x40, 0x1500+0x6D, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                          {NO_BANK_SWITCH,                         0x1500+0x80, 0x1500+0xAD, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                          {0}
                                                        }; // end of list

//
// This region allows the PVI to be initialized.
// If the PVI is not initialized, the screen ends up all white such that no
// other graphics can be seen.
//

static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                         "012", "012345"
                                                           {NO_BANK_SWITCH,  0x1500, 0x15FF, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                           {0}
                                                         }; // end of list

//
// Input region is the same for all versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                               "012", "012345"
                                                      {NO_BANK_SWITCH, 0x12000L, 0x3F, "5M ", "IN0   "}, // EXT - Row0 - Coin & Start
                                                      {NO_BANK_SWITCH, 0x12002L, 0x3F, "5M ", "IN1   "}, // EXT - Row2 - Joystick & button
                                                      {NO_BANK_SWITCH, 0x12006L, 0x3F, "3N ", "SW A  "}, // Ext - Dipswitch A
                                                      {NO_BANK_SWITCH, 0x12007L, 0x3F, "2N ", "SW B  "}, // EXT - Dipswitch B
                                                      {NO_BANK_SWITCH, 0x10000L, 0x03, "8M ", "Coll.D"}, // CTRL - Collision detect
                                                      {NO_BANK_SWITCH, 0x14000L, 0xFF, " - ", "Coll.C"}, // DATA - Collision clear (no data)
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x14000L, 0x01, 0x00, "3L ", "Snd  1"}, // DATA
                                                        {NO_BANK_SWITCH, 0x14000L, 0x02, 0x00, "3L ", "Snd  2"}, // DATA
                                                        {NO_BANK_SWITCH, 0x14000L, 0x04, 0x00, "3L ", "Snd  3"}, // DATA
                                                        {NO_BANK_SWITCH, 0x14000L, 0x08, 0x00, "3L ", "Snd  4"}, // DATA
                                                        {NO_BANK_SWITCH, 0x14000L, 0x10, 0x00, "3L ", "Snd  5"}, // DATA
                                                        {NO_BANK_SWITCH, 0x14000L, 0x20, 0x00, "3L ", "Snd  6"}, // DATA
                                                        {NO_BANK_SWITCH, 0x10000L, 0x08, 0x00, "2L ", "Snd  7"}, // CTRL
                                                        {NO_BANK_SWITCH, 0x10000L, 0x10, 0x00, "2L ", "Snd  8"}, // CTRL
                                                        {NO_BANK_SWITCH, 0x10000L, 0x20, 0x00, "2L ", "Stars "}, // CTRL
                                                        {NO_BANK_SWITCH, 0x10000L, 0x01, 0x00, "2L ", "Ctr. 1"}, // CTRL
                                                        {NO_BANK_SWITCH, 0x10000L, 0x02, 0x00, "2L ", "Ctr. 2"}, // CTRL
                                                        {NO_BANK_SWITCH, 0x10000L, 0x04, 0x00, "2L ", "Ctr. 3"}, // CTRL
                                                        {0}
                                                      }; // end of list

IGame*
CAstroWarsGame::createInstance(
)
{
    return (new CAstroWarsGame(s_romRegion));
}

IGame*
CAstroWarsGame::createInstanceSet2(
)
{
    return (new CAstroWarsGame(s_romRegionSet2));
}

IGame*
CAstroWarsGame::createInstanceSet3(
)
{
    return (new CAstroWarsGame(s_romRegionSet3));
}

IGame*
CAstroWarsGame::createInstanceSet02(
)
{
    return (new CAstroWarsGame(s_romRegionSet02));
}

CAstroWarsGame::CAstroWarsGame(
    const ROM_REGION *romRegion
) : CAstroWarsBaseGame( romRegion,
                        s_ramRegion,
                        s_ramRegionByteOnly,
                        s_ramRegionWriteOnly,
                        s_inputRegion,
                        s_outputRegion )
{
}


