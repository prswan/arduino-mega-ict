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
#include "CGalaxianBaseGame.h"
#include "CZ80Cpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   Tie CPU board IC 8L (74LS00) pin 10 to GND.
//
// Board Designations:
//   'r' - ROM board.
//   ...everything else main board.
//
// Z80 Compatibility Notes
//   * IO space is not used.
//   * M1 (1st machine cycle) is not used.
//   * Access to the background RAM via VRAM causes the CPU to WAIT when access occurs outside of HBLANK
//     (except when in VBLANK).
//     This gate is Z80 clock input synchronous. It *almost* works as is with the tester but it looks like
//     the HBLANK period is so short that it can shut off the VRAM before the tester has actually got
//     the data off the bus.
//
//     To work around this, in CZ80Cpu we synchronize with HBLANK by waiting for the WAIT to occur and clear
//     before letting the cycle go ahead. This ensure that the WAIT doesn't occur in the middle of the cycle.
//


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0x4000, 0x43FF, 1, 0x0F, " 7P", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x4000, 0x43FF, 1, 0xF0, " 7N", "Prog. "}, // "Program RAM, 2114"
                                                  {NO_BANK_SWITCH, 0x5800, 0x58FF, 1, 0x0F, " 4F", "ObjRam"}, // "Object RAM, 2101, 256 Bytes used."
                                                  {NO_BANK_SWITCH, 0x5800, 0x58FF, 1, 0xF0, " 5F", "ObjRam"}, // "Object RAM, 2101, 256 Bytes used."
                                                  //
                                                  // See note above about access restrictions w.r.t HBLANK & WAIT.
                                                  // These regions are access with special support in the CZ80Cpu triggered via address 0x10xxxx.
                                                  //
                                                  //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x105000, 0x1053FF, 1, 0x0F, " 3H", "BkVRam"}, // "Background VRAM, 2114"
                                                  {NO_BANK_SWITCH, 0x105000, 0x1053FF, 1, 0xF0, " 3F", "BkVRam"}, // "Background VRAM, 2114"
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                        "012", "012345"
                                                          {NO_BANK_SWITCH, 0x4000, 0x43FF, 1, 0xFF, "7PN", "Prog. "}, // "Program RAM, 2114, 7P/7N"
                                                          {NO_BANK_SWITCH, 0x5800, 0x58FF, 1, 0xFF, "45F", "ObjRam"}, // "Object RAM, 2101, 256 Bytes used., 4F/5F"
                                                          //
                                                          // See note above about access restrictions w.r.t HBLANK & WAIT.
                                                          // These regions are access with special support in the CZ80Cpu triggered via address 0x10xxxx.
                                                          //
                                                          //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x105000, 0x1053FF, 1, 0xFF, "3HF", "BkVRam"}, // "Background VRAM, 2114, 3H/3F"
                                                          {0}
                                                        }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                               "012", "012345"
                                                      {NO_BANK_SWITCH, 0x6000L, 0x0C,  " 9D", "IN SW0"}, // Read Inputs (IN0)
                                                      {NO_BANK_SWITCH, 0x6000L, 0xF3,  " 9E", "IN SW0"}, // Read Inputs (IN0)
                                                      {NO_BANK_SWITCH, 0x6800L, 0xC0,  " 9C", "DIPSW1"}, // Read DIP switches (IN1)
                                                      {NO_BANK_SWITCH, 0x6800L, 0x2F,  " 9F", "IN SW1"}, // Read Inputs (IN1)
                                                      {NO_BANK_SWITCH, 0x7000L, 0x0F,  " 9C", "DIPSW "}, // Read DIP switches (IN2)
                                                      {NO_BANK_SWITCH, 0x7800L, 0xFF,  " 8L", "WD Res"}, // Watchdog reset
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                     "012", "012345"
                                                        {NO_BANK_SWITCH, 0x6000L, 0x01, 0x00,  " 9M", "Lamp 1"}, // Player 1 start lamp
                                                        {NO_BANK_SWITCH, 0x6001L, 0x01, 0x00,  " 9M", "Lamp 2"}, // Player 2 start lamp
                                                        {NO_BANK_SWITCH, 0x6002L, 0x01, 0x00,  " 9M", "Lck Ot"}, // Coin lockout
                                                        {NO_BANK_SWITCH, 0x6003L, 0x01, 0x00,  " 9M", "Cntr  "}, // Coin counter
                                                        {NO_BANK_SWITCH, 0x6004L, 0x01, 0x00,  " 9M", "9R1M  "}, // Rack speed?
                                                        {NO_BANK_SWITCH, 0x6005L, 0x01, 0x00,  " 9M", "9R470K"}, // Rack speed?
                                                        {NO_BANK_SWITCH, 0x6006L, 0x01, 0x00,  " 9M", "9R220K"}, // Rack speed?
                                                        {NO_BANK_SWITCH, 0x6007L, 0x01, 0x00,  " 9M", "9R100K"}, // Rack speed?
                                                        {NO_BANK_SWITCH, 0x6800L, 0x01, 0x00,  " 9L", "8R FS1"}, // Rack? sound enable
                                                        {NO_BANK_SWITCH, 0x6801L, 0x01, 0x00,  " 9L", "8S FS2"}, // Rack? sound enable
                                                        {NO_BANK_SWITCH, 0x6802L, 0x01, 0x00,  " 9L", "8T FS3"}, // Rack? sound enable
                                                        {NO_BANK_SWITCH, 0x6803L, 0x01, 0x00,  " 9L", "HitExp"}, // Base hit explosion
                                                        {NO_BANK_SWITCH, 0x6804L, 0x01, 0x00,  " 9L", "n/c   "}, // n/c
                                                        {NO_BANK_SWITCH, 0x6805L, 0x01, 0x00,  " 9L", "Fire  "}, // Base fire
                                                        {NO_BANK_SWITCH, 0x6806L, 0x01, 0x00,  " 9L", "LFO V1"}, // LFO Volume 1
                                                        {NO_BANK_SWITCH, 0x6807L, 0x01, 0x00,  " 9L", "LFO V2"}, // LFO Volume 2
                                                        {NO_BANK_SWITCH, 0x7001L, 0x01, 0x00,  " 9N", "NMI En"}, // NMI enable
                                                        {NO_BANK_SWITCH, 0x7004L, 0x01, 0x00,  " 9N", "Stars "}, // Stars on
                                                        {NO_BANK_SWITCH, 0x7006L, 0x01, 0x00,  " 9N", "Flip X"}, // Flip X
                                                        {NO_BANK_SWITCH, 0x7007L, 0x01, 0x00,  " 9N", "Flip Y"}, // Flip Y
                                                        {NO_BANK_SWITCH, 0x7800L, 0x01, 0x00,  " 9J", "LFO F0"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0x7800L, 0x02, 0x00,  " 9J", "LFO F1"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0x7800L, 0x04, 0x00,  " 9J", "LFO F2"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0x7800L, 0x08, 0x00,  " 9J", "LFO F3"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0x7800L, 0x10, 0x00,  " 9J", "LFO F4"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0x7800L, 0x20, 0x00,  " 9J", "LFO F5"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0x7800L, 0x40, 0x00,  " 9J", "LFO F6"}, // LFO frequency
                                                        {NO_BANK_SWITCH, 0x7800L, 0x80, 0x00,  " 9J", "LFO F7"}, // LFO frequency
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                    "0123456789"
                                                            {CGalaxianBaseGame::clearVideo,       "Clear Vid."},
                                                            {CGalaxianBaseGame::shellMissileTest, "Shell Mis."},
                                                            {NO_CUSTOM_FUNCTION}}; // end of list


