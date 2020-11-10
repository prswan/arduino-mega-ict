//
// Copyright (c) 2020, Paul R. Swan
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
#ifndef CGyrussSoundBaseGame_h
#define CGyrussSoundBaseGame_h

#include "CGame.h"
#include "CAY38910.h"


class CGyrussSoundBaseGame : public CGame
{
    public:

        //
        // CGyrussSoundBaseGame
        //

        virtual PERROR interruptCheck(
        );

        static PERROR ayIdle(
            void *cGyrussSoundBaseGame
        );

        static PERROR ayCheck(
            void *cGyrussSoundBaseGame
        );

        static PERROR ayNoise(
            void *cGyrussSoundBaseGame
        );

    protected:

        CGyrussSoundBaseGame(
            const ROM_DATA2N *romData2n,
            const ROM_REGION *romRegion
        );

        ~CGyrussSoundBaseGame(
        );

    private:

        PERROR ayNoiseFilter(
            CAY38910 *ay,
            CAY38910::Channel channel,
            UINT32 filterAd1,
            UINT32 filterAd2
        );

    private:

        // 0 - 11D - Port B filter o/p
        // 1 - 12D - Port B filter o/p
        // 2 - 10B - Port A clock i/p
        // 3 -  9B
        // 4  - 8B

        CAY38910 *m_ay[5];

};

#endif

