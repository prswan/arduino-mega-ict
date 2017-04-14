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
#ifndef PinDescription_h
#define PinDescription_h

#include "Arduino.h"
#include "Error.h"
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
extern const CONNECTION s_GND1_i   ;
extern const CONNECTION s_RDY_i    ;
extern const CONNECTION s_CLK1o_o  ;
extern const CONNECTION s__IRQ_i   ;
extern const CONNECTION s__NMI_i   ;
extern const CONNECTION s_SYNC_o   ;
extern const CONNECTION s_Vcc_i    ;
extern const CONNECTION s_GND2_i   ;
extern const CONNECTION s_R_W_o    ;
extern const CONNECTION s_CLK0i_i  ;
extern const CONNECTION s_SO_i     ;
extern const CONNECTION s_CLK2o_o  ;
extern const CONNECTION s_RES_i    ;
extern const CONNECTION s_A_ot[16] ;
extern const CONNECTION s_D_iot[8] ;


#endif
