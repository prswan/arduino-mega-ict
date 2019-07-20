//
// Copyright (c) 2019, Paul R. Swan
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
#include "C6809EPinOut.h"


//
// Initialize the native 6809E pinout
//
const C6809EPinOut s_6809EPinOut =
{
    {  1, "GND"      },
    {  2, "_NMI"     },
    {  3, "_IRQ"     },
    {  4, "_FIRQ"    },
    {  5, "BS"       },
    {  6, "BA"       },
    {  7, "Vcc"      },
    { 32, "RW"       },
    { 33, "BUSY"     },
    { 34, "E"        },
    { 35, "Q"        },
    { 36, "AVMA"     },
    { 37, "_RESET"   },
    { 38, "LIC"      },
    { 39, "TSC"      },
    { 40, "_HALT"    },
    {  // Address Bus
        { 8, "A0"  },
        { 9, "A1"  },
        {10, "A2"  },
        {11, "A3"  },
        {12, "A4"  },
        {13, "A5"  },
        {14, "A6"  },
        {15, "A7"  },
        {16, "A8"  },
        {17, "A9"  },
        {18, "A10" },
        {19, "A11" },
        {20, "A12" },
        {21, "A13" },
        {22, "A14" },
        {23, "A15" }
    }, // 16 bits
    {  // Data Bus
        {31, "D0" },
        {30, "D1" },
        {29, "D2" },
        {28, "D3" },
        {27, "D4" },
        {26, "D5" },
        {25, "D6" },
        {24, "D7" }
    }  // 8 bits.
};


//
// Initialize the custom Konami-1 pinout
//
// Konami-1 is a 42-pin package but the tester has only 40-pins
// so the bottom two pins (21-HALT and 22-GND) are left unconnected
// and are not needed for most testing.
//
// For ICT GND, use pin 20-TSC that is connected to ground on all
// schematics that I have been seen so far.
//
// The pin numbering here is expressed in a way that compensates for
// pin number shift w.r.t. 40 pins (i.e. all pins > 20 subtract 2.
//
// Unnaccounted for pins:
// 11, 14, 16, 18, 26, 27, 28
//
// NOTES:
//  There is a scehmatic that has:-
//   Pin 11 - ~ROM   - Input - from the address decode that's also driving ROM OE.
//   Pin 14 - ~Z09EN - Input - from a 2nd Z80 CPU address decode
//   Pin 16 - 220    - Unknown
//   Pin 18 - GND    - Input - Unknown
//   Pin 27 - GND    - Input - Unknown
//
const C6809EPinOut s_Konami1PinOut =
{
    { 0  , "GND"      }, // Pin 22 not connected on ICT
    {23-2, "_NMI "    },
    {24-2, "_IRQ"     },
    {25-2, "_FIRQ"    },
    { 0  , "BS"       },
    {28-2, "BA"       }, // ? estimated, there is one schematic that uses this to gate ROM OE
    {29-2, "Vcc"      },
    {13  , "RW"       },
    { 0  , "BUSY"     },
    {15  , "E"        }, //
    {17  , "Q"        }, //
    { 0  , "AVMA"     },
    {19  , "_RESET"   }, //
    { 0  , "LIC"      },
    {20  , "TSC"      }, // One schematic shows pin 22 on the bottom hinting this is likely TSC
    { 0  , "_HALT"    }, // Pin 21 not connected on ICT
    {  // Address Bus
        {30-2, "A0"  }, //
        {31-2, "A1"  }, //
        {32-2, "A2"  }, //
        {33-2, "A3"  }, //
        {34-2, "A4"  }, //
        {35-2, "A5"  }, //
        {36-2, "A6"  }, //
        {37-2, "A7"  }, //
        {38-2, "A8"  }, //
        {39-2, "A9"  }, //
        {40-2, "A10" }, //
        {41-2, "A11" }, //
        {42-2, "A12" }, //
        { 1  , "A13" }, //
        { 2  , "A14" }, //
        { 3  , "A15" }  //
    }, // 16 bits
    {  // Data Bus
        {12  , "D0" }, //
        {10  , "D1" }, //
        { 9  , "D2" }, //
        { 8  , "D3" }, //
        { 7  , "D4" }, //
        { 6  , "D5" }, //
        { 5  , "D6" }, //
        { 4  , "D7" }
    }  // 8 bits.
};

