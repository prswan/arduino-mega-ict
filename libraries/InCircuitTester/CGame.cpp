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
#include "CGame.h"
#include "CRomCheck.h"
#include "CRamCheck.h"
#include "CIoCheck.h"
#include "keypad.h"

#include <avr/pgmspace.h>

CGame::CGame(
    const ROM_DATA2N            *romData2n,
    const ROM_REGION            *romRegion,
    const RAM_REGION            *ramRegion,
    const RAM_REGION            *ramRegionByteOnly,
    const RAM_REGION            *ramRegionWriteOnly,
    const INPUT_REGION          *inputRegion,
    const OUTPUT_REGION         *outputRegion,
    const CUSTOM_FUNCTION       *customFunction,
    const DelayFunctionCallback  delayFunction
)
{
    constructor(romData2n,
                romRegion,
                ramRegion,
                ramRegionByteOnly,
                ramRegionWriteOnly,
                inputRegion,
                outputRegion,
                customFunction,
                delayFunction);
}


CGame::CGame(
    const ROM_REGION            *romRegion,
    const RAM_REGION            *ramRegion,
    const RAM_REGION            *ramRegionByteOnly,
    const RAM_REGION            *ramRegionWriteOnly,
    const INPUT_REGION          *inputRegion,
    const OUTPUT_REGION         *outputRegion,
    const CUSTOM_FUNCTION       *customFunction,
    const DelayFunctionCallback  delayFunction
)
{
    constructor(0,
                romRegion,
                ramRegion,
                ramRegionByteOnly,
                ramRegionWriteOnly,
                inputRegion,
                outputRegion,
                customFunction,
                delayFunction);
}


CGame::~CGame(
)
{
    free( m_romRegion );

    if (m_romData2n != 0)
    {
        free( m_romData2n );
    }

    free( m_ramRegion );

    if (m_ramRegionByteOnly != m_ramRegion)
    {
        free( m_ramRegionByteOnly );
    }

    free( m_ramRegionWriteOnly );
    free( m_inputRegion );
    free( m_outputRegion );
    free( m_customFunction );
}


PERROR
CGame::busIdle(
)
{
    return m_cpu->idle();
}


PERROR
CGame::busCheck(
)
{
    PERROR error = m_cpu->idle();

    if (SUCCESS(error))
    {
        error = m_cpu->check();
    }

    return error;
}


PERROR
CGame::romCheckAll(
)
{
    PERROR error = errorSuccess;

    CRomCheck romCheck( m_cpu,
                        m_romRegion,
                        (void *) this );

    error = romCheck.check();

    return error;
}


PERROR
CGame::ramCheckAll(
)
{
    PERROR error = errorNotImplemented;

    // Only handle if a region was defined
    if (m_ramRegion[0].end != 0)
    {
        CRamCheck ramCheck( m_cpu,
                            m_delayFunction,
                            m_ramRegion,
                            m_ramRegionByteOnly,
                            m_ramRegionWriteOnly,
                            (void *) this );

        error = ramCheck.check();
    }

    return error;
}


PERROR
CGame::ramCheckAllChipSelect(
)
{
    PERROR error = errorNotImplemented;

    // Only handle if a region was defined
    if (m_ramRegion[0].end != 0)
    {
        CRamCheck ramCheck( m_cpu,
                            m_delayFunction,
                            m_ramRegion,
                            m_ramRegionByteOnly,
                            m_ramRegionWriteOnly,
                            (void *) this );

        error = ramCheck.checkChipSelect();
    }

    return error;
}


PERROR
CGame::ramCheckAllRandomAccess(
)
{
    PERROR error = errorNotImplemented;

    // Only handle if a region was defined
    if (m_ramRegionByteOnly[0].end != 0)
    {
        CRamCheck ramCheck( m_cpu,
                            m_delayFunction,
                            m_ramRegion,
                            m_ramRegionByteOnly,
                            m_ramRegionWriteOnly,
                            (void *) this );

        error = ramCheck.checkRandomAccess();
    }

    return error;
}


