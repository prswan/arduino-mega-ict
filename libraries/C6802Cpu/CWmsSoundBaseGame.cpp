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
#include "CWmsSoundBaseGame.h"
#include "C6802Cpu.h"
#include <DFR_Key.h>


//
// Probe Head GND:
//   6802 GND Pin 1 and Pin 21
//
// Watchdog Disable:
//   This board does not have a watchdog circuit.
//
// 6802 Compatibility Notes
//   * When RE is enabled removing W14 the 6802's internal RAM is enabled.  This cannot be used with a 6808.
//


//
// PIA Bank Start Addresses
//
static const UINT32 addressPIA1A  = 0x0400;
static const UINT32 addressPIA1B  = 0x0402;


//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { 
    //                                     "012", "012345"
    {NO_BANK_SWITCH, 0x0000, 0x007F, 0xFF, "U11", "RAM   "}, // 6810 RAM
    {0}
}; // end of list


//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} };


//
// Input region - IC10 6821 PIA Inputs from ROM board (PB0-7)
//
// Sound board Inputs on 0x4000
//       4J3-1 - Key                                         0x01  PB0 - 4J3-3
// 0x02  4J3-2 - PB1                                         0x02  PB1 - 4J3-2
// 0x01  4J3-3 - PB0                                         0x04  PB2 - 4J3-5
// 0x08  4J3-4 - PB3                                         0x08  PB3 - 4J3-4
// 0x04  4J3-5 - PB2                                         0x10  PB4 - 4J3-7
// 0x20  4J3-6 - PB5 (W4 Installed & W9 Removed)             0x20  PB5 - 4J3-6 (W4) / DSW-2 (W9)
// 0x10  4J3-7 - PB4                                         0x40  PB6 - DSW-1
// 0x80  4J3-8 - PB7 (W12 Installed)                         0x80  PB7 - 4J3-8 (W12)
//       4J3-9 - No Connection
// 
// 0x40  DSW-1 - PB6
// 0x20  DSW-2 - PB5 (W4 Removed & W9 Installed)
//
// CWmsSoundBaseGame::onBankSwitchSetupPIA1B initialises Bank B of 6821 U10 to be inputs
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { 
    //                                                              "012", "012345"
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1B, addressPIA1B, 0xFF, "U10", "PB0-7 "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1B, addressPIA1B, 0x01, "U10", "PB0   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1B, addressPIA1B, 0x02, "U10", "PB1   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1B, addressPIA1B, 0x04, "U10", "PB2   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1B, addressPIA1B, 0x08, "U10", "PB3   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1B, addressPIA1B, 0x10, "U10", "PB4   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1B, addressPIA1B, 0x20, "U10", "PB5   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1B, addressPIA1B, 0x40, "U10", "PB6   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1B, addressPIA1B, 0x80, "U10", "PB7   "},
    {0}
}; // end of list


//
// Output region - IC10 6821 PIA Outputs to DAC (PA0-7) and Speech board (CA2 & CB2)
//
// CWmsSoundBaseGame::onBankSwitchSetupPIA1A initialises Bank A of 6821 U10 to be outputs
// Better output tests are implememted in the custom functions "Test DAC" and "Test CVSDM"
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { 
    //                                                                       "012", "012345"
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1A, addressPIA1A,   0xFF, 0x00,  "U10", "PA0-7 "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1A, addressPIA1A,   0x01, 0x00,  "U10", "PA0   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1A, addressPIA1A,   0x02, 0x00,  "U10", "PA1   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1A, addressPIA1A,   0x04, 0x00,  "U10", "PA2   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1A, addressPIA1A,   0x08, 0x00,  "U10", "PA3   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1A, addressPIA1A,   0x10, 0x00,  "U10", "PA4   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1A, addressPIA1A,   0x20, 0x00,  "U10", "PA5   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1A, addressPIA1A,   0x40, 0x00,  "U10", "PA6   "},
    {CWmsSoundBaseGame::onBankSwitchSetupPIA1A, addressPIA1A,   0x80, 0x00,  "U10", "PA7   "},
    {NO_BANK_SWITCH,                            addressPIA1A+1, 0x38, 0x30,  "U10", "CA2   "},
    {NO_BANK_SWITCH,                            addressPIA1B+1, 0x38, 0x30,  "U10", "CB2   "},
    {0}
}; // end of list


//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { 
    //                                  "0123456789"
    {CWmsSoundBaseGame::soundTestDAC,   "Test DAC  "},
    {CWmsSoundBaseGame::soundTestCVSDM, "Test CVSDM"},
    {0}
};


