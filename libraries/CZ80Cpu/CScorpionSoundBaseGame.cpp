//
// Copyright (c) 2021, Paul R. Swan
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
#include "CScorpionSoundBaseGame.h"
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
// - Functionally, the hardware appears to be the same as Scramble with the addition of a 3rd 8910 and
//   speech. As far as I can tell from the PCB two of the 8910 have the same filter mechanism used on
//   Scramble but the 3rd 8910 does not.
//
//   Preliminary Mapping
//     IC37 == Scramble 3D == AY0
//     IC36 == Scramble 3C == AY1
//     IC35                == AY2
//
// - AY-3-8910 Port Address Mask (from MAME)
//   0x04 - #2 (35) Addr Write
//   0x08 - #2 (35) Data Read/Write
//   0x10 - #1 (36) Addr Write
//   0x20 - #1 (36) Data Read/Write
//   0x40 - #0 (37) Addr Write
//   0x80 - #0 (37) Data Read/Write
//
// - Filter Address Mask (from Scramble schematics)
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
                                                  {NO_BANK_SWITCH, 0x8000, 0x83FF, 1, 0x0F, "  6", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x8000, 0x83FF, 1, 0xF0, "  5", "Prog. "}, // "Program RAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x8000,   0x83FF,   1, 0xFF, " 65", "Prog. "}, // "Program RAM, 2114, 1K/1G"
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
                                                      {NO_BANK_SWITCH, 0x10008L, 0xFF, " 35", "AY2 Rd"},
                                                      {NO_BANK_SWITCH, 0x10020L, 0xFF, " 36", "AY1 Rd"},
                                                      {NO_BANK_SWITCH, 0x10080L, 0xFF, " 37", "AY0 Rd"},
                                                      {0}
                                                    }; // end of list

//
// AY-3-8910 channel & filter address map
//
static const UINT32 AY_FLT_OFF        = 0x09000L;
static const UINT32 AY_FLT_1_CHA_AV0  = 0x09001L;
static const UINT32 AY_FLT_1_CHA_AV1  = 0x09002L;
static const UINT32 AY_FLT_1_CHB_AV2  = 0x09004L;
static const UINT32 AY_FLT_1_CHB_AV3  = 0x09008L;
static const UINT32 AY_FLT_1_CHC_AV4  = 0x09010L;
static const UINT32 AY_FLT_1_CHC_AV5  = 0x09020L;
static const UINT32 AY_FLT_0_CHA_AV6  = 0x09040L;
static const UINT32 AY_FLT_0_CHA_AV7  = 0x09080L;
static const UINT32 AY_FLT_0_CHB_AV8  = 0x09100L;
static const UINT32 AY_FLT_0_CHB_AV9  = 0x09200L;
static const UINT32 AY_FLT_0_CHC_AV10 = 0x09400L;
static const UINT32 AY_FLT_0_CHC_AV11 = 0x09800L;

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                               "012", "012345"
                                                        {NO_BANK_SWITCH, 0x10004L,          0x0F, 0x00, " 35", "AY2 Ad"},
                                                        {NO_BANK_SWITCH, 0x10008L,          0xFF, 0x00, " 35", "AY2 Wr"},
                                                        {NO_BANK_SWITCH, 0x10010L,          0x0F, 0x00, " 36", "AY1 Ad"},
                                                        {NO_BANK_SWITCH, 0x10020L,          0xFF, 0x00, " 36", "AY1 Wr"},
                                                        {NO_BANK_SWITCH, 0x10040L,          0x0F, 0x00, " 37", "AY0 Ad"},
                                                        {NO_BANK_SWITCH, 0x10080L,          0xFF, 0x00, " 37", "AY0 Wr"},
                                                        {NO_BANK_SWITCH, AY_FLT_OFF,        0xFF, 0x00, " - ", "F Clr "},
                                                        {NO_BANK_SWITCH, AY_FLT_1_CHA_AV0,  0xFF, 0x00, " - ", "F AV0 "},
                                                        {NO_BANK_SWITCH, AY_FLT_1_CHA_AV1,  0xFF, 0x00, " - ", "F AV1 "},
                                                        {NO_BANK_SWITCH, AY_FLT_1_CHB_AV2,  0xFF, 0x00, " - ", "F AV2 "},
                                                        {NO_BANK_SWITCH, AY_FLT_1_CHB_AV3,  0xFF, 0x00, " - ", "F AV3 "},
                                                        {NO_BANK_SWITCH, AY_FLT_1_CHC_AV4,  0xFF, 0x00, " - ", "F AV4 "},
                                                        {NO_BANK_SWITCH, AY_FLT_1_CHC_AV5,  0xFF, 0x00, " - ", "F AV5 "},
                                                        {NO_BANK_SWITCH, AY_FLT_0_CHA_AV6,  0xFF, 0x00, " - ", "F AV6 "},
                                                        {NO_BANK_SWITCH, AY_FLT_0_CHA_AV7,  0xFF, 0x00, " - ", "F AV7 "},
                                                        {NO_BANK_SWITCH, AY_FLT_0_CHB_AV8,  0xFF, 0x00, " - ", "F AV8 "},
                                                        {NO_BANK_SWITCH, AY_FLT_0_CHB_AV9,  0xFF, 0x00, " - ", "F AV9 "},
                                                        {NO_BANK_SWITCH, AY_FLT_0_CHC_AV10, 0xFF, 0x00, " - ", "F AV10"},
                                                        {NO_BANK_SWITCH, AY_FLT_0_CHC_AV11, 0xFF, 0x00, " - ", "F AV11"},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                   "0123456789"
                                                            {CScorpionSoundBaseGame::ayIdle,     "AY Idle   "},
                                                            {CScorpionSoundBaseGame::ayCheckAll, "AY Chk All"},
                                                            {CScorpionSoundBaseGame::ayCheck0,   "AY Chk 37 "},
                                                            {CScorpionSoundBaseGame::ayCheck1,   "AY Chk 36 "},
                                                            {CScorpionSoundBaseGame::ayCheck2,   "AY Chk 35 "},
                                                            {CScorpionSoundBaseGame::ay0_ChA,    "AY 37 CHA "},
                                                            {CScorpionSoundBaseGame::ay0_ChB,    "AY 37 CHB "},
                                                            {CScorpionSoundBaseGame::ay0_ChC,    "AY 37 CHC "},
                                                            {CScorpionSoundBaseGame::ay1_ChA,    "AY 36 CHA "},
                                                            {CScorpionSoundBaseGame::ay1_ChB,    "AY 36 CHB "},
                                                            {CScorpionSoundBaseGame::ay1_ChC,    "AY 36 CHC "},
                                                            {CScorpionSoundBaseGame::ay2_ChA,    "AY 35 CHA "},
                                                            {CScorpionSoundBaseGame::ay2_ChB,    "AY 35 CHB "},
                                                            {CScorpionSoundBaseGame::ay2_ChC,    "AY 35 CHC "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CScorpionSoundBaseGame::CScorpionSoundBaseGame(
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

    // AY #0 37
    m_ay[0] = new CAY38910(m_cpu, 0x10040L, 0x10080L);

    // AY #1 36
    m_ay[1] = new CAY38910(m_cpu, 0x10010L, 0x10020L);

    // AY #1 35
    m_ay[2] = new CAY38910(m_cpu, 0x10004L, 0x10008L);

}


CScorpionSoundBaseGame::~CScorpionSoundBaseGame(
)
{
    delete m_ay[2];
    m_ay[2] = (CAY38910 *) NULL;

    delete m_ay[1];
    m_ay[1] = (CAY38910 *) NULL;

    delete m_ay[0];
    m_ay[0] = (CAY38910 *) NULL;

    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// Interrupt comes from the main board.
PERROR
CScorpionSoundBaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}


// Idle (quiet) the AY-3-8910's.
PERROR
CScorpionSoundBaseGame::ayIdle(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay[0]->idle();
    if (SUCCESS(error))
    {
        error = pThis->m_ay[1]->idle();
    }
    if (SUCCESS(error))
    {
        error = pThis->m_ay[2]->idle();
    }

    return error;
}


// Check (test) the AY-3-8910's.
PERROR
CScorpionSoundBaseGame::ayCheckAll(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay[0]->check();
    if (SUCCESS(error))
    {
        error = pThis->m_ay[1]->check();
    }
    if (SUCCESS(error))
    {
        error = pThis->m_ay[2]->check();
    }

    return error;
}


// Check (test) the AY-3-8910's.
PERROR
CScorpionSoundBaseGame::ayCheck0(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay[0]->check();

    return error;
}


// Check (test) the AY-3-8910's.
PERROR
CScorpionSoundBaseGame::ayCheck1(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay[1]->check();

    return error;
}


// Check (test) the AY-3-8910's.
PERROR
CScorpionSoundBaseGame::ayCheck2(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay[2]->check();

    return error;
}


PERROR
CScorpionSoundBaseGame::ay0_ChA(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay[0], CAY38910::CHA, AY_FLT_0_CHA_AV6, AY_FLT_0_CHA_AV7);
}