PERROR
CGame::interruptCheck(
)
{
    PERROR error = errorSuccess;
    UINT16 response = 0;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "OK:";

    for (int i = 0 ; i < 4 ; i++)
    {
        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        3000);
        if (FAILED(error))
        {
            break;
        }

        if (!m_interruptAutoVector)
        {
            error = m_cpu->acknowledgeInterrupt(&response);
            if (FAILED(error))
            {
                break;
            }

            CHECK_VALUE_UINT8_BREAK(error, String("Int"), i, m_interruptResponse, response);
        }

        error = m_cpu->waitForInterrupt(m_interrupt,
                                        true,
                                        0);
        if (SUCCESS(error))
        {
            error = errorUnexpected;
            break;
        }
        else
        {
            error = errorSuccess;
        }

    }

    return error;
}


PERROR
CGame::romCheck(
    int key
)
{
    PERROR error = errorSuccess;

    if (key == SELECT_KEY)
    {
        const ROM_REGION *region = &m_romRegion[m_RomReadRegion];

        CRomCheck romCheck( m_cpu,
                            m_romRegion,
                            (void *) this );

        error = romCheck.check(region);
    }
    else
    {
        error = onRomKeyMove(key);
    }

    return error;
}


PERROR
CGame::romCrc(
    int key
)
{
    PERROR error = errorSuccess;

    if (key == SELECT_KEY)
    {
        const ROM_REGION *region = &m_romRegion[m_RomReadRegion];

        CRomCheck romCheck( m_cpu,
                            m_romRegion,
                            (void *) this );

        UINT32 crc = 0;

        error = romCheck.calculateCrc(region, &crc);

        if (SUCCESS(error))
        {
            error = errorCustom;

            error->code = ERROR_SUCCESS;
            error->description = "OK:";
            error->description += region->location;
            STRING_UINT32_HEX(error->description, crc);
        }
    }
    else
    {
        error = onRomKeyMove(key);
    }

    return error;
}


PERROR
CGame::romRead(
    int key
)
{
    PERROR error = errorSuccess;

    if (key == SELECT_KEY)
    {
        const ROM_REGION *region = &m_romRegion[m_RomReadRegion];

        CRomCheck romCheck( m_cpu,
                            m_romRegion,
                            (void *) this );

        error = romCheck.readData(region);
    }
    else
    {
        error = onRomKeyMove(key);
    }

    return error;
}


PERROR
CGame::ramCheck(
    int key
)
{
    PERROR error = errorNotImplemented;

    // Only handle if a region was defined
    if (m_ramRegion[0].end != 0)
    {
        if (key == SELECT_KEY)
        {
            const RAM_REGION *region = &m_ramRegion[m_RamWriteReadRegion];

            CRamCheck ramCheck( m_cpu,
                                m_delayFunction,
                                m_ramRegion,
                                m_ramRegionByteOnly,
                                m_ramRegionWriteOnly,
                                (void *) this );

            error = ramCheck.check(region);
        }
        else
        {
            error = onRamKeyMove(key);
        }
    }

    return error;
}


PERROR
CGame::ramCheckRandomAccess(
    int key
)
{
    PERROR error = errorNotImplemented;

    // Only handle if a region was defined
    if (m_ramRegionByteOnly[0].end != 0)
    {
        if (key == SELECT_KEY)
        {
            const RAM_REGION *region = &m_ramRegionByteOnly[m_RamWriteReadByteRegion];

            CRamCheck ramCheck( m_cpu,
                                m_delayFunction,
                                m_ramRegion,
                                m_ramRegionByteOnly,
                                m_ramRegionWriteOnly,
                                (void *) this );

            error = ramCheck.checkRandomAccess(region);
        }
        else
        {
            error = onRamByteKeyMove(key);
        }
    }
    return error;
}


PERROR
CGame::ramCheckAddress(
    int key
)
{
    PERROR error = errorNotImplemented;

    // Only handle if a region was defined
    if (m_ramRegion[0].end != 0)
    {
        if (key == SELECT_KEY)
        {
            const RAM_REGION *region = &m_ramRegion[m_RamWriteReadRegion];

            CRamCheck ramCheck( m_cpu,
                                m_delayFunction,
                                m_ramRegion,
                                m_ramRegionByteOnly,
                                m_ramRegionWriteOnly,
                                (void *) this );

            error = ramCheck.checkAddress(region);
        }
        else
        {
            error = onRamKeyMove(key);
        }
    }

    return error;
}


