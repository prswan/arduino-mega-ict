//
// Copyright (c) 2016, Paul R. Swan
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
#include "Arduino.h"
#include "Error.h"
#include "CT11Cpu.h"
#include "PinMap.h"


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

static const CONNECTION s_BGND_i    = {  8, "BGND"     };

static const CONNECTION s__BCLR_o   = { 18, "_BCLR"    };
static const CONNECTION s_PUP_i     = { 19, "PUP"      };
static const CONNECTION s_GND_i     = { 20, "GND"      };
static const CONNECTION s_COUT_o    = { 21, "COUT"     };
static const CONNECTION s_XTL1_i    = { 22, "XTL1"     };
static const CONNECTION s_XTL0_i    = { 23, "XTL0"     };
static const CONNECTION s_SEL1_o    = { 24, "SEL1"     };
static const CONNECTION s_SEL0_o    = { 25, "SEL0"     };
static const CONNECTION s_READY_i   = { 26, "READY"    };
static const CONNECTION s_R_WHB_o   = { 27, "R_WHB"    };
static const CONNECTION s_R_WLB_o   = { 28, "R_WLB"    };
static const CONNECTION s__RAS_o    = { 29, "_RAS"     };
static const CONNECTION s__CAS_o    = { 30, "_CAS"     };
static const CONNECTION s_PI_o      = { 31, "PI"       };
static const CONNECTION s_Vcc_i     = { 40, "Vcc"      };


//
// Bus pins
//
static const CONNECTION s_DALHi_iot[] = { { 9, "DAL8"  },
                                          { 7, "DAL9"  },
                                          { 6, "DAL10" },
                                          { 5, "DAL11" },
                                          { 4, "DAL12" },
                                          { 3, "DAL13" },
                                          { 2, "DAL14" },
                                          { 1, "DAL15" } }; // Upper 8 of 16 bits

static const CONNECTION s_DALLo_iot[] = { {17, "DAL0" },
                                          {16, "DAL1" },
                                          {15, "DAL2" },
                                          {14, "DAL3" },
                                          {13, "DAL4" },
                                          {12, "DAL5" },
                                          {11, "DAL6" },
                                          {10, "DAL7" } }; // Lower 8 of 16 bits

//
// The AI pins represented as an 8-bit bus
//
static const CONNECTION s_AI_io[] = { {32, "AI0_DMR" },
                                      {33, "AI1_CP3" },
                                      {34, "AI2_CP2" },
                                      {35, "AI3_CP1" },
                                      {36, "AI4_CP0" },
                                      {37, "AI5_VEC" },
                                      {38, "AI6_PF " },
                                      {39, "AI7_HLT" } };

//
// Address flag for 16-bit selection
//
static const UINT32 s_16BitAddress = 0x01000000;

//
// Address flag for video sync memory access
//
static const UINT32 s_vSyncAddress = 0x02000000;


CT11Cpu::CT11Cpu(
    AddressRemapCallback  addressRemapCallback,
    void                 *addressRemapCallbackContext
) : m_addressRemapCallback(addressRemapCallback),
    m_addressRemapCallbackContext(addressRemapCallbackContext),
    m_busDALHi(g_pinMap40DIL, s_DALHi_iot,  ARRAYSIZE(s_DALHi_iot)),
    m_busDALLo(g_pinMap40DIL, s_DALLo_iot,  ARRAYSIZE(s_DALLo_iot)),
    m_busAI(g_pinMap40DIL, s_AI_io,  ARRAYSIZE(s_AI_io)),
    m_pinPI(g_pinMap40DIL, &s_PI_o),
    m_pinSEL0(g_pinMap40DIL, &s_SEL0_o),
    m_pinSEL1(g_pinMap40DIL, &s_SEL1_o),
    m_pinR_WHB(g_pinMap40DIL, &s_R_WHB_o),
    m_pinR_WLB(g_pinMap40DIL, &s_R_WLB_o),
    m_pin_RAS(g_pinMap40DIL, &s__RAS_o),
    m_pin_CAS(g_pinMap40DIL, &s__CAS_o),
    m_pinCOUT(g_pinMap40DIL, &s_COUT_o),
    m_pinXTL1(g_pinMap40DIL, &s_XTL1_i)
{
};

