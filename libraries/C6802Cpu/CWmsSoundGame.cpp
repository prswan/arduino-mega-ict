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
#include "CWmsSoundGame.h"



//
// Video Sound ROM 1 (Defender)
// ROM Type: 2716/2516 x1
// Jumpers: W1, W2, W5, W7, W9, W10 & W15
//                                             01   02   04   08   10   20   40   80  100  200  400
static const UINT8 s_romData2nSet1_IC12[] = {0x0f,0x8e,0x7f,0x6f,0x6f,0x04,0x1b,0xcb,0x5a,0x97,0xce};
static const ROM_REGION s_romRegionSet1[] PROGMEM = {
    {NO_BANK_SWITCH, 0xf800, 0x0800, s_romData2nSet1_IC12, 0xfefd5b48, "U12"},
    {0} 
};


//
// Video Sound ROM 2 (Stargate)
// ROM Type: 2716/2516 x1
// Jumpers: W1, W2, W5, W7, W9, W10 & W15
//                                             01   02   04   08   10   20   40   80  100  200  400
static const UINT8 s_romData2nSet2_IC12[] = {0x0f,0x8e,0x7f,0x6f,0x6f,0x04,0x04,0x1b,0x5a,0x97,0x96};
static const ROM_REGION s_romRegionSet2[] PROGMEM = {
    {NO_BANK_SWITCH, 0xf800, 0x0800, s_romData2nSet2_IC12, 0x2fcf6c4d, "U12"},
    {0} 
};

//
// Video Sound ROM 3 (Robotron)
// ROM Type: 2532 x1
// Jumpers: W1, W3, W4, W5, W7, W10 & W15
//                                             01   02   04   08   10   20   40   80  100  200  400  800
static const UINT8 s_romData2nSet3_IC12[] = {0x28,0x43,0x31,0x20,0x53,0x7f,0x02,0x06,0xce,0x25,0x00,0x06};
static const ROM_REGION s_romRegionSet3[] PROGMEM = {
    {NO_BANK_SWITCH, 0xf000, 0x1000, s_romData2nSet3_IC12, 0xc56c1d28, "U12"},
    {0} 
};

//
// Video Sound ROM 4 (Joust)
// ROM Type: 2532 x1
// Jumpers: W1, W3, W4, W5, W7, W10 & W15
//                                               01   02   04   08   10   20   40   80   100  200  400  800
static const UINT8 s_romData2nSet4_IC12[]    = {0x0f,0x8e,0x7f,0x6f,0x6f,0x07,0x00,0xde,0xb7,0x97,0x16,0x24};
static const ROM_REGION s_romRegionSet4[] PROGMEM = {
    {NO_BANK_SWITCH, 0xf000, 0x1000, s_romData2nSet4_IC12, 0xf1835bdd, "U12"},
    {0}
};

//
// Video Sound ROM 5 (Bubbles)
// ROM Type: 2532 x1
// Jumpers: W1, W3, W4, W5, W7, W10 & W15
//                                               01   02   04   08   10   20   40   80   100  200  400  800
static const UINT8 s_romData2nSet5_IC12[]    = {0x0f,0x8e,0x7f,0x6f,0xc6,0x7f,0x01,0x7e,0x38,0x96,0xa6,0x00};
static const ROM_REGION s_romRegionSet5[] PROGMEM = {
    {NO_BANK_SWITCH, 0xf000, 0x1000, s_romData2nSet5_IC12, 0x689ce2aa, "U12"},
    {0}
};

