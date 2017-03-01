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
#ifndef PinMap68000_h
#define PinMap68000_h

#include "Arduino.h"
#include "Types.h"

//
// The 68000 probe head connector.
//
// Since we're two pins short the following pins are not mapped and pulled up to Vcc:
//
// - 12-BG
// - 13-BGACK
//
// WARNING: Ports E, G & H are used by the LCD/Keypad shield and thus cannot be directly used!
//
static const UINT8 g_pinMap68000[] = { 0xFF,   //        0 - Not used
                                         26,   // 26-PA4-1-D4
                                         25,   // 25-PA3-2-D3
                                         24,   // 24-PA2-3-D2
                                         23,   // 23-PA1-4-D1
                                         22,   // 22-PA0-5-D0
                                         21,   // 21-PD0-6-AS
                                         20,   // 20-PD1-7-UDS
                                         19,   // 19-PD2-8-LDS
                                         49,   // 49-PL0-9-RW
                                         51,   // 51-PB2-10-DTACK
                                       0xFF,   // n/c pu-11-BG
                                       0xFF,   // n/c pu-12-BGACK
                                         16,   // 16-PH1-13-BR
                                         17,   // 17-PH0-14-VCC
                                         53,   // 53-PB0-15-CLK
                                       0xFF,   // n/c   -16-GND
                                         14,   // 14-PJ1-17-HALT
                                         15,   // 15-PJ0-18-RESET
                                         18,   // 18-PD3-19-VMA
                                         38,   // 38-PD7-20-E
                                         52,   // 52-PB1-21-VPA
                                         50,   // 50-PB3-22-BERR
                                         39,   // 39-PG2-23-IPL2
                                         40,   // 40-PG1-24-IPL1
                                         41,   // 41-PG0-25-IPL0
                                         46,   // 46-PL3-26-FC2
                                         47,   // 47-PL2-27-FC1
                                         48,   // 48-PL1-28-FC0
                                       0xFF,   // n/c   -29-A1
                                       0xFF,   // n/c   -30-A2
                                       0xFF,   // n/c   -31-A3
                                       0xFF,   // n/c   -32-A4
                                       0xFF,   // n/c   -33-A5
                                       0xFF,   // n/c   -34-A6
                                       0xFF,   // n/c   -35-A7
                                       0xFF,   // n/c   -36-A8
                                       0xFF,   // n/c   -37-A9
                                       0xFF,   // n/c   -38-A10
                                       0xFF,   // n/c   -39-A11
                                       0xFF,   // n/c   -40-A12
                                       0xFF,   // n/c   -41-A13
                                       0xFF,   // n/c   -42-A14
                                       0xFF,   // n/c   -43-A15
                                       0xFF,   // n/c   -44-A16
                                       0xFF,   // n/c   -45-A17
                                       0xFF,   // n/c   -46-A18
                                       0xFF,   // n/c   -47-A19
                                       0xFF,   // n/c   -48-A20
                                       0xFF,   // n/c   -49-VCC
                                       0xFF,   // n/c   -50-A21
                                       0xFF,   // n/c   -51-A22
                                       0xFF,   // n/c   -52-A23
                                       0xFF,   // n/c   -53-GND
                                         30,   // 30-PC7-54-D15
                                         31,   // 31-PC6-55-D14
                                         32,   // 32-PC5-56-D13
                                         33,   // 33-PC4-57-D12
                                         34,   // 34-PC3-58-D11
                                         35,   // 35-PC2-59-D10
                                         36,   // 36-PC1-60-D9
                                         37,   // 37-PC0-61-D8
                                         29,   // 29-PA7-62-D7
                                         28,   // 28-PA6-63-D6
                                         27,   // 27-PA5-64-D5
                                         42,   // 42-PL7-LE2
                                         43,   // 43-PL6-LE1
                                         44,   // 44-PL5-LE0
                                         45,   // 45-PL4-LOE
                                     };


//
// Byte-wide port control registers
//
static volatile UINT8 * const g_dirDataLo          = &DDRA;
static volatile UINT8 * const g_portInDataLo       = &PINA;
static volatile UINT8 * const g_portOutDataLo      = &PORTA;

static volatile UINT8 * const g_dirDataHi          = &DDRC;
static volatile UINT8 * const g_portInDataHi       = &PINC;
static volatile UINT8 * const g_portOutDataHi      = &PORTC;

static volatile UINT8 * const g_dirControlIn       = &DDRB;
static volatile UINT8 * const g_portInControlIn    = &PINB;
static volatile UINT8 * const g_portOutControlIn   = &PORTB;

static volatile UINT8 * const g_dirControlOutD     = &DDRD;
static volatile UINT8 * const g_portInControlOutD  = &PIND;
static volatile UINT8 * const g_portOutControlOutD = &PORTD;

static volatile UINT8 * const g_dirControlOutL     = &DDRL;
static volatile UINT8 * const g_portInControlOutL  = &PINL;
static volatile UINT8 * const g_portOutControlOutL = &PORTL;

//
// Byte-wide state controls
//
static const UINT8 s_PORT_BYTE_OFF    = 0x00;
static const UINT8 s_PORT_BYTE_PULLUP = 0xFF;

static const UINT8 s_DIR_BYTE_INPUT   = 0x00;
static const UINT8 s_DIR_BYTE_OUTPUT  = 0xFF;

//
// 4-bit-wise (Port B) input definitions.
//
static const UINT8 s_BIT_IN_CLK       = 0x01;
static const UINT8 s_BIT_IN_VPA       = 0x02;
static const UINT8 s_BIT_IN_DTACK     = 0x04;
static const UINT8 s_BIT_IN_BERR      = 0x08;

//
// 5-bit-wise (Port D) output definitions.
//
static const UINT8 s_BIT_OUT_AS       = 0x01;
static const UINT8 s_BIT_OUT_UDS      = 0x02;
static const UINT8 s_BIT_OUT_LDS      = 0x04;
static const UINT8 s_BIT_OUT_VMA      = 0x08;

static const UINT8 s_BIT_OUT_E        = 0x80;

static const UINT8 s_BYTE_OUT_IDLE_D  = s_BIT_OUT_AS | s_BIT_OUT_UDS | s_BIT_OUT_LDS | s_BIT_OUT_VMA;

//
// 8-bit-wise (Port L) output definitions.
//
static const UINT8 s_BIT_OUT_RW       = 0x01;
static const UINT8 s_BIT_OUT_FC0      = 0x02;
static const UINT8 s_BIT_OUT_FC1      = 0x04;
static const UINT8 s_BIT_OUT_FC2      = 0x08;

static const UINT8 s_BIT_OUT_LOE      = 0x10;
static const UINT8 s_BIT_OUT_LE0      = 0x20;
static const UINT8 s_BIT_OUT_LE1      = 0x40;
static const UINT8 s_BIT_OUT_LE2      = 0x80;

static const UINT8 s_BYTE_OUT_IDLE_L  = s_BIT_OUT_RW  | s_BIT_OUT_FC0 | s_BIT_OUT_LOE;


#endif
