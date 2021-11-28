//
// Copyright (c) 2021, Paul R. Swan
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
#include "CFroggerBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   Tie CPU board IC 67 (74LS00) pin 1 to GND.
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
// void galaxian_state::frogger_map(address_map &map)
//

//
// Base addresses for the base Frogger main PCB hardware.
//
static const UINT32 s_FroggerProgRamBase     = 0x008000L;
static const UINT32 s_FroggerBkVRamBase      = 0x10A800L;
static const UINT32 s_FroggerObjRamBase      = 0x00B000L;
static const UINT32 s_FroggerWdResInputBase  = 0x008800L;
static const UINT32 s_FroggerCtrlOutputBase  = 0x00B800L;
static const UINT32 s_Frogger8255IoBase0     = 0x00E000L;
static const UINT32 s_Frogger8255IoBase1     = 0x00D000L;


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                                                    "012", "012345"
                                                  {NO_BANK_SWITCH, s_FroggerProgRamBase + 0x000, s_FroggerProgRamBase + 0x3FF, 1, 0x0F, " 19", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, s_FroggerProgRamBase + 0x000, s_FroggerProgRamBase + 0x3FF, 1, 0xF0, " 22", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, s_FroggerProgRamBase + 0x400, s_FroggerProgRamBase + 0x7FF, 1, 0x0F, " 20", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, s_FroggerProgRamBase + 0x400, s_FroggerProgRamBase + 0x7FF, 1, 0xF0, " 21", "Prog. "}, // "Program RAM, 2114"
                                                  //
                                                  // See note above about access restrictions w.r.t HBLANK & WAIT.
                                                  // These regions are access with special support in the CZ80Cpu triggered via address 0x10xxxx.
                                                  //
                                                  //                                                                                    "012", "012345"
                                                  {NO_BANK_SWITCH, s_FroggerBkVRamBase  + 0x000, s_FroggerBkVRamBase  + 0x3FF, 1, 0x0F, " 85", "BkVRam"}, // "Background VRAM, 2114"
                                                  {NO_BANK_SWITCH, s_FroggerBkVRamBase  + 0x000, s_FroggerBkVRamBase  + 0x3FF, 1, 0xF0, " 86", "BkVRam"}, // "Background VRAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                                                    "012", "012345"
                                                          {NO_BANK_SWITCH, s_FroggerProgRamBase + 0x000, s_FroggerProgRamBase + 0x3FF, 1, 0xFF, " - ", "Prog. "}, // "Program RAM, 2114, 1K/1G"
                                                          {NO_BANK_SWITCH, s_FroggerProgRamBase + 0x400, s_FroggerProgRamBase + 0x7FF, 1, 0xFF, " - ", "Prog. "}, // "Program RAM, 2114, 1J/1H"
                                                          {NO_BANK_SWITCH, s_FroggerBkVRamBase  + 0x000, s_FroggerBkVRamBase  + 0x3FF, 1, 0xFF, " - ", "BkVRam"}, // "Background VRAM, 2114, 3K/3J"
                                                          {0}
                                                        }; // end of list

