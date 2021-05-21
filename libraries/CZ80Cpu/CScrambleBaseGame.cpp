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
#include "CZ80ACpu.h"
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
// Base addresses for the base Scramble main PCB hardware.
//
static const UINT32 s_scrambleProgRamBase     = 0x004000L;
static const UINT32 s_scrambleBkVRamBase      = 0x104800L;
static const UINT32 s_scrambleObjRamBase      = 0x005000L;
static const UINT32 s_scrambleWdResInputBase  = 0x007000L;
static const UINT32 s_scrambleCtrlOutputBase  = 0x006800L;
static const UINT32 s_scramble8255IoBase0     = 0x008100L;
static const UINT32 s_scramble8255IoBase1     = 0x008200L;

//
// Address offset for the Hustler Scramble conversion that maps
// to the "hustlerb4" configuration in MAME. Everything moves
// up 0x4000 to make more ROM space (that isn't actually used
// on Hustler but is on Super Cobra).
//
static const UINT32 s_hustlerScrambleBaseOffset = 0x4000L;


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                                                      "012", "012345"
                                                  {NO_BANK_SWITCH, s_scrambleProgRamBase + 0x000, s_scrambleProgRamBase + 0x3FF, 1, 0x0F, " 1K", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, s_scrambleProgRamBase + 0x000, s_scrambleProgRamBase + 0x3FF, 1, 0xF0, " 1G", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, s_scrambleProgRamBase + 0x400, s_scrambleProgRamBase + 0x7FF, 1, 0x0F, " 1J", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, s_scrambleProgRamBase + 0x400, s_scrambleProgRamBase + 0x7FF, 1, 0xF0, " 1H", "Prog. "}, // "Program RAM, 2114"
                                                  //
                                                  // See note above about access restrictions w.r.t HBLANK & WAIT.
                                                  // These regions are access with special support in the CZ80Cpu triggered via address 0x10xxxx.
                                                  //
                                                  //                                                                                      "012", "012345"
                                                  {NO_BANK_SWITCH, s_scrambleBkVRamBase  + 0x000, s_scrambleBkVRamBase  + 0x3FF, 1, 0x0F, " 3K", "BkVRam"}, // "Background VRAM, 2114"
                                                  {NO_BANK_SWITCH, s_scrambleBkVRamBase  + 0x000, s_scrambleBkVRamBase  + 0x3FF, 1, 0xF0, " 3J", "BkVRam"}, // "Background VRAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                                                      "012", "012345"
                                                          {NO_BANK_SWITCH, s_scrambleProgRamBase + 0x000, s_scrambleProgRamBase + 0x3FF, 1, 0xFF, "1KG", "Prog. "}, // "Program RAM, 2114, 1K/1G"
                                                          {NO_BANK_SWITCH, s_scrambleProgRamBase + 0x400, s_scrambleProgRamBase + 0x7FF, 1, 0xFF, "1JH", "Prog. "}, // "Program RAM, 2114, 1J/1H"
                                                          {NO_BANK_SWITCH, s_scrambleBkVRamBase  + 0x000, s_scrambleBkVRamBase  + 0x3FF, 1, 0xFF, "3KJ", "BkVRam"}, // "Background VRAM, 2114, 3K/3J"
                                                          {0}
                                                        }; // end of list

