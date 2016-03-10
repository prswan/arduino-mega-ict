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
#include "CDefenderBaseGame.h"
#include "C6809ECpu.h"
#include <DFR_Key.h>
#include <LiquidCrystal.h>

static LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


//
// Probe Head GND:
// 6809E GND Pin 1
//
// Watchdog:
// Watchdog is cleared by the program writing 0x38 or 0x39 to address 0xc3ff
// On early series boards watchdog is disabled by cutting the jumper pad between 1D and 1F on the solder side
// On later series boards watchdog is disabled by cutting the jumper pad between 5M and 5O on the component side 
//


//
// PIA Addresses
//
static const UINT32 addressPIA1A  = 0xcc00;             // ROM Board PIA Port A 
static const UINT32 addressPIA1B  = addressPIA1A+2;     // ROM Board PIA Port B

static const UINT32 addressPIA2A  = 0xcc04;             // Interface Board PIA Port A
static const UINT32 addressPIA2B  = addressPIA2A+2;     // Interface Board PIA Port B


//
// RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { 
    //                                                          "012", "012345"
    {NO_BANK_SWITCH,                      0x0000, 0xbfff, 0xff, "RAM", "DRAM  "}, // 4116 DRAM x24
    {CDefenderBaseGame::onROMBankSelect0, 0xc400, 0x0100, 0x0f, "1E ", "CMOS  "}, // 5101 CMOS RAM bits 0-3 only
    {0}
}; // end of list


//
// Write only RAM region is the same for all versions.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = {
    //                                                          "012", "012345"
    {CDefenderBaseGame::onROMBankSelect0, 0xc000, 0x0010, 0xff, "12C", "Colour"}, // 2x 7489 Colour RAM
    {0} 
}; // end of list


//
// Input region:
// ROM Board U1 6821 PIA Inputs PA0-5 and CA2
// Interface board U1 6821 PIA Inputs PA0-7 and PB0-7
//
//
// ROM board inputs
// 
// 0x01 PA0 2J3-1  - Auto/Manual
// 0x02 PA1 2J3-2  - Advance
// 0x04 PA2 2J3-3  - Right Coin
// 0x08 PA3 2J3-4  - High Score Reset
// 0x10 PA4 2J3-5  - Left Coin
// 0x20 PA5 2J3-6  - Center Coin
//      CA2 2J3-7  - Slam Tilt (Interrupt)
//          2J3-8  - No Connection
//          2J3-9  - Key 
//          2J3-10 - Ground
//
//
// Early series interface board inputs
// Inputs PA0-7 and PB0-7 are connected however PB1-7 are not used by the game
// IRQA, IRQB, CA1, CA2, CB1 and CB2 are not used
//
// 0x01 PA0 3J3-10 - Fire                           0x01 PB0 3J3-10 - Up                 
// 0x02 PA1 3J3-9  - Thrust                         0x02 PB1 3J3-9  - Not Used 
// 0x04 PA2 3J3-8  - Smart Bomb                     0x04 PB2 3J3-8  - Not Used 
// 0x08 PA3 3J3-7  - Hyperspace                     0x08 PB3 3J3-7  - Not Used 
// 0x10 PA4 3J3-6  - 2 Player Start                 0x10 PB4 3J3-6  - Not Used 
// 0x20 PA5 3J3-5  - 1 Player Start                          3J3-5  - Key
// 0x40 PA6 3J3-4  - Reverse                        0x20 PB5 3J3-4  - Not Used 
// 0x80 PA7 3J3-3  - Down                           0x40 PB6 3J3-3  - Not Used 
//          3J3-2  - Key                            0x80 PB7 3J3-2  - Not Used 
//          3J3-1  - Ground                                  3J3-1  - Ground
//
//
// Later series interface board inputs
// Only inputs PA0-7, PB0 and PB7 are connected
// IRQA, IRQB, PB1-6, CA1, CA2, and CB1 are not used
//
// Output CB2 is used to determine which set of player controls is used in cocktail mode high=Player 1 low=Player 2
// When using ROM colours other than red W2 should be removed as the state of PB2 cannot be assured
//
// 0x01 PA0 3J3-1  - Fire Player 1                  0x01 PB0 3J3-1  - Up                 
// 0x02 PA1 3J3-2  - Thrust Player 1                0x01 PA0 3J3-2  - Fire Player 2 
// 0x04 PA2 3J3-3  - Smart Bomb Player 1            0x02 PA1 3J3-3  - Thrust Player 2  
// 0x08 PA3 3J3-4  - Hyperspace Player 1            0x04 PA2 3J3-4  - Smart Bomb Player 2  
// 0x10 PA4 3J3-5  - 2 Player Start                 0x08 PA3 3J3-5  - Hyperspace Player 2  
// 0x20 PA5 3J3-6  - 1 Player Start                          3J3-6  - Key
// 0x40 PA6 3J3-7  - Reverse Player 1               0x40 PA6 3J3-7  - Reverse Player 2 
// 0x80 PA7 3J3-8  - Down Player 1                  0x80 PA7 3J3-8  - Down Player 2 
//          3J3-9  - Key                            0x01 PB0 3J3-9  - Up Player 2 
//          3J3-10 - Ground                                  3J3-10 - Ground
//
// 0x80 PB7 is pulled up via R2 and grounded via W1 to determine if the game is upright (fitted) or cocktail (removed)
// Cocktail operation is only possible with red label ROMs
//
//
// CDefenderBaseGame::setupPIA1A initialises Port A of ROM board 6821 U1 to be inputs on bits 0-5
// CDefenderBaseGame::setupPIA2A initialises Port A of interface board 6821 U1 to be inputs
// CDefenderBaseGame::setupPIA2B initialises Port B of interface board 6821 U1 to be inputs
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { 
    //                                                        "012", "012345"
    {CDefenderBaseGame::setupPIA1A,       addressPIA1A, 0x3f, "ROM", "PB0-5 "}, // Coin Door Inputs
    {CDefenderBaseGame::setupPIA2A,       addressPIA2A, 0xff, "Int", "PA0-7 "}, // Control Panel
    {CDefenderBaseGame::setupPIA2B,       addressPIA2B, 0xff, "Int", "PB0-7 "}, // Control Panel/Upright or Cocktail
    {CDefenderBaseGame::onROMBankSelect0, 0xc800,       0xfc, "CPU", "VA8-13"}, // 6 bits of video counter chain VA8-13
    {0}
}; // end of list


