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
#include "CGalaxiaGame.h"


//                                         01   02   04   08   10   20   40   80  100  200
static const UINT16 s_romData2n__8H[] = {0x00,0xEF,0x20,0x00,0x00,0x00,0x37,0x7B,0x8E,0x72};
static const UINT16 s_romData2n_10H[] = {0x18,0xEC,0x0C,0x4C,0xCC,0xCC,0x3F,0x7C,0x7A,0x9F};
static const UINT16 s_romData2n_11H[] = {0x0E,0x1C,0x86,0x85,0x02,0x62,0x14,0x03,0x03,0x0C};
static const UINT16 s_romData2n_13H[] = {0x14,0xCC,0xBA,0xFB,0x0D,0xBB,0xF8,0x0D,0xC8,0x0C};
static const UINT16 s_romData2n__8I[] = {0x15,0x0C,0x15,0x0A,0xCC,0xCE,0x17,0x8C,0x1C,0x0F};
static const UINT16 s_romData2n_10I[] = {0x10,0xCC,0x05,0x06,0x9E,0x30,0xC8,0xC0,0xE0,0x18};
static const UINT16 s_romData2n_11I[] = {0x63,0xC8,0x66,0xF4,0x66,0xB0,0x0F,0x3F,0x05,0xCC};
static const UINT16 s_romData2n_13I[] = {0x04,0x01,0x1C,0x00,0xE5,0x20,0xCD,0x9C,0xAF,0x3B};
static const UINT16 s_romData2n_11L[] = {0x1D,0x38,0x04,0x07,0x0E,0xCF,0x02,0x69,0x1E,0x00};
static const UINT16 s_romData2n_13L[] = {0x13,0x03,0xFF,0x1F,0x17,0x1D,0x05,0x01,0x05,0xF1};

//
// Not yet found a board that matches this set from MAME.
//
static const ROM_REGION s_romRegion[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2n__8H, 0xf3b4ffde, " 8H"},
                                                  {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2n_10H, 0x6d07fdd4, "10H"},
                                                  {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2n_11H, 0x1520eb3d, "11H"},
                                                  {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2n_13H, 0xc4482770, "13H"},
                                                  {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2n__8I, 0x45b88599, " 8I"},
                                                  {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2n_10I, 0xc0baa654, "10I"},
                                                  {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2n_11I, 0x4456808a, "11I"},
                                                  {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2n_13I, 0xcf653b9a, "13I"},
                                                  {NO_BANK_SWITCH, 0x2c00, 0x0400, s_romData2n_11L, 0x50c6a645, "11L"},
                                                  {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2n_13L, 0x3a9c38c7, "13L"},
                                                  {0} }; // end of list

//
// Found on at least two boards, a few bytes different in ROM 10I at offset 0x02b5->0x2bf
// "08I" designation.
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2n__8H, 0xf3b4ffde, " 8H"},
                                                      {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2n_10H, 0x6d07fdd4, "10H"},
                                                      {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2n_11H, 0x1520eb3d, "11H"},
                                                      {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2n_13H, 0xc4482770, "13H"},
                                                      {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2n__8I, 0x45b88599, " 8I"},
                                                      {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2n_10I, 0x76bd9fe3, "10I"}, // Set 2
                                                      {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2n_11I, 0x4456808a, "11I"},
                                                      {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2n_13I, 0xcf653b9a, "13I"},
                                                      {NO_BANK_SWITCH, 0x2c00, 0x0400, s_romData2n_11L, 0x50c6a645, "11L"},
                                                      {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2n_13L, 0x3a9c38c7, "13L"},
                                                      {0} }; // end of list

//
// Found on at least two boards, a single bit difference in ROM 13H at offset 0x17f is C5 (versus D5 in Set 2).
// "08I" designation
//
static const ROM_REGION s_romRegionSet3[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2n__8H, 0xf3b4ffde, " 8H"},
                                                      {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2n_10H, 0x6d07fdd4, "10H"},
                                                      {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2n_11H, 0x1520eb3d, "11H"},
                                                      {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2n_13H, 0x1d22219b, "13H"}, // Set 3
                                                      {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2n__8I, 0x45b88599, " 8I"},
                                                      {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2n_10I, 0x76bd9fe3, "10I"}, // Set 2
                                                      {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2n_11I, 0x4456808a, "11I"},
                                                      {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2n_13I, 0xcf653b9a, "13I"},
                                                      {NO_BANK_SWITCH, 0x2c00, 0x0400, s_romData2n_11L, 0x50c6a645, "11L"},
                                                      {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2n_13L, 0x3a9c38c7, "13L"},
                                                      {0} }; // end of list

