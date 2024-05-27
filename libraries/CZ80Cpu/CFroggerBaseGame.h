//
// Copyright (c) 2021, Paul R. Swan
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
#ifndef CFroggerBaseGame_h
#define CFroggerBaseGame_h

#include "CGame.h"


class CFroggerBaseGame : public CGame
{
    public:

        //
        // CFroggerBaseGame
        //

        virtual PERROR interruptCheck(
        );

        static PERROR onBankSwitchSetup8255_0(
            void *cFroggerBaseGame
        );

        static PERROR onBankSwitchSetup8255_1(
            void *cFroggerBaseGame
        );

        //
        // Custom function to clear the video display to blank.
        //
        static PERROR clearBk(
            void *context
        );

        static PERROR clearObj(
            void *context
        );

    protected:

        CFroggerBaseGame(
            const ROM_DATA2N *romData2n,
            const ROM_REGION *romRegion
        );

        ~CFroggerBaseGame(
        );

    private:

        //
        // These are used internally and vary between platforms.
        //
        UINT32 m_intMaskWriteAddress;
        UINT32 m_8255WriteBaseAddress0;
        UINT32 m_8255WriteBaseAddress1;

};

#endif

