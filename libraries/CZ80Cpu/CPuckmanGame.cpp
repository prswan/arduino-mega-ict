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
#include "CPuckmanGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// Namco Set 1 from MAME, converted to 2532
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                       {0x3e,0x3f,0x47,0x77,0x85,0xe1,0x38,0x06,0x07,0x23,0x2b,0x07}, // 0
                                                       {0x32,0xd4,0x21,0xd2,0x4e,0x66,0xa4,0xc9,0x4d,0xfd,0x22,0x63}, // 1
                                                       {0x7e,0x00,0x86,0x7e,0x00,0x20,0x24,0x4d,0x90,0x21,0x3e,0x2a}, // 2
                                                       {0x00,0x00,0x00,0x50,0xfe,0xfe,0xbd,0x4f,0x41,0x5f,0x05,0x48}, // 3
                                                       {0} };  // end of list

//
// Namco Set 1 from MAME, converted to 2532
//
static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nSet1[0].data2n, 0xc1e6ab10, " 7D"}, // 2d(__scrambles)
                                                      {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nSet1[1].data2n, 0x1a6fb2d4, " 7E"}, // 2e(__scrambles)
                                                      {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nSet1[2].data2n, 0xbcdd1beb, " 7F"}, // 2f(__scrambles)"
                                                      {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nSet1[3].data2n, 0x67a513e4, " 7H"}, // 2h(__scrambles)"
                                                      {0} }; // end of list

//
// Bootleg "puckmanb" Set 1 from MAME
//
static const ROM_DATA2N s_romData2nBSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                        {0x3e,0x00,0x47,0x77,0x85,0xe1,0x38,0x06,0x07,0x23,0x2b,0x07}, // 0
                                                        {0x32,0xd4,0x21,0xd2,0x4e,0x66,0xa4,0xc9,0x4d,0xfd,0x22,0x63}, // 1
                                                        {0x7e,0x00,0x86,0x7e,0x00,0x20,0x24,0x4d,0x90,0x21,0x3e,0x2a}, // 2
                                                        {0x00,0x00,0x00,0x50,0xfe,0xfe,0xbd,0x4f,0x41,0x5f,0x05,0x48}, // 3
                                                        {0} };  // end of list

//
// Bootleg "puckmanb" Set 1 from MAME
//
static const ROM_REGION s_romRegionBSet1[] PROGMEM = { //
                                                       {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nBSet1[0].data2n, 0xfee263b3, " 7D"}, // 2d(__scrambles)
                                                       {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nBSet1[1].data2n, 0x39d1fc83, " 7E"}, // 2e(__scrambles)
                                                       {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nBSet1[2].data2n, 0x02083b03, " 7F"}, // 2f(__scrambles)"
                                                       {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nBSet1[3].data2n, 0x7a36fe55, " 7H"}, // 2h(__scrambles)"
                                                       {0} }; // end of list

IGame*
CPuckmanGame::createInstanceSet1(
)
{
    return (new CPuckmanGame(s_romData2nSet1, s_romRegionSet1));
}


IGame*
CPuckmanGame::createInstanceBSet1(
)
{
    return (new CPuckmanGame(s_romData2nBSet1, s_romRegionBSet1));
}


CPuckmanGame::CPuckmanGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CPuckmanBaseGame(  romData2n, romRegion )
{
}


