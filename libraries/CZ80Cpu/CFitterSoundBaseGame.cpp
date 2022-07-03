//
// Copyright (c) 2022, Paul R. Swan
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
#include "CFitterSoundBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   None
//
// Board Designations:
//   This is the top main & sound board.
//
// Z80 Compatibility Notes
//
// Notes:
//
// - AY-3-8910 Port Address Mask (from MAME)
//   0x8e - (8) Addr Write
//   0x8f - (8) Data Read/Write
//
// NOTE: Unless the game is in play mode the audio output amp is muted.
//       Coin and start to enable audio output.


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0x3800, 0x3bFF, 1, 0x0F, " 20", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x3800, 0x3bFF, 1, 0xF0, " 19", "Prog. "}, // "Program RAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x3800,   0x3bFF,   1, 0xFF, "219", "Prog. "}, // "Program RAM, 2114, 1K/1G"
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
                                                      {NO_BANK_SWITCH, 0x1008fL, 0xFF, "  8", "AY  Rd"},
                                                      {0}
                                                    }; // end of list


//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                               "012", "012345"
                                                        {NO_BANK_SWITCH, 0x10000L,           0xFF, 0x00, "  9", "SndClr"},
                                                        {NO_BANK_SWITCH, 0x1008eL,           0x0F, 0x00, "  8", "AY  Ad"},
                                                        {NO_BANK_SWITCH, 0x1008fL,           0xFF, 0x00, "  8", "AY  Wr"},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                  "0123456789"
                                                            {CFitterSoundBaseGame::ayIdle,      "AY Idle   "},
                                                            {CFitterSoundBaseGame::ayCheck,     "AY Check  "},
                                                            {CFitterSoundBaseGame::ay_8_ChA,    "AY 8 CHA  "},
                                                            {CFitterSoundBaseGame::ay_8_ChB,    "AY 8 CHB  "},
                                                            {CFitterSoundBaseGame::ay_8_ChC,    "AY 8 CHC  "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CFitterSoundBaseGame::CFitterSoundBaseGame(
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

    // The sound commandinterrupt is on the INT pin.
    m_interrupt = ICpu::IRQ0;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;

    // AY #0 8
    m_ay = new CAY38910(m_cpu, 0x1008eL, 0x1008fL);

}


CFitterSoundBaseGame::~CFitterSoundBaseGame(
)
{
    delete m_ay;
    m_ay = (CAY38910 *) NULL;

    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// Interrupt comes from the main board.
PERROR
CFitterSoundBaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}


// Idle (quiet) the AY-3-8910's.
PERROR
CFitterSoundBaseGame::ayIdle(
    void *cFitterSoundBaseGame
)
{
    CFitterSoundBaseGame *pThis = (CFitterSoundBaseGame *) cFitterSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay->idle();

    return error;
}


// Check (test) the AY-3-8910's.
PERROR
CFitterSoundBaseGame::ayCheck(
    void *cFitterSoundBaseGame
)
{
    CFitterSoundBaseGame *pThis = (CFitterSoundBaseGame *) cFitterSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay->check();

    return error;
}


PERROR
CFitterSoundBaseGame::ay_8_ChA(
    void *cFitterSoundBaseGame
)
{
    CFitterSoundBaseGame *pThis = (CFitterSoundBaseGame *) cFitterSoundBaseGame;
    return pThis->ayNoise(pThis->m_ay, CAY38910::CHA);
}


PERROR
CFitterSoundBaseGame::ay_8_ChB(
    void *cFitterSoundBaseGame
)
{
    CFitterSoundBaseGame *pThis = (CFitterSoundBaseGame *) cFitterSoundBaseGame;
    return pThis->ayNoise(pThis->m_ay, CAY38910::CHB);
}


PERROR
CFitterSoundBaseGame::ay_8_ChC(
    void *cFitterSoundBaseGame
)
{
    CFitterSoundBaseGame *pThis = (CFitterSoundBaseGame *) cFitterSoundBaseGame;
    return pThis->ayNoise(pThis->m_ay, CAY38910::CHC);
}


// Generate noise filter on the AY's.
PERROR
CFitterSoundBaseGame::ayNoise(
    CAY38910 *ay,
    CAY38910::Channel channel
)
{
    static const UINT32 holdTimeInMs = 2000;
    PERROR error = errorSuccess;

    error = ay->noise(channel);
    if (SUCCESS(error))
    {
        delay(holdTimeInMs);
        error = ay->idle();
    }

    return error;
}

