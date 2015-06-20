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
        for (UINT16 shift = 0 ; (1 << shift) < romRegion->length ; shift++)
        {
            UINT32 address = romRegion->start + (1 << shift);
            UINT8  expData = romRegion->data2n[shift];
            UINT8  recData = 0;

            error = m_cpu->memoryRead(address, &recData);

            if (FAILED(error))
            {
                break;
            }

            if (expData != recData)
            {
                error = errorCustom;
                error->code = ERROR_FAILED;
                error->description = "E:";
                error->description += romRegion->location;
                STRING_UINT16_HEX(error->description, address);
                STRING_UINT8_HEX(error->description, expData);
                STRING_UINT8_HEX(error->description, recData);
                break;
            }
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
        UINT8 data = 0;
        UINT32 crc = 0;

        for (UINT32 address = romRegion->start ; address < (romRegion->start + romRegion->length) ; address++)
        {
            error = m_cpu->memoryRead(address, &data);

            if (FAILED(error))
            {
                break;
            }

            crc = crc32(crc, &data, sizeof(data));
        }

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
        for (UINT32 address = romRegion->start ; address < (romRegion->start + romRegion->length) ; address++)
        {
            UINT8 recData = 0;

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

