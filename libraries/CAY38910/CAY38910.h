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
#ifndef CAY38910_h
#define CAY38910_h

#include "ICpu.h"


//
// Implementation for testing the AY-3-8910 series of programmable sound
// generator and IO peripheral ICs. The device is accessed using two
// IO locations, one to set the register address to access and the other
// to read or write data to it.
//
class CAY38910
{
    public:

        //
        // The AY-3-8910 has the sound channels.
        //
        typedef enum {
            CHA,
            CHB,
            CHC
        } Channel;

        //
        // The AY-3-8910 has two IO ports.
        //
        typedef enum {
            IOA,
            IOB
        } Port;

        CAY38910(
            ICpu   *cpu,
            UINT32 regAddress,
            UINT32 regData
        );

        ~CAY38910(
        );

        PERROR idle(
        );

        PERROR check(
        );

        PERROR noise(
            Channel channel
        );

    private:

        PERROR read(
            UINT8 reg,
            UINT8 *data
        );

        PERROR write(
            UINT8 reg,
            UINT8 data
        );

    private:

        ICpu    *m_cpu;
        UINT32  m_regAddress;
        UINT32  m_regData;
};

#endif