PERROR
CGame::ramWriteRead(
    int key
)
{
    PERROR error = errorNotImplemented;

    // Only handle if a region was defined
    if (m_ramRegion[0].end != 0)
    {
        if (key == SELECT_KEY)
        {
            const RAM_REGION *region = &m_ramRegion[m_RamWriteReadRegion];

            CRamCheck ramCheck( m_cpu,
                                m_delayFunction,
                                m_ramRegion,
                                m_ramRegionByteOnly,
                                m_ramRegionWriteOnly,
                                (void *) this );

            error = ramCheck.writeReadData(region);
        }
        else
        {
            error = onRamKeyMove(key);
        }
    }

    return error;
}


PERROR
CGame::romReadAll(
    int key
)
{
    PERROR error = errorSuccess;

    CRomCheck romCheck( m_cpu,
                        m_romRegion,
                        (void *) this );

    error = romCheck.read();

    return error;
}


PERROR
CGame::ramWriteAllAD(
    int key
)
{
    PERROR error = errorSuccess;

    CRamCheck ramCheck( m_cpu,
                        m_delayFunction,
                        m_ramRegion,
                        m_ramRegionByteOnly,
                        m_ramRegionWriteOnly,
                        (void *) this );

    error = ramCheck.write();

    return error;
}


PERROR
CGame::ramWriteAllLo(
    int key
)
{
    PERROR error = errorSuccess;

    CRamCheck ramCheck( m_cpu,
                        m_delayFunction,
                        m_ramRegion,
                        m_ramRegionByteOnly,
                        m_ramRegionWriteOnly,
                        (void *) this );

    error = ramCheck.write( (UINT8) 0x00 );

    return error;
}


PERROR
CGame::ramWriteAllHi(
    int key
)
{
    PERROR error = errorSuccess;

    CRamCheck ramCheck( m_cpu,
                        m_delayFunction,
                        m_ramRegion,
                        m_ramRegionByteOnly,
                        m_ramRegionWriteOnly,
                        (void *) this );

    error = ramCheck.write( (UINT8) 0xFF );

    return error;
}


PERROR
CGame::ramReadAll(
    int key
)
{
    PERROR error = errorSuccess;

    CRamCheck ramCheck( m_cpu,
                        m_delayFunction,
                        m_ramRegion,
                        m_ramRegionByteOnly,
                        m_ramRegionWriteOnly,
                        (void *) this );

    error = ramCheck.read();

    return error;
}


PERROR
CGame::inputRead(
    int key
)
{
    PERROR error = errorSuccess;

    if (key == DOWN_KEY)
    {
        if (m_inputReadRegion > 0)
        {
            m_inputReadRegion--;
        }
    }

    if (key == UP_KEY)
    {
        if (m_inputRegion[m_inputReadRegion+1].mask != 0)
        {
            m_inputReadRegion++;
        }
    }

    {
        const INPUT_REGION *region = &m_inputRegion[m_inputReadRegion];

        if (key == SELECT_KEY)
        {
            CIoCheck ioCheck( m_cpu,
                              m_inputRegion,
                              m_outputRegion,
                              (void *) this );

            error = ioCheck.input(region);
        }
        else
        {
            UINT8 dataAccessWidth = m_cpu->dataAccessWidth(region->address);

            if (dataAccessWidth == 1)
            {
                STRING_IO8_SUMMARY(errorCustom, region->location, region->mask, region->description);
            }
            else if (dataAccessWidth == 2)
            {
                STRING_IO16_SUMMARY(errorCustom, region->location, region->mask, region->description);
            }
            else
            {
                error = errorNotImplemented;
            }

            if (SUCCESS(error))
            {
                error = errorCustom;
            }
        }
    }

    return error;
}


PERROR
CGame::outputWrite(
    int key
)
{
    PERROR error = errorSuccess;

    if (key == DOWN_KEY)
    {
        if (m_outputWriteRegion > 0)
        {
            m_outputWriteRegion--;
        }
    }

    if (key == UP_KEY)
    {
        if (m_outputRegion[m_outputWriteRegion+1].activeMask != 0)
        {
            m_outputWriteRegion++;
        }
    }

    {
        const OUTPUT_REGION *region = &m_outputRegion[m_outputWriteRegion];
        UINT8 dataAccessWidth = m_cpu->dataAccessWidth(region->address);

        if (key == SELECT_KEY)
        {
            CIoCheck ioCheck( m_cpu,
                              m_inputRegion,
                              m_outputRegion,
                              (void *) this );

            error = ioCheck.output(region, m_outputWriteRegionOn);

            m_outputWriteRegionOn = !m_outputWriteRegionOn;
        }
        else
        {
            if (dataAccessWidth == 1)
            {
                STRING_IO8_SUMMARY(errorCustom, region->location, region->activeMask, region->description);
            }
            else if (dataAccessWidth == 2)
            {
                STRING_IO16_SUMMARY(errorCustom, region->location, region->activeMask, region->description);
            }
            else
            {
                error = errorNotImplemented;
            }

            if (SUCCESS(error))
            {
                error = errorCustom;
            }
        }
    }

    return error;
}


