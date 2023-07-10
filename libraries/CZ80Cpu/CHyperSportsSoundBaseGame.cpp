//
// Copyright (c) 2023, Paul R. Swan
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
#include "CHyperSportsSoundBaseGame.h"
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

// Used for custom functions
static const UINT32 s_dacAddress        = 0xe000L;
static const UINT32 s_psgCommandAddress = 0xe001L;
static const UINT32 s_psgTriggerAddress = 0xe002L;

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0x4000, 0x43FF, 1, 0x0F, "C16", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x4000, 0x43FF, 1, 0xF0, "C17", "Prog. "}, // "Program RAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x4000,   0x43FF,   1, 0xFF, "C1x", "Prog. "}, // "Program RAM, 2114, 1K/1G"
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
                                                      {NO_BANK_SWITCH,  0x6000L, 0xFF, "F12", "SndCmd"},
                                                      {NO_BANK_SWITCH,  0x8000L, 0xFF, " A5", "Timer "},
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0xe000L, 0xFF, 0x00, "E15", "DAC   "},
                                                        {NO_BANK_SWITCH, 0xe001L, 0xFF, 0x00, "F13", "489Cmd"},
                                                        {NO_BANK_SWITCH, 0xe002L, 0xFF, 0x00, "F15", "489Tri"},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                       "0123456789"
                                                            {CHyperSportsSoundBaseGame::testDAC,     "Test DAC  "},
                                                            {CHyperSportsSoundBaseGame::testSN76489, "Test 76489"},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CHyperSportsSoundBaseGame::CHyperSportsSoundBaseGame(
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

    // Clear the sound generator to silent
    (void) clearSN76489(m_cpu);

}


CHyperSportsSoundBaseGame::~CHyperSportsSoundBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// Interrupt comes from the main board.
PERROR
CHyperSportsSoundBaseGame::interruptCheck(
)
{
    PERROR error = errorNotImplemented;
    return error;
}


//
// Test the audio output DAC with a simple sawtooth pattern
//
PERROR
CHyperSportsSoundBaseGame::testDAC(
    void *context
)
{
    CHyperSportsSoundBaseGame *thisGame = (CHyperSportsSoundBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT32 count;

    // Loop to generate a repeating pattern for a few seconds to hear and see it on the scope.
    for (UINT32 x = 0 ; x < 1200 ; x++)
    {
        // Ramp up
        for (count = 0 ; count != 250 ; count += 25)
        {
            (void) cpu->memoryWrite(s_dacAddress, count);
        }

        // Ramp down
        for (count = 250 ; count != 0 ; count -= 25)
        {
            (void) cpu->memoryWrite(s_dacAddress, count);
        }
    }

    return error;
}


//
// Test the SN76489 tone generator
//
PERROR
CHyperSportsSoundBaseGame::testSN76489(
    void *context
)
{
    CHyperSportsSoundBaseGame *thisGame = (CHyperSportsSoundBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;

    // Channel 0 - Tone - Lo
    (void) cpu->memoryWrite(s_psgCommandAddress, 0x80);
    (void) cpu->memoryWrite(s_psgTriggerAddress, 0x00);

    // Channel 0 - Tone - Hi
    (void) cpu->memoryWrite(s_psgCommandAddress, 0x0F);
    (void) cpu->memoryWrite(s_psgTriggerAddress, 0x00);

    // Channel 0 - Amplitude
    (void) cpu->memoryWrite(s_psgCommandAddress, 0x90);
    (void) cpu->memoryWrite(s_psgTriggerAddress, 0x00);

    // wait 5 seconds
    delay(5000);

    // Clear back to silent
    error = thisGame->clearSN76489(cpu);

    return error;
}


//
// Test the SN76489 tone generator
//
PERROR
CHyperSportsSoundBaseGame::clearSN76489(
    ICpu *cpu
)
{
    PERROR error = errorSuccess;

    // Channel 0 - Clear
    (void) cpu->memoryWrite(s_psgCommandAddress, 0x9F);
    (void) cpu->memoryWrite(s_psgTriggerAddress, 0x00);

    // Channel 1 - Clear
    (void) cpu->memoryWrite(s_psgCommandAddress, 0xBF);
    (void) cpu->memoryWrite(s_psgTriggerAddress, 0x00);

    // Channel 2 - Clear
    (void) cpu->memoryWrite(s_psgCommandAddress, 0xDF);
    (void) cpu->memoryWrite(s_psgTriggerAddress, 0x00);

    // Channel 3 - Clear
    (void) cpu->memoryWrite(s_psgCommandAddress, 0xFF);
    (void) cpu->memoryWrite(s_psgTriggerAddress, 0x00);

    return error;
}

