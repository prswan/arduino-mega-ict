//
// Copyright (c) 2026, patzik
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
#include "CKikiKaikaiBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   Not needed. F018 has been confirmed on a PCB to be the watchdog and the
//   tester writes it at least every 5ms during all bus accesses. Between
//   tests (no bus accesses) the board resets, so after the bus has been idle
//   the watchdog is written until RESET has been released for a while before
//   the access goes ahead, otherwise the first writes can be lost (seen on a
//   PCB with a fixed 250ms wait).
//   The "WD Kick" custom function writes F018 continuously for 10 seconds.
//
// Board Designations:
//   This is the main CPU on the single main board (K1100195A). It is the
//   Z80 next to the A85_16.H18 & A85_17.H16 program ROMs.
//
// Z80 Compatibility Notes
//   * IO space is not used.
//   * M1 (1st machine cycle) is not used.
//
// Notes:
//   The memory map is taken from MAME (src/mame/taito/kikikai.cpp,
//   base_state::main_map) and the IC locations from the PCB layout in
//   the same file. The two TMM2064 at D1 & D3 are the RAM shared with the
//   sound CPU (16KB, the main CPU sees the first 10KB). Which 2KB RAM is at
//   E800 isn't known yet so those regions have no IC location given ("-").
//
//   0000-7FFF ROM H16 1st half
//   8000-BFFF ROM banked 16KB at a time by F000 D0-D2,
//             banks 0-3 are H18 and banks 4-5 are the H16 2nd half
//   C000-E7FF RAM shared with the sound CPU (sound CPU 8000-A7FF)
//   E800-E8FF RAM shared with the MCU (MC6801U4 A85-01 at H8)
//   E900-EFFF RAM
//   F000      W  D0-D2 ROM bank, D5 character bank
//   F008      W  D0 flip (?), D1 MCU reset (0=reset), D2 sound CPU reset (0=reset)
//   F010      R  IN3 - service, tilt, start 1 & 2
//   F018      W  watchdog
//   F800-FFFF RAM for the 4 player sub board link (not tested).
//
//   The joystick & coin inputs are read by the MCU and the DIP switches
//   by the YM2203 on the sound CPU, so they are not in the input list here.
//
//   The sound CPU & MCU are held in reset (F008 = 0) before the shared RAM
//   is tested so they don't interfere with it.
//
// void base_state::main_map(address_map &map)
//

static const UINT32 s_KikiKaikaiBankAddress  = 0xF000L;
static const UINT32 s_KikiKaikaiResetAddress = 0xF008L;
static const UINT32 s_KikiKaikaiMcuRamBase   = 0xE800L;
static const UINT32 s_KikiKaikaiWatchdog     = 0xF018L;

//
// The interval the watchdog is written at during bus accesses, how long the
// bus can be idle before the board may be resetting, how long RESET must then
// be released for before continuing and the maximum time to wait for that.
//
static const unsigned long s_watchdogIntervalInMs = 5;
static const unsigned long s_watchdogIdleInMs     = 50;
static const unsigned long s_watchdogSettleInMs   = 100;
static const unsigned long s_watchdogTimeoutInMs  = 3000;

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                                         "012", "012345"
                                                  {CKikiKaikaiBaseGame::onBankSwitchHoldResets, 0xC000, 0xE7FF, 1, 0xFF, "D13", "Shared"}, // RAM shared with the sound CPU, TMM2064 D1 & D3
                                                  {CKikiKaikaiBaseGame::onBankSwitchHoldResets, 0xE800, 0xE8FF, 1, 0xFF, " - ", "McuRam"}, // RAM shared with the MCU
                                                  {CKikiKaikaiBaseGame::onBankSwitchHoldResets, 0xE900, 0xEFFF, 1, 0xFF, " - ", "Prog. "}, // RAM
                                                  {0}
                                                }; // end of list

