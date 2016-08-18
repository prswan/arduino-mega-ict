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
#include "CSpaceInvadersBaseGame.h"
#include "C8080Cpu.h"

//
// Probe Head GND:
//   The 8080 specific probe head must be used that has the none-TTL voltages isolated.
//
// Watchdog Disable:
//   TBD.
//
// Board Designations:
//   's' - Sound board.
//   'c' - CPU board.
//   'r' - ROM board.
//
// CPU Compatibility Notes
//   * READY is used to hold off the CPU. This is for holding off access to the shared VRAM.
//     There is a workaround in the 8080 CPU implementation to pre-synchronize with READY to
//     ensure that the access to VRAM works properly.
//


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //
                                                  // See note above about access restrictions w.r.t READY
                                                  // These regions are access with special support in the C8080Cpu triggered
                                                  // via address 0x10xxxx.
                                                  //
                                                  //                                         "012", "012345"
                                                  {NO_BANK_SWITCH, 0x102000, 0x103FFF, 0xFF, "   ", "VRAM  "}, // Video RAM
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
                                                      {NO_BANK_SWITCH, 0x10000L, 0xFF, "   ", "IN0   "}, // Switch Inputs (IN0)
                                                      {NO_BANK_SWITCH, 0x10001L, 0xFF, "   ", "IN1   "}, // Switch Inputs (IN1)
                                                      {NO_BANK_SWITCH, 0x10002L, 0xFF, "   ", "IN2   "}, // Switch Inputs (IN2)
                                                      {NO_BANK_SWITCH, 0x10003L, 0xFF, "   ", "ShiftR"}, // Shift Result
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //
                                                        // Note that in order to hear the sounds the Sound On bit must be high.
                                                        // Describing that in the invert mask leaves the sound always on so that
                                                        // the decay after trigger can be heard.
                                                        //
                                                        //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x10003L, 0x01, 0x20, "S0 ", "Ufo   "}, // S0 - SM1
                                                        {NO_BANK_SWITCH, 0x10003L, 0x02, 0x20, "S1 ", "Fire  "}, // S1 - SM2
                                                        {NO_BANK_SWITCH, 0x10003L, 0x04, 0x20, "S2 ", "BseHit"}, // S2 - SM3
                                                        {NO_BANK_SWITCH, 0x10003L, 0x08, 0x20, "S3 ", "InvHit"}, // S3 - SM4
                                                        {NO_BANK_SWITCH, 0x10003L, 0x10, 0x20, "S4 ", "Bonus "}, // S4 - SM5
                                                        {NO_BANK_SWITCH, 0x10003L, 0x20, 0x00, "S5 ", "Snd On"}, // S5 - Sound On
                                                        {NO_BANK_SWITCH, 0x10005L, 0x01, 0x20, "S6 ", "Step01"}, // S6 - Step Bit 1
                                                        {NO_BANK_SWITCH, 0x10005L, 0x02, 0x20, "S7 ", "Step02"}, // S7 - Step Bit 2
                                                        {NO_BANK_SWITCH, 0x10005L, 0x04, 0x20, "S8 ", "Step04"}, // S8 - Step Bit 4
                                                        {NO_BANK_SWITCH, 0x10005L, 0x08, 0x20, "S9 ", "Step08"}, // S9 - Step Bit 8
                                                        {NO_BANK_SWITCH, 0x10005L, 0x10, 0x20, "S10", "UfoHit"}, // S10 - UFO Hit
                                                        {NO_BANK_SWITCH, 0x10005L, 0x20, 0x00, "S11", "Flip  "}, // S11 - Flip screen
                                                        {NO_BANK_SWITCH, 0x10004L, 0xFF, 0x00, "   ", "ShiftD"}, // Shift Data
                                                        {NO_BANK_SWITCH, 0x10002L, 0x07, 0x20, "   ", "ShiftC"}, // Shift Count
                                                        {NO_BANK_SWITCH, 0x10006L, 0x01, 0x00, "   ", "Wd Res"}, // Watchdog Reset
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { {NO_CUSTOM_FUNCTION}}; // end of list


CSpaceInvadersBaseGame::CSpaceInvadersBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new C8080Cpu();
    m_cpu->idle();

    //
    // Space Invaders uses INT with an external vector where the
    // vector depends on the video count triggering it
    // (0xcf and 0xd7)
    //
    m_interrupt = ICpu::IRQ0;
    m_interruptAutoVector = false;
}


CSpaceInvadersBaseGame::~CSpaceInvadersBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// TBD.
//
PERROR
CSpaceInvadersBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}

