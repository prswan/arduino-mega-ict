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
#include "CTimePilotBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   TBD.
//
// Board Designations:
//   This is the CPU/main board.
//
// Z80 Compatibility Notes
//   * IO space is not used.
//   * M1 (1st machine cycle) is not used.
//
// Notes:
//   The memory map is taken from MAME (src/mame/konami/timeplt.cpp,
//   timeplt_state::main_map). MAME doesn't document the RAM IC locations
//   so the RAM regions are tested byte wide with no IC location given ("-").
//   The only location MAME gives is the LS259 output latch at B3.
//
//   Read      Write
//   C000      C000     Scanline counter / sound command (mirror 0x0CFF)
//   C200      C200     DSW2 (SW2) / watchdog reset (mirror 0x0CFF)
//   C300               IN0 (system)
//   C320               IN1 (player 1)
//   C340               IN2 (player 2)
//   C360               DSW1 (SW1, coinage)
//             C300-C30F LS259 B3, D0 is the data and A1-A3 select the output.
//
// void timeplt_state::main_map(address_map &map)
//

//
// Base addresses for the Time Pilot main PCB hardware.
//
static const UINT32 s_TimePilotColRamBase     = 0x00A000L;
static const UINT32 s_TimePilotVidRamBase     = 0x00A400L;
static const UINT32 s_TimePilotProgRamBase    = 0x00A800L;
static const UINT32 s_TimePilotObjRam1Base    = 0x00B000L;
static const UINT32 s_TimePilotObjRam2Base    = 0x00B400L;
static const UINT32 s_TimePilotLatchBase      = 0x00C300L;


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                                                      "012", "012345"
                                                  {NO_BANK_SWITCH, s_TimePilotColRamBase  + 0x000, s_TimePilotColRamBase  + 0x3FF, 1, 0xFF, " - ", "ColRam"}, // Colour RAM
                                                  {NO_BANK_SWITCH, s_TimePilotVidRamBase  + 0x000, s_TimePilotVidRamBase  + 0x3FF, 1, 0xFF, " - ", "VidRam"}, // Video RAM
                                                  {NO_BANK_SWITCH, s_TimePilotProgRamBase + 0x000, s_TimePilotProgRamBase + 0x7FF, 1, 0xFF, " - ", "Prog. "}, // Program RAM
                                                  {NO_BANK_SWITCH, s_TimePilotObjRam1Base + 0x000, s_TimePilotObjRam1Base + 0x0FF, 1, 0xFF, " - ", "ObjRm1"}, // Sprite RAM 1
                                                  {NO_BANK_SWITCH, s_TimePilotObjRam2Base + 0x000, s_TimePilotObjRam2Base + 0x0FF, 1, 0xFF, " - ", "ObjRm2"}, // Sprite RAM 2
                                                  {0}
                                                }; // end of list

//
// No write-only RAM on this platform.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                               "012", "012345"
                                                      {NO_BANK_SWITCH, 0x00C000L, 0xFF, " - ", "VScan "}, // Scanline counter
                                                      {NO_BANK_SWITCH, 0x00C200L, 0xFF, " - ", "DSW2  "}, // SW2
                                                      {NO_BANK_SWITCH, 0x00C300L, 0xFF, " - ", "IN0   "}, // System (coin, start, service)
                                                      {NO_BANK_SWITCH, 0x00C320L, 0xFF, " - ", "IN1   "}, // Player 1
                                                      {NO_BANK_SWITCH, 0x00C340L, 0xFF, " - ", "IN2   "}, // Player 2 (cocktail)
                                                      {NO_BANK_SWITCH, 0x00C360L, 0xFF, " - ", "DSW1  "}, // SW1
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                      "012", "012345"
                                                        {NO_BANK_SWITCH, 0x00C000L,                    0xFF, 0x00, " - ", "SndCmd"}, // Sound command latch
                                                        {NO_BANK_SWITCH, 0x00C200L,                    0xFF, 0x00, " - ", "WD Res"}, // Watchdog reset
                                                        {NO_BANK_SWITCH, s_TimePilotLatchBase + 0x000, 0x01, 0x00, " B3", "NMI En"}, // Q0 NMI enable
                                                        {NO_BANK_SWITCH, s_TimePilotLatchBase + 0x002, 0x01, 0x00, " B3", "Flip  "}, // Q1 Flip screen
                                                        {NO_BANK_SWITCH, s_TimePilotLatchBase + 0x004, 0x01, 0x00, " B3", "SndIrq"}, // Q2 Sound CPU IRQ trigger (0->1)
                                                        {NO_BANK_SWITCH, s_TimePilotLatchBase + 0x006, 0x01, 0x00, " B3", "Mute  "}, // Q3 Amp mute
                                                        {NO_BANK_SWITCH, s_TimePilotLatchBase + 0x008, 0x01, 0x00, " B3", "VidEn "}, // Q4 Video enable
                                                        {NO_BANK_SWITCH, s_TimePilotLatchBase + 0x00A, 0x01, 0x00, " B3", "Coin 1"}, // Q5 Coin counter 1
                                                        {NO_BANK_SWITCH, s_TimePilotLatchBase + 0x00C, 0x01, 0x00, " B3", "Coin 2"}, // Q6 Coin counter 2
                                                        {NO_BANK_SWITCH, s_TimePilotLatchBase + 0x00E, 0x01, 0x00, " B3", "PayOut"}, // Q7 Pay out (unused)
                                                        {0}
                                                      }; // end of list

//
// No custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CTimePilotBaseGame::CTimePilotBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction),
    m_intMaskWriteAddress(s_TimePilotLatchBase + 0x000)
{
    m_cpu = new CZ80ACpu();
    m_cpu->idle();

    // The VBLANK interrupt is on the NMI pin.
    m_interrupt = ICpu::NMI;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;
}


CTimePilotBaseGame::~CTimePilotBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// The VBLANK NMI is held until it is cleared by disabling it via
// LS259 B3 Q0, the same scheme as Frogger.
//
PERROR
CTimePilotBaseGame::interruptCheck(
)
{
    PERROR error = errorSuccess;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "OK:";

    for (int i = 0 ; i < 4 ; i++)
    {
        // Unmask the interrupt.
        m_cpu->memoryWrite(m_intMaskWriteAddress, 0x01);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        1000);
        if (FAILED(error))
        {
            break;
        }

        // Mask the interrupt (also resets the latch)
        m_cpu->memoryWrite(m_intMaskWriteAddress, 0x00);

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
        m_cpu->memoryWrite(m_intMaskWriteAddress, 0x01);

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
        m_cpu->memoryWrite(m_intMaskWriteAddress, 0x00);

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