//
// No write-only RAM on this platform.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                             "012", "012345"
                                                      {NO_BANK_SWITCH, 0xF010L, 0xFF, " - ", "IN3   "}, // Service, tilt, start
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                  "012", "012345"
                                                        {NO_BANK_SWITCH, 0xF000L, 0x27, 0x00, " - ", "RomBnk"}, // ROM bank (D0-D2) & char bank (D5)
                                                        {NO_BANK_SWITCH, 0xF008L, 0xFF, 0x00, " - ", "Resets"}, // D1 MCU reset, D2 sound CPU reset
                                                        {NO_BANK_SWITCH, 0xF018L, 0xFF, 0x00, " - ", "WD Res"}, // Watchdog
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                      "0123456789"
                                                            {CKikiKaikaiBaseGame::watchdogKick,     "WD Kick   "},
                                                            {CKikiKaikaiBaseGame::cycleTypeSelect,  "Cycle Type"},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CKikiKaikaiBaseGame::CKikiKaikaiBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction),
    m_watchdogTime(0),
    m_inWatchdogWrite(false)
{
    // The RAM shared with the video uses WAIT and needs extra time after it.
    m_z80Cpu = new CZ80ACpu(0,
                            onAddressRemap,
                            this,
                            NO_DATA_REMAP,
                            NULL,
                            CZ80ACpu::CYCLE_TYPE_KIKIKAI);
    m_cpu = m_z80Cpu;
    m_cpu->idle();

    // The VBLANK interrupt is on the INT pin.
    m_interrupt = ICpu::IRQ0;

    // The vector is supplied from the MCU shared RAM, see interruptCheck.
    m_interruptAutoVector = false;
    m_interruptResponse = 0x5A;
}


CKikiKaikaiBaseGame::~CKikiKaikaiBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// The bus check looks at the RESET pin before any bus access, so let the
// board come out of reset first.
//
PERROR
CKikiKaikaiBaseGame::busCheck(
)
{
    (void) watchdogSettle();

    return CGame::busCheck();
}


//
// MAME supplies the interrupt vector from the first byte of the MCU shared
// RAM (mcram_vect_r) so a known vector is placed there first with the MCU
// held in reset. MAME also notes it isn't sure whether the MCU is involved in
// generating the VBLANK interrupt, so a failure here with the MCU held in
// reset isn't conclusive.
//
PERROR
CKikiKaikaiBaseGame::interruptCheck(
)
{
    PERROR error = onBankSwitchHoldResets(this);

    if (SUCCESS(error))
    {
        error = m_cpu->memoryWrite(s_KikiKaikaiMcuRamBase, m_interruptResponse);
    }

    if (SUCCESS(error))
    {
        error = CGame::interruptCheck();
    }

    return error;
}


//
// Write the suspected watchdog (F018) every 1ms for 10 seconds.
//
PERROR
CKikiKaikaiBaseGame::watchdogKick(
    void *cKikiKaikaiBaseGame
)
{
    static const unsigned long runTimeInMs = 10000;
    CKikiKaikaiBaseGame *thisGame = (CKikiKaikaiBaseGame *) cKikiKaikaiBaseGame;
    ICpu                *cpu      = thisGame->m_cpu;
    PERROR error = errorSuccess;
    unsigned long startTime = millis();

    while ((millis() - startTime) < runTimeInMs)
    {
        error = cpu->memoryWrite(s_KikiKaikaiWatchdog, 0x00);
        if (FAILED(error))
        {
            break;
        }

        delay(1);
    }

    return error;
}


PERROR
CKikiKaikaiBaseGame::onBankSwitchHoldResets(
    void *cKikiKaikaiBaseGame
)
{
    CKikiKaikaiBaseGame *thisGame = (CKikiKaikaiBaseGame *) cKikiKaikaiBaseGame;
    ICpu                *cpu      = thisGame->m_cpu;

    return cpu->memoryWrite(s_KikiKaikaiResetAddress, 0x00);
}


//
// Cycle through the Z80 bus cycle implementations, showing the new one.
//
PERROR
CKikiKaikaiBaseGame::cycleTypeSelect(
    void *cKikiKaikaiBaseGame
)
{
    CKikiKaikaiBaseGame *thisGame = (CKikiKaikaiBaseGame *) cKikiKaikaiBaseGame;

    switch (thisGame->m_z80Cpu->getCycleType())
    {
        case CZ80ACpu::CYCLE_TYPE_KIKIKAI :
        {
            thisGame->m_z80Cpu->setCycleType(CZ80ACpu::CYCLE_TYPE_DEFAULT);
            errorCustom->description = "OK: Default";
            break;
        }

        case CZ80ACpu::CYCLE_TYPE_DEFAULT :
        {
            thisGame->m_z80Cpu->setCycleType(CZ80ACpu::CYCLE_TYPE_PUCKMAN);
            errorCustom->description = "OK: Puckman";
            break;
        }

        case CZ80ACpu::CYCLE_TYPE_PUCKMAN :
        {
            thisGame->m_z80Cpu->setCycleType(CZ80ACpu::CYCLE_TYPE_CRAZYKONG);
            errorCustom->description = "OK: CrazyKong";
            break;
        }

        case CZ80ACpu::CYCLE_TYPE_CRAZYKONG :
        {
            thisGame->m_z80Cpu->setCycleType(CZ80ACpu::CYCLE_TYPE_LADYBUG);
            errorCustom->description = "OK: Ladybug";
            break;
        }

        default :
        {
            thisGame->m_z80Cpu->setCycleType(CZ80ACpu::CYCLE_TYPE_KIKIKAI);
            errorCustom->description = "OK: KiKiKaiKai";
            break;
        }
    }

    errorCustom->code = ERROR_SUCCESS;

    return errorCustom;
}


