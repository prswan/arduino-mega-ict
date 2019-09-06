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
#include "CMegaZoneBaseGame.h"
#include "C6809EClockMasterCpu.h"
#include "CRamCheck.h"

//
// Probe Head GND:
//   Konmai-1 GND is pin 20 (actually TSC but so far always GND)
//
// Watchdog Disable:
//   CPU (top) PCB J1, IC DE13 (LS00) pin 12
//
// CPU Compatibility Notes
//
//
// Shield Configuration
// ---------------------
//  This was run on the 470-Ohm shield.
//

static const UINT32 s_intMaskA    = 0x0007L;
static const UINT16 s_intMaskD    = 0x01;

//
// It takes a while to make it through to VBLANK.
// Measured wall clock Hi = ~12s, Lo = ~2s
//
static const int s_intWaitInMs = 20000; // 20s


//
// RAM region is the same for all games on this board set.
//
// Notes
// -----
// - There is another pair of 2114 RAM at E14 & E15 that appears to be burried
//   video buffers likely written during blanking.
// - The designations in MAME for the "vram" 1 & 2 sections don't match the PCB
//   schematics that are used here.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x002000, 0x0027FF, 1, 0xFF, "F8 ", "VRam 1"}, // Vid PCB, 2128
                                                  {NO_BANK_SWITCH, 0x002800, 0x002FFF, 1, 0xFF, "F10", "VRam 2"}, // Vid PCB, 2128
                                                  {NO_BANK_SWITCH, 0x003000, 0x0033FF, 1, 0x0F, "F14", "Sprite"}, // Vid PCB, 2114
                                                  {NO_BANK_SWITCH, 0x003000, 0x0033FF, 1, 0xF0, "F15", "Sprite"}, // Vid PCB, 2114
                                                  {NO_BANK_SWITCH, 0x003800, 0x003FFF, 1, 0xFF, "G8 ", "Prog.S"}, // CPU PCB - 2128 Program RAM (shared with Z80)
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
// This description is used for the byte-wide intensive random access memory test.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x002000, 0x0027FF, 1, 0xFF, "F8 ", "VRam 1"}, // Vid PCB, 2128
                                                          {NO_BANK_SWITCH, 0x002800, 0x002FFF, 1, 0xFF, "F10", "VRam 2"}, // Vid PCB, 2128
                                                          {NO_BANK_SWITCH, 0x003000, 0x0033FF, 1, 0xFF, "F1x", "Sprite"}, // Vid PCB, 2114 x 2
                                                          {NO_BANK_SWITCH, 0x003800, 0x003FFF, 1, 0xFF, "G8 ", "Prog.S"}, // CPU PCB - 2128 Program RAM (shared with Z80)
                                                          {0}
                                                        }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
// The inputs are handled by the sound CPU via the shared RAM.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                              "012", "012345"
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                             "012", "012345"
                                                        {NO_BANK_SWITCH, 0x0000L,    0x01,       0x00, "A13", "CCntr2"}, // Coin Counter 2
                                                        {NO_BANK_SWITCH, 0x0001L,    0x01,       0x00, "A13", "CCntr1"}, // Coin Counter 1
                                                        {NO_BANK_SWITCH, 0x0003L,    0x01,       0x00, "A13", "Int   "},
                                                        {NO_BANK_SWITCH, 0x0005L,    0x01,       0x00, "A13", "ScFlip"}, // Screen Flip
                                                        {NO_BANK_SWITCH, s_intMaskA, s_intMaskD, 0x00, "A13", "IntMsk"}, // Interrupt Mask
                                                        {NO_BANK_SWITCH, 0x0800L,    0x01,       0x00, "B13", "W. Dog"}, // Watchdog Reset
                                                        {NO_BANK_SWITCH, 0x1000L,    0xFF,       0x00, "B13", "ScrolY"}, // Scroll Y
                                                        {NO_BANK_SWITCH, 0x1800L,    0xFF,       0x00, "B13", "ScrolX"}, // Scroll X
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                         "0123456789"
                                                            {CMegaZoneBaseGame::runClock1m,            "Run Clk 1m"},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list



CMegaZoneBaseGame::CMegaZoneBaseGame(
    const ROM_REGION *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction,
           CMegaZoneBaseGame::delayFunction)
{
    m_cpu = new C6809EClockMasterCpu(&s_Konami1PinOut);
    m_cpu->idle();

    m_interrupt = ICpu::IRQ1; // IRQ
    m_interruptAutoVector = true;
}


CMegaZoneBaseGame::~CMegaZoneBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// This is a specific implementation that uses an externally
// maskable/resetable latch for the VBLANK interrupt on
// Konami-1 pin 24 IRQ line (maps to IRQ1 on the ICT).
//
PERROR
CMegaZoneBaseGame::interruptCheck(
)
{
    PERROR error = errorSuccess;

    for (int i = 0 ; i < 2 ; i++)
    {
        // Unmask the interrupt.
        m_cpu->memoryWrite(s_intMaskA, s_intMaskD);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        s_intWaitInMs);
        if (FAILED(error))
        {
            break;
        }

        // Mask the interrupt (also resets the latch)
        m_cpu->memoryWrite(s_intMaskA, 0x00);

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
        m_cpu->memoryWrite(s_intMaskA, s_intMaskD);

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
    }

    return error;
}


// Note that this takes an ICpu as context
static PERROR CMegaZoneBaseGame::delayFunction(
    void *context,
    unsigned long ms
)
{
    C6809EClockMasterCpu *cpu = (C6809EClockMasterCpu *) context;
    PERROR error = errorSuccess;
    UINT16 data;

    unsigned long startTime = millis();
    unsigned long endTime   = startTime + ms;

    // Dummy read to run the clock
    while (millis() < endTime)
    {
        error = cpu->memoryRead(0xFFFF, &data);
    }

    return error;
}


// Run the system clock for 1 minute
PERROR
CMegaZoneBaseGame::runClock1m(
    void *cMegaZoneBaseGame
)
{
    CMegaZoneBaseGame *thisGame  = (CMegaZoneBaseGame *) cMegaZoneBaseGame;
    ICpu                 *cpu       = (ICpu *) thisGame->m_cpu;

    return delayFunction(cpu, 60 * 1000UL);
}