//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
CT11Cpu::idle(
)
{
    pinMode(g_pinMap40DIL[s_BGND_i.pin],          INPUT);

    digitalWrite(g_pinMap40DIL[s__BCLR_o.pin],    HIGH);
    pinMode(g_pinMap40DIL[s__BCLR_o.pin],         OUTPUT);

    pinMode(g_pinMap40DIL[s_PUP_i.pin],           INPUT);
    pinMode(g_pinMap40DIL[s_GND_i.pin],           INPUT);

    m_pinCOUT.digitalWrite(LOW);
    m_pinCOUT.pinMode(OUTPUT);

    m_pinXTL1.pinMode(INPUT);
    pinMode(g_pinMap40DIL[s_XTL0_i.pin],          INPUT);

    m_pinSEL1.digitalWrite(LOW);
    m_pinSEL1.pinMode(OUTPUT);

    m_pinSEL0.digitalWrite(LOW);
    m_pinSEL0.pinMode(OUTPUT);

    pinMode(g_pinMap40DIL[s_READY_i.pin],         INPUT);

    m_pinR_WHB.digitalWrite(HIGH);
    m_pinR_WHB.pinMode(OUTPUT);

    m_pinR_WLB.digitalWrite(HIGH);
    m_pinR_WLB.pinMode(OUTPUT);

    m_pin_RAS.digitalWrite(HIGH);
    m_pin_RAS.pinMode(OUTPUT);

    m_pin_CAS.digitalWrite(HIGH);
    m_pin_CAS.pinMode(OUTPUT);

    m_pinPI.digitalWrite(LOW);
    m_pinPI.pinMode(OUTPUT);

    pinMode(g_pinMap40DIL[s_Vcc_i.pin],             INPUT);

    // Use the pullup input as the float to detect shorts to ground.
    m_busDALHi.pinMode(INPUT_PULLUP);
    m_busDALLo.pinMode(INPUT_PULLUP);
    m_busAI.pinMode(INPUT_PULLUP);

    return errorSuccess;
}

//
// The check function performs a basic pin check that all the outputs can be pulled high
// by the pullup resistor as a way to detect a GND short or pulled output. It also validates
// the default state of the control pins would allow the CPU to execute instructions.
//
// If check fails the pins are left in the failing state. If check succeeds the pins are reset
// to idle state.
//
// On the T11 the timing & configuration of the external bus cycle is configured as part of reset
// based on reading in the MR (Mode Register). As a result of that the "check" function also
// performs MR verification that a supported bus configuration is selected.
//
// Only the mode settings applicable to Atari System 2 are currently supported:
// DAL(MR) = 0011 0110 1111 1111 (0x36FF)
// => Processor Clock (COUT)
// => Standard Cycle
// => Normal Read/Write
// => Static Memory
// => 16-bit
//
//
PERROR
CT11Cpu::check(
)
{
    PERROR error = errorSuccess;

    // The ground pin (with pullup) should be connected to GND (LOW)
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_BGND_i, LOW);
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_GND_i, LOW);

    // The Vcc pin should be high (power is on).
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_Vcc_i, HIGH);

    // The power-up (reset) pin should be low.
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_PUP_i, LOW);

    // The READY input is not supported and it should be HIGH (ready)
    CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_READY_i, HIGH);

    // Loop to detect a clock by sampling and detecting both high and lows.
    {
        UINT16 hiCount = 0;
        UINT16 loCount = 0;

        for (int i = 0 ; i < 1000 ; i++)
        {
            int value = ::digitalRead(g_pinMap40DIL[s_XTL1_i.pin]);

            if (value == LOW)
            {
                loCount++;
            }
            else
            {
                hiCount++;
            }
        }

        if (loCount == 0)
        {
            CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_XTL1_i, LOW);
        }
        else if (hiCount == 0)
        {
            CHECK_VALUE_EXIT(error, g_pinMap40DIL, s_XTL1_i, HIGH);
        }
    }

    // Check that the mode register is properly set and the data bus is properly
    // pulled high (the T11 is explicit in pulling high the databus and thus this
    // is an important check on the T11).
    {
        CHECK_BUS_VALUE_UINT8_EXIT(error, m_busDALHi, s_DALHi_iot, 0xFF);
        CHECK_BUS_VALUE_UINT8_EXIT(error, m_busDALLo, s_DALLo_iot, 0xFF);

        ::digitalWrite(g_pinMap40DIL[s__BCLR_o.pin], LOW);
        CHECK_BUS_VALUE_UINT8_EXIT(error, m_busDALHi, s_DALHi_iot, 0x36);
        CHECK_BUS_VALUE_UINT8_EXIT(error, m_busDALLo, s_DALLo_iot, 0xFF);
        ::digitalWrite(g_pinMap40DIL[s__BCLR_o.pin], HIGH);
    }

    // Perform the ASPI cycle to read in the control pin state on the AI lines.
    {
        UINT16 ai = 0;

        m_pin_CAS.digitalWriteLOW();
        m_pinPI.digitalWriteHIGH();

        m_busAI.digitalRead(&ai);

        CHECK_LITERAL_VALUE_EXIT(error, s_AI_io[0], (ai & 0x01), 0x01); // DMR Hi
        CHECK_LITERAL_VALUE_EXIT(error, s_AI_io[5], (ai & 0x20), 0x20); // VEC Hi
        CHECK_LITERAL_VALUE_EXIT(error, s_AI_io[6], (ai & 0x40), 0x40); // PF Hi
        CHECK_LITERAL_VALUE_EXIT(error, s_AI_io[7], (ai & 0x80), 0x80); // HLT Hi

        m_pin_CAS.digitalWriteHIGH();
        m_pinPI.digitalWriteLOW();
    }

    // Final check that the databus has gone back to idle state.
    // Some time is needed after BCLR to allow the bus to float back
    // to all HI

    CHECK_BUS_VALUE_UINT8_EXIT(error, m_busDALHi, s_DALHi_iot, 0xFF);
    CHECK_BUS_VALUE_UINT8_EXIT(error, m_busDALLo, s_DALLo_iot, 0xFF);