//
// Output region:
// Interface board U1 6821 PIA output CB2 to select Player 1 or Player 2 controls
// ROM board U1 6821 PIA outputs to Sound board PB0-5 and PA6-7/PB6-7 to ROM board diagnostic LEDs
//
// CWmsSoundBaseGame::setupPIA1A initialises Bank A of ROM board PIA 6821 U1 to be outputs
// CWmsSoundBaseGame::setupPIA1B initialises Bank B of ROM board PIA 6821 U1 to be outputs
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { 
    //                                                                "012", "012345"
    {CDefenderBaseGame::setupPIA1A,       addressPIA1A,   0x80, 0x00, "ROM", "LED1  "}, // PA7
    {CDefenderBaseGame::setupPIA1A,       addressPIA1A,   0x40, 0x00, "ROM", "LED2  "}, // PA6
    {CDefenderBaseGame::setupPIA1B,       addressPIA1B,   0x80, 0x00, "ROM", "LED3  "}, // PB7
    {CDefenderBaseGame::setupPIA1B,       addressPIA1B,   0x40, 0x00, "ROM", "LED4  "}, // PB6
    {CDefenderBaseGame::setupPIA1B,       addressPIA1B,   0x02, 0x00, "ROM", "Sound0"}, // PB1
    {CDefenderBaseGame::setupPIA1B,       addressPIA1B,   0x01, 0x00, "ROM", "Sound1"}, // PB0
    {CDefenderBaseGame::setupPIA1B,       addressPIA1B,   0x08, 0x00, "ROM", "Sound2"}, // PB3
    {CDefenderBaseGame::setupPIA1B,       addressPIA1B,   0x04, 0x00, "ROM", "Sound3"}, // PB2
    {CDefenderBaseGame::setupPIA1B,       addressPIA1B,   0x20, 0x00, "ROM", "Sound4"}, // PB5
    {CDefenderBaseGame::setupPIA1B,       addressPIA1B,   0x10, 0x00, "ROM", "Sound5"}, // PB4
    {CDefenderBaseGame::onROMBankSelect0, addressPIA2B+1, 0x38, 0x30, "Int", "P1/P2 "}, // CB2
    {0}
}; // end of list


