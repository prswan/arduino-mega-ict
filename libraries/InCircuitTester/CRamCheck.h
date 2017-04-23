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
#ifndef CRamCheck_h
#define CRamCheck_h

#include "Arduino.h"
#include "Types.h"
#include "ICpu.h"


class CRamCheck
{
    public:

        CRamCheck(
            ICpu *cpu,
            const RAM_REGION ramRegion[],
            const RAM_REGION ramRegionByteOnly[],
            const RAM_REGION ramRegionWriteOnly[],
            void *bankSwitchContext
        );

        PERROR
        check(
        );

        PERROR
        checkChipSelect(
        );

        PERROR
        checkRandomAccess(
        );

        PERROR
        write(
        );

        PERROR
        write(
            UINT8 value
        );

        PERROR
        read(
        );

        PERROR
        checkRandom(
            const RAM_REGION *ramRegion,
            int   seed
        );

        PERROR
        writeReadData(
            const RAM_REGION *ramRegion
        );

        PERROR
        write(
            const RAM_REGION *ramRegion
        );

        PERROR
        write(
            const RAM_REGION *ramRegion,
            UINT8 value
        );

        PERROR
        read(
            const RAM_REGION *ramRegion
        );

        PERROR
        check(
            const RAM_REGION *ramRegion
        );

        PERROR
        checkRandomAccess(
            const RAM_REGION *ramRegion
        );

        PERROR
        checkAddress(
            const RAM_REGION *ramRegion
        );

        PERROR
        writeRandom(
            const RAM_REGION *ramRegion,
            int   seed,
            bool  invert
        );

        PERROR
        readVerifyRandom(
            const RAM_REGION *ramRegion,
            int   seed,
            bool  invert
        );

    private:

        ICpu             *m_cpu;
        const RAM_REGION *m_ramRegion;
        const RAM_REGION *m_ramRegionByteOnly;
        const RAM_REGION *m_ramRegionWriteOnly;
        void             *m_bankSwitchContext;
};

#endif
