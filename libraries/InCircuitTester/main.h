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
#ifndef main_h
#define main_h

#include "Arduino.h"
#include <Types.h>

//
// When set (none-zero) causes the select to repeat the selection callback
// for the set number of seconds.
//
extern int s_repeatSelectTimeInS;

//
// When true causes the repeat to ignore any reported error and continue the repeat
//
extern bool s_repeatIgnoreError;


PERROR
onSelectConfig(
    void *context,
    int  key
);


//
// Handler for the game select callback that will switch the current
// selector to the one supplied.
//
PERROR
onSelectGame(
    void *context,
    int  key
);

//
// Handler for the generic select callback that will switch the current
// selector to the one supplied.
//
PERROR
onSelectGeneric(
    void *context,
    int  key
);

//
// Handler for the soak test select callback that will run the soak test
// for the current game forever (if no error occurs).
//
PERROR
onSelectSoakTest(
    void *context,
    int  key
);

//
// This is the main entry point for the Arduino script files into the normal C++ domain.
//

void mainSetup(
    const SELECTOR *gameSelector
);

void mainLoop();

#endif
