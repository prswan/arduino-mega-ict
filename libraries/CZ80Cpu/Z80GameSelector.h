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
#include <CAsoGame.h>
#include <CBuckRogersGame.h>
#include <CCrazyKongGame.h>
#include <CDambustersGame.h>
#include <CFitterGame.h>
#include <CGalaxianGame.h>
#include <CGyrussSoundGame.h>
#include <CHustlerGame.h>
#include <CHustlerSoundGame.h>
#include <CJackrabbitGame.h>
#include <CLadybugGame.h>
#include <CMegaZoneSoundGame.h>
#include <CPuckmanGame.h>
#include <CRallyXGame.h>
#include <CScrambleGame.h>
#include <CScrambleSoundGame.h>
#include <CSuperCobraGame.h>
#include <CSuperCobraSoundGame.h>
#include <CZaxxonGame.h>

//
// The initial selector to select the game to test.
//
static const SELECTOR s_gameSelector[] PROGMEM = {//0123456789abcde
                                                  {"Arian Mis. A(1)",  onSelectGame,    (void*) (CAsoAGame::createInstanceArianSet1),                false},
                                                  {"Arian Mis. B(1)",  onSelectGame,    (void*) (CAsoBGame::createInstanceArianSet1),                false},
                                                  {"ASO        A(1)",  onSelectGame,    (void*) (CAsoAGame::createInstanceAsoSet1),                  false},
                                                  {"Buck Rog. A(e1)",  onSelectGame,    (void*) (CBuckRogersGame::createInstanceEncSet1),            false},
                                                  {"Buck Rog. A(u1)",  onSelectGame,    (void*) (CBuckRogersGame::createInstanceSet1),               false},
                                                  {"Buck Rog. A(u2)",  onSelectGame,    (void*) (CBuckRogersGame::createInstanceSet2),               false},
                                                  {"Cosmic Avenger ",  onSelectGame,    (void*) (CLadybugGame::createInstanceSet2),                  false},
                                                  {"Crazy K. P2( 1)",  onSelectGame,    (void*) (CCrazyKongGame::createInstancePt2Set1),             false},
                                                  {"Crazy K. P2(Z1)",  onSelectGame,    (void*) (CCrazyKongGame::createInstancePt2ZacSet1),          false},
                                                  {"Dambusters(UK1)",  onSelectGame,    (void*) (CDambustersGame::createInstanceUKSet1),             false},
                                                  {"Fitter    (KD1)",  onSelectGame,    (void*) (CFitterGame::createInstanceKagaDenshiSet1),         false},
                                                  {"Fitter     (T1)",  onSelectGame,    (void*) (CFitterGame::createInstanceTaitoSet1),              false},
                                                  {"Galaxian   (M1)",  onSelectGame,    (void*) (CGalaxianGame::createInstanceSet1),                 false},
                                                  {"Galaxian   (M2)",  onSelectGame,    (void*) (CGalaxianGame::createInstanceSet2),                 false},
                                                  {"Gyruss    S(S1)",  onSelectGame,    (void*) (CGyrussSoundGame::createInstanceSet1),              false},
                                                  {"Hustler   (ZS2)",  onSelectGame,    (void*) (CHustlerGame::createInstanceZacScramConvSet2),      false},
                                                  {"Hustler  S(ZS2)",  onSelectGame,    (void*) (CHustlerSoundGame::createInstanceZacScramConvSet2), false},
                                                  {"Jackrabbit  (1)",  onSelectGame,    (void*) (CJackrabbitGame::createInstanceSet1),               false},
                                                  {"Jackrabbit  (2)",  onSelectGame,    (void*) (CJackrabbitGame::createInstanceSet2),               false},
                                                  {"Ladybug        ",  onSelectGame,    (void*) (CLadybugGame::createInstanceSet1),                  false},
                                                  {"Mega Zone S(S1)",  onSelectGame,    (void*) (CMegaZoneSoundGame::createInstanceSet1),            false},
                                                  {"Puckman    (S1)",  onSelectGame,    (void*) (CPuckmanGame::createInstanceSet1),                  false},
                                                  {"Puckman    (B1)",  onSelectGame,    (void*) (CPuckmanGame::createInstanceBSet1),                 false},
                                                  {"Rally-X(Midway)",  onSelectGame,    (void*) (CRallyXGame::createInstanceSet2),                   false},
                                                  {"Rally-X (Namco)",  onSelectGame,    (void*) (CRallyXGame::createInstanceSet1),                   false},
                                                  {"Scramble   (S1)",  onSelectGame,    (void*) (CScrambleGame::createInstanceSet1),                 false},
                                                  {"Scramble  S(S1)",  onSelectGame,    (void*) (CScrambleSoundGame::createInstanceSet1),            false},
                                                  {"Scramble   (Z1)",  onSelectGame,    (void*) (CScrambleGame::createInstanceZacSet1),              false},
                                                  {"Scramble  S(Z1)",  onSelectGame,    (void*) (CScrambleSoundGame::createInstanceZacSet1),         false},
                                                  {"S. Cobra   (K1)",  onSelectGame,    (void*) (CSuperCobraGame::createInstanceKonamiSet1),         false},
                                                  {"S. Cobra   (S1)",  onSelectGame,    (void*) (CSuperCobraGame::createInstanceSternSet1),          false},
                                                  {"S. Cobra  S(S1)",  onSelectGame,    (void*) (CSuperCobraSoundGame::createInstanceSternSet1),     false},
                                                  {"Zaxxon      (1)",  onSelectGame,    (void*) (CZaxxonGame::createInstanceSet1),                   false},
                                                  {"Zaxxon      (2)",  onSelectGame,    (void*) (CZaxxonGame::createInstanceSet2),                   false},
                                                  {"Zaxxon      (3)",  onSelectGame,    (void*) (CZaxxonGame::createInstanceSet3),                   false},
                                                  { 0, 0 }
                                                 };