CWmsSoundBaseGame::CWmsSoundBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new C6802Cpu();
    m_cpu->idle();

    // The sound request interrupt is on /IRQ pin 6
    m_interrupt = ICpu::INT;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;
}


CWmsSoundBaseGame::~CWmsSoundBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// This is used to setup the hardware configuration Port A of the 6821 PIA U10 for output
//
PERROR
CWmsSoundBaseGame::onBankSwitchSetupPIA1A(
    void *cWmsSoundBaseGame
)
{
    PERROR            error     = errorSuccess;
    CWmsSoundBaseGame *thisGame = (CWmsSoundBaseGame *) cWmsSoundBaseGame;
    ICpu              *cpu      = thisGame->m_cpu;

    // Set DDR (xxxxx0xx)
    error = cpu->memoryWrite(addressPIA1A+1, 0x00);     

    if (SUCCESS(error))
    {
        // Set PA0-PA7 as output pins
        error = cpu->memoryWrite(addressPIA1A, 0xFF); 
    }

    if (SUCCESS(error))
    {
        // Disable CA1 (xxxxxx00), Set PR (xxxxx1xx), Set CA2 speech data output low (xx110xxx)
        error = cpu->memoryWrite(addressPIA1A+1, 0x34);    
    }

    if (SUCCESS(error))
    {
        // Set PA0-PA7 outputs low
        error = cpu->memoryWrite(addressPIA1A, 0x00); 
    }

    return error;
}


//
// This is used to setup the hardware configuration Port B of the 6821 PIA U10 for input
//
PERROR
CWmsSoundBaseGame::onBankSwitchSetupPIA1B(
    void *cWmsSoundBaseGame
)
{
    PERROR            error     = errorSuccess;
    CWmsSoundBaseGame *thisGame = (CWmsSoundBaseGame *) cWmsSoundBaseGame;
    ICpu              *cpu      = thisGame->m_cpu;

    // Set DDR (xxxxx0xx)
    error = cpu->memoryWrite(addressPIA1B+1, 0x00);     

    if (SUCCESS(error))
    {
        // Set PB0-PB7 as input pins
        error = cpu->memoryWrite(addressPIA1B, 0x00); 
    }

    if (SUCCESS(error))
    {
        // Enable CB1 low > high IRQ (xxxxxx11), Set PR (xxxxx1xx), Set CB2 speech clock output low (xx110xxx)
        error = cpu->memoryWrite(addressPIA1B+1, 0x37); 
    }

    return error;
}


//
// This is used to test /IRQ: 6802 CPU U9 Pin 4 is pulled low by 6821 PIA U10 /IRQA Pin 37 or /IRQB Pin 38  
// This is triggered by 6821 PIA U10 CB1 Pin 18 being pulled high when a sound input is triggered through 4068 U6
//
// To run the test initiate it and then ground one of the sound input lines on 4J3 pins 2-5 or 7 before the 5 second timer expires
// (4J3 Pin 6 may not be connected depending on jumpers, e.g. Defender/Stargate)
//
PERROR
CWmsSoundBaseGame::interruptCheck(
)
{
    PERROR error = errorSuccess;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "/IRQ Tested!";

    // Set the CR to ensure we are set up for IRQ
    m_cpu->memoryWrite(addressPIA1B+1, 0x37);

    // Deselect PIA and read DR to ensure /IRQ is not set
    m_cpu->memoryRead(0xF800, 0x00);
    m_cpu->memoryRead(addressPIA1B+1, 0x00);

    error = m_cpu->waitForInterrupt(m_interrupt, 5000);

    if (SUCCESS(error)) 
    {
        // Deselect PIA and read PR to release /IRQ
        m_cpu->memoryRead(0xF800, 0x00);
        m_cpu->memoryRead(addressPIA1B, 0x00);

        error = m_cpu->waitForInterrupt(m_interrupt, 0);

        if (SUCCESS(error))
        {
            error = errorUnexpected;
        }
        else
        {
            error = errorCustom;
        }
    }

    return error;
}

