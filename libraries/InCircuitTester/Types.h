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
#ifndef Types_h
#define Types_h

#include "Arduino.h"

//
// System wide definitions
//

#define NULL ((void*)(0))

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

//                     "0123456789ABCDEF"
#define BLANK_LINE_16  "*               "

//
// Error codes.
//

#define ERROR_SUCCESS      (0)
#define ERROR_FAILED       (1)

//
// Base types
//

typedef signed char  INT8;
typedef signed short INT16;
typedef signed long  INT32;

typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned long  UINT32;

typedef char  CHAR;

typedef       char *PSTR;
typedef const char *PCSTR;


//
// System wide types.
//

//
// Representation of an error as a code plus description to print.
//
typedef struct _ERROR {

    UINT16 code;
    String description;

} ERROR, *PERROR;

//
// This is used as the call made based on a button selection.
//
typedef PERROR (*SelectorCallback)(void *context, int key);


//
// This is used as the callback for any bank switching.
//
typedef PERROR (*BankSwitchCallback)(void *context);

//
// Setting for the bank switch callback that none is required.
//
#define NO_BANK_SWITCH ((BankSwitchCallback) (NULL))

//
// This is used as the callback for address remapping.
// The remapped address is returned based on the supplied address.
//
typedef PERROR (*AddressRemapCallback)(void *context, UINT32 addressIn, UINT32 *addressOut);

//
// Setting for the address remap callback that none is required.
//
#define NO_ADDRESS_REMAP ((AddressRemapCallback) (NULL))

//
// This is used as the callback for external interrupt setup/enable.
//
typedef PERROR (*ExternalIntSetupCallback)(void *context);

//
// Setting for the external interrupt setup callback that none is required.
//
#define NO_EXTERANL_INT_SETUP ((ExternalIntSetupCallback) (NULL))

//
// This is used as the callback for external interrupt acknowledge/clear.
//
typedef PERROR (*ExternalIntAckCallback)(void *context);

//
// Setting for the external interrupt acknowledge callback that none is required.
//
#define NO_EXTERANL_INT_ACK ((ExternalIntAckCallback) (NULL))

//
// This is used for the custom function implementation.
//
typedef PERROR (*CustomFunctionCallback)(void *context);

//
// Setting for the custom function that indicates none.
//
#define NO_CUSTOM_FUNCTION ((CustomFunctionCallback) (NULL))

//
// This is used to construct an object.
//
typedef void* (*GameConstructor)();


//
// Selection description and function to call for it.
// If the selection callback implements a sub menu then
// "subMenu" is set to true and the callback will receive
// key presses.
//
typedef struct _SELECTOR {

  CHAR             description[17]; // 16 characters
  SelectorCallback function;
  void             *context;
  bool             subMenu;

} SELECTOR, *PSELECTOR;

//
// Pin connection definition to represent a native pin number and it's logical name.
//

typedef struct _CONNECTION {

    UINT8  pin;
    CHAR   name[12];

} CONNECTION, *PCONNECTION;


//
// ROM region definition for one device, sample data and it's complete CRC.
// "data2n" represents the ROM data at power of 2 offset addresses for the address bit check.
//        e.g. [0]===0x001, [1]===0x002, [2]===0x004, [3]===0x008 ...
// For example a ROM length of:-
//  - 0x0400 bytes (max address 0x3FF) has 10 data samples.
//  - 0x1000 bytes (max address 0xFFF) has 12 data samples.
//
// ROM Regions support 8-bit & 16-bit data access.
//

typedef struct _ROM_REGION {

    BankSwitchCallback bankSwitch;  // NULL if no bank switch is needed.
    UINT32             start;
    UINT32             length;
    const UINT16       *data2n;
    UINT32             crc;
    CHAR               location[4]; // 3 characters

} ROM_REGION, *PROM_REGION;


//
// RAM region definition for one device (maskable)
//
// step
//   Step use to support interleaved memory arrangements e.g.
//    - 8-bit access to 8-bit memories on an 8-bit bus then step == 1
//    - 2 x 8-bit RAMS configured as a 16-bit word on an 8-bit bus then step == 2
//    - 4 x 8-bit RAMS configured as a 32-bit word on an 8-bit bus then step == 4
//
//    - 16-bit access to 16-bit memories on an 16-bit bus then step == 1
//    - 2 x 16-bit RAMS configured as a 32-bit word on an 16-bit bus then step == 2
//

