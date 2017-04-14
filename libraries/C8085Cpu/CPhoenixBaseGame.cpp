//
// Copyright (c) 2016, Paul R. Swan
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
#include "CPhoenixBaseGame.h"
#include "C8085Cpu.h"

//
// Probe Head GND:
//   8085 GND Pin 20
//
// Watchdog Disable:
//   There is no watchdog.
//
// Board Designations:
//   'c' - CPU board.
//   'r' - ROM board.
//
// CPU Compatibility Notes
//   * READY is used to hold off the CPU. This is for holding off access to the shared VRAM however I
//     wasn't able to see from the schematics how READY doesn't apply to everything (i.e. the CPU is
//     prevented from executing during video display). There is a workaround in the 8085 CPU implementation
//     to pre-synchronize with READY to ensure that the access to VRAM works properly.
//


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //
                                                  // See note above about access restrictions w.r.t READY
                                                  // These regions are access with special support in the C8085Cpu triggered
                                                  // via address 0x10xxxx.
                                                  //
                                                  //                                                                     "012", "012345"
                                                  {CPhoenixBaseGame::onBankSwitchVRamBank1, 0x104000, 0x1043FF, 1, 0x0F, "r41", "FgRam1"}, // Foreground Video RAM Bank 1, 2114
                                                  {CPhoenixBaseGame::onBankSwitchVRamBank1, 0x104000, 0x1043FF, 1, 0xF0, "r43", "FgRam1"}, // Foreground Video RAM Bank 1, 2114
                                                  {CPhoenixBaseGame::onBankSwitchVRamBank1, 0x104800, 0x104BFF, 1, 0x0F, "r25", "BgRam1"}, // Background Video RAM Bank 1, 2114
                                                  {CPhoenixBaseGame::onBankSwitchVRamBank1, 0x104800, 0x104BFF, 1, 0xF0, "r27", "BgRam1"}, // Background Video RAM Bank 1, 2114
                                                  {CPhoenixBaseGame::onBankSwitchVRamBank2, 0x104000, 0x1043FF, 1, 0x0F, "r42", "FgRam2"}, // Foreground Video RAM Bank 2, 2114
                                                  {CPhoenixBaseGame::onBankSwitchVRamBank2, 0x104000, 0x1043FF, 1, 0xF0, "r44", "FgRam2"}, // Foreground Video RAM Bank 2, 2114
                                                  {CPhoenixBaseGame::onBankSwitchVRamBank2, 0x104800, 0x104BFF, 1, 0x0F, "r26", "BgRam2"}, // Background Video RAM Bank 2, 2114
                                                  {CPhoenixBaseGame::onBankSwitchVRamBank2, 0x104800, 0x104BFF, 1, 0xF0, "r28", "BgRam2"}, // Background Video RAM Bank 2, 2114
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                                     "012", "012345"
                                                          {CPhoenixBaseGame::onBankSwitchVRamBank1, 0x104000, 0x1043FF, 1, 0xFF, "r4?", "FgRam1"}, // Foreground Video RAM Bank 1, 2114, 41/43
                                                          {CPhoenixBaseGame::onBankSwitchVRamBank1, 0x104800, 0x104BFF, 1, 0xFF, "r2?", "BgRam1"}, // Background Video RAM Bank 1, 2114, 25/27
                                                          {CPhoenixBaseGame::onBankSwitchVRamBank2, 0x104000, 0x1043FF, 1, 0xFF, "r4?", "FgRam2"}, // Foreground Video RAM Bank 2, 2114, 42/44
                                                          {CPhoenixBaseGame::onBankSwitchVRamBank2, 0x104800, 0x104BFF, 1, 0xFF, "r2?", "BgRam2"}, // Background Video RAM Bank 2, 2114, 26/28
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
                                                      {NO_BANK_SWITCH, 0x7000L, 0x0F,  "c04", "InStrt"}, // Switch Inputs (IN0)
                                                      {NO_BANK_SWITCH, 0x7000L, 0xF0,  "c12", "InPlay"}, // Switch Inputs (IN0)
                                                      {NO_BANK_SWITCH, 0x7800L, 0xFF,  "c03", "DIP SW"}, // DIP Switch
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x5000L, 0x01, 0x00,  "c37", "V Bank"}, // Video bank select
                                                        {NO_BANK_SWITCH, 0x5000L, 0x02, 0x00,  "c37", "C Bank"}, // Colour bank select
                                                        {NO_BANK_SWITCH, 0x5800L, 0x55, 0x00,  "r08", "Scrl55"}, // Background scroll register
                                                        {NO_BANK_SWITCH, 0x5800L, 0xAA, 0x00,  "r08", "ScrlAA"}, // Background scroll register
                                                        {NO_BANK_SWITCH, 0x5800L, 0xFF, 0x00,  "r08", "ScrlFF"}, // Background scroll register
                                                        {NO_BANK_SWITCH, 0x6000L, 0x0F, 0x0F,  "c06", "SCntA "}, // Sound A counter
                                                        {NO_BANK_SWITCH, 0x6000L, 0x38, 0x0F,  "c06", "SFrqA "}, // Sound A frequency
                                                        {NO_BANK_SWITCH, 0x6000L, 0x40, 0x0F,  "c06", "SNoiA1"}, // Sound A noise
                                                        {NO_BANK_SWITCH, 0x6000L, 0x80, 0x0F,  "c06", "SNoiA2"}, // Sound A noise
                                                        {NO_BANK_SWITCH, 0x6800L, 0x0F, 0x0F,  "c05", "SCntB "}, // Sound B counter
                                                        {NO_BANK_SWITCH, 0x6800L, 0x18, 0x0F,  "c05", "SFrqB "}, // Sound B frequency
                                                        {NO_BANK_SWITCH, 0x6800L, 0x28, 0x0F,  "c05", "SFltB "}, // Sound B filter
                                                        {NO_BANK_SWITCH, 0x6800L, 0x80, 0x0F,  "c05", "Mus 2 "}, // Sound B music, ending (MN6221AA)
                                                        {NO_BANK_SWITCH, 0x6800L, 0xC0, 0x0F,  "c05", "Mus 1 "}, // Sound B music, starting (MN6221AA)
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { {NO_CUSTOM_FUNCTION}}; // end of list