//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { 
    //                                        "0123456789"
    {CDefenderBaseGame::soundTestA,           "Sound 1-31"},
    {CDefenderBaseGame::soundTestB,           "Sound 0-4 "},
    {CDefenderBaseGame::watchdogAndVABusTest, "WD & VABus"},
    {CDefenderBaseGame::resetPIAs,            "Reset PIAs"},
    {0}
};


CDefenderBaseGame::CDefenderBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction),
    PIA1Ainitialised(false),
    PIA1Binitialised(false),
    PIA2Ainitialised(false),
    PIA2Binitialised(false) 
{
    m_cpu = new C6809ECpu(2, true);
    m_cpu->idle();

    // Interrupt request
    m_interrupt = ICpu::INT;

    // There is no direct hardware response of a vector on this platform.
    m_interruptAutoVector = true;
}


CDefenderBaseGame::~CDefenderBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// This is used to select Bank 0 (I/O) of the banked ROMs 0xc000 to 0xcfff
//
PERROR
CDefenderBaseGame::onROMBankSelect0(
    void *cDefenderBaseGame
)
{
    PERROR error = errorSuccess;
    CDefenderBaseGame *thisGame = (CDefenderBaseGame *) cDefenderBaseGame;
    ICpu *cpu = thisGame->m_cpu;

    // Address d000 sets the I/O to be available for 0xc000 to 0xcfff
    error = cpu->memoryWrite(0xd000, 0x00);

    return error;
}


//
// This is used to select Bank 1 of the banked ROMs 0xc000 to 0xcfff
//
PERROR
CDefenderBaseGame::onROMBankSelect1(
    void *cDefenderBaseGame
)
{
    PERROR error = errorSuccess;
    CDefenderBaseGame *thisGame = (CDefenderBaseGame *) cDefenderBaseGame;
    ICpu *cpu = thisGame->m_cpu;

    // Address d000 sets the ROM bank available for 0xc000 to 0xcfff
    error = cpu->memoryWrite(0xd000, 0x01);

    return error;
}


//
// This is used to select Bank 2 of the banked ROMs 0xc000 to 0xcfff
//
PERROR
CDefenderBaseGame::onROMBankSelect2(
    void *cDefenderBaseGame
)
{
    PERROR error = errorSuccess;
    CDefenderBaseGame *thisGame = (CDefenderBaseGame *) cDefenderBaseGame;
    ICpu *cpu = thisGame->m_cpu;

    // Address d000 sets the ROM bank available for 0xc000 to 0xcfff
    error = cpu->memoryWrite(0xd000, 0x02);

    return error;
}


//
// This is used to select Bank 3 of the banked ROMs 0xc000 to 0xcfff
//
PERROR
CDefenderBaseGame::onROMBankSelect3(
    void *cDefenderBaseGame
)
{
    PERROR error = errorSuccess;
    CDefenderBaseGame *thisGame = (CDefenderBaseGame *) cDefenderBaseGame;
    ICpu *cpu = thisGame->m_cpu;

    // Address d000 sets the ROM bank available for 0xc000 to 0xcfff
    error = cpu->memoryWrite(0xd000, 0x03);

    return error;
}


//
// This is used to select Bank 7 of the banked ROMs 0xc000 to 0xcfff
//
PERROR
CDefenderBaseGame::onROMBankSelect7(
    void *cDefenderBaseGame
)
{
    PERROR error = errorSuccess;
    CDefenderBaseGame *thisGame = (CDefenderBaseGame *) cDefenderBaseGame;
    ICpu *cpu = thisGame->m_cpu;

    // Address d000 sets the ROM bank available for 0xc000 to 0xcfff
    error = cpu->memoryWrite(0xd000, 0x07);

    return error;
}