PERROR
CGame::custom(
    int key
)
{
    PERROR error = errorNotImplemented;

    //
    // Only handle custom functions if custom functions have been
    // implemented
    //
    if (m_customFunction[0].function != NULL)
    {
        if (key == SELECT_KEY)
        {
            CustomFunctionCallback function = m_customFunction[m_customSelect].function;

            error = function(this);
        }
        else
        {
            error = onCustomKeyMove(key);
        }
    }

    return error;
}


PERROR
CGame::onRomKeyMove(
    int key
)
{
    PERROR error = errorSuccess;

    if (key == DOWN_KEY)
    {
        if (m_RomReadRegion > 0)
        {
            m_RomReadRegion--;
        }
    }

    if (key == UP_KEY)
    {
        if (m_romRegion[m_RomReadRegion+1].length != 0)
        {
            m_RomReadRegion++;
        }
    }

    if (key != SELECT_KEY)
    {
        const ROM_REGION *region = &m_romRegion[m_RomReadRegion];
        UINT8 dataAccessWidth = m_cpu->dataAccessWidth(region->start);

        if (dataAccessWidth == 1)
        {
            STRING_REGION8_SUMMARY(errorCustom, region->start, 0xFF, region->location);
        }
        else if (dataAccessWidth == 2)
        {
            STRING_REGION16_SUMMARY(errorCustom, region->start, 0xFFFF, region->location);
        }
        else
        {
            error = errorNotImplemented;
        }
    }

    if (SUCCESS(error))
    {
        error = errorCustom;
    }

    return error;
}


PERROR
CGame::onRamKeyMove(
    int key
)
{
    PERROR error = errorSuccess;

    if (key == DOWN_KEY)
    {
        if (m_RamWriteReadRegion > 0)
        {
            m_RamWriteReadRegion--;
        }
    }

    if (key == UP_KEY)
    {
        if (m_ramRegion[m_RamWriteReadRegion+1].end != 0)
        {
            m_RamWriteReadRegion++;
        }
    }

    if (key != SELECT_KEY)
    {
        const RAM_REGION *region = &m_ramRegion[m_RamWriteReadRegion];
        UINT8 dataAccessWidth = m_cpu->dataAccessWidth(region->start);

        if (dataAccessWidth == 1)
        {
            STRING_REGION8_SUMMARY(errorCustom, region->start, region->mask, region->location);
        }
        else if (dataAccessWidth == 2)
        {
            STRING_REGION16_SUMMARY(errorCustom, region->start, region->mask, region->location);
        }
        else
        {
            error = errorNotImplemented;
        }
    }

    if (SUCCESS(error))
    {
        error = errorCustom;
    }

    return error;
}


PERROR
CGame::onRamByteKeyMove(
    int key
)
{
    PERROR error = errorSuccess;

    if (key == DOWN_KEY)
    {
        if (m_RamWriteReadByteRegion > 0)
        {
            m_RamWriteReadByteRegion--;
        }
    }

    if (key == UP_KEY)
    {
        if (m_ramRegionByteOnly[m_RamWriteReadByteRegion+1].end != 0)
        {
            m_RamWriteReadByteRegion++;
        }
    }

    if (key != SELECT_KEY)
    {
        const RAM_REGION *region = &m_ramRegionByteOnly[m_RamWriteReadByteRegion];
        UINT8 dataAccessWidth = m_cpu->dataAccessWidth(region->start);

        if (dataAccessWidth == 1)
        {
            STRING_REGION8_SUMMARY(errorCustom, region->start, region->mask, region->location);
        }
        else if (dataAccessWidth == 2)
        {
            STRING_REGION16_SUMMARY(errorCustom, region->start, region->mask, region->location);
        }
        else
        {
            error = errorNotImplemented;
        }
    }

    if (SUCCESS(error))
    {
        error = errorCustom;
    }

    return error;
}


