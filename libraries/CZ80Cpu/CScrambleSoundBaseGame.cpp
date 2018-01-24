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
#include "CScrambleSoundBaseGame.h"
#include "CZ80Cpu.h"
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
// - In MAME the hardware for Scramble sound is mapped to "Konami generic with 2 x AY-8910A" as "konami_sound_map".
//
// - AY-3-8910 Port Address Mask (from MAME)
//   0x10 - #1 (3C) Addr Write
//   0x20 - #1 (3C) Data Read/Write
//   0x40 - #0 (3D) Addr Write
//   0x80 - #0 (3D) Data Read/Write
//
// - Filter Address Mask (from schematics)
//   0x001 - #1 CHA F0
//   0x002 - #1 CHA F1
//   0x004 - #1 CHB F0
//   0x008 - #1 CHB F1
//   0x010 - #1 CHC F0
//   0x020 - #1 CHC F1
//   0x040 - #0 CHA F0
//   0x080 - #0 CHA F1
//   0x100 - #0 CHB F0
//   0x200 - #0 CHB F1
//   0x400 - #0 CHC F0
//   0x800 - #0 CHC F1


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0x8000, 0x83FF, 1, 0x0F, " 6C", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x8000, 0x83FF, 1, 0xF0, " 6D", "Prog. "}, // "Program RAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x8000,   0x83FF,   1, 0xFF, "6CD", "Prog. "}, // "Program RAM, 2114, 1K/1G"
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
                                                      {NO_BANK_SWITCH, 0x10020L, 0xFF, " 3C", "AY1 Rd"},
                                                      {NO_BANK_SWITCH, 0x10080L, 0xFF, " 3D", "AY0 Rd"},
                                                      {0}
                                                    }; // end of list

//
// AY-3-8910 channel & filter address map
//
static const UINT32 AY_FLT_OFF         = 0x09000L;
static const UINT32 AY_FLT_3C_CHA_AV0  = 0x09001L;
static const UINT32 AY_FLT_3C_CHA_AV1  = 0x09002L;
static const UINT32 AY_FLT_3C_CHB_AV2  = 0x09004L;
static const UINT32 AY_FLT_3C_CHB_AV3  = 0x09008L;
static const UINT32 AY_FLT_3C_CHC_AV4  = 0x09010L;
static const UINT32 AY_FLT_3C_CHC_AV5  = 0x09020L;
static const UINT32 AY_FLT_3D_CHA_AV6  = 0x09040L;
static const UINT32 AY_FLT_3D_CHA_AV7  = 0x09080L;
static const UINT32 AY_FLT_3D_CHB_AV8  = 0x09100L;
static const UINT32 AY_FLT_3D_CHB_AV9  = 0x09200L;
static const UINT32 AY_FLT_3D_CHC_AV10 = 0x09400L;
static const UINT32 AY_FLT_3D_CHC_AV11 = 0x09800L;

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                               "012", "012345"
                                                        {NO_BANK_SWITCH, 0x10010L,           0x0F, 0x00, " 3C", "AY1 Ad"},
                                                        {NO_BANK_SWITCH, 0x10020L,           0xFF, 0x00, " 3C", "AY1 Wr"},
                                                        {NO_BANK_SWITCH, 0x10040L,           0x0F, 0x00, " 3D", "AY0 Ad"},
                                                        {NO_BANK_SWITCH, 0x10080L,           0xFF, 0x00, " 3D", "AY0 Wr"},
                                                        {NO_BANK_SWITCH, AY_FLT_OFF,         0xFF, 0x00, " 6E", "F Clr "},
                                                        {NO_BANK_SWITCH, AY_FLT_3C_CHA_AV0,  0xFF, 0x00, " 6E", "F AV0 "},
                                                        {NO_BANK_SWITCH, AY_FLT_3C_CHA_AV1,  0xFF, 0x00, " 6E", "F AV1 "},
                                                        {NO_BANK_SWITCH, AY_FLT_3C_CHB_AV2,  0xFF, 0x00, " 7E", "F AV2 "},
                                                        {NO_BANK_SWITCH, AY_FLT_3C_CHB_AV3,  0xFF, 0x00, " 7E", "F AV3 "},
                                                        {NO_BANK_SWITCH, AY_FLT_3C_CHC_AV4,  0xFF, 0x00, " 6F", "F AV4 "},
                                                        {NO_BANK_SWITCH, AY_FLT_3C_CHC_AV5,  0xFF, 0x00, " 6F", "F AV5 "},
                                                        {NO_BANK_SWITCH, AY_FLT_3D_CHA_AV6,  0xFF, 0x00, " 7F", "F AV6 "},
                                                        {NO_BANK_SWITCH, AY_FLT_3D_CHA_AV7,  0xFF, 0x00, " 7F", "F AV7 "},
                                                        {NO_BANK_SWITCH, AY_FLT_3D_CHB_AV8,  0xFF, 0x00, " 6G", "F AV8 "},
                                                        {NO_BANK_SWITCH, AY_FLT_3D_CHB_AV9,  0xFF, 0x00, " 6G", "F AV9 "},
                                                        {NO_BANK_SWITCH, AY_FLT_3D_CHC_AV10, 0xFF, 0x00, " 6H", "F AV10"},
                                                        {NO_BANK_SWITCH, AY_FLT_3D_CHC_AV11, 0xFF, 0x00, " 6H", "F AV11"},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                    "0123456789"
                                                            {CScrambleSoundBaseGame::ayIdle,      "AY Idle   "},
                                                            {CScrambleSoundBaseGame::ayCheck,     "AY Check  "},
                                                            {CScrambleSoundBaseGame::ay_3D_ChA,   "AY 3D CHA "},
                                                            {CScrambleSoundBaseGame::ay_3D_ChB,   "AY 3D CHB "},
                                                            {CScrambleSoundBaseGame::ay_3D_ChC,   "AY 3D CHC "},
                                                            {CScrambleSoundBaseGame::ay_3C_ChA,   "AY 3C CHA "},
                                                            {CScrambleSoundBaseGame::ay_3C_ChB,   "AY 3C CHB "},
                                                            {CScrambleSoundBaseGame::ay_3C_ChC,   "AY 3C CHC "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CScrambleSoundBaseGame::CScrambleSoundBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new CZ80Cpu();
    m_cpu->idle();

    // The sound commandinterrupt is on the INT pin.
    m_interrupt = ICpu::IRQ0;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;

    // AY #0 3D
    m_ay[0] = new CAY38910(m_cpu, 0x10040L, 0x10080L);

    // AY #1 3C
    m_ay[1] = new CAY38910(m_cpu, 0x10010L, 0x10020L);

}


