//
// Copyright (c) 2026, patzik
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
#include "CKikiKaikaiGame.h"

//
// ROM set, addresses & lengths are from MAME (src/mame/taito/kikikai.cpp).
//
// The 64KB H16 & H18 ROMs are mapped in 16KB banks at 0x8000 (the first half
// of H16 is fixed at 0x0000) so they are checked in parts, each with its own
// CRC. The CRCs & data2n samples were generated from the MAME ROM images.
//
//   H16 0x0000-0x7FFF - fixed at 0x0000
//   H18 0x0000-0xFFFF - banks 0-3
//   H16 0x8000-0xFFFF - banks 4-5
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800 1000 2000 4000
                                                       {0xed,0x56,0xcf,0xc3,0xe5,0xe5,0x04,0x00,0xed,0x08,0x06,0x08,0x42,0xc7,0xe1}, // a85-17.h16 0x0000-0x7FFF
                                                       {0x07,0xf8,0x36,0x36,0x07,0xc3,0xfd,0x57,0x01,0xd8,0x4f,0x3e,0x00,0xe4     }, // a85-16.h18 bank 0
                                                       {0x10,0x00,0x1b,0x9f,0x1a,0x08,0x08,0x8d,0x10,0x08,0x08,0x99,0x08,0x44     }, // a85-16.h18 bank 1
                                                       {0x10,0x00,0x18,0x04,0x04,0x00,0x00,0x00,0x9e,0x5a,0x08,0x01,0x01,0x01     }, // a85-16.h18 bank 2
                                                       {0x8b,0xcb,0x8a,0x06,0x00,0xef,0x74,0x96,0x96,0x96,0x30,0x07,0xcf,0x00     }, // a85-16.h18 bank 3
                                                       {0xe5,0xfd,0xc5,0x3a,0x06,0xcb,0x23,0x89,0x75,0xcb,0x3c,0xd2,0xc8,0x58     }, // a85-17.h16 bank 4
                                                       {0xc2,0x8f,0x08,0x00,0xc9,0x01,0x19,0xc2,0x07,0x3a,0x3b,0xdd,0xfb,0x8d     }, // a85-17.h16 bank 5
                                                       {0} }; // end of list

static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH,                            0x0000, 0x8000, s_romData2nSet1[0].data2n, 0x7d54c2c5, "H16"}, // a85-17.h16 0x0000-0x7FFF
                                                      {CKikiKaikaiBaseGame::onBankSwitchRomBank0, 0x8000, 0x4000, s_romData2nSet1[1].data2n, 0x6d91cc7a, "H18"}, // a85-16.h18 0x0000-0x3FFF
                                                      {CKikiKaikaiBaseGame::onBankSwitchRomBank1, 0x8000, 0x4000, s_romData2nSet1[2].data2n, 0xb259a70d, "H18"}, // a85-16.h18 0x4000-0x7FFF
                                                      {CKikiKaikaiBaseGame::onBankSwitchRomBank2, 0x8000, 0x4000, s_romData2nSet1[3].data2n, 0x4cfe2961, "H18"}, // a85-16.h18 0x8000-0xBFFF
                                                      {CKikiKaikaiBaseGame::onBankSwitchRomBank3, 0x8000, 0x4000, s_romData2nSet1[4].data2n, 0x78fdaf77, "H18"}, // a85-16.h18 0xC000-0xFFFF
                                                      {CKikiKaikaiBaseGame::onBankSwitchRomBank4, 0x8000, 0x4000, s_romData2nSet1[5].data2n, 0x8d584702, "H16"}, // a85-17.h16 0x8000-0xBFFF
                                                      {CKikiKaikaiBaseGame::onBankSwitchRomBank5, 0x8000, 0x4000, s_romData2nSet1[6].data2n, 0xb6d419fe, "H16"}, // a85-17.h16 0xC000-0xFFFF
                                                      {0} }; // end of list


IGame*
CKikiKaikaiGame::createInstanceSet1(
)
{
    return (new CKikiKaikaiGame(s_romData2nSet1, s_romRegionSet1));
}


CKikiKaikaiGame::CKikiKaikaiGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CKikiKaikaiBaseGame( romData2n, romRegion )
{
}

