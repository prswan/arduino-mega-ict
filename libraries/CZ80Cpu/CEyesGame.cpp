//
// Copyright (c) 2023, Paul R. Swan
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
#include "CEyesGame.h"
#include "Bitswap.h"

//
// See the base game cpp comments for details about this platform.
//
// Watchdog Disable:
//   There doesn't seem to be a way to disable it but it only resets the CPU an IO port latch.

//
// Zaccaria Set 1 "eyeszac" from MAME, converted to 2532
//
static const ROM_DATA2N s_romData2nZacSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800
                                                          {0xaf,0x06,0x09,0x0b,0xc3,0x2b,0x00,0x5f,0x17,0xfe,0x28,0x09}, // 1
                                                          {0xfd,0x5b,0xfd,0x5f,0x5a,0x04,0x5a,0xc3,0xe3,0x11,0x01,0xe3}, // 2
                                                          {0x52,0x67,0x52,0x64,0x67,0x66,0x64,0x52,0x54,0x44,0xe1,0x3e}, // 3
                                                          {0x27,0xf5,0x30,0x06,0x57,0xe3,0x3c,0xf4,0x67,0xfe,0xf8,0xd2}, // 4
                                                          {0} };  // end of list

//
// Zaccaria Set 1 "eyeszac" from MAME
//
static const ROM_REGION s_romRegionZacSet1[] PROGMEM = { //
                                                         {NO_BANK_SWITCH, 0x0000, 0x1000, s_romData2nZacSet1[0].data2n, 0xe555b265, " 7E"},
                                                         {NO_BANK_SWITCH, 0x1000, 0x1000, s_romData2nZacSet1[1].data2n, 0xd6d73eb5, " 7F"},
                                                         {NO_BANK_SWITCH, 0x2000, 0x1000, s_romData2nZacSet1[2].data2n, 0x604c940c, " 7H"},
                                                         {NO_BANK_SWITCH, 0x3000, 0x1000, s_romData2nZacSet1[3].data2n, 0xacc9cd8b, " 7J"},
                                                         {0} }; // end of list


//
// Handle data bit swapping on the program ROM.
//
PERROR
CEyesGame::onDataRemap(
    void   *cEyesGame,
    UINT32  address,
    UINT16  dataIn,
    UINT16 *dataOut
)
{
    PERROR error = errorSuccess;

    if (address < 0x4000)
    {
        *dataOut = INBITSWAP8((UINT8) dataIn, 7,6,3,4,5,2,1,0);
    }
    else
    {
        *dataOut = dataIn;
    }

    return error;
}


IGame*
CEyesGame::createInstanceZacSet1(
)
{
    return (new CEyesGame(s_romData2nZacSet1, s_romRegionZacSet1));
}


CEyesGame::CEyesGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CPuckmanBaseGame(  romData2n, romRegion, onDataRemap )
{
}


