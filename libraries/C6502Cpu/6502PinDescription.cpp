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
#include "6502PinDescription.h"


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
const CONNECTION s_GND1_i     = {  1, "GND1"     };
const CONNECTION s_RDY_i      = {  2, "RDY"      };
const CONNECTION s_CLK1o_o    = {  3, "CLK1o"    };
const CONNECTION s__IRQ_i     = {  4, "_IRQ"     };
const CONNECTION s__NMI_i     = {  6, "_NMI"     };
const CONNECTION s_SYNC_o     = {  7, "SYNC"     };
const CONNECTION s_Vcc_i      = {  8, "Vcc"      };
const CONNECTION s_GND2_i     = { 21, "GND2"     };
const CONNECTION s_R_W_o      = { 34, "R_W"      };
const CONNECTION s_CLK0i_i    = { 37, "CLK0i"    };
const CONNECTION s_SO_i       = { 38, "SO"       };
const CONNECTION s_CLK2o_o    = { 39, "CLK2o"    };
const CONNECTION s_RES_i      = { 40, "RES"      };


//
// Bus pins
//
const CONNECTION s_A_ot[16]   = { { 9, "A0"  },
                                   {10, "A1"  },
                                   {11, "A2"  },
                                   {12, "A3"  },
                                   {13, "A4"  },
                                   {14, "A5"  },
                                   {15, "A6"  },
                                   {16, "A7"  },
                                   {17, "A8"  },
                                   {18, "A9"  },
                                   {19, "A10" },
                                   {20, "A11" },
                                   {22, "A12" },
                                   {23, "A13" },
                                   {24, "A14" },
                                   {25, "A15" } }; // 16 bits

const CONNECTION s_D_iot[8] = { {33, "D0" },
                                {32, "D1" },
                                {31, "D2" },
                                {30, "D3" },
                                {29, "D4" },
                                {28, "D5" },
                                {27, "D6" },
                                {26, "D7" } }; // 8 bits.