//
// RAM region is the same for all games on this board set.
// Unlike Galaxian, it appears that the object RAM cannot be read.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                                                                    "012", "012345"
                                                           {NO_BANK_SWITCH, s_scrambleObjRamBase + 0x000, s_scrambleObjRamBase + 0x0FF, 1, 0x0F, " 3L", "ObjRam"}, // "Object RAM, 2114, 256 Bytes used."
                                                           {NO_BANK_SWITCH, s_scrambleObjRamBase + 0x000, s_scrambleObjRamBase + 0x0FF, 1, 0xF0, " 3M", "ObjRam"}, // "Object RAM, 2114, 256 Bytes used."
                                                           {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                                                                    "012", "012345"
                                                      {NO_BANK_SWITCH,                             s_scrambleWdResInputBase + 0x000, 0xFF,  " 5C", "WD Res"}, // Watchdog reset
                                                      {CScrambleBaseGame::onBankSwitchSetup8255_0, s_scramble8255IoBase0    + 0x000, 0xFF,  "s1E", "Port A"}, // CP Inputs
                                                      {CScrambleBaseGame::onBankSwitchSetup8255_0, s_scramble8255IoBase0    + 0x001, 0xFF,  "s1E", "Port B"}, // CP Inputs
                                                      {CScrambleBaseGame::onBankSwitchSetup8255_0, s_scramble8255IoBase0    + 0x002, 0xFF,  "s1E", "Port C"}, // CP Inputs
                                                      {CScrambleBaseGame::onBankSwitchSetup8255_1, s_scramble8255IoBase1    + 0x002, 0xF0,  "s1E", "Prot R"}, // Protection Read
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                                                          "012", "012345"
                                                        {NO_BANK_SWITCH,                             s_scrambleCtrlOutputBase + 0x001, 0x01, 0x00,  " 5B", "NMI En"}, // NMI enable
                                                        {NO_BANK_SWITCH,                             s_scrambleCtrlOutputBase + 0x002, 0x01, 0x00,  " 5B", "Cntr  "}, // Coin counter
                                                        {NO_BANK_SWITCH,                             s_scrambleCtrlOutputBase + 0x003, 0x01, 0x00,  " 5B", "POUT1 "}, // Blue video drive
                                                        {NO_BANK_SWITCH,                             s_scrambleCtrlOutputBase + 0x004, 0x01, 0x00,  " 5B", "Stars "}, // Stars on
                                                        {NO_BANK_SWITCH,                             s_scrambleCtrlOutputBase + 0x005, 0x01, 0x00,  " 5B", "POUT2 "}, // unused
                                                        {NO_BANK_SWITCH,                             s_scrambleCtrlOutputBase + 0x006, 0x01, 0x00,  " 5B", "Flip X"}, // Flip X
                                                        {NO_BANK_SWITCH,                             s_scrambleCtrlOutputBase + 0x007, 0x01, 0x00,  " 5B", "Flip Y"}, // Flip Y
                                                        {CScrambleBaseGame::onBankSwitchSetup8255_1, s_scramble8255IoBase1    + 0x000, 0xFF, 0x00,  "s1D", "Snd Cm"}, // Sound command
                                                        {CScrambleBaseGame::onBankSwitchSetup8255_1, s_scramble8255IoBase1    + 0x001, 0x04, 0x00,  "s1D", "Snd In"}, // Sound interrupt
                                                        {CScrambleBaseGame::onBankSwitchSetup8255_1, s_scramble8255IoBase1    + 0x001, 0x10, 0x00,  "s1D", "Amp Mt"}, // Amp Mute
                                                        {CScrambleBaseGame::onBankSwitchSetup8255_1, s_scramble8255IoBase1    + 0x002, 0x0F, 0x00,  "s1D", "Prot W"}, // Protection Write
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                    "0123456789"
                                                            {CScrambleBaseGame::clearBk,          "Clear Bk  "},
                                                            {CScrambleBaseGame::clearObj,         "Clear Obj "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list

CScrambleBaseGame::CScrambleBaseGame(
    const Base base,
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : m_intMaskWriteAddress(s_scrambleCtrlOutputBase + 0x001),
    m_8255WriteBaseAddress0(s_scramble8255IoBase0),
    m_8255WriteBaseAddress1(s_scramble8255IoBase1),
    CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new CZ80ACpu();
    m_cpu->idle();

    // The VBLANK interrupt is on the NMI pin.
    m_interrupt = ICpu::NMI;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;

    // Dynamically create the various memory mappings
    switch(base)
    {
        // Simple move of everything up.
        case HUSTLER_SCRAMBLE :
        {
            m_intMaskWriteAddress   += s_hustlerScrambleBaseOffset;
            m_8255WriteBaseAddress0 += s_hustlerScrambleBaseOffset;
            m_8255WriteBaseAddress1 += s_hustlerScrambleBaseOffset;

            addAddressOffset(m_ramRegion,          s_hustlerScrambleBaseOffset);
            addAddressOffset(m_ramRegionByteOnly,  s_hustlerScrambleBaseOffset);
            addAddressOffset(m_ramRegionWriteOnly, s_hustlerScrambleBaseOffset);
            addAddressOffset(m_inputRegion,        s_hustlerScrambleBaseOffset);
            addAddressOffset(m_outputRegion,       s_hustlerScrambleBaseOffset);
        }

        case SCRAMBLE :
        default :
        {
            // Nothing more to do
        }
    }
}


CScrambleBaseGame::CScrambleBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion,
    const RAM_REGION *ramRegion,
    const RAM_REGION *ramRegionByteOnly,
    const RAM_REGION *ramRegionWriteOnly,
    const INPUT_REGION  *inputRegion,
    const OUTPUT_REGION *outputRegion,
    UINT32 intMaskWriteAddress,
    UINT32 i8255WriteBaseAddress0,
    UINT32 i8255WriteBaseAddress1
) : m_intMaskWriteAddress(intMaskWriteAddress),
    m_8255WriteBaseAddress0(i8255WriteBaseAddress0),
    m_8255WriteBaseAddress1(i8255WriteBaseAddress1),
    CGame( romData2n,
           romRegion,
           ramRegion,
           ramRegionByteOnly,
           ramRegionWriteOnly,
           inputRegion,
           outputRegion,
           s_customFunction)
{
    m_cpu = new CZ80ACpu();
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

    return cpu->memoryWrite(thisGame->m_8255WriteBaseAddress0 + 0x003, 0x9B);
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

    return cpu->memoryWrite(thisGame->m_8255WriteBaseAddress1 + 0x003, 0x88);
}


//
// Clear the video display to blank (background)
//
PERROR
CScrambleBaseGame::clearBk(
    void *context
)
{
    CScrambleBaseGame *thisGame = (CScrambleBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT32 count;

    //
    // The 0x080 is the address in the 5F/5H graphics ROMS
    // of the all-zero character on Scorpion.
    //
    static const UINT32 bkVRamStart  = s_scrambleBkVRamBase;
    static const UINT32 bkVRamLen    = 0x400;
    static const UINT8  bkVBlankChar = (0x080 >> 3); // 8x8

    //
    // Clear the background RAM to a blank character.
    //
    for (count = 0 ; count < bkVRamLen ; count++)
    {
        (void) cpu->memoryWrite(bkVRamStart + count, bkVBlankChar);
    }

    return error;
}


//
// Clear the video display to blank (sprites)
//
PERROR
CScrambleBaseGame::clearObj(
    void *context
)
{
    CScrambleBaseGame *thisGame = (CScrambleBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT32 count;

    //
    // The 0x080 is the address in the 5F/5H graphics ROMS
    // of the all-zero character on Scorpion.
    //
    static const UINT32 objRamStart  = s_scrambleObjRamBase;
    static const UINT32 objRamLen    = 0x100;
    static const UINT8  objBlankChar = (0x080 >> 5); // 16x16

    //
    // Clear the object RAM to a blank character.
    //
    for (count = 0 ; count < objRamLen ; count++)
    {
        (void) cpu->memoryWrite(objRamStart + count, objBlankChar);
    }

    return error;
}


