//
// Copyright (c) 2017, Paul R. Swan
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
#include "CGradiusIIIGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// From MAME (gradius3)
//                                            01   02   04   08   10   20   40   80  100  200  400  800 1000 2000 4000 800010000
static const UINT16 s_romData2n_F15[]    = {0x06,0x00,0x00,0x00,0x00,0x11,0x06,0x00,0x47,0x00,0x13,0x0a,0x00,0x23,0x20,0x42,0x00}; //
static const UINT16 s_romData2n_E15[]    = {0x00,0x00,0x00,0x00,0x00,0xc0,0x66,0x40,0xfa,0x00,0xc6,0xb6,0x31,0xc6,0x86,0xa8,0x10}; //

//
// From MAME (gradius3)
//
static const ROM_REGION s_romRegion[] PROGMEM = { //
                                                  {NO_BANK_SWITCH, 0x00000000, 0x00020000, s_romData2n_F15, 0xcffd103f, "F15"}, //
                                                  {NO_BANK_SWITCH, 0x00000001, 0x00020000, s_romData2n_E15, 0x0b968ef6, "E15"}, //
                                                  {0} }; // end of list

//
// From MAME (gradius3j)
//                                            01   02   04   08   10   20   40   80  100  200  400  800 1000 2000 4000 800010000
static const UINT16 s_romData2nJ_F15[]   = {0x06,0x00,0x00,0x00,0x00,0x11,0x06,0x00,0x47,0x00,0x13,0x0c,0x23,0x00,0x00,0x00,0xff}; //
static const UINT16 s_romData2nJ_E15[]   = {0x00,0x00,0x00,0x00,0x00,0xc0,0x66,0x40,0xfa,0x00,0xc6,0xb6,0xc7,0x2a,0x4f,0x10,0x80}; //

//
// From MAME (gradius3j)
//
static const ROM_REGION s_romRegionJ[] PROGMEM = { //
                                                  {NO_BANK_SWITCH, 0x00000000, 0x00020000, s_romData2nJ_F15, 0x70c240a2, "F15"}, //
                                                  {NO_BANK_SWITCH, 0x00000001, 0x00020000, s_romData2nJ_E15, 0xbbc300d4, "E15"}, //
                                                  {0} }; // end of list


IGame*
CGradiusIIIGame::createInstance(
)
{
    return (new CGradiusIIIGame(s_romRegion));
}


IGame*
CGradiusIIIGame::createInstanceJ(
)
{
    return (new CGradiusIIIGame(s_romRegionJ));
}


CGradiusIIIGame::CGradiusIIIGame(
    const ROM_REGION *romRegion
) : CKonamiGX945BaseGame( romRegion )
{
}


