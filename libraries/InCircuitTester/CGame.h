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
#ifndef CGame_h
#define CGame_h

#include "IGame.h"
#include "ICpu.h"

class CGame : public IGame
{
    public:

        //
        // IGame Interface
        //

        virtual PERROR busIdle(
        );

        virtual PERROR busCheck(
        );

        virtual PERROR romCheckAll(
        );

        virtual PERROR ramCheckAll(
        );

        virtual PERROR ramCheckAllChipSelect(
        );

        virtual PERROR ramCheckAllRandomAccess(
        );

        virtual PERROR interruptCheck(
        );

        virtual PERROR romCheck(
            int key
        );

        virtual PERROR romCrc(
            int key
        );

        virtual PERROR romRead(
            int key
        );

        virtual PERROR ramCheck(
            int key
        );

        virtual PERROR ramCheckRandomAccess(
            int key
        );

        virtual PERROR ramCheckAddress(
            int key
        );

        virtual PERROR ramWriteRead(
            int key
        );

        virtual PERROR romReadAll(
            int key
        );

        virtual PERROR ramWriteAllAD(
            int key
        );

        virtual PERROR ramWriteAllLo(
            int key
        );

        virtual PERROR ramWriteAllHi(
            int key
        );

        virtual PERROR ramReadAll(
            int key
        );

        virtual PERROR inputRead(
            int key
        );

        virtual PERROR outputWrite(
            int key
        );

        virtual PERROR custom(
            int key
        );

        virtual PERROR onRomKeyMove(
            int key
        );

        virtual PERROR onRamKeyMove(
            int key
        );

        virtual PERROR onRamByteKeyMove(
            int key
        );

        virtual PERROR onCustomKeyMove(
            int key
        );

    protected:

        //
        // NOTE: These are all assumed to be defined in PROGMEM since
        // they are large data structures. The constructor will copy them
        // over to SRAM.
        //

        CGame(
            const ROM_REGION      *romRegion,
            const RAM_REGION      *ramRegion,
            const RAM_REGION      *ramRegionByteOnly,
            const RAM_REGION      *ramRegionWriteOnly,
            const INPUT_REGION    *inputRegion,
            const OUTPUT_REGION   *outputRegion,
            const CUSTOM_FUNCTION *customFunction
        );

        ~CGame();

        //
        // These are filled in by the derived concrete game and are SRAM copies
        // of the supplied PROGMEM source data.
        //

        ROM_REGION      *m_romRegion;
        RAM_REGION      *m_ramRegion;
        RAM_REGION      *m_ramRegionByteOnly;
        RAM_REGION      *m_ramRegionWriteOnly;
        INPUT_REGION    *m_inputRegion;
        OUTPUT_REGION   *m_outputRegion;
        CUSTOM_FUNCTION *m_customFunction;

        //
        // The applicable processor for this game.
        //

        ICpu *m_cpu;

        //
        // The interrupt pin to use for the video interrupt test.
        //
        ICpu::Interrupt m_interrupt;

        //
        // When true, auto-vector mode is assumed and no acknowledge is tested.
        //
        bool            m_interruptAutoVector;

        //
        // The expected response to an interrupt vector cycle for none auto-vector mode.
        //
        UINT8           m_interruptResponse;

        //
        // These are the region selectors for simple read/write tests.
        //
        int  m_RomReadRegion;
        int  m_RamWriteReadRegion;
        int  m_RamWriteReadByteRegion;
        int  m_inputReadRegion;
        int  m_outputWriteRegion;
        bool m_outputWriteRegionOn;

        //
        // The current selector for the custom functions, if any.
        //
        int  m_customSelect;

};

#endif

