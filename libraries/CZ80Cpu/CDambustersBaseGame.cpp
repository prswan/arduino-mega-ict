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
#include "CDambustersBaseGame.h"
#include "CZ80Cpu.h"
#include "Bitswap.h"
#include "CRomCheck.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   Tie CPU board IC 8L (74LS00) pin 10 to GND.
//
// Board Designations:
//   'r' - ROM board.
//   ...everything else main board.
//
// Z80 Compatibility Notes
//   * IO space is not used.
//   * M1 (1st machine cycle) is not used.
//   * Access to the background RAM via VRAM causes the CPU to WAIT when access occurs outside of HBLANK
//     (except when in VBLANK).
//     This gate is Z80 clock input synchronous. It *almost* works as is with the tester but it looks like
//     the HBLANK period is so short that it can shut off the VRAM before the tester has actually got
//     the data off the bus.
//
//     To work around this, in CZ80Cpu we synchronize with HBLANK by waiting for the WAIT to occur and clear
//     before letting the cycle go ahead. This ensure that the WAIT doesn't occur in the middle of the cycle.
//


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0xC000, 0xC7FF, 1, 0xFF, "R13", "Prog. "}, // "Program RAM, 6116"
                                                  {NO_BANK_SWITCH, 0xD800, 0xD8FF, 1, 0x0F, " 4F", "ObjRam"}, // "Object RAM, 2101, 256 Bytes used."
                                                  {NO_BANK_SWITCH, 0xD800, 0xD8FF, 1, 0xF0, " 5F", "ObjRam"}, // "Object RAM, 2101, 256 Bytes used."
                                                  //
                                                  // See note above about access restrictions w.r.t HBLANK & WAIT.
                                                  // These regions are access with special support in the CZ80Cpu triggered via address 0x10xxxx.
                                                  //
                                                  //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x10D000, 0x10D3FF, 1, 0x0F, " 3H", "BkVRam"}, // "Background VRAM, 2114"
                                                  {NO_BANK_SWITCH, 0x10D000, 0x10D3FF, 1, 0xF0, " 3F", "BkVRam"}, // "Background VRAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                        "012", "012345"
                                                          {NO_BANK_SWITCH, 0xC000, 0xC7FF, 1, 0xFF, " --", "Prog. "}, // "Program RAM, 6116"
                                                          {NO_BANK_SWITCH, 0xD800, 0xD8FF, 1, 0xFF, "45F", "ObjRam"}, // "Object RAM, 2101, 256 Bytes used., 4F/5F"
                                                          //
                                                          // See note above about access restrictions w.r.t HBLANK & WAIT.
                                                          // These regions are access with special support in the CZ80Cpu triggered via address 0x10xxxx.
                                                          //
                                                          //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x10D000, 0x10D3FF, 1, 0xFF, "3HF", "BkVRam"}, // "Background VRAM, 2114, 3H/3F"
                                                          {0}
                                                        }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                               "012", "012345"
                                                      {NO_BANK_SWITCH, 0xE000L, 0x0C,  " 9D", "IN SW0"}, // Read Inputs (IN0)
                                                      {NO_BANK_SWITCH, 0xE000L, 0xF3,  " 9E", "IN SW0"}, // Read Inputs (IN0)
                                                      {NO_BANK_SWITCH, 0xE800L, 0xC0,  " 9C", "DIPSW1"}, // Read DIP switches (IN1)
                                                      {NO_BANK_SWITCH, 0xE800L, 0x2F,  " 9F", "IN SW1"}, // Read Inputs (IN1)
                                                      {NO_BANK_SWITCH, 0xF000L, 0x0F,  " 9C", "DIPSW "}, // Read DIP switches (IN2)
                                                      {NO_BANK_SWITCH, 0xF800L, 0xFF,  " 8L", "WD Res"}, // Watchdog reset
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0xE000L, 0x01, 0x00,  " 9M", "Lamp 1"}, // Player 1 start lamp
                                                        {NO_BANK_SWITCH, 0xE001L, 0x01, 0x00,  " 9M", "Lamp 2"}, // Player 2 start lamp
                                                        {NO_BANK_SWITCH, 0xE002L, 0x01, 0x00,  " 9M", "Lck Ot"}, // Coin lockout
                                                        {NO_BANK_SWITCH, 0xE003L, 0x01, 0x00,  " 9M", "Cntr  "}, // Coin counter
                                                        {NO_BANK_SWITCH, 0xE004L, 0x01, 0x00,  " 9M", "9R1M  "}, // Rack speed?
                                                        {NO_BANK_SWITCH, 0xE005L, 0x01, 0x00,  " 9M", "9R470K"}, // Rack speed?
                                                        {NO_BANK_SWITCH, 0xE006L, 0x01, 0x00,  " 9M", "9R220K"}, // Rack speed?
                                                        {NO_BANK_SWITCH, 0xE007L, 0x01, 0x00,  " 9M", "9R100K"}, // Rack speed?
                                                        {NO_BANK_SWITCH, 0xE800L, 0x01, 0x00,  " 9L", "8R FS1"}, // Rack? sound enable
                                                        {NO_BANK_SWITCH, 0xE801L, 0x01, 0x00,  " 9L", "8S FS2"}, // Rack? sound enable
                                                        {NO_BANK_SWITCH, 0xE802L, 0x01, 0x00,  " 9L", "8T FS3"}, // Rack? sound enable
                                                        {NO_BANK_SWITCH, 0xE803L, 0x01, 0x00,  " 9L", "HitExp"}, // Base hit explosion
                                                        {NO_BANK_SWITCH, 0xE804L, 0x01, 0x00,  " 9L", "Fire 1"}, // Base Fire, loud
                                                        {NO_BANK_SWITCH, 0xE805L, 0x01, 0x00,  " 9L", "Fire 2"}, // Base Fire, normal
                                                        {NO_BANK_SWITCH, 0xE806L, 0x01, 0x00,  " 9L", "LFO V1"}, // LFO Volume 1
                                                        {NO_BANK_SWITCH, 0xE807L, 0x01, 0x00,  " 9L", "LFO V2"}, // LFO Volume 2
                                                        {NO_BANK_SWITCH, 0xF001L, 0x01, 0x00,  " 9N", "NMI En"}, // NMI enable
                                                        {NO_BANK_SWITCH, 0xF004L, 0x01, 0x00,  " 9N", "Stars "}, // Stars on
                                                        {NO_BANK_SWITCH, 0xF006L, 0x01, 0x00,  " 9N", "Flip X"}, // Flip X
                                                        {NO_BANK_SWITCH, 0xF007L, 0x01, 0x00,  " 9N", "Flip Y"}, // Flip Y
                                                        {NO_BANK_SWITCH, 0xF800L, 0x01, 0x00,  " 9J", "LFO F0"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0xF800L, 0x02, 0x00,  " 9J", "LFO F1"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0xF800L, 0x04, 0x00,  " 9J", "LFO F2"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0xF800L, 0x08, 0x00,  " 9J", "LFO F3"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0xF800L, 0x10, 0x00,  " 9J", "LFO F4"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0xF800L, 0x20, 0x00,  " 9J", "LFO F5"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0xF800L, 0x40, 0x00,  " 9J", "LFO F6"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0xF800L, 0x80, 0x00,  " 9J", "LFO F7"}, // LFO frequency
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                    "0123456789"
                                                            {CDambustersBaseGame::nvRamCrc,       "NV RAM CRC"},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CDambustersBaseGame::CDambustersBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    //
    // Dambusters does not appear to work properly with the CZ80ACpu implementation.
    // Possibly this is due to the additional address decode PROM on the daughter PCB
    //
    m_cpu = new CZ80Cpu(0x10D000, onAddressRemap, this, onDataRemap, this);
    m_cpu->idle();

    // The VBLANK interrupt is on the NMI pin.
    m_interrupt = ICpu::NMI;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;

}


