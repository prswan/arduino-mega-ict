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
#include "CGravitarBaseGame.h"
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
                                                  {NO_BANK_SWITCH, 0x0000,      0x07FF,      1, 0xFF, "NP1", "Progrm"}, // "program RAM"
                                                  {NO_BANK_SWITCH, 0x2000,      0x27FF,      1, 0xFF, " K7", "Vector"}, // "vector RAM"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all versions.
// This description is used for the byte-wide intensive random access memory test.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                        "012", "012345"
                                                          {NO_BANK_SWITCH, 0x0000, 0x07FF, 1, 0xFF, "NP1", "Progrm"}, // "program RAM"
                                                          {NO_BANK_SWITCH, 0x2000, 0x27FF, 1, 0xFF, " K7", "Vector"}, // "vector RAM"
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
                                                      {NO_BANK_SWITCH, 0x7800, 0x01,  " M9", "COIN R"}, // Coin Right
                                                      {NO_BANK_SWITCH, 0x7800, 0x02,  " M9", "COIN L"}, // Coin Left
                                                      {NO_BANK_SWITCH, 0x7800, 0x04,  " M9", "COIN A"}, // Coin Aux
                                                      {NO_BANK_SWITCH, 0x7800, 0x08,  " M9", "SLAM  "}, // Slam Switch
                                                      {NO_BANK_SWITCH, 0x7800, 0x10,  " M9", "TEST  "}, // Self Test
                                                      {NO_BANK_SWITCH, 0x7800, 0x20,  " M9", "SA    "}, // SA
                                                      {NO_BANK_SWITCH, 0x7800, 0x40,  " M9", "HALT  "}, // Halt
                                                      {NO_BANK_SWITCH, 0x7800, 0x80,  " M9", "3KHZ  "}, // 3KHZ
                                                      {NO_BANK_SWITCH, 0x8000, 0x01,  " L9", "SHLD 1"}, // Shields 1
                                                      {NO_BANK_SWITCH, 0x8000, 0x02,  " L9", "FIRE 1"}, // Fire 1
                                                      {NO_BANK_SWITCH, 0x8000, 0x04,  " L9", "ROTR 1"}, // Rotate Right 1
                                                      {NO_BANK_SWITCH, 0x8000, 0x08,  " L9", "ROTL 1"}, // Rotate Left 1
                                                      {NO_BANK_SWITCH, 0x8000, 0x10,  " L9", "THST 1"}, // Thrust 1
                                                      {NO_BANK_SWITCH, 0x8000, 0x20,  " L9", "OPT S4"}, // P10 Switch 4
                                                      {NO_BANK_SWITCH, 0x8000, 0x40,  " L9", "OPT S3"}, // P10 Switch 3
                                                      {NO_BANK_SWITCH, 0x8000, 0x80,  " L9", "OPT S2"}, // P10 Switch 2
                                                      {NO_BANK_SWITCH, 0x8800, 0x01,  " N9", "SHLD 2"}, // Shields 2
                                                      {NO_BANK_SWITCH, 0x8800, 0x02,  " N9", "FIRE 2"}, // Fire 2
                                                      {NO_BANK_SWITCH, 0x8800, 0x04,  " N9", "ROTR 2"}, // Rotate Right 2
                                                      {NO_BANK_SWITCH, 0x8800, 0x08,  " N9", "ROTL 2"}, // Rotate Left 2
                                                      {NO_BANK_SWITCH, 0x8800, 0x10,  " N9", "THST 2"}, // Thrust 2
                                                      {NO_BANK_SWITCH, 0x8800, 0x20,  " N9", "START1"}, // Start 1
                                                      {NO_BANK_SWITCH, 0x8800, 0x40,  " N9", "START2"}, // Start 2
                                                      {NO_BANK_SWITCH, 0x8800, 0x80,  " N9", "CABINT"}, // Cabinet
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all ROM versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x8800, 0x01, 0x00,  " R9", "CNCTRR"}, // coin counter R
                                                        {NO_BANK_SWITCH, 0x8800, 0x02, 0x00,  " R9", "CNCTRL"}, // coin counter L
                                                        {NO_BANK_SWITCH, 0x8800, 0x04, 0x00,  " R9", "BANKSL"}, // Bank Select
                                                        {NO_BANK_SWITCH, 0x8800, 0x08, 0x00,  " R9", "CNLOCK"}, // coin lockout
                                                        {NO_BANK_SWITCH, 0x8800, 0x10, 0x10,  " R9", "P1 LED"}, // player 1 LED
                                                        {NO_BANK_SWITCH, 0x8800, 0x20, 0x20,  " R9", "P2 LED"}, // player 2 LED
                                                        {NO_BANK_SWITCH, 0x8800, 0x40, 0x00,  " R9", "INV X "}, // invert X
                                                        {NO_BANK_SWITCH, 0x8800, 0x80, 0x00,  " R9", "INV Y "}, // invert Y
                                                        {NO_BANK_SWITCH, 0x8840, 0xFF, 0x00,  " P3", "VG GO "}, // vec gen GO
                                                        {NO_BANK_SWITCH, 0x8880, 0xFF, 0x00,  " P3", "VG RST"}, // vec gen RESET
                                                        {NO_BANK_SWITCH, 0x88C0, 0xFF, 0x00,  " P3", "INTACK"}, // interrupt ACK
                                                        {NO_BANK_SWITCH, 0x8980, 0xFF, 0x00,  " P3", "WD CLR"}, // watchdog clear
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list

CGravitarBaseGame::CGravitarBaseGame(
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


CGravitarBaseGame::~CGravitarBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}

//
// TBD
//
PERROR
CGravitarBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}
