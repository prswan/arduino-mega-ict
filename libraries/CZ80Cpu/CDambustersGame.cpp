//
// Copyright (c) 2019, Paul R. Swan
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
#include "CDambustersGame.h"

//
// See the base game cpp comments for details about this platform.
//
// The ROM layout is from MAME Dambustr.cpp. Note that some ROMs have both
// address and data lines swapped around. The regions here are described
// based on the ROM image with the decoding performed as part of the CPU
// access. See CDambustersBaseGame onAddressRemap & onDataRemap functions.
//

//
// UK Set 1 from MAME.
//                                                        A00  A01  A02  A03  A04  A05  A06  A07  A08  A09  A10  A11
static const ROM_DATA2N s_romData2nUKSet1[] PROGMEM = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                        {0x00,0x01,0x00,0x01,0x36,0x00,0xff,0xc9,0x21,0x00,0x01,0xc9}, // db11
                                                        {0x03,0xfe,0x28,0x10,0xe0,0x36,0x1b,0x13,0x7d,0xea,0xcd,0x00}, // db06
                                                        {0x00,0x32,0xc1,0x45,0xe8,0x20,0x20,0x21,0x18,0xb0,0xa2,0x00}, // db10
                                                        {0x69,0x6e,0x00,0x4d,0x69,0x74,0x4f,0x43,0x00,0x4a,0x20,0x59}, // db12
                                                        {0x41,0x24,0x36,0x80,0x36,0x62,0x62,0x00,0xff,0x44,0x01,0x97}, // db08
                                                        {0xc8,0x00,0x01,0x25,0x01,0x37,0x01,0x01,0x86,0x52,0x54,0x00}, // db07
                                                        {0x52,0xff,0xff,0x00,0xfe,0x01,0x7b,0x66,0xb7,0xa6,0xff,0x2a}, // db09
                                                        {0x00,0x01,0x4f,0x49,0x20,0x20,0x45,0x55,0x31,0xfd,0x20,0x6e}, // db05
                                                        {0} }; // end of list
//
// UK Set 1 from MAME.
//
static const ROM_REGION s_romRegionUKSet1[] PROGMEM = { //
                                                        {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nUKSet1[0].data2n, 0x9e6b34fe, "d11"}, //
                                                        {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nUKSet1[1].data2n, 0x35dcee01, "d06"}, //
                                                        {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nUKSet1[2].data2n, 0xc129c57b, "d10"}, //
                                                        {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nUKSet1[3].data2n, 0xea4c65f5, "d12"}, //
                                                        {NO_BANK_SWITCH, 0x4000, 0x1000, s_romData2nUKSet1[4].data2n, 0xfd041ff4, "d08"}, //
                                                        {NO_BANK_SWITCH, 0x5000, 0x1000, s_romData2nUKSet1[5].data2n, 0x675b1f5e, "d07"}, //
                                                        {NO_BANK_SWITCH, 0x6000, 0x1000, s_romData2nUKSet1[6].data2n, 0x57164563, "d09"}, //
                                                        {NO_BANK_SWITCH, 0x7000, 0x1000, s_romData2nUKSet1[7].data2n, 0x75659ecc, "d05"}, //
                                                        {0} }; // end of list

IGame*
CDambustersGame::createInstanceUKSet1(
)
{
    return (new CDambustersGame(s_romData2nUKSet1, s_romRegionUKSet1));
}


CDambustersGame::CDambustersGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CDambustersBaseGame( romData2n, romRegion )
{
}