typedef struct _RAM_REGION {

    BankSwitchCallback bankSwitch;     // NULL if no bank switch is needed.
    UINT32             start;
    UINT32             end;
    UINT8              step;           // See note above
    UINT16             mask;
    CHAR               location[4];    // 3 characters
    CHAR               description[7]; // 6 characters

} RAM_REGION, *PRAM_REGION;


//
// Input region definition for one register (maskable)
// The address is 32-bits to allow the IO space to be selected.
//

typedef struct _INPUT_REGION {

    BankSwitchCallback bankSwitch;     // NULL if no bank switch is needed.
    UINT32             address;
    UINT16             mask;
    CHAR               location[4];    // 3 characters
    CHAR               description[7]; // 6 characters

} INPUT_REGION, *PINPUT_REGION;


//
// Output region definition for one register (maskable)
// The address is 32-bits to allow the IO space to be selected.
//
// How the masks work:
// activeMask = 0x01, invertMask = 0x00 => ON=0x01, OFF=0x00
// activeMask = 0x01, invertMask = 0xFF => ON=0xFE, OFF=0xFF
//

typedef struct _OUTPUT_REGION {

    BankSwitchCallback bankSwitch;     // NULL if no bank switch is needed.
    UINT32             address;
    UINT16             activeMask;     // Bitwise: 0 - Inactive   1 - Active
    UINT16             invertMask;     // Bitwise: 0 - Active hi, 1 - Active lo
    CHAR               location[4];    // 3 characters
    CHAR               description[7]; // 6 characters

} OUTPUT_REGION, *POUTPUT_REGION;

//
// Custom function definitions to allow the implementation of special
// test routines and game specific tests.
//
typedef struct _CUSTOM_FUNCTION {

    CustomFunctionCallback  function;         // NULL for end of function list.
    CHAR                    description[11];  // 10 characters

} CUSTOM_FUNCTION, *PCUSTOM_FUNCTION;


//
// Interrupt definition for the type of interrupt to expect.
//

typedef struct _INTERRUPT_DEFINITION {

    ExternalIntSetupCallback externalIntSetup; // NULL if no external interrupt setup is needed.
    ExternalIntAckCallback   externalIntAck;   // NULL if no external interrupt acknowledge is needed.
    UINT8                    type;             // ICpu::Interrupt
    UINT8                    response;         // The vector, 0 if there is no external hardware vector.
    CHAR                     location[4];      // 3 characters
    CHAR                     description[7];   // 6 characters

} INTERRUPT_DEFINITION, *PINTERRUPT_DEFINITION;


//
// Macro to format a UINT8 hex value into a string with leading zeros.
// The Arduino String library does not appear to have an option to do this.
//

#define STRING_UINT8_HEX(string, value)                     \
    {                                                       \
        UINT8 value8 = (UINT8) value;                       \
                                                            \
        if (value8 <= 0xF)                                  \
        {                                                   \
            string += " 0" + String(value8, HEX);           \
        }                                                   \
        else                                                \
        {                                                   \
            string += " "  + String(value8, HEX);           \
        }                                                   \
    }                                                       \

//
// Macro to format a UINT16 hex value into a string with leading zeros.
// The Arduino String library does not appear to have an option to do this.
//

#define STRING_UINT16_HEX(string, value)                      \
    {                                                         \
        UINT16 value16 = (UINT16) value;                      \
                                                              \
        if (value16 <= 0xF)                                   \
        {                                                     \
            string += " 000" + String(value16, HEX);          \
        }                                                     \
        else if (value16 <= 0xFF)                             \
        {                                                     \
            string += " 00" + String(value16, HEX);           \
        }                                                     \
        else if (value16 <= 0xFFF)                            \
        {                                                     \
            string += " 0" + String(value16, HEX);            \
        }                                                     \
        else                                                  \
        {                                                     \
            string += " " + String(value16, HEX);             \
        }                                                     \
    }                                                         \

//
// Macro to format a UINT32 24-bit hex value into a string with leading zeros.
// The Arduino String library does not appear to have an option to do this.
//

