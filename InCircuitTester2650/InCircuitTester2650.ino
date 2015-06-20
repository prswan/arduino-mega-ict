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
#include <LiquidCrystal.h>
#include <main.h>
#include <DFR_Key.h>
#include <zutil.h>

#include <C2650Cpu.h>
#include <CAstroWarsGame.h>
#include <CGalaxiaGame.h>
#include <CQuasarGame.h>

//
// The initial selector to select the game to test.
//
static SELECTOR s_gameSelector[] = {//0123456789abcde
                                    {"- Set Repeat   ",  onSelectConfig, (void*) (&s_repeatSelectTimeInS),             false},
                                    {"Astro Wars  (1)",  onSelectGame,   (void*) (CAstroWarsGame::createInstance),     false},
                                    {"Astro Wars  (2)",  onSelectGame,   (void*) (CAstroWarsGame::createInstanceSet2), false},
                                    {"Astro Wars  (3)",  onSelectGame,   (void*) (CAstroWarsGame::createInstanceSet3), false},
                                    {"Galaxia     (1)",  onSelectGame,   (void*) (CGalaxiaGame::createInstance),       false},
                                    {"Galaxia     (2)",  onSelectGame,   (void*) (CGalaxiaGame::createInstanceSet2),   false},
                                    {"Galaxia     (3)",  onSelectGame,   (void*) (CGalaxiaGame::createInstanceSet3),   false},
                                    {"Galaxia     (4)",  onSelectGame,   (void*) (CGalaxiaGame::createInstanceSet4),   false},
                                    {"Quasar      (1)",  onSelectGame,   (void*) (CQuasarGame::createInstance),        false},
                                    {"Quasar      (2)",  onSelectGame,   (void*) (CQuasarGame::createInstanceSet2),    false},
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

