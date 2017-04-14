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
#include "PinMap.h"

/*
 * Production Shield V1.00
 *  1 - 40
 * ..   ..
 * 20 - 21 order
 *
*/
const UINT8 g_pinMap40DIL[] = { 0xFF,   //        0 - Not used
                                  14,   // PJ1 -  1
                                  16,   // PH1 -  2
                                  18,   // PD3 -  3
                                  20,   // PD1 -  4
                                  22,   // PA0 -  5
                                  24,   // PA2 -  6
                                  26,   // PA4 -  7
                                  28,   // PA6 -  8
                                  30,   // PC7 -  9
                                  32,   // PC5 - 10
                                  34,   // PC3 - 11
                                  36,   // PC1 - 12
                                  38,   // PD7 - 13
                                  40,   // PG1 - 14
                                  42,   // PL7 - 15
                                  44,   // PL5 - 16
                                  46,   // PL3 - 17
                                  48,   // PL1 - 18
                                  50,   // PB3 - 19
                                  52,   // PB1 - 20
                                  53,   // PB0 - 21
                                  51,   // PB2 - 22
                                  49,   // PL0 - 23
                                  47,   // PL2 - 24
                                  45,   // PL4 - 25
                                  43,   // PL6 - 26
                                  41,   // PG0 - 27
                                  39,   // PG2 - 28
                                  37,   // PC0 - 29
                                  35,   // PC2 - 30
                                  33,   // PC4 - 31
                                  31,   // PC6 - 32
                                  29,   // PA7 - 33
                                  27,   // PA5 - 34
                                  25,   // PA3 - 35
                                  23,   // PA1 - 36
                                  21,   // PD0 - 37
                                  19,   // PD2 - 38
                                  17,   // PH0 - 39
                                  15    // PJ0 - 40
                              };

/*
 *  Port to DIL40 Pin Number Mapping Table
 *
 *           7    6    5    4    3    2    1    0
 *          15   14   13   12   11   10    9    8
 *   ----------------------------------------------
 *   Port | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 |
 *   ----------------------------------------------
 *   PA   | 33 |  8 | 34 |  7 | 35 |  6 | 36 |  5 |
 *   PB   |    |    |    |    | 19 | 22 | 20 | 21 |
 *   PC   |  9 | 32 | 10 | 31 | 11 | 30 | 12 | 29 |
 *   PD   | 13 |    |    |    |  3 | 38 |  4 | 37 |
 *   PE   |    |    |    |    |    |    |    |    |
 *   PF   |    |    |    |    |    |    |    |    |
 *   PG   |    |    |    |    |    | 28 | 14 | 27 |
 *   PH   |    |    |    |    |    |    |  2 | 39 |
 *   PI   |    |    |    |    |    |    |    |    |
 *   PJ   |    |    |    |    |    |    |  1 | 40 |
 *   PK   |    |    |    |    |    |    |    |    |
 *   PL   | 15 | 26 | 16 | 25 | 17 | 24 | 18 | 23 |
 *   ----------------------------------------------
*/


/*
 * Production Shield V1.00
 *  J14, 1 - 8
 *
*/
const UINT8 g_pinMap8Aux[] = { 0xFF,   //   0 - Not used
                               54+ 8,  // PK0 - A8
                               54+ 9,  // PK1 - A9
                               54+10,  // PK2 - A10
                               54+11,  // PK3 - A11
                               54+12,  // PK4 - A12
                               54+13,  // PK5 - A13
                               54+14,  // PK6 - A14
                               54+15   // PK7 - A15
                             };


