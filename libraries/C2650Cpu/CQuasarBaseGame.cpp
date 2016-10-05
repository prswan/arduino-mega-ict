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
#include "CQuasarBaseGame.h"
#include "C2650Cpu.h"
#include <DFR_Key.h>

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


// Watchdog Disable:
//   Tie CPU board IC 12D (74LS90) pin 11 to GND.
//   ...though it appears not to be needed other than
// to suppress an intermittant RESET error in the BusCheck.
//

CQuasarBaseGame::CQuasarBaseGame(
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

    // The vector is hard coded
    m_interruptResponse = 0x0A;

}


CQuasarBaseGame::~CQuasarBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


PERROR
CQuasarBaseGame::onBankSwitchCharacterRam(
    void *cQuasarBaseGame
)
{
    PERROR           error     = errorSuccess;
    CQuasarBaseGame *thisGame  = (CQuasarBaseGame *) cQuasarBaseGame;
    C2650Cpu        *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // Data is ignored, A1-A0 are latched
    error = cpu->memoryWrite(0x12000, 0);

    return error;
}


PERROR
CQuasarBaseGame::onBankSwitchColourRam(
    void *cQuasarBaseGame
)
{
    PERROR           error     = errorSuccess;
    CQuasarBaseGame *thisGame  = (CQuasarBaseGame *) cQuasarBaseGame;
    C2650Cpu        *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // Data is ignored, A1-A0 are latched.
    error = cpu->memoryWrite(0x12001, 0);

    return error;
}


PERROR
CQuasarBaseGame::onBankSwitchEffectRam(
    void *cQuasarBaseGame
)
{
    PERROR           error     = errorSuccess;
    CQuasarBaseGame *thisGame  = (CQuasarBaseGame *) cQuasarBaseGame;
    C2650Cpu        *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // Data is ignored, A1-A0 are latched.
    error = cpu->memoryWrite(0x12002, 0);

    return error;
}


PERROR
CQuasarBaseGame::onBankSwitchEffectBlank(
    void *cQuasarBaseGame
)
{
    PERROR           error     = errorSuccess;
    CQuasarBaseGame *thisGame  = (CQuasarBaseGame *) cQuasarBaseGame;
    C2650Cpu        *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // Data is ignored, A1-A0 are latched.
    error = cpu->memoryWrite(0x12003, 0);

    return error;
}


PERROR
CQuasarBaseGame::onBankSwitchIN0(
    void *cQuasarBaseGame
)
{
    PERROR           error     = errorSuccess;
    CQuasarBaseGame *thisGame  = (CQuasarBaseGame *) cQuasarBaseGame;
    C2650Cpu        *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // Data is ignored, A1-A0 are latched
    error = cpu->memoryWrite(0x12008, 0);

    return error;
}


PERROR
CQuasarBaseGame::onBankSwitchIN1(
    void *cQuasarBaseGame
)
{
    PERROR           error     = errorSuccess;
    CQuasarBaseGame *thisGame  = (CQuasarBaseGame *) cQuasarBaseGame;
    C2650Cpu        *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // Data is ignored, A1-A0 are latched
    error = cpu->memoryWrite(0x12009, 0);

    return error;
}


PERROR
CQuasarBaseGame::onBankSwitchSW1(
    void *cQuasarBaseGame
)
{
    PERROR           error     = errorSuccess;
    CQuasarBaseGame *thisGame  = (CQuasarBaseGame *) cQuasarBaseGame;
    C2650Cpu        *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // Data is ignored, A1-A0 are latched
    error = cpu->memoryWrite(0x1200A, 0);

    return error;
}


PERROR
CQuasarBaseGame::onBankSwitchSW2(
    void *cQuasarBaseGame
)
{
    PERROR           error     = errorSuccess;
    CQuasarBaseGame *thisGame  = (CQuasarBaseGame *) cQuasarBaseGame;
    C2650Cpu        *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // Data is ignored, A1-A0 are latched
    error = cpu->memoryWrite(0x1200B, 0);

    return error;
}

