//
// Copyright (c) 2018, Paul R. Swan
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
#ifndef CFroggerSoundBaseGame_h
#define CFroggerSoundBaseGame_h

#include "CGame.h"
#include "CAY38910.h"


class CFroggerSoundBaseGame : public CGame
{
    public:

        //
        // CFroggerSoundBaseGame
        //

        virtual PERROR interruptCheck(
        );

        static PERROR ayIdle(
            void *cFroggerSoundBaseGame
        );

        static PERROR ayCheck(
            void *cFroggerSoundBaseGame
        );

        static PERROR ay_35_ChA(
            void *cFroggerSoundBaseGame
        );

        static PERROR ay_35_ChB(
            void *cFroggerSoundBaseGame
        );

        static PERROR ay_35_ChC(
            void *cFroggerSoundBaseGame
        );

    protected:

        CFroggerSoundBaseGame(
            const ROM_DATA2N *romData2n,
            const ROM_REGION *romRegion
        );

        ~CFroggerSoundBaseGame(
        );

    private:

        PERROR ayNoiseFilter(
            CAY38910 *ay,
            CAY38910::Channel channel,
            UINT32 filterAd1,
            UINT32 filterAd2
        );

    private:

        CAY38910  *m_ay;

};

#endif

