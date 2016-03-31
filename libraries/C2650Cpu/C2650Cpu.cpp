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
#include "Arduino.h"
#include "Error.h"
#include "C2650Cpu.h"
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
static const CONNECTION s_SENSE_i    = {  1, "SENSE"    };
static const CONNECTION s__ADREN_i   = { 15, "_ADREN"   };
static const CONNECTION s_RESET_i    = { 16, "RESET"    };
static const CONNECTION s__INTREQ_i  = { 17, "_INTREQ"  };
static const CONNECTION s_M_IO_o     = { 20, "M_IO"     };
static const CONNECTION s_GND_i      = { 21, "GND"      };
static const CONNECTION s_WRP_o      = { 22, "WRP"      };
static const CONNECTION s__RW_o      = { 23, "_RW"      };
static const CONNECTION s_OPREQ_o    = { 24, "OPREQ"    };
static const CONNECTION s__DBUSEN_i  = { 25, "_DBUSEN"  };
static const CONNECTION s_INTACK_o   = { 34, "INTACK"   };
static const CONNECTION s_RUN_WAIT_o = { 35, "RUN_WAIT" };
static const CONNECTION s__OPACK_i   = { 36, "_OPACK"   };
static const CONNECTION s__PAUSE_i   = { 37, "_PAUSE"   };
static const CONNECTION s_CLOCK_i    = { 38, "CLOCK"    };
static const CONNECTION s_Vcc_i      = { 39, "Vcc"      };
static const CONNECTION s_FLAG_o     = { 40, "FLAG"     };;

//
// Dual function pins
//
static const CONNECTION s_D_C_ot  = { 18, "D_C"  };
static const CONNECTION s_E_NE_ot = { 19, "N_NE" };

//
// Bus pins
//
static const CONNECTION s_ADR_ot[]   = { {14, "ADR0"  },
                                         {13, "ADR1"  },
                                         {12, "ADR2"  },
                                         {11, "ADR3"  },
                                         {10, "ADR4"  },
                                         { 9, "ADR5"  },
                                         { 8, "ADR6"  },
                                         { 7, "ADR7"  },
                                         { 6, "ADR8"  },
                                         { 5, "ADR9"  },
                                         { 4, "ADR10" },
                                         { 3, "ADR11" },
                                         { 2, "ADR12" },
                                         {19, "ADR13" },
                                         {18, "ADR14" } }; // 15 bits

static const CONNECTION s_DBUS_iot[] = { {33, "DBUS0" },
                                         {32, "DBUS1" },
                                         {31, "DBUS2" },
                                         {30, "DBUS3" },
                                         {29, "DBUS4" },
                                         {28, "DBUS5" },
                                         {27, "DBUS6" },
                                         {26, "DBUS7" } }; // 8 bits.


C2650Cpu::C2650Cpu(
) : m_busADR(g_pinMap40DIL, s_ADR_ot, ARRAYSIZE(s_ADR_ot)),
    m_busDBUS(g_pinMap40DIL, s_DBUS_iot, ARRAYSIZE(s_DBUS_iot)),
    m_pinCLOCK(g_pinMap40DIL, &s_CLOCK_i),
    m_pinOPREQ(g_pinMap40DIL, &s_OPREQ_o),
    m_pinWRP(g_pinMap40DIL, &s_WRP_o),
    m_pin_OPACK(g_pinMap40DIL, &s__OPACK_i)
{
};