//
// This is used to setup the hardware configuration of the ROM board 6821 PIA U1 Bank A
//
PERROR
CDefenderBaseGame::setupPIA1A(
    void *cDefenderBaseGame
)
{
    PERROR error = errorSuccess;
    CDefenderBaseGame *thisGame = (CDefenderBaseGame *) cDefenderBaseGame;
    ICpu *cpu = thisGame->m_cpu;

    // If we have previously set this PIA up there is no need to do it again as it interfers with the tests
    // If power has been cycled or PIAs need reinitialising for any other reason use the custom test 'Reset PIAs'
    if (thisGame->PIA1Ainitialised == false) 
    {
        // Address d000 sets the I/O to be available for 0xc000 to 0xcfff
        error = cpu->memoryWrite(0xd000, 0x00);

        if (SUCCESS(error)) {
            // Enable Data Direction Register (xxxxx0xx)
            error = cpu->memoryWrite(addressPIA1A+1, 0x00);
        }     

        if (SUCCESS(error)) {
            // Set PB0-PB5 as input pins and CA6-7 as output pins
            error = cpu->memoryWrite(addressPIA1A, 0xc0); 
        }

        if (SUCCESS(error)) {
            // Enable CA2 low > high IRQ (xx011xxx), Enable Output Register (xxxxx1xx), Enable CA1 low > high IRQ (xxxxxx11)
            error = cpu->memoryWrite(addressPIA1A+1, 0x1f); 
        }

        if (SUCCESS(error)) {
            // Set CA6-7 initial state as low
            error = cpu->memoryWrite(addressPIA1A, 0xc0); 
        }

        if (SUCCESS(error)) {
            thisGame->PIA1Ainitialised = true;
        }
    }

    return error;
}


//
// This is used to setup the hardware configuration of the ROM board 6821 PIA U1 Bank B
//
PERROR
CDefenderBaseGame::setupPIA1B(
    void *cDefenderBaseGame
)
{
    PERROR error = errorSuccess;
    CDefenderBaseGame *thisGame = (CDefenderBaseGame *) cDefenderBaseGame;
    ICpu *cpu = thisGame->m_cpu;

    // If we have previously set this PIA up there is no need to do it again as it interfers with the tests
    // If power has been cycled or PIAs need reinitialising for any other reason use the custom test 'Reset PIAs'
    if (thisGame->PIA1Binitialised == false) 
    {
        // Address d000 sets the I/O to be available for 0xc000 to 0xcfff
        error = cpu->memoryWrite(0xd000, 0x00);

        if (SUCCESS(error)) {
            // Enable Data Direction Register (xxxxx0xx)
            error = cpu->memoryWrite(addressPIA1B+1, 0x00);
        }

        if (SUCCESS(error)) {
            // Set PB0-PB7 as output pins
            error = cpu->memoryWrite(addressPIA1B, 0xff); 
        }

        if (SUCCESS(error)) {
            // Enable CB2 low > high IRQ (xx011xxx), Enable Output Register (xxxxx1xx), Enable CB1 low > high IRQ (xxxxxx11)
            error = cpu->memoryWrite(addressPIA1B+1, 0x1f); 
        }

        if (SUCCESS(error)) {
            // Set PB0-PB7 initial state as high
            error = cpu->memoryWrite(addressPIA1B, 0xff); 
        }

        if (SUCCESS(error)) {
            thisGame->PIA1Binitialised = true;
        }
    }

    return error;
}


//
// This is used to setup the hardware configuration of the interface board 6821 PIA U1 Bank A
//
PERROR
CDefenderBaseGame::setupPIA2A(
    void *cDefenderBaseGame
)
{
    PERROR error = errorSuccess;
    CDefenderBaseGame *thisGame = (CDefenderBaseGame *) cDefenderBaseGame;
    ICpu *cpu = thisGame->m_cpu;

    // If we have previously set this PIA up there is no need to do it again as it interfers with the tests
    // If power has been cycled or PIAs need reinitialising for any other reason use the custom test 'Reset PIAs'
    if (thisGame->PIA2Ainitialised == false) 
    {
        // Address d000 sets the I/O to be available for 0xc000 to 0xcfff
        error = cpu->memoryWrite(0xd000, 0x00);

        if (SUCCESS(error)) {
            // Enable Data Direction Register (xxxxx0xx)
            error = cpu->memoryWrite(addressPIA2A+1, 0x00);
        }  

        if (SUCCESS(error)) {
            // Set PA0-PA7 as input pins
            error = cpu->memoryWrite(addressPIA2A, 0x00); 
        }

        if (SUCCESS(error)) {
            // Set CA2 output low (xx110xxx), Enable Output Register (xxxxx1xx), Disable CA1 (xxxxxx00)
            error = cpu->memoryWrite(addressPIA2A+1, 0x34); 
        }

        if (SUCCESS(error)) {
            thisGame->PIA2Ainitialised = true;
        }
    }

    return error;
}


