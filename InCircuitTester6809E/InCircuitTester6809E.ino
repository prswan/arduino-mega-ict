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

#include <C6809ECpu.h>
#include <CStarWarsGame.h>
#include <CDefenderGame.h>
#include <CWilliamsGame.h>

//
// The initial selector to select the game to test.
//
static SELECTOR s_gameSelector[] = {
//   "0123456789abcde"
    {"- Set Repeat   ",  onSelectConfig, (void*) (&s_repeatSelectTimeInS),                            false},
    {"- Set Error    ",  onSelectConfig, (void*) (&s_repeatIgnoreError),                              false},
    {"Star Wars      ",  onSelectGame,   (void*) (CStarWarsGame::createInstance),                     false},
    {"Star Wars (O)  ",  onSelectGame,   (void*) (CStarWarsGame::createInstanceSetO),                 false},
    {"Defender Red   ",  onSelectGame,   (void*) (CDefenderGame::createInstanceDefenderRed),          false},
    {"Defender Green ",  onSelectGame,   (void*) (CDefenderGame::createInstanceDefenderGreen),        false},
    {"Defender Blue  ",  onSelectGame,   (void*) (CDefenderGame::createInstanceDefenderBlue),         false},
    {"Defender White ",  onSelectGame,   (void*) (CDefenderGame::createInstanceDefenderWhite),        false},
    {"Stargate       ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceStargate),             false},
    {"Robotron TieDie",  onSelectGame,   (void*) (CWilliamsGame::createInstanceRobotronTieDie),       false},
    {"Robotron Blue  ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceRobotronBlue),         false},
//  {"Robotron Patch ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceRobotronBluePatched),  false},
//  {"Robotron Pause ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceRobotronBluePause),    false},
    {"Robotron Y/O   ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceRobotronYellowOrange), false},
    {"Joust Green    ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceJoustWhiteGreen),      false},
//  {"Joust Pause    ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceJoustPause),           false},
//  {"Joust Red      ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceJoustSolidRed),        false},
//  {"Joust Yellow   ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceJoustSolidYellow),     false},
    {"Bubbles        ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceBubbles),              false},
//  {"Bubbles Red    ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceBubblesSolidRed),      false},
//  {"Bubbles Proto  ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceBubblesProto),         false},
    {"Splat!         ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceSplat),                false},
//  {"Splat! SC1     ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceSplatSC1),             false},
    {"Sinistar Rev 3 ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceSinistarRev3),         false},
//  {"Sinistar Rev 2 ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceSinistarRev2),         false},
//  {"Sinistar Proto ",  onSelectGame,   (void*) (CWilliamsGame::createInstanceSinistarProto),        false},
    { 0, 0 }
};

//
// Some less used Williams ROM sets have been commented out as Arduino runs low on dynamic memory for global variables if they are all included.
// To enable a specific variant uncomment above and in /libraries/C6809ECpu/CWilliamsGame.h and /libraries/C6809ECpu/CWilliamsGame.cpp
//


void setup()
{
    mainSetup(s_gameSelector);
}

void loop()
{
    mainLoop();
}

