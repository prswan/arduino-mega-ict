//
// Copyright (c) 2017, Paul R. Swan
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
#ifndef CZ80ACpu_h
#define CZ80ACpu_h

#include "Arduino.h"
#include "ICpu.h"
#include "CBus.h"
#include "CFast8BitBus.h"
#include "CFastPin.h"

//
// This a more optimized version of the Z80 CPU implementation that works better
// with 4MHz Z80A based games, especially those that utlize synchronous bus cycles.
//

class CZ80ACpu : public ICpu
{
    public:

        typedef enum {
            CYCLE_TYPE_DEFAULT,
            CYCLE_TYPE_PUCKMAN,
            CYCLE_TYPE_LADYBUG
        } CycleType;

        //
        // Constructor
        //
        // vramAddress
        //  If a none-zero VRAM address is supplied then WAIT toggling should be
        //  active for that access. This is verified in the "check" function
        //  because there is no timeout in the bus cycles and thus they hang if the
        //  VRAM WAIT hold-off isn't working.
        //
        // addressRemapCallback
        //  If this is supplied then the callback is made as the first action
        //  for all IO operations to allow the caller to remap the address.
        //  e.g. to support basic copy protection schemes that swap address bits.
        //
        // dataRemapCallback
        //  If this is supplied then the callback is made:
        //    reads  - after any address remap and after any data read to allow the
        //             read data to be post-remapped.
        //    writes - after any address remap before any data is written to allow
        //             the write data to be pre-remapped.
        //
        // cycleType
        //  Sets the specific Z80 cycle implementation to use.
        //

        CZ80ACpu(
            UINT32                vramAddress                 = 0,
            AddressRemapCallback  addressRemapCallback        = NO_ADDRESS_REMAP,
            void                 *addressRemapCallbackContext = NULL,
            DataRemapCallback     dataRemapCallback           = NO_DATA_REMAP,
            void                 *dataRemapCallbackContext    = NULL,
            CycleType             cycleType                   = CYCLE_TYPE_DEFAULT
        );

        // ICpu Interface
        //

        virtual PERROR idle(
        );

        virtual PERROR check(
        );

        virtual
        UINT8
        dataBusWidth(
            UINT32 address
        );

        virtual
        UINT8
        dataAccessWidth(
            UINT32 address
        );

        // Z80A Address Space:
        // 0x000000 -> 0x00FFFF - Memory Mapped Data
        // 0x010000 -> 0x01FFFF - Input/Output Ports
        // 0x1xxxxx             - WAIT pre-synchronized space

        virtual PERROR memoryRead(
            UINT32 address,
            UINT16 *data
        );

        virtual PERROR memoryWrite(
            UINT32 address,
            UINT16 data
        );

        virtual
        PERROR
        waitForInterrupt(
            Interrupt interrupt,
            bool      active,
            UINT32    timeoutInMs
        );

        virtual
        PERROR
        acknowledgeInterrupt(
            UINT16 *response
        );

        //
        // CZ80ACpu Interface
        //

    private:

        //
        // The original implementation of the Z80 bus
        // cycle that's used for most games.
        //
        PERROR
        MREQread(
            UINT16 *data
        );

        PERROR
        MREQwrite(
            UINT16 *data
        );

        //
        // An implementation that has a longer wait prior
        // to the initial detection of WAIT and immediate
        // data read after WAIT is cleared.
        // This handles the Puckman Sync Bus Controller
        //
        PERROR
        MREQreadPuckman(
            UINT16 *data
        );

        PERROR
        MREQwritePuckman(
            UINT16 *data
        );

        //
        // An implementation that has a longer wait prior
        // to the initial detection of WAIT.
        // This handles reading the Ladybug video and color ram
        //
        PERROR
        MREQreadLadybug(
            UINT16 *data
        );

        PERROR
        IORQread(
            UINT16 *data
        );

        PERROR
        IORQwrite(
            UINT16 *data
        );

    private:

        CBus          m_busA;
        CFast8BitBus  m_busD;

        CFastPin      m_pin_RD;
        CFastPin      m_pin_WR;
        CFastPin      m_pin_WAIT;

        CFastPin      m_pin_M1;
        CFastPin      m_pin_IORQ;
        CFastPin      m_pin_MREQ;

        UINT32        m_vramAddress;

        AddressRemapCallback  m_addressRemapCallback;
        void                 *m_addressRemapCallbackContext;
        DataRemapCallback     m_dataRemapCallback;
        void                 *m_dataRemapCallbackContext;
        CycleType             m_cycleType;

};

#endif
