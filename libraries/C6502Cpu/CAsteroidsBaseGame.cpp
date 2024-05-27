//
// Copyright (c) 2021, Marc Deslauriers
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
#include "CAsteroidsBaseGame.h"
#include "C6502Cpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   6502 GND Pin 1 & GND Pin 21
//
// Watchdog Disable:
//   Ground the WD DIS test point on the PCB near the crystal

//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                  "012", "012345"
                                                  {NO_BANK_SWITCH, 0x0000,      0x03FF,      1, 0x0F, " D2", "Progrm"}, // "program RAM, Delux: L1"
                                                  {NO_BANK_SWITCH, 0x0000,      0x03FF,      1, 0xF0, " E2", "Progrm"}, // "program RAM, Delux: M1"
                                                  {NO_BANK_SWITCH, 0x4000,      0x43FF,      1, 0x0F, " M4", "Vector"}, // "vector RAM, Delux M3"
                                                  {NO_BANK_SWITCH, 0x4000,      0x43FF,      1, 0xF0, " R4", "Vector"}, // "vector RAM, Delux R3"
                                                  {NO_BANK_SWITCH, 0x4400,      0x47FF,      1, 0x0F, " N4", "Vector"}, // "vector RAM, Delux N3"
                                                  {NO_BANK_SWITCH, 0x4400,      0x47FF,      1, 0xF0, " P4", "Vector"}, // "vector RAM, Delux P3"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all versions.
// This description is used for the byte-wide intensive random access memory test.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                        "012", "012345"
                                                          {NO_BANK_SWITCH, 0x0000, 0x03FF, 1, 0xFF, "DE2", "Progrm"}, // "program RAM"
                                                          {NO_BANK_SWITCH, 0x4000, 0x43FF, 1, 0xFF, "MR4", "Vector"}, // "vector RAM"
                                                          {NO_BANK_SWITCH, 0x4400, 0x47FF, 1, 0xFF, "NP4", "Vector"}, // "vector RAM"
                                                          {0}
                                                        }; // end of list

//
// No write-only RAM on this platform.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all ROM versions.
// Input is mapped over 8 memory locations. Should be split out by bit.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                              "012", "012345"
                                                      {NO_BANK_SWITCH, 0x2001, 0x80,  "L10", "3KHZ  "}, // 3Khz
                                                      {NO_BANK_SWITCH, 0x2002, 0x80,  "L10", "HALT  "}, // HALT
                                                      {NO_BANK_SWITCH, 0x2003, 0x80,  "L10", "HYPERS"}, // Hyperspace
                                                      {NO_BANK_SWITCH, 0x2004, 0x80,  "L10", "FIRE  "}, // Fire
                                                      {NO_BANK_SWITCH, 0x2005, 0x80,  "L10", "D STEP"}, // Diag Step
                                                      {NO_BANK_SWITCH, 0x2006, 0x80,  "L10", "SLAM  "}, // Slam
                                                      {NO_BANK_SWITCH, 0x2007, 0x80,  "L10", "TEST  "}, // Self-Test
                                                      {NO_BANK_SWITCH, 0x2400, 0x80,  "J10", "COIN L"}, // Left Coin Sw
                                                      {NO_BANK_SWITCH, 0x2401, 0x80,  "J10", "COIN C"}, // Center Coin Sw
                                                      {NO_BANK_SWITCH, 0x2402, 0x80,  "J10", "COIN R"}, // Right Coin Sw
                                                      {NO_BANK_SWITCH, 0x2403, 0x80,  "J10", "START1"}, // 1 Player Start
                                                      {NO_BANK_SWITCH, 0x2404, 0x80,  "J10", "START2"}, // 2 Player Start
                                                      {NO_BANK_SWITCH, 0x2405, 0x80,  "J10", "THRUST"}, // Thrust
                                                      {NO_BANK_SWITCH, 0x2406, 0x80,  "J10", "ROTATR"}, // Rotate Right
                                                      {NO_BANK_SWITCH, 0x2407, 0x80,  "J10", "ROTATL"}, // Rotate Left
                                                      {NO_BANK_SWITCH, 0x2800, 0x02,  " P6", "OPT S8"}, // Option S8
                                                      {NO_BANK_SWITCH, 0x2800, 0x01,  " P6", "OPT S7"}, // Option S7
                                                      {NO_BANK_SWITCH, 0x2801, 0x02,  " P6", "OPT S6"}, // Option S6
                                                      {NO_BANK_SWITCH, 0x2801, 0x01,  " P6", "OPT S5"}, // Option S5
                                                      {NO_BANK_SWITCH, 0x2802, 0x02,  " P6", "OPT S4"}, // Option S4
                                                      {NO_BANK_SWITCH, 0x2802, 0x01,  " P6", "OPT S3"}, // Option S3
                                                      {NO_BANK_SWITCH, 0x2803, 0x02,  " P6", "OPT S2"}, // Option S2
                                                      {NO_BANK_SWITCH, 0x2803, 0x01,  " P6", "OPT S1"}, // Option S1
                                                      {0}
                                                    }; // end of list


//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list

CAsteroidsBaseGame::CAsteroidsBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion,
    const OUTPUT_REGION *s_outputRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction,
           NO_DELAY_FUNCTION)
{
    m_cpu = new C6502Cpu(false);

    m_cpu->idle();

    // A timer is on the INT pin (vector game thus no VBLANK).
    m_interrupt = ICpu::IRQ0;

    // The interrupt is based on an internal vector
    m_interruptAutoVector = true;

}


CAsteroidsBaseGame::~CAsteroidsBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}

//
// TBD
//
PERROR
CAsteroidsBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}
