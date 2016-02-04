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
#ifndef CWmsSoundGame_h
#define CWmsSoundGame_h

#include "CWmsSoundBaseGame.h"

class CWmsSoundGame : public CWmsSoundBaseGame
{
    public:

        //
        // Constructors for this game.
        //

        static IGame* createInstanceVideoSoundROM1(     // Defender
        );

        static IGame* createInstanceVideoSoundROM2(     //Stargate
        );

        static IGame* createInstanceVideoSoundROM3(     // Robotron
        );

        static IGame* createInstanceVideoSoundROM4(     // Joust
        );

        static IGame* createInstanceVideoSoundROM5(     // Bubbles
        );

        static IGame* createInstanceVideoSoundROM9(     // Sinsiar main and speech
        );

        static IGame* createInstanceVideoSoundROM10(    // Sinsiar aux (no Speech)
        );

        static IGame* createInstancePinballSoundROM3(   // Firepower Pinball
        );

        static IGame* createInstancePinballSoundROMD(   // Defender Pinball
        );

        static IGame* createInstancePinballSoundROMH(   // Hyperball
        );

        static IGame* createInstancePinballSoundROMV(   // Varkon Pinball
        );

        //
        // IGame Interface - wholly implemented in the Base game.
        //

    private:

        //
        // Different ROM sets supplied.
        //
        CWmsSoundGame(
            const ROM_REGION *romRegion
        );

};

#endif


