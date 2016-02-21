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
#ifndef CDefenderBaseGame_h
#define CDefenderBaseGame_h

#include "CGame.h"


class CDefenderBaseGame : public CGame
{
    public:

        //
        // Constructors for CDefenderBaseGame
        //

        //
        // This is used to select Bank 0 (I/O) of the banked ROMs 0xc000 to 0xcfff
        //
        static PERROR onROMBankSelect0(
            void *cDefenderBaseGame
        );

        //
        // This is used to select Bank 1 of the banked ROMs 0xc000 to 0xcfff
        //
        static PERROR onROMBankSelect1(
            void *cDefenderBaseGame
        );

        //
        // This is used to select Bank 2 of the banked ROMs 0xc000 to 0xcfff
        //
        static PERROR onROMBankSelect2(
            void *cDefenderBaseGame
        );

        //
        // This is used to select Bank 3 of the banked ROMs 0xc000 to 0xcfff
        //
        static PERROR onROMBankSelect3(
            void *cDefenderBaseGame
        );

        //
        // This is used to select Bank 7 of the banked ROMs 0xc000 to 0xcfff
        //
        static PERROR onROMBankSelect7(
            void *cDefenderBaseGame
        );

        //
        // This is used to setup the hardware configuration of the ROM board 6821 PIA U1 Bank A
        //
        static PERROR setupPIA1A(
            void *cDefenderBaseGame
        );

        //
        // This is used to setup the hardware configuration of the ROM board 6821 PIA U1 Bank B
        //
        static PERROR setupPIA1B(
            void *cDefenderBaseGame
        );

        //
        // This is used to setup the hardware configuration of the interface board 6821 PIA U1 Bank A
        //
        static PERROR setupPIA2A(
            void *cDefenderBaseGame
        );

        //
        // This is used to setup the hardware configuration of the interface board 6821 PIA U1 Bank B
        //
        static PERROR setupPIA2B(
            void *cDefenderBaseGame
        );

        //
        // This is used for testing the interrupts
        //
        virtual PERROR interruptCheck(
        );

        //
        // Custom function to cycle through all 32 output combinations using Sound0 to Sound4 (PB0-PB4) on PIA1B one at a time
        // Sound5 (PB5) is not used on Defender
        //
        static PERROR soundTestA(
            void *cDefenderBaseGame
        );

        //
        // Custom function to cycle through sound control lines Sound0 to Sound4 (PB0-PB4) on PIA1B one at a time
        // Sound5 (PB5) is not used on Defender
        //
        static PERROR soundTestB(
            void *cDefenderBaseGame
        );

        //
        // Custom function to re-initialise PIA1 and PIA2
        //
        static PERROR resetPIAs(
            void *cDefenderBaseGame
        );

    protected:

        CDefenderBaseGame(
            const ROM_REGION *romRegion
        );

        ~CDefenderBaseGame(
        );

    private:

        boolean PIA1Ainitialised;
        boolean PIA1Binitialised;        
        boolean PIA2Ainitialised;
        boolean PIA2Binitialised;        
};

#endif
