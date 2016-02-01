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
#include "C6802Cpu.h"
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
static const CONNECTION s_GND1_i    = {  1, "GND1 "   };
static const CONNECTION s__HALT_i   = {  2, "/HALT "  };
static const CONNECTION s_MR_i      = {  3, "MR "     };
static const CONNECTION s__IRQ_i    = {  4, "/IRQ "   };
static const CONNECTION s_VMA_o     = {  5, "VMA "    };
static const CONNECTION s__NMI_i    = {  6, "/NMI "   };
static const CONNECTION s_BA_o      = {  7, "BA "     };
static const CONNECTION s_VCC1_i    = {  8, "Vcc1 "   };
static const CONNECTION s_GND2_i    = { 21, "GND2 "   };
static const CONNECTION s_R_W_o     = { 34, "R/W "    };
static const CONNECTION s_VCC2_i    = { 35, "Vcc2 "   };
static const CONNECTION s_RE_i      = { 36, "RE "     };
static const CONNECTION s_E_o       = { 37, "E "      };
static const CONNECTION s_XTAL_i    = { 38, "XTAL "   };
static const CONNECTION s_EXTAL_i   = { 39, "EXTAL "  };
static const CONNECTION s__RESET_i  = { 40, "/RESET " };

//
// Bus pins
//
static const CONNECTION s_A_o[]   = { {  9, "A0"  },
                                      { 10, "A1"  },
                                      { 11, "A2"  },
                                      { 12, "A3"  },
                                      { 13, "A4"  },
                                      { 14, "A5"  },
                                      { 15, "A6"  },
                                      { 16, "A7"  },
                                      { 17, "A8"  },
                                      { 18, "A9"  },
                                      { 19, "A10" },
                                      { 20, "A11" },
                                      { 22, "A12" },
                                      { 23, "A13" },
                                      { 24, "A14" },
                                      { 25, "A15" } }; // 16 bits

static const CONNECTION s_D_iot[] = { { 33, "D0"  },
                                      { 32, "D1"  },
                                      { 31, "D2"  },
                                      { 30, "D3"  },
                                      { 29, "D4"  },
                                      { 28, "D5"  },
                                      { 27, "D6"  },
                                      { 26, "D7"  } }; // 8 bits.

C6802Cpu::C6802Cpu(
) : m_busA(g_pinMap40DIL, s_A_o,  ARRAYSIZE(s_A_o)),
    m_busD(g_pinMap40DIL, s_D_iot, ARRAYSIZE(s_D_iot)),
    m_pinE(g_pinMap40DIL, &s_E_o),
    m_pinR_W(g_pinMap40DIL, &s_R_W_o)
{
};