//
// Found on one board, many differences in ROMs 11H, 13H, 10I, 13I & 11L.
// "08" designation
//
static const UINT16 s_romData2nSet4_13H[] = {0x0B,0xB1,0x08,0xBB,0x84,0xCC,0x02,0x0C,0x1B,0x94};
static const UINT16 s_romData2nSet4_13I[] = {0x04,0x01,0x1C,0x00,0xE5,0x20,0xCD,0x9C,0x05,0x3B};


static const ROM_REGION s_romRegionSet4[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2n__8H,     0xf3b4ffde, " 8H"},
                                                      {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2n_10H,     0x6d07fdd4, "10H"},
                                                      {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2n_11H,     0x5682d56f, "11H"}, // Set 4
                                                      {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2nSet4_13H, 0x80dafe84, "13H"}, // Set 4
                                                      {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2n__8I,     0x45b88599, " 8I"},
                                                      {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2n_10I,     0x76bd9fe3, "10I"}, // Set 2
                                                      {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2n_11I,     0x4456808a, "11I"},
                                                      {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2nSet4_13I, 0xffe86fdb, "13I"}, // Set 4
                                                      {NO_BANK_SWITCH, 0x2c00, 0x0400, s_romData2n_11L,     0x8e3f5343, "11L"}, // Set 4
                                                      {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2n_13L,     0x3a9c38c7, "13L"},
                                                      {0} }; // end of list