Exit:
    return error;
}


UINT8
CT11Cpu::dataBusWidth(
    UINT32 address
)
{
    // 16-bit data bus only.
    return 2;
}


UINT8
CT11Cpu::dataAccessWidth(
    UINT32 address
)
{
    bool is16Bit = (address & s_16BitAddress) ? true : false;
    UINT8 width = 1;

    if (is16Bit)
    {
        width = 2;
    }

    return width;
}


PERROR
CT11Cpu::memoryReadWrite(
    UINT32 address,
    UINT16 *data,
    UINT8  *ai,
    bool   read
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;
    UINT16 data16 = 0;

    // Before processing anything, perform any address remapping.
    if (m_addressRemapCallback)
    {
        error = m_addressRemapCallback(m_addressRemapCallbackContext,
                                       address, &address);
        if (FAILED(error))
        {
            return error;
        }
    }

    bool lo      = ((address & 1) == 0) ? true : false;
    bool hi      = ((address & 1) == 1) ? true : false;
    bool is16Bit = (address & s_16BitAddress) ? true : false;
    bool vSync   = (address & s_vSyncAddress) ? true : false;
    UINT32 physicalAddress = address & 0xFFFE; // A0 not used

    //
    // This is implemented in the address remap callback so we
    // should never see it here.
    //
    if (vSync)
    {
        return errorNotImplemented;
    }

    // Drive the address
    m_busDALLo.pinMode(OUTPUT);
    m_busDALLo.digitalWrite((physicalAddress >> 0)  & 0xFF);

    m_busDALHi.pinMode(OUTPUT);
    m_busDALHi.digitalWrite((physicalAddress >> 8)  & 0xFF);

    // Drive the byte lane write strobes
    if (!read)
    {
        if (lo | is16Bit)
        {
            m_pinR_WLB.digitalWriteLOW();
        }

        if (hi | is16Bit)
        {
            m_pinR_WHB.digitalWriteLOW();
        }
    }

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    // Assert RAS to latch the address.
    m_pin_RAS.digitalWriteLOW();

    // Pulse COUT
    m_pinCOUT.digitalWriteHIGH();
    m_pinCOUT.digitalWriteLOW();

    if (read)
    {
        m_busDALLo.pinMode(INPUT);
        m_busDALHi.pinMode(INPUT);
    }
    else
    {
        if (lo | is16Bit)
        {
            // 8 & 16-bit cycle, Lo
            m_busDALLo.digitalWrite((*data >> 0) & 0xFF);
        }

        if (is16Bit)
        {
            // 16-bit cycle, Hi
            m_busDALHi.digitalWrite((*data >> 8) & 0xFF);
        }
        else if (hi)
        {
            // 8-bit cycle, Hi
            m_busDALHi.digitalWrite((*data >> 0) & 0xFF);
        }
    }

    // Assert CAS to signal data
    m_pin_CAS.digitalWriteLOW();

    // Assert PI to latch the AI lines
    m_pinPI.digitalWriteHIGH();

    // Pulse COUT
    m_pinCOUT.digitalWriteHIGH();
    m_pinCOUT.digitalWriteLOW();

    // Read AI if requested
    if (ai != NULL)
    {
        UINT16 ai16 = 0;

        m_busAI.digitalRead(&ai16);
        *ai = (UINT8) ai16;
    }

    // Read in the data
    if (read)
    {
        UINT16 tempData16 = 0;

        if (lo | is16Bit)
        {
            // 8 & 16-bit cycle, Lo
            m_busDALLo.digitalRead(&tempData16);
            data16 |= (tempData16 & 0xFF) << 0;
        }

        if (is16Bit)
        {
            // 16-bit cycle, Hi
            m_busDALHi.digitalRead(&tempData16);
            data16 |= (tempData16 & 0xFF) << 8;
        }
        else if (hi)
        {
            // 8-bit cycle, Hi
            m_busDALHi.digitalRead(&tempData16);
            data16 |= (tempData16 & 0xFF) << 0;
        }
    }

    // Deassert CAS, PI & RAS
    m_pin_CAS.digitalWriteHIGH();
    m_pinPI.digitalWriteLOW();
    m_pin_RAS.digitalWriteHIGH();

    // Restore data state
    if (!read)
    {
        if (lo | is16Bit)
        {
            m_busDALLo.pinMode(INPUT_PULLUP);
            m_pinR_WLB.digitalWriteHIGH();
        }

        if (hi | is16Bit)
        {
            m_busDALHi.pinMode(INPUT_PULLUP);
            m_pinR_WHB.digitalWriteHIGH();
        }
    }

    if (interruptsDisabled)
    {
        interrupts();
    }

    *data = data16;

    return error;
}


