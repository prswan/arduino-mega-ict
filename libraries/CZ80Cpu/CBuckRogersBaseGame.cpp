//
// Copyright (c) 2018, Paul R. Swan
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
#include "CBuckRogersBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   TBD
//
// Board Designations:
//   's' - Sound PCB
//   'c' - CPU/Main PCB
//   'v' - Video PCB
//
// Z80 Compatibility Notes
//   * IO space is not used.
//   * M1 (1st machine cycle) is not used.
//
//   * Sprite RAM (0xE0xx -> 0xE7FF)
//     From what I could determine from the schematics and scope, access to the sprite
//     RAM activates WAIT that is held until a short access window appears in the video
//     timing (derived from the PROM IC78). On the scope, IC31 (74LS74) pin 4 is ~1400ns
//     of high with ~200ns of low. I suspect the 200ns low portion to tbe the CPU access
//     window.
//     Alas, the current ICT Z80 implementation is not compatible with this timing.
//

//
// Base addresses for the 8255
//
static const UINT32 s_8255IoBase0 = 0x00C800L; // IC90,  Sub CPU
static const UINT32 s_8255IoBase1 = 0x00D000L; // IC155, Sound

static const UINT16 s_8255IoPortIdle1A = 0xF0;
static const UINT16 s_8255IoPortIdle1B = 0xBF;
static const UINT16 s_8255IoPortIdle1C = 0xFF;

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x00C000, 0x00C7FF, 2, 0x0F, "c9 ", "VRAM E"}, // Background RAM, Even
                                                  {NO_BANK_SWITCH, 0x00C000, 0x00C7FF, 2, 0xF0, "c8 ", "VRAM E"}, // Background RAM, Even
                                                  {NO_BANK_SWITCH, 0x00C001, 0x00C7FF, 2, 0x0F, "c7 ", "VRAM O"}, // Background RAM, Odd
                                                  {NO_BANK_SWITCH, 0x00C001, 0x00C7FF, 2, 0xF0, "c6 ", "VRAM O"}, // Background RAM, Odd
                                                  {NO_BANK_SWITCH, 0x00F800, 0x00FFFF, 1, 0xFF, "c14", "Prog. "}, // Program RAM
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x00C000, 0x00C7FF, 1, 0xFF, " - ", "VRAM  "}, // Background RAM, Even
                                                          {NO_BANK_SWITCH, 0x00F800, 0x00FFFF, 1, 0xFF, "c14", "Prog. "}, // Program RAM
                                                          {0}
                                                        }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                            "012", "012345"
