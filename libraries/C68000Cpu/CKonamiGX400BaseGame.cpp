//
// Copyright (c) 2017, Paul R. Swan
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
#include "CKonamiGX400BaseGame.h"
#include "C68000DedicatedCpu.h"
#include "CRamCheck.h"

//
// Probe Head GND:
//   The 68000 specific probe head must be used.
//   The CLK rate is 6MHz (166ns per cycle)
//
// Watchdog Disable:
//   Pin 3 of the custom SIL 005924 at 1D (near the audio heatsink) is grounded.
//   It's routed to a pair of pads nearby where a 2-pin header can be fitted to allow a jumper link.
//
// Board Designations:
//   'c' - CPU board.
//   'v' - Video board.
//
// CPU Compatibility Notes
//
//   ** WARNING **
//
//   The timing is marginal accross all memory regions on this platform due to synchronous
//   cycle completion between DTACK, CLK and video timing. Writes appear to go through OK
//   but for reads the data is gone by the time the Arduino has sensed DTACK and read the
//   data port. The compiler has this already optimized to be the bare minimum of 3 instructions
//   so there is no known workaround at the present time to make it reliable.
//   ROM and Program RAM are by most reliable and thus sufficient for diagnosing most dead boards.
//   This configuration was enough to get a dead Konami GT booting (a bad ROM that read OK in the DataIO).
//


//
// RAM region is the same for all games on this board set.
// All RAM is byte-wide.
//
// xx1 = LDS
// xx0 = UDS
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                "012", "012345"
                                                  {NO_BANK_SWITCH, 0x00040000, 0x0004ffff, 1, 0xFF, "   ", "CHARA "}, // Character RAM, B49 => CHACS
                                                  {NO_BANK_SWITCH, 0x00040001, 0x0004ffff, 1, 0xFF, "   ", "CHARA "}, // Character RAM
                                                  {NO_BANK_SWITCH, 0x00050001, 0x00050fff, 1, 0xFF, "22D", "VZURE "}, // Scroll RAM,    B37 => VZCS
                                                  {NO_BANK_SWITCH, 0x00052000, 0x00053fff, 1, 0xFF, "15C", "VRAMC1"}, // Video RAM 1,   B41 => VCS1
                                                  {NO_BANK_SWITCH, 0x00052001, 0x00053fff, 1, 0xFF, "15B", "VRAMC1"}, // Video RAM 1
                                                  {NO_BANK_SWITCH, 0x00054001, 0x00055fff, 1, 0xFF, "15D", "VRAMC2"}, // Colour RAM 1,  B39 => VCS2
                                                  {NO_BANK_SWITCH, 0x00056001, 0x00056fff, 1, 0xFF, "25D", "OBJRAM"}, // Sprite RAM,    B35 => OBJRAM
                                                  {NO_BANK_SWITCH, 0x0005a000, 0x0005afff, 1, 0xFF, "15K", "COLORR"}, // Palette RAM
                                                  {NO_BANK_SWITCH, 0x0005a001, 0x0005afff, 1, 0xFF, "14K", "COLORR"}, // Palette RAM
                                                  {NO_BANK_SWITCH, 0x00060000, 0x00067fff, 1, 0xFF, "   ", "Prog  "}, // Program RAM
                                                  {NO_BANK_SWITCH, 0x00060001, 0x00067fff, 1, 0xFF, "   ", "Prog  "}, // Program RAM
                                                  {0}
                                                }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                  "012", "012345"
                                                      {NO_BANK_SWITCH, 0x0005c403L, 0xFF, "   ", "DSW0  "}, // Dip switches - 0
                                                      {NO_BANK_SWITCH, 0x0005c405L, 0xFF, "   ", "DSW1  "}, // Dip switches - 1
                                                      {NO_BANK_SWITCH, 0x0005cc01L, 0xFF, "   ", "IN0   "}, // Inputs - 0
                                                      {NO_BANK_SWITCH, 0x0005cc03L, 0xFF, "   ", "IN1   "}, // Inputs - 1
                                                      {NO_BANK_SWITCH, 0x0005cc05L, 0xFF, "   ", "IN2   "}, // Inputs - 2
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                          "012", "012345"
                                                        {NO_BANK_SWITCH, 0x0005c001L, 0xFF, 0x0000, "   ", "Snd-FF"},
                                                        {NO_BANK_SWITCH, 0x0005c001L, 0x55, 0x0000, "   ", "Snd-55"},
                                                        {NO_BANK_SWITCH, 0x0005c001L, 0xAA, 0x0000, "   ", "Snd-AA"},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { {NO_CUSTOM_FUNCTION} }; // end of list



CKonamiGX400BaseGame::CKonamiGX400BaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new C68000DedicatedCpu();
    m_cpu->idle();

    m_interrupt = ICpu::IRQ0;
    m_interruptAutoVector = false;
}


CKonamiGX400BaseGame::~CKonamiGX400BaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// TBD.
//
PERROR
CKonamiGX400BaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}

