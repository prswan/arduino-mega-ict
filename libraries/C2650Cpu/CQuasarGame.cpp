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
#include "CQuasarGame.h"


//
// ROM Memory Map
//   The ROM memory map is a bit strange in that the 2Kb ROMS are split addressed:
//      Lo 4Kb - 0x0000
//      Hi 4Kb - 0x4000
//   This causes some complications with MAME because MAME calculates CRC's accross the image and
//   the tester calculates the CRC based on contigiously addressed block. Therefore, we need to split
//   the images and recalculate the individual CRC's of the two halves. HxD can be used to do this.
//

//                                            01   02   04   08   10   20   40   80  100  200
static const UINT16 s_romData2n__7B_Lo[] = {0x08,0x94,0x44,0x62,0x9a,0x85,0x03,0x98,0xc8,0x1f};//
static const UINT16 s_romData2n__6B_Lo[] = {0x31,0x44,0x0d,0x01,0xf5,0x1c,0x0d,0xc8,0x03,0x03};//
static const UINT16 s_romData2n__5B_Lo[] = {0x1c,0x02,0x10,0x98,0xad,0xd4,0x1b,0x87,0x1d,0x57};//
static const UINT16 s_romData2n__3B_Lo[] = {0x0f,0x7c,0x1c,0x01,0xd4,0x01,0x17,0x7c,0x11,0xa4};//
static const UINT16 s_romData2n__2B_Lo[] = {0x18,0x14,0x01,0x98,0x4b,0x10,0x0b,0x7c,0x18,0x11};//
static const UINT16 s_romData2n__7C[]    = {0x10,0xcc,0x23,0x24,0x9e,0x0c,0x1f,0x84,0x0c,0xf4};//
static const UINT16 s_romData2n__6C[]    = {0x29,0x08,0xcf,0xe7,0x1c,0xff,0xfe,0x0d,0x44,0x10};//
static const UINT16 s_romData2n__5C[]    = {0x00,0x00,0x00,0x80,0x1e,0x00,0x00,0xf8,0x00,0xdc};//
static const UINT16 s_romData2n__3C[]    = {0xf0,0x03,0x0b,0x00,0x08,0x08,0x10,0x8f,0xf9,0x96};//
static const UINT16 s_romData2n__2C[]    = {0xc2,0x0d,0xe5,0x86,0x9c,0x0e,0x7a,0x40,0x26,0xf4};//
static const UINT16 s_romData2n__7B_Hi[] = {0x40,0x17,0x16,0xd4,0x1f,0xf4,0x80,0x8d,0x05,0xcd};//
static const UINT16 s_romData2n__6B_Hi[] = {0x8e,0x1e,0xca,0x1f,0x05,0x85,0xf4,0x5e,0x1f,0x02};//
static const UINT16 s_romData2n__5B_Hi[] = {0x21,0x00,0x00,0xff,0x18,0x1b,0x48,0xff,0x49,0x16};//
static const UINT16 s_romData2n__3B_Hi[] = {0xcd,0xce,0x3e,0xd6,0xcc,0xc7,0xc8,0x32,0xc8,0x01};//
static const UINT16 s_romData2n__2B_Hi[] = {0xe7,0x04,0xc8,0xcc,0xc8,0x1a,0xc7,0xc0,0x08,0x13};//

//
// Set 1 from MAME
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2n__7B_Lo, 0x40462B57, " 7B"},//
                                                      {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2n__6B_Lo, 0x0A91EF0D, " 6B"},//
                                                      {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2n__5B_Lo, 0x853F1358, " 5B"},//
                                                      {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2n__3B_Lo, 0x56558572, " 3B"},//
                                                      {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2n__2B_Lo, 0x5E59ACBB, " 2B"},//
                                                      {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2n__7C,    0xF7F1267D, " 7C"},//
                                                      {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2n__6C,    0x772004EB, " 6C"},//
                                                      {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2n__5C,    0x7A87B6F3, " 5C"},//
                                                      {NO_BANK_SWITCH, 0x2C00, 0x0400, s_romData2n__3C,    0xEF87C2CB, " 3C"},//
                                                      {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2n__2C,    0xBE6C4F84, " 2C"},//
                                                      {NO_BANK_SWITCH, 0x4000, 0x0400, s_romData2n__7B_Hi, 0xA27523CC, " 7B"},//
                                                      {NO_BANK_SWITCH, 0x4400, 0x0400, s_romData2n__6B_Hi, 0x798B3434, " 6B"},//
                                                      {NO_BANK_SWITCH, 0x4800, 0x0400, s_romData2n__5B_Hi, 0x133F54BB, " 5B"},//
                                                      {NO_BANK_SWITCH, 0x4C00, 0x0400, s_romData2n__3B_Hi, 0x0F1C6BFA, " 3B"},//
                                                      {NO_BANK_SWITCH, 0x5000, 0x0400, s_romData2n__2B_Hi, 0x2AFB576A, " 2B"},//
                                                      {0} }; // end of list

