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
#include "CSprintBaseGame.h"
#include "C6502Cpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   6502 GND Pin 1 & GND Pin 21
//

//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                  "012", "012345"
                                                  {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0x01, " F4", "Progrm"}, // "program RAM"
                                                  {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0x02, " H4", "Progrm"}, // "program RAM"
                                                  {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0x04, " J4", "Progrm"}, // "program RAM"
                                                  {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0x08, " K4", "Progrm"}, // "program RAM"
                                                  {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0x10, " F3", "Progrm"}, // "program RAM"
                                                  {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0x20, " H3", "Progrm"}, // "program RAM"
                                                  {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0x40, " J3", "Progrm"}, // "program RAM"
                                                  {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0x80, " K3", "Progrm"}, // "program RAM"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all versions.
// This description is used for the byte-wide intensive random access memory test.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                        "012", "012345"
                                                          {NO_BANK_SWITCH, 0x0400, 0x07FF, 1, 0xFF, "RAM", "Progrm"}, // "program RAM"
                                                          {0}
                                                        }; // end of list

//
// No write-only RAM on this platform.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list


//
// Output region is the same for all ROM versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x0c00, 0xFF, 0x00,  " H8", "ATTTR0"}, // Attract 0
                                                        {NO_BANK_SWITCH, 0x0c01, 0xFF, 0x00,  " H8", "ATTTR1"}, // Attract 1
                                                        {NO_BANK_SWITCH, 0x0c10, 0xFF, 0x00,  " H8", "SKID10"}, // Skid1 0
                                                        {NO_BANK_SWITCH, 0x0c11, 0xFF, 0x00,  " H8", "SKID11"}, // Skid1 1
                                                        {NO_BANK_SWITCH, 0x0c20, 0xFF, 0x00,  " H8", "SKID20"}, // Skid2 0
                                                        {NO_BANK_SWITCH, 0x0c21, 0xFF, 0x00,  " H8", "SKID21"}, // Skid2 1
                                                        {NO_BANK_SWITCH, 0x0c30, 0xFF, 0x00,  " H8", "LAMP10"}, // Lamp1 0
                                                        {NO_BANK_SWITCH, 0x0c31, 0xFF, 0x00,  " H8", "LAMP11"}, // Lamp1 1
                                                        {NO_BANK_SWITCH, 0x0c40, 0xFF, 0x00,  " H8", "LAMP20"}, // Lamp2 0
                                                        {NO_BANK_SWITCH, 0x0c41, 0xFF, 0x00,  " H8", "LAMP21"}, // Lamp2 1
                                                        {NO_BANK_SWITCH, 0x0c60, 0xFF, 0x00,  " H8", "SPARE0"}, // Spare 0
                                                        {NO_BANK_SWITCH, 0x0c61, 0xFF, 0x00,  " H8", "SPARE1"}, // Spare 1
                                                        {NO_BANK_SWITCH, 0x0c80, 0xFF, 0x00,  " E8", "WDRES "}, // watchdog reset
                                                        {NO_BANK_SWITCH, 0x0d00, 0xFF, 0x00,  " E8", "CL1RST"}, // collision 1 reset
                                                        {NO_BANK_SWITCH, 0x0d80, 0xFF, 0x00,  " E8", "CL2RST"}, // collision 2 reset
                                                        {NO_BANK_SWITCH, 0x0e00, 0xFF, 0x00,  " E8", "ST1RST"}, // steering 1 reset
                                                        {NO_BANK_SWITCH, 0x0e80, 0xFF, 0x00,  " E8", "ST2RST"}, // steering 2 reset
                                                        {NO_BANK_SWITCH, 0x0f00, 0xFF, 0x00,  " E8", "SNDRST"}, // noise reset
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list

CSprintBaseGame::CSprintBaseGame(
    const ROM_REGION *romRegion,
    const INPUT_REGION *s_inputRegion
) : CGame( romRegion,
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

    // VBLANK is on the INT pin (==IRQ0).
    m_interrupt = ICpu::IRQ0;

    // The interrupt is based on an internal vector
    m_interruptAutoVector = true;

}


CSprintBaseGame::~CSprintBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}

//
// TBD
//
PERROR
CSprintBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}
