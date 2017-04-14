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
#include "CKonamiGX945BaseGame.h"
#include "C68000DedicatedCpu.h"
#include "CRamCheck.h"

//
// Probe Head GND:
//   The 68000 specific probe head must be used.
//   The CLK rate is ~9MHz (111ns per cycle)
//
//   ** WARNING **
//
//   Konami routed the bus request (BR) directly to the VCC power plane. The CPU pin is next to a
//   SIL resistor array making it near impossible to cut the pin close to the board to release it.
//   As a result, this game is configured to not support bus request clip over and instead the 68K
//   must be removed :(
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
//


//
// RAM region is the same for all games on this board set.
// All RAM is byte-wide.
//
// xx1 = LDS
// xx0 = UDS
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                "012", "012345"
                                                  {NO_BANK_SWITCH, 0x00040000, 0x00043fff, 1, 0xFF, "prg", "Prog  "}, // Program RAM
                                                  {NO_BANK_SWITCH, 0x00040001, 0x00043fff, 1, 0xFF, "prg", "Prog  "}, // Program RAM
                                                  {NO_BANK_SWITCH, 0x00080000, 0x00080fff, 1, 0xFF, "plt", "Palate"}, // Palate RAM
                                                  {NO_BANK_SWITCH, 0x00080001, 0x00080fff, 1, 0xFF, "plt", "Palate"}, // Palate RAM
                                                  {NO_BANK_SWITCH, 0x00100000, 0x00103fff, 1, 0xFF, "shr", "Share "}, // Shared RAM
                                                  {NO_BANK_SWITCH, 0x00100001, 0x00103fff, 1, 0xFF, "shr", "Share "}, // Shared RAM
                                                  {NO_BANK_SWITCH, 0x0014c000, 0x00153fff, 1, 0xFF, "vr ", "052109"}, // 052109 Byte RAM
                                                  {NO_BANK_SWITCH, 0x0014c001, 0x00153fff, 1, 0xFF, "vr ", "052109"}, // 052109 Byte RAM
                                                  {NO_BANK_SWITCH, 0x00180000, 0x0019ffff, 1, 0xFF, "gfx", "VRAM  "}, // Graphics Video RAM
                                                  {NO_BANK_SWITCH, 0x00180001, 0x0019ffff, 1, 0xFF, "gfx", "VRAM  "}, // Graphics Video RAM
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
                                                      {NO_BANK_SWITCH, 0x000c8003L, 0xFF, "p1 ", "IN1   "}, // Inputs - 1
                                                      {NO_BANK_SWITCH, 0x000c8005L, 0xFF, "p2 ", "IN2   "}, // Inputs - 2
                                                      {NO_BANK_SWITCH, 0x000c8007L, 0xFF, "ds3", "DSW3  "}, // Dip switches - 1
                                                      {NO_BANK_SWITCH, 0x000d0001L, 0xFF, "ds1", "DSW1  "}, // Dip switches - 1
                                                      {NO_BANK_SWITCH, 0x000d0003L, 0xFF, "ds2", "DSW2  "}, // Dip switches - 1
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                          "012", "012345"
                                                        {NO_BANK_SWITCH, 0x000e8001L, 0xFF, 0x0000, "   ", "Snd-FF"},
                                                        {NO_BANK_SWITCH, 0x000e8001L, 0x55, 0x0000, "   ", "Snd-55"},
                                                        {NO_BANK_SWITCH, 0x000e8001L, 0xAA, 0x0000, "   ", "Snd-AA"},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { {NO_CUSTOM_FUNCTION} }; // end of list



CKonamiGX945BaseGame::CKonamiGX945BaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new C68000DedicatedCpu(false); // No BR
    m_cpu->idle();

    m_interrupt = ICpu::IRQ0;
    m_interruptAutoVector = false;
}


CKonamiGX945BaseGame::~CKonamiGX945BaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// TBD.
//
PERROR
CKonamiGX945BaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}