//
// Video Sound ROM 9 (Sinistar Main and Speech)
// ROM Type: 2532 x5
// Jumpers: W3, W4, W5, W7, W10 & W15 (W1 fitted if speech board not present)
//                                             01   02   04   08   10   20   40   80  100  200  400  800
static const UINT8 s_romData2nSet9_IC12[] = {0x0f,0x8e,0x7f,0x6f,0xc6,0x7f,0xbd,0xe7,0x03,0x00,0x2a,0x06};
static const UINT8 s_romData2nSet9_IC04[] = {0x41,0x5d,0xb5,0x69,0x55,0xaf,0x02,0x94,0xaa,0xa8,0xbd,0x56};
static const UINT8 s_romData2nSet9_IC05[] = {0x00,0xfe,0xfd,0x01,0xff,0xf8,0x81,0x00,0x0f,0xea,0x03,0xa8};
static const UINT8 s_romData2nSet9_IC06[] = {0x07,0x89,0xbe,0xa2,0x67,0x5c,0xe0,0x38,0xdb,0x41,0xbf,0x2a};
static const UINT8 s_romData2nSet9_IC07[] = {0x00,0x00,0x00,0x00,0x00,0x42,0x03,0x87,0x00,0x00,0x00,0x00};
static const ROM_REGION s_romRegionSet9[] PROGMEM = {
    {NO_BANK_SWITCH, 0xf000, 0x1000, s_romData2nSet9_IC12, 0xb82f4ddb, "U12"},
    {NO_BANK_SWITCH, 0xe000, 0x1000, s_romData2nSet9_IC04, 0x4b56a626, "U04"},
    {NO_BANK_SWITCH, 0xc000, 0x1000, s_romData2nSet9_IC05, 0xcf3b5ffd, "U05"},
    {NO_BANK_SWITCH, 0xd000, 0x1000, s_romData2nSet9_IC06, 0xff8d2645, "U06"},
    {NO_BANK_SWITCH, 0xb000, 0x1000, s_romData2nSet9_IC07, 0xe1019568, "U07"},
    {0} 
};

//
// Video Sound ROM 10 (Sinistar Aux)
// ROM Type: 2532 x1
// Jumpers: W1, W3, W4, W5, W7, W10 & W15
//                                              01   02   04   08   10   20   40   80  100  200  400  800
static const UINT8 s_romData2nSet10_IC12[] = {0x0f,0x8e,0x7f,0x6f,0xc6,0x7f,0xbd,0xe7,0x81,0x04,0x97,0xf9};
static const ROM_REGION s_romRegionSet10[] PROGMEM = {
    {NO_BANK_SWITCH, 0xf000, 0x1000, s_romData2nSet10_IC12, 0xb5c70082, "U12"},
    {0} 
};

//
// Pinball Sound ROM 3 (Firepower)
// ROM Type: 2716/2516 x1 & 2532 x3
// Jumpers: W2, W5, W7, W9, W10 & W15 (W1 fitted if speech board not present)
//                                              01   02   04   08   10   20   40   80  100  200  400  800
static const UINT8 s_romData2nSetP3_IC12[] = {0x0f,0x8e,0x7f,0x6f,0xc6,0x4f,0x7e,0x1e,0x17,0x9b,0x26     };
static const UINT8 s_romData2nSetP3_IC05[] = {0xaa,0xae,0xaa,0x52,0x4a,0x05,0x02,0xb5,0xd4,0x28,0x2a,0x04};
static const UINT8 s_romData2nSetP3_IC06[] = {0x68,0xd1,0x1a,0xb1,0xc6,0xe5,0x19,0xb4,0x38,0x45,0x15,0x52};
static const UINT8 s_romData2nSetP3_IC07[] = {0xaa,0xaa,0xaa,0x4a,0x4a,0xa9,0xa5,0x5f,0xe0,0xca,0xf8,0x90};
static const ROM_REGION s_romRegionSetP3[] PROGMEM = {
    {NO_BANK_SWITCH, 0xf800, 0x0800, s_romData2nSetP3_IC12, 0x55a10d13, "U12"},
    {NO_BANK_SWITCH, 0xc000, 0x1000, s_romData2nSetP3_IC05, 0x1737fdd2, "U05"},
    {NO_BANK_SWITCH, 0xd000, 0x1000, s_romData2nSetP3_IC06, 0xe56f7aa2, "U06"},
    {NO_BANK_SWITCH, 0xb000, 0x1000, s_romData2nSetP3_IC07, 0x94c5c0a7, "U07"},
    {0} 
};

