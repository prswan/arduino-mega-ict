#include "Arduino.h"
#include "DFR_Key.h"

static int DEFAULT_KEY_PIN = 0;
static int DEFAULT_THRESHOLD = 20;

/*
	To use any alternate set of values you will need to enable it by a define.
	The define should NOT be done in code or it will impact all users. Visual
	Studio users can set 'DF_ROBOT_V1' in the Preprocessor definitions of the
	project, or add '-DDF_ROBOT_V1' under advanced options.

	Users of the standard Arduino IDE should create the file 'platform.local.txt'
	under <arduino_install>/hardware.arduino/avr and add the following line but
	be aware that when you upgrade your IDE this file may need to be re-created:

	compiler.cpp.extra_flags=-DDF_ROBOT_V1

	If further values are added in the future then of course adjust the name
	of the define that you specify accordingly.
*/
//
// The keys are read via a resistor ladder on one analog input. Each key is
// classified as everything up to the midpoint between it and the next key,
// rather than a narrow window around the nominal value, so that resistor
// tolerance and supply variation don't cause keys to be missed. LEFT, being
// far up the ladder, was the most affected by the narrow windows.
//
#ifdef DF_ROBOT_V1
	// Nominal: RIGHT 0, UP 98, DOWN 254, LEFT 407, SELECT 638, none 1023
	static int RIGHTKEY_MAX = 49;
	static int UPKEY_MAX    = 176;
	static int DOWNKEY_MAX  = 330;
	static int LEFTKEY_MAX  = 522;
	static int SELKEY_MAX   = 830;
#else
	// Nominal: RIGHT 0, UP 144, DOWN 329, LEFT 505, SELECT 742, none 1023
	static int RIGHTKEY_MAX = 72;
	static int UPKEY_MAX    = 236;
	static int DOWNKEY_MAX  = 417;
	static int LEFTKEY_MAX  = 623;
	static int SELKEY_MAX   = 882;
#endif

//
// The number of back to back samples that must agree, to reject noise and
// readings taken while a key contact is settling.
//
static const int SAMPLE_COUNT = 3;

static int classifyKey(int input)
{
  if (input <= RIGHTKEY_MAX) return RIGHT_KEY;
  if (input <= UPKEY_MAX)    return UP_KEY;
  if (input <= DOWNKEY_MAX)  return DOWN_KEY;
  if (input <= LEFTKEY_MAX)  return LEFT_KEY;
  if (input <= SELKEY_MAX)   return SELECT_KEY;
  return NO_KEY;
}

DFR_Key::DFR_Key()
{
  _refreshRate = 10;
  _keyPin = DEFAULT_KEY_PIN;
  _threshold = DEFAULT_THRESHOLD;
  _keyIn = NO_KEY;
  _curInput = NO_KEY;
  _curKey = NO_KEY;
  _prevInput = NO_KEY;
  _prevKey = NO_KEY;
  _oldTime = 0;
}

//
// Returns the key when it changes (including NO_KEY on release), otherwise
// SAMPLE_WAIT.
//
int DFR_Key::getKey()
{
  if ((millis() - _oldTime) < (unsigned long) _refreshRate)
  {
    return SAMPLE_WAIT;
  }

  _oldTime = millis();

  _curInput = analogRead(_keyPin);
  int key = classifyKey(_curInput);

  for (int i = 1 ; i < SAMPLE_COUNT ; i++)
  {
    delay(1);

    _curInput = analogRead(_keyPin);

    if (classifyKey(_curInput) != key)
    {
      // Still settling, try again next time.
      return SAMPLE_WAIT;
    }
  }

  _change = (key != _curKey);

  if (!_change)
  {
    return SAMPLE_WAIT;
  }

  _prevKey = _curKey;
  _curKey = key;

  return _curKey;
}

void DFR_Key::setRate(int rate)
{
  _refreshRate = rate;
}