//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                                          "012", "012345"
                                                  {NO_BANK_SWITCH,                         0x1C00,      0x1FFF,      1, 0x0F, "13F", "Prog. "}, // "Program RAM, 2114, 13F"
                                                  {NO_BANK_SWITCH,                         0x1C00,      0x1FFF,      1, 0xF0, "13G", "Prog. "}, // "Program RAM, 2114, 13G"
                                                  {NO_BANK_SWITCH,                         0x1400,      0x14FF,      1, 0x0F, " 3F", "Shell "}, // "Bullet (SHELL) RAM, 2101, 3F"
                                                  {NO_BANK_SWITCH,                         0x1400,      0x14FF,      1, 0xF0, " 2F", "Shell "}, // "Bullet (SHELL) RAM, 2101, 2F"
                                                  {CAstroWarsBaseGame::onBankSwitchFlagHi, 0x1800,      0x1BFF,      1, 0x0F, " 2C", "Char. "},  // "Video Character RAM, 2114, FLAG=Hi"
                                                  {CAstroWarsBaseGame::onBankSwitchFlagHi, 0x1800,      0x1BFF,      1, 0xF0, " 1C", "Char. "},  // "Video Character RAM, 2114, FLAG=Hi"
                                                  {CAstroWarsBaseGame::onBankSwitchFlagLo, 0x1800,      0x1BFF,      1, 0x02, " 1B", "Colour"},  // "Colour RAM, 2102, FLAG=Lo"
                                                  {CAstroWarsBaseGame::onBankSwitchFlagLo, 0x1800,      0x1BFF,      1, 0x01, " 3C", "Colour"},  // "Colour RAM, 2102, FLAG=Lo"
                                                  {NO_BANK_SWITCH,                         0x1500+0x00, 0x1500+0x2D, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                  {NO_BANK_SWITCH,                         0x1500+0x40, 0x1500+0x6D, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                  {NO_BANK_SWITCH,                         0x1500+0x80, 0x1500+0xAD, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                  {NO_BANK_SWITCH,                         0x1600+0x00, 0x1600+0x2D, 1, 0xFF, "10F", "2636-2"}, // "2636 PVI 2"
                                                  {NO_BANK_SWITCH,                         0x1600+0x40, 0x1600+0x6D, 1, 0xFF, "10F", "2636-2"}, // "2636 PVI 2"
                                                  {NO_BANK_SWITCH,                         0x1600+0x80, 0x1600+0xAD, 1, 0xFF, "10F", "2636-2"}, // "2636 PVI 2"
                                                  {NO_BANK_SWITCH,                         0x1700+0x00, 0x1700+0x2D, 1, 0xFF, "11F", "2636-3"}, // "2636 PVI 3"
                                                  {NO_BANK_SWITCH,                         0x1700+0x40, 0x1700+0x6D, 1, 0xFF, "11F", "2636-3"}, // "2636 PVI 3"
                                                  {NO_BANK_SWITCH,                         0x1700+0x80, 0x1700+0xAD, 1, 0xFF, "11F", "2636-3"}, // "2636 PVI 3"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all versions.
// This description is used for the byte-wide intensive random access memory test.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                                          "012", "012345"
                                                          {NO_BANK_SWITCH,                         0x1C00,      0x1FFF,      1, 0xFF, "13?", "Prog. "}, // "Program RAM, 2114, 13F/13G"
                                                          {NO_BANK_SWITCH,                         0x1400,      0x14FF,      1, 0xFF, "32F", "Shell "}, // "Bullet (SHELL) RAM, 2101, 3F/2F"
                                                          {CAstroWarsBaseGame::onBankSwitchFlagHi, 0x1800,      0x1BFF,      1, 0xFF, "21C", "Char. "},  // "Video Character RAM, 2114, FLAG=Hi, 2C/1C"
                                                          {NO_BANK_SWITCH,                         0x1500+0x00, 0x1500+0x2D, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                          {NO_BANK_SWITCH,                         0x1500+0x40, 0x1500+0x6D, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                          {NO_BANK_SWITCH,                         0x1500+0x80, 0x1500+0xAD, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                          {NO_BANK_SWITCH,                         0x1600+0x00, 0x1600+0x2D, 1, 0xFF, "10F", "2636-2"}, // "2636 PVI 2"
                                                          {NO_BANK_SWITCH,                         0x1600+0x40, 0x1600+0x6D, 1, 0xFF, "10F", "2636-2"}, // "2636 PVI 2"
                                                          {NO_BANK_SWITCH,                         0x1600+0x80, 0x1600+0xAD, 1, 0xFF, "10F", "2636-2"}, // "2636 PVI 2"
                                                          {NO_BANK_SWITCH,                         0x1700+0x00, 0x1700+0x2D, 1, 0xFF, "11F", "2636-3"}, // "2636 PVI 3"
                                                          {NO_BANK_SWITCH,                         0x1700+0x40, 0x1700+0x6D, 1, 0xFF, "11F", "2636-3"}, // "2636 PVI 3"
                                                          {NO_BANK_SWITCH,                         0x1700+0x80, 0x1700+0xAD, 1, 0xFF, "11F", "2636-3"}, // "2636 PVI 3"
                                                          {0}
                                                        }; // end of list

//
// This region allows the PVI's to be initialized.
// If the PVI's are not initialized, the screen ends up all white such that no
// other graphics can be seen.
//

static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                         "012", "012345"
                                                           {NO_BANK_SWITCH,  0x1500, 0x15FF, 1, 0xFF, " 8F", "2636-1"}, // "2636 PVI 1"
                                                           {NO_BANK_SWITCH,  0x1600, 0x16FF, 1, 0xFF, "10F", "2636-2"}, // "2636 PVI 2"
                                                           {NO_BANK_SWITCH,  0x1700, 0x17FF, 1, 0xFF, "11F", "2636-3"}, // "2636 PVI 3"
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
                                                      {NO_BANK_SWITCH, 0x10000L, 0xFF, "8M ", "Coll.D"}, // CTRL - Collision detect
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
                                                        {NO_BANK_SWITCH, 0x10000L, 0x08, 0x00, "2L ", "Snd T7"}, // CTRL
                                                        {NO_BANK_SWITCH, 0x10000L, 0x10, 0x00, "2L ", "Snd  8"}, // CTRL
                                                        {NO_BANK_SWITCH, 0x10000L, 0x20, 0x00, "2L ", "Stars "}, // CTRL
                                                        {NO_BANK_SWITCH, 0x10000L, 0x01, 0x00, "2L ", "Ctr. 1"}, // CTRL
                                                        {NO_BANK_SWITCH, 0x10000L, 0x02, 0x00, "2L ", "Ctr. 2"}, // CTRL
                                                        {NO_BANK_SWITCH, 0x10000L, 0x04, 0x00, "2L ", "Ctr. 3"}, // CTRL
                                                        {NO_BANK_SWITCH, 0x12000L, 0x05, 0x00, "3B ", "Scrl05"}, // EXT - Scroll register
                                                        {NO_BANK_SWITCH, 0x12000L, 0x0A, 0x00, "3B ", "Scrl0A"}, // EXT - Scroll register
                                                        {NO_BANK_SWITCH, 0x12000L, 0x50, 0x00, "4B ", "Scrl50"}, // EXT - Scroll register
                                                        {NO_BANK_SWITCH, 0x12000L, 0xA0, 0x00, "4B ", "ScrlA0"}, // EXT - Scroll register
                                                        {0}
                                                      }; // end of list

IGame*
CGalaxiaGame::createInstance(
)
{
    return (new CGalaxiaGame(s_romRegion));
}

IGame*
CGalaxiaGame::createInstanceSet2(
)
{
    return (new CGalaxiaGame(s_romRegionSet2));
}

IGame*
CGalaxiaGame::createInstanceSet3(
)
{
    return (new CGalaxiaGame(s_romRegionSet3));
}

IGame*
CGalaxiaGame::createInstanceSet4(
)
{
    return (new CGalaxiaGame(s_romRegionSet4));
}

CGalaxiaGame::CGalaxiaGame(
    const ROM_REGION *romRegion
) : CAstroWarsBaseGame( romRegion,
                        s_ramRegion,
                        s_ramRegionByteOnly,
                        s_ramRegionWriteOnly,
                        s_inputRegion,
                        s_outputRegion )
{
}