CScrambleSoundBaseGame::~CScrambleSoundBaseGame(
)
{
    delete m_ay[1];
    m_ay[1] = (CAY38910 *) NULL;

    delete m_ay[0];
    m_ay[0] = (CAY38910 *) NULL;

    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// Interrupt comes from the main board.
PERROR
CScrambleSoundBaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}


// Idle (quiet) the AY-3-8910's.
PERROR
CScrambleSoundBaseGame::ayIdle(
    void *cScrambleSoundBaseGame
)
{
    CScrambleSoundBaseGame *pThis = (CScrambleSoundBaseGame *) cScrambleSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay[0]->idle();
    if (SUCCESS(error))
    {
        error = pThis->m_ay[1]->idle();
    }

    return error;
}


// Check (test) the AY-3-8910's.
PERROR
CScrambleSoundBaseGame::ayCheck(
    void *cScrambleSoundBaseGame
)
{
    CScrambleSoundBaseGame *pThis = (CScrambleSoundBaseGame *) cScrambleSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay[0]->check();
    if (SUCCESS(error))
    {
        error = pThis->m_ay[1]->check();
    }

    return error;
}


PERROR
CScrambleSoundBaseGame::ay_3D_ChA(
    void *cScrambleSoundBaseGame
)
{
    CScrambleSoundBaseGame *pThis = (CScrambleSoundBaseGame *) cScrambleSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay[0], CAY38910::CHA, AY_FLT_3D_CHA_AV6, AY_FLT_3D_CHA_AV7);
}


PERROR
CScrambleSoundBaseGame::ay_3D_ChB(
    void *cScrambleSoundBaseGame
)
{
    CScrambleSoundBaseGame *pThis = (CScrambleSoundBaseGame *) cScrambleSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay[0], CAY38910::CHB, AY_FLT_3D_CHB_AV8, AY_FLT_3D_CHB_AV9);
}


PERROR
CScrambleSoundBaseGame::ay_3D_ChC(
    void *cScrambleSoundBaseGame
)
{
    CScrambleSoundBaseGame *pThis = (CScrambleSoundBaseGame *) cScrambleSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay[0], CAY38910::CHC, AY_FLT_3D_CHC_AV10, AY_FLT_3D_CHC_AV11);
}


PERROR
CScrambleSoundBaseGame::ay_3C_ChA(
    void *cScrambleSoundBaseGame
)
{
    CScrambleSoundBaseGame *pThis = (CScrambleSoundBaseGame *) cScrambleSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay[1], CAY38910::CHA, AY_FLT_3C_CHA_AV0, AY_FLT_3C_CHA_AV1);
}


PERROR
CScrambleSoundBaseGame::ay_3C_ChB(
    void *cScrambleSoundBaseGame
)
{
    CScrambleSoundBaseGame *pThis = (CScrambleSoundBaseGame *) cScrambleSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay[1], CAY38910::CHB, AY_FLT_3C_CHB_AV2, AY_FLT_3C_CHB_AV3);
}


PERROR
CScrambleSoundBaseGame::ay_3C_ChC(
    void *cScrambleSoundBaseGame
)
{
    CScrambleSoundBaseGame *pThis = (CScrambleSoundBaseGame *) cScrambleSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay[1], CAY38910::CHC, AY_FLT_3C_CHC_AV4, AY_FLT_3C_CHC_AV5);
}


// Generate noise filter on the AY's.
PERROR
CScrambleSoundBaseGame::ayNoiseFilter(
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