#define STRING_UINT32_24_HEX(string, value)                  \
    {                                                        \
        if (value <= 0xF)                                    \
        {                                                    \
            string += " 00000" + String(value, HEX);         \
        }                                                    \
        else if (value <= 0xFF)                              \
        {                                                    \
            string += " 0000" + String(value, HEX);          \
        }                                                    \
        else if (value <= 0xFFF)                             \
        {                                                    \
            string += " 000" + String(value, HEX);           \
        }                                                    \
        else if (value <= 0xFFFF)                            \
        {                                                    \
            string += " 00" + String(value, HEX);            \
        }                                                    \
        else if (value <= 0xFFFFF)                           \
        {                                                    \
            string += " 0" + String(value, HEX);             \
        }                                                    \
        else                                                 \
        {                                                    \
            string += " " + String((value & 0xFFFFFF), HEX); \
        }                                                    \
    }                                                        \


//
// Macro to format a UINT32 hex value into a string with leading zeros.
// The Arduino String library does not appear to have an option to do this.
//

#define STRING_UINT32_HEX(string, value)               \
    {                                                  \
        if (value <= 0xF)                              \
        {                                              \
            string += " 0000000" + String(value, HEX); \
        }                                              \
        else if (value <= 0xFF)                        \
        {                                              \
            string += " 000000" + String(value, HEX);  \
        }                                              \
        else if (value <= 0xFFF)                       \
        {                                              \
            string += " 00000" + String(value, HEX);   \
        }                                              \
        else if (value <= 0xFFFF)                      \
        {                                              \
            string += " 0000" + String(value, HEX);    \
        }                                              \
        else if (value <= 0xFFFFF)                     \
        {                                              \
            string += " 000" + String(value, HEX);     \
        }                                              \
        else if (value <= 0xFFFFFF)                    \
        {                                              \
            string += " 00" + String(value, HEX);      \
        }                                              \
        else if (value <= 0xFFFFFFF)                   \
        {                                              \
            string += " 0" + String(value, HEX);       \
        }                                              \
        else                                           \
        {                                              \
            string += " " + String(value, HEX);        \
        }                                              \
    }                                                  \

//
// Macro perform a CPU memory read and exit on error
//
#define CHECK_CPU_READ_EXIT(error, cpu, address, data)                      \
    {                                                                       \
        error = cpu->memoryRead(address, data);                             \
        if (error->code != ERROR_SUCCESS)                                   \
        {                                                                   \
            goto Exit;                                                      \
        }                                                                   \
    }                                                                       \

//
// Macro perform a CPU memory write and exit on error
//
#define CHECK_CPU_WRITE_EXIT(error, cpu, address, data)                     \
    {                                                                       \
        error = cpu->memoryWrite(address, data);                            \
        if (error->code != ERROR_SUCCESS)                                   \
        {                                                                   \
            goto Exit;                                                      \
        }                                                                   \
    }                                                                       \

//
// Macro to check a boolean value and exit with an error if it's wrong.
//
#define CHECK_BOOL_VALUE_EXIT(error, message, recValue, expValue)               \
        {                                                                       \
            if (recValue != expValue)                                           \
            {                                                                   \
                error = errorCustom;                                            \
                error->code = ERROR_FAILED;                                     \
                error->description = "E:";                                      \
                error->description += message;                                  \
                error->description += (expValue) ? " Hi" : " Lo";               \
                error->description += (recValue) ? " Hi" : " Lo";               \
                goto Exit;                                                      \
            }                                                                   \
        }                                                                       \

//
// Macro to check an 8-bit value and exit with an error if it's wrong.
//
#define CHECK_UINT8_VALUE_EXIT(error, message, recValue, expValue)              \
        {                                                                       \
            if (recValue != expValue)                                           \
            {                                                                   \
                error = errorCustom;                                            \
                error->code = ERROR_FAILED;                                     \
                error->description = "E:";                                      \
                error->description += message;                                  \
                STRING_UINT8_HEX(error->description, expValue);                 \
                STRING_UINT8_HEX(error->description, recValue);                 \
                goto Exit;                                                      \
            }                                                                   \
        }                                                                       \

