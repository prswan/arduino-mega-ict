//
// Copyright (c) 2015, Paul R. Swan
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
#include "CAstroFighterBaseGame.h"
#include "C6502Cpu.h"
#include "C6502ClockMasterCpu.h"
#include <DFR_Key.h>

//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                               "012", "012345"
                                                  {NO_BANK_SWITCH, 0x0000,      0x03FF,      0x0F, "r2M", "Prog. "}, // "Program RAM, 2114, ???"
                                                  {NO_BANK_SWITCH, 0x0000,      0x03FF,      0xF0, "r2F", "Prog. "}, // "Program RAM, 2114, ???"
                                                  //
                                                  // *** NOTE 0x4000-0x5FFF is DRAM ***
                                                  //
                                                  // In clock master mode testing the DRAM is out of specification and may yield
                                                  // incorrect test results (failures).
                                                  //
                                                  // Split into two halves seems to work OK with ITT 4027 (single block testing yielded a
                                                  // consistent single bit error in the test).
                                                  //
                                                  {NO_BANK_SWITCH, 0x4000,      0x4FFF,      0xFF, "2AT", "Video "}, // "Video RAM, lower"
                                                  {NO_BANK_SWITCH, 0x5000,      0x5FFF,      0xFF, "2AT", "Video "}, // "Video RAM, upper"
                                    /* TEST */    {NO_BANK_SWITCH, 0x4000,      0x5FFF,      0xFF, "2AT", "Video "}, // "Video RAM, whole"
                                                  {0}
                                                }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list

//
// Notes
// -----
//
// * The bus check fails with pattern 0xDC. The databus is behind a couple of 8216
//   From the datasheet for that, HiZ draws 100uA but the Arduino pullups can only
//   source 50uA @ 1.8Vcc so it would seem the 8216's draws too much even in HiZ.
//
// * The program RAM is read synchrounously using a split CLK2oHi cycle.
//   The 2114 CS is gated by TCLD, a quarter CLK2o pulse, that effectively allows the
//   CLK2o high time to be halved to allow the A/D to setup on the RAM for the 2nd half
//   of CS.
//
// * Looking more deeply at the schematics, it looks like the VRAM access for display
//   occurs in the CLK2o low time with the CPU access in the CLK2o high time.
//
// * The clock output from the ICT should be connected to IC 6H (04) pin 11 that's
//   been disconnected from the 10.595MHz clock source (e.g. socket and insert the IC
//   with the pin bent out and connect the ICT clock to the bent out pin).
//

CAstroFighterBaseGame::CAstroFighterBaseGame(
    const bool          clockMaster,
    const ROM_REGION    *romRegion,
    const INPUT_REGION  *inputRegion,
    const OUTPUT_REGION *outputRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           inputRegion,
           outputRegion,
           s_customFunction )
{
    //        _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _
    // XTAL    \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
    //
    //            ___     ___     ___     ___     ___     ___     ___     ___     ___     ___
    // 4H QA  ___/   \___/   \___/   \___/   \___/   \___/   \___/   \___/   \___/   \___/   \_
    //
    //                _______         _______         _______         _______         _______
    // 4H QB  _______/       \_______/       \_______/       \_______/       \_______/       \_
    //
    //                        _______________                 _______________                 _
    // 4H QC  _______________/               \_______________/               \_______________/
    //
    //                                        ________________________________
    // 4H QD  _______________________________/                                \________________
    //                                       ^                           ^
    //                                       CLK2oHi                     D
    //
    // In this diagam:
    //  * IC 4H QD is inverted and feeds CLK0i (the CPU clock input) that in turn
    //    generates CLK1o & CLK2o.
    //  * IC 4H QC is the TCLD signal that is used to provide the half cycle for address setup
    //    to chip select time.
    //
    // Counting the clocks, the D should be read 7 XTAL clocks from CLK2oHi.
    //

    if (clockMaster)
    {
        m_cpu = new C6502ClockMasterCpu(7);
    }
    else
    {
        m_cpu = new C6502Cpu();
    }

    m_cpu->idle();

    // VBLANK is on the INT pin.
    m_interrupt = ICpu::INT;

    // The interrupt uses an external hardware vector.
    m_interruptAutoVector = true;
}


CAstroFighterBaseGame::~CAstroFighterBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}