//
// No address remapping is done, this is used to write the watchdog at least
// every s_watchdogIntervalInMs during bus accesses. If the bus has been idle
// long enough for the board to be resetting then it is given time to come
// out of reset first. The watchdog writes come back through here so it is
// guarded against recursion.
//
PERROR
CKikiKaikaiBaseGame::onAddressRemap(
    void   *cKikiKaikaiBaseGame,
    UINT32  addressIn,
    UINT32 *addressOut
)
{
    CKikiKaikaiBaseGame *thisGame = (CKikiKaikaiBaseGame *) cKikiKaikaiBaseGame;
    PERROR error = errorSuccess;

    *addressOut = addressIn;

    if (!thisGame->m_inWatchdogWrite)
    {
        unsigned long idleTime = millis() - thisGame->m_watchdogTime;

        if (idleTime >= s_watchdogIdleInMs)
        {
            error = thisGame->watchdogSettle();
        }
        else if (idleTime >= s_watchdogIntervalInMs)
        {
            thisGame->m_inWatchdogWrite = true;

            error = thisGame->m_cpu->memoryWrite(s_KikiKaikaiWatchdog, 0x00);

            thisGame->m_watchdogTime = millis();
            thisGame->m_inWatchdogWrite = false;
        }

        //
        // A reset during a test silently loses writes, so report it instead.
        //
        if (SUCCESS(error) && thisGame->m_z80Cpu->isResetActive())
        {
            errorCustom->code = ERROR_FAILED;
            errorCustom->description = "E: RESET active";
            error = errorCustom;
        }
    }

    return error;
}


PERROR
CKikiKaikaiBaseGame::watchdogSettle(
)
{
    PERROR error = errorSuccess;
    unsigned long startTime = millis();
    unsigned long releaseTime = startTime;

    m_inWatchdogWrite = true;

    //
    // Write the watchdog until RESET has been released for s_watchdogSettleInMs.
    // If it's never released the access goes ahead anyway and the bus check
    // will report the RESET pin.
    //
    while ((millis() - startTime) < s_watchdogTimeoutInMs)
    {
        error = m_cpu->memoryWrite(s_KikiKaikaiWatchdog, 0x00);
        if (FAILED(error))
        {
            break;
        }

        if (m_z80Cpu->isResetActive())
        {
            releaseTime = millis();
        }
        else if ((millis() - releaseTime) >= s_watchdogSettleInMs)
        {
            break;
        }

        delay(1);
    }

    m_watchdogTime = millis();
    m_inWatchdogWrite = false;

    return error;
}


PERROR
CKikiKaikaiBaseGame::romBankSwitch(
    void *cKikiKaikaiBaseGame,
    UINT8 bank
)
{
    CKikiKaikaiBaseGame *thisGame = (CKikiKaikaiBaseGame *) cKikiKaikaiBaseGame;
    ICpu                *cpu      = thisGame->m_cpu;

    return cpu->memoryWrite(s_KikiKaikaiBankAddress, bank);
}


PERROR
CKikiKaikaiBaseGame::onBankSwitchRomBank0(
    void *cKikiKaikaiBaseGame
)
{
    return romBankSwitch(cKikiKaikaiBaseGame, 0);
}


PERROR
CKikiKaikaiBaseGame::onBankSwitchRomBank1(
    void *cKikiKaikaiBaseGame
)
{
    return romBankSwitch(cKikiKaikaiBaseGame, 1);
}


PERROR
CKikiKaikaiBaseGame::onBankSwitchRomBank2(
    void *cKikiKaikaiBaseGame
)
{
    return romBankSwitch(cKikiKaikaiBaseGame, 2);
}


PERROR
CKikiKaikaiBaseGame::onBankSwitchRomBank3(
    void *cKikiKaikaiBaseGame
)
{
    return romBankSwitch(cKikiKaikaiBaseGame, 3);
}


PERROR
CKikiKaikaiBaseGame::onBankSwitchRomBank4(
    void *cKikiKaikaiBaseGame
)
{
    return romBankSwitch(cKikiKaikaiBaseGame, 4);
}


PERROR
CKikiKaikaiBaseGame::onBankSwitchRomBank5(
    void *cKikiKaikaiBaseGame
)
{
    return romBankSwitch(cKikiKaikaiBaseGame, 5);
}

