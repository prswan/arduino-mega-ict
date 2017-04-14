//
// Copyright (c) 2016, Paul R. Swan
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
#include "CGenericBaseGame.h"


//
// Not used in the generic platform.
//                                           01   02   04   08   10   20   40   80  100  200  400   800
static const UINT16 s_romData2n2716[]   = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static const UINT16 s_romData2n2732[]   = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00};

//
// 2716 (2Kb, 0x0800) regions.
//
static const ROM_REGION s_romRegion2716[] PROGMEM = { //                                                            "012'
                                                      {NO_BANK_SWITCH, 0x0000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x0800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x1000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x1800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x2000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x2800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x3000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x3800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x4000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x4800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x5000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x5800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x6000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x6800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x7000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x7800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x8000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x8800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x9000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0x9800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0xA000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0xA800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0xB000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0xB800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0xC000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0xC800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0xD000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0xD800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0xE000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0xE800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0xF000, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {NO_BANK_SWITCH, 0xF800, 0x0800, s_romData2n2716, 0x00000000, " 2K"},
                                                      {0} }; // end of list

//
// 2732 (4Kb, 0x1000) regions.
//
static const ROM_REGION s_romRegion2732[] PROGMEM = { //                                                            "012'
                                                      {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0x6000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0x7000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0x8000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0x9000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0xA000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0xB000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0xC000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0xD000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0xE000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {NO_BANK_SWITCH, 0xF000, 0x1000, s_romData2n2732, 0x00000000, " 4K"},
                                                      {0} }; // end of list

//
// 1Kb RAM regions in normal & video sync access formats.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x000000, 0x0003FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x100000, 0x1003FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x000400, 0x0007FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x100400, 0x1007FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x000800, 0x000BFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x100800, 0x100BFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x000C00, 0x000FFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x100C00, 0x100FFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x001000, 0x0013FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x101000, 0x1013FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x001400, 0x0017FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x101400, 0x1017FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x001800, 0x001BFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x101800, 0x101BFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x001C00, 0x001FFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x101C00, 0x101FFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x002000, 0x0023FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x102000, 0x1023FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x002400, 0x0027FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x102400, 0x1027FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x002800, 0x002BFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x102800, 0x102BFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x002C00, 0x002FFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x102C00, 0x102FFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x003000, 0x0033FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x103000, 0x1033FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x003400, 0x0037FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x103400, 0x1037FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x003800, 0x003BFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x103800, 0x103BFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x003C00, 0x003FFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x103C00, 0x103FFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x004000, 0x0043FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x104000, 0x1043FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x004400, 0x0047FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x104400, 0x1047FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x004800, 0x004BFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x104800, 0x104BFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x004C00, 0x004FFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x104C00, 0x104FFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x005000, 0x0053FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x105000, 0x1053FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x005400, 0x0057FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x105400, 0x1057FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x005800, 0x005BFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x105800, 0x105BFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x005C00, 0x005FFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x105C00, 0x105FFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x006000, 0x0063FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x106000, 0x1063FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x006400, 0x0067FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x106400, 0x1067FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x006800, 0x006BFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x106800, 0x106BFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x006C00, 0x006FFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x106C00, 0x106FFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x007000, 0x0073FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x107000, 0x1073FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x007400, 0x0077FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x107400, 0x1077FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x007800, 0x007BFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x107800, 0x107BFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x007C00, 0x007FFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x107C00, 0x107FFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x008000, 0x0083FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x108000, 0x1083FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x008400, 0x0087FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x108400, 0x1087FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x008800, 0x008BFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x108800, 0x108BFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x008C00, 0x008FFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x108C00, 0x108FFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x009000, 0x0093FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x109000, 0x1093FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x009400, 0x0097FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x109400, 0x1097FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x009800, 0x009BFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x109800, 0x109BFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x009C00, 0x009FFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x109C00, 0x109FFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00A000, 0x00A3FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10A000, 0x10A3FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00A400, 0x00A7FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10A400, 0x10A7FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00A800, 0x00ABFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10A800, 0x10ABFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00AC00, 0x00AFFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10AC00, 0x10AFFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00B000, 0x00B3FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10B000, 0x10B3FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00B400, 0x00B7FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10B400, 0x10B7FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00B800, 0x00BBFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10B800, 0x10BBFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00BC00, 0x00BFFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10BC00, 0x10BFFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00C000, 0x00C3FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10C000, 0x10C3FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00C400, 0x00C7FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10C400, 0x10C7FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00C800, 0x00CBFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10C800, 0x10CBFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00CC00, 0x00CFFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10CC00, 0x10CFFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00D000, 0x00D3FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10D000, 0x10D3FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00D400, 0x00D7FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10D400, 0x10D7FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00D800, 0x00DBFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10D800, 0x10DBFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00DC00, 0x00DFFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10DC00, 0x10DFFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00E000, 0x00E3FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10E000, 0x10E3FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00E400, 0x00E7FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10E400, 0x10E7FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00E800, 0x00EBFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10E800, 0x10EBFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00EC00, 0x00EFFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10EC00, 0x10EFFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00F000, 0x00F3FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10F000, 0x10F3FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00F400, 0x00F7FF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10F400, 0x10F7FF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00F800, 0x00FBFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10F800, 0x10FBFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {NO_BANK_SWITCH, 0x00FC00, 0x00FFFF, 1, 0xFF, " 1K", "   RAM"},
                                                  {NO_BANK_SWITCH, 0x10FC00, 0x10FFFF, 1, 0xFF, "v1K", "V. RAM"},
                                                  {0}
                                                }; // end of list

//
// Nothing else is used in the generic platform
//
static const RAM_REGION      s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list
static const INPUT_REGION    s_inputRegion[]        PROGMEM = { {0} }; // end of list
static const OUTPUT_REGION   s_outputRegion[]       PROGMEM = { {0} }; // end of list
static const CUSTOM_FUNCTION s_customFunction[]     PROGMEM = { {NO_CUSTOM_FUNCTION}}; // end of list


CGenericBaseGame::CGenericBaseGame(
    ICpu *cpu,
    RomSize romSize
) : CGame( ((romSize == I2716) ? s_romRegion2716 : s_romRegion2732),
           s_ramRegion,
           s_ramRegion, // All Byte-wide.
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = cpu;
    m_cpu->idle();

    // Generic has no interrupts connected
    m_interrupt = ICpu::NMI;
    m_interruptAutoVector = true;
}


CGenericBaseGame::~CGenericBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}

