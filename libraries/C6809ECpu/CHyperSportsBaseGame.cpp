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
#include "CHyperSportsBaseGame.h"
#include "C6809EClockMasterCpu.h"
#include "CRamCheck.h"

//
// Probe Head GND:
//   Konmai-1 GND is pin 20 (actually TSC but so far always GND)
//
// Watchdog Disable:
//   CPU (top) PCB IC A2 (LS08) pin 4
//
// CPU Compatibility Notes
//   According to MAME CPU frequency is 1.4MHz giving a half cycle time of 357nS
//   (5 Arduino instructions per half cycle)
//


static const UINT32 s_intMaskA    = 0x1487L;
static const UINT16 s_intMaskD    = 0x01;

//
// It takes a while to make it through to VBLANK.
// Measured wall clock Hi = ~12s, Lo = ~2s
//
static const int s_intWaitInMs = 20000; // 20s


//
// RAM region is the same for all games on this board set.
//
// Background RAM G9/G10 Note
// --------------------------
// Using standard 6809E data latching on E rising edge these return 0xFF on read.
// On the scope this appears to be because the gate G on pin 19 of E10 (LS245)
// is disabled before E rises due to the gating with clock 2H on E11 (LS32) pin 10.
// Comparing with the 18Mhz system clock, the gate of G is 1 clock pulse before
// the rise of E.
//
// Further, ~V1CS and ~V2CS are specially delayed by the RC circuit on the
// input pins 1 & 2 of A2 (LS08) on the CPU PCB suggesting there was a tweak
// needed on the RAM timing, possibly to push CS into the 2H window.
//
// To accomodate this timing violation we use an address flag that cause
// C6809EClockMaster to read the data 1 clock before E rises.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x001000, 0x0013FF, 1, 0x0F, "JH8", "Object"}, // Vid PCB - 2114 Object RAM
                                                  {NO_BANK_SWITCH, 0x001000, 0x0013FF, 1, 0xF0, "JH9", "Object"}, // Vid PCB - 2114 Object RAM
                                                  {NO_BANK_SWITCH, 0x102000, 0x1027FF, 1, 0xFF, "G10", "Bckgnd"}, // Vid PCB - 2128 Background RAM
                                                  {NO_BANK_SWITCH, 0x102800, 0x102FFF, 1, 0xFF, "G9 ", "Colour"}, // Vid PCB - 2128 Colour RAM
                                                  {NO_BANK_SWITCH, 0x003000, 0x0037FF, 1, 0xFF, "G2 ", "Prog. "}, // CPU PCB - 2128 Program RAM
                                                  {NO_BANK_SWITCH, 0x003800, 0x003FFF, 1, 0xFF, "G4 ", "Nv Ram"}, // CPU PCB - 6116 Battery backed RAM
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
// This description is used for the byte-wide intensive random access memory test.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                        "012", "012345"
                                                          {NO_BANK_SWITCH, 0x001000, 0x0013FF, 1, 0xFF, "JH ", "Object"}, // Vid PCB - 2114 Object RAM
                                                          {NO_BANK_SWITCH, 0x102000, 0x1027FF, 1, 0xFF, "G10", "Bckgnd"}, // Vid PCB - 2128 Background RAM
                                                          {NO_BANK_SWITCH, 0x102800, 0x102FFF, 1, 0xFF, "G9 ", "Colour"}, // Vid PCB - 2128 Colour RAM
                                                          {NO_BANK_SWITCH, 0x003000, 0x0037FF, 1, 0xFF, "G2 ", "Prog. "}, // CPU PCB - 2128 Program RAM
                                                          {NO_BANK_SWITCH, 0x003800, 0x003FFF, 1, 0xFF, "G4 ", "Nv Ram"}, // CPU PCB - 6116 Battery backed RAM
                                                          {0}
                                                        }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                              "012", "012345"
                                                      {NO_BANK_SWITCH, 0x1600L, 0xFF, "   ", "DSW2  "}, // DIP Switches 2
                                                      {NO_BANK_SWITCH, 0x1680L, 0xFF, "   ", "System"}, // System
                                                      {NO_BANK_SWITCH, 0x1681L, 0xFF, "   ", "P1_P2 "}, // Control Inputs
                                                      {NO_BANK_SWITCH, 0x1682L, 0xFF, "   ", "P3_P4 "}, // Conreol Inputs
                                                      {NO_BANK_SWITCH, 0x1683L, 0xFF, "   ", "DSW1  "}, // DIP Switches 1
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                             "012", "012345"
                                                        {NO_BANK_SWITCH, 0x1400L,    0x01,       0x00, "   ", "W. Dog"}, // Watchdog Reset
                                                        {NO_BANK_SWITCH, 0x1480L,    0x01,       0x00, "F16", "ScFlip"}, // Screen Flip
                                                        {NO_BANK_SWITCH, 0x1481L,    0x01,       0x00, "F16", "SndInt"}, // Sound Interrupt
                                                        {NO_BANK_SWITCH, 0x1482L,    0x01,       0x00, "F16", "End   "}, // ?
                                                        {NO_BANK_SWITCH, 0x1483L,    0x01,       0x00, "F16", "CCntr1"}, // Coin Counter 1
                                                        {NO_BANK_SWITCH, 0x1484L,    0x01,       0x00, "F16", "CCntr2"}, // Coin Counter 2
                                                        {NO_BANK_SWITCH, 0x1485L,    0x01,       0x00, "F16", "SA    "}, // ?
                                                        {NO_BANK_SWITCH, s_intMaskA, s_intMaskD, 0x00, "F16", "IntMsk"}, // Interrupt Mask
                                                        {NO_BANK_SWITCH, 0x1500L,    0xFF,       0x00, "   ", "SndDat"}, // Sound Data
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                            "0123456789"
                                                            {CHyperSportsBaseGame::bankSwitchObjectRamJH, "JH Bank Sw"},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list



