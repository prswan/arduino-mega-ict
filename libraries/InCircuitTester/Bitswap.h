//
// Copyright (c) 2019, Nicola Salmoria, Aaron Giles, Paul R. Swan.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the <organization> nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#ifndef Bitswap_h
#define Bitswap_h

#include "Types.h"

//
// This extract is derived from src\emu\emucore.h thus
//
// license:BSD-3-Clause
// copyright-holders:Nicola Salmoria, Aaron Giles
//


#define INBITSWAP8(val,B7,B6,B5,B4,B3,B2,B1,B0) \
		(((((val) >> (B7)) & 1) << 7) | \
		 ((((val) >> (B6)) & 1) << 6) | \
		 ((((val) >> (B5)) & 1) << 5) | \
		 ((((val) >> (B4)) & 1) << 4) | \
		 ((((val) >> (B3)) & 1) << 3) | \
		 ((((val) >> (B2)) & 1) << 2) | \
		 ((((val) >> (B1)) & 1) << 1) | \
		 ((((val) >> (B0)) & 1) << 0))

#define INBITSWAP16(val,B15,B14,B13,B12,B11,B10,B9,B8,B7,B6,B5,B4,B3,B2,B1,B0) \
		(((((val) >> (B15)) & 1) << 15) | \
		 ((((val) >> (B14)) & 1) << 14) | \
		 ((((val) >> (B13)) & 1) << 13) | \
		 ((((val) >> (B12)) & 1) << 12) | \
		 ((((val) >> (B11)) & 1) << 11) | \
		 ((((val) >> (B10)) & 1) << 10) | \
		 ((((val) >> ( B9)) & 1) <<  9) | \
		 ((((val) >> ( B8)) & 1) <<  8) | \
		 ((((val) >> ( B7)) & 1) <<  7) | \
		 ((((val) >> ( B6)) & 1) <<  6) | \
		 ((((val) >> ( B5)) & 1) <<  5) | \
		 ((((val) >> ( B4)) & 1) <<  4) | \
		 ((((val) >> ( B3)) & 1) <<  3) | \
		 ((((val) >> ( B2)) & 1) <<  2) | \
		 ((((val) >> ( B1)) & 1) <<  1) | \
		 ((((val) >> ( B0)) & 1) <<  0))

#define OUTBITSWAP16(val,B15,B14,B13,B12,B11,B10,B9,B8,B7,B6,B5,B4,B3,B2,B1,B0) \
		(((((val) >> (15)) & 1) << B15) | \
		 ((((val) >> (14)) & 1) << B14) | \
		 ((((val) >> (13)) & 1) << B13) | \
		 ((((val) >> (12)) & 1) << B12) | \
		 ((((val) >> (11)) & 1) << B11) | \
		 ((((val) >> (10)) & 1) << B10) | \
		 ((((val) >> ( 9)) & 1) << B9) | \
		 ((((val) >> ( 8)) & 1) << B8) | \
		 ((((val) >> ( 7)) & 1) << B7) | \
		 ((((val) >> ( 6)) & 1) << B6) | \
		 ((((val) >> ( 5)) & 1) << B5) | \
		 ((((val) >> ( 4)) & 1) << B4) | \
		 ((((val) >> ( 3)) & 1) << B3) | \
		 ((((val) >> ( 2)) & 1) << B2) | \
		 ((((val) >> ( 1)) & 1) << B1) | \
		 ((((val) >> ( 0)) & 1) << B0))

#endif
