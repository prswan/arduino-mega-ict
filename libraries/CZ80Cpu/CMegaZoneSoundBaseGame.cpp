//
// Copyright (c) 2019, Paul R. Swan
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
#include "CMegaZoneSoundBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   CPU (top) PCB J1, IC DE13 (LS00) pin 12
//
//   There are two watchdog circuits, one kicked by the Konami-1
//   and the other kicked by the sound Z80. If the sound Z80
//   isn't booting then it'll look like the game doesn't boot
//   because the sound watchdog kills the Konami-1 before
//   it can display anything. In this case when the watchdog
//   is disabled the game boots with self test failure
//   "SOUND ROM BAD"
//
// Board Designations:
//   This is the top sound board.
//
// Z80 Compatibility Notes
//   *************************************************************************
//   NOTE: THE MEGA ZONE SOUND PROJECT WAS ABANDONED DUE TO THE POOR STATE
//         OF THE TARGET GAME PCB. BASIC ROM CRC VERIFICATION WAS NOT
//         RELIABLE.
//   *************************************************************************
//
// Notes:
// - The shared RAM may not test correctly because the Konami-1 is using it. To
//   test it reliably from the Z80 the Konami-1 IC FG12 can be removed to
//   isolate it.
//

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0xE000, 0xE7FF, 1, 0xFF, "G8 ", "Prog.S"}, // Program RAM, 2128
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
                                                      {NO_BANK_SWITCH, 0x10020L, 0xFF, "B8 ", "AY  Rd"},
                                                      {NO_BANK_SWITCH, 0x06000L, 0xFF, "E4 ", "IN0 Cn"},
                                                      {NO_BANK_SWITCH, 0x06001L, 0xFF, "E4 ", "IN1 P1"},
                                                      {NO_BANK_SWITCH, 0x06002L, 0xFF, "E4 ", "IN2 P2"},
                                                      {NO_BANK_SWITCH, 0x08000L, 0xFF, "G1 ", "DSW2  "},
                                                      {NO_BANK_SWITCH, 0x08001L, 0xFF, "G1 ", "DSW1  "},
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x10000L, 0x0F, 0x00, "B8 ", "AY  Ad"},
                                                        {NO_BANK_SWITCH, 0x10020L, 0xFF, 0x00, "B8 ", "AY  Wr"},
                                                        {NO_BANK_SWITCH, 0x02000L, 0xFF, 0x00, "C10", "39 Irq"},
                                                        {NO_BANK_SWITCH, 0x04000L, 0xFF, 0x00, "D4 ", "39 Dat"},
                                                        {NO_BANK_SWITCH, 0x0A000L, 0xFF, 0x00, "C12", "K1 Irq"},
                                                        {NO_BANK_SWITCH, 0x0C000L, 0x01, 0x00, "C9 ", "IntMsk"},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                 "0123456789"
                                                            {CMegaZoneSoundBaseGame::ayIdle,   "AY Idle   "},
                                                            {CMegaZoneSoundBaseGame::ayCheck,  "AY Check  "},
                                                            {CMegaZoneSoundBaseGame::ayChA,    "AY CHA    "},
                                                            {CMegaZoneSoundBaseGame::ayChB,    "AY CHB    "},
                                                            {CMegaZoneSoundBaseGame::ayChC,    "AY CHC    "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CMegaZoneSoundBaseGame::CMegaZoneSoundBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new CZ80ACpu();
    m_cpu->idle();

    // The sound commandinterrupt is on the NMI pin.
    m_interrupt = ICpu::NMI;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;

    // AY
    m_ay = new CAY38910(m_cpu, 0x10000L, 0x10020L);

}


CMegaZoneSoundBaseGame::~CMegaZoneSoundBaseGame(
)
{
    delete m_ay;
    m_ay = (CAY38910 *) NULL;

    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// Interrupt comes from the main board.
PERROR
CMegaZoneSoundBaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}


// Idle (quiet) the AY-3-8910's.
PERROR
CMegaZoneSoundBaseGame::ayIdle(
    void *cMegaZoneSoundBaseGame
)
{
    CMegaZoneSoundBaseGame *pThis = (CMegaZoneSoundBaseGame *) cMegaZoneSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay->idle();

    return error;
}


// Check (test) the AY-3-8910's.
PERROR
CMegaZoneSoundBaseGame::ayCheck(
    void *cMegaZoneSoundBaseGame
)
{
    CMegaZoneSoundBaseGame *pThis = (CMegaZoneSoundBaseGame *) cMegaZoneSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay->check();

    return error;
}


PERROR
CMegaZoneSoundBaseGame::ayChA(
    void *cMegaZoneSoundBaseGame
)
{
    CMegaZoneSoundBaseGame *pThis = (CMegaZoneSoundBaseGame *) cMegaZoneSoundBaseGame;
    return pThis->ayNoise(pThis->m_ay, CAY38910::CHA);
}


PERROR
CMegaZoneSoundBaseGame::ayChB(
    void *cMegaZoneSoundBaseGame
)
{
    CMegaZoneSoundBaseGame *pThis = (CMegaZoneSoundBaseGame *) cMegaZoneSoundBaseGame;
    return pThis->ayNoise(pThis->m_ay, CAY38910::CHB);
}


PERROR
CMegaZoneSoundBaseGame::ayChC(
    void *cMegaZoneSoundBaseGame
)
{
    CMegaZoneSoundBaseGame *pThis = (CMegaZoneSoundBaseGame *) cMegaZoneSoundBaseGame;
    return pThis->ayNoise(pThis->m_ay, CAY38910::CHC);
}


// Generate noise on the AY.
PERROR
CMegaZoneSoundBaseGame::ayNoise(
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