CHyperSportsBaseGame::CHyperSportsBaseGame(
    const ROM_REGION *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction,
           CHyperSportsBaseGame::delayFunction)
{
    m_cpu = new C6809EClockMasterCpu(&s_Konami1PinOut);
    m_cpu->idle();

    m_interrupt = ICpu::IRQ1; // IRQ
    m_interruptAutoVector = true;
}


CHyperSportsBaseGame::~CHyperSportsBaseGame(
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
CHyperSportsBaseGame::interruptCheck(
)
{
    PERROR error = errorSuccess;

    //
    // Since each pass flips the object RAM bank an odd
    // count here always results in the bank flipped after
    // the test. This allows the Soak Test to test both
    // object RAM banks.
    //
    for (int i = 0 ; i < 3 ; i++)
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
static PERROR CHyperSportsBaseGame::delayFunction(
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


//
// There are two banks of object RAM J8,J9 and H8,H9. At any given time, one
// bank is facing the CPU and the other active display. The banks are swapped on
// each rising edge of INTST (CK of latch H2) that is the active low IRQ input
// (in other words, when the CPU clears the interrupt by presetting latch H2).
//
// As far as I can tell there is no programnatic way to determine which bank is
// currently selected - the latch H2 will come up after power on in random state.
//
// On failure, check video PCB IC H2 pin 2 (same as ~OBJSEL):
//
//  Lo: J8, J9 = CPU
//  Hi: H8, H9 = CPU
//
// The custom function here flips the bank over for testing.
//
PERROR
CHyperSportsBaseGame::bankSwitchObjectRamJH(
    void *cHyperSportsBaseGame
)
{
    PERROR               error      = errorSuccess;
    CHyperSportsBaseGame *thisGame  = (CHyperSportsBaseGame *) cHyperSportsBaseGame;
    ICpu                 *cpu       = (ICpu *) thisGame->m_cpu;
    ICpu::Interrupt       interrupt = thisGame->m_interrupt;

    // Unmask the interrupt.
    cpu->memoryWrite(s_intMaskA, s_intMaskD);

    // Wait for the VBLANK interrupt
    error = cpu->waitForInterrupt(interrupt,
                                  true,
                                  s_intWaitInMs);
    if (FAILED(error))
    {
        return error;
    }

    // Reset & mask the interrupt, flipping banks
    cpu->memoryWrite(s_intMaskA, 0x00);

    // Interrupt should have been cleared
    error = cpu->waitForInterrupt(interrupt,
                                  true,
                                  0);
    if (SUCCESS(error))
    {
        error = errorUnexpected;
    }
    else
    {
        error = errorSuccess;
    }

    return error;
}

