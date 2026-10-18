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
#include "CKikiKaikaiSoundBaseGame.h"
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
//   This is the sound CPU on the single main board (K1100195A). It is the
//   Z80 next to the A85_11.F6 sound ROM and the YM2203.
//
// Z80 Compatibility Notes
//   * IO space is not used.
//   * M1 (1st machine cycle) is not used.
//
// Notes:
//   The memory map is taken from MAME (src/mame/taito/kikikai.cpp,
//   base_state::sound_map). The RAM is the 16KB in the two TMM2064 at D1 & D3,
//   tested byte wide as which chip holds which half isn't known.
//
//   0000-7FFF ROM F6
//   8000-A7FF RAM shared with the main CPU (main CPU C000-E7FF)
//   A800-BFFF RAM
//   C000      YM2203 address (W) / status (R)
//   C001      YM2203 data
//
//   The DIP switches SWA & SWB are read via the YM2203 I/O ports A & B.
//   The interrupt is VBLANK on the INT pin, latched until acknowledged.
//
//   The main CPU can reset this CPU (main F008 D2) and also accesses the
//   shared RAM so it should be removed (or held in reset) for testing.
//
// void base_state::sound_map(address_map &map)
//

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0x8000, 0xA7FF, 1, 0xFF, "D13", "Shared"}, // RAM shared with the main CPU, TMM2064 D1 & D3
                                                  {NO_BANK_SWITCH, 0xA800, 0xBFFF, 1, 0xFF, "D13", "Prog. "}, // RAM, the rest of the TMM2064 D1 & D3
                                                  {0}
                                                }; // end of list

//
// No write-only RAM on this platform.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                             "012", "012345"
                                                      {NO_BANK_SWITCH, 0xC000L, 0xFF, " - ", "YM Sts"}, // YM2203 status
                                                      {NO_BANK_SWITCH, 0xC001L, 0xFF, " - ", "YM Rd "}, // YM2203 data
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                  "012", "012345"
                                                        {NO_BANK_SWITCH, 0xC000L, 0xFF, 0x00, " - ", "YM Ad "}, // YM2203 address
                                                        {NO_BANK_SWITCH, 0xC001L, 0xFF, 0x00, " - ", "YM Wr "}, // YM2203 data
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                      "0123456789"
                                                            {CKikiKaikaiSoundBaseGame::ssgIdle,     "SSG Idle  "},
                                                            {CKikiKaikaiSoundBaseGame::ssgCheck,    "SSG Check "},
                                                            {CKikiKaikaiSoundBaseGame::ssgNoise,    "SSG Noise "},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CKikiKaikaiSoundBaseGame::CKikiKaikaiSoundBaseGame(
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

    // The VBLANK interrupt is on the INT pin.
    m_interrupt = ICpu::IRQ0;

    // The interrupt is latched until acknowledged. Nothing drives a vector
    // so the data bus is expected to read 0xFF (unverified).
    m_interruptAutoVector = false;
    m_interruptResponse = 0xFF;

    // YM2203 SSG
    m_ssg = new CAY38910(m_cpu, 0xC000L, 0xC001L, 0xC001L);
}


CKikiKaikaiSoundBaseGame::~CKikiKaikaiSoundBaseGame(
)
{
    delete m_ssg;
    m_ssg = (CAY38910 *) NULL;

    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


PERROR
CKikiKaikaiSoundBaseGame::ssgIdle(
    void *cKikiKaikaiSoundBaseGame
)
{
    CKikiKaikaiSoundBaseGame *pThis = (CKikiKaikaiSoundBaseGame *) cKikiKaikaiSoundBaseGame;

    return pThis->m_ssg->idle();
}


PERROR
CKikiKaikaiSoundBaseGame::ssgCheck(
    void *cKikiKaikaiSoundBaseGame
)
{
    CKikiKaikaiSoundBaseGame *pThis = (CKikiKaikaiSoundBaseGame *) cKikiKaikaiSoundBaseGame;

    return pThis->m_ssg->check();
}


PERROR
CKikiKaikaiSoundBaseGame::ssgNoise(
    void *cKikiKaikaiSoundBaseGame
)
{
    static const UINT32 holdTimeInMs  = 2000;
    static const UINT32 pauseTimeInMs = 1000;
    static const CAY38910::Channel channels[] = {CAY38910::CHA, CAY38910::CHB, CAY38910::CHC};
    CKikiKaikaiSoundBaseGame *pThis = (CKikiKaikaiSoundBaseGame *) cKikiKaikaiSoundBaseGame;
    PERROR error = errorSuccess;

    for (int c = 0 ; c < 3 ; c++)
    {
        error = pThis->m_ssg->noise(channels[c]);
        if (FAILED(error))
        {
            break;
        }
        delay(holdTimeInMs);
        pThis->m_ssg->idle();
        delay(pauseTimeInMs);
    }

    return error;
}

