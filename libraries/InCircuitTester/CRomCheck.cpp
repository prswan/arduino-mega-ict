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
#include "CRomCheck.h"
#include "zutil.h"


CRomCheck::CRomCheck(
    ICpu *cpu,
    const ROM_REGION romRegion[],
    void *bankSwitchContext
) : m_cpu(cpu),
    m_romRegion(romRegion),
    m_bankSwitchContext(bankSwitchContext)
{
};


//
// Performs all the ROM checks on all the regions supplied to the object.
//
PERROR
CRomCheck::check(
)
{
    PERROR error = errorSuccess;

    for (int index = 0 ; m_romRegion[index].length != 0 ; index++)
    {
        error = check( &m_romRegion[index] );

        if (FAILED(error))
        {
            break;
        }
    }

    return error;
}


//
// Performs a simple read of all the regions supplied to the object.
//
PERROR
CRomCheck::read(
)
{
    PERROR error = errorSuccess;

    for (int index = 0 ; m_romRegion[index].length != 0 ; index++)
    {
        error = read( &m_romRegion[index] );

        if (FAILED(error))
        {
            break;
        }
    }

    return error;
}

//
// Perform the simple binary power of 2 ROM check for the supplied region.
//
PERROR
CRomCheck::checkData2n(
    const ROM_REGION *romRegion
)
{
    PERROR error = errorSuccess;

    //
    // Check if we need to perform a bank switch for this region.
    // and do that now for all the testing to be done upon it.
    //

    if (romRegion->bankSwitch != NO_BANK_SWITCH)
    {
        error = romRegion->bankSwitch( m_bankSwitchContext );
    }

    if (SUCCESS(error))
    {
        UINT8 dataBusWidth    = m_cpu->dataBusWidth(romRegion->start);
        UINT8 dataAccessWidth = m_cpu->dataAccessWidth(romRegion->start);

        UINT16 dataBusWidthShift = (dataBusWidth == 2) ? 1 : 0;

        for (UINT32 shift = 0 ; (1UL << shift) < romRegion->length ; shift++)
        {
            UINT32 address = romRegion->start + (1UL << (shift + dataBusWidthShift));
            UINT16 expData = romRegion->data2n[shift];
            UINT16 recData = 0;

            error = m_cpu->memoryRead(address, &recData);

            if (FAILED(error))
            {
                break;
            }

            if (dataAccessWidth == 1)
            {
                CHECK_VALUE_UINT8_BREAK(error, romRegion->location, address, expData, recData);
            }
            else if (dataAccessWidth == 2)
            {
                CHECK_VALUE_UINT16_BREAK(error, romRegion->location, address, expData, recData);
            }
            else
            {
                error = errorNotImplemented;
                break;
            }
        }
    }

    return error;
}


//
// Calculate the CRC for the supplied ROM region.
//
PERROR
CRomCheck::calculateCrc(
    const ROM_REGION *romRegion,
    UINT32 *crc
)
{
    PERROR error = errorSuccess;

    //
    // Check if we need to perform a bank switch for this region.
    // and do that now for all the testing to be done upon it.
    //

    if (romRegion->bankSwitch != NO_BANK_SWITCH)
    {
        error = romRegion->bankSwitch( m_bankSwitchContext );
    }

    if (SUCCESS(error))
    {
        UINT8 dataBusWidth    = m_cpu->dataBusWidth(romRegion->start);
        UINT8 dataAccessWidth = m_cpu->dataAccessWidth(romRegion->start);

        UINT16 data = 0;
        UINT8  data8 = 0;
        UINT32 tempCrc = 0;

        for (UINT32 address = romRegion->start ;
             address < (romRegion->start + (romRegion->length * dataBusWidth));
             address += dataBusWidth)
        {
            error = m_cpu->memoryRead(address, &data);

            if (FAILED(error))
            {
                break;
            }

            if (dataAccessWidth == 1)
            {
                data8 = (UINT8) data;
                tempCrc = crc32(tempCrc, &data8, sizeof(data8));
            }
            else if (dataAccessWidth == 2)
            {
                data8 = (UINT8) (data >> 0);
                tempCrc = crc32(tempCrc, &data8, sizeof(data8));

                data8 = (UINT8) (data >> 8);
                tempCrc = crc32(tempCrc, &data8, sizeof(data8));
            }
            else
            {
                error = errorNotImplemented;
                break;
            }
        }

        if (SUCCESS(error))
        {
            *crc = tempCrc;
        }
    }
    return error;
}