//
// The idle function sets up the pins into the correct direction (input/output)
// and idle state ready for the next bus cycle.
//
PERROR
C6802Cpu::idle(
)
{
    pinMode(g_pinMap40DIL[s_RE_i.pin],            INPUT);   // Don't Care - internal/external RAM select
    pinMode(g_pinMap40DIL[s_XTAL_i.pin],          INPUT);   // Don't Care - clock input
    pinMode(g_pinMap40DIL[s_EXTAL_i.pin],         INPUT);   // Don't Care - clock input
    pinMode(g_pinMap40DIL[s_BA_o.pin],           OUTPUT);   // Don't Care - not connected
    digitalWrite(g_pinMap40DIL[s_BA_o.pin],         LOW);   // Don't Care - not connected

    pinMode(g_pinMap40DIL[s_GND1_i.pin],   INPUT_PULLUP);   // Always low
    pinMode(g_pinMap40DIL[s_GND2_i.pin],   INPUT_PULLUP);   // Always low
    pinMode(g_pinMap40DIL[s_VCC1_i.pin],          INPUT);   // Always high
    pinMode(g_pinMap40DIL[s_VCC2_i.pin],          INPUT);   // Always high


    pinMode(g_pinMap40DIL[s__RESET_i.pin],        INPUT);   // Always high unless fault in reset circuit
    pinMode(g_pinMap40DIL[s__HALT_i.pin],         INPUT);   // Pulled high by R27
    pinMode(g_pinMap40DIL[s_MR_i.pin],            INPUT);   // Pulled high by R31
    pinMode(g_pinMap40DIL[s__NMI_i.pin],          INPUT);   // Pulled high by R1 grounded if sound test button is pressed

    pinMode(g_pinMap40DIL[s_VMA_o.pin],          OUTPUT);   // Always high?
    digitalWrite(g_pinMap40DIL[s_VMA_o.pin],       HIGH);   // Always high?

    pinMode(g_pinMap40DIL[s__IRQ_i.pin],          INPUT);   // IRQ from 6821 U10 Pin 37+38

    // Use the pullup input as the float to detect shorts to ground.
    m_busA.pinMode(INPUT_PULLUP);
    m_busD.pinMode(INPUT_PULLUP);

    // Initialise the E pin to be low
    m_pinE.pinMode(OUTPUT);
    m_pinE.digitalWrite(LOW);

    // Initialise the R/W pin to be high
    m_pinR_W.pinMode(OUTPUT);
    m_pinR_W.digitalWrite(HIGH);

    // Initialise outputs on the 6821 U10
    memoryWrite(0x0401, 0x00); // Set DDR (xxxxx0xx)
    memoryWrite(0x0400, 0xFF); // Set PA0-PA7 as output pins
    memoryWrite(0x0401, 0x34); // Disable CA1 (xxxxxx00), Set PR (xxxxx1xx), Enable output low CA2 (xx110xxx)
    memoryWrite(0x0400, 0x00); // Set outputs PA0-PA7 to all be low

    // Initialise inputs on the 6821 U10
    memoryWrite(0x0403, 0x00); // Set DDR (xxxxx0xx)
    memoryWrite(0x0402, 0x00); // Set PB0-PB7 as input pins
    memoryWrite(0x0403, 0x37); // Enable CB1 low > high IRQ (xxxxxx11), Set PR (xxxxx1xx), Enable output low CB2 (xx110xxx)

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
C6802Cpu::check(
)
{
    PERROR error = errorSuccess;

    // The ground pins should be connected to GND
    CHECK_VALUE_EXIT(error, s_GND1_i, LOW);
    CHECK_VALUE_EXIT(error, s_GND2_i, LOW);

    // The Vcc pins should be high (power is on)
    CHECK_VALUE_EXIT(error, s_VCC1_i, HIGH);
    CHECK_VALUE_EXIT(error, s_VCC2_i, HIGH);

    // The /RESET pin should be high, there is no watchdog here
    CHECK_VALUE_EXIT(error, s__RESET_i, HIGH);

    // The /HALT pin is pulled high by R27
    CHECK_VALUE_EXIT(error, s__HALT_i, HIGH);

    // The MR pin is pulled high by R31
    CHECK_VALUE_EXIT(error, s_MR_i, HIGH);

    // The /NMI pin is pulled high by R1 and should only go low if the test button is pressed
    CHECK_VALUE_EXIT(error, s__NMI_i, HIGH);

    // The address bus should be uncontended and pulled high
    CHECK_BUS_VALUE_UINT16_EXIT(error, m_busA, s_A_o, 0xFFFF);

    // The data bus should be uncontended and pulled high
    // CHECK_BUS_VALUE_UINT8_EXIT(error, m_busD, s_D_iot, 0xFF);  // Fails on D0???

    // The VMA pin should be high
    CHECK_VALUE_EXIT(error, s_VMA_o, HIGH);

    // The E pin should be low
    CHECK_VALUE_EXIT(error, s_E_o, LOW);

    // The R/W pin should be high
    CHECK_VALUE_EXIT(error, s_R_W_o, HIGH);

Exit:

    return error;
}


PERROR
C6802Cpu::memoryRead(
    UINT32 address,
    UINT8  *data
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;
    UINT16 data16 = 0;
    *data = 0;

    // The ground pins (with pullup) should be connected to GND (LOW)
    CHECK_VALUE_EXIT(error, s_GND1_i, LOW);
    CHECK_VALUE_EXIT(error, s_GND2_i, LOW);

    // The Vcc pins should be high
    CHECK_VALUE_EXIT(error, s_VCC1_i, HIGH);
    CHECK_VALUE_EXIT(error, s_VCC2_i, HIGH);

    // The VMA pin should be high
    CHECK_VALUE_EXIT(error, s_VMA_o, HIGH);

    // Enable the address bus and set the value
    m_busA.pinMode(OUTPUT);
    m_busA.digitalWrite((UINT16) (address & 0xFFFF));

    // Set up a read cycle
    m_pinR_W.digitalWrite(HIGH);
    CHECK_VALUE_EXIT(error, s_R_W_o, HIGH);

    // pulse the clock to high
    m_pinE.digitalWrite(LOW);
    m_pinE.digitalWrite(HIGH);

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    // Set the databus to input and read data
    m_busD.pinMode(INPUT);
    m_busD.digitalRead(&data16);
    *data = (UINT8) data16;

Exit:

    // re-enable interrupts
    if (interruptsDisabled) interrupts();

    return error;
}


PERROR
C6802Cpu::memoryWrite(
    UINT32 address,
    UINT8  data
)
{
    PERROR error = errorSuccess;
    bool interruptsDisabled = false;
    UINT16 data16 = data;

    // The ground pins (with pullup) should be connected to GND (LOW)
    CHECK_VALUE_EXIT(error, s_GND1_i, LOW);
    CHECK_VALUE_EXIT(error, s_GND2_i, LOW);

    // The Vcc pins should be high
    CHECK_VALUE_EXIT(error, s_VCC1_i, HIGH);
    CHECK_VALUE_EXIT(error, s_VCC2_i, HIGH);

    // The VMA pin should be high
    CHECK_VALUE_EXIT(error, s_VMA_o, HIGH);

    // Enable the address bus and set the value
    m_busA.pinMode(OUTPUT);
    m_busA.digitalWrite((UINT16) (address & 0xFFFF));

    // Set up a write cycle
    m_pinR_W.digitalWrite(LOW);
    CHECK_VALUE_EXIT(error, s_R_W_o, LOW);

    // pulse the clock to high
    m_pinE.digitalWrite(LOW);
    m_pinE.digitalWrite(HIGH);

    // Critical timing section
    noInterrupts();
    interruptsDisabled = true;

    // Set the databus to output and write data
    m_busD.pinMode(OUTPUT);
    m_busD.digitalWrite((UINT16) data);

Exit:

    // re-enable interrupts
    if (interruptsDisabled) interrupts();

    // Go back to read mode
    m_pinR_W.digitalWrite(HIGH);
    m_busD.pinMode(INPUT);

    return error;
}


PERROR
C6802Cpu::waitForInterrupt(
    Interrupt interrupt,
    UINT16 timeoutInMs
)
{
    PERROR error = errorSuccess;
    unsigned long startTime = millis();
    unsigned long endTime = startTime + timeoutInMs;
    int value = 0;

    UINT8 intPin = ((interrupt == NMI) ? (g_pinMap40DIL[s__NMI_i.pin]) :
                                         (g_pinMap40DIL[s__IRQ_i.pin]));

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


PERROR
C6802Cpu::acknowledgeInterrupt(
    UINT8     *response
)
{
    PERROR error = errorSuccess;

    *response = 0;

    return error;
}