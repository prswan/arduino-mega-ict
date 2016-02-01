//
// Copyright (c) 2015, Paul R. Swan
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include <LiquidCrystal.h>
#include <main.h>
#include <DFR_Key.h>
#include <zutil.h>

#include <C6802Cpu.h>
#include <CWmsSoundGame.h>

//
// The initial selector to select the game to test.
//
static SELECTOR s_gameSelector[] = {//0123456789abcde
                                    {"- Set Repeat   ",  onSelectConfig, (void*) (&s_repeatSelectTimeInS),                      false},
                                    {"- Set Error    ",  onSelectConfig, (void*) (&s_repeatIgnoreError),                        false},
                                    {"Defender: ROM1 ",  onSelectGame,   (void*) (CWmsSoundGame::createInstanceVideoSoundROM1), false},
                                    {"Stargate: ROM2 ",  onSelectGame,   (void*) (CWmsSoundGame::createInstanceVideoSoundROM2), false},
                                    {"Robotron: ROM3 ",  onSelectGame,   (void*) (CWmsSoundGame::createInstanceVideoSoundROM3), false},
                                    {"Joust: ROM4    ",  onSelectGame,   (void*) (CWmsSoundGame::createInstanceVideoSoundROM4), false},
                                    {"Sinistar: ROM9 ",  onSelectGame,   (void*) (CWmsSoundGame::createInstanceVideoSoundROM9), false},
                                    { 0, 0 }
                                   };

void setup()
{
  mainSetup(s_gameSelector);
}

void loop()
{
  mainLoop();
}

