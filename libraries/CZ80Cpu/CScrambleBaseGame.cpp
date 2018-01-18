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
#include "CScrambleBaseGame.h"
#include "CZ80Cpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   Tie CPU board IC 5C (74LS00) pin 13 to GND.
//
// Board Designations:
//   This is the CPU/main board base.
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
// Note: In MAME the hardware for Scramble is mapped to "theend_map".
//

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0x4000, 0x43FF, 1, 0x0F, " 1K", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x4000, 0x43FF, 1, 0xF0, " 1G", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x4400, 0x47FF, 1, 0x0F, " 1J", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x4400, 0x47FF, 1, 0xF0, " 1H", "Prog. "}, // "Program RAM, 2114"
                                                  //
                                                  // See note above about access restrictions w.r.t HBLANK & WAIT.
                                                  // These regions are access with special support in the CZ80Cpu triggered via address 0x10xxxx.
                                                  //
                                                  //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x104800, 0x104BFF, 1, 0x0F, " 3K", "BkVRam"}, // "Background VRAM, 2114"
                                                  {NO_BANK_SWITCH, 0x104800, 0x104BFF, 1, 0xF0, " 3J", "BkVRam"}, // "Background VRAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH,   0x4000,   0x43FF,   1, 0xFF, "1KG", "Prog. "}, // "Program RAM, 2114, 1K/1G"
                                                          {NO_BANK_SWITCH,   0x4400,   0x47FF,   1, 0xFF, "1JH", "Prog. "}, // "Program RAM, 2114, 1J/1H"
                                                          {NO_BANK_SWITCH, 0x104800, 0x104BFF, 1, 0xFF, "3KJ", "BkVRam"}, // "Background VRAM, 2114, 3K/3J"
                                                          {0}
                                                        }; // end of list

//
// RAM region is the same for all games on this board set.
// Unlike Galaxian, it appears that the object RAM cannot be read.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                            "012", "012345"
                                                           {NO_BANK_SWITCH, 0x5000, 0x50FF, 1, 0x0F, " 3L", "ObjRam"}, // "Object RAM, 2114, 256 Bytes used."
                                                           {NO_BANK_SWITCH, 0x5000, 0x50FF, 1, 0xF0, " 3M", "ObjRam"}, // "Object RAM, 2114, 256 Bytes used."
                                                           {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                                           "012", "012345"
                                                      {NO_BANK_SWITCH,                             0x7000L, 0xFF,  " 5C", "WD Res"}, // Watchdog reset
                                                      {CScrambleBaseGame::onBankSwitchSetup8255_0, 0x8100L, 0xFF,  "s1E", "Port A"}, // CP Inputs
                                                      {CScrambleBaseGame::onBankSwitchSetup8255_0, 0x8101L, 0xFF,  "s1E", "Port B"}, // CP Inputs
                                                      {CScrambleBaseGame::onBankSwitchSetup8255_0, 0x8102L, 0xFF,  "s1E", "Port C"}, // CP Inputs
                                                      {CScrambleBaseGame::onBankSwitchSetup8255_1, 0x8202L, 0xF0,  "s1E", "Prot R"}, // Protection Read
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                                 "012", "012345"
                                                        {NO_BANK_SWITCH,                             0x6801L, 0x01, 0x00,  " 5B", "NMI En"}, // NMI enable
                                                        {NO_BANK_SWITCH,                             0x6802L, 0x01, 0x00,  " 5B", "Cntr  "}, // Coin counter
                                                        {NO_BANK_SWITCH,                             0x6803L, 0x01, 0x00,  " 5B", "POUT1 "}, // Blue video drive
                                                        {NO_BANK_SWITCH,                             0x6804L, 0x01, 0x00,  " 5B", "Stars "}, // Stars on
                                                        {NO_BANK_SWITCH,                             0x6805L, 0x01, 0x00,  " 5B", "POUT2 "}, // unused
                                                        {NO_BANK_SWITCH,                             0x6806L, 0x01, 0x00,  " 5B", "Flip X"}, // Flip X
                                                        {NO_BANK_SWITCH,                             0x6807L, 0x01, 0x00,  " 5B", "Flip Y"}, // Flip Y
                                                        {CScrambleBaseGame::onBankSwitchSetup8255_1, 0x8200L, 0xFF, 0x00,  "s1D", "Snd Cm"}, // Sound command
                                                        {CScrambleBaseGame::onBankSwitchSetup8255_1, 0x8201L, 0x04, 0x00,  "s1D", "Snd In"}, // Sound interrupt
                                                        {CScrambleBaseGame::onBankSwitchSetup8255_1, 0x8201L, 0x10, 0x00,  "s1D", "Amp Mt"}, // Amp Mute
                                                        {CScrambleBaseGame::onBankSwitchSetup8255_1, 0x8202L, 0x0F, 0x00,  "s1D", "Prot W"}, // Protection Write
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CScrambleBaseGame::CScrambleBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new CZ80Cpu();
    m_cpu->idle();

    // The VBLANK interrupt is on the NMI pin.
    m_interrupt = ICpu::NMI;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;
}


CScrambleBaseGame::~CScrambleBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// This is a specific implementation for Scramble that uses an externally
// maskable/resetable latch for the VBLANK interrupt on the NMI line.
PERROR
CScrambleBaseGame::interruptCheck(
)
{
    PERROR error = errorSuccess;
    UINT8 response = 0;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "OK:";

    for (int i = 0 ; i < 4 ; i++)
    {
        // Unmask the interrupt.
        m_cpu->memoryWrite(0x6801L, 0x01);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        1000);
        if (FAILED(error))
        {
            break;
        }

        // Mask the interrupt (also resets the latch)
        m_cpu->memoryWrite(0x6801L, 0x00);

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
        m_cpu->memoryWrite(0x6801L, 0x01);

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
        m_cpu->memoryWrite(0x6801L, 0x00);

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
//
//  PPI0 (A15 & A8) = 0x8100 - Control inputs
//   - Port A (All) Mode 0 Input
//   - Port B (All) Mode 0 Input
//   - Port C (Lo)  Mode 0 Input
//   - Port C (Hi)  Mode 0 Input
//
//     == 1 0011 011 == 0x9B
//
PERROR
CScrambleBaseGame::onBankSwitchSetup8255_0(
    void *cScrambleBaseGame
)
{
    CScrambleBaseGame *thisGame  = (CScrambleBaseGame *) cScrambleBaseGame;
    ICpu              *cpu       = thisGame->m_cpu;

    return cpu->memoryWrite(0x8103, 0x9B);
}


//
// This is used to setup the 8255 to reflect the hardware configuration:
//
//  PPI1 (A15 & A9) = 0x8200 - Sound latch & protection.
//   - Port A (All) Mode 0 Output (Sound Command)
//   - Port B (All) Mode 0 Output (PB3 - Sound Interrupt, PB4 - Amp Mute)
//   - Port C (Lo)  Mode 0 Output (Protection Write)
//   - Port C (Hi)  Mode 0 Input  (Protection Read)
//
//     == 1 0001 000 == 0x88
//
// Protection
// Scramble main set always writes sequences of 3 or more nibbles to the low port
// and expects certain results in the upper nibble afterwards.
//
PERROR
CScrambleBaseGame::onBankSwitchSetup8255_1(
    void *cScrambleBaseGame
)
{
    CScrambleBaseGame *thisGame  = (CScrambleBaseGame *) cScrambleBaseGame;
    ICpu              *cpu       = thisGame->m_cpu;

    return cpu->memoryWrite(0x8203, 0x88);
}