PERROR
CT11Cpu::memoryRead(
    UINT32 address,
    UINT16  *data
)
{
    return memoryReadWrite(address, data, (UINT8*) NULL, true);
}


PERROR
CT11Cpu::memoryWrite(
    UINT32 address,
    UINT16 data
)
{
    return memoryReadWrite(address, &data, (UINT8*) NULL, false);
}


PERROR
CT11Cpu::waitForInterrupt(
    Interrupt interrupt,
    bool      active,
    UINT32    timeoutInMs
)
{
    PERROR error     = errorSuccess;
    UINT16 aiBitMask = 0;
    bool   triggered = false;

    switch (interrupt)
    {
        case IRQ3 : { aiBitMask = 0x02; break; } // AI1_CP3
        case IRQ2 : { aiBitMask = 0x04; break; } // AI2_CP2
        case IRQ1 : { aiBitMask = 0x08; break; } // AI3_CP1
        case IRQ0 : { aiBitMask = 0x10; break; } // AI4_CP0
        case HALT : { aiBitMask = 0x80; break; } // AI7_HLT

        default :
        {
            error = errorNotImplemented;
            goto Exit;;
        }
    }

    {
        unsigned long startTime = millis();
        unsigned long endTime   = startTime + timeoutInMs;
        UINT8 value = (active ? 0 : aiBitMask);

        do
        {
            UINT32 address = 0xFFFE;
            UINT16 data    = 0;
            UINT8  ai      = 0;

            error = memoryReadWrite(address, &data, &ai, true);
            if (FAILED(error))
            {
                goto Exit;
            }

            if ((ai & aiBitMask) == value)
            {
                triggered = true;
                break;
            }
        }
        while (millis() < endTime);

        if (!triggered)
        {
            error = errorTimeout;
        }
    }

Exit:

    return error;
}


//
// TBD.
//
PERROR
CT11Cpu::acknowledgeInterrupt(
    UINT16 *response
)
{
    PERROR error = errorSuccess;

    *response = 0;

    return error;
}

