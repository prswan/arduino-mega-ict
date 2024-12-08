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
#include "CCvsBaseGame.h"
#include "C2650Cpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   2650 GND Pin 21
//
// Watchdog Disable:
//   I didn't see a watchdog implemented, RESET wasn't pulsing.
//

//
// RAM region is the same for all versions.
// Taken from MAME Cvs.cpp
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                                    "012", "012345"
                                                  {NO_BANK_SWITCH,                   0x1C00,      0x1FFF,      1, 0x0F, "  ?", "System"}, // SYSTEM RAM
                                                  {NO_BANK_SWITCH,                   0x1C00,      0x1FFF,      1, 0xF0, "  ?", "System"}, // SYSTEM RAM
                                                  {CCvsBaseGame::onBankSwitchFlagLo, 0x1800,      0x1BFF,      1, 0x0F, "A  ", "Attri."}, // ATTRIBUTE RAM 32X28
                                                  {CCvsBaseGame::onBankSwitchFlagLo, 0x1800,      0x1BFF,      1, 0xF0, "A  ", "Attri."}, // ATTRIBUTE RAM 32X28
                                                  {CCvsBaseGame::onBankSwitchFlagHi, 0x1800,      0x1BFF,      1, 0x0F, "S  ", "Screen"}, // SCREEN RAM 32X28
                                                  {CCvsBaseGame::onBankSwitchFlagHi, 0x1800,      0x1BFF,      1, 0xF0, "S  ", "Screen"}, // SCREEN RAM 32X28
                                                  {CCvsBaseGame::onBankSwitchFlagLo, 0x1700+0x00, 0x1700+0x2D, 1, 0xFF, "12F", "2636-1"}, // 2636 PVI 1
                                                  {CCvsBaseGame::onBankSwitchFlagLo, 0x1600+0x00, 0x1600+0x2D, 1, 0xFF, "11F", "2636-2"}, // 2636 PVI 2
                                                  {CCvsBaseGame::onBankSwitchFlagLo, 0x1500+0x00, 0x1500+0x2D, 1, 0xFF, " 9F", "2636-3"}, // 2636 PVI 3
                                                  {CCvsBaseGame::onBankSwitchFlagHi, 0x1700,      0x17FF,      1, 0xFF, "C  ", "Char 1"}, // CHARACTER RAM 1 256BYTES OF 1K
                                                  {CCvsBaseGame::onBankSwitchFlagHi, 0x1600,      0x16FF,      1, 0xFF, "C  ", "Char 2"}, // CHARACTER RAM 2 256BYTES OF 1K
                                                  {CCvsBaseGame::onBankSwitchFlagHi, 0x1500,      0x15FF,      1, 0xFF, "C  ", "Char 3"}, // CHARACTER RAM 3 256BYTES OF 1K
                                                  {CCvsBaseGame::onBankSwitchFlagLo, 0x1400,      0x14FF,      1, 0x0F, "B  ", "Bullet"}, // BULLET RAM
                                                  {CCvsBaseGame::onBankSwitchFlagLo, 0x1400,      0x14FF,      1, 0xF0, "B  ", "Bullet"}, // BULLET RAM
                                                  {CCvsBaseGame::onBankSwitchFlagHi, 0x1400,      0x140F,      1, 0x0F, "P  ", "Pallet"}, // PALETTE RAM 16BYTES
                                                  {CCvsBaseGame::onBankSwitchFlagHi, 0x1400,      0x140F,      1, 0xF0, "P  ", "Pallet"}, // PALETTE RAM 16BYTES
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all versions.
// This description is used for the byte-wide intensive random access memory test.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                                                    "012", "012345"
                                                          {NO_BANK_SWITCH,                   0x1C00,      0x1FFF,      1, 0xFF, "  ?", "System"}, // SYSTEM RAM
                                                          {CCvsBaseGame::onBankSwitchFlagLo, 0x1800,      0x1BFF,      1, 0xFF, "A  ", "Attri."}, // ATTRIBUTE RAM 32X28
                                                          {CCvsBaseGame::onBankSwitchFlagHi, 0x1800,      0x1BFF,      1, 0xFF, "S  ", "Screen"}, // SCREEN RAM 32X28
                                                          {CCvsBaseGame::onBankSwitchFlagLo, 0x1700+0x00, 0x1700+0x2D, 1, 0xFF, "12F", "2636-1"}, // 2636 PVI 1
                                                          {CCvsBaseGame::onBankSwitchFlagLo, 0x1600+0x00, 0x1600+0x2D, 1, 0xFF, "11F", "2636-2"}, // 2636 PVI 2
                                                          {CCvsBaseGame::onBankSwitchFlagLo, 0x1500+0x00, 0x1500+0x2D, 1, 0xFF, " 9F", "2636-3"}, // 2636 PVI 3
                                                          {CCvsBaseGame::onBankSwitchFlagHi, 0x1700,      0x17FF,      1, 0xFF, "C  ", "Char 1"}, // CHARACTER RAM 1 256BYTES OF 1K
                                                          {CCvsBaseGame::onBankSwitchFlagHi, 0x1600,      0x16FF,      1, 0xFF, "C  ", "Char 2"}, // CHARACTER RAM 2 256BYTES OF 1K
                                                          {CCvsBaseGame::onBankSwitchFlagHi, 0x1500,      0x15FF,      1, 0xFF, "C  ", "Char 3"}, // CHARACTER RAM 3 256BYTES OF 1K
                                                          {CCvsBaseGame::onBankSwitchFlagLo, 0x1400,      0x14FF,      1, 0xFF, "B  ", "Bullet"}, // BULLET RAM
                                                          {CCvsBaseGame::onBankSwitchFlagHi, 0x1400,      0x140F,      1, 0xFF, "P  ", "Pallet"}, // PALETTE RAM 16BYTES
                                                          {0}
                                                        }; // end of list

