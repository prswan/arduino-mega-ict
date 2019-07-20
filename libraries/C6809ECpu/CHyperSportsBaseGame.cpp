//
// Copyright (c) 2019, Paul R. Swan
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
#include "CHyperSportsBaseGame.h"
#include "C6809EClockMasterCpu.h"
#include "CRamCheck.h"

//
// Probe Head GND:
//   Konmai-1 GND is pin 20 (actually TSC but so far always GND)
//
// Watchdog Disable:
//   TBD.
//
// Board Designations:
//   's' - Sound board.
//   'c' - CPU board.
//
// CPU Compatibility Notes
//   According to MAME CPU frequency is 1.4MHz giving a half cycle time of 357nS
//   (5 Arduino instructions per half cycle)
//


//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                        "012", "012345"
                                                  {NO_BANK_SWITCH, 0x1000, 0x10BF, 1, 0xFF, "   ", "Sprite"}, // Sprite RAM
                                                  {NO_BANK_SWITCH, 0x10C0, 0x10FF, 1, 0xFF, "   ", "Scroll"}, // Scroll RAM
                                                  {NO_BANK_SWITCH, 0x2000, 0x27FF, 1, 0xFF, "   ", "Video "}, // Video RAM
                                                  {NO_BANK_SWITCH, 0x2800, 0x2FFF, 1, 0xFF, "   ", "Colour"}, // Colour RAM
                                                  {NO_BANK_SWITCH, 0x3000, 0x37FF, 1, 0xFF, "G2 ", "Prog. "}, // 2128 Program RAM
                                                  {NO_BANK_SWITCH, 0x3800, 0x3FFF, 1, 0xFF, "G4 ", "Nv Ram"}, // 6116 Battery backed RAM
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
// This description is used for the byte-wide intensive random access memory test.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                        "012", "012345"
                                                          {NO_BANK_SWITCH, 0x1000, 0x10BF, 1, 0xFF, "   ", "Sprite"}, // Sprite RAM
                                                          {NO_BANK_SWITCH, 0x10C0, 0x10FF, 1, 0xFF, "   ", "Scroll"}, // Scroll RAM
                                                          {NO_BANK_SWITCH, 0x2000, 0x27FF, 1, 0xFF, "   ", "Video "}, // Video RAM
                                                          {NO_BANK_SWITCH, 0x2800, 0x2FFF, 1, 0xFF, "   ", "Colour"}, // Colour RAM
                                                          {NO_BANK_SWITCH, 0x3000, 0x37FF, 1, 0xFF, "   ", "Prog. "}, // 2128 Program RAM
                                                          {NO_BANK_SWITCH, 0x3800, 0x3FFF, 1, 0xFF, "   ", "Nv Ram"}, // Battery backed RAM
                                                          {0}
                                                        }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                              "012", "012345"
                                                      {NO_BANK_SWITCH, 0x1600L, 0xFF, "   ", "DSW2  "}, // DIP Switches 2
                                                      {NO_BANK_SWITCH, 0x1680L, 0xFF, "   ", "System"}, // System
                                                      {NO_BANK_SWITCH, 0x1681L, 0xFF, "   ", "P1_P2 "}, // Control Inputs
                                                      {NO_BANK_SWITCH, 0x1682L, 0xFF, "   ", "P3_P4 "}, // Conreol Inputs
                                                      {NO_BANK_SWITCH, 0x1683L, 0xFF, "   ", "DSW1  "}, // DIP Switches 1
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x1400L, 0x01, 0x00, "   ", "W. Dog"}, // Watchdog Reset
                                                        {NO_BANK_SWITCH, 0x1480L, 0xFF, 0x00, "   ", "ScFlip"}, // Screen Flip
                                                        {NO_BANK_SWITCH, 0x1481L, 0xFF, 0x00, "   ", "SndInt"}, // Sound Interrupt
                                                        {NO_BANK_SWITCH, 0x1483L, 0xFF, 0x00, "   ", "CCntr1"}, // Coin Counter 1
                                                        {NO_BANK_SWITCH, 0x1484L, 0xFF, 0x00, "   ", "CCntr2"}, // Coin Counter 2
                                                        {NO_BANK_SWITCH, 0x1487L, 0xFF, 0x00, "   ", "IntMsk"}, // Interrupt Mask
                                                        {NO_BANK_SWITCH, 0x1500L, 0xFF, 0x00, "   ", "SndDat"}, // Sound Data
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { // "0123456789"
                                                            {NO_CUSTOM_FUNCTION}}; // end of list



CHyperSportsBaseGame::CHyperSportsBaseGame(
    const ROM_REGION *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new C6809EClockMasterCpu(&s_Konami1PinOut);
    m_cpu->idle();

    m_interrupt = ICpu::IRQ0;
    m_interruptAutoVector = true;
}


CHyperSportsBaseGame::~CHyperSportsBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// TBD.
//
PERROR
CHyperSportsBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}


