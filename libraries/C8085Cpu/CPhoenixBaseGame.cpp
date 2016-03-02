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
                                                  //                                         "012", "012345"
                                                  {NO_BANK_SWITCH, 0x104000, 0x1043FF, 0x0F, "???", "VRAM  "}, // Video RAM, 2114
                                                  {NO_BANK_SWITCH, 0x104000, 0x1043FF, 0xF0, "???", "VRAM  "}, // Video RAM, 2114
                                                  {NO_BANK_SWITCH, 0x104400, 0x1047FF, 0x0F, "???", "VRAM  "}, // Video RAM, 2114
                                                  {NO_BANK_SWITCH, 0x104400, 0x1047FF, 0xF0, "???", "VRAM  "}, // Video RAM, 2114
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
                                                      {NO_BANK_SWITCH, 0x7000L, 0xFF,  "???", "IN SW0"}, // Switch Inputs (IN0)
                                                      {NO_BANK_SWITCH, 0x7800L, 0xFF,  "???", "DIP SW"}, // DIP Switch
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x5000L, 0x01, 0x00,  "???", "V.Bank"}, // Video bank select
                                                        {NO_BANK_SWITCH, 0x5000L, 0x02, 0x00,  "???", "C.Bank"}, // Colour bank select
                                                        {NO_BANK_SWITCH, 0x5800L, 0xFF, 0x00,  "???", "Scroll"}, // Scroll register
                                                        {NO_BANK_SWITCH, 0x6000L, 0xFF, 0x00,  "???", "Snd A "}, // Sound A
                                                        {NO_BANK_SWITCH, 0x6800L, 0xFF, 0x00,  "???", "Snd B "}, // Sound B
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

