//
// Copyright (c) 2020, Paul R. Swan
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
#include "CCrazyKongBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   Tie CPU board IC 4J (74LS00) pin 2 to GND (1K res and pads nearby)
//   On the PCB I was looking at the pads were already joined
//
// Board Designations:
//   This is the CPU/main board base.
//
// Z80 Compatibility Notes
//   The wait configuration is similar to Puckman.
//


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x006000, 0x0067FF, 1, 0xFF, " 5B", "Wk1Ram"}, // 6116
                                                  {NO_BANK_SWITCH, 0x006800, 0x006BFF, 1, 0x0F, " 4B", "Wk2Ram"}, // 2114
                                                  {NO_BANK_SWITCH, 0x006800, 0x006BFF, 1, 0xF0, " 4A", "Wk2Ram"}, // 2114
                                                  // This RAM needs strict cycle sync. It's marginal at best :(
//                                                  {NO_BANK_SWITCH, 0x208800, 0x2088FF, 1, 0x0F, "10C", "BigRam"}, // 2101, RD/WR 1
//                                                  {NO_BANK_SWITCH, 0x208800, 0x2088FF, 1, 0xF0, "10A", "BigRam"}, // 2101, RD/WR 1
                                                  {NO_BANK_SWITCH, 0x209000, 0x2093FF, 1, 0x0F, "10H", "VidRam"}, // 2114, RD/WR 2
                                                  {NO_BANK_SWITCH, 0x209000, 0x2093FF, 1, 0xF0, "10J", "VidRam"}, // 2114, RD/WR 2
                                                  // This RAM needs VBLANK sync & address remapping

                                                  // This RAM needs strict cycle sync. It's marginal at best :(
                                                  {CCrazyKongBaseGame::onBankSwitchWaitVblank, 0x008800, 0x0088FF, 1, 0x0F, "10C", "BigRam"}, // 2101, RD/WR 1
                                                  {CCrazyKongBaseGame::onBankSwitchWaitVblank, 0x008800, 0x0088FF, 1, 0xF0, "10A", "BigRam"}, // 2101, RD/WR 1
//                                                  {CCrazyKongBaseGame::onBankSwitchWaitVblank, 0x009000, 0x0093FF, 1, 0x0F, "10H", "VidRam"}, // 2114, RD/WR 2
//                                                  {CCrazyKongBaseGame::onBankSwitchWaitVblank, 0x009000, 0x0093FF, 1, 0xF0, "10J", "VidRam"}, // 2114, RD/WR 2

                                                  {CCrazyKongBaseGame::onBankSwitchWaitVblank, 0x009800, 0x009BFF, 1, 0x0F, " 6F", "CD Ram"}, // 2114, RD/WR 3, A5 skipped
                                                  {CCrazyKongBaseGame::onBankSwitchWaitVblank, 0x009800, 0x009BFF, 1, 0xF0, " 6H", "CD Ram"}, // 2114, RD/WR 3, A5 skipped
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x006000, 0x0067FF, 1, 0xFF, " 5B", "Wk1Ram"}, // 6116
                                                          {NO_BANK_SWITCH, 0x006800, 0x006BFF, 1, 0xFF, "4AB", "Wk2Ram"}, // 2114
                                                          // The video RAM is too marginal for the randomAcess test
                                                          // {NO_BANK_SWITCH, 0x208800, 0x2088FF, 1, 0xFF, "10-", "BigRam"}, // 2101, RD/WR 1
                                                          // {NO_BANK_SWITCH, 0x209000, 0x2093FF, 1, 0xFF, "10-", "VidRam"}, // 2114, RD/WR 2
                                                          // {CCrazyKongBaseGame::onBankSwitchWaitVblank, 0x009800, 0x009BFF, 1, 0xFF, "6HF", "CD Ram"}, // 2114, RD/WR 3, A5 skipped
                                                          {0}
                                                        }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                            "012", "012345"
                                                           {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                             "012", "012345"
                                                      {NO_BANK_SWITCH, 0xA000, 0xFF, "2JF", "P1    "},
                                                      {NO_BANK_SWITCH, 0xA800, 0xFF, "2JK", "P2    "},
                                                      {NO_BANK_SWITCH, 0xB000, 0xFF, "2BK", "DSW   "},
                                                      {NO_BANK_SWITCH, 0xB800, 0xFF, " 2L", "SYSTEM"},
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x0A000, 0x01, 0x00, " 4L", "NMI En"}, //
                                                        {NO_BANK_SWITCH, 0x0A001, 0x01, 0x00, " 4L", "H. Inv"}, //
                                                        {NO_BANK_SWITCH, 0x0A002, 0x01, 0x00, " 4L", "V. Inv"}, //
                                                        {NO_BANK_SWITCH, 0x0A004, 0x01, 0x00, " 4L", "VCTR  "}, // "sample trigger"
                                                        {NO_BANK_SWITCH, 0x0A007, 0x01, 0x00, " 4L", "SCS   "}, // 8910 select
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CCrazyKongBaseGame::CCrazyKongBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : m_vblankAccessCount(0),
    CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction )
{
    m_cpu = new CZ80ACpu(0,
                         onAddressRemap,
                         this,
                         NO_DATA_REMAP,
                         NULL,
                         CZ80ACpu::CYCLE_TYPE_CRAZYKONG);

    m_cpu->idle();

    // The VBLANK interrupt is on the NMI pin.
    m_interrupt = ICpu::NMI;

    // TBD
    m_interruptAutoVector = true;
}


