//
// Copyright (c) 2023, Paul R. Swan
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
#ifndef CHyperSportsSoundBaseGame_h
#define CHyperSportsSoundBaseGame_h

#include "CGame.h"


class CHyperSportsSoundBaseGame : public CGame
{
    public:

        //
        // CHyperSportsSoundBaseGame
        //

        virtual PERROR interruptCheck(
        );

        //
        // Custom function to test the audio output DAC with a simple sawtooth pattern
        //
        static PERROR testDAC(
            void *context
        );

        //
        // Custom function to test the SN76489 tone generator
        //
        static PERROR testSN76489(
            void *context
        );

    protected:

        CHyperSportsSoundBaseGame(
            const ROM_DATA2N *romData2n,
            const ROM_REGION *romRegion
        );

        ~CHyperSportsSoundBaseGame(
        );

    private:

        PERROR clearSN76489(
            ICpu *cpu
        );

};

#endif