//
// Read the first few data bytes and print them into the error string.
//
PERROR
CRomCheck::readData(
    const ROM_REGION *romRegion
)
{
    PERROR error = errorSuccess;
    UINT16 data[4] = {0};

    errorCustom->description = "OK:";

    //
    // Check if we need to perform a bank switch for this region.
    // and do that now for all the testing to be done upon it.
    //
    if (romRegion->bankSwitch != NO_BANK_SWITCH)
    {
        error = romRegion->bankSwitch( m_bankSwitchContext );
    }

    //
    // Read the first 4 bytes, maximum.
    //
    if (SUCCESS(error))
    {
        UINT8 dataBusWidth    = m_cpu->dataBusWidth(romRegion->start);
        UINT8 dataAccessWidth = m_cpu->dataAccessWidth(romRegion->start);

        if (dataAccessWidth == 1)
        {
            for (UINT32 index = 0 ; index < 4 ; index++)
            {
                error = m_cpu->memoryRead( (index * dataBusWidth) + romRegion->start,
                                           &data[index] );

                if (FAILED(error))
                {
                    break;
                }

                STRING_UINT8_HEX(errorCustom->description, data[index]);
                error = errorCustom;
            }
        }
        else if (dataAccessWidth = 2)
        {
            for (UINT32 index = 0 ; index < 2 ; index++)
            {
                error = m_cpu->memoryRead( (index * dataBusWidth) + romRegion->start,
                                           &data[index] );

                if (FAILED(error))
                {
                    break;
                }

                STRING_UINT16_HEX(errorCustom->description, data[index]);
                error = errorCustom;
            }
        }
        else
        {
            error = errorNotImplemented;
        }
    }

    return error;
}


//
// Perform the full CRC ROM check for the supplied region.
//
PERROR
CRomCheck::checkCrc(
    const ROM_REGION *romRegion
)
{
    PERROR error = errorSuccess;
    UINT32 crc = 0;

    error = calculateCrc(romRegion, &crc);

    if (SUCCESS(error))
    {
        if (crc != romRegion->crc)
        {
            error = errorCustom;

            error->code = ERROR_FAILED;
            error->description = "E:";
            error->description += romRegion->location;
            STRING_UINT32_HEX(error->description, crc);
        }
    }

    return error;
}


//
// Perform a simple read of all of the supplied region in order.
//
PERROR
CRomCheck::read(
    const ROM_REGION *romRegion
)
{
    PERROR error = errorSuccess;

    //
    // Check if we need to perform a bank switch for this region.
    // and do that now for all the testing to be done upon it.
    //

    if (romRegion->bankSwitch != NO_BANK_SWITCH)
    {
        error = romRegion->bankSwitch( m_bankSwitchContext );
    }

    if (SUCCESS(error))
    {
        UINT8 dataBusWidth = m_cpu->dataBusWidth(romRegion->start);

        for (UINT32 address = romRegion->start ;
             address < (romRegion->start + (romRegion->length * dataBusWidth)) ;
             address += dataBusWidth)
        {
            UINT16 recData = 0;

            error = m_cpu->memoryRead(address, &recData);

            if (FAILED(error))
            {
                break;
            }
        }
    }

    return error;
}

//
// Performs all the ROM checks on the region supplied to the object.
//
PERROR
CRomCheck::check(
    const ROM_REGION *romRegion
)
{
    PERROR error = checkData2n( romRegion );

    if (SUCCESS(error))
    {
        error = checkCrc( romRegion );
    }

    return error;
}