CGalaxianBaseGame::CGalaxianBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new CZ80Cpu();
    m_cpu->idle();

    // The VBLANK interrupt is on the NMI pin.
    m_interrupt = ICpu::NMI;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;

}


CGalaxianBaseGame::~CGalaxianBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// This is a specific implementation for Galaxian that uses an externally
// maskable/resetable latch for the VBLANK interrupt on the NMI line.
PERROR
CGalaxianBaseGame::interruptCheck(
)
{
    PERROR error = errorSuccess;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "OK:";

    for (int i = 0 ; i < 4 ; i++)
    {
        // Unmask the interrupt.
        m_cpu->memoryWrite(0x7001L, 0x01);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        1000);
        if (FAILED(error))
        {
            break;
        }

        // Mask the interrupt (also resets the latch)
        m_cpu->memoryWrite(0x7001L, 0x00);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        0);
        if (SUCCESS(error))
        {
            error = errorUnexpected;
            break;
        }
        else
        {
            error = errorSuccess;
        }

        // Unmask the interrupt. Irq should have been cleared.
        m_cpu->memoryWrite(0x7001L, 0x01);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        0);
        if (SUCCESS(error))
        {
            error = errorUnexpected;
            break;
        }
        else
        {
            error = errorSuccess;
        }

        // Mask the interrupt (also resets the latch)
        m_cpu->memoryWrite(0x7001L, 0x00);

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        1000);
        if (SUCCESS(error))
        {
            error = errorUnexpected;
            break;
        }
        else
        {
            error = errorSuccess;
        }
    }

    return error;
}


