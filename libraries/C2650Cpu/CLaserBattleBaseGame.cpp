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
#include "CLaserBattleBaseGame.h"
#include "C2650Cpu.h"
#include <DFR_Key.h>


static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                                                   "012", "012345"
                                                  {NO_BANK_SWITCH,                                  0x1C00,      0x1FFF,      1, 0x0F, " 7E", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH,                                  0x1C00,      0x1FFF,      1, 0xF0, " 6E", "Prog. "}, // "Program RAM, 2114"
                                                  {CLaserBattleBaseGame::onBankSwitchBackgroundRam, 0x1800,      0x1BFF,      1, 0x0F, "12F", "Bkgrnd"}, // "Video Background RAM, 2114"
                                                  {CLaserBattleBaseGame::onBankSwitchBackgroundRam, 0x1800,      0x1BFF,      1, 0xF0, "13F", "Bkgrnd"}, // "Video Background RAM, 2114"
                                                  {NO_BANK_SWITCH,                                  0x1500+0x00, 0x1500+0x2D, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                  {NO_BANK_SWITCH,                                  0x1500+0x40, 0x1500+0x6D, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                  {NO_BANK_SWITCH,                                  0x1500+0x80, 0x1500+0xAD, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                  {NO_BANK_SWITCH,                                  0x1600+0x00, 0x1600+0x2D, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                  {NO_BANK_SWITCH,                                  0x1600+0x40, 0x1600+0x6D, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                  {NO_BANK_SWITCH,                                  0x1600+0x80, 0x1600+0xAD, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                  {NO_BANK_SWITCH,                                  0x1700+0x00, 0x1700+0x2D, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                  {NO_BANK_SWITCH,                                  0x1700+0x40, 0x1700+0x6D, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                  {NO_BANK_SWITCH,                                  0x1700+0x80, 0x1700+0xAD, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                  {0}
                                                }; // end of list

static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                                                   "012", "012345"
                                                          {NO_BANK_SWITCH,                                  0x1C00,      0x1FFF,      1, 0xFF, "76E", "Prog. "}, // "Program RAM, 2114, 7E/6E"
                                                          {CLaserBattleBaseGame::onBankSwitchBackgroundRam, 0x1800,      0x1BFF,      1, 0xFF, "1?F", "Bkgrnd"}, // "Video Background RAM, 2114, 12F/13F"
                                                          {NO_BANK_SWITCH,                                  0x1500+0x00, 0x1500+0x2D, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                          {NO_BANK_SWITCH,                                  0x1500+0x40, 0x1500+0x6D, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                          {NO_BANK_SWITCH,                                  0x1500+0x80, 0x1500+0xAD, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                          {NO_BANK_SWITCH,                                  0x1600+0x00, 0x1600+0x2D, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                          {NO_BANK_SWITCH,                                  0x1600+0x40, 0x1600+0x6D, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                          {NO_BANK_SWITCH,                                  0x1600+0x80, 0x1600+0xAD, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                          {NO_BANK_SWITCH,                                  0x1700+0x00, 0x1700+0x2D, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                          {NO_BANK_SWITCH,                                  0x1700+0x40, 0x1700+0x6D, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                          {NO_BANK_SWITCH,                                  0x1700+0x80, 0x1700+0xAD, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                          {0}
                                                        }; // end of list

//
// The effect RAM is write-ony, confirmed with the schematics.
//
// This region allows the PVI's to be initialized.
// If the PVI's are no initialized, the screen ends up all white such that no
// other graphics can be seen.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                                                     "012", "012345"
                                                           {CLaserBattleBaseGame::onBankSwitchEffectRam, 0x1800, 0x19FF, 1, 0x0F, "14F", "Effect"}, // "Video Effect RAM, 2114"
                                                           {CLaserBattleBaseGame::onBankSwitchEffectRam, 0x1800, 0x19FF, 1, 0xF0, "15F", "Effect"}, // "Video Effect RAM, 2114"
                                                           {NO_BANK_SWITCH,                              0x1500, 0x15FF, 1, 0xFF, " 5E", "2636-1"}, // "2636 PVI 1"
                                                           {NO_BANK_SWITCH,                              0x1600, 0x16FF, 1, 0xFF, " 3E", "2636-2"}, // "2636 PVI 2"
                                                           {NO_BANK_SWITCH,                              0x1700, 0x17FF, 1, 0xFF, " 2E", "2636-3"}, // "2636 PVI 3"
                                                           {0}
                                                         }; // end of list

//
// Input region is the same for all versions.
//
// Unlike prior platforms, this one does not appear to use hardware collision detection.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                                      "012", "012345"
                                                      {CLaserBattleBaseGame::onBankSwitchIN0, 0x12002L, 0x0F, "2I ", "IN0   "}, // EXT - Row0
                                                      {CLaserBattleBaseGame::onBankSwitchIN0, 0x12002L, 0xF0, "3I ", "IN0   "}, // EXT - Row0
                                                      {CLaserBattleBaseGame::onBankSwitchIN1, 0x12002L, 0x0F, "2I ", "IN1   "}, // EXT - Row1
                                                      {CLaserBattleBaseGame::onBankSwitchIN1, 0x12002L, 0xF0, "3I ", "IN1   "}, // EXT - Row1
                                                      {CLaserBattleBaseGame::onBankSwitchSW1, 0x12002L, 0x0F, "2I ", "SW1   "}, // EXT - Dipswitch 1, 1-4
                                                      {CLaserBattleBaseGame::onBankSwitchSW1, 0x12002L, 0xF0, "3I ", "SW1   "}, // EXT - Dipswitch 1, 5-8
                                                      {CLaserBattleBaseGame::onBankSwitchSW2, 0x12002L, 0x0F, "2I ", "SW2   "}, // EXT - Dipswitch 2, 1-4
                                                      {CLaserBattleBaseGame::onBankSwitchSW2, 0x12002L, 0xF0, "3I ", "SW2   "}, // EXT - Dipswitch 2, 5-8
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                               "012", "012345"
                                                        // cnt_eff_w / ~CNT_EFF
                                                        {NO_BANK_SWITCH,                           0x12000L, 0x01, 0x00, "4I ", "ABEFF1"}, //
                                                        {NO_BANK_SWITCH,                           0x12000L, 0x02, 0x00, "4I ", "ABEFF2"}, //
                                                        {NO_BANK_SWITCH,                           0x12000L, 0x04, 0x00, "4I ", "MPXEF2"}, //
                                                        {NO_BANK_SWITCH,                           0x12000L, 0x08, 0x00, "4I ", "COLEF0"}, //
                                                        {NO_BANK_SWITCH,                           0x12000L, 0x10, 0x00, "4I ", "COLEF1"}, //
                                                        {NO_BANK_SWITCH,                           0x12000L, 0x20, 0x00, "4I ", "NEG1  "}, //
                                                        {NO_BANK_SWITCH,                           0x12000L, 0x40, 0x00, "4I ", "NEG2  "}, //
                                                        {NO_BANK_SWITCH,                           0x12000L, 0x80, 0x00, "4I ", "MPXP12"}, //
                                                        // cnt_nav_w / ~CNT_NAV
                                                        {NO_BANK_SWITCH,                           0x12001L, 0x01, 0x00, "5H ", "ABNAVE"}, //
                                                        {NO_BANK_SWITCH,                           0x12001L, 0x02, 0x00, "5H ", "CLN0  "}, //
                                                        {NO_BANK_SWITCH,                           0x12001L, 0x04, 0x00, "5H ", "CLN1  "}, //
                                                        {NO_BANK_SWITCH,                           0x12001L, 0x08, 0x00, "5H ", "LUM   "}, //
                                                        {NO_BANK_SWITCH,                           0x12001L, 0x10, 0x00, "5H ", "MPXBEF"}, //
                                                        {NO_BANK_SWITCH,                           0x12001L, 0x20, 0x00, "5H ", "SHPA  "}, //
                                                        {NO_BANK_SWITCH,                           0x12001L, 0x40, 0x00, "5H ", "SHPB  "}, //
                                                        {NO_BANK_SWITCH,                           0x12001L, 0x80, 0x00, "5H ", "SHPC  "}, //
                                                        // csound1_w / ~CSOUND3
                                                        {NO_BANK_SWITCH,                           0x12002L, 0x01, 0x00, "4H ", "Snd1-1"}, //
                                                        {NO_BANK_SWITCH,                           0x12002L, 0x02, 0x00, "4H ", "Snd1-2"}, //
                                                        {NO_BANK_SWITCH,                           0x12002L, 0x04, 0x00, "4H ", "Snd1-3"}, //
                                                        {NO_BANK_SWITCH,                           0x12002L, 0x08, 0x00, "4H ", "Snd1-4"}, //
                                                        {NO_BANK_SWITCH,                           0x12002L, 0x10, 0x00, "4H ", "Snd1-5"}, //
                                                        {NO_BANK_SWITCH,                           0x12002L, 0x20, 0x00, "4H ", "Snd1-6"}, //
                                                        {NO_BANK_SWITCH,                           0x12002L, 0x40, 0x00, "4H ", "Snd1-7"}, //
                                                        {NO_BANK_SWITCH,                           0x12002L, 0x80, 0x00, "4H ", "Snd1-8"}, //
                                                        // csound2_w / ~CSOUND2
                                                        {NO_BANK_SWITCH,                           0x12007L, 0x01, 0x00, "3H ", "Snd2-1"}, //
                                                        {NO_BANK_SWITCH,                           0x12007L, 0x02, 0x00, "3H ", "Snd2-2"}, //
                                                        {NO_BANK_SWITCH,                           0x12007L, 0x04, 0x00, "3H ", "Snd2-3"}, //
                                                        {NO_BANK_SWITCH,                           0x12007L, 0x08, 0x00, "3H ", "Snd2-4"}, //
                                                        {NO_BANK_SWITCH,                           0x12007L, 0x10, 0x00, "3H ", "Snd2-5"}, //
                                                        {NO_BANK_SWITCH,                           0x12007L, 0x20, 0x00, "3H ", "Snd2-6"}, //
                                                        {NO_BANK_SWITCH,                           0x12007L, 0x40, 0x00, "3H ", "Snd2-7"}, //
                                                        {NO_BANK_SWITCH,                           0x12007L, 0x80, 0x00, "3H ", "Snd2-8"}, //
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


// Watchdog Disable:
//   Tie CPU board IC 14B (74LS02) pin 12 to GND.
//   There is a small link loop next to it to attach to.
//

CLaserBattleBaseGame::CLaserBattleBaseGame(
    const ROM_DATA2N    *romData2n,
    const ROM_REGION    *romRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
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

    // There are a bunch of control latches that aren't reset
    for (int x = 0 ; x < 8 ; x++)
    {
        m_cpu->memoryWrite(0x12000 + x, 0x00);
    }
}


CLaserBattleBaseGame::~CLaserBattleBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


PERROR
CLaserBattleBaseGame::onBankSwitchBackgroundRam(
    void *cLaserBattleBaseGame
)
{
    PERROR               error     = errorSuccess;
    CLaserBattleBaseGame *thisGame  = (CLaserBattleBaseGame *) cLaserBattleBaseGame;
    C2650Cpu             *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // cnt_nav_w
    // |  4  | MPX BKEFF | access background RAM or effect RAM  |
    // m_mpx_bkeff = bool(data & 0x10);
    error = cpu->memoryWrite(0x12001, 0x00);

    return error;
}


PERROR
CLaserBattleBaseGame::onBankSwitchEffectRam(
    void *cLaserBattleBaseGame
)
{
    PERROR               error     = errorSuccess;
    CLaserBattleBaseGame *thisGame  = (CLaserBattleBaseGame *) cLaserBattleBaseGame;
    C2650Cpu             *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // cnt_nav_w
    // |  4  | MPX BKEFF | access background RAM or effect RAM  |
    // m_mpx_bkeff = bool(data & 0x10);
    error = cpu->memoryWrite(0x12001, 0x10);

    return error;
}


PERROR
CLaserBattleBaseGame::onBankSwitchIN0(
    void *cLaserBattleBaseGame
)
{
    PERROR                error     = errorSuccess;
    CLaserBattleBaseGame *thisGame  = (CLaserBattleBaseGame *) cLaserBattleBaseGame;
    C2650Cpu             *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // cnt_eff_w
    // |  7  | MPX P_1/2   | selects input row 2
    error = cpu->memoryWrite(0x12000, 0x00);

    // ct_io_w
    // |  4  | input row select A
    // |  5  | input row select B
    // m_input_mux = (data >> 4) & 0x03;

    if (SUCCESS(error))
    {
        error = cpu->memoryWrite(0x12006, (0x00 << 4));
    }

    return error;
}


PERROR
CLaserBattleBaseGame::onBankSwitchIN1(
    void *cLaserBattleBaseGame
)
{
    PERROR                error     = errorSuccess;
    CLaserBattleBaseGame *thisGame  = (CLaserBattleBaseGame *) cLaserBattleBaseGame;
    C2650Cpu             *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // cnt_eff_w
    // |  7  | MPX P_1/2   | selects input row 2
    error = cpu->memoryWrite(0x12000, 0x00);

    // ct_io_w
    // |  4  | input row select A
    // |  5  | input row select B
    // m_input_mux = (data >> 4) & 0x03;

    if (SUCCESS(error))
    {
        error = cpu->memoryWrite(0x12006, (0x01 << 4));
    }

    return error;
}


PERROR
CLaserBattleBaseGame::onBankSwitchSW1(
    void *cLaserBattleBaseGame
)
{
    PERROR                error     = errorSuccess;
    CLaserBattleBaseGame *thisGame  = (CLaserBattleBaseGame *) cLaserBattleBaseGame;
    C2650Cpu             *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // cnt_eff_w
    // |  7  | MPX P_1/2   | selects input row 2
    error = cpu->memoryWrite(0x12000, 0x00);

    // ct_io_w
    // |  4  | input row select A
    // |  5  | input row select B
    // m_input_mux = (data >> 4) & 0x03;

    if (SUCCESS(error))
    {
        error = cpu->memoryWrite(0x12006, (0x02 << 4));
    }

    return error;
}


PERROR
CLaserBattleBaseGame::onBankSwitchSW2(
    void *cLaserBattleBaseGame
)
{
    PERROR                error     = errorSuccess;
    CLaserBattleBaseGame *thisGame  = (CLaserBattleBaseGame *) cLaserBattleBaseGame;
    C2650Cpu             *cpu       = (C2650Cpu *) thisGame->m_cpu;

    // cnt_eff_w
    // |  7  | MPX P_1/2   | selects input row 2
    error = cpu->memoryWrite(0x12000, 0x00);

    // ct_io_w
    // |  4  | input row select A
    // |  5  | input row select B
    // m_input_mux = (data >> 4) & 0x03;

    if (SUCCESS(error))
    {
        error = cpu->memoryWrite(0x12006, (0x03 << 4));
    }

    return error;
}