CDambustersBaseGame::~CDambustersBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// This is a specific implementation for Dambusters that uses an externally
// maskable/resetable latch for the VBLANK interrupt on the NMI line.
PERROR
CDambustersBaseGame::interruptCheck(
)
{
    PERROR error = errorSuccess;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "OK:";

    for (int i = 0 ; i < 4 ; i++)
    {
        // Unmask the interrupt.
        m_cpu->memoryWrite(0xF001L, 0x01);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        1000);
        if (FAILED(error))
        {
            break;
        }

        // Mask the interrupt (also resets the latch)
        m_cpu->memoryWrite(0xF001L, 0x00);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        0);
        if (SUCCESS(error))
        {
            error = errorUnexpected;
            break;
        }
        else
        {
            error = errorSuccess;
        }

        // Unmask the interrupt. Irq should have been cleared.
        m_cpu->memoryWrite(0xF001L, 0x01);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        0);
        if (SUCCESS(error))
        {
            error = errorUnexpected;
            break;
        }
        else
        {
            error = errorSuccess;
        }

        // Mask the interrupt (also resets the latch)
        m_cpu->memoryWrite(0xF001L, 0x00);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        1000);
        if (SUCCESS(error))
        {
            error = errorUnexpected;
            break;
        }
        else
        {
            error = errorSuccess;
        }
    }

    return error;
}


