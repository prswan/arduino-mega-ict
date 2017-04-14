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
#include "CTaitoSpaceInvadersGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// SV Rev 4 from MAME (sisv)
//                                                  01   02   04   08   10   20   40   80  100  200
static const UINT16 s_romData2nSVRev4_r36[]    = {0x00,0x00,0xd4,0xf5,0xf5,0xdb,0xea,0x09,0x21,0xcd}; //
static const UINT16 s_romData2nSVRev4_r35[]    = {0x32,0x2a,0xcd,0x00,0xcd,0x14,0x25,0x7d,0xc2,0x3a}; //
static const UINT16 s_romData2nSVRev4_r34[]    = {0x32,0xc1,0xcd,0x67,0x02,0x18,0x0a,0xc3,0x00,0xf1}; //
static const UINT16 s_romData2nSVRev4_r31[]    = {0xcd,0x74,0x00,0xd3,0xaf,0x14,0x09,0x13,0xb8,0x05}; //
static const UINT16 s_romData2nSVRev4_r42[]    = {0xc3,0xc9,0x21,0xa7,0x06,0x3e,0x18,0x20,0x20,0xfe}; //
static const UINT16 s_romData2nSVRev4_r41[]    = {0x00,0x39,0x7a,0xfa,0x00,0x00,0x00,0x40,0x01,0x00}; //

//
// SV Rev 4 from MAME (sisv)
//
static const ROM_REGION s_romRegionSVRev4[] PROGMEM = { //
                                                        {NO_BANK_SWITCH, 0x0000, 0x0400, s_romData2nSVRev4_r36, 0x86bb8cb6, "r36"}, //
                                                        {NO_BANK_SWITCH, 0x0400, 0x0400, s_romData2nSVRev4_r35, 0xfebe6d1a, "r35"}, //
                                                        {NO_BANK_SWITCH, 0x0800, 0x0400, s_romData2nSVRev4_r34, 0xa08e7202, "r34"}, //
                                                        {NO_BANK_SWITCH, 0x1400, 0x0400, s_romData2nSVRev4_r31, 0x1293b826, "r31"}, //
                                                        {NO_BANK_SWITCH, 0x1800, 0x0400, s_romData2nSVRev4_r42, 0xa9011634, "r42"}, //
                                                        {NO_BANK_SWITCH, 0x1C00, 0x0400, s_romData2nSVRev4_r41, 0x58730370, "r41"}, //
                                                        {0} }; // end of list


IGame*
CTaitoSpaceInvadersGame::createInstanceSVRev4(
)
{
    return (new CTaitoSpaceInvadersGame(s_romRegionSVRev4));
}


CTaitoSpaceInvadersGame::CTaitoSpaceInvadersGame(
    const ROM_REGION *romRegion
) : CSpaceInvadersBaseGame( romRegion )
{
}


