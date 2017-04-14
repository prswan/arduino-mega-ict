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
#ifndef PinMap8080_h
#define PinMap8080_h

#include "Arduino.h"
#include "Types.h"

//
// The 8080 probe head connector.
//
static const UINT8 g_pinMap8080[] = { 0xFF,   //        0 - Not used
                                        24,   // 24-PA2-1-A10
                                        16,   // 16-PH1-2-GND
                                        33,   // 33-PC4-3-D4
                                        32,   // 32-PC5-4-D5
                                        31,   // 31-PC6-5-D6
                                        30,   // 30-PC7-6-D7
                                        34,   // 34-PC3-7-D3
                                        35,   // 35-PC2-8-D2
                                        36,   // 36-PC1-9-D1
                                        37,   // 37-PC0-10-D0
                                      0xFF,   // n/c   -11-VBB(-5V)
                                        14,   // 14-PJ1-12-RESET
                                        50,   // 50-PB3-13-HOLD
                                        39,   // 39-PG2-14-INT
                                        51,   // 51-PB2-15-CLK2
                                        40,   // 40-PG1-16-INTE
                                        18,   // 18-PD3-17-DBIN
                                        19,   // 19-PD2-18-WR
                                        20,   // 20-PD1-19-SYNC
                                        17,   // 17-PH0-20-VCC
                                        21,   // 21-PD0-21-HLDA
                                        53,   // 53-PB0-22-CLK1
                                        52,   // 52-PB1-23-READY
                                        38,   // 38-PD7-24-WAIT
                                        49,   // 49-PL0-25-A0
                                        48,   // 48-PL1-26-A1
                                        47,   // 47-PL2-27-A2
                                      0xFF,   // n/c   -28-VDD(+12V)
                                        46,   // 46-PL3-29-A3
                                        45,   // 45-PL4-30-A4
                                        44,   // 44-PL5-31-A5
                                        43,   // 43-PL6-32-A6
                                        42,   // 42-PL7-33-A7
                                        22,   // 22-PA0-34-A8
                                        23,   // 23-PA1-35-A9
                                        29,   // 29-PA7-36-A15
                                        26,   // 26-PA4-37-A12
                                        27,   // 27-PA5-38-A13
                                        28,   // 28-PA6-39-A14
                                        25    // 25-PA3-40-A11
                                    };


//
// Byte-wide port control registers
//
static volatile UINT8 * const g_dirAddressLo      = &DDRL;
static volatile UINT8 * const g_portInAddressLo   = &PINL;
static volatile UINT8 * const g_portOutAddressLo  = &PORTL;

static volatile UINT8 * const g_dirAddressHi      = &DDRA;
static volatile UINT8 * const g_portInAddressHi   = &PINA;
static volatile UINT8 * const g_portOutAddressHi  = &PORTA;

static volatile UINT8 * const g_dirData           = &DDRC;
static volatile UINT8 * const g_portInData        = &PINC;
static volatile UINT8 * const g_portOutData       = &PORTC;

static volatile UINT8 * const g_dirControlIn      = &DDRB;
static volatile UINT8 * const g_portInControlIn   = &PINB;
static volatile UINT8 * const g_portOutControlIn  = &PORTB;

static volatile UINT8 * const g_dirControlOut     = &DDRD;
static volatile UINT8 * const g_portInControlOut  = &PIND;
static volatile UINT8 * const g_portOutControlOut = &PORTD;

//
// Byte-wide state controls
//
static const UINT8 s_PORT_BYTE_OFF    = 0x00;
static const UINT8 s_PORT_BYTE_PULLUP = 0xFF;

static const UINT8 s_DIR_BYTE_INPUT   = 0x00;
static const UINT8 s_DIR_BYTE_OUTPUT  = 0xFF;

//
// Bit-wise input definitions.
//
static const UINT8 s_BIT_IN_CLK1      = 0x01;
static const UINT8 s_BIT_IN_READY     = 0x02;
static const UINT8 s_BIT_IN_CLK2      = 0x04;
static const UINT8 s_BIT_IN_HOLD      = 0x08;

//
// Bit-wise output definitions.
//
static const UINT8 s_BIT_OUT_HLDA     = 0x01;
static const UINT8 s_BIT_OUT_SYNC     = 0x02;
static const UINT8 s_BIT_OUT_WR       = 0x04;
static const UINT8 s_BIT_OUT_DBIN     = 0x08;

static const UINT8 s_BIT_OUT_WAIT     = 0x80;

#endif