//
// Set 2 from MAME
//
static const ROM_REGION s_romRegionSet2[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2n__7B_Lo, 0x40462B57, " 7B"},//
                                                      {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2n__6B_Lo, 0x0A91EF0D, " 6B"},//
                                                      {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2n__5B_Lo, 0x853F1358, " 5B"},//
                                                      {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2n__3B_Lo, 0x56558572, " 3B"},//
                                                      {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2n__2B_Lo, 0x5E59ACBB, " 2B"},//
                                                      {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2n__7C,    0xF7F1267D, " 7C"},//
                                                      {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2n__6C,    0x772004EB, " 6C"},//
                                                      {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2n__5C,    0x7A87B6F3, " 5C"},//
                                                      {NO_BANK_SWITCH, 0x2C00, 0x0400, s_romData2n__3C,    0xEF87C2CB, " 3C"},//
                                                      {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2n__2C,    0xA31C0435, " 2C"},// Diff.
                                                      {NO_BANK_SWITCH, 0x4000, 0x0400, s_romData2n__7B_Hi, 0xA27523CC, " 7B"},//
                                                      {NO_BANK_SWITCH, 0x4400, 0x0400, s_romData2n__6B_Hi, 0x798B3434, " 6B"},//
                                                      {NO_BANK_SWITCH, 0x4800, 0x0400, s_romData2n__5B_Hi, 0x133F54BB, " 5B"},//
                                                      {NO_BANK_SWITCH, 0x4C00, 0x0400, s_romData2n__3B_Hi, 0x0F1C6BFA, " 3B"},//
                                                      {NO_BANK_SWITCH, 0x5000, 0x0400, s_romData2n__2B_Hi, 0x2AFB576A, " 2B"},//
                                                      {0} }; // end of list

//
// Set 3, found on a Quasar PCB (from the Quasar upright with Moon Crest marque).
// The set is the same as Set 1 from MAME except that it has differences around offset 0x5B0.
// I suspect this Set 3 is older than Set 1 because it appears that Set 1 has a patch applied
// that makes some changes and NO-OP patches out something that is in this Set 3.
//
static const ROM_REGION s_romRegionSet3[] PROGMEM = { {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2n__7B_Lo, 0x40462B57, " 7B"},//
                                                      {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2n__6B_Lo, 0x0A91EF0D, " 6B"},//
                                                      {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2n__5B_Lo, 0x853F1358, " 5B"},//
                                                      {NO_BANK_SWITCH, 0x0c00, 0x0400, s_romData2n__3B_Lo, 0x56558572, " 3B"},//
                                                      {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2n__2B_Lo, 0x5E59ACBB, " 2B"},//
                                                      {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2n__7C,    0xF7F1267D, " 7C"},//
                                                      {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2n__6C,    0x772004EB, " 6C"},//
                                                      {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2n__5C,    0x7A87B6F3, " 5C"},//
                                                      {NO_BANK_SWITCH, 0x2C00, 0x0400, s_romData2n__3C,    0xEF87C2CB, " 3C"},//
                                                      {NO_BANK_SWITCH, 0x3000, 0x0400, s_romData2n__2C,    0xBE6C4F84, " 2C"},//
                                                      {NO_BANK_SWITCH, 0x4000, 0x0400, s_romData2n__7B_Hi, 0x9DED7808, " 7B"},// Diff.
                                                      {NO_BANK_SWITCH, 0x4400, 0x0400, s_romData2n__6B_Hi, 0x798B3434, " 6B"},//
                                                      {NO_BANK_SWITCH, 0x4800, 0x0400, s_romData2n__5B_Hi, 0x133F54BB, " 5B"},//
                                                      {NO_BANK_SWITCH, 0x4C00, 0x0400, s_romData2n__3B_Hi, 0x0F1C6BFA, " 3B"},//
                                                      {NO_BANK_SWITCH, 0x5000, 0x0400, s_romData2n__2B_Hi, 0x2AFB576A, " 2B"},//
                                                      {0} }; // end of list