//
// This region allows the PVI's to be initialized.
// If the PVI's are not initialized, the screen ends up all white such that no
// other graphics can be seen.
//

static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                                           "012", "012345"
                                                           {CCvsBaseGame::onBankSwitchFlagLo,  0x1500, 0x15FF, 1, 0xFF, " 9F", "2636-1"}, // 2636 PVI 1
                                                           {CCvsBaseGame::onBankSwitchFlagLo,  0x1600, 0x16FF, 1, 0xFF, "11F", "2636-2"}, // 2636 PVI 2
                                                           {CCvsBaseGame::onBankSwitchFlagLo,  0x1700, 0x17FF, 1, 0xFF, "12F", "2636-3"}, // 2636 PVI 3
                                                           {0}
                                                         }; // end of list

//
// Input region is the same for all versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                               "012", "012345"
                                                      {NO_BANK_SWITCH, 0x14000L, 0xFF, "  ?", "Coll.C"}, // DATA - Collision clear (no data)
                                                      {NO_BANK_SWITCH, 0x12000L, 0xFF, "  ?", "CN2 1 "}, // EXT - STROBE CN2 1
                                                      {NO_BANK_SWITCH, 0x12001L, 0xFF, "  ?", "CN2 11"}, // EXT - STROBE CN2 11
                                                      // TODO: there are more definitions for port strobes and char ram selects
                                                      {NO_BANK_SWITCH, 0x12006L, 0xFF, "  ?", "SW 3  "}, // EXT - Dipswitch SW3
                                                      {NO_BANK_SWITCH, 0x12007L, 0xFF, "  ?", "SW 2  "}, // EXT - Dipswitch SW2
                                                      {NO_BANK_SWITCH, 0x10000L, 0xFF, "  ?", "Coll.D"}, // CTRL - Collision detect
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x14000L, 0x01, 0x00, "  ?", "Stars "}, // DATA - D0 = STARS ON
                                                        {NO_BANK_SWITCH, 0x14000L, 0x02, 0x00, "  ?", "BrRght"}, // DATA - D1 = SHADE BRIGHTER TO RIGHT
                                                        {NO_BANK_SWITCH, 0x14000L, 0x04, 0x00, "  ?", "ScrRot"}, // DATA - D2 = SCREEN ROTATE
                                                        {NO_BANK_SWITCH, 0x14000L, 0x08, 0x00, "  ?", "BrLeft"}, // DATA - D3 = SHADE BRIGHTER TO LEFT
                                                        {NO_BANK_SWITCH, 0x14000L, 0x10, 0x00, "  ?", "Lamp 1"}, // DATA - D4 = LAMP 1 (CN1 1)
                                                        {NO_BANK_SWITCH, 0x14000L, 0x20, 0x00, "  ?", "Lamp 2"}, // DATA - D5 = LAMP 2 (CN1 2)
                                                        {NO_BANK_SWITCH, 0x14000L, 0x40, 0x00, "  ?", "BrBott"}, // DATA - D6 = SHADE BRIGHTER TO BOTTOM
                                                        {NO_BANK_SWITCH, 0x14000L, 0x80, 0x00, "  ?", "BrTop "}, // DATA - D7 = SHADE BRIGHTER TO TOP
                                                        {NO_BANK_SWITCH, 0x12000L, 0x05, 0x00, "  ?", "Scrl05"}, // EXT - Scroll register
                                                        {NO_BANK_SWITCH, 0x12000L, 0x0A, 0x00, "  ?", "Scrl0A"}, // EXT - Scroll register
                                                        {NO_BANK_SWITCH, 0x12000L, 0x50, 0x00, "  ?", "Scrl50"}, // EXT - Scroll register
                                                        {NO_BANK_SWITCH, 0x12000L, 0xA0, 0x00, "  ?", "ScrlA0"}, // EXT - Scroll register
                                                        {NO_BANK_SWITCH, 0x10000L, 0x55, 0x00, "  ?", "Snd 55"}, // CTRL
                                                        {NO_BANK_SWITCH, 0x10000L, 0xAA, 0x00, "  ?", "Snd AA"}, // CTRL
                                                        // Star colour specifically
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1407, 0xFF, 0x00, "P  ", "StrCFF"}, // Star colour in the palette
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1407, 0x55, 0x00, "P  ", "StrC55"}, // Star colour in the palette
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1407, 0xAA, 0x00, "P  ", "StrCAA"}, // Star colour in the palette
                                                        // Palette RAM
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1400, 0xFF, 0x00, "P  ", "00 FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1401, 0xFF, 0x00, "P  ", "01 FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1402, 0xFF, 0x00, "P  ", "02 FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1403, 0xFF, 0x00, "P  ", "03 FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1404, 0xFF, 0x00, "P  ", "04 FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1405, 0xFF, 0x00, "P  ", "05 FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1406, 0xFF, 0x00, "P  ", "06 FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1407, 0xFF, 0x00, "P  ", "07 FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1408, 0xFF, 0x00, "P  ", "08 FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x1409, 0xFF, 0x00, "P  ", "09 FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x140A, 0xFF, 0x00, "P  ", "0A FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x140B, 0xFF, 0x00, "P  ", "0B FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x140C, 0xFF, 0x00, "P  ", "0C FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x140D, 0xFF, 0x00, "P  ", "0D FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x140E, 0xFF, 0x00, "P  ", "0E FF "},
                                                        {CCvsBaseGame::onBankSwitchFlagHi, 0x140F, 0xFF, 0x00, "P  ", "0F FF "},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CCvsBaseGame::CCvsBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
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

    // The vector is hard coded.
    m_interruptResponse = 0x03;

}


CCvsBaseGame::~CCvsBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


PERROR
CCvsBaseGame::onBankSwitchFlagLo(
    void *cCvsBaseGame
)
{
    PERROR        error     = errorSuccess;
    CCvsBaseGame *thisGame  = (CCvsBaseGame *) cCvsBaseGame;
    C2650Cpu     *cpu       = (C2650Cpu *) thisGame->m_cpu;

    cpu->flagWrite(LOW);

    return error;
}


PERROR
CCvsBaseGame::onBankSwitchFlagHi(
    void *cCvsBaseGame
)
{
    PERROR        error     = errorSuccess;
    CCvsBaseGame *thisGame  = (CCvsBaseGame *) cCvsBaseGame;
    C2650Cpu     *cpu       = (C2650Cpu *) thisGame->m_cpu;

    cpu->flagWrite(HIGH);

    return error;
}

