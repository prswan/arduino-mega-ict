//
// Copyright (c) 2015, Paul R. Swan
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
#include "CMoneyMoneyBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   Tie CPU board IC 5F (74LS00) pin 1 to GND.
//
// Board Designations:
//   'r' - ROM board.
//   'c' - CPU board.
//   'v' - Video board.
//   's' - Sound board.
//
// Z80 Compatibility Notes
//   * IO space is not used.
//   * M1 (1st machine cycle) is not used.
//   * Access to the background RAM via SABKG causes the CPU to WAIT when access occurs outside of HBLANK.
//     This gate is Z80 clock input synchronous. It *almost* works as is with the tester but it looks like
//     the HBLANK period is so short that it can shut off the VRAM before the tester has actually got
//     the data off the bus.
//
//     To work around this, in CZ80Cpu we synchronize with HBLANK by waiting for the WAIT to occur and clear
//     before letting the cycle go ahead. This ensure that the WAIT doesn't occur in the middle of the cycle.
//
// 8255 Notes
//   Port A (all) - Inputs.
//   Port B (all) - Inputs.
//   Port C (Lo ) - Inputs.
//   Port C (Hi ) - Outputs (DIP switch bank select)
//


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0x7000, 0x73FF, 1, 0x0F, "c2A", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x7000, 0x73FF, 1, 0xF0, "c2B", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x7400, 0x77FF, 1, 0x0F, "c2C", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x7400, 0x77FF, 1, 0xF0, "c2D", "Prog. "}, // "Program RAM, 2114"
                                                  //
                                                  // MAME appears to show the Obj. region as a bunch of different usages but the hardware is one RAM block.
                                                  //
                                                  {NO_BANK_SWITCH, 0x6800, 0x68FF, 1, 0x0F, "v1L", "Obj.  "}, // "Object RAM, 2114, 256 Bytes used."
                                                  {NO_BANK_SWITCH, 0x6800, 0x68FF, 1, 0xF0, "v1M", "Obj.  "}, // "Object RAM, 2114, 256 Bytes used."
                                                  //
                                                  // See note above about access restrictions w.r.t HBLANK & WAIT. It's a 12-bit block.
                                                  // These regions are access with special support in the CZ80Cpu triggered via address 0x10xxxx.
                                                  //
                                                  //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x106000, 0x1063FF, 1, 0x0F, "v2C", "Bkg.  "}, // "Background RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x106000, 0x1063FF, 1, 0xF0, "v2B", "Bkg.  "}, // "Background RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x106400, 0x1067FF, 1, 0x0F, "v2A", "Bkg.  "}, // "Background RAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x7000,   0x73FF,   1, 0xFF, "2AB", "Prog. "}, // "Program RAM, 2114, 2A/2B"
                                                          {NO_BANK_SWITCH, 0x7400,   0x77FF,   1, 0xFF, "2CD", "Prog. "}, // "Program RAM, 2114, 2C/2D"
                                                          {NO_BANK_SWITCH, 0x6800,   0x68FF,   1, 0xFF, "1LM", "Obj.  "}, // "Object RAM, 2114, 256 Bytes used, 1L/1M"
                                                          {NO_BANK_SWITCH, 0x106000, 0x1063FF, 1, 0xFF, "2CB", "Bkg.  "}, // "Background RAM, 2114, 2C/2B"
                                                          {0}
                                                        }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                                             "012", "012345"
                                                      {NO_BANK_SWITCH,                               0x6C00L, 0x03,  " 3H", "Cn 1&2"}, // Coin 1 & 2
                                                      {NO_BANK_SWITCH,                               0x6C00L, 0x04,  " 2I", "Cn 3  "}, // Coin 3
                                                      {NO_BANK_SWITCH,                               0x6C00L, 0x08,  " 2I", "HSOUND"}, // HSOUND
                                                      {CMoneyMoneyBaseGame::onBankSwitchSelectDSW5I, 0x6E00L, 0x0F,  " 3H", "DSW 5I"}, // Read DIP switches
                                                      {CMoneyMoneyBaseGame::onBankSwitchSelectDSW5I, 0x6E00L, 0xF0,  " 2I", "DSW 5I"}, // Read DIP switches
                                                      {CMoneyMoneyBaseGame::onBankSwitchSelectDSW4I, 0x6E00L, 0x0F,  " 3H", "DSW 4I"}, // Read DIP switches
                                                      {CMoneyMoneyBaseGame::onBankSwitchSelectDSW4I, 0x6E00L, 0xF0,  " 2I", "DSW 4I"}, // Read DIP switches
                                                      {CMoneyMoneyBaseGame::onBankSwitchSelectDSW3I, 0x6E00L, 0x0F,  " 3H", "DSW 3I"}, // Read DIP switches
                                                      {CMoneyMoneyBaseGame::onBankSwitchSelectDSW3I, 0x6E00L, 0xF0,  " 2I", "DSW 3I"}, // Read DIP switches
                                                      {CMoneyMoneyBaseGame::onBankSwitchSetup8255,   0x7800L, 0xFF,  " 4H", "Port A"}, // 8255 Port A - Inputs
                                                      {CMoneyMoneyBaseGame::onBankSwitchSetup8255,   0x7801L, 0xFF,  " 4H", "Port B"}, // 8255 Port B - Inputs
                                                      {CMoneyMoneyBaseGame::onBankSwitchSetup8255,   0x7802L, 0x0F,  " 4H", "Port C"}, // 8255 Port C - Inputs
                                                      {NO_BANK_SWITCH,                               0x7C00L, 0xFF,  " 5F", "WD Res"}, // Watchdog reset
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x6E00L, 0xFF, 0x00,  "c2H", "Snd FF"}, // Sound FF
                                                        {NO_BANK_SWITCH, 0x6E00L, 0x01, 0x00,  "c2H", "Snd 01"}, // Sound 01
                                                        {NO_BANK_SWITCH, 0x6E00L, 0x02, 0x00,  "c2H", "Snd 02"}, // Sound 02
                                                        {NO_BANK_SWITCH, 0x6E00L, 0x04, 0x00,  "c2H", "Snd 04"}, // Sound 04
                                                        {NO_BANK_SWITCH, 0x6E00L, 0x08, 0x00,  "c2H", "Snd 08"}, // Sound 08
                                                        {NO_BANK_SWITCH, 0x6E00L, 0x10, 0x00,  "c2H", "Snd 10"}, // Sound 10
                                                        {NO_BANK_SWITCH, 0x6E00L, 0x20, 0x00,  "c2H", "Snd 20"}, // Sound 20
                                                        {NO_BANK_SWITCH, 0x6E00L, 0x40, 0x00,  "c2H", "Snd 40"}, // Sound 40
                                                        {NO_BANK_SWITCH, 0x6E00L, 0x80, 0x00,  "c2H", "Snd 80"}, // Sound 80
                                                        {NO_BANK_SWITCH, 0x6C00L, 0x01, 0x00,  "c3G", "X-Flip"}, // X flip screen
                                                        {NO_BANK_SWITCH, 0x6C01L, 0x01, 0x00,  "c3G", "Y-Flip"}, // Y flip screen
                                                        {NO_BANK_SWITCH, 0x6C02L, 0x01, 0x00,  "c3G", "SndRst"}, // Reset sound
                                                        {NO_BANK_SWITCH, 0x6C06L, 0x01, 0x00,  "c3G", "Cntr  "}, // Coin counter
                                                        {NO_BANK_SWITCH, 0x6C07L, 0x01, 0x00,  "c3G", "IntEn "}, // Int (NMI) enable & reset
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CMoneyMoneyBaseGame::CMoneyMoneyBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    // Enable the VRAM WAIT bus check on this platform for the background RAM.
    m_cpu = new CZ80ACpu(0x106000);
    m_cpu->idle();

    // The VBLANK interrupt is on the NMI pin.
    m_interrupt = ICpu::NMI;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;
}


