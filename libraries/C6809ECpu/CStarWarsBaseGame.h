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
#ifndef CStarWarsBaseGame_h
#define CStarWarsBaseGame_h

#include "CGame.h"


class CStarWarsBaseGame : public CGame
{
    public:

        //
        // CStarWarsBaseGame
        //

        static PERROR onBankSwitchMPAGE0(
            void *cStarWarsBaseGame
        );

        static PERROR onBankSwitchMPAGE1(
            void *cStarWarsBaseGame
        );

        static PERROR test10(
            void   *context
        );

        static PERROR test11(
            void   *context
        );

        static PERROR test12(
            void   *context
        );

        static PERROR test13(
            void   *context
        );

        static PERROR test14(
            void   *context
        );

        static PERROR test15(
            void   *context
        );

        static PERROR test16(
            void   *context
        );

        static PERROR test17(
            void   *context
        );

        static PERROR test18(
            void   *context
        );

        static PERROR test19(
            void   *context
        );

        static PERROR test20(
            void   *context
        );

        static PERROR test21(
            void   *context
        );

        static PERROR test22(
            void   *context
        );

        static PERROR test23(
            void   *context
        );

        static PERROR test24(
            void   *context
        );

        static PERROR test25(
            void   *context
        );

        static PERROR testRepeatLastMatrixProgram(
            void   *context
        );

        static PERROR testClockPulse(
            void   *context
        );

        static PERROR testCapture(
            void   *context
        );

    protected:

        CStarWarsBaseGame(
            const ROM_REGION *romRegion
        );

        ~CStarWarsBaseGame(
        );

    private:

        PERROR waitForMathRunLo(
        );

        static PERROR testMatrix(
            void   *context,
            int    srcDataLength,
            UINT32 *srcDataAddress,
            UINT16 *srcData,
            int    expDataLength,
            UINT32 *expDataAddress,
            UINT16 *expData,
            UINT16 programAddress
        );

        static PERROR testDivider(
            void   *context,
            UINT16 dividend,
            UINT16 divisor,
            UINT16 quotient
        );

        UINT32 m_clockPulseCount;
        UINT16 m_lastMatrixProgramAddress;

};

#endif

