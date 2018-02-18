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
#include "CFroggerSoundBaseGame.h"
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
//   This is the top sound board.
//
// Z80 Compatibility Notes
//
// Notes:
// - In MAME the hardware for Frogger sound is mapped to "Konami Frogger with 1 x AY-8910A" as "frogger_sound_map".
//
// - AY-3-8910 Port Address Mask (from MAME)
//   0x40 - (35) Addr Write
//   0x80 - (35) Data Read/Write
//
// - Filter Address Mask (from schematics)
//   0x040 - CHA F0
//   0x080 - CHA F1
//   0x100 - CHB F0
//   0x200 - CHB F1
//   0x400 - CHC F0
//   0x800 - CHC F1


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0x4000, 0x43FF, 1, 0x0F, " 30", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x4000, 0x43FF, 1, 0xF0, " 31", "Prog. "}, // "Program RAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x4000,   0x43FF,   1, 0xFF, "301", "Prog. "}, // "Program RAM, 2114, 1K/1G"
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
                                                      {NO_BANK_SWITCH, 0x10040L, 0xFF, " 35", "AY  Rd"},
                                                      {0}
                                                    }; // end of list

//
// AY-3-8910 channel & filter address map
//
static const UINT32 AY_FLT_OFF         = 0x06000L;
static const UINT32 AY_FLT_35_CHA_AV6  = 0x06040L;
static const UINT32 AY_FLT_35_CHA_AV7  = 0x06080L;
static const UINT32 AY_FLT_35_CHB_AV8  = 0x06100L;
static const UINT32 AY_FLT_35_CHB_AV9  = 0x06200L;
static const UINT32 AY_FLT_35_CHC_AV10 = 0x06400L;
static const UINT32 AY_FLT_35_CHC_AV11 = 0x06800L;

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                               "012", "012345"
                                                        {NO_BANK_SWITCH, 0x10040L,           0xFF, 0x00, " 35", "AY  Wr"},
                                                        {NO_BANK_SWITCH, 0x10080L,           0x0F, 0x00, " 35", "AY  Ad"},
                                                        {NO_BANK_SWITCH, AY_FLT_OFF,         0xFF, 0x00, " --", "F Clr "},
                                                        {NO_BANK_SWITCH, AY_FLT_35_CHA_AV6,  0xFF, 0x00, " 18", "F AV6 "},
                                                        {NO_BANK_SWITCH, AY_FLT_35_CHA_AV7,  0xFF, 0x00, " 18", "F AV7 "},
                                                        {NO_BANK_SWITCH, AY_FLT_35_CHB_AV8,  0xFF, 0x00, " 19", "F AV8 "},
                                                        {NO_BANK_SWITCH, AY_FLT_35_CHB_AV9,  0xFF, 0x00, " 19", "F AV9 "},
                                                        {NO_BANK_SWITCH, AY_FLT_35_CHC_AV10, 0xFF, 0x00, " 20", "F AV10"},
                                                        {NO_BANK_SWITCH, AY_FLT_35_CHC_AV11, 0xFF, 0x00, " 20", "F AV11"},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                    "0123456789"
                                                            {CFroggerSoundBaseGame::ayIdle,       "AY Idle   "},
                                                            {CFroggerSoundBaseGame::ayCheck,      "AY Check  "},
                                                            {CFroggerSoundBaseGame::ay_35_ChA,    "AY 35 CHA "},
                                                            {CFroggerSoundBaseGame::ay_35_ChB,    "AY 35 CHB "},
                                                            {CFroggerSoundBaseGame::ay_35_ChC,    "AY 35 CHC "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CFroggerSoundBaseGame::CFroggerSoundBaseGame(
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

    // AY #0 3D
    m_ay = new CAY38910(m_cpu, 0x10080L, 0x10040L);

}


CFroggerSoundBaseGame::~CFroggerSoundBaseGame(
)
{
    delete m_ay;
    m_ay = (CAY38910 *) NULL;

    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// Interrupt comes from the main board.
PERROR
CFroggerSoundBaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}


// Idle (quiet) the AY-3-8910's.
PERROR
CFroggerSoundBaseGame::ayIdle(
    void *cFroggerSoundBaseGame
)
{
    CFroggerSoundBaseGame *pThis = (CFroggerSoundBaseGame *) cFroggerSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay->idle();

    return error;
}


// Check (test) the AY-3-8910's.
PERROR
CFroggerSoundBaseGame::ayCheck(
    void *cFroggerSoundBaseGame
)
{
    CFroggerSoundBaseGame *pThis = (CFroggerSoundBaseGame *) cFroggerSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay->check();

    return error;
}


PERROR
CFroggerSoundBaseGame::ay_35_ChA(
    void *cFroggerSoundBaseGame
)
{
    CFroggerSoundBaseGame *pThis = (CFroggerSoundBaseGame *) cFroggerSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay, CAY38910::CHA, AY_FLT_35_CHA_AV6, AY_FLT_35_CHA_AV7);
}


PERROR
CFroggerSoundBaseGame::ay_35_ChB(
    void *cFroggerSoundBaseGame
)
{
    CFroggerSoundBaseGame *pThis = (CFroggerSoundBaseGame *) cFroggerSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay, CAY38910::CHB, AY_FLT_35_CHB_AV8, AY_FLT_35_CHB_AV9);
}


PERROR
CFroggerSoundBaseGame::ay_35_ChC(
    void *cFroggerSoundBaseGame
)
{
    CFroggerSoundBaseGame *pThis = (CFroggerSoundBaseGame *) cFroggerSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay, CAY38910::CHC, AY_FLT_35_CHC_AV10, AY_FLT_35_CHC_AV11);
}


// Generate noise filter on the AY's.
PERROR
CFroggerSoundBaseGame::ayNoiseFilter(
    CAY38910 *ay,
    CAY38910::Channel channel,
    UINT32 filterAd1,
    UINT32 filterAd2
)
{
    static const UINT32 holdTimeInMs = 2000;
    PERROR error = errorSuccess;

    error = ay->noise(channel);
    if (SUCCESS(error))
    {
        delay(holdTimeInMs);
        error = m_cpu->memoryWrite(filterAd1, 0);
    }
    if (SUCCESS(error))
    {
        delay(holdTimeInMs);
        error = m_cpu->memoryWrite(filterAd2, 0);
    }
    if (SUCCESS(error))
    {
        delay(holdTimeInMs);
        error = m_cpu->memoryWrite(AY_FLT_OFF, 0);
    }
    if (SUCCESS(error))
    {
        error = ay->idle();
    }

    return error;
}

