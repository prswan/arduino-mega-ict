//
// Copyright (c) 2019, Paul R. Swan
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
#ifndef C6809EPinOut_h
#define C6809EPinOut_h

#include "Arduino.h"
#include "Types.h"


class C6809EPinOut
{
    public:

        //
        // Pin prefixes
        //
        // _ - active low
        //
        // Pin suffixes
        //
        // i - input
        // o - output
        // t - tri-state
        //

        //
        // Control Pins
        //
        const CONNECTION m_GND_i;
        const CONNECTION m__NMI_i;
        const CONNECTION m__IRQ_i;
        const CONNECTION m__FIRQ_i;
        const CONNECTION m_BS_o;
        const CONNECTION m_BA_o;
        const CONNECTION m_VCC_i;
        const CONNECTION m_RW_o;
        const CONNECTION m_BUSY_o;
        const CONNECTION m_E_i;
        const CONNECTION m_Q_i;
        const CONNECTION m_AVMA_o;
        const CONNECTION m__RESET_i;
        const CONNECTION m_LIC_o;
        const CONNECTION m_TSC_i;
        const CONNECTION m__HALT_i;

        //
        // Bus pins
        //
        const CONNECTION m_A_ot[16]; // 16 bits

        const CONNECTION m_D_iot[8]; // 8 bits.

};

// Standard 6809E pinout
extern const C6809EPinOut s_6809EPinOut;

// Konami-1 custom CPU pinout
extern const C6809EPinOut s_Konami1PinOut;

#endif
