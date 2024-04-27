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
#include "CAtariSystem1SoundBaseGame.h"
#include "C6502Cpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   6502 GND Pin 1 & GND Pin 21
//
// Watchdog Disable:
//   Ground the WD DIS test point or WDIS signal IC 9K (LS08) pin 9 on the System 1 main PCB

// From the manual/MAME
/*
========================================================================
SOUND CPU
========================================================================
0000-0FFF   R/W   xxxxxxxx   Program RAM
1000-100F   R/W   xxxxxxxx   M6522 (on game board)
1000-1FFF   R/W   xxxxxxxx   Catridge external RAM/ROM
1800-1801   R/W   xxxxxxxx   YM2151 communications
1810        R     xxxxxxxx   Sound command read
1810          W   xxxxxxxx   Sound response write
1820        R     x--xxxxx   Sound status/input read
            R     x-------      (Self-test)
            R     ---x----      (Response buffer full)
            R     ----x---      (Command buffer full)
            R     -----x--      (Service coin)
            R     ------x-      (Left coin)
            R     -------x      (Right coin)
1824-1825     W   -------x   LED control
1826          W   -------x   Right coin counter
1827          W   -------x   Left coin counter
1870-187F   R/W   xxxxxxxx   POKEY communications
4000-FFFF   R     xxxxxxxx   Program ROM
========================================================================
Interrupts:
    IRQ = YM2151 interrupt
    NMI = latch on sound command
========================================================================
*/

//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                  "012", "012345"
                                                  {NO_BANK_SWITCH, 0x0000,      0x07FF,      1, 0xFF, "15L", "Progrm"}, // Program RAM, 6116
                                                  {NO_BANK_SWITCH, 0x0800,      0x0FFF,      1, 0xFF, "14L", "Progrm"}, // Program RAM, 6116
                                                  {0}
                                                }; // end of list

//
// No write-only RAM on this platform.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all ROM versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                              "012", "012345"
                                                      {NO_BANK_SWITCH, 0x1000, 0xFF,  " --", "SNDEXT"}, // External optional 6522 VIA
                                                      {NO_BANK_SWITCH, 0x1800, 0xFF,  "17H", "YMHCSA"}, // YM2151 Address
                                                      {NO_BANK_SWITCH, 0x1801, 0x83,  "17H", "YMHCSD"}, // YM2151 Data
                                                      {NO_BANK_SWITCH, 0x1810, 0xFF,  "14E", "RD68K "}, // Sound Command
                                                      {NO_BANK_SWITCH, 0x1820, 0x9F,  "15C", "SIORD "}, // Sound Status Read, D5/D6 not connected
                                                      {NO_BANK_SWITCH, 0x187A, 0xFF,  "17J", "POKEYr"}, // POKEY, the random number generator
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all ROM versions
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x1800, 0xFF, 0x00,  " --", "SNDEXT"}, // External optional 6522 VIA
                                                        {NO_BANK_SWITCH, 0x1800, 0xFF, 0x00,  "17H", "YMHCSA"}, // YM2151 Address
                                                        {NO_BANK_SWITCH, 0x1801, 0xFF, 0x00,  "17H", "YMHCSD"}, // YM2151 Data
                                                        {NO_BANK_SWITCH, 0x1810, 0x55, 0x00,  "15E", "WR68K5"}, // Sound Response, 0x55
                                                        {NO_BANK_SWITCH, 0x1810, 0xAA, 0x00,  "15E", "WR68KA"}, // Sound Response, 0xAA
                                                        {0}
                                                      }; // end of list


//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list

CAtariSystem1SoundBaseGame::CAtariSystem1SoundBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction,
           NO_DELAY_FUNCTION)
{
    m_cpu = new C6502Cpu(false);

    m_cpu->idle();

    // IRQ = YM2151 interrupt
    // NMI = latch on sound command
    m_interrupt = ICpu::NMI;

    // The interrupt is based on an internal vector
    m_interruptAutoVector = true;

}


CAtariSystem1SoundBaseGame::~CAtariSystem1SoundBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}

//
// TBD
//
PERROR
CAtariSystem1SoundBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}
