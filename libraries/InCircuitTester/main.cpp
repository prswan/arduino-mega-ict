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
#include "main.h"

#include "Arduino.h"
#include <LiquidCrystal.h>
#include <DFR_Key.h>
#include <CGameCallback.h>

//
// Basic LCD diplay object (in this case, Sain 16 x 2).
//
static LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//
// Sain supplied keypad driver.
//
static DFR_Key keypad;

//
// The single on board LED is used to flash a heartbeat of sorts
// to be able to tell if the system in not running in the main loop.
//
static const UINT8 led = 13;

//
// This is the current selector.
//
static const SELECTOR *s_currentSelector;

//
// This is the game selector.
//
static SELECTOR *s_gameSelector;

//
// This is the current selection.
//
static int s_currentSelection;

//
// When true causes the soak test to run as soon as a game is selected.
//
bool s_runSoakTest;

//
// When set (none-zero) causes the select to repeat the selection callback
// for the set number of seconds.
//
int s_repeatSelectTimeInS;

//
// When true causes the repeat to ignore any reported error and continue the repeat
//
bool s_repeatIgnoreError;

//
// The selector used for the general tester configuration options.
//
static const SELECTOR s_configSelector[] PROGMEM = {//0123456789abcde
                                                    {"- Soak Test    ",  onSelectConfig, (void*) (&s_runSoakTest),           false},
                                                    {"- Set Repeat   ",  onSelectConfig, (void*) (&s_repeatSelectTimeInS),   false},
                                                    {"- Set Error    ",  onSelectConfig, (void*) (&s_repeatIgnoreError),     false},
                                                    { 0, 0 }
                                                   };

//
// Handler for the configuration callback to set options.
//
PERROR
onSelectConfig(
    void *context,
    int  key
)
{
    PERROR error = errorCustom;

    if (context == (void *) &s_repeatSelectTimeInS)
    {
        if (s_repeatSelectTimeInS == 0)
        {
            s_repeatSelectTimeInS = 5;
        }
        else if (s_repeatSelectTimeInS == 5)
        {
            s_repeatSelectTimeInS = 20;
        }
        else
        {
            s_repeatSelectTimeInS = 0;
        }

        errorCustom->description = String("OK: Repeat ") + String(s_repeatSelectTimeInS, DEC) + String("S");
        errorCustom->code = ERROR_SUCCESS;
    }

    if (context == (void *) &s_repeatIgnoreError)
    {
        if (s_repeatIgnoreError == false)
        {
            s_repeatIgnoreError = true;
            errorCustom->description = "OK: Ignore err";
        }
        else
        {
            s_repeatIgnoreError = false;
            errorCustom->description = "OK: Stop on err";
        }

        errorCustom->code = ERROR_SUCCESS;
    }

    if (context == (void *) &s_runSoakTest)
    {
        if (s_runSoakTest == false)
        {
            s_runSoakTest = true;
            errorCustom->description = "OK: Soak Test";
        }
        else
        {
            s_runSoakTest = false;
            errorCustom->description = "OK: Manual";
        }

        errorCustom->code = ERROR_SUCCESS;
    }

    return error;
}


//
// Handler for the game select callback that will switch the current
// game to the one supplied.
//
PERROR
onSelectGameCallback(
    void *context,
    int  key,
    const SELECTOR *selector
)
{
    PERROR error = errorSuccess;
    GameConstructor gameConstructor = (GameConstructor) context;

    if (CGameCallback::game != NULL)
    {
        delete CGameCallback::game;
        CGameCallback::game = (IGame *) NULL;
    }

    //
    // Assign a new game and reset the selector & selection.
    //

    CGameCallback::game = (IGame *) gameConstructor();

    s_currentSelector  = selector;
    s_currentSelection = 0;

    return error;
}

//
// Handler for the game select callback that will switch the current
// selector to the one supplied.
//
PERROR
onSelectGame(
    void *context,
    int  key
)
{
    PERROR error = errorNotImplemented;

    error = onSelectGameCallback(context,
                                 key,
                                 CGameCallback::selectorGame);

    if (SUCCESS(error) && s_runSoakTest)
    {
        error = onSelectSoakTest(context,
                                 key);
    }

    return error;
}

//
// Handler for the generic select callback that will switch the current
// selector to the one supplied.
//
PERROR
onSelectGeneric(
    void *context,
    int  key
)
{
    return onSelectGameCallback(context,
                                key,
                                CGameCallback::selectorGeneric);
}

