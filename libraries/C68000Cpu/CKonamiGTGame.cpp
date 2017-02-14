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
#include "CKonamiGTGame.h"

//
// See the base game cpp comments for details about this platform.
//

//
// From MAME (konamigt)
//                                            01   02   04   08   10   20   40   80  100  200  400  800 1000 2000 4000
static const UINT16 s_romData2n_12A[]    = {0x70,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; //
static const UINT16 s_romData2n_12C[]    = {0x00,0x01,0x01,0x01,0x01,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; //
static const UINT16 s_romData2n_13A[]    = {0x13,0x00,0x13,0x00,0x13,0x80,0xb1,0x3d,0x2a,0x02,0x00,0x30,0xff,0x66,0xc1}; //
static const UINT16 s_romData2n_13C[]    = {0xc0,0x05,0xc0,0x07,0xc0,0x00,0xfc,0x7c,0x04,0x44,0x05,0x3c,0xce,0x02,0xc2}; //
static const UINT16 s_romData2n_14A[]    = {0xff,0x00,0x0f,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x33,0x00,0x11,0x00}; //
static const UINT16 s_romData2n_14C[]    = {0xf0,0xff,0xff,0xf0,0x0f,0x0f,0x00,0x0f,0x00,0xff,0x00,0x44,0x00,0x11,0x00}; //
static const UINT16 s_romData2n_15A[]    = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0xf4}; //
static const UINT16 s_romData2n_15C[]    = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x44}; //

//
// From MAME (konamigt)
//
static const ROM_REGION s_romRegion[] PROGMEM = { //
                                                  {NO_BANK_SWITCH, 0x00000000, 0x00008000, s_romData2n_12A, 0x56245bfd, "12A"}, // PROM0
                                                  {NO_BANK_SWITCH, 0x00000001, 0x00008000, s_romData2n_12C, 0x8d651f44, "12C"}, // PROM0
                                                  {NO_BANK_SWITCH, 0x00010000, 0x00008000, s_romData2n_13A, 0x3407b7cb, "13A"}, // PROM1
                                                  {NO_BANK_SWITCH, 0x00010001, 0x00008000, s_romData2n_13C, 0x209942d4, "13C"}, // PROM1
                                                  {NO_BANK_SWITCH, 0x00020000, 0x00008000, s_romData2n_14A, 0xaef7df48, "14A"}, // PROM2
                                                  {NO_BANK_SWITCH, 0x00020001, 0x00008000, s_romData2n_14C, 0xe9bd6250, "14C"}, // PROM2
                                                  {NO_BANK_SWITCH, 0x00030000, 0x00008000, s_romData2n_15A, 0x94bd4bd7, "15A"}, // PROM3
                                                  {NO_BANK_SWITCH, 0x00030001, 0x00008000, s_romData2n_15C, 0xb7236567, "15C"}, // PROM3
                                                  {0} }; // end of list


IGame*
CKonamiGTGame::createInstance(
)
{
    return (new CKonamiGTGame(s_romRegion));
}


CKonamiGTGame::CKonamiGTGame(
    const ROM_REGION *romRegion
) : CKonamiGX400BaseGame( romRegion )
{
}


