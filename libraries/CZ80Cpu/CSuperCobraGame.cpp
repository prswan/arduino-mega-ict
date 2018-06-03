//
// Copyright (c) 2018, Paul R. Swan
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
#include "CSuperCobraGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                                                      "012", "012345"
                                                  {NO_BANK_SWITCH, 0x008000, 0x0083FF, 1, 0x0F, " 1K", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x008000, 0x0083FF, 1, 0xF0, " 1G", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x008400, 0x0087FF, 1, 0x0F, " 1J", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x008400, 0x0087FF, 1, 0xF0, " 1H", "Prog. "}, // "Program RAM, 2114"
                                                  //
                                                  // See note above about access restrictions w.r.t HBLANK & WAIT.
                                                  // These regions are access with special support in the CZ80Cpu triggered via address 0x10xxxx.
                                                  //
                                                  //                                                                                      "012", "012345"
                                                  {NO_BANK_SWITCH, 0x108800, 0x108BFF, 1, 0x0F, " 3K", "BkVRam"}, // "Background VRAM, 2114"
                                                  {NO_BANK_SWITCH, 0x108800, 0x108BFF, 1, 0xF0, " 3J", "BkVRam"}, // "Background VRAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                                                      "012", "012345"
                                                          {NO_BANK_SWITCH, 0x008000, 0x0083FF, 1, 0xFF, "1KG", "Prog. "}, // "Program RAM, 2114, 1K/1G"
                                                          {NO_BANK_SWITCH, 0x008400, 0x0087FF, 1, 0xFF, "1JH", "Prog. "}, // "Program RAM, 2114, 1J/1H"
                                                          {NO_BANK_SWITCH, 0x108800, 0x108BFF, 1, 0xFF, "3KJ", "BkVRam"}, // "Background VRAM, 2114, 3K/3J"
                                                          {0}
                                                        }; // end of list

//
// RAM region is the same for all games on this board set.
// Unlike Galaxian, it appears that the object RAM cannot be read.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                                                                    "012", "012345"
                                                           {NO_BANK_SWITCH, 0x9000, 0x90FF, 1, 0x0F, " 3L", "ObjRam"}, // "Object RAM, 2114, 256 Bytes used."
                                                           {NO_BANK_SWITCH, 0x9000, 0x90FF, 1, 0xF0, " 3M", "ObjRam"}, // "Object RAM, 2114, 256 Bytes used."
                                                           {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                                                                    "012", "012345"
                                                      {NO_BANK_SWITCH,                             0xB000, 0xFF,  " 5C", "WD Res"}, // Watchdog reset
                                                      {CScrambleBaseGame::onBankSwitchSetup8255_0, 0x9800, 0xFF,  "s1E", "Port A"}, // CP Inputs
                                                      {CScrambleBaseGame::onBankSwitchSetup8255_0, 0x9801, 0xFF,  "s1E", "Port B"}, // CP Inputs
                                                      {CScrambleBaseGame::onBankSwitchSetup8255_0, 0x9802, 0xFF,  "s1E", "Port C"}, // CP Inputs
                                                      {CScrambleBaseGame::onBankSwitchSetup8255_1, 0xA002, 0xF0,  "s1E", "Prot R"}, // Protection Read
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                                                          "012", "012345"
                                                        {NO_BANK_SWITCH,                             0xA801, 0x01, 0x00,  " 5B", "NMI En"}, // NMI enable
                                                        {NO_BANK_SWITCH,                             0xA802, 0x01, 0x00,  " 5B", "Cntr  "}, // Coin counter
                                                        {NO_BANK_SWITCH,                             0xA803, 0x01, 0x00,  " 5B", "POUT1 "}, // Blue video drive
                                                        {NO_BANK_SWITCH,                             0xA804, 0x01, 0x00,  " 5B", "Stars "}, // Stars on
                                                        {NO_BANK_SWITCH,                             0xA805, 0x01, 0x00,  " 5B", "POUT2 "}, // unused
                                                        {NO_BANK_SWITCH,                             0xA806, 0x01, 0x00,  " 5B", "Flip X"}, // Flip X
                                                        {NO_BANK_SWITCH,                             0xA807, 0x01, 0x00,  " 5B", "Flip Y"}, // Flip Y
                                                        {CScrambleBaseGame::onBankSwitchSetup8255_1, 0xA000, 0xFF, 0x00,  "s1D", "Snd Cm"}, // Sound command
                                                        {CScrambleBaseGame::onBankSwitchSetup8255_1, 0xA001, 0x04, 0x00,  "s1D", "Snd In"}, // Sound interrupt
                                                        {CScrambleBaseGame::onBankSwitchSetup8255_1, 0xA001, 0x10, 0x00,  "s1D", "Amp Mt"}, // Amp Mute
                                                        {CScrambleBaseGame::onBankSwitchSetup8255_1, 0xA002, 0x0F, 0x00,  "s1D", "Prot W"}, // Protection Write
                                                        {0}
                                                      }; // end of list

