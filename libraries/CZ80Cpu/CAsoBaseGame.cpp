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
#include "CAsoBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   There doesn't appear to be a watchdog.
//
// Board Designations:
//   c - CPU board
//   v - Video board
//
// Z80 Compatibility Notes
//   Neither CPU uses WAIT for VRAM access synchronization. A clock synchronous based method is utilized.
//   The Z80A CPU implementation is able to hit this timing and is tuned for it.
//
//   CPU A - This one doesn't use WAIT at all (tied to Vcc).
//   CPU B - This one uses WAIT synchronization to wait for CPU A accesses to complete.
//

//
// RAM region is the same for all games on this board set.
// CPU A
//
static const RAM_REGION s_ramRegionA[] PROGMEM = { //                                            "012", "012345"
                                                   {NO_BANK_SWITCH, 0x00D800, 0x00DFFF, 1, 0xFF, "PR ", "Prog. "}, // "Program RAM"
                                                   {NO_BANK_SWITCH, 0x00E000, 0x00E7FF, 1, 0xFF, "SP ", "SP RAM"}, // "Sprite RAM"
                                                   {NO_BANK_SWITCH, 0x00E800, 0x00F7FF, 1, 0xFF, "BG ", "BG RAM"}, // "Background RAM"
                                                   {NO_BANK_SWITCH, 0x00F800, 0x00FFFF, 1, 0xFF, "TX ", "TX RAM"}, // "TX RAM"
                                                   {0}
                                                 }; // end of list

//
// RAM region is the same for all games on this board set.
// CPU B
//
static const RAM_REGION s_ramRegionB[] PROGMEM = { //                                            "012", "012345"
                                                   {NO_BANK_SWITCH, 0x00C800, 0x00CFFF, 1, 0xFF, "PR ", "Prog. "}, // "Program RAM"
                                                   {NO_BANK_SWITCH, 0x00D000, 0x00D7FF, 1, 0xFF, "SP ", "SP RAM"}, // "Sprite RAM"
                                                   {NO_BANK_SWITCH, 0x00D800, 0x00E7FF, 1, 0xFF, "BG ", "BG RAM"}, // "Background RAM"
                                                   {NO_BANK_SWITCH, 0x00F800, 0x00FFFF, 1, 0xFF, "TX ", "TX RAM"}, // "TX RAM"
                                                   {0}
                                                 }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { // "012", "012345"
                                                           {0}
                                                         }; // end of list

//
// Input region is the same for all games on this board set.
// CPU A
//
static const INPUT_REGION s_inputRegionA[] PROGMEM = { //                              "012", "012345"
                                                       {NO_BANK_SWITCH, 0xC000L, 0xFF, "   ", "IN0   "}, //
                                                       {NO_BANK_SWITCH, 0xC100L, 0xFF, "   ", "IN1   "}, //
                                                       {NO_BANK_SWITCH, 0xC200L, 0xFF, "   ", "IN2   "}, //
                                                       {NO_BANK_SWITCH, 0xC500L, 0xFF, "   ", "DSW1  "}, //
                                                       {NO_BANK_SWITCH, 0xC600L, 0xFF, "   ", "DSW2  "}, //
                                                       {NO_BANK_SWITCH, 0xC000L, 0xFF, "   ", "B NMI "}, // CPU B NMI Trigger
                                                       {0}
                                                     }; // end of list

//
// Output region is the same for all versions on this board set.
// CPU A
//
static const OUTPUT_REGION s_outputRegionA[] PROGMEM = { //                                     "012", "012345"
                                                         {NO_BANK_SWITCH, 0xC400L, 0xFF, 0x00,  "   ", "Snd   "}, // Sound Latch
                                                         {NO_BANK_SWITCH, 0xC700L, 0xFF, 0x00,  "   ", "B Ack "}, // CPU B NMI Ack.
                                                         {NO_BANK_SWITCH, 0xC800L, 0xFF, 0x00,  "   ", "V Attr"}, // Video attributes
                                                         {NO_BANK_SWITCH, 0xC900L, 0xFF, 0x00,  "   ", "Scr SY"}, // Scroll Sprite Y
                                                         {NO_BANK_SWITCH, 0xCA00L, 0xFF, 0x00,  "   ", "Scr SX"}, // Scroll Sprite X
                                                         {NO_BANK_SWITCH, 0xCB00L, 0xFF, 0x00,  "   ", "Scr BY"}, // Scroll Background Y
                                                         {NO_BANK_SWITCH, 0xCC00L, 0xFF, 0x00,  "   ", "Scr BX"}, // Scroll Background X
                                                         {NO_BANK_SWITCH, 0xCF00L, 0xFF, 0x00,  "   ", "Bank B"}, // Background Bank Switch
                                                         {0}
                                                       }; // end of list

//
// Input region is the same for all games on this board set.
// CPU B
//
static const INPUT_REGION s_inputRegionB[] PROGMEM = { //                              "012", "012345"
                                                       {NO_BANK_SWITCH, 0xC000L, 0xFF, "   ", "A NMI "}, // CPU A NMI Trigger
                                                       {0}
                                                     }; // end of list

//
// Output region is the same for all versions on this board set.
// CPU B
//
static const OUTPUT_REGION s_outputRegionB[] PROGMEM = { //                                     "012", "012345"
                                                         {NO_BANK_SWITCH, 0xC000L, 0xFF, 0x00,  "   ", "A Ack "}, // CPU A NMI Ack
                                                         {0}
                                                       }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CAsoABaseGame::CAsoABaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegionA,
           s_ramRegionA, //ByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegionA,
           s_outputRegionA,
           s_customFunction)
{
    m_cpu = new CZ80ACpu();
    m_cpu->idle();

    // The VBLANK interrupt is on the IRQ pin.
    // NMI is used for interprocessor communication.
    m_interrupt = ICpu::IRQ0;

    // Vector TBD
    m_interruptAutoVector = true;
}


CAsoABaseGame::~CAsoABaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// TBD
PERROR
CAsoABaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}


CAsoBBaseGame::CAsoBBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegionB,
           s_ramRegionB, //ByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegionB,
           s_outputRegionB,
           s_customFunction)
{
    m_cpu = new CZ80ACpu();
    m_cpu->idle();

    // The VBLANK interrupt is on the IRQ pin.
    // NMI is used for interprocessor communication.
    m_interrupt = ICpu::IRQ0;

    // Vector TBD
    m_interruptAutoVector = true;
}


CAsoBBaseGame::~CAsoBBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// TBD
PERROR
CAsoBBaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}