//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
C2650Cpu::idle(
)
{
    pinMode(g_pinMap40DIL[s_SENSE_i.pin],         INPUT);
    pinMode(g_pinMap40DIL[s__ADREN_i.pin],        INPUT);
    pinMode(g_pinMap40DIL[s_RESET_i.pin],         INPUT);
    pinMode(g_pinMap40DIL[s__INTREQ_i.pin],       INPUT);

    digitalWrite(g_pinMap40DIL[s_M_IO_o.pin],     HIGH);
    pinMode(g_pinMap40DIL[s_M_IO_o.pin],          OUTPUT);

    pinMode(g_pinMap40DIL[s_GND_i.pin],           INPUT_PULLUP);

    digitalWrite(g_pinMap40DIL[s__RW_o.pin],      HIGH);
    pinMode(g_pinMap40DIL[s__RW_o.pin],           OUTPUT);

    pinMode(g_pinMap40DIL[s__DBUSEN_i.pin],       INPUT);

    digitalWrite(g_pinMap40DIL[s_INTACK_o.pin],   LOW);
    pinMode(g_pinMap40DIL[s_INTACK_o.pin],        OUTPUT);

    digitalWrite(g_pinMap40DIL[s_RUN_WAIT_o.pin], LOW);
    pinMode(g_pinMap40DIL[s_RUN_WAIT_o.pin],      OUTPUT);

    pinMode(g_pinMap40DIL[s__PAUSE_i.pin],        INPUT);
    pinMode(g_pinMap40DIL[s_Vcc_i.pin],           INPUT);

    digitalWrite(g_pinMap40DIL[s_FLAG_o.pin],     LOW);
    pinMode(g_pinMap40DIL[s_FLAG_o.pin],          OUTPUT);

    // since the address bus is controlled by ADREN we can't set it output.
    m_busADR.pinMode(INPUT_PULLUP);

    // since the data bus is controlled by DBUSEN we can't set it output.
    m_busDBUS.pinMode(INPUT_PULLUP);

    // Set the fast clock & opack pins to input
    m_pinCLOCK.pinMode(INPUT);
    m_pin_OPACK.pinMode(INPUT);

    // Set the fast WRP & OPREQ pin to output and LOW.
    m_pinOPREQ.digitalWrite(LOW);
    m_pinOPREQ.pinMode(OUTPUT);

    m_pinWRP.digitalWrite(HIGH);
    m_pinWRP.pinMode(OUTPUT);

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
PERROR
C2650Cpu::check(
)
{
    PERROR error = errorSuccess;

    // The ground pin (with pullup) should be connected to GND (LOW)
    CHECK_VALUE_EXIT(error, s_GND_i, LOW);

    // The Vcc pin should be high (power is on).
    CHECK_VALUE_EXIT(error, s_Vcc_i, HIGH);

    // The reset pin should be low (no reset).
    CHECK_VALUE_EXIT(error, s_RESET_i, LOW);

    // The pause pin should be high (running).
    CHECK_VALUE_EXIT(error, s__PAUSE_i, HIGH);

    // In everything we'll be testing, ADREN is tied low.
    CHECK_VALUE_EXIT(error, s__ADREN_i, LOW);

    // In everything we'll be testing, DBUSEN is tied low.
    CHECK_VALUE_EXIT(error, s__DBUSEN_i, LOW);

    // The address bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT16_EXIT(error, m_busADR,  s_ADR_ot,   0x7FFF);

    // The data bus should be uncontended and pulled high.
    CHECK_BUS_VALUE_UINT8_EXIT(error, m_busDBUS, s_DBUS_iot, 0xFF);

    // Loop to detect a clock by sampling and detecting both high and lows.
    {
        UINT16 hiCount = 0;
        UINT16 loCount = 0;

        for (int i = 0 ; i < 1000 ; i++)
        {
            int value = ::digitalRead(g_pinMap40DIL[s_CLOCK_i.pin]);

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
            CHECK_VALUE_EXIT(error, s_CLOCK_i, LOW);
        }
        else if (hiCount == 0)
        {
            CHECK_VALUE_EXIT(error, s_CLOCK_i, HIGH);
        }
    }

Exit:
    return error;
}


UINT8
C2650Cpu::dataBusWidth(
    UINT32 address
)
{
    return 1;
}


UINT8
C2650Cpu::dataAccessWidth(
    UINT32 address
)
{
    return 1;
}


PERROR
C2650Cpu::memoryRead(
    UINT32 address,
    UINT16 *data
)
{
    if (address > (UINT32) 0xFFFF)
    {
        digitalWrite(g_pinMap40DIL[s_M_IO_o.pin], LOW);
    }
    else
    {
        digitalWrite(g_pinMap40DIL[s_M_IO_o.pin], HIGH);
    }

    digitalWrite(g_pinMap40DIL[s_INTACK_o.pin], LOW);

    return read((address & 0xFFFF), data);
}

PERROR
C2650Cpu::memoryWrite(
    UINT32 address,
    UINT16 data
)
{
    if (address > (UINT32) 0xFFFF)
    {
        digitalWrite(g_pinMap40DIL[s_M_IO_o.pin], LOW);
    }
    else
    {
        digitalWrite(g_pinMap40DIL[s_M_IO_o.pin], HIGH);
    }

    digitalWrite(g_pinMap40DIL[s_INTACK_o.pin], LOW);

    return write((address & 0xFFFF), data);
}


//
// This is the internal function that does a read cycle. The caller is expected
// to set the M_IO pin to reflect the actual space being accessed.
//
PERROR
C2650Cpu::read(
    UINT32 address,
    UINT16 *data
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;

    // Set a read cycle.
    digitalWrite(g_pinMap40DIL[s__RW_o.pin], LOW);

    // By default WRP is high unless pulsed for write.
    m_pinWRP.digitalWrite(HIGH);

    // Enable the address bus and set the value.
    m_busADR.pinMode(OUTPUT);
    m_busADR.digitalWrite((UINT16) address);

    // Set the databus to input.
    m_busDBUS.pinMode(INPUT);

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    // Timing Note
    // -----------
    // A note about timing. The Mega is a 16MHz device with mostly 1
    // clock per instruction so one would expect this to be fast enough
    // to pin drive a 2MHz bus. That said, from the scope trace even a
    // simple Hi/Lo on OPREQ looks to be taking >1 2MHz cycle, at least
    // 8 clocks or so. Therefore, there is no point in doing clock edge
    // synchronization.
    //
    // The "write" OPREQ length shows about 3x longer than the 2650 cycle.
    // The "read" OPREQ length shows longer than the scope screen, 6x plus
    // at least. Likely this is due to the 8 pin reads to read the data bus.
    // This length exceeds the 2636 hold time and thus the 2636 reads can be
    // unreliable.
    //
    // In retrospect, it makes sense to layout the ICT shield to group the
    // data bus into a single port so a single port read can read the data.
    // This would, however, make the shield itself 2650 specific.
    //

    // Start the cycle by asserting OPREQ.
    m_pinOPREQ.digitalWriteHIGH();

    // Poll OPACK for cycle completion
    {
        int opackValue;

        for (int i = 0 ; i < 64 ; i++)
        {
            opackValue = m_pin_OPACK.digitalRead();

            if (opackValue == LOW)
            {
                // Read the data presented on the bus as soon as we see OPACK set then clear OPREQ.
                m_busDBUS.digitalReadThenDigitalWriteLOW(data, m_pinOPREQ);

                break;
            }
        }

        CHECK_LITERAL_VALUE_EXIT(error, s__OPACK_i, opackValue, LOW);
    }

Exit:

    if (interruptsDisabled)
    {
        interrupts();
    }
    return error;
}

//
// This is the internal function that does a write cycle. The caller is expected
// to set the M_IO pin to reflect the actual space being accessed.
//
PERROR
C2650Cpu::write(
    UINT32 address,
    UINT16 data
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;

    // Set a write cycle.
    digitalWrite(g_pinMap40DIL[s__RW_o.pin], HIGH);

    // By default WRP is high unless pulsed for write.
    m_pinWRP.digitalWrite(HIGH);

    // Enable the address bus and set the value.
    m_busADR.pinMode(OUTPUT);
    m_busADR.digitalWrite((UINT16) address);

    // Set the databus to output and set a value.
    m_busDBUS.pinMode(OUTPUT);
    m_busDBUS.digitalWrite(data);

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    // Timing Note
    // -----------
    // See "read".

    // Clear WRP
    m_pinWRP.digitalWriteLOW();

    // Start the cycle by asserting OPREQ.
    m_pinOPREQ.digitalWriteHIGH();

    // Set WRP
    m_pinWRP.digitalWriteHIGH();

    // Poll OPACK for cycle completion
    {
        int opackValue;

        for (int i = 0 ; i < 64 ; i++)
        {
            opackValue = m_pin_OPACK.digitalRead();

            if (opackValue == LOW)
            {
                // Clear WRP
                m_pinWRP.digitalWriteLOW();

                // End the cycle by clearing OPREQ.
                m_pinOPREQ.digitalWriteLOW();

                // Set WRP
                m_pinWRP.digitalWriteHIGH();
                break;
            }
        }
        CHECK_LITERAL_VALUE_EXIT(error, s__OPACK_i, opackValue, LOW);
    }

Exit:

    if (interruptsDisabled)
    {
        interrupts();
    }

    return error;
}


void
C2650Cpu::flagWrite(
    UINT8 value
)
{
    digitalWrite(g_pinMap40DIL[s_FLAG_o.pin], value);
}


UINT8
C2650Cpu::senseRead(
)
{
    return digitalRead(g_pinMap40DIL[s_SENSE_i.pin]);
}


PERROR
C2650Cpu::waitForInterrupt(
    Interrupt interrupt,
    UINT16    timeoutInMs
)
{
    PERROR error = errorSuccess;
    unsigned long startTime = millis();
    unsigned long endTime = startTime + timeoutInMs;
    int value = 0;

    UINT8 intPin = g_pinMap40DIL[s__INTREQ_i.pin];

    do
    {
        value = ::digitalRead(intPin);

        if (value == LOW)
        {
            break;
        }
    }
    while (millis() < endTime);

    if (value != LOW)
    {
        error = errorTimeout;
    }

Exit:

    return error;
}


//
// The 2650 UM states that only INTACK asserted with OPREQ are all that are needed to get the
// interrupt acknowledged and the address returned. However, both MAME, Galaxia & Quasar show
// INTACK is only used to gate off ~REDC, i.e. the INTACK cycle is an IO read control
// (at least A14 and A15 low).
// FYI, Galaxia is hard coded to respond with 0x03.
//
PERROR
C2650Cpu::acknowledgeInterrupt(
    UINT16 *response
)
{
    digitalWrite(g_pinMap40DIL[s_M_IO_o.pin], LOW);

    digitalWrite(g_pinMap40DIL[s_INTACK_o.pin], HIGH);

    return read(0, response);
}