PERROR
CGame::onCustomKeyMove(
    int key
)
{
    PERROR error = errorSuccess;

    if (key == DOWN_KEY)
    {
        if (m_customSelect > 0)
        {
            m_customSelect--;
        }
    }

    if (key == UP_KEY)
    {
        if (m_customFunction[m_customSelect+1].function != NULL)
        {
            m_customSelect++;
        }
    }

    if (key != SELECT_KEY)
    {
        const CUSTOM_FUNCTION *customFunction = &m_customFunction[m_customSelect];

        errorCustom->code        = ERROR_SUCCESS;
        errorCustom->description = " ";
        errorCustom->description += customFunction->description;
    }

    if (SUCCESS(error))
    {
        error = errorCustom;
    }

    return error;
}


void CGame::constructor(
    const ROM_DATA2N            *romData2n,
    const ROM_REGION            *romRegion,
    const RAM_REGION            *ramRegion,
    const RAM_REGION            *ramRegionByteOnly,
    const RAM_REGION            *ramRegionWriteOnly,
    const INPUT_REGION          *inputRegion,
    const OUTPUT_REGION         *outputRegion,
    const CUSTOM_FUNCTION       *customFunction,
    const DelayFunctionCallback  delayFunction
)
{
    m_interrupt              = ICpu::NMI;
    m_interruptAutoVector    = false;
    m_interruptResponse      = 0;
    m_RomReadRegion          = 0;
    m_RamWriteReadRegion     = 0;
    m_RamWriteReadByteRegion = 0;
    m_inputReadRegion        = 0;
    m_outputWriteRegion      = 0;
    m_outputWriteRegionOn    = true;
    m_customSelect           = 0;

    m_romRegion = mallocProgMem(romRegion);

    if (romData2n != 0)
    {
        m_romData2n = mallocProgMem(romData2n, m_romRegion);
    }
    else
    {
        m_romData2n = 0;
    }

    m_ramRegion = mallocProgMem(ramRegion);

    if (ramRegion != ramRegionByteOnly)
    {
        m_ramRegionByteOnly = mallocProgMem(ramRegionByteOnly);
    }
    else
    {
        m_ramRegionByteOnly = m_ramRegion;
    }

    m_ramRegionWriteOnly = mallocProgMem(ramRegionWriteOnly);

    m_inputRegion = mallocProgMem(inputRegion);

    m_outputRegion = mallocProgMem(outputRegion);

    m_customFunction = mallocProgMem(customFunction);

    // Select the default if none was provided
    if (delayFunction == NO_DELAY_FUNCTION)
    {
        m_delayFunction = CGame::delayFunction;
    }
    else
    {
        m_delayFunction = delayFunction;
    }
}


//
// Copy the PROGMEM based region into local SRAM.
// Also updates the romRegion (that must be in SRAM) to
// point to the SRAM-based entries in romData2n.
//
ROM_DATA2N* CGame::mallocProgMem(
    const ROM_DATA2N *romData2n,
    ROM_REGION *romRegion
)
{
    ROM_DATA2N *retRomData2n;
    UINT16 uRegionSize = 0;
    UINT16 uIndexCount;

    // The number of entries is determined from the romRegion
    for (uIndexCount = 0 ; (romRegion[uIndexCount].length != 0) ; uIndexCount++) {}

    uRegionSize = sizeof(romData2n[0]) * (uIndexCount+1);

    retRomData2n = (PROM_DATA2N) malloc( uRegionSize );

    memcpy_P(retRomData2n, romData2n, uRegionSize);

    // Update the data2n pointers in the romRegion to reference SRAM
    for (uIndexCount = 0 ; (romRegion[uIndexCount].length != 0) ; uIndexCount++)
    {
        romRegion[uIndexCount].data2n = retRomData2n[uIndexCount].data2n;
    }

    return retRomData2n;
}


//
// Copy the PROGMEM based region into local SRAM.
//
ROM_REGION* CGame::mallocProgMem(
    const ROM_REGION *romRegion
)
{
    ROM_REGION *retRomRegion;
    UINT16 uIndexCount = 0;
    UINT16 uRegionSize = 0;

    for ( ;
          ((pgm_read_word_near((UINT16*) (&romRegion[uIndexCount].length) + 0) != 0) ||
           (pgm_read_word_near((UINT16*) (&romRegion[uIndexCount].length) + 1) != 0)) ;
          uIndexCount++) {}

    uRegionSize = sizeof(romRegion[0]) * (uIndexCount+1);

    retRomRegion = (PROM_REGION) malloc( uRegionSize );

    memcpy_P(retRomRegion, romRegion, uRegionSize);

    return retRomRegion;
}