CMoneyMoneyBaseGame::~CMoneyMoneyBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// This is a specific implementation for Money Money that uses an externally
// maskable/resetable latch for the VBLANK interrupt on the NMI line.
PERROR
CMoneyMoneyBaseGame::interruptCheck(
)
{
    PERROR error = errorSuccess;
    UINT8 response = 0;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "OK:";

    for (int i = 0 ; i < 4 ; i++)
    {
        // Unmask the interrupt.
        m_cpu->memoryWrite(0x6C07L, 0x01);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        1000);
        if (FAILED(error))
        {
            break;
        }

        // Mask the interrupt (also resets the latch)
        m_cpu->memoryWrite(0x6C07L, 0x00);

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
        m_cpu->memoryWrite(0x6C07L, 0x01);

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
        m_cpu->memoryWrite(0x6C07L, 0x00);

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
// This is used to setup the 8255 to reflect the hardware configuration:
//   - Port A (All) Mode 0 Input
//   - Port B (All) Mode 0 Input
//   - Port C (Lo)  Mode 0 Input
//   - Port C (Hi)  Mode 0 Output
//
//     == 1 0010 011 == 0x93
//
// We also need to set the initial state of the Port C outputs to idle (Hi)
// using the bit Set\Reset command.
//   - PC4 100 1 == 0x09
//   - PC5 101 1 == 0x0B
//   - PC6 110 1 == 0x0D
//
PERROR
CMoneyMoneyBaseGame::onBankSwitchSetup8255(
    void *cMoneyMoneyBaseGame
)
{
    PERROR              error     = errorSuccess;
    CMoneyMoneyBaseGame *thisGame  = (CMoneyMoneyBaseGame *) cMoneyMoneyBaseGame;
    ICpu                *cpu       = thisGame->m_cpu;

    error = cpu->memoryWrite(0x7803, 0x93);

    if (SUCCESS(error))
    {
        error = cpu->memoryWrite(0x7803, 0x09);
    }

    if (SUCCESS(error))
    {
        error = cpu->memoryWrite(0x7803, 0x0B);
    }

    if (SUCCESS(error))
    {
        error = cpu->memoryWrite(0x7803, 0x0D);
    }

    return error;
}


//
// This is used to set the port C upper pins to select DSW 5I
// Because it's a split port, the bit Set/Reset instruction must
// be used.
//   0000 1000 = 0x08
//
PERROR
CMoneyMoneyBaseGame::onBankSwitchSelectDSW5I(
    void *cMoneyMoneyBaseGame
)
{
    PERROR              error     = errorSuccess;
    CMoneyMoneyBaseGame *thisGame  = (CMoneyMoneyBaseGame *) cMoneyMoneyBaseGame;
    ICpu                *cpu       = thisGame->m_cpu;

    error = onBankSwitchSetup8255(cMoneyMoneyBaseGame);

    if (SUCCESS(error))
    {
        error = cpu->memoryWrite(0x7803, 0x08);
    }

    return error;
}


//
// This is used to set the port C upper pins to select DSW 4I
// Because it's a split port, the bit Set/Reset instruction must
// be used.
//   0000 1010 = 0x0A
//
PERROR
CMoneyMoneyBaseGame::onBankSwitchSelectDSW4I(
    void *cMoneyMoneyBaseGame
)
{
    PERROR              error     = errorSuccess;
    CMoneyMoneyBaseGame *thisGame  = (CMoneyMoneyBaseGame *) cMoneyMoneyBaseGame;
    ICpu                *cpu       = thisGame->m_cpu;

    error = onBankSwitchSetup8255(cMoneyMoneyBaseGame);

    if (SUCCESS(error))
    {
        error = cpu->memoryWrite(0x7803, 0x0A);
    }

    return error;
}


//
// This is used to set the port C upper pins to select DSW 3I
// Because it's a split port, the bit Set/Reset instruction must
// be used.
//   0000 1100 = 0x0C
//
PERROR
CMoneyMoneyBaseGame::onBankSwitchSelectDSW3I(
    void *cMoneyMoneyBaseGame
)
{
    PERROR              error     = errorSuccess;
    CMoneyMoneyBaseGame *thisGame  = (CMoneyMoneyBaseGame *) cMoneyMoneyBaseGame;
    ICpu                *cpu       = thisGame->m_cpu;

    error = onBankSwitchSetup8255(cMoneyMoneyBaseGame);

    if (SUCCESS(error))
    {
        error = cpu->memoryWrite(0x7803, 0x0C);
    }

    return error;
}

