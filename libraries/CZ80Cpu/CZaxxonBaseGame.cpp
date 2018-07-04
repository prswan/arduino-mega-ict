//
// Copyright (c) 2018, Paul R. Swan
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
#include "CZaxxonBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   TBD
//
// Board Designations:
//   'c' - Main/CPU/Sound PCB
//   'v' - Video PCB
//
// Z80 Compatibility Notes
//   * IO space is not used.
//   * M1 (1st machine cycle) is not used.
//

//
// Base addresses for the 8255
//
static const UINT32 s_8255IoBase = 0x00e03cL;

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                            "012", "012345"
                                                  {NO_BANK_SWITCH, 0x006000, 0x0067FF, 1, 0xFF, "c37", "Prog. "}, // Program RAM, TMM2016
                                                  {NO_BANK_SWITCH, 0x006800, 0x006FFF, 1, 0xFF, "c38", "Prog. "}, // Program RAM, TMM2016
                                                  {NO_BANK_SWITCH, 0x108000, 0x1083FF, 1, 0x0F, "c70", "VRAM  "}, // Video RAM, Z80 WAIT sync
                                                  {NO_BANK_SWITCH, 0x108000, 0x1083FF, 1, 0xF0, "c71", "VRAM  "}, // Video RAM, Z80 WAIT sync
                                                  {NO_BANK_SWITCH, 0x00A000, 0x00A0FF, 1, 0x0F, "v36", "TGT   "}, // Sprite RAM
                                                  {NO_BANK_SWITCH, 0x00A000, 0x00A0FF, 1, 0xF0, "v17", "TGT   "}, // Sprite RAM
                                                  {0}
                                                }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionByteOnly[] PROGMEM = { //                                            "012", "012345"
                                                          {NO_BANK_SWITCH, 0x006000, 0x0067FF, 1, 0xFF, "c37", "Prog. "}, // Program RAM, TMM2016
                                                          {NO_BANK_SWITCH, 0x006800, 0x006FFF, 1, 0xFF, "c38", "Prog. "}, // Program RAM, TMM2016
                                                          {NO_BANK_SWITCH, 0x108000, 0x1083FF, 1, 0xFF, " - ", "VRAM  "}, // Video RAM, Z80 WAIT sync
                                                          {NO_BANK_SWITCH, 0x00A000, 0x00A0FF, 1, 0xFF, " - ", "TGT   "}, // Sprite RAM
                                                          {0}
                                                        }; // end of list

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = {{0}};

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                              "012", "012345"
                                                      {NO_BANK_SWITCH, 0xC000, 0xFF,  "IN1", "InPly1"}, // Player 1 inputs
                                                      {NO_BANK_SWITCH, 0xC001, 0xFF,  "IN2", "InPly2"}, // Player 2 inputs
                                                      {NO_BANK_SWITCH, 0xC002, 0xFF,  "SW1", "DipSw1"}, // DIP Switches
                                                      {NO_BANK_SWITCH, 0xC003, 0xFF,  "SW2", "DipSw2"}, // DIP Switches
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                "012", "012345"
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x0, 0x01, 0xFF,  "c23", "Ship A"},
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x0, 0x02, 0xFF,  "c23", "Ship B"},
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x0, 0x0C, 0xFF,  "c23", "ShipC0"}, // Note the LS139
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x0, 0x08, 0xFF,  "c23", "ShipD1"}, // Note the LS139
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x0, 0x10, 0xFF,  "c23", "Home M"},
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x0, 0x20, 0xFF,  "c23", "Base M"},
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x0, 0x40, 0xFF,  "c23", "Laser "},
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x0, 0x80, 0xFF,  "c23", "Battle"},
                                                        //
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x1, 0x10, 0xFF,  "c23", "Sm Exp"},
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x1, 0x20, 0xFF,  "c23", "Md Exp"},
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x1, 0x80, 0xFF,  "c23", "Cannon"},
                                                        //
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x2, 0x01, 0xFF,  "c23", "Shot  "},
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x2, 0x04, 0xFF,  "c23", "Alrm 1"},
                                                        {NO_BANK_SWITCH, s_8255IoBase + 0x2, 0x08, 0xFF,  "c23", "Alrm 2"},
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CZaxxonBaseGame::CZaxxonBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegionByteOnly,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new CZ80ACpu();
    m_cpu->idle();

    //
    // The TEST switch is connected to the NMI pin.
    // The VBLANK interrupt is on the INT pin.
    //
    m_interrupt = ICpu::IRQ0;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;

    //
    // Because the 8255 resets the output latches low when the mode
    // is changed and the sound triggers are active low, reprogramming
    // the 8255 on every output write causes the sounds to trigger.
    // The same artifact can be heard when the game boots.
    // Therefore, we setup the 8255 here as part of platform initialization.
    //
    //  U23 - Sound Outputs
    //   - Port A (All) Mode 0 Output
    //   - Port B (All) Mode 0 Output
    //   - Port C (Lo)  Mode 0 Output
    //   - Port C (Hi)  Mode 0 Output
    //
    //     == 1 0000 000 == 0x80
    //
    m_cpu->memoryWrite(s_8255IoBase + 0x3, 0x80);

    m_cpu->memoryWrite(s_8255IoBase + 0x0, 0xFF);
    m_cpu->memoryWrite(s_8255IoBase + 0x1, 0xFF);
    m_cpu->memoryWrite(s_8255IoBase + 0x2, 0xFF);

}


CZaxxonBaseGame::~CZaxxonBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


// TBD
PERROR
CZaxxonBaseGame::interruptCheck(
)
{
    return errorNotImplemented;
}


