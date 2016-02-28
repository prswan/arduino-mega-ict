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
#ifndef CWilliamsBaseGame_h
#define CWilliamsBaseGame_h

#include "CGame.h"


class CWilliamsBaseGame : public CGame
{
    public:

        //
        // Constructors for CWilliamsBaseGame
        //

        //
        // This is used to select ROM insead of RAM from 0x0000 to 0x98ff
        //
        static PERROR onROMBankSelect(
            void *cWilliamsBaseGame
        );

        //
        // This is used to select RAM insead of ROM from 0x0000 to 0x98ff
        //
        static PERROR onRAMBankSelect(
            void *cWilliamsBaseGame
        );

        //
        // This is used to setup the hardware configuration of the ROM board 6821 PIA U1 Bank A
        //
        static PERROR setupPIA1A(
            void *cWilliamsBaseGame
        );

        //
        // This is used to setup the hardware configuration of the ROM board 6821 PIA U1 Bank B
        //
        static PERROR setupPIA1B(
            void *cWilliamsBaseGame
        );

        //
        // This is used to setup the hardware configuration of the interface board 6821 PIA U1 Bank A
        //
        static PERROR setupPIA2A(
            void *cWilliamsBaseGame
        );

        //
        // This is used to setup the hardware configuration of the interface board 6821 PIA U1 Bank B
        //
        static PERROR setupPIA2B(
            void *cWilliamsBaseGame
        );

        //
        // This is used for testing the interrupts
        //
        virtual PERROR interruptCheck(
        );

        //
        // Custom function to cycle through all 64 output combinations using Sound0 to Sound5 (PB0-PB5) on PIA1B one at a time
        //
        static PERROR soundTestA(
            void *cWilliamsBaseGame
        );

        //
        // Custom function to cycle through sound control lines Sound0 to Sound5 (PB0-PB5) on PIA1B one at a time
        //
        static PERROR soundTestB(
            void *cWilliamsBaseGame
        );

        //
        // Custom function to re-initialise PIA1 and PIA2
        //
        static PERROR resetPIAs(
            void *cWilliamsBaseGame
        );

    protected:

        CWilliamsBaseGame(
            const ROM_REGION *romRegion,
            const RAM_REGION *ramRegion
        );

        ~CWilliamsBaseGame(
        );

    private:

        boolean PIA1Ainitialised;
        boolean PIA1Binitialised;        
        boolean PIA2Ainitialised;
        boolean PIA2Binitialised;   
};

#endif