//                                                         {NO_BANK_SWITCH, 0x00E000, 0x00E7FF, 1, 0xFF, "c84", "SP RAM"}, // Sprite RAM
                                                           {0}
                                                         }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                              "012", "012345"
                                                      {NO_BANK_SWITCH, 0xE800, 0xFF,  "IN1", "Inputs"}, //
                                                      {NO_BANK_SWITCH, 0xE801, 0xFF,  "IN2", "Inputs"}, //
                                                      {NO_BANK_SWITCH, 0xE802, 0xFF,  "SW1", "DipSw "}, //
                                                      {NO_BANK_SWITCH, 0xE803, 0xFF,  "SW2", "DipSw "}, //
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                                                             "012", "012345"
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x0, 0x01, s_8255IoPortIdle1A, " - ", "Data 0"}, //
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x0, 0x02, s_8255IoPortIdle1A, " - ", "Data 1"}, //
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x0, 0x04, s_8255IoPortIdle1A, " - ", "Data 2"}, //
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x0, 0x08, s_8255IoPortIdle1A, " - ", "Data 3"}, //
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x0, 0x10, s_8255IoPortIdle1A, "s2 ", "HitClk"}, //
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x0, 0x20, s_8255IoPortIdle1A, "s6 ", "ShpClk"}, //
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x0, 0x40, s_8255IoPortIdle1A, "s3 ", "Alarm0"}, //  7 -
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x0, 0x80, s_8255IoPortIdle1A, "s3 ", "Alarm1"}, //  8 -
                                                        //
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x1, 0x01, s_8255IoPortIdle1B, "s7 ", "Alarm2"}, //  9 -
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x1, 0x02, s_8255IoPortIdle1B, "s7 ", "Alarm3"}, // 10 -
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x1, 0x04, s_8255IoPortIdle1B, "s4 ", "Fire  "}, // 11 -
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x1, 0x08, s_8255IoPortIdle1B, "s8 ", "Exp   "}, // 12 -
                                                        {CBuckRogersBaseGame::onBankSwitchSetupHitDs0, s_8255IoBase1 + 0x1, 0x10, s_8255IoPortIdle1B, "s13", "HitDs0"}, //
                                                        {CBuckRogersBaseGame::onBankSwitchSetupHitDs1, s_8255IoBase1 + 0x1, 0x10, s_8255IoPortIdle1B, "s13", "HitDs1"}, //
                                                        {CBuckRogersBaseGame::onBankSwitchSetupHitDs2, s_8255IoBase1 + 0x1, 0x10, s_8255IoPortIdle1B, "s13", "HitDs2"}, //
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x1, 0x10, s_8255IoPortIdle1B, "s13", "Hit   "}, //
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x1, 0x20, s_8255IoPortIdle1B, "s13", "Rbound"}, // 14 -
                                                        {CBuckRogersBaseGame::onBankSwitchSetupShpAc0, s_8255IoBase1 + 0x1, 0x40, s_8255IoPortIdle1B, "s13", "ShpAc0"}, //
                                                        {CBuckRogersBaseGame::onBankSwitchSetupShpAc1, s_8255IoBase1 + 0x1, 0x40, s_8255IoPortIdle1B, "s13", "ShpAc1"}, //
                                                        {CBuckRogersBaseGame::onBankSwitchSetupShpAc2, s_8255IoBase1 + 0x1, 0x40, s_8255IoPortIdle1B, "s13", "ShpAc2"}, //
                                                        {CBuckRogersBaseGame::onBankSwitchSetupShpAc3, s_8255IoBase1 + 0x1, 0x40, s_8255IoPortIdle1B, "s13", "ShpAc3"}, //
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x1, 0x40, s_8255IoPortIdle1B, "s10", "Shp On"}, // 15 -
                                                        //
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x1, 0x80, s_8255IoPortIdle1B,  "Amp", "GameOn"}, // 16 -
                                                        //
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x2, 0x01, s_8255IoPortIdle1C,  " - ", "OBCH0 "},
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x2, 0x02, s_8255IoPortIdle1C,  " - ", "OBCH1 "},
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x2, 0x04, s_8255IoPortIdle1C,  " - ", "OBCH2 "},
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x2, 0x08, s_8255IoPortIdle1C,  " - ", "OBCH3 "},
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x2, 0x10, s_8255IoPortIdle1C,  "118", "CMtr 1"}, // Coin Meter 1
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x2, 0x20, s_8255IoPortIdle1C,  "118", "CMtr 2"}, // Coin Meter 2
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x2, 0x40, s_8255IoPortIdle1C,  "118", "Strt L"}, // Start Lamp
                                                        {NO_BANK_SWITCH,                               s_8255IoBase1 + 0x2, 0x80, s_8255IoPortIdle1C,  "118", "Bdy Sc"}, // Body Sonic
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CBuckRogersBaseGame::CBuckRogersBaseGame(
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
    m_cpu = new CZ80ACpu();
    m_cpu->idle();

    //
    // The VBLANK interrupt is on the INT pin.
    //
    m_interrupt = ICpu::IRQ0;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;

    //
    // Because the 8255 resets the output latches low when the mode
    // is changed and the sound triggers are active low, reprogramming
    // the 8255 on every output write causes the sounds to trigger.
    // The same artifact can be heard when the game boots.
    // Therefore, we setup the 8255 here as part of platform initialization.
    //
    //  U23 - Sound Outputs
    //   - Port A (All) Mode 0 Output
    //   - Port B (All) Mode 0 Output
    //   - Port C (Lo)  Mode 0 Output
    //   - Port C (Hi)  Mode 0 Output
    //
    //     == 1 0000 000 == 0x80
    //
    m_cpu->memoryWrite(s_8255IoBase1 + 0x3, 0x80);

    m_cpu->memoryWrite(s_8255IoBase1 + 0x0, s_8255IoPortIdle1A);
    m_cpu->memoryWrite(s_8255IoBase1 + 0x1, s_8255IoPortIdle1B);
    m_cpu->memoryWrite(s_8255IoBase1 + 0x2, s_8255IoPortIdle1C);

}