//
// This is used to setup the hardware configuration of the interface board 6821 PIA U1 Bank B
//
PERROR
CDefenderBaseGame::setupPIA2B(
    void *cDefenderBaseGame
)
{
    PERROR error = errorSuccess;
    CDefenderBaseGame *thisGame = (CDefenderBaseGame *) cDefenderBaseGame;
    ICpu *cpu = thisGame->m_cpu;

    // If we have previously set this PIA up there is no need to do it again as it interfers with the tests
    // If power has been cycled or PIAs need reinitialising for any other reason use the custom test 'Reset PIAs'
    if (thisGame->PIA2Binitialised == false) 
    {
        // Address d000 sets the I/O to be available for 0xc000 to 0xcfff
        error = cpu->memoryWrite(0xd000, 0x00);

        if (SUCCESS(error)) {
            // Enable Data Direction Register (xxxxx0xx)
            error = cpu->memoryWrite(addressPIA2B+1, 0x00);
        } 

        if (SUCCESS(error)) {
            // Set PB0-PB7 as input pins
            error = cpu->memoryWrite(addressPIA2B, 0x00); 
        }

        if (SUCCESS(error)) {
            // Set CB2 output high for P1 controls (xx111xxx), Enable Output Register (xxxxx1xx), Disable CB1 (xxxxxx00)
            error = cpu->memoryWrite(addressPIA2B+1, 0x34); 
        }

        if (SUCCESS(error)) {
            thisGame->PIA2Binitialised = true;
        }
    }

    return error;
}