//
// RAM region is the same for all games on this board set.
// Unlike Galaxian, it appears that the object RAM cannot be read.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                                                                   "012", "012345"
                                                           {NO_BANK_SWITCH, s_FroggerObjRamBase + 0x000, s_FroggerObjRamBase + 0x0FF, 1, 0x0F, " 50", "ObjRam"}, // "Object RAM, 2114, 256 Bytes used."
                                                           {NO_BANK_SWITCH, s_FroggerObjRamBase + 0x000, s_FroggerObjRamBase + 0x0FF, 1, 0xF0, " 51", "ObjRam"}, // "Object RAM, 2114, 256 Bytes used."
                                                           {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                                                                  "012", "012345"
                                                      {NO_BANK_SWITCH,                            s_FroggerWdResInputBase + 0x000, 0xFF,  " 16", "WD Res"}, // Watchdog reset
                                                      {CFroggerBaseGame::onBankSwitchSetup8255_0, s_Frogger8255IoBase0    + 0x000, 0xFF,  "s 7", "Port A"}, // CP Inputs
                                                      {CFroggerBaseGame::onBankSwitchSetup8255_0, s_Frogger8255IoBase0    + 0x002, 0xFF,  "s 7", "Port B"}, // CP Inputs
                                                      {CFroggerBaseGame::onBankSwitchSetup8255_0, s_Frogger8255IoBase0    + 0x004, 0xFF,  "s 7", "Port C"}, // CP Inputs
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                                                         "012", "012345"
                                                        {NO_BANK_SWITCH,                             s_FroggerCtrlOutputBase + 0x008, 0x01, 0x00,  " 42", "NMI En"}, // NMI enable
                                                        {NO_BANK_SWITCH,                             s_FroggerCtrlOutputBase + 0x00C, 0x01, 0x00,  " 42", "Flip Y"}, // Flip Y
                                                        {NO_BANK_SWITCH,                             s_FroggerCtrlOutputBase + 0x010, 0x01, 0x00,  " 42", "Flip X"}, // Flip X
                                                        {NO_BANK_SWITCH,                             s_FroggerCtrlOutputBase + 0x018, 0x01, 0x00,  " 42", "IOPC7 "}, // Coin counter 0
                                                        {NO_BANK_SWITCH,                             s_FroggerCtrlOutputBase + 0x01C, 0x01, 0x00,  " 42", "POUT1 "}, // Coin counter 1
                                                        {CFroggerBaseGame::onBankSwitchSetup8255_1,  s_Frogger8255IoBase1    + 0x000, 0xFF, 0x00,  "s40", "Snd Cm"}, // Sound command
                                                        {CFroggerBaseGame::onBankSwitchSetup8255_1,  s_Frogger8255IoBase1    + 0x002, 0x08, 0x00,  "s40", "Snd In"}, // Sound interrupt
                                                        {CFroggerBaseGame::onBankSwitchSetup8255_1,  s_Frogger8255IoBase1    + 0x002, 0x10, 0x00,  "s40", "Amp Mt"}, // Amp Mute
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                   "0123456789"
                                                            {CFroggerBaseGame::clearBk,          "Clear Bk  "},
                                                            {CFroggerBaseGame::clearObj,         "Clear Obj "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list

CFroggerBaseGame::CFroggerBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : m_intMaskWriteAddress(s_FroggerCtrlOutputBase + 0x008),
    m_8255WriteBaseAddress0(s_Frogger8255IoBase0),
    m_8255WriteBaseAddress1(s_Frogger8255IoBase1),
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
}


CFroggerBaseGame::~CFroggerBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// This is a specific implementation for Frogger that uses an externally
// maskable/resetable latch for the VBLANK interrupt on the NMI line.
PERROR
CFroggerBaseGame::interruptCheck(
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
//  PPI0 (A15 & A14 & A13) = 0xE000 - Control inputs
//   - Port A (All) Mode 0 Input
//   - Port B (All) Mode 0 Input
//   - Port C (Lo)  Mode 0 Input
//   - Port C (Hi)  Mode 0 Input
//
//     == 1 0011 011 == 0x9B
//
PERROR
CFroggerBaseGame::onBankSwitchSetup8255_0(
    void *cFroggerBaseGame
)
{
    CFroggerBaseGame *thisGame  = (CFroggerBaseGame *) cFroggerBaseGame;
    ICpu              *cpu       = thisGame->m_cpu;

    return cpu->memoryWrite(thisGame->m_8255WriteBaseAddress0 + 0x006, 0x9B);
}


//
// This is used to setup the 8255 to reflect the hardware configuration:
//
//  PPI1 (A15 & A14 & A12) = 0xD000 - Sound latch & protection.
//   - Port A (All) Mode 0 Output (Sound Command)
//   - Port B (All) Mode 0 Output (PB3 - Sound Interrupt, PB4 - Amp Mute)
//   - Port C (Lo)  Mode 0 Output (Protection Write on Scramble)
//   - Port C (Hi)  Mode 0 Input  (Protection Read on Scramble)
//
//     == 1 0001 000 == 0x88
//
PERROR
CFroggerBaseGame::onBankSwitchSetup8255_1(
    void *cFroggerBaseGame
)
{
    CFroggerBaseGame *thisGame  = (CFroggerBaseGame *) cFroggerBaseGame;
    ICpu              *cpu       = thisGame->m_cpu;

    return cpu->memoryWrite(thisGame->m_8255WriteBaseAddress1 + 0x006, 0x88);
}


//
// Clear the video display to blank (background)
//
PERROR
CFroggerBaseGame::clearBk(
    void *context
)
{
    CFroggerBaseGame *thisGame = (CFroggerBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT32 count;

    //
    // The 0x080 is the address in the 5F/5H graphics ROMS
    // of the all-zero character on Scorpion.
    //
    static const UINT32 bkVRamStart  = s_FroggerBkVRamBase;
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
CFroggerBaseGame::clearObj(
    void *context
)
{
    CFroggerBaseGame *thisGame = (CFroggerBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT32 count;

    //
    // The 0x080 is the address in the 5F/5H graphics ROMS
    // of the all-zero character on Scorpion.
    //
    static const UINT32 objRamStart  = s_FroggerObjRamBase;
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


