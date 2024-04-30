//
// Copyright (c) 2016, Paul R. Swan
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
#include <C6502GenericGame.h>
#include <CAsteroidsGame.h>
#include <CAsteroidsDlxGame.h>
#include <CAstroFighterGame.h>
#include <CGravitarGame.h>
#include <CMarbleMadnessSoundGame.h>
#include <CSpaceDuelGame.h>
#include <CTempestGame.h>
#include <CTomahawk777Game.h>
#include <CVanguardGame.h>

//
// The initial selector to select the game to test.
//
static const SELECTOR s_gameSelector[] PROGMEM = {//0123456789abcde
                                                  {"Generic 2716   ",  onSelectGeneric, (void*) (C6502GenericGame::createInstance2716),             false},
                                                  {"Generic 2732   ",  onSelectGeneric, (void*) (C6502GenericGame::createInstance2732),             false},
                                                  {"Asteroids (1)  ",  onSelectGame,    (void*) (CAsteroidsGame::createInstanceAsteroidsSet1),      false},
                                                  {"Asteroids (2)  ",  onSelectGame,    (void*) (CAsteroidsGame::createInstanceAsteroidsSet2),      false},
                                                  {"Asteroids (4)  ",  onSelectGame,    (void*) (CAsteroidsGame::createInstanceAsteroidsSet4),      false},
                                                  {"AsteroidsDlx(1)",  onSelectGame,    (void*) (CAsteroidsDlxGame::createInstanceAstDlxSet1),      false},
                                                  {"AsteroidsDlx(2)",  onSelectGame,    (void*) (CAsteroidsDlxGame::createInstanceAstDlxSet2),      false},
                                                  {"AsteroidsDlx(3)",  onSelectGame,    (void*) (CAsteroidsDlxGame::createInstanceAstDlxSet3),      false},
                                                  {"Astro F.(2)    ",  onSelectGame,    (void*) (CAstroFighterGame::createInstanceSet2),            false},
                                                  {"Astro F.(2) clk",  onSelectGame,    (void*) (CAstroFighterGame::createInstanceClockMasterSet2), false},
                                                  {"Black Widow    ",  onSelectGame,    (void*) (CGravitarGame::createInstanceBlackWidowSet),       false},
                                                  {"Gravitar (1)   ",  onSelectGame,    (void*) (CGravitarGame::createInstanceGravitarSet1),        false},
                                                  {"Gravitar (2)   ",  onSelectGame,    (void*) (CGravitarGame::createInstanceGravitarSet2),        false},
                                                  {"Gravitar (3)   ",  onSelectGame,    (void*) (CGravitarGame::createInstanceGravitarSet3),        false},
                                                  {"MarbMad Snd(42)",  onSelectGame,    (void*) (CMarbleMadnessSoundGame::createInstanceRev42),     false},
                                                  {"Space Duel (1) ",  onSelectGame,    (void*) (CSpaceDuelGame::createInstanceSpaceDuelSet1),      false},
                                                  {"Space Duel (2) ",  onSelectGame,    (void*) (CSpaceDuelGame::createInstanceSpaceDuelSet2),      false},
                                                  {"T-777   (5)    ",  onSelectGame,    (void*) (CTomahawk777Game::createInstanceSet5),             false},
                                                  {"T-777   (5) clk",  onSelectGame,    (void*) (CTomahawk777Game::createInstanceClockMasterSet5),  false},
                                                  {"Tempest (1)    ",  onSelectGame,    (void*) (CTempestGame::createInstanceTempestSet1),          false},
                                                  {"Tempest (2)    ",  onSelectGame,    (void*) (CTempestGame::createInstanceTempestSet2),          false},
                                                  {"Tempest (3)    ",  onSelectGame,    (void*) (CTempestGame::createInstanceTempestSet3),          false},
                                                  {"Vanguard Cen(1)",  onSelectGame,    (void*) (CVanguardGame::createInstanceCenturiSet1),         false},
                                                  {"Vanguard Ger(1)",  onSelectGame,    (void*) (CVanguardGame::createInstanceGermanSet1),          false},
                                                  { 0, 0 }
                                                 };


