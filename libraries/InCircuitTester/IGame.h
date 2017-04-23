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
#ifndef IGame_h
#define IGame_h

#include "Arduino.h"
#include "Error.h"

class IGame
{
    public:

        //
        // Set the CPU pins into default idle/inactive state.
        //
        virtual PERROR busIdle(
        ) = 0;

        //
        // Perform a basic pin check on the CPU pins, for example:
        // Vcc is high, GND is low, the address and data bus pullup high.
        //
        virtual PERROR busCheck(
        ) = 0;

        //
        // Performs a check of all the ROM.
        //
        virtual PERROR romCheckAll(
        ) = 0;

        //
        // Performs a check of all the accessible RAM
        // Based on write-one & read-one pattern.
        //
        virtual PERROR ramCheckAll(
        ) = 0;

        // Performs a check of all the accessible RAM
        // Based on the read/write random access pattern
        //
        virtual PERROR ramCheckAllRandomAccess(
        ) = 0;

        // Performs a check of all the accessible RAM
        // Based on the write-all & read-all pattern.
        //
        virtual PERROR ramCheckAllChipSelect(
        ) = 0;

        //
        // Verify that the CPU interrupt is running (usually VBlank)
        //
        virtual PERROR interruptCheck(
        ) = 0;

        //
        // Performs a check of the specified rom
        //
        virtual PERROR romCheck(
            int key
        ) = 0;

        //
        // Calculates the CRC of the specified rom
        //
        virtual PERROR romCrc(
            int key
        ) = 0;

        //
        // Basic test function to read the ROM space.
        //
        virtual PERROR romRead(
            int key
        ) = 0;

        //
        // Performs a check of the specified ram
        //
        virtual PERROR ramCheck(
            int key
        ) = 0;

        //
        // Performs a check of the specified ram
        //
        virtual PERROR ramCheckRandomAccess(
            int key
        ) = 0;

        //
        // Performs a stuck address check of the specified ram
        //
        virtual PERROR ramCheckAddress(
            int key
        ) = 0;

        //
        // Basic test function to write & read the RAM space.
        //
        virtual PERROR ramWriteRead(
            int key
        ) = 0;

        //
        // Basic test function to read the ROM space.
        //
        virtual PERROR romReadAll(
            int key
        ) = 0;

        //
        // Basic test function to write the RAM space.
        // Data = Address
        //
        virtual PERROR ramWriteAllAD(
            int key
        ) = 0;

        //
        // Basic test function to write the RAM space.
        // Data = 0x00
        //
        virtual PERROR ramWriteAllLo(
            int key
        ) = 0;

        //
        // Basic test function to write the RAM space.
        // Data = 0xFF
        //
        virtual PERROR ramWriteAllHi(
            int key
        ) = 0;

        //
        // Basic test function to read the RAM space.
        //
        virtual PERROR ramReadAll(
            int key
        ) = 0;

        //
        // Performs a read of the selected input
        //
        virtual PERROR inputRead(
            int key
        ) = 0;

        //
        // Performs a write of the selected output
        //
        virtual PERROR outputWrite(
            int key
        ) = 0;

        //
        // Invokes game specific custom functions.
        //
        virtual PERROR custom(
            int key
        ) = 0;

};

#endif
