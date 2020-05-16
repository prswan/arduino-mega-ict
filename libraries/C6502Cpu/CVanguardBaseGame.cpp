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
#include "CVanguardBaseGame.h"
#include "C6502ClockMasterCpu.h"
#include <DFR_Key.h>

//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                  "012", "012345"
                                                  {NO_BANK_SWITCH, 0x0000,      0x03FF,      1, 0xFF, "c12", "Work  "}, // "Working RAM, 4118, CPU PCB"
                                                  {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0x0F, "v35", "Front "}, // "Front RAM / Video RAM 2, 2114 Video PCB"
                                                  {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0xF0, "v36", "Front "}, // "Front RAM / Video RAM 2, 2114 Video PCB"
                                                  {NO_BANK_SWITCH, 0x0800,      0x0BFF,      1, 0x0F, "v32", "Back  "}, // "Back RAM  / Video RAM 1, 2114 Video PCB"
                                                  {NO_BANK_SWITCH, 0x0800,      0x0BFF,      1, 0xF0, "v33", "Back  "}, // "Back RAM  / Video RAM 1, 2114 Video PCB"
                                                  {NO_BANK_SWITCH, 0x0C00,      0x0FFF,      1, 0x07, "v37", "Color "}, // "Colour RAM, 2114 Video PCB, D0-D2"
                                                  {NO_BANK_SWITCH, 0x0C00,      0x0FFF,      1, 0x38, "v34", "Color "}, // "Colour RAM, 2114 Video PCB, D3-D5"
                                                  {NO_BANK_SWITCH, 0x1000,      0x17FF,      1, 0xFF, "v67", "FChar0"}, // "Front Character RAM, 2128 Video PCB, RAM 1"
                                                  {NO_BANK_SWITCH, 0x1800,      0x1FFF,      1, 0xFF, "v68", "FChar1"}, // "Front Character RAM, 2128 Video PCB, RAM 2"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
// This description is used for the byte-wide intensive random access memory test.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                  "012", "012345"
                                                          {NO_BANK_SWITCH, 0x0000,      0x03FF,      1, 0xFF, "c12", "Work  "}, // "Working RAM, 4118, CPU PCB"
                                                          {NO_BANK_SWITCH, 0x0400,      0x07FF,      1, 0xFF, " - ", "Front "}, // "Front RAM / Video RAM 2, 2114 Video PCB"
                                                          {NO_BANK_SWITCH, 0x0800,      0x0BFF,      1, 0xFF, " - ", "Back  "}, // "Back RAM  / Video RAM 1, 2114 Video PCB"
                                                          {NO_BANK_SWITCH, 0x1000,      0x17FF,      1, 0xFF, "v67", "FChar0"}, // "Front Character RAM, 2128 Video PCB, RAM 1"
                                                          {NO_BANK_SWITCH, 0x1800,      0x1FFF,      1, 0xFF, "v68", "FChar1"}, // "Front Character RAM, 2128 Video PCB, RAM 2"
                                                          {0}
                                                        }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all ROM versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                               "012", "012345"
                                                      {NO_BANK_SWITCH, 0x3104L, 0xFF,  "c27", "IN0 P1"}, // Inputs, P1
                                                      {NO_BANK_SWITCH, 0x3105L, 0xFF,  "c28", "IN1 P2"}, // Inputs, P2
                                                      {NO_BANK_SWITCH, 0x3106L, 0xFF,  "c3 ", "DSW   "}, // Dip switch
                                                      {NO_BANK_SWITCH, 0x3107L, 0xFF,  "c29", "IN3 CS"}, // Inputs, coin & start
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all ROM versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x3103L, 0x07, 0x00,  "c33", "Colour"}, // Background Colour
                                                        {NO_BANK_SWITCH, 0x3103L, 0x08, 0x00,  "c33", "C.Bank"}, // Character Bank
                                                        {NO_BANK_SWITCH, 0x3103L, 0x80, 0x00,  "c33", "ScrFlp"}, // Screen Flip
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list

//
// Notes
// -----
//
// * The clock output from the ICT should be connected to IC 26 (LS04) pin 9 that's
//   been disconnected from the 11.289MHz clock source (e.g. socket and insert the IC
//   with the pin bent out and connect the ICT clock to the bent out pin).
//
//   The schematic shows the clock circuit on the video PCB SK-5 but it's actually on
//   the CPU PCB SK-4.
//

CVanguardBaseGame::CVanguardBaseGame(
    const ROM_REGION *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction,
           CVanguardBaseGame::delayFunction)
{
    m_cpu = new C6502ClockMasterCpu(false);

    m_cpu->idle();

    // VBLANK is on the INT pin (==IRQ0).
    m_interrupt = ICpu::IRQ0;

    // The interrupt is based on an internal vector
    m_interruptAutoVector = true;

    // Clear the sound latch to quiet the sound hardware
    m_cpu->memoryWrite(0x8006L, 0);
    m_cpu->memoryWrite(0x8007L, 0);

}


CVanguardBaseGame::~CVanguardBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


PERROR CVanguardBaseGame::delayFunction(
    void *context,
    unsigned long ms
)
{
    C6502ClockMasterCpu *cpu = (C6502ClockMasterCpu *) context;
    PERROR error = errorSuccess;
    UINT16 data;

    unsigned long startTime = millis();
    unsigned long endTime   = startTime + ms;

    // This *should* be aligned
    while (millis() < endTime)
    {
        for (int x = 0 ; x < 64 ; x++)
        {
            cpu->clockPulse();
        }
    }

    // This is to bring clock alignment back to the start of a cycle
    error = cpu->memoryRead(0xFFFF, &data);

    return error;
}

