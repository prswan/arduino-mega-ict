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
#include "CAstroWarsBaseGame.h"
#include "C2650Cpu.h"
#include <DFR_Key.h>


//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CAstroWarsBaseGame::CAstroWarsBaseGame(
    const ROM_REGION    *romRegion,
    const RAM_REGION    *ramRegion,
    const RAM_REGION    *ramRegionByteOnly,
    const RAM_REGION    *ramRegionWriteOnly,
    const INPUT_REGION  *inputRegion,
    const OUTPUT_REGION *outputRegion
) : CGame( romRegion,
           ramRegion,
           ramRegionByteOnly,
           ramRegionWriteOnly,
           inputRegion,
           outputRegion,
           s_customFunction )
{
    m_cpu = new C2650Cpu();
    m_cpu->idle();

    // There is only a single interrupt pin on the 2650.
    m_interrupt = ICpu::IRQ0;

    // The interrupt uses an external hardware vector.
    m_interruptAutoVector = false;

    // The vector is hard coded to the same value on both games.
    m_interruptResponse = 0x03;

}


CAstroWarsBaseGame::~CAstroWarsBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


PERROR
CAstroWarsBaseGame::onBankSwitchFlagLo(
    void *cAstroWarsBaseGame
)
{
    PERROR              error     = errorSuccess;
    CAstroWarsBaseGame *thisGame  = (CAstroWarsBaseGame *) cAstroWarsBaseGame;
    C2650Cpu           *cpu       = (C2650Cpu *) thisGame->m_cpu;

    cpu->flagWrite(LOW);

    return error;
}


PERROR
CAstroWarsBaseGame::onBankSwitchFlagHi(
    void *cAstroWarsBaseGame
)
{
    PERROR              error     = errorSuccess;
    CAstroWarsBaseGame *thisGame  = (CAstroWarsBaseGame *) cAstroWarsBaseGame;
    C2650Cpu           *cpu       = (C2650Cpu *) thisGame->m_cpu;

    cpu->flagWrite(HIGH);

    return error;
}