PERROR
CScorpionSoundBaseGame::ay0_ChB(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay[0], CAY38910::CHB, AY_FLT_0_CHB_AV8, AY_FLT_0_CHB_AV9);
}


PERROR
CScorpionSoundBaseGame::ay0_ChC(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay[0], CAY38910::CHC, AY_FLT_0_CHC_AV10, AY_FLT_0_CHC_AV11);
}


PERROR
CScorpionSoundBaseGame::ay1_ChA(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay[1], CAY38910::CHA, AY_FLT_1_CHA_AV0, AY_FLT_1_CHA_AV1);
}


PERROR
CScorpionSoundBaseGame::ay1_ChB(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay[1], CAY38910::CHB, AY_FLT_1_CHB_AV2, AY_FLT_1_CHB_AV3);
}


PERROR
CScorpionSoundBaseGame::ay1_ChC(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    return pThis->ayNoiseFilter(pThis->m_ay[1], CAY38910::CHC, AY_FLT_1_CHC_AV4, AY_FLT_1_CHC_AV5);
}

PERROR
CScorpionSoundBaseGame::ay2_ChA(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    return pThis->ayNoise(pThis->m_ay[2], CAY38910::CHA);
}


PERROR
CScorpionSoundBaseGame::ay2_ChB(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    return pThis->ayNoise(pThis->m_ay[2], CAY38910::CHB);
}


PERROR
CScorpionSoundBaseGame::ay2_ChC(
    void *cScorpionSoundBaseGame
)
{
    CScorpionSoundBaseGame *pThis = (CScorpionSoundBaseGame *) cScorpionSoundBaseGame;
    return pThis->ayNoise(pThis->m_ay[2], CAY38910::CHC);
}

// Generate noise on the AY's.
PERROR
CScorpionSoundBaseGame::ayNoise(
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
    }
    if (SUCCESS(error))
    {
        error = ay->idle();
    }

    return error;
}


// Generate noise filter on the AY's.
PERROR
CScorpionSoundBaseGame::ayNoiseFilter(
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