//
// Macro to check a 16-bit value and exit with an error if it's wrong.
//
#define CHECK_UINT16_VALUE_EXIT(error, message, recValue, expValue)         \
    {                                                                       \
        if (recValue != expValue)                                           \
        {                                                                   \
            error = errorCustom;                                            \
            error->code = ERROR_FAILED;                                     \
            error->description = "E:";                                      \
            error->description += message;                                  \
            STRING_UINT16_HEX(error->description, expValue);                \
            STRING_UINT16_HEX(error->description, recValue);                \
            goto Exit;                                                      \
        }                                                                   \
    }                                                                       \

//
// Macro to check a literal value and exit with an error if it's wrong.
//
#define CHECK_LITERAL_VALUE_EXIT(error, connection, recValue, expValue)     \
    {                                                                       \
        if (recValue != expValue)                                           \
        {                                                                   \
            error = errorCustom;                                            \
            error->code = ERROR_FAILED;                                     \
            error->description = "E:";                                      \
            error->description += connection.name;                          \
            error->description += connection.pin;                           \
            error->description += (recValue == HIGH) ? " Hi " : " Lo ";     \
            error->description += __LINE__;                                 \
            goto Exit;                                                      \
        }                                                                   \
    }                                                                       \

//
// Macro to check a single pin value and exit with an error if it's wrong.
//
#define CHECK_VALUE_EXIT(error, pinMap, connection, expValue)               \
    {                                                                       \
        int recValue = digitalRead(pinMap[connection.pin]);                 \
        CHECK_LITERAL_VALUE_EXIT(error, connection, recValue, expValue);    \
    }                                                                       \

//
// Macro to check a single pin value and exit with an error if it's wrong.
// CFastPin version.
//
#define CHECK_PIN_VALUE_EXIT(error, pin, connection, expValue)              \
    {                                                                       \
        int recValue = pin.digitalRead();                                   \
        CHECK_LITERAL_VALUE_EXIT(error, connection, recValue, expValue);    \
    }                                                                       \

//
// Macro to check an 8-bit bus value and exit with an error if it's wrong.
//
#define CHECK_BUS_VALUE_UINT8_EXIT(error, bus, connection, expValue)     \
    {                                                                    \
        UINT16 value = 0;                                                \
        bus.digitalRead(&value);                                         \
        if (value != expValue)                                           \
        {                                                                \
            error = errorCustom;                                         \
            error->code = ERROR_FAILED;                                  \
            error->description  = "E:";                                  \
            error->description += connection[0].name;                    \
            STRING_UINT8_HEX(error->description, expValue);              \
            STRING_UINT8_HEX(error->description, value);                 \
            goto Exit;                                                   \
        }                                                                \
    }                                                                    \


//
// Macro to check a 16-bit bus value and exit with an error if it's wrong.
//
#define CHECK_BUS_VALUE_UINT16_EXIT(error, bus, connection, expValue) \
    {                                                                 \
        UINT16 value = 0;                                             \
        bus.digitalRead(&value);                                      \
        if (value != expValue)                                        \
        {                                                             \
            error = errorCustom;                                      \
            error->code = ERROR_FAILED;                               \
            error->description  = "E:";                               \
            error->description += connection[0].name;                 \
            STRING_UINT16_HEX(error->description, value);             \
            goto Exit;                                                \
        }                                                             \
    }                                                                 \

//
// Macro to check an 8-bit bus value and exit with an error if it's wrong.
//
// 0123456789abcdef
// E:r22 1234 55 AA
//
#define CHECK_VALUE_UINT8_BREAK(error, string, address, expValue, recValue)  \
    {                                                                        \
        if ((UINT8) expValue != (UINT8) recValue)                            \
        {                                                                    \
            error = errorCustom;                                             \
            error->code = ERROR_FAILED;                                      \
            error->description = "E:";                                       \
            error->description += string;                                    \
            STRING_UINT16_HEX(error->description, address);                  \
            STRING_UINT8_HEX(error->description, expValue);                  \
            STRING_UINT8_HEX(error->description, recValue);                  \
            break;                                                           \
        }                                                                    \
    }                                                                        \

//
// Macro to check an 16-bit bus value and exit with an error if it's wrong.
//
// 0123456789abcdef
// E:r22 5555 AAAA
//
#define CHECK_VALUE_UINT16_BREAK(error, string, address, expValue, recValue)  \
    {                                                                        \
        if ((UINT16) expValue != (UINT16) recValue)                            \
        {                                                                    \
            error = errorCustom;                                             \
            error->code = ERROR_FAILED;                                      \
            error->description = "E:";                                       \
            error->description += string;                                    \
            STRING_UINT16_HEX(error->description, expValue);                 \
            STRING_UINT16_HEX(error->description, recValue);                 \
            break;                                                           \
        }                                                                    \
    }                                                                        \

