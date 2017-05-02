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
#include <CZ80GenericGame.h>
#include <CAsoGame.h>
#include <CGalaxianGame.h>
#include <CJackrabbitGame.h>
#include <CScrambleGame.h>

//
// The initial selector to select the game to test.
//
static const SELECTOR s_gameSelector[] PROGMEM = {//0123456789abcde
                                                  {"Generic 2716   ",  onSelectGeneric, (void*) (CZ80GenericGame::createInstance2716),  false},
                                                  {"Generic 2732   ",  onSelectGeneric, (void*) (CZ80GenericGame::createInstance2732),  false},
                                                  {"ASO        A(1)",  onSelectGame,    (void*) (CAsoAGame::createInstanceAsoSet1),     false},
                                                  {"Arian Mis. A(1)",  onSelectGame,    (void*) (CAsoAGame::createInstanceArianSet1),   false},
                                                  {"Arian Mis. B(1)",  onSelectGame,    (void*) (CAsoBGame::createInstanceArianSet1),   false},
                                                  {"Galaxian   (M1)",  onSelectGame,    (void*) (CGalaxianGame::createInstanceSet1),    false},
                                                  {"Galaxian   (M2)",  onSelectGame,    (void*) (CGalaxianGame::createInstanceSet2),    false},
                                                  {"Jackrabbit  (1)",  onSelectGame,    (void*) (CJackrabbitGame::createInstanceSet1),  false},
                                                  {"Jackrabbit  (2)",  onSelectGame,    (void*) (CJackrabbitGame::createInstanceSet2),  false},
                                                  {"Scramble   (S1)",  onSelectGame,    (void*) (CScrambleGame::createInstanceSet1),    false},
                                                  { 0, 0 }
                                                 };


