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
#ifndef CGameCallback_h
#define CGameCallback_h

#include "IGame.h"

//
// This wraps an IGame interface to allow it to be called via the function
// selector UI. This is a static class.
//
// The "context" for all the functions is an IGame.
// The functions may receive: SELECT, UP & DOWN.
// The functions may use the 2nd row of the LCD.
//
class CGameCallback
{
    public:

        //
        // This is the selector object array for the UI for games use..
        //
        static const SELECTOR *selectorGame;

        //
        // This is the selector object array for the UI for generic use.
        //
        static const SELECTOR *selectorGeneric;

        //
        // This is the selector object array for the UI for soak test use.
        //
        static const SELECTOR *selectorSoakTest;

        //
        // The game object to use.
        //
        static IGame *game;

        //
        // IGame function wrappers.
        //

        static PERROR onSelectBusIdle(
            void *iGame,
            int  key
        );

        static PERROR onSelectBusCheck(
            void *iGame,
            int  key
        );

        static PERROR onSelectRomCheckAll(
            void *iGame,
            int  key
        );

        static PERROR onSelectRamCheckAll(
            void *iGame,
            int  key
        );

        static PERROR onSelectRamCheckAllRA(
            void *iGame,
            int  key
        );

        static PERROR onSelectRamCheckAllCS(
            void *iGame,
            int  key
        );

        static PERROR onSelectInterruptCheck(
            void *iGame,
            int  key
        );

        static PERROR onSelectRomCheck(
            void *iGame,
            int  key
        );

        static PERROR onSelectRomCrc(
            void *iGame,
            int  key
        );

        static PERROR onSelectRomRead(
            void *iGame,
            int  key
        );

        static PERROR onSelectRamCheck(
            void *iGame,
            int  key
        );

        static PERROR onSelectRamCheckRA(
            void *iGame,
            int  key
        );

        static PERROR onSelectRamCheckAd(
            void *iGame,
            int  key
        );

        static PERROR onSelectRamWriteRead(
            void *iGame,
            int  key
        );

        static PERROR onSelectRomReadAll(
            void *iGame,
            int  key
        );

        static PERROR onSelectRamWriteAllAD(
            void *iGame,
            int  key
        );

        static PERROR onSelectRamWriteAllHi(
            void *iGame,
            int  key
        );

        static PERROR onSelectRamWriteAllLo(
            void *iGame,
            int  key
        );

        static PERROR onSelectRamReadAll(
            void *iGame,
            int  key
        );

        static PERROR onSelectInputRead(
            void *iGame,
            int  key
        );

        static PERROR onSelectOutputWrite(
            void *iGame,
            int  key
        );

        static PERROR onSelectCustom(
            void *iGame,
            int  key
        );

};

#endif