//
// Macro to load a string with an 8-bit region summary.
// 0123456789adcdef
//  001800 0F 11D
//
#define STRING_REGION8_SUMMARY(error, start, mask, location)   \
    {                                                          \
        error->code = ERROR_SUCCESS;                           \
        error->description = "";                               \
        STRING_UINT32_24_HEX(error->description, start);       \
        error->description += " ";                             \
        STRING_UINT8_HEX(error->description, mask);            \
        error->description += " ";                             \
        error->description += location;                        \
    }                                                          \

//
// Macro to load a string with an 16-bit region summary.
// 0123456789adcdef
//  001800 0F0F 11D
//
#define STRING_REGION16_SUMMARY(error, start, mask, location)  \
    {                                                          \
        error->code = ERROR_SUCCESS;                           \
        error->description = "";                               \
        STRING_UINT32_24_HEX(error->description, start);       \
        error->description += " ";                             \
        STRING_UINT16_HEX(error->description, mask);           \
        error->description += " ";                             \
        error->description += location;                        \
    }                                                          \

//
// Macro to load a string with an IO summary (8-bit)
// 0123456789adcdef
//  13F 0F 012345
//
#define STRING_IO8_SUMMARY(error, location, mask, regionDescription)  \
    {                                                                \
        error->code = ERROR_SUCCESS;                                 \
        error->description = "";                                     \
        error->description += " ";                                   \
        error->description += location;                              \
        STRING_UINT8_HEX(error->description, mask);                  \
        error->description += " ";                                   \
        error->description += regionDescription;                     \
    }                                                                \

//
// Macro to load a string with an IO summary (16-bit)
// 0123456789adcdef
//  13F 0F0F 012345
//
#define STRING_IO16_SUMMARY(error, location, mask, regionDescription)  \
    {                                                                \
        error->code = ERROR_SUCCESS;                                 \
        error->description = "";                                     \
        error->description += " ";                                   \
        error->description += location;                              \
        STRING_UINT16_HEX(error->description, mask);                 \
        error->description += " ";                                   \
        error->description += regionDescription;                     \
    }                                                                \

//
// Macro to wait for the low value of a CFastPin.
// Loop to wait for a logic LOW.
//
#define WAIT_FOR_LOW(fastPin)                                        \
    {                                                                \
        for (int x = 0 ; x < 256 ; x++)                              \
        {                                                            \
            if (fastPin.digitalRead() == LOW)                        \
            {                                                        \
                break;                                               \
            }                                                        \
        }                                                            \
    }                                                                \

//
// Macro to wait for the rising edge of a CFastPin.
// Loop to wait for a logic LOW and then wait for the logic HIGH
//
#define WAIT_FOR_RISING_EDGE(fastPin)                                        \
    {                                                                        \
        for (int x = 0 ; x < 32 ; x++)                                      \
        {                                                                    \
            if (fastPin.digitalRead() == LOW)                                \
            {                                                                \
                for (int y = 0 ; y < 32 ; y++)                              \
                {                                                            \
                    if (fastPin.digitalRead() == HIGH)                       \
                    {                                                        \
                        break;                                               \
                    }                                                        \
                }                                                            \
                break;                                                       \
            }                                                                \
        }                                                                    \
    }                                                                        \

//
// Macro to wait for the falling edge of a CFastPin.
// Loop to wait for a logic HIGH and then wait for the logic LOW
//
#define WAIT_FOR_FALLING_EDGE(fastPin)                                       \
    {                                                                        \
        for (int x = 0 ; x < 32 ; x++)                                      \
        {                                                                    \
            if (fastPin.digitalRead() == HIGH)                               \
            {                                                                \
                for (int y = 0 ; y < 32 ; y++)                              \
                {                                                            \
                    if (fastPin.digitalRead() == LOW)                        \
                    {                                                        \
                        break;                                               \
                    }                                                        \
                }                                                            \
                break;                                                       \
            }                                                                \
        }                                                                    \
    }                                                                        \

#endif
