//
// Copyright (c) 2020, Paul R. Swan
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
#include "CGyrussSoundBaseGame.h"
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
//   The CPU clock edge detection seems not to work. The clock edge isn't
//   being detected but the clock is running hi/lo OK.
//
// Notes:
//

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0x6000, 0x63FF, 1, 0x0F, " A4", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x6000, 0x63FF, 1, 0xF0, " A5", "Prog. "}, // "Program RAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x6000,   0x63FF,   1, 0xFF, "A45", "Prog. "}, // "Program RAM, 2114, 1K/1G"
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
                                                      {NO_BANK_SWITCH,  0x8000L, 0xFF, " C5", "SndCmd"},
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                               "012", "012345"
                                                        {NO_BANK_SWITCH, 0x10014L,           0xFF, 0x00, " D5", "8039-I"},
                                                        {NO_BANK_SWITCH, 0x10018L,           0xFF, 0x00, "H12", "8039-D"},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                  "0123456789"
                                                            {CGyrussSoundBaseGame::ayIdle,      "AY Idle   "},
                                                            {CGyrussSoundBaseGame::ayCheck,     "AY Check  "},
                                                            {CGyrussSoundBaseGame::ayNoise,     "AY Noise  "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CGyrussSoundBaseGame::CGyrussSoundBaseGame(
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
    // Gets stuck on the cycle sync so use Crazy Kong that skips it
    m_cpu = new CZ80ACpu(0,
                         NO_ADDRESS_REMAP,
                         NULL,
                         NO_DATA_REMAP,
                         NULL,
                         CZ80ACpu::CYCLE_TYPE_CRAZYKONG);

    m_cpu->idle();

    // The sound commandinterrupt is on the INT pin.
    m_interrupt = ICpu::IRQ0;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;

    // AY #0 11D
    m_ay[0] = new CAY38910(m_cpu, 0x10000L, 0x10001L, 0x10002L);

    // AY #1 12D
    m_ay[1] = new CAY38910(m_cpu, 0x10004L, 0x10005L, 0x10006L);

    // AY #2 10B
    m_ay[2] = new CAY38910(m_cpu, 0x10008L, 0x10009L, 0x1000AL);

    // AY #3  9B
    m_ay[3] = new CAY38910(m_cpu, 0x1000CL, 0x1000DL, 0x1000EL);

    // AY #4  8B
    m_ay[4] = new CAY38910(m_cpu, 0x10010L, 0x10011L, 0x10012L);

}


CGyrussSoundBaseGame::~CGyrussSoundBaseGame(
)
{
    delete m_ay[4];
    m_ay[4] = (CAY38910 *) NULL;

    delete m_ay[3];
    m_ay[3] = (CAY38910 *) NULL;

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
CGyrussSoundBaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}

// Idle (quiet) the AY-3-8910's.
PERROR
CGyrussSoundBaseGame::ayIdle(
    void *cGyrussSoundBaseGame
)
{
    CGyrussSoundBaseGame *pThis = (CGyrussSoundBaseGame *) cGyrussSoundBaseGame;
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
    if (SUCCESS(error))
    {
        error = pThis->m_ay[3]->idle();
    }
    if (SUCCESS(error))
    {
        error = pThis->m_ay[4]->idle();
    }

    return error;
}


// Check (test) the AY-3-8910's.
PERROR
CGyrussSoundBaseGame::ayCheck(
    void *cGyrussSoundBaseGame
)
{
    CGyrussSoundBaseGame *pThis = (CGyrussSoundBaseGame *) cGyrussSoundBaseGame;
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
    if (SUCCESS(error))
    {
        error = pThis->m_ay[3]->check();
    }
    if (SUCCESS(error))
    {
        error = pThis->m_ay[4]->check();
    }

    return error;
}


// Noise test the AY-3-8910's.
PERROR
CGyrussSoundBaseGame::ayNoise(
    void *cGyrussSoundBaseGame
)
{
    static const UINT32 holdTimeInMs  = 2000;
    static const UINT32 pauseTimeInMs = 1000;
    CGyrussSoundBaseGame *pThis = (CGyrussSoundBaseGame *) cGyrussSoundBaseGame;
    PERROR error = errorSuccess;

    for (int x = 0 ; x < 5 ; x++)
    {
        error = pThis->m_ay[x]->noise(CAY38910::CHA);
        if (FAILED(error))
        {
            break;
        }
        delay(holdTimeInMs);
        pThis->m_ay[x]->idle();
        delay(pauseTimeInMs);

        error = pThis->m_ay[x]->noise(CAY38910::CHB);
        if (FAILED(error))
        {
            break;
        }
        delay(holdTimeInMs);
        pThis->m_ay[x]->idle();
        delay(pauseTimeInMs);

        error = pThis->m_ay[x]->noise(CAY38910::CHC);
        if (FAILED(error))
        {
            break;
        }
        delay(holdTimeInMs);
        pThis->m_ay[x]->idle();
        delay(pauseTimeInMs);
    }

    return error;
}