CPhoenixBaseGame::CPhoenixBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new C8085Cpu();
    m_cpu->idle();

    // Phoenix has no interrupts connected
    m_interrupt = ICpu::NMI;
    m_interruptAutoVector = true;

    // Pheonix has no hardware reset clear of the sound latches
    // so we'll do a clear here fo convenience to save our ears.
    m_cpu->memoryWrite(0x6000, 0x0F);
    m_cpu->memoryWrite(0x6800, 0x0F);
    m_cpu->idle();

}


CPhoenixBaseGame::~CPhoenixBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// According to the schematics Pheonix has no interrupts connected.
//
PERROR
CPhoenixBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}


//
// Select video bank 1
// ONE-00 is Lo, CS on 2114 is Lo (active)
// ONE-01 is Hi
//
PERROR
CPhoenixBaseGame::onBankSwitchVRamBank1(
    void *cPhoenixBaseGame
)
{
    PERROR           error     = errorSuccess;
    CPhoenixBaseGame *thisGame  = (CPhoenixBaseGame *) cPhoenixBaseGame;
    ICpu             *cpu       = thisGame->m_cpu;

    error = cpu->memoryWrite(0x5000, 0x00);

    return error;
}

//
// Select video bank 2
// ONE-00 is Hi
// ONE-01 is Lo, CS on 2114 is Lo (active)
//
PERROR
CPhoenixBaseGame::onBankSwitchVRamBank2(
    void *cPhoenixBaseGame
)
{
    PERROR           error     = errorSuccess;
    CPhoenixBaseGame *thisGame  = (CPhoenixBaseGame *) cPhoenixBaseGame;
    ICpu             *cpu       = thisGame->m_cpu;

    error = cpu->memoryWrite(0x5000, 0x01);

    return error;
}