CCrazyKongBaseGame::~CCrazyKongBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// Uses a combined reset/mask bit.
//
PERROR
CCrazyKongBaseGame::interruptCheck(
)
{
    PERROR error = errorSuccess;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "OK:";

    for (int i = 0 ; i < 4 ; i++)
    {
        // Unmask the interrupt.
        m_cpu->memoryWrite(0xA000L, 0x01);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        1000);
        if (FAILED(error))
        {
            break;
        }

        // Mask the interrupt (also resets the latch)
        m_cpu->memoryWrite(0xA000L, 0x00);

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
        m_cpu->memoryWrite(0xA000L, 0x01);

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
        m_cpu->memoryWrite(0xA000L, 0x00);

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
// Wait for the VBLANK period by using the CPU interrupt
//
PERROR
CCrazyKongBaseGame::waitVblank(
)
{
    PERROR error = errorSuccess;

    // Unmask the interrupt.
    m_cpu->memoryWrite(0xA000L, 0x01);

    error = m_cpu->waitForInterrupt(m_interrupt,
                                    true,
                                    1000);

    // Mask the interrupt (also resets the latch)
    m_cpu->memoryWrite(0xA000L, 0x00);

    return error;
}


//
// The bank switch call is used to reset synchronization at the start
// of the test run.
//
PERROR
CCrazyKongBaseGame::onBankSwitchWaitVblank(
    void   *cCrazyKongBaseGame
)
{
    PERROR             error      = errorSuccess;
    CCrazyKongBaseGame *thisGame  = (CCrazyKongBaseGame *) cCrazyKongBaseGame;

    // Start of a new access run so reset the counter
    thisGame->m_vblankAccessCount = 0;

    return error;
}


//
// Handle the skipped A5 on the RD/WR 3 RAM and synchronization with the
// VBLANK period. The heuristic assumes 4 cycles will fit in one blanking
// period that seems to work for standard RAM tests but not the intensive
// randomAccess test that includes pauses to confirm data holds.
//
PERROR
CCrazyKongBaseGame::onAddressRemap(
    void   *cCrazyKongBaseGame,
    UINT32  addressIn,
    UINT32 *addressOut
)
{
    PERROR             error      = errorSuccess;
    CCrazyKongBaseGame *thisGame  = (CCrazyKongBaseGame *) cCrazyKongBaseGame;

    // Address remap
    if ((addressIn >= 0x9800) && (addressIn <= 0x9BFF))
    {
        *addressOut = 0x9800 +                     // Base address
                      (addressIn & 0x001F) +       // A0-A4,  lower 5 bits
                      ((addressIn & 0x03E0) << 1); // A6-A10, upper 5 bits
    }
    else
    {
        *addressOut = addressIn;
    }

    // VBLANK sync
    if ((addressIn >= 0x8800) && (addressIn <= 0x88FF) ||
        (addressIn >= 0x9800) && (addressIn <= 0x9BFF))
    {
        // Check if we're at risk from leaving VBLANK and reset if we are
        if (thisGame->m_vblankAccessCount == 0)
        {
            thisGame->m_vblankAccessCount = 4;

            thisGame->waitVblank();
        }
        else
        {
            thisGame->m_vblankAccessCount--;
        }
    }

    return error;
}

