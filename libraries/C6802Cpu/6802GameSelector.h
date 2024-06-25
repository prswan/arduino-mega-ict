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
#include <C6802GenericGame.h>
#include <CCatnMouseSoundGame.h>
#include <CMoneyMoneySoundGame.h>
#include <CWmsSoundGame.h>

//
// The initial selector to select the game to test.
//
static const SELECTOR s_gameSelector[] PROGMEM = {//0123456789abcde
                                                  {"Generic 2716   ",  onSelectGeneric, (void*) (C6802GenericGame::createInstance2716),              false},
                                                  {"Generic 2732   ",  onSelectGeneric, (void*) (C6802GenericGame::createInstance2732),              false},
                                                  {"Cat'n Mouse (S)",  onSelectGame,    (void*) (CCatnMouseSoundGame::createInstanceSet1),          false},
                                                  {"Money Money (S)",  onSelectGame,    (void*) (CMoneyMoneySoundGame::createInstanceSet1),          false},
                                                  {"Defender       ",  onSelectGame,    (void*) (CWmsSoundGame::createInstanceDefenderSound),        false},
                                                  {"Stargate       ",  onSelectGame,    (void*) (CWmsSoundGame::createInstanceStargateSound),        false},
                                                  {"Robotron       ",  onSelectGame,    (void*) (CWmsSoundGame::createInstanceRobotronSound),        false},
                                                  {"Joust          ",  onSelectGame,    (void*) (CWmsSoundGame::createInstanceJoustSound),           false},
                                                  {"Bubbles        ",  onSelectGame,    (void*) (CWmsSoundGame::createInstanceBubblesSound),         false},
                                                  {"Sinistar Main  ",  onSelectGame,    (void*) (CWmsSoundGame::createInstanceSinistarMainSound),    false},
                                                  {"Sinistar Aux   ",  onSelectGame,    (void*) (CWmsSoundGame::createInstanceSinistarAuxSound),     false},
                                                  {"Splat          ",  onSelectGame,    (void*) (CWmsSoundGame::createInstanceSplatSound),           false},
                                                  {"Firepower      ",  onSelectGame,    (void*) (CWmsSoundGame::createInstanceFirepowerSound),       false},
                                                  {"Defender Pin   ",  onSelectGame,    (void*) (CWmsSoundGame::createInstanceDefenderPinballSound), false},
                                                  {"Hyperball      ",  onSelectGame,    (void*) (CWmsSoundGame::createInstanceHyperballSound),       false},
                                                  {"Varkon         ",  onSelectGame,    (void*) (CWmsSoundGame::createInstanceVarkonSound),          false},
                                                  { 0, 0 }
                                                 };