//
// Custom function for testing the 6821 PIA U10 Bank A outputs to the MC1408 DAC U13 and the analog sound hardware
//
PERROR
CWmsSoundBaseGame::soundTestDAC(
    void *context
)
{
    PERROR            error     = errorSuccess;
    CWmsSoundBaseGame *thisGame = (CWmsSoundBaseGame *) context;
    ICpu              *cpu      = thisGame->m_cpu;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "DAC Tested!";

    // Firstly initialse PIA1A in case the output test hasn't been run

    // Set DDR (xxxxx0xx)
    error = cpu->memoryWrite(addressPIA1A+1, 0x00);     

    if (SUCCESS(error))
    {
        // Set PA0-PA7 as output pins
        error = cpu->memoryWrite(addressPIA1A, 0xFF); 
    }

    if (SUCCESS(error))
    {
        // Disable CA1 (xxxxxx00), Set PR (xxxxx1xx), Set CA2 output low (xx110xxx)
        error = cpu->memoryWrite(addressPIA1A+1, 0x34);    
    }

    if (SUCCESS(error))
    {
        // Set PA0-PA7 outputs low
        error = cpu->memoryWrite(addressPIA1A, 0x00); 
    }

    if (SUCCESS(error))
    {
        //
        // DAC waveform parameters
        //
        //   step         {   0,   1,   2,   3,   4,   5,}
        byte loops[]    = {  40,  25,  50, 100,  50, 100, }; // how many times the waveform is repeated      max=255
        byte delay[]    = {   0,  15,  30,   0,  25,   0, }; // how long each data value is left on the DAC  max=255
        byte waveform[] = {   2,   2,   0,   0,   1,   1, }; // waveform shape 0=Sine 1=Sawtooth 2=Square
        byte sine[] = { 0x80,0xa0,0xbf,0xda,0xee,0xfb,0xff,0xfb,0xee,0xda,0xbf,0xa0,0x80,0x5f,0x40,0x25,0x11,0x4,0x0,0x4,0x11,0x25,0x40,0x5f,0x80, }; // 24 Points - any more is just too slow

        for (int step = 0; step < sizeof(loops); step++)
        {
            for (int loop = 0; loop < loops[step]; loop++)
            {
                for (int point = 0; point < sizeof(sine); point++)
                {
                    switch (waveform[step]) {
                        case 0:
                            // Sine Wave
                            cpu->memoryWrite(addressPIA1A, sine[point]);
                            delayMicroseconds(delay[step]);
                            break;
                        case 1:
                            // Sawtooth Wave
                            cpu->memoryWrite(addressPIA1A, int (256/sizeof(sine)) * point);
                            delayMicroseconds(delay[step]);
                            break;
                        case 2:
                            // Square Wave
                            cpu->memoryWrite(addressPIA1A, 0);
                            delayMicroseconds(delay[step]);
                            cpu->memoryWrite(addressPIA1A, 255);
                            delayMicroseconds(delay[step]);
                            break;
                    }
                }
            }
        }

        error = errorCustom;
    }

    return error;
}

//
// Custom function for testing the optional speech board 55516 CVSDM Chip U1
// The CVSDM chip has two inputs:
// Speech data is fed from 6821 PAI U10 Pin 39 CA2
// Speech clock is fed from 6821 PAI U10 Pin 19 CB2
//
// On each speech clock pulse the CVSDM chip increases the slope if speech data is high and decrease it if low
//
PERROR
CWmsSoundBaseGame::soundTestCVSDM(
    void *context
)
{
    PERROR            error     = errorSuccess;
    CWmsSoundBaseGame *thisGame = (CWmsSoundBaseGame *) context;
    ICpu              *cpu      = thisGame->m_cpu;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "CVSDM Tested!";

    // Disable CA1 (xxxxxx00), Set PR (xxxxx1xx), Set CA2 speech data output low (xx110xxx) 
    error = cpu->memoryWrite(addressPIA1A+1, 0x34);

    if (SUCCESS(error))
    {
        // Enable CB1 low > high IRQ (xxxxxx11), Set PR (xxxxx1xx), Set CB2 speech clock output low (xx110xxx)
        error = cpu->memoryWrite(addressPIA1B+1, 0x37);
    }  

    if (SUCCESS(error))
    {
        //
        // CVSDM waveform parameters
        //
        byte change[]  = { 10, 11, 10, 13, 15, }; // number of clocks before change of data state min~10 max~25
        byte direction = 0;                       // counter for set slope increase/decrease
        int loops      = 1000;                    // how many times the wave is repeated

        for (int step = 0; step < sizeof(change); step++)
        {
            for (int loop = 0; loop < loops; loop++)
            {
                // CB2 speech clock low
                cpu->memoryWrite(addressPIA1B+1, 0x3F); 

                // CA2 speech data high
                if (direction == 0) cpu->memoryWrite(addressPIA1A+1, 0x3C);   

                // CA2 speech data low       
                if (direction == change[step]) cpu->memoryWrite(addressPIA1A+1, 0x34);                   
                
                // CB2 speech clock high
                cpu->memoryWrite(addressPIA1B+1, 0x37);

                direction++;
                if (direction == change[step] * 2) direction = 0;
            }
        }
        error = errorCustom;
    }

    return error;
}