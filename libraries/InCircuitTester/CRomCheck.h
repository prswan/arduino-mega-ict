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
#ifndef CRomCheck_h
#define CRomCheck_h

#include "Arduino.h"
#include "Types.h"
#include "ICpu.h"


class CRomCheck
{
    public:

        CRomCheck(
            ICpu *cpu,
            const ROM_REGION romRegion[],
            void *bankSwitchContext
        );

        //
        // Whole set functions.
        //

        PERROR
        check(
        );

        PERROR
        read(
        );

        //
        // Region specific functions.
        //

        PERROR
        checkData2n(
            const ROM_REGION *romRegion
        );

        PERROR
        calculateCrc(
            const ROM_REGION *romRegion,
            UINT32 *crc
        );

        PERROR
        readData(
            const ROM_REGION *romRegion
        );

        PERROR
        checkCrc(
            const ROM_REGION *romRegion
        );

        PERROR
        read(
            const ROM_REGION *romRegion
        );

        PERROR
        check(
            const ROM_REGION *romRegion
        );

   private:

        ICpu             *m_cpu;
        const ROM_REGION *m_romRegion;
        void             *m_bankSwitchContext;

};

#endif
