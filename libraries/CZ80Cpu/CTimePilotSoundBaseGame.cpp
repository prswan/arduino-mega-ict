//
// Copyright (c) 2026, patzik
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
#include "CTimePilotSoundBaseGame.h"
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
//   This is the sound section, the same as Pooyan.
//
// Z80 Compatibility Notes
//   * IO space is not used.
//   * M1 (1st machine cycle) is not used.
//
// Notes:
//   The memory map is taken from MAME (src/mame/shared/timeplt_a.cpp,
//   timeplt_audio_device::timeplt_sound_map). MAME doesn't document the
//   IC locations so they are not given ("-").
//
//   0000-2FFF ROM (Time Pilot only uses 0000-0FFF)
//   3000-33FF RAM (mirror 0x0C00)
//   4000      AY #1 data read/write (mirror 0x0FFF)
//   5000      AY #1 address write   (mirror 0x0FFF)
//   6000      AY #2 data read/write (mirror 0x0FFF)
//   7000      AY #2 address write   (mirror 0x0FFF)
//   8000-FFFF RC filter select, A0-A11 select the filters, the data is ignored.
//
//   The sound command is read via AY #1 port A and a timer via AY #1 port B.
//   The sound command interrupt is triggered by the main CPU on the INT pin (IM 1).
//
// void timeplt_audio_device::timeplt_sound_map(address_map &map)
//

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0x3000, 0x33FF, 1, 0xFF, " - ", "Prog. "}, // Program RAM
                                                  {0}
                                                }; // end of list

//
// No write-only RAM on this platform.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                               "012", "012345"
                                                      {NO_BANK_SWITCH, 0x4000L, 0xFF, " - ", "AY1 Rd"},
                                                      {NO_BANK_SWITCH, 0x6000L, 0xFF, " - ", "AY2 Rd"},
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x5000L, 0x0F, 0x00, " - ", "AY1 Ad"},
                                                        {NO_BANK_SWITCH, 0x4000L, 0xFF, 0x00, " - ", "AY1 Wr"},
                                                        {NO_BANK_SWITCH, 0x7000L, 0x0F, 0x00, " - ", "AY2 Ad"},
                                                        {NO_BANK_SWITCH, 0x6000L, 0xFF, 0x00, " - ", "AY2 Wr"},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                    "0123456789"
                                                            {CTimePilotSoundBaseGame::ayIdle,     "AY Idle   "},
                                                            {CTimePilotSoundBaseGame::ayCheck,    "AY Check  "},
                                                            {CTimePilotSoundBaseGame::ayNoise,    "AY Noise  "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CTimePilotSoundBaseGame::CTimePilotSoundBaseGame(
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

    // The sound command interrupt is on the INT pin.
    m_interrupt = ICpu::IRQ0;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;

    // AY #1
    m_ay[0] = new CAY38910(m_cpu, 0x5000L, 0x4000L);

    // AY #2
    m_ay[1] = new CAY38910(m_cpu, 0x7000L, 0x6000L);
}


CTimePilotSoundBaseGame::~CTimePilotSoundBaseGame(
)
{
    delete m_ay[1];
    m_ay[1] = (CAY38910 *) NULL;

    delete m_ay[0];
    m_ay[0] = (CAY38910 *) NULL;

    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// The interrupt is triggered by the main CPU so it can't be tested from here.
//
PERROR
CTimePilotSoundBaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}


PERROR
CTimePilotSoundBaseGame::ayIdle(
    void *cTimePilotSoundBaseGame
)
{
    CTimePilotSoundBaseGame *pThis = (CTimePilotSoundBaseGame *) cTimePilotSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay[0]->idle();
    if (SUCCESS(error))
    {
        error = pThis->m_ay[1]->idle();
    }

    return error;
}


PERROR
CTimePilotSoundBaseGame::ayCheck(
    void *cTimePilotSoundBaseGame
)
{
    CTimePilotSoundBaseGame *pThis = (CTimePilotSoundBaseGame *) cTimePilotSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay[0]->check();
    if (SUCCESS(error))
    {
        error = pThis->m_ay[1]->check();
    }

    return error;
}


PERROR
CTimePilotSoundBaseGame::ayNoise(
    void *cTimePilotSoundBaseGame
)
{
    static const UINT32 holdTimeInMs  = 2000;
    static const UINT32 pauseTimeInMs = 1000;
    static const CAY38910::Channel channels[] = {CAY38910::CHA, CAY38910::CHB, CAY38910::CHC};
    CTimePilotSoundBaseGame *pThis = (CTimePilotSoundBaseGame *) cTimePilotSoundBaseGame;
    PERROR error = errorSuccess;

    for (int x = 0 ; (x < 2) && SUCCESS(error) ; x++)
    {
        for (int c = 0 ; c < 3 ; c++)
        {
            error = pThis->m_ay[x]->noise(channels[c]);
            if (FAILED(error))
            {
                break;
            }
            delay(holdTimeInMs);
            pThis->m_ay[x]->idle();
            delay(pauseTimeInMs);
        }
    }

    return error;
}

