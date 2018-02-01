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
#ifndef CWmsSoundBaseGame_h
#define CWmsSoundBaseGame_h

#include "CGame.h"


class CWmsSoundBaseGame : public CGame
{
    public:

        //
        // Constructors for CWmsSoundBaseGame
        //

        //
        // This is used to setup the hardware configuration of the 6821 PIA U10 Bank A for output
        //
        static PERROR onBankSwitchSetupPIA1A(
            void *cWmsSoundBaseGame
        );

        //
        // This is used to setup the hardware configuration of the 6821 PIA U10 Bank B for input
        //
        static PERROR onBankSwitchSetupPIA1B(
            void *cWmsSoundBaseGame
        );

        //
        // This is used for testing the the /IRQ on 6802 CPU U9 Pin 4
        //
        virtual PERROR interruptCheck(
        );

        //
        // Custom function for testing the 6821 PIA U10 Bank A outputs to the MC1408 DAC U13 and the analog sound hardware
        //
        static PERROR soundTestDAC(
            void *context
        );

        //
        // Custom function for testing the optional speech board 55516 CVSDM Chip U1
        //
        static PERROR soundTestCVSDM(
            void *context
        );

    protected:

        CWmsSoundBaseGame(
            const ROM_REGION *romRegion
        );

        ~CWmsSoundBaseGame(
        );

};

#endif

