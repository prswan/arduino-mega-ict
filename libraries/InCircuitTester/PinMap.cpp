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
const UINT8 g_pinMap40DIL[] = { 0xFF,   //  0 - Not used
                                  14,   //  1
                                  16,   //  2
                                  18,   //  3
                                  20,   //  4
                                  22,   //  5
                                  24,   //  6
                                  26,   //  7
                                  28,   //  8
                                  30,   //  9
                                  32,   // 10
                                  34,   // 11
                                  36,   // 12
                                  38,   // 13
                                  40,   // 14
                                  42,   // 15
                                  44,   // 16
                                  46,   // 17
                                  48,   // 18
                                  50,   // 19
                                  52,   // 20 - Native cable
                                  53,   // 21 - Native cable
//                                  53,   // 20 - Swapped for 1802 pod cable
//                                  52,   // 21 - Swapped for 1802 pod cable
                                  51,   // 22
                                  49,   // 23
                                  47,   // 24
                                  45,   // 25
                                  43,   // 26
                                  41,   // 27
                                  39,   // 28
                                  37,   // 29
                                  35,   // 30
                                  33,   // 31
                                  31,   // 32
                                  29,   // 33
                                  27,   // 34
                                  25,   // 35
                                  23,   // 36
                                  21,   // 37
                                  19,   // 38
                                  17,   // 39
                                  15    // 40
                              };

/*
 * Shield Prototype pinout
 *
 * 40 - 1
 * ..   ..
 * 21 - 20 order
 *
const UINT8 g_pinMap40DIL[] = { 0xFF,   //  0 - Not used
                                  15,   //  1
                                  17,   //  2
                                  19,   //  3
                                  21,   //  4
                                  23,   //  5
                                  25,   //  6
                                  27,   //  7
                                  29,   //  8
                                  31,   //  9
                                  33,   // 10
                                  35,   // 11
                                  37,   // 12
                                  39,   // 13
                                  41,   // 14
                                  43,   // 15
                                  45,   // 16
                                  47,   // 17
                                  49,   // 18
                                  51,   // 19
//                                  53,   // 20
//                                  54,   // 21
                                  52,   // 20 - Swapped for 1802 pod cable
                                  53,   // 21 - Swapped for 1802 pod cable
                                  50,   // 22
                                  48,   // 23
                                  46,   // 24
                                  44,   // 25
                                  42,   // 26
                                  40,   // 27
                                  38,   // 28
                                  36,   // 29
                                  34,   // 30
                                  32,   // 31
                                  30,   // 32
                                  28,   // 33
                                  26,   // 34
                                  24,   // 35
                                  22,   // 36
                                  20,   // 37
                                  18,   // 38
                                  16,   // 39
                                  14    // 40
                                 };
*/

/*
 *  1 - 40
 * ..   ..
 * 20 - 21 order
 *
const UINT8 g_pinMap40DIL[] = { 0xFF,   //  0 - Not used
                                  14,   //  1
                                  16,   //  2
                                  18,   //  3
                                  20,   //  4
                                  22,   //  5
                                  24,   //  6
                                  26,   //  7
                                  28,   //  8
                                  30,   //  9
                                  32,   // 10
                                  34,   // 11
                                  36,   // 12
                                  38,   // 13
                                  40,   // 14
                                  42,   // 15
                                  44,   // 16
                                  46,   // 17
                                  48,   // 18
                                  50,   // 19
                                  52,   // 20
                                  53,   // 21
                                  51,   // 22
                                  49,   // 23
                                  47,   // 24
                                  45,   // 25
                                  43,   // 26
                                  41,   // 27
                                  39,   // 28
                                  37,   // 29
                                  35,   // 30
                                  33,   // 31
                                  31,   // 32
                                  29,   // 33
                                  27,   // 34
                                  25,   // 35
                                  23,   // 36
                                  21,   // 37
                                  19,   // 38
                                  17,   // 39
                                  15    // 40
                                 };
*/