//
// Konami Set 1 from MAME.
//
static const ROM_DATA2N s_romData2nKonamiSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                             {0x00,0x32,0xa8,0x77,0x85,0x77,0x7e,0x90,0x05,0x02,0x21,0x80}, // 0 - 2C
                                                             {0x21,0x00,0xdd,0xe6,0x0f,0x0f,0x11,0x20,0x8b,0xb7,0x40,0x0f}, // 1 - 2E
                                                             {0xeb,0x20,0x21,0x1b,0x3a,0xc6,0x21,0xc9,0xfe,0x74,0xdd,0x02}, // 2 - 2F
                                                             {0x01,0x01,0x36,0x0a,0x37,0xdd,0x06,0x0f,0x21,0x0c,0xc8,0xd0}, // 3 - 2H
                                                             {0x00,0xc0,0xc0,0x4c,0x61,0x36,0x00,0xd2,0x61,0x00,0x78,0xb8}, // 4 - 2J
                                                             {0x36,0xa9,0x00,0xa9,0x00,0xc9,0x00,0xc4,0x57,0xc8,0x00,0x0e}, // 5 - 2L
                                                             {0} };  // end of list

//
// Konami Set 1 from MAME.
//
static const ROM_REGION s_romRegionKonamiSet1[] PROGMEM = { //
                                                            {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nKonamiSet1[0].data2n, 0xa0744b3f, " 2C"}, // 2c
                                                            {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nKonamiSet1[1].data2n, 0x8e7245cd, " 2E"}, // 2e
                                                            {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nKonamiSet1[2].data2n, 0x47a4e6fb, " 2F"}, // 2f
                                                            {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nKonamiSet1[3].data2n, 0x7244f21c, " 2H"}, // 2h
                                                            {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nKonamiSet1[4].data2n, 0xe1f8a801, " 2J"}, // 2j
                                                            {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nKonamiSet1[5].data2n, 0xd52affde, " 2L"}, // 2l
                                                            {0} }; // end of list

//
// Stern Set 1 from MAME.
//
static const ROM_DATA2N s_romData2nSternSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                            {0x00,0x32,0xa8,0x77,0x85,0x77,0x7e,0x32,0xaf,0x19,0xb2,0x2a}, // 0 - 2C
                                                            {0x3e,0x10,0x1f,0x19,0x40,0x14,0x35,0x84,0x80,0xed,0x83,0x86}, // 1 - 2E
                                                            {0x20,0x18,0x21,0x13,0x06,0x74,0xc9,0x05,0x10,0x15,0x24,0xdd}, // 2 - 2F
                                                            {0x06,0x2c,0xdd,0xdd,0xdd,0x00,0x0f,0x3f,0x06,0x4b,0x83,0x8d}, // 3 - 2H
                                                            {0x00,0xac,0xb0,0x74,0x63,0x36,0x00,0x2c,0x55,0x03,0xa0,0x33}, // 4 - 2J
                                                            {0x00,0x00,0x2e,0x00,0x33,0x00,0x2d,0x3f,0x4f,0xb0,0xa0,0x0e}, // 5 - 2L
                                                            {0} };  // end of list

//
// Stern Set 1 from MAME.
//
static const ROM_REGION s_romRegionSternSet1[] PROGMEM = { //
                                                           {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSternSet1[0].data2n, 0xe15ade38, " 2C"}, // scobra2c
                                                           {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSternSet1[1].data2n, 0xa270e44d, " 2E"}, // scobra2e
                                                           {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSternSet1[2].data2n, 0xbdd70346, " 2F"}, // scobra2f
                                                           {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nSternSet1[3].data2n, 0xdca5ec31, " 2H"}, // scobra2h
                                                           {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nSternSet1[4].data2n, 0x0d8f6b6e, " 2J"}, // scobra2j
                                                           {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nSternSet1[5].data2n, 0x6f80f3a9, " 2L"}, // scobra2l
                                                           {0} }; // end of list


IGame*
CSuperCobraGame::createInstanceKonamiSet1(
)
{
    return (new CSuperCobraGame(s_romData2nKonamiSet1, s_romRegionKonamiSet1));
}


IGame*
CSuperCobraGame::createInstanceSternSet1(
)
{
    return (new CSuperCobraGame(s_romData2nSternSet1, s_romRegionSternSet1));
}


CSuperCobraGame::CSuperCobraGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CScrambleBaseGame( romData2n,
                       romRegion,
                       s_ramRegion,
                       s_ramRegionByteOnly,
                       s_ramRegionWriteOnly,
                       s_inputRegion,
                       s_outputRegion,
                       0xA801, // intMaskWriteAddress,
                       0x9800, // i8255WriteBaseAddress0
                       0xA000) // i8255WriteBaseAddress0
{
}