//
// This is used to test /IRQ: 6809 CPU Pin 2 is pulled low by 6821 PIA U10 /IRQA Pin 37 or /IRQB Pin 38  
// This is triggered by ROM board 6821 PIA U1 CA1, CA2, CB1 or CB2 being pulled high
// CA1 (pin 40) - Count 240 from CPU Board
// CA2 (Pin 39) - Coin door slam switch 2J3 Pin 7
// CB1 (Pin 18) - VA11 from CPU board
// CB2 (Pin 19) - Sound board handshake 2J4 Pin 7 (not used on Defender) 
//
// When testing the interrupts are tested in the following order: CA1, CB1, CA2, CB2
// CA1 and CB1 should be picked up automatically, whereas CA2 and CB2 will needs grounds applying to 2J3-7 or 2J4-7 before timeout
//
// As tests progress feedback is given on the LCD:
// All good interrupts tested good:     *CA1 CB1 CA2 CB2
// CB1 timeout and CB2 unexpected IRQ:  *CA1 E:T CA2 E:U
//
// Error Codes:
// E:T - Timeout: Interrupt not recieved in specified time
// E:U - Unexpected: Interrupt did not clear
//
PERROR
CDefenderBaseGame::interruptCheck(
)
{
    PERROR error = errorCustom;
    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "/IRQ Tested OK!";

    UINT8 test;
    UINT8 data;
    UINT8 maskedData;
    UINT8 checked = 0;
    String lcdOutput = "*";
    String setPort[] = { "CA1", "CA2", "CB1", "CB2" };
    byte setPIA1A[]  = {  0x17,  0x1e,  0x16,  0x16 };
    byte setPIA1B[]  = {  0x16,  0x16,  0x17,  0x1e };
    byte flagMask[]  = {  0x80,  0x40,  0x80,  0x40 };
    boolean portA[]  = {  true,  true, false, false };

    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Interrupt Checks");
    lcd.setCursor(0, 1);
    lcd.print(lcdOutput);

    // Address d000 sets the I/O to be available for 0xc000 to 0xcfff
    m_cpu->memoryWrite(0xd000, 0x00);

    for (test = 0; test < 4; test++) {
        if (test > 0) lcdOutput += " ";

        // Set CA2/CB2 (xxnn nxxx), Set Output Register (xxxx xnxx), Set CA1/CB1 (xxxx xxnn)
        // Only one interrupt at a time enabled
        m_cpu->memoryWrite(addressPIA1A+1, setPIA1A[test]); 
        m_cpu->memoryWrite(addressPIA1B+1, setPIA1B[test]);

        // Ensure /IRQ and interrupt flags not set
        m_cpu->memoryRead(addressPIA1A, 0x00);
        m_cpu->memoryRead(addressPIA1B, 0x00);

        // Call Interrupt and wait for respose in specified time
        error = m_cpu->waitForInterrupt(m_interrupt, 5000);

        if (SUCCESS(error)) {
            if (portA[test] == true) {
                // Read port A Control Register to determine which IRQ has triggered
                m_cpu->memoryRead(addressPIA1A+1, &data);
                maskedData = data & 0xc0;

                // If valid flag read Output Register to release /IRQ and clear interrupt flag
                if (flagMask[test] = maskedData) m_cpu->memoryRead(addressPIA1A, 0x00);
            } else {
                // Read port B Control Registers to determine which IRQ has triggered
                m_cpu->memoryRead(addressPIA1B+1, &data);
                maskedData = data & 0xc0;

                // If valid flag read Output Register to release /IRQ and clear interrupt flag
                if (flagMask[test] = maskedData) m_cpu->memoryRead(addressPIA1B, 0x00);
            }

            // Check interrupt cleared, if a valid flag was not found error here will be errorUnexpected
            error = m_cpu->waitForInterrupt(m_interrupt, 0);

            if (SUCCESS(error)) {
                error = errorUnexpected;
                lcdOutput += "E:U";
            } else {
                lcdOutput += setPort[test];
                checked++;
            }
        } else {
            lcdOutput += "E:T";
        }
        lcd.setCursor(0, 1);
        lcd.print(lcdOutput);
    }

    delay(2000);
    error = errorCustom;
    if (checked < test) errorCustom->description = "E:/IRQ " + (String(checked)) + "/"+String(test)+" OK   ";

    return error;
}


//
// Custom function to cycle through all 32 output combinations using Sound0 to Sound4 (PB0-PB4) on PIA1B one at a time
// Sound5 (PB5) is not used on Defender
//
// Currently tests 19, 27 and 28 do not make any sound. 
// Also need to test the game end and high score of the day tunes
//
PERROR
CDefenderBaseGame::soundTestA(
    void *cDefenderBaseGame
)
{
    PERROR error = errorSuccess;
    CDefenderBaseGame *thisGame = (CDefenderBaseGame *) cDefenderBaseGame;
    ICpu *cpu = thisGame->m_cpu;

    // Re-initialse PIA1 so it's state is known
    thisGame->PIA1Ainitialised = false;
    thisGame->PIA1Binitialised = false;
    setupPIA1A(thisGame);
    setupPIA1B(thisGame);

    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Sound Test 1-31 ");

    // Loop through each sound test
    for (UINT8 test = 1; test < 32; test++) {
        lcd.setCursor(0, 1);
        lcd.print("* Test: " + String(test));

        cpu->memoryWrite(addressPIA1B, ~test & 0x7f);
        cpu->memoryWrite(addressPIA1A, 0x80);
        delay(50);

        cpu->memoryWrite(addressPIA1B, 0x7f);
        delay(450);

        cpu->memoryWrite(addressPIA1B, 0xbf);
        cpu->memoryWrite(addressPIA1A, 0x40);
        delay(500);
    }

    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("                ");

    // Pulse a series of sounds
    for (UINT8 flash = 0; flash < 16; flash=flash+2) {
        cpu->memoryWrite(addressPIA1B, 0x00);
        cpu->memoryWrite(addressPIA1A, 0x00);
        delay(50);

        cpu->memoryWrite(addressPIA1B, 0x3f);
        delay(50);

        lcd.setCursor(flash, 1);
        lcd.print(">");

        cpu->memoryWrite(addressPIA1B, 0xff);
        cpu->memoryWrite(addressPIA1A, 0xc0);
        delay(100);

        lcd.setCursor(flash+1, 1);
        lcd.print(">");
    }

    // Clear any running sounds
    cpu->memoryWrite(addressPIA1B, ~19 & 0x7f);
    cpu->memoryWrite(addressPIA1B, 0xff);
    cpu->memoryWrite(addressPIA1A, 0xc0);

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "Sound Test Done!";
    error = errorCustom;

    return error;
}