// Copy the PROGMEM based region into local SRAM.
RAM_REGION* CGame::mallocProgMem(
    const RAM_REGION *ramRegion
)
{
    RAM_REGION *retRamRegion;
    UINT16 uIndexCount = 0;
    UINT16 uRegionSize = 0;

    for ( ;
          ((pgm_read_word_near((UINT16*) (&ramRegion[uIndexCount].end) + 0) != 0) ||
           (pgm_read_word_near((UINT16*) (&ramRegion[uIndexCount].end) + 1) != 0)) ;
          uIndexCount++) {}

    uRegionSize = sizeof(ramRegion[0]) * (uIndexCount+1);

    retRamRegion = (PRAM_REGION) malloc( uRegionSize );

    memcpy_P( retRamRegion, ramRegion, uRegionSize );

    return retRamRegion;
}


// Copy the PROGMEM based region into local SRAM.
INPUT_REGION* CGame::mallocProgMem(
    const INPUT_REGION *inputRegion
)
{
    INPUT_REGION *retInputRegion;
    UINT16 uIndexCount = 0;
    UINT16 uRegionSize = 0;

    for ( ; pgm_read_word_near(&inputRegion[uIndexCount].mask) != 0 ; uIndexCount++) {}

    uRegionSize = sizeof(inputRegion[0]) * (uIndexCount+1);

    retInputRegion = (PINPUT_REGION) malloc( uRegionSize );

    memcpy_P( retInputRegion, inputRegion, uRegionSize );

    return retInputRegion;
}


// Copy the PROGMEM based region into local SRAM.
OUTPUT_REGION* CGame::mallocProgMem(
    const OUTPUT_REGION *outputRegion
)
{
    OUTPUT_REGION *retOutputRegion;
    UINT16 uIndexCount = 0;
    UINT16 uRegionSize = 0;

    for ( ; pgm_read_word_near(&outputRegion[uIndexCount].activeMask) != 0 ; uIndexCount++) {}

    uRegionSize = sizeof(outputRegion[0]) * (uIndexCount+1);

    retOutputRegion = (POUTPUT_REGION) malloc( uRegionSize );

    memcpy_P( retOutputRegion, outputRegion, uRegionSize );

    return retOutputRegion;
}


// Copy the PROGMEM based region into local SRAM.
CUSTOM_FUNCTION* CGame::mallocProgMem(
    const CUSTOM_FUNCTION *customFunction
)
{
    CUSTOM_FUNCTION *retCustomFunction;
    UINT16 uIndexCount = 0;
    UINT16 uRegionSize = 0;

    for ( ; pgm_read_ptr_near(&customFunction[uIndexCount].function) != NULL ; uIndexCount++) {}

    uRegionSize = sizeof(customFunction[0]) * (uIndexCount+1);

    retCustomFunction = (PCUSTOM_FUNCTION) malloc( uRegionSize );

    memcpy_P( retCustomFunction, customFunction, uRegionSize );

    return retCustomFunction;
}


// Add an offset to the start and end addresses.
void CGame::addAddressOffset(
    RAM_REGION *ramRegion,
    UINT32 offset
)
{
    for ( UINT16 i = 0 ; ramRegion[i].end != 0 ; i++ )
    {
        ramRegion[i].start += offset;
        ramRegion[i].end   += offset;
    }
}


// Add an offset to the address.
void CGame::addAddressOffset(
    INPUT_REGION *inputRegion,
    UINT32 offset
)
{
    for ( UINT16 i = 0 ; inputRegion[i].mask != 0 ; i++ )
    {
        inputRegion[i].address += offset;
    }
}


// Add an offset to the address.
void CGame::addAddressOffset(
    OUTPUT_REGION *outputRegion,
    UINT32 offset
)
{
    for ( UINT16 i = 0 ; outputRegion[i].activeMask != 0 ; i++ )
    {
        outputRegion[i].address += offset;
    }
}


// Default delay function
PERROR CGame::delayFunction(
    void *context,
    unsigned long ms
)
{
    // NOTE: The context supplied is an ICpu object.
    delay(ms);
    return ERROR_SUCCESS;
}


