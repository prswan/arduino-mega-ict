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
#include "CSystem2BaseGame.h"
#include "CT11Cpu.h"

//
// Probe Head GND:
//   T11 GND Pin 20
//
// Watchdog Disable:
//   IC 5J (LS08) to GND
//
// Board Designations:
//   'c' - CPU board.
//   'v' - Video board.
//
// CPU Compatibility Notes
//   * TBD. Some sort of clock stretching circuit used with VRAM access.
//


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //
                                                  // See note above about access restrictions w.r.t video RAM access
                                                  // These regions are access with special support in the CT11Cpu triggered
                                                  // via address 0x10xxxx.
                                                  //
                                                  //                                                               "012", "012345"
                                                  {NO_BANK_SWITCH, 0x10000|(0x0000>>1), 0x10000|(0x0FFF>>1), 0xFF, "c7K", "Prog L"}, // Program RAM 6116, Lo
                                                  {NO_BANK_SWITCH, 0x20000|(0x0000>>1), 0x20000|(0x0FFF>>1), 0xFF, "c7P", "Prog H"}, // Program RAM 6116, Hi
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
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                 "012", "012345"
                                                        {NO_BANK_SWITCH, 0x20000|(0x1400>>1), 0xFC, 0x00,  "c??", "PPage0"}, // Prog. Mem page 0
                                                        {NO_BANK_SWITCH, 0x20000|(0x1402>>1), 0xFC, 0x00,  "c??", "PPage1"}, // Prog. Mem page 1
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { {NO_CUSTOM_FUNCTION}}; // end of list


CSystem2BaseGame::CSystem2BaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new CT11Cpu();
    m_cpu->idle();

    // Default
    m_interrupt = ICpu::NMI;
    m_interruptAutoVector = true;
}


CSystem2BaseGame::~CSystem2BaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// According to the schematics Pheonix has no interrupts connected.
//
PERROR
CSystem2BaseGame::interruptCheck(
)
{
    PERROR           error     = errorNotImplemented;
    return error;
}


//
// Select program page 0
// TBD
//
PERROR
CSystem2BaseGame::onBankSwitchProgramPage0(
    void *cSystem2BaseGame
)
{
    PERROR           error     = errorNotImplemented;
    return error;
}

//
// Select program page 1
// TBD
//
PERROR
CSystem2BaseGame::onBankSwitchProgramPage1(
    void *cSystem2BaseGame
)
{
    PERROR           error     = errorNotImplemented;
    return error;
}

