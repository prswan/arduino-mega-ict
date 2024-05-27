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
#ifndef CStarWarsAvgBaseGame_h
#define CStarWarsAvgBaseGame_h

#include "CGame.h"
#include "CCapture.h"

class CStarWarsAvgBaseGame : public CGame
{
    public:

        //
        // CStarWarsAvgBaseGame
        //

        static PERROR vgRst(
            void *context
        );

        static PERROR loadHALT(
            void *context
        );

        static PERROR loadVCTR(
            void *context
        );

        static PERROR loadSTAT(
            void *context
        );

        static PERROR loadSCAL(
            void *context
        );

        static PERROR loadCNTR(
            void *context
        );

        static PERROR loadAll(
            void *context
        );

        static PERROR vgGo(
            void *context
        );

        static PERROR capture32(
            void *context
        );

        static PERROR rstGoCap32(
            void *context
        );

        static PERROR runToHalt(
            void *context
        );

    protected:

        CStarWarsAvgBaseGame(
            const ROM_REGION *romRegion
        );

        ~CStarWarsAvgBaseGame(
        );

        static PERROR load(
            void   *context,
            UINT8   dataLen,
            UINT16 *data
        );

    private:

        CCapture *m_capture;

};

#endif

