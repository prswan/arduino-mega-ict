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
#include "CHustlerGame.h"

//
// See the base game cpp comments for details about this platform.
//


//
// Found on a Zaccaria Scramble PCB set that uses 2708 EPROMS.
// Similar to "hustlerb4" in MAME.
//                                                            01   02   04   08   10   20   40   80  100  200
static const UINT16 s_romData2nZacScramConvSet2_2XC[]    = {0x32,0x01,0xc3,0x77,0x77,0x85,0x7e,0x32,0xcd,0x42};
static const UINT16 s_romData2nZacScramConvSet2_2P[]     = {0x00,0x00,0x00,0x00,0x01,0x00,0xc3,0xc9,0x07,0x40};
static const UINT16 s_romData2nZacScramConvSet2_2XE[]    = {0x08,0xe6,0xee,0x26,0x81,0xfe,0xc6,0x19,0xe0,0x35};
static const UINT16 s_romData2nZacScramConvSet2_2M[]     = {0x35,0xc0,0x1e,0x16,0x2d,0xff,0x35,0x26,0x20,0x0f};
static const UINT16 s_romData2nZacScramConvSet2_2XF[]    = {0x32,0x93,0x21,0xc9,0x21,0x33,0x3a,0x10,0x21,0x72};
static const UINT16 s_romData2nZacScramConvSet2_2L[]     = {0x12,0xc9,0x01,0x83,0xa0,0x7e,0x81,0x3e,0xca,0x3a};
static const UINT16 s_romData2nZacScramConvSet2_2XH[]    = {0x03,0x32,0x81,0x32,0xd0,0x18,0x95,0x81,0xcd,0xe6};
static const UINT16 s_romData2nZacScramConvSet2_2J[]     = {0x1c,0x3e,0xc3,0x20,0x28,0x21,0x24,0xe8,0x06,0x11};
static const UINT16 s_romData2nZacScramConvSet2_2XJ[]    = {0x80,0x80,0x80,0x20,0x12,0x00,0x12,0xc6,0x81,0x7a};
static const UINT16 s_romData2nZacScramConvSet2_2H[]     = {0x6f,0xed,0xe6,0x22,0xef,0x02,0x42,0x24,0x85,0x16};
static const UINT16 s_romData2nZacScramConvSet2_2XL[]    = {0x0f,0x0f,0x0f,0x0f,0xf0,0xff,0xf0,0xf0,0xff,0xf0};
static const UINT16 s_romData2nZacScramConvSet2_2F[]     = {0x77,0x55,0x33,0xff,0x33,0xcc,0xee,0x00,0x00,0xcc};

static const ROM_REGION s_romRegionZacScramConvSet2[] PROGMEM = { //
                                                                  {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2nZacScramConvSet2_2XC, 0x9443ff23, "2XC"}, // N01_2XC
                                                                  {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2nZacScramConvSet2_2P,  0xe1e8be0c, " 2P"}, // N09_2P
                                                                  {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2nZacScramConvSet2_2XE, 0x5d69c0ba, "2XE"}, // N02_2XE
                                                                  {NO_BANK_SWITCH, 0x0C00, 0x0400, s_romData2nZacScramConvSet2_2M,  0xaf3cd4ec, " 2M"}, // N10_2M
                                                                  {NO_BANK_SWITCH, 0x1000, 0x0400, s_romData2nZacScramConvSet2_2XF, 0xa1c12ee8, "2XF"}, // N03_2XF
                                                                  {NO_BANK_SWITCH, 0x1400, 0x0400, s_romData2nZacScramConvSet2_2L,  0x64426a6f, " 2L"}, // N11_2L
                                                                  {NO_BANK_SWITCH, 0x1800, 0x0400, s_romData2nZacScramConvSet2_2XH, 0x0885bb42, "2XH"}, // N04_2XH
                                                                  {NO_BANK_SWITCH, 0x1C00, 0x0400, s_romData2nZacScramConvSet2_2J,  0x00e00a12, " 2J"}, // N12_2J
                                                                  {NO_BANK_SWITCH, 0x2000, 0x0400, s_romData2nZacScramConvSet2_2XJ, 0xf66af3ab, "2XJ"}, // N05_2XJ
                                                                  {NO_BANK_SWITCH, 0x2400, 0x0400, s_romData2nZacScramConvSet2_2H,  0x732b1e3e, " 2H"}, // N13_2H
                                                                  {NO_BANK_SWITCH, 0x2800, 0x0400, s_romData2nZacScramConvSet2_2XL, 0xfb881a59, "2XL"}, // N06_2XL
                                                                  {NO_BANK_SWITCH, 0x2C00, 0x0400, s_romData2nZacScramConvSet2_2F,  0x12c04c70, " 2F"}, // N14_2F
                                                                  {0} }; // end of list


IGame*
CHustlerGame::createInstanceZacScramConvSet2(
)
{
    return (new CHustlerGame(HUSTLER_SCRAMBLE, s_romRegionZacScramConvSet2));
}


CHustlerGame::CHustlerGame(
    const Base base,
    const ROM_REGION *romRegion
) : CScrambleBaseGame( base, romRegion )
{
}


