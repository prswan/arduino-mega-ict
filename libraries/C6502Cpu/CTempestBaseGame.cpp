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
#include "CTempestBaseGame.h"
#include "C6502Cpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   6502 GND Pin 1 & GND Pin 21
//
// Watchdog Disable:
//   Ground the WD DIS test point on the PCB near the CPU

//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                  "012", "012345"
                                                  {NO_BANK_SWITCH, 0x0000,      0x03FF,      1, 0x0F, " R2", "Progrm"}, // "program RAM"
                                                  {NO_BANK_SWITCH, 0x0000,      0x03FF,      1, 0xF0, " P2", "Progrm"}, // "program RAM"
                                                  {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0x0F, " R4", "Progrm"}, // "program RAM"
                                                  {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0xF0, " P4", "Progrm"}, // "program RAM"
                                                  {NO_BANK_SWITCH, 0x2000,      0x23FF,      1, 0x0F, " M3", "Vector"}, // "vector RAM"
                                                  {NO_BANK_SWITCH, 0x2000,      0x23FF,      1, 0xF0, " M4", "Vector"}, // "vector RAM"
                                                  {NO_BANK_SWITCH, 0x2400,      0x27FF,      1, 0x0F, " L3", "Vector"}, // "vector RAM"
                                                  {NO_BANK_SWITCH, 0x2400,      0x27FF,      1, 0xF0, " L4", "Vector"}, // "vector RAM"
                                                  {NO_BANK_SWITCH, 0x2800,      0x2BFF,      1, 0x0F, " K3", "Vector"}, // "vector RAM"
                                                  {NO_BANK_SWITCH, 0x2800,      0x2BFF,      1, 0xF0, " K4", "Vector"}, // "vector RAM"
                                                  {NO_BANK_SWITCH, 0x2C00,      0x2FFF,      1, 0x0F, " J3", "Vector"}, // "vector RAM"
                                                  {NO_BANK_SWITCH, 0x2C00,      0x2FFF,      1, 0xF0, " J4", "Vector"}, // "vector RAM"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all versions.
// This description is used for the byte-wide intensive random access memory test.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                        "012", "012345"
                                                          {NO_BANK_SWITCH, 0x0000, 0x03FF, 1, 0xFF, "PR2", "Progrm"}, // "program RAM"
                                                          {NO_BANK_SWITCH, 0x0400, 0x07FF, 1, 0xFF, "PR4", "Progrm"}, // "program RAM"
                                                          {NO_BANK_SWITCH, 0x2000, 0x23FF, 1, 0xFF, "M34", "Vector"}, // "vector RAM"
                                                          {NO_BANK_SWITCH, 0x2400, 0x27FF, 1, 0xFF, "L34", "Vector"}, // "vector RAM"
                                                          {NO_BANK_SWITCH, 0x2800, 0x2BFF, 1, 0xFF, "K34", "Vector"}, // "vector RAM"
                                                          {NO_BANK_SWITCH, 0x2C00, 0x2FFF, 1, 0xFF, "J34", "Vector"}, // "vector RAM"
                                                          {0}
                                                        }; // end of list

//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                       "012", "012345"
                                                          {NO_BANK_SWITCH, 0x0800, 0x080F, 1, 0x0F, " P9", "Color "}, // "color RAM"
                                                          {0}
                                                         }; // end of list

//
// Input region is the same for all ROM versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                              "012", "012345"
                                                      {NO_BANK_SWITCH, 0x0C00, 0xFF,  " R9", "IN0   "}, // Coin & test
                                                      {NO_BANK_SWITCH, 0x0D00, 0xFF,  "N12", "DSW1  "}, // Dip switch 1
                                                      {NO_BANK_SWITCH, 0x0E00, 0xFF,  "M12", "DSW2  "}, // Dip switch 2
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all ROM versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x4000, 0x01, 0x00,  "---", "CNCTRT"}, // right coin counter
                                                        {NO_BANK_SWITCH, 0x4000, 0x02, 0x00,  "---", "CNCTLT"}, // left coin counter
                                                        {NO_BANK_SWITCH, 0x4800, 0xFF, 0x01,  " J5", "VG GO "}, // vec gen GO
                                                        {NO_BANK_SWITCH, 0x5000, 0xFF, 0x01,  " J5", "WD CLR"}, // watchdog clear
                                                        {NO_BANK_SWITCH, 0x5800, 0xFF, 0x01,  " J5", "VG RST"}, // vec gen RESET
                                                        {NO_BANK_SWITCH, 0x60E0, 0x01, 0x01,  " H4", "LED 1 "}, // one player led
                                                        {NO_BANK_SWITCH, 0x60E0, 0x02, 0x02,  " H4", "LED 2 "}, // two player led
                                                        {NO_BANK_SWITCH, 0x60E0, 0x04, 0x00,  " H4", " FLIP "}, // FLIP
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list

CTempestBaseGame::CTempestBaseGame(
    const ROM_REGION *romRegion
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

    // A timer is on the INT pin (vector game thus no VBLANK).
    m_interrupt = ICpu::IRQ0;

    // The interrupt is based on an internal vector
    m_interruptAutoVector = true;

}


CTempestBaseGame::~CTempestBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}

//
// TBD
//
PERROR
CTempestBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}