//
// Handler for the soak test select callback that will run the soak test
// for the current game forever (if no error occurs).
//
PERROR
onSelectSoakTest(
    void *context,
    int  key
)
{
    PERROR error = errorNotImplemented;
    const SELECTOR *selector = CGameCallback::selectorSoakTest;
    int numSelections = 0;
    int selection = 0;
    int loop = 1;

    //
    // Count up how many selections were provided.
    //
    while(selector[numSelections].function != NULL)
    {
        numSelections++;
    }

    //
    // Loop to execute selections in random order forever.
    //
    do
    {
        String status = "* ";

        //
        // The random function seems to be averse to selecting 0
        // so this adjusts the range to skew a bit more towards 0
        //
        selection = random(numSelections + 1);
        if (selection != 0)
        {
            selection--;
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(selector[selection].description);

        lcd.setCursor(0, 1);
        status += loop;
        lcd.print(status);

        error = selector[selection].function(
                   selector[selection].context,
                   SELECT_KEY );

        //
        // The reset of the seed based on the loop count is done because the
        // tests reset the random seed so on exit the result of "random" could
        // select the same test again and cause the soak test to get stuck
        // running one selection.
        //
        randomSeed(loop++);
    }
    while (SUCCESS(error));

    //
    // If we get an error, leave the selector set and parked at the failing
    // test.
    //

    s_currentSelector  = selector;
    s_currentSelection = selection;

    return error;
}


void mainSetup(
    const SELECTOR *gameSelector
)
{
    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("In Circuit Test");
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);

    delay(2000);

    keypad.setRate(10);

    // Copy the PROGMEM based selectors into a single local SRAM selector
    {
        UINT16 uConfigRegionSize = 0;
        UINT16 uConfigIndexCount = 0;

        UINT16 uGameRegionSize   = 0;
        UINT16 uGameIndexCount   = 0;

        for ( ; pgm_read_word_near(&s_configSelector[uConfigIndexCount].function) != 0 ; uConfigIndexCount++) {}

        for ( ; pgm_read_word_near(&gameSelector[uGameIndexCount].function) != 0 ; uGameIndexCount++) {}

        uConfigRegionSize += sizeof(s_configSelector[0]) * uConfigIndexCount;
        uGameRegionSize += sizeof(gameSelector[0]) * (uGameIndexCount+1); // +1 to include the null terminator.

        s_gameSelector = (PSELECTOR) malloc( uConfigRegionSize + uGameRegionSize );

        memcpy_P( &s_gameSelector[0], s_configSelector, uConfigRegionSize );
        memcpy_P( &s_gameSelector[uConfigIndexCount], gameSelector, uGameRegionSize );
    }

    s_currentSelector = s_gameSelector;
}

void mainLoop()
{
    int previousKey = SAMPLE_WAIT;

    do {

        int currentKey = keypad.getKey();

        //
        // Special case of the first pass through to park at the first selector.
        //
        if (previousKey == SAMPLE_WAIT)
        {
            currentKey = LEFT_KEY;
        }

        if ( (currentKey == SAMPLE_WAIT) ||
             (currentKey == previousKey) )
        {
            digitalWrite(led, LOW);
            delay(100);
            digitalWrite(led, HIGH);

            continue;
        }

        switch (currentKey)
        {
            case NO_KEY : { break; }

            case LEFT_KEY :
            {
                if (s_currentSelection > 0)
                {
                    s_currentSelection--;
                }

                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(s_currentSelector[s_currentSelection].description);

                if (s_currentSelector[s_currentSelection].subMenu)
                {
                    PERROR error = s_currentSelector[s_currentSelection].function(
                        s_currentSelector[s_currentSelection].context,
                        NO_KEY );

                    lcd.setCursor(0, 1);
                    lcd.print(error->description);
                }

                break;
            }

            case RIGHT_KEY :
            {
                if (s_currentSelector[s_currentSelection+1].function != NULL)
                {
                    s_currentSelection++;
                }

                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(s_currentSelector[s_currentSelection].description);

                if (s_currentSelector[s_currentSelection].subMenu)
                {
                    PERROR error = s_currentSelector[s_currentSelection].function(
                        s_currentSelector[s_currentSelection].context,
                        NO_KEY );

                    lcd.setCursor(0, 1);
                    lcd.print(error->description);
                }

                break;
            }

            case UP_KEY     :
            case DOWN_KEY   :
            {
                if (s_currentSelector[s_currentSelection].subMenu)
                {
                    lcd.setCursor(0, 1);
                    lcd.print(BLANK_LINE_16);

                    PERROR error = s_currentSelector[s_currentSelection].function(
                                    s_currentSelector[s_currentSelection].context,
                                    currentKey );

                    lcd.setCursor(0, 1);
                    lcd.print(error->description);
                }
                break;
            }

            case SELECT_KEY :
            {
                unsigned long startTime = millis();
                unsigned long endTime = startTime + ((unsigned long) s_repeatSelectTimeInS * 1000);
                const SELECTOR *inSelector = s_currentSelector;
                PERROR error = errorSuccess;

                lcd.setCursor(0, 1);
                lcd.print(BLANK_LINE_16);

                do {

                    error = s_currentSelector[s_currentSelection].function(
                               s_currentSelector[s_currentSelection].context,
                               currentKey );
                }
                while ( (s_repeatIgnoreError || SUCCESS(error)) &&  // Ignoring or no failures
                        (millis() < endTime)                    &&  // Times not up.
                        (inSelector != s_gameSelector) );           // The input selector wasn't the game selector.

                //
                // The selection may have changed so update the whole display.
                //
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(s_currentSelector[s_currentSelection].description);

                lcd.setCursor(0, 1);
                lcd.print(error->description);
            }

            default : { break; };
        }

        previousKey = currentKey;

    } while (1);
}

