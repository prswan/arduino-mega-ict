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
#include "CKonamiTwin16BaseGame.h"
#include "C68000DedicatedCpu.h"

//
// Probe Head GND:
//   The 68000 specific probe head must be used.
//   The CLK rate is likely ~9MHz (111ns per cycle)
//
// Watchdog Disable:
//   Pin 3 of the custom SIL 005924 at 1D (near the audio heatsink) is grounded.
//   It's routed to a pair of pads near the stereo connecter jumper
//   where a 2-pin header can be fitted to allow a jumper link.
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
//   Also note that with CPU A being probed with the tester, CPU B is still running unless it's
//   BR is also tied off.
//


//
// RAM region is the same for all games on this board set.
// All RAM is byte-wide.
//
// xx1 = LDS
// xx0 = UDS
//
// No schematics for this board so these are MAME designations.
// This is CPU A at 4J, the one closest to the JAMMA connector.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                "012", "012345"
                                                  {NO_BANK_SWITCH, 0x00040000, 0x00043fff, 1, 0xFF, "shr", "Share "}, // Shared RAM
                                                  {NO_BANK_SWITCH, 0x00040001, 0x00043fff, 1, 0xFF, "shr", "Share "}, // Shared RAM
                                                  {NO_BANK_SWITCH, 0x00060000, 0x00063fff, 1, 0xFF, "prg", "Prog  "}, // Program RAM
                                                  {NO_BANK_SWITCH, 0x00060001, 0x00063fff, 1, 0xFF, "prg", "Prog  "}, // Program RAM
                                                  {NO_BANK_SWITCH, 0x00080001, 0x00080fff, 1, 0xFF, "plt", "Pallet"}, // Pallete RAM
                                                  {NO_BANK_SWITCH, 0x00100000, 0x00103fff, 1, 0xFF, "fix", "Fixed "}, // Fixed RAM
                                                  {NO_BANK_SWITCH, 0x00100001, 0x00103fff, 1, 0xFF, "fix", "Fixed "}, // Fixed RAM
                                                  {NO_BANK_SWITCH, 0x00120000, 0x00121fff, 1, 0xFF, "vr0", "VRAM0 "}, // Video RAM 0
                                                  {NO_BANK_SWITCH, 0x00120001, 0x00121fff, 1, 0xFF, "vr0", "VRAM0 "}, // Video RAM 0
                                                  {NO_BANK_SWITCH, 0x00122000, 0x00123fff, 1, 0xFF, "vr1", "VRAM1 "}, // Video RAM 1
                                                  {NO_BANK_SWITCH, 0x00122001, 0x00123fff, 1, 0xFF, "vr1", "VRAM1 "}, // Video RAM 1
                                                  {NO_BANK_SWITCH, 0x00140000, 0x00143fff, 1, 0xFF, "spr", "Sprite"}, // Sprite RAM
                                                  {NO_BANK_SWITCH, 0x00140001, 0x00143fff, 1, 0xFF, "spr", "Sprite"}, // Sprite RAM
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
                                                      {NO_BANK_SWITCH, 0x000a0003L, 0xFF, "p1 ", "P1    "}, // Inputs
                                                      {NO_BANK_SWITCH, 0x000a0005L, 0xFF, "p2 ", "P2    "}, // Inputs
                                                      {NO_BANK_SWITCH, 0x000a0007L, 0xFF, "p3 ", "P3    "}, // Inputs
                                                      {NO_BANK_SWITCH, 0x000a0011L, 0xFF, "ds2", "DSW2  "}, // Dip switches
                                                      {NO_BANK_SWITCH, 0x000a0013L, 0xFF, "ds1", "DSW1  "}, // Dip switches
                                                      {NO_BANK_SWITCH, 0x000a0019L, 0xFF, "ds3", "DWS3  "}, // Dip switches
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                          "012", "012345"
                                                        {NO_BANK_SWITCH, 0x000a0009L, 0xFF, 0x0000, "   ", "Snd-FF"},
                                                        {NO_BANK_SWITCH, 0x000a0009L, 0x55, 0x0000, "   ", "Snd-55"},
                                                        {NO_BANK_SWITCH, 0x000a0009L, 0xAA, 0x0000, "   ", "Snd-AA"},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { {NO_CUSTOM_FUNCTION} }; // end of list



CKonamiTwin16BaseGame::CKonamiTwin16BaseGame(
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


CKonamiTwin16BaseGame::~CKonamiTwin16BaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// TBD.
//
PERROR
CKonamiTwin16BaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}