//
// Clear the video display to blank (background & sprites)
//
PERROR
CGalaxianBaseGame::clearVideo(
    void *context
)
{
    CGalaxianBaseGame *thisGame = (CGalaxianBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT32 count;

    static const UINT32 objRamStart  = 0x5800;
    static const UINT32 objRamLen    = 0x100;
    static const UINT8  objBlankChar = (0x160 >> 5); // 16x16
    static const UINT32 bkVRamStart  = 0x105000;
    static const UINT32 bkVRamLen    = 0x400;
    static const UINT8  bkVBlankChar = (0x160 >> 3); // 8x8

    //
    // Clear the background RAM to a blank character.
    //
    for (count = 0 ; count < bkVRamLen ; count++)
    {
        (void) cpu->memoryWrite(bkVRamStart + count, bkVBlankChar);
    }

    //
    // Clear the object RAM to a blank character.
    //
    for (count = 0 ; count < objRamLen ; count++)
    {
        (void) cpu->memoryWrite(objRamStart + count, objBlankChar);
    }

    return error;
}

//
// Custom function for testing the shell/missile hardware.
//
PERROR
CGalaxianBaseGame::shellMissileTest(
    void *context
)
{
    CGalaxianBaseGame *thisGame = (CGalaxianBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT32 count;

    static const UINT32 objRamStart  = 0x5800;
    static const UINT32 intEnable    = 0x7001L;

    for (count = 0 ; ((count < 10) && !FAILED(error)) ; count++)
    {
        UINT8 position;

        for (position = 0 ; position < 0xE0 ; position++)
        {
            UINT8 projectile;

            //
            // Reset and enable the VBLANK interrupt.
            //
            cpu->memoryWrite(intEnable, 0x00);
            cpu->memoryWrite(intEnable, 0x01);

            error = cpu->waitForInterrupt(thisGame->m_interrupt, true, 1000);
            if (FAILED(error))
            {
                break;
            }

            //
            // Set shell X+Y to be the same to cause a diagonal sweep.
            // This will take 255 VBLANKs and thus a few seconds. The 7
            // shells (white) and 1 missile (yellow) are separated by 1 line
            // to make them easier to see individually.
            //
            // Note that the first 3 white shells match Y-1 and thus on screen
            // there is a diagnonal group of 3, a gap, and then a diagonal
            // group of 4 white shells plus the yellow missile.
            //
            for (projectile = 0 ; projectile < 8 ; projectile++)
            {
                (void) cpu->memoryWrite(objRamStart + 0x60 + 1 + (4 * projectile),
                                        position + (2 * projectile));

                (void) cpu->memoryWrite(objRamStart + 0x60 + 3 + (4 * projectile),
                                        position + (2 * projectile));
            }
        }
    }

    return error;
}