//
// Handle address bit swapping on the program ROM.
//
PERROR
CDambustersBaseGame::onAddressRemap(
    void   *cDambustersBaseGame,
    UINT32  addressIn,
    UINT32 *addressOut
)
{
    PERROR error = errorSuccess;

    // Address line swap mapping
    if ( ((addressIn & 0xF000) == 0x0000) || // d11
         ((addressIn & 0xF000) == 0x2000) || // d10
         ((addressIn & 0xF000) == 0x3000) || // d12
         ((addressIn & 0xF000) == 0x6000) )  // d09
    {
        *addressOut = OUTBITSWAP16((UINT16) addressIn, 15,14,13,12,4,10,9,8,7,6,5,3,11,2,1,0);
    }
    else
    {
        *addressOut = addressIn;
    }

    return error;
}


//
// Handle data bit swapping on the program ROM.
//
PERROR
CDambustersBaseGame::onDataRemap(
    void   *cDambustersBaseGame,
    UINT32  address,
    UINT16  dataIn,
    UINT16 *dataOut
)
{
    PERROR error = errorSuccess;

    // MAME does some odd region moving prior to swapping the data
    if ( ((address & 0xF000) == 0x1000) || // d06
         ((address & 0xF000) == 0x4000) || // d08
         ((address & 0xF000) == 0x5000) )  // d07
    {
        *dataOut = INBITSWAP8((UINT8) dataIn, 7,6,5,1,3,2,4,0);
    }
    else
    {
        *dataOut = dataIn;
    }

    return error;
}


//
// Custom function to calculate the CRC of the NV RAM to verify it's keeping data
//
PERROR
CDambustersBaseGame::nvRamCrc(
    void *cDambustersBaseGame
)
{
    CDambustersBaseGame *thisGame = (CDambustersBaseGame *) cDambustersBaseGame;
    PERROR error = errorSuccess;

    //
    // The CRC function is currently intended for ROM's so we make a
    // ROM_REGION representation for the battery backed RAM we want to CRC
    //
    // Known CRC:
    //   RAM Check All    => 0xdd54b6c9
    //   RAM Write All AD => 0x9f5edd58
    //
    ROM_REGION nvRamRegion = {NO_BANK_SWITCH, 0xC000, 0x0800, NULL, 0x00000000, "R13"};

    CRomCheck romCheck( thisGame->m_cpu,
                        thisGame->m_romRegion,
                        (void *) thisGame );

    UINT32 crc = 0;

    error = romCheck.calculateCrc(&nvRamRegion, &crc);

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