//
// Pinball Sound ROM ? (Defender)
// ROM Type: 2716/2516 x1
// Jumpers: W1, W2, W5, W7, W9, W10 & W15
//                                              01   02   04   08   10   20   40   80  100  200  400
static const UINT8 s_romData2nSetPD_IC12[] = {0x0f,0x8e,0x7f,0x6f,0x6f,0x00,0x17,0xcb,0x5a,0x97,0x96};
static const ROM_REGION s_romRegionSetPD[] PROGMEM = {
    {NO_BANK_SWITCH, 0xf800, 0x0800, s_romData2nSetPD_IC12, 0xcabaec58, "U12"},
    {0}
};

//
// Pinball Sound ROM ? (Hyperball)
// ROM Type: 2532 x1
// Jumpers: W1, W3, W4, W5, W7, W10 & W15
//                                              01   02   04   08   10   20   40   80  100  200  400  800
static const UINT8 s_romData2nSetPH_IC12[] = {0xce,0xf0,0x4f,0x26,0x00,0x03,0xd7,0xce,0x00,0x45,0xf4,0x00};
static const ROM_REGION s_romRegionSetPH[] PROGMEM = {
    {NO_BANK_SWITCH, 0xf000, 0x1000, s_romData2nSetPH_IC12, 0x06051e5e, "U12"},
    {0}
};

//
// Pinball Sound ROM ? (Varkon)
// ROM Type: 2716/2516 x1
// Jumpers: W1, W2, W5, W7, W9, W10 & W15
//                                              01   02   04   08   10   20   40   80  100  200  400
static const UINT8 s_romData2nSetPV_IC12[] = {0x0f,0x8e,0x7f,0x6f,0x6f,0x86,0x8d,0x0e,0x7e,0xca,0x8d};
static const ROM_REGION s_romRegionSetPV[] PROGMEM = {
    {NO_BANK_SWITCH, 0xf800, 0x0800, s_romData2nSetPV_IC12, 0xd13db2bb, "U12"},
    {0}
};


IGame*
CWmsSoundGame::createInstanceVideoSoundROM1(
)
{
    return (new CWmsSoundGame(s_romRegionSet1));
}

IGame*
CWmsSoundGame::createInstanceVideoSoundROM2(
)
{
    return (new CWmsSoundGame(s_romRegionSet2));
}

IGame*
CWmsSoundGame::createInstanceVideoSoundROM3(
)
{
    return (new CWmsSoundGame(s_romRegionSet3));
}

IGame*
CWmsSoundGame::createInstanceVideoSoundROM4(
)
{
    return (new CWmsSoundGame(s_romRegionSet4));
}

IGame*
CWmsSoundGame::createInstanceVideoSoundROM5(
)
{
    return (new CWmsSoundGame(s_romRegionSet5));
}

IGame*
CWmsSoundGame::createInstanceVideoSoundROM9(
)
{
    return (new CWmsSoundGame(s_romRegionSet9));
}

IGame*
CWmsSoundGame::createInstanceVideoSoundROM10(
)
{
    return (new CWmsSoundGame(s_romRegionSet10));
}

IGame*
CWmsSoundGame::createInstancePinballSoundROM3(
)
{
    return (new CWmsSoundGame(s_romRegionSetP3));
}

IGame*
CWmsSoundGame::createInstancePinballSoundROMD(
)
{
    return (new CWmsSoundGame(s_romRegionSetPD));
}

IGame*
CWmsSoundGame::createInstancePinballSoundROMH(
)
{
    return (new CWmsSoundGame(s_romRegionSetPH));
}

IGame*
CWmsSoundGame::createInstancePinballSoundROMV(
)
{
    return (new CWmsSoundGame(s_romRegionSetPV));
}

CWmsSoundGame::CWmsSoundGame(
    const ROM_REGION *romRegion
) : CWmsSoundBaseGame( romRegion )
{
}

