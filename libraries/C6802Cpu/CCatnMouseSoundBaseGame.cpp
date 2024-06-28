//
// Copyright (c) 2024, Paul R. Swan
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
#include "CCatnMouseSoundBaseGame.h"
#include "C6802Cpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   6802 GND Pins 1 & 21
//
// Watchdog Disable:
//   Sound CPU has none.
//
// This is the primary sound CPU (1B11107) that:
// 1) Handles the main CPU sound commands
// 2) Uses a 6821 PIA to pin-drive 2 x AY-3-8910
//    The sound command inputs and filter controls hang off the AY IO ports.
//
// At the time of writing no schematics had been found but accroding to MAME it's
// very similar to the music subsystem of Money Money.
//
/*
    NOTE: From MAME but typo corrected - the address is 0x500C

    1B11107 sound CPU, produces music and sound effects
    mapping (from tracing sound program and cross-referencing 1B1142 schematic):
    A15 A14 A13 A12 A11 A10 A09 A08 A07 A06 A05 A04 A03 A02 A01 A00
     0   0   0   0   0   0   0   0   0   *   *   *   *   *   *   *  RW 6802 internal ram
     0   0   0   x   x   x   x   x   x   x   x   x   x   x   x   x  Open bus (for area that doesn't overlap RAM)
     0   0   1   x   x   x   x   x   x   x   x   x   x   x   x   x  Open bus
     0   1   0   x   x   x   x   x   x   x   x   x   0   0   x   x  Open bus
     0   1   0   x   x   x   x   x   x   x   x   x   0   1   x   x  Open bus
     0   1   0   x   x   x   x   x   x   x   x   x   1   0   x   x  Open bus
     0   1   0   1   x   x   x   x   x   x   x   x   1   1   *   *  RW 6821 PIA @ 1G
     0   1   1   x   x   x   x   x   x   x   x   x   x   x   x   x  Open bus
     1   0   x   x   x   x   x   x   x   x   x   x   x   x   x   x  Open bus
     1   1   0   0   *   *   *   *   *   *   *   *   *   *   *   *  R  Enable ROM @ 1F
     1   1   0   1   *   *   *   *   *   *   *   *   *   *   *   *  Open bus
     1   1   1   0   *   *   *   *   *   *   *   *   *   *   *   *  R  Enable ROM @ 1D
     1   1   1   1   *   *   *   *   *   *   *   *   *   *   *   *  R  Enable ROM @ 1E

    6821 PIA:
    * CA1 comes from the SOUND 5 line on the input (which may also be connected to an input on the AY chip at 1H)
    * CB1 comes from the 6802's clock divided by 4096*2 (about 437Hz)
    * PA0-7 connect to the data busses of the AY-3-8910 chips
    * PB0 and PB1 connect to the BC1 and BDIR pins of the AY chip at 1H
    * PB2 and PB3 connect to the BC1 and BDIR pins of the AY chip at 1I
*/

/* 6821 Port B Decode

       D3        D2       D1       D0
       4H_BDIR   4H_BC1   4G_BDIR  4G_BC1
       0         0        0        0         0x00 INACTIVE
       x         x        0        1         0x01 4G READ  PSG
       x         x        1        0         0x02 4G WRITE PSG
       x         x        1        1         0x03 4G WRITE ADDRESS
       0         1        x        x         0x04 4H READ  PSG
       1         0        x        x         0x08 4H WRITE PSG
       1         1        x        x         0x0C 4H WRITE ADDRESS

*/

static const UINT32 AY_4G_READ_PSG      = 0x01;
static const UINT32 AY_4G_WRITE_PSG     = 0x02;
static const UINT32 AY_4G_WRITE_ADDRESS = 0x03;

static const UINT32 AY_4H_READ_PSG      = 0x04;
static const UINT32 AY_4H_WRITE_PSG     = 0x08;
static const UINT32 AY_4H_WRITE_ADDRESS = 0x0C;

//
// RAM region is internal to the 6802 (no external RAM)
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {0}
                                                }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                "012", "012345"
                                                      {NO_BANK_SWITCH,  0x500CL, 0xFF,  " 1G", "6821 0"}, // 6821
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x500CL, 0xFF, 0x00,  " 1G", "6821 0"}, // 6821
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                    "0123456789"
                                                            {CCatnMouseSoundBaseGame::ayIdle,     "AY Idle   "},
                                                            {CCatnMouseSoundBaseGame::ayCheck,    "AY Check  "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CCatnMouseSoundBaseGame::CCatnMouseSoundBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegion,
           s_ramRegion,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new C6802Cpu();
    m_cpu->idle();

    // Create the proxy to get to the AY PSGs
    m_6821ProxyCpu = new C6821ProxyCpu(m_cpu, 0x500CL, 0x00);
    m_6821ProxyCpu->idle();

    // AY #0 4G
    m_ay[0] = new CAY38910(m_6821ProxyCpu, AY_4G_WRITE_ADDRESS, AY_4G_READ_PSG, AY_4G_WRITE_PSG);

    // AY #1 4H
    m_ay[1] = new CAY38910(m_6821ProxyCpu, AY_4H_WRITE_ADDRESS, AY_4H_READ_PSG, AY_4H_WRITE_PSG);

    // The interrupt is on the IRQ pin.
    m_interrupt = ICpu::IRQ0;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;
}


CCatnMouseSoundBaseGame::~CCatnMouseSoundBaseGame(
)
{
    delete m_ay[1];
    m_ay[1] = (CAY38910 *) NULL;

    delete m_ay[0];
    m_ay[0] = (CAY38910 *) NULL;

    delete m_6821ProxyCpu;
    m_6821ProxyCpu = (ICpu *) NULL;

    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


PERROR
CCatnMouseSoundBaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}


// Idle (quiet) the AY-3-8910's.
PERROR
CCatnMouseSoundBaseGame::ayIdle(
    void *cCatnMouseSoundBaseGame
)
{
    CCatnMouseSoundBaseGame *pThis = (CCatnMouseSoundBaseGame *) cCatnMouseSoundBaseGame;
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
CCatnMouseSoundBaseGame::ayCheck(
    void *cCatnMouseSoundBaseGame
)
{
    CCatnMouseSoundBaseGame *pThis = (CCatnMouseSoundBaseGame *) cCatnMouseSoundBaseGame;
    PERROR error = errorSuccess;

    error = pThis->m_ay[0]->check();
    if (SUCCESS(error))
    {
        error = pThis->m_ay[1]->check();
    }

    return error;
}

