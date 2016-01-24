#include "Arduino.h"
#include "DFR_Key.h"

static int DEFAULT_KEY_PIN = 0; 
static int DEFAULT_THRESHOLD = 5;

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
#ifdef DF_ROBOT_V1
	static int RIGHTKEY_ARV = 0;	//that's read "analogue read value"
	static int UPKEY_ARV	= 98;  
	static int DOWNKEY_ARV	= 254;
	static int LEFTKEY_ARV	= 407;
	static int SELKEY_ARV	= 638;
	static int NOKEY_ARV	= 1023;
#else
	static int RIGHTKEY_ARV = 0;
	static int UPKEY_ARV	= 144;
	static int DOWNKEY_ARV	= 329;
	static int LEFTKEY_ARV	= 505;
	static int SELKEY_ARV	= 742;
	static int NOKEY_ARV	= 1023;
#endif

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

int DFR_Key::getKey()
{
 if (millis() > _oldTime + _refreshRate)
 {
    _prevInput = _curInput;
    _curInput = analogRead(_keyPin);
  
    if (_curInput == _prevInput)
    {
      _change = false;
      _curKey = _prevKey;
    }
    else
    {
      _change = true;
      _prevKey = _curKey;
  
      if (_curInput > UPKEY_ARV - _threshold && _curInput < UPKEY_ARV + _threshold ) _curKey = UP_KEY;
      else if (_curInput > DOWNKEY_ARV - _threshold && _curInput < DOWNKEY_ARV + _threshold ) _curKey = DOWN_KEY;
      else if (_curInput > RIGHTKEY_ARV - _threshold && _curInput < RIGHTKEY_ARV + _threshold ) _curKey = RIGHT_KEY;
      else if (_curInput > LEFTKEY_ARV - _threshold && _curInput < LEFTKEY_ARV + _threshold ) _curKey = LEFT_KEY;
      else if (_curInput > SELKEY_ARV - _threshold && _curInput < SELKEY_ARV + _threshold ) _curKey = SELECT_KEY;
      else _curKey = NO_KEY;
   }
   
   if (_change) return _curKey; else return SAMPLE_WAIT;
   _oldTime = millis();
 }
}

void DFR_Key::setRate(int rate)
{
  _refreshRate = rate;
}