CBuckRogersBaseGame::~CBuckRogersBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// TBD
PERROR
CBuckRogersBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}


PERROR
CBuckRogersBaseGame::loadSoundLatch(
    UINT16 clock,
    UINT16 data
)
{
    PERROR error = errorSuccess;

    if (SUCCESS(error))
    {
        error = m_cpu->memoryWrite(s_8255IoBase1 + 0x0, s_8255IoPortIdle1A ^ data);
    }

    if (SUCCESS(error))
    {
        error = m_cpu->memoryWrite(s_8255IoBase1 + 0x0, s_8255IoPortIdle1A ^ (data | clock));
    }

    if (SUCCESS(error))
    {
        error = m_cpu->memoryWrite(s_8255IoBase1 + 0x0, s_8255IoPortIdle1A ^ data);
    }

    return error;
}


PERROR
CBuckRogersBaseGame::onBankSwitchSetupHitDs0(
    void *cBuckRogersBaseGame
)
{
    CBuckRogersBaseGame *thisGame = (CBuckRogersBaseGame *) cBuckRogersBaseGame;

    return thisGame->loadSoundLatch(0x10, 0x01);
}


PERROR
CBuckRogersBaseGame::onBankSwitchSetupHitDs1(
    void *cBuckRogersBaseGame
)
{
    CBuckRogersBaseGame *thisGame = (CBuckRogersBaseGame *) cBuckRogersBaseGame;

    return thisGame->loadSoundLatch(0x10, 0x02);
}

PERROR
CBuckRogersBaseGame::onBankSwitchSetupHitDs2(
    void *cBuckRogersBaseGame
)
{
    CBuckRogersBaseGame *thisGame = (CBuckRogersBaseGame *) cBuckRogersBaseGame;

    return thisGame->loadSoundLatch(0x10, 0x04);
}


PERROR
CBuckRogersBaseGame::onBankSwitchSetupShpAc0(
    void *cBuckRogersBaseGame
)
{
    CBuckRogersBaseGame *thisGame = (CBuckRogersBaseGame *) cBuckRogersBaseGame;

    return thisGame->loadSoundLatch(0x20, 0x01);
}


PERROR
CBuckRogersBaseGame::onBankSwitchSetupShpAc1(
    void *cBuckRogersBaseGame
)
{
    CBuckRogersBaseGame *thisGame = (CBuckRogersBaseGame *) cBuckRogersBaseGame;

    return thisGame->loadSoundLatch(0x20, 0x02);
}


PERROR
CBuckRogersBaseGame::onBankSwitchSetupShpAc2(
    void *cBuckRogersBaseGame
)
{
    CBuckRogersBaseGame *thisGame = (CBuckRogersBaseGame *) cBuckRogersBaseGame;

    return thisGame->loadSoundLatch(0x20, 0x04);
}


PERROR
CBuckRogersBaseGame::onBankSwitchSetupShpAc3(
    void *cBuckRogersBaseGame
)
{
    CBuckRogersBaseGame *thisGame = (CBuckRogersBaseGame *) cBuckRogersBaseGame;

    return thisGame->loadSoundLatch(0x20, 0x08);
}