static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                                             "012", "012345"
                                                  {NO_BANK_SWITCH,                            0x1C00,      0x1FFF,      1, 0x0F, " 7E", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH,                            0x1C00,      0x1FFF,      1, 0xF0, " 6E", "Prog. "}, // "Program RAM, 2114"
                                                  {CQuasarBaseGame::onBankSwitchCharacterRam, 0x1800,      0x1BFF,      1, 0x0F, "12F", "Char. "}, // "Video Character RAM, 2114"
                                                  {CQuasarBaseGame::onBankSwitchCharacterRam, 0x1800,      0x1BFF,      1, 0xF0, "11F", "Char. "}, // "Video Character RAM, 2114"
                                                  {CQuasarBaseGame::onBankSwitchColourRam,    0x1800,      0x1BFF,      1, 0x01, "11H", "Colour"}, // "Colour RAM, 2102"
                                                  {CQuasarBaseGame::onBankSwitchColourRam,    0x1800,      0x1BFF,      1, 0x02, "12H", "Colour"}, // "Colour RAM, 2102"
                                                  {CQuasarBaseGame::onBankSwitchColourRam,    0x1800,      0x1BFF,      1, 0x04, "13H", "Colour"}, // "Colour RAM, 2102"
                                                  {CQuasarBaseGame::onBankSwitchEffectRam,    0x1800,      0x1BFF,      1, 0x0F, "  9", "Effect"}, // "Video Effect RAM, 2114" - Note the flipped Dbus
                                                  {CQuasarBaseGame::onBankSwitchEffectRam,    0x1800,      0x1BFF,      1, 0xF0, "  8", "Effect"}, // "Video Effect RAM, 2114" - Note the flipped DBus
                                                  {NO_BANK_SWITCH,                            0x1500+0x00, 0x1500+0x2D, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                  {NO_BANK_SWITCH,                            0x1500+0x40, 0x1500+0x6D, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                  {NO_BANK_SWITCH,                            0x1500+0x80, 0x1500+0xAD, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                  {NO_BANK_SWITCH,                            0x1600+0x00, 0x1600+0x2D, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                  {NO_BANK_SWITCH,                            0x1600+0x40, 0x1600+0x6D, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                  {NO_BANK_SWITCH,                            0x1600+0x80, 0x1600+0xAD, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                  {NO_BANK_SWITCH,                            0x1700+0x00, 0x1700+0x2D, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                  {NO_BANK_SWITCH,                            0x1700+0x40, 0x1700+0x6D, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                  {NO_BANK_SWITCH,                            0x1700+0x80, 0x1700+0xAD, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                  {0}
                                                }; // end of list

static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                                             "012", "012345"
                                                          {NO_BANK_SWITCH,                            0x1C00,      0x1FFF,      1, 0xFF, "76E", "Prog. "}, // "Program RAM, 2114, 7E/6E"
                                                          {CQuasarBaseGame::onBankSwitchCharacterRam, 0x1800,      0x1BFF,      1, 0xFF, "1?F", "Char. "}, // "Video Character RAM, 2114, 12F/11F"
                                                          {CQuasarBaseGame::onBankSwitchEffectRam,    0x1800,      0x1BFF,      1, 0xFF, " 98", "Effect"}, // "Video Effect RAM, 2114, 9/8" - Note the flipped Dbus
                                                          {NO_BANK_SWITCH,                            0x1500+0x00, 0x1500+0x2D, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                          {NO_BANK_SWITCH,                            0x1500+0x40, 0x1500+0x6D, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                          {NO_BANK_SWITCH,                            0x1500+0x80, 0x1500+0xAD, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                          {NO_BANK_SWITCH,                            0x1600+0x00, 0x1600+0x2D, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                          {NO_BANK_SWITCH,                            0x1600+0x40, 0x1600+0x6D, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                          {NO_BANK_SWITCH,                            0x1600+0x80, 0x1600+0xAD, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                          {NO_BANK_SWITCH,                            0x1700+0x00, 0x1700+0x2D, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                          {NO_BANK_SWITCH,                            0x1700+0x40, 0x1700+0x6D, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                          {NO_BANK_SWITCH,                            0x1700+0x80, 0x1700+0xAD, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                          {0}
                                                        }; // end of list

//
// This region allows the bullet RAM & PVI's to be initialized.
// If the PVI's are no initialized, the screen ends up all white such that no
// other graphics can be seen.
//

static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                         "012", "012345"
                                                           {NO_BANK_SWITCH,  0x1400, 0x14FF, 1, 0x0F, "15G", "Shell "}, // "Bullet (SHELL) RAM, 2112"
                                                           {NO_BANK_SWITCH,  0x1400, 0x14FF, 1, 0xF0, "14G", "Shell "}, // "Bullet (SHELL) RAM, 2112"
                                                           {NO_BANK_SWITCH,  0x1500, 0x15FF, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                           {NO_BANK_SWITCH,  0x1600, 0x16FF, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                           {NO_BANK_SWITCH,  0x1700, 0x17FF, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                           {0}
                                                         }; // end of list

//
// Input region is the same for all versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                                 "012", "012345"
                                                      {CQuasarBaseGame::onBankSwitchIN0, 0x12000L, 0x3F, "5I ", "IN0   "}, // EXT - Row0
                                                      {CQuasarBaseGame::onBankSwitchIN0, 0x12000L, 0xC0, "4I ", "IN0   "}, // EXT - Row0
                                                      {CQuasarBaseGame::onBankSwitchIN1, 0x12000L, 0x3F, "5I ", "IN1   "}, // EXT - Row1
                                                      {CQuasarBaseGame::onBankSwitchIN1, 0x12000L, 0xC0, "4I ", "IN1   "}, // EXT - Row1
                                                      {CQuasarBaseGame::onBankSwitchSW1, 0x12000L, 0x3F, "5I ", "SW 1  "}, // EXT - Dipswitch 1, 1-6
                                                      {CQuasarBaseGame::onBankSwitchSW1, 0x12000L, 0xC0, "4I ", "SW 1  "}, // EXT - Dipswitch 1, 7-8
                                                      {CQuasarBaseGame::onBankSwitchSW2, 0x12000L, 0x3F, "5I ", "SW 2  "}, // EXT - Dipswitch 2, 1-6
                                                      {CQuasarBaseGame::onBankSwitchSW2, 0x12000L, 0xC0, "4I ", "SW 2  "}, // EXT - Dipswitch 2, 7-8
                                                      {NO_BANK_SWITCH,                   0x10000L, 0x0F, "4I ", "Coll.D"}, // CTRL - Collision detect
                                                      {NO_BANK_SWITCH,                   0x14000L, 0xFF, "2M ", "Coll.C"}, // DATA - Collision clear & counters
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                               "012", "012345"
                                                        {CQuasarBaseGame::onBankSwitchEffectBlank, 0x01800L, 0x30, 0x00, " 12", "E.Blnk"}, // "Video Effect Blanking"
                                                        {NO_BANK_SWITCH,                           0x14000L, 0x01, 0x00, "8L ", "Snd  1"}, // DATA
                                                        {NO_BANK_SWITCH,                           0x14000L, 0x02, 0x00, "8L ", "Snd  2"}, // DATA
                                                        {NO_BANK_SWITCH,                           0x14000L, 0x04, 0x00, "8L ", "Snd  3"}, // DATA
                                                        {NO_BANK_SWITCH,                           0x14000L, 0x08, 0x00, "8L ", "Snd  4"}, // DATA
                                                        {NO_BANK_SWITCH,                           0x14000L, 0x10, 0x00, "8L ", "Snd  5"}, // DATA
                                                        {NO_BANK_SWITCH,                           0x14000L, 0x20, 0x00, "8L ", "Snd  6"}, // DATA
                                                        {NO_BANK_SWITCH,                           0x14000L, 0x40, 0x00, "7M ", "Snd  7"}, // DATA
                                                        {NO_BANK_SWITCH,                           0x14000L, 0x80, 0x00, "7M ", "Snd  8"}, // DATA
                                                        {NO_BANK_SWITCH,                           0x10000L, 0x01, 0x00, "7L ", "Ctr. 1"}, // CTRL
                                                        {NO_BANK_SWITCH,                           0x10000L, 0x02, 0x00, "7L ", "Ctr. 2"}, // CTRL
                                                        {NO_BANK_SWITCH,                           0x10000L, 0x04, 0x00, "7L ", "Ctr. 3"}, // CTRL
                                                        {NO_BANK_SWITCH,                           0x10000L, 0x08, 0x00, "7L ", "A. Inv"}, // CTRL
                                                        {NO_BANK_SWITCH,                           0x10000L, 0x10, 0x00, "7L ", "Snd 10"}, // CTRL
                                                        {NO_BANK_SWITCH,                           0x10000L, 0x20, 0x00, "7L ", "Snd  9"}, // CTRL
                                                        {NO_BANK_SWITCH,                           0x10000L, 0x40, 0x00, "7M ", "Snd 11"}, // CTRL
                                                        {NO_BANK_SWITCH,                           0x10000L, 0x80, 0x00, "7M ", "Snd 12"}, // CTRL
                                                        {0}
                                                      }; // end of list

IGame*
CQuasarGame::createInstanceSet1(
)
{
    return (new CQuasarGame(s_romRegionSet1));
}

IGame*
CQuasarGame::createInstanceSet2(
)
{
    return (new CQuasarGame(s_romRegionSet2));
}

IGame*
CQuasarGame::createInstanceSet3(
)
{
    return (new CQuasarGame(s_romRegionSet3));
}

CQuasarGame::CQuasarGame(
    const ROM_REGION *romRegion
) : CQuasarBaseGame( romRegion,
                     s_ramRegion,
                     s_ramRegionByteOnly,
                     s_ramRegionWriteOnly,
                     s_inputRegion,
                     s_outputRegion )
{
}


