//
// Copyright (c) 2025, Paul R. Swan
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
#include "CPinball1B1165BaseGame.h"
#include "C2650Cpu.h"
#include "zutil.h"
#include <DFR_Key.h>

//
// From PinMAME
//
// 2114 & 6514 (1024x4) - Effective 8 bit RAM.
// Read:  1800 - 1bff                         (and mirrored addresses)
// Write: 1800 - 18b0 WHEN DIP.SW4 NOT SET!   (and mirrored addresses)
// Write: 1800 - 1bff WHEN DIP.SW4 SET!       (and mirrored addresses)
//
// Farfalla Schematics
//  - CS3 with A7, A8, A9, PROGR
// xx380 + 1800 = 1B80 so the above is a typo and should be 1B7F (?) so the top 128 bytes is protected?
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                  "012", "012345"
                                                  {NO_BANK_SWITCH, 0x1800,      0x1B7F,      1, 0x0F, " 10", "Prog. "}, // 6514 battery backed
                                                  {NO_BANK_SWITCH, 0x1800,      0x1B7F,      1, 0xF0, "  9", "Prog. "}, // 2114
                                                  {NO_BANK_SWITCH, 0x1B80,      0x1BFF,      1, 0x0F, " 10", "DS4 on"}, // DS4 ON - 6514 battery backed DS4
                                                  {NO_BANK_SWITCH, 0x1B80,      0x1BFF,      1, 0xF0, "  9", "DS4 on"}, // DS4 ON - 2114
                                                  {0}
                                                }; // end of list

//
// Just the permanent accessible RAM mapped here.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                  "012", "012345"
                                                          {NO_BANK_SWITCH, 0x1800,      0x1B7F,      1, 0xFF, " - ", "Prog. "}, // Program RAM
                                                          {0}
                                                        }; // end of list

//
// The VFD's are scanned out of a single 4-bit 2101, write only.
// This is actually duplicated with the program RAM, writes go to both, reads come from Program RAM?
//
// Farfalla Schematics
//  - CS3 with A8, A9, == 0,
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                        "012", "012345"
                                                           {NO_BANK_SWITCH, 0x1800, 0x18FF, 1, 0x0F, " 23", "Disp  "}, // 2101
                                                           {0}
                                                         }; // end of list

//
// Input region is the same for all versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                               "012", "012345"
                                                      {NO_BANK_SWITCH, 0x10000L, 0x3F, " 25", "COL 05"}, // CTRL - Switches Col Returns 0-5
                                                      {NO_BANK_SWITCH, 0x10000L, 0xC0, " 15", "COL 67"}, // CTRL - Switches Col Returns 6-7
                                                      {NO_BANK_SWITCH, 0x14000L, 0x0F, " 24", "DIP   "}, // DATA - Dip Switches
                                                      {NO_BANK_SWITCH, 0x14000L, 0xF0, " 18", "ActSnd"}, // DATA - Sound Response
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x10000L, 0x07, 0x00, " 33", "ROW 07"}, // CTRL 3-to-8 ROW 0-7 drive
                                                        {NO_BANK_SWITCH, 0x10000L, 0x08, 0x00, " 33", "RFSH  "}, // CTRL - Watchdog, TP7 pads to disable.
                                                        {NO_BANK_SWITCH, 0x10000L, 0x20, 0x00, " 11", "LED   "}, // CTRL - PCB LED
                                                        {NO_BANK_SWITCH, 0x14000L, 0x0F, 0x00, "  6", "Snd 03"}, // DATA - Sound Command bits 0-3
                                                        {NO_BANK_SWITCH, 0x14000L, 0xF0, 0x00, " 10", "Snd 47"}, // DATA - Sound Command bits 4-7
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                       "0123456789"
                                                            {CPinball1B1165BaseGame::nvRamCrc,       "NV RAM CRC"},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CPinball1B1165BaseGame::CPinball1B1165BaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction )
{
    m_cpu = new C2650Cpu();
    m_cpu->idle();

    // There is only a single interrupt pin on the 2650.
    m_interrupt = ICpu::IRQ0;

    // Was not able to find an external vector on the schematic
    m_interruptAutoVector = true;
}


CPinball1B1165BaseGame::~CPinball1B1165BaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


PERROR
CPinball1B1165BaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}


//
// Custom function to calculate the CRC of the NV RAM to verify it's keeping data
//
PERROR
CPinball1B1165BaseGame::nvRamCrc(
    void *cPinball1B1165BaseGame
)
{
    CPinball1B1165BaseGame *thisGame = (CPinball1B1165BaseGame *) cPinball1B1165BaseGame;
    PERROR error = errorSuccess;
    UINT32 crc = 0;

    //
    // The CRC function is currently intended for ROM's so we make a
    // ROM_REGION representation for the battery backed RAM we want to CRC
    //
    // Known CRC, can be used to verify the function of DIP 4 (protect) as well
    //   Soccer Kings A - RAM Check All    => 0xff9190cc
    //   Soccer Kings A - RAM Write All AD => 0x34a27257
    //
    // We can't use the same mechanism as Dambusters by using the romCheck.calculateCrc because
    // that function only supports RomRegions that are 8-bits wide and this is a 4-bit region.
    //

    RAM_REGION nvRamRegion = {NO_BANK_SWITCH, 0x1B80, 0x1BFF, 1, 0x0F, " 10", "DS4 on"};

    {
        UINT16 data = 0;
        UINT8  data8 = 0;
        UINT32 tempCrc = 0;

        for (UINT32 address = nvRamRegion.start ; address <= nvRamRegion.end ; address++)
        {
            error = thisGame->m_cpu->memoryRead(address, &data);

            if (FAILED(error))
            {
                break;
            }

            data8 = (UINT8) (data & 0x0F); // only the lower 4 bits are battery backed.
            tempCrc = crc32(tempCrc, &data8, sizeof(data8));
        }

        if (SUCCESS(error))
        {
            crc = tempCrc;
        }
    }

    if (SUCCESS(error))
    {
        error = errorCustom;

        error->code = ERROR_SUCCESS;
        error->description = "OK:";
        error->description += nvRamRegion.location;
        STRING_UINT32_HEX(error->description, crc);
    }

    return error;
}