//
// Custom function to cycle through sound control lines Sound0 to Sound4 (PB0-PB4) on PIA1B one at a time
// Sound5 (PB5) is not used on Defender
//
PERROR
CDefenderBaseGame::soundTestB(
    void *cDefenderBaseGame
)
{
    PERROR error = errorSuccess;
    CDefenderBaseGame *thisGame = (CDefenderBaseGame *) cDefenderBaseGame;
    ICpu *cpu = thisGame->m_cpu;

    // Re-initialse PIA1 so it's state is known
    thisGame->PIA1Ainitialised = false;
    thisGame->PIA1Binitialised = false;
    setupPIA1A(thisGame);
    setupPIA1B(thisGame);

    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Sound Test 0-4");
    lcd.setCursor(0, 1);
    lcd.print("* Test: ");

    // Loop through each sound line
    for (UINT8 test = 0; test < 5; test++) {
        lcd.setCursor(8+test, 1);
        lcd.print(String(test));

        cpu->memoryWrite(addressPIA1B, ~(1 << test) & 0x7f);
        cpu->memoryWrite(addressPIA1A, 0x80);
        delay(50);

        cpu->memoryWrite(addressPIA1B, 0x7f);
        delay(450);

        cpu->memoryWrite(addressPIA1B, 0xbf);
        cpu->memoryWrite(addressPIA1A, 0x40);
        delay(500);
    }

    lcd.setCursor(0, 1);
    lcd.print("                ");

    // Pulse a series of sounds with increasing speed
    for (UINT8 flash = 0; flash < 16; flash++) {
        cpu->memoryWrite(addressPIA1B, ~(1 << 2) & 0x7f);
        cpu->memoryWrite(addressPIA1A, 0x80);
        delay(50);

        cpu->memoryWrite(addressPIA1B, 0x7f);
        delay(200-(flash*12));

        lcd.setCursor(flash, 1);
        lcd.print(">");  

        cpu->memoryWrite(addressPIA1B, 0xbf);
        cpu->memoryWrite(addressPIA1A, 0x40);
        delay(250-(flash*12));
    }

    // Clear any running sounds
    cpu->memoryWrite(addressPIA1B, ~19 & 0x7f);
    cpu->memoryWrite(addressPIA1B, 0xff);
    cpu->memoryWrite(addressPIA1A, 0xc0);

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "Sound Test Done!";
    error = errorCustom;

    return error;
}

//
// Custom function to test the Video Address Bus counter chain and the Watchdog timer clear
//
PERROR
CDefenderBaseGame::watchdogAndVABusTest(
    void *cDefenderBaseGame
)
{

    return errorNotImplemented;
}


//
// Custom function to reset PIAs as we only initialse them once
// If there was a power interruption or some other issue we can re-initialse them without resetting the Arduino
//
PERROR
CDefenderBaseGame::resetPIAs(
    void *cDefenderBaseGame
)
{
    PERROR error = errorSuccess;
    CDefenderBaseGame *thisGame = (CDefenderBaseGame *) cDefenderBaseGame;
    ICpu *cpu = thisGame->m_cpu;

    thisGame->PIA1Ainitialised = false;
    thisGame->PIA1Binitialised = false;
    thisGame->PIA2Ainitialised = false;
    thisGame->PIA2Binitialised = false;

    setupPIA1A(thisGame);
    setupPIA1B(thisGame);
    setupPIA1A(thisGame);
    setupPIA1B(thisGame);

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "All PIAs Reset! ";
    error = errorCustom;

    return error;
}
