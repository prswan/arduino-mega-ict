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
#include "CRamCheck.h"
#include "zutil.h"

static const long s_randomSeed[] = {7, 144};
static const long s_randomSize = 0x10000;


CRamCheck::CRamCheck(
    ICpu *cpu,
    const RAM_REGION ramRegion[],
    const RAM_REGION ramRegionWriteOnly[],
    void *bankSwitchContext
) : m_cpu(cpu),
    m_ramRegion(ramRegion),
    m_ramRegionWriteOnly(ramRegionWriteOnly),
    m_bankSwitchContext(bankSwitchContext)
{
};

//
// Performs all the RAM checks on all the regions supplied to the object.
//
PERROR
CRamCheck::check(
)
{
    PERROR error = errorSuccess;

    for (int i = 0 ; m_ramRegion[i].end != 0 ; i++)
    {
        error = check( &m_ramRegion[i] );

        if (FAILED(error))
        {
            break;
        }
    }

    return error;
}


//
// Performs all the RAM checks on all the regions supplied to the object.
//
// This variation uses a different seed for each region and performs write
// of all regions followed by the verifying read of all regions. It's aim is to
// detect chip select problems where two memorys respond to the same address.
//
// NOTE: The seed cannot be "0".
//
PERROR
CRamCheck::checkChipSelect(
)
{
    PERROR error = errorSuccess;

    //
    // Step 1 - Write all the regions
    //

    for (int i = 0 ; m_ramRegion[i].end != 0 ; i++)
    {
        error = writeRandom( &m_ramRegion[i],
                             (m_ramRegion[i].start & 0xFFFE) + 1,
                             true );

        if (FAILED(error))
        {
            break;
        }
    }

    //
    // Step 2 - Read & verify all the regions
    //

    if (SUCCESS(error))
    {
        for (int i = 0 ; m_ramRegion[i].end != 0 ; i++)
        {
            error = readVerifyRandom( &m_ramRegion[i],
                                      (m_ramRegion[i].start & 0xFFFE) + 1,
                                      true );

            if (FAILED(error))
            {
                break;
            }
        }
    }
    return error;
}


//
// Performs all the RAM writes on all the regions supplied to the object.
//
PERROR
CRamCheck::write(
)
{
    PERROR error = errorSuccess;

    for (int i = 0 ; m_ramRegion[i].end != 0 ; i++)
    {
        error = write( &m_ramRegion[i] );

        if (FAILED(error))
        {
            break;
        }
    }

    if (SUCCESS(error))
    {
        for (int i = 0 ; m_ramRegionWriteOnly[i].end != 0 ; i++)
        {
            error = write( &m_ramRegionWriteOnly[i] );

            if (FAILED(error))
            {
                break;
            }
        }
    }

    return error;
}


//
// Performs all the RAM writes on all the regions supplied to the object.
//
PERROR
CRamCheck::write(
    UINT8 value
)
{
    PERROR error = errorSuccess;

    for (int i = 0 ; m_ramRegion[i].end != 0 ; i++)
    {
        error = write( &m_ramRegion[i],
                       value );

        if (FAILED(error))
        {
            break;
        }
    }

    if (SUCCESS(error))
    {
        for (int i = 0 ; m_ramRegionWriteOnly[i].end != 0 ; i++)
        {
            error = write( &m_ramRegionWriteOnly[i],
                           value );

            if (FAILED(error))
            {
                break;
            }
        }
    }

    return error;
}

//
// Performs all the RAM reads on all the regions supplied to the object.
//
PERROR
CRamCheck::read(
)
{
    PERROR error = errorSuccess;

    for (int i = 0 ; m_ramRegion[i].end != 0 ; i++)
    {
        error = read( &m_ramRegion[i] );

        if (FAILED(error))
        {
            break;
        }
    }

    return error;
}


//
// Perform the simple random number write/read RAM check for the supplied region.
//
PERROR
CRamCheck::checkRandom(
    const RAM_REGION *ramRegion,
    int   seed
)
{
    PERROR error = errorSuccess;

    //
    // 2-pass - first pass normal data, second pass invert data.
    //
    for (int pass = 0 ; pass < 2 ; pass++)
    {
        if (SUCCESS(error))
        {
            error = writeRandom( ramRegion,
                                 (int) ramRegion->start + seed,
                                 (pass != 0) );

            if (FAILED(error))
            {
                break;
            }
        }

        if (SUCCESS(error))
        {
            error = readVerifyRandom( ramRegion,
                                      (int) ramRegion->start + seed,
                                      (pass != 0) );

            if (FAILED(error))
            {
                break;
            }
        }
    }
    return error;
}

//
// Write & read the first few data bytes and print them into the error string.
//
PERROR
CRamCheck::writeReadData(
    const RAM_REGION *ramRegion
)
{
    PERROR error = errorSuccess;
    UINT16 expData4[4] = {0x1111, 0x2222, 0x4444, 0x8888};
    UINT16 expData1[4] = {0x5555, 0xAAAA, 0x5555, 0xAAAA};
    UINT16 *expData = (UINT16*) NULL;;
    UINT16 recData[4] = {0};

    UINT8 dataBusWidth    = m_cpu->dataBusWidth(ramRegion->start);
    UINT8 dataAccessWidth = m_cpu->dataAccessWidth(ramRegion->start);

    //
    // This is more complicated than this simple test
    // as it needs the bit shift done to be correct.
    //
    if ((ramRegion->mask < 4) ||
        (dataAccessWidth > 1))
    {
        expData = expData1;
    }
    else
    {
        expData = expData4;
    }

    errorCustom->description = "OK:";

    //
    // Check if we need to perform a bank switch for this region.
    // and do that now for all the testing to be done upon it.
    //
    if (ramRegion->bankSwitch != NO_BANK_SWITCH)
    {
        error = ramRegion->bankSwitch( m_bankSwitchContext );
    }

    //
    // Write the first 4 bytes, maximum.
    //
    if (SUCCESS(error))
    {
        for (UINT32 index = 0 ; index < (4 / dataAccessWidth) ; index++)
        {
            error = m_cpu->memoryWrite( (index * dataBusWidth) + ramRegion->start,
                                        expData[index] );

            if (FAILED(error))
            {
                break;
            }
        }
    }

    //
    // Read the first 4 bytes, maximum.
    //
    if (SUCCESS(error))
    {
        if (dataAccessWidth == 1)
        {
            for (UINT32 index = 0 ; index < 4 ; index++)
            {
                error = m_cpu->memoryRead( (index * dataBusWidth) + ramRegion->start,
                                           &recData[index] );

                if (FAILED(error))
                {
                    break;
                }

                STRING_UINT8_HEX(errorCustom->description, (recData[index] & ramRegion->mask) );
                error = errorCustom;
            }
        }
        else if (dataAccessWidth == 2)
        {
            for (UINT32 index = 0 ; index < 2 ; index++)
            {
                error = m_cpu->memoryRead( (index * dataBusWidth) + ramRegion->start,
                                           &recData[index] );

                if (FAILED(error))
                {
                    break;
                }

                STRING_UINT16_HEX(errorCustom->description, (recData[index] & ramRegion->mask) );
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
// Perform a simple write of all of the supplied region in order.
// The data is written is the address.
//
PERROR
CRamCheck::write(
    const RAM_REGION *ramRegion
)
{
    PERROR error = errorSuccess;

    //
    // Check if we need to perform a bank switch for this region.
    // and do that now for all the testing to be done upon it.
    //

    if (ramRegion->bankSwitch != NO_BANK_SWITCH)
    {
        error = ramRegion->bankSwitch( m_bankSwitchContext );
    }

    if (SUCCESS(error))
    {
        UINT8 dataBusWidth = m_cpu->dataBusWidth(ramRegion->start);

        for (UINT32 address = ramRegion->start ; address <= ramRegion->end ; address += dataBusWidth)
        {
            //
            // The write is a simple data = address.
            //

            error = m_cpu->memoryWrite(address, (UINT8) address);

            if (FAILED(error))
            {
                break;
            }
        }
    }

    return error;
}


//
// Perform a simple write of all of the supplied region in order.
// The data is written is the value supplied
//
PERROR
CRamCheck::write(
    const RAM_REGION *ramRegion,
    UINT8 value
)
{
    PERROR error = errorSuccess;

    //
    // Check if we need to perform a bank switch for this region.
    // and do that now for all the testing to be done upon it.
    //

    if (ramRegion->bankSwitch != NO_BANK_SWITCH)
    {
        error = ramRegion->bankSwitch( m_bankSwitchContext );
    }

    if (SUCCESS(error))
    {
        UINT8 dataBusWidth = m_cpu->dataBusWidth(ramRegion->start);

        for (UINT32 address = ramRegion->start ; address <= ramRegion->end ; address += dataBusWidth)
        {
            error = m_cpu->memoryWrite(address, value);

            if (FAILED(error))
            {
                break;
            }
        }
    }

    return error;
}

//
// Perform a simple read of all of the supplied region in order.
//
PERROR
CRamCheck::read(
    const RAM_REGION *ramRegion
)
{
    PERROR error = errorSuccess;

    //
    // Check if we need to perform a bank switch for this region.
    // and do that now for all the testing to be done upon it.
    //

    if (ramRegion->bankSwitch != NO_BANK_SWITCH)
    {
        error = ramRegion->bankSwitch( m_bankSwitchContext );
    }

    if (SUCCESS(error))
    {
        UINT8 dataBusWidth = m_cpu->dataBusWidth(ramRegion->start);

        for (UINT32 address = ramRegion->start ; address <= ramRegion->end ; address += dataBusWidth)
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
// Performs all the RAM checks on the region supplied to the object.
//
PERROR
CRamCheck::check(
    const RAM_REGION *ramRegion
)
{
    PERROR error = errorSuccess;

    for (int j = 0 ; j < ARRAYSIZE(s_randomSeed) ; j++)
    {
        error = checkRandom( ramRegion,
                             s_randomSeed[j] );

        if (FAILED(error))
        {
            break;
        }
    }

    return error;
}


//
// Perform the simple random number write RAM check for the supplied region.
//
PERROR
CRamCheck::writeRandom(
    const RAM_REGION *ramRegion,
    int   seed,
    bool  invert
)
{
    PERROR error = errorSuccess;

    //
    // Check if we need to perform a bank switch for this region.
    // and do that now for all the testing to be done upon it.
    //

    if (ramRegion->bankSwitch != NO_BANK_SWITCH)
    {
        error = ramRegion->bankSwitch( m_bankSwitchContext );
    }

    if (SUCCESS(error))
    {
        UINT8 dataBusWidth    = m_cpu->dataBusWidth(ramRegion->start);
        UINT8 dataAccessWidth = m_cpu->dataAccessWidth(ramRegion->start);

        randomSeed(seed);
        for (UINT32 address = ramRegion->start ; address <= ramRegion->end ; address += dataBusWidth)
        {
            UINT16 data = (UINT16) random(s_randomSize);
            data = (invert) ? ~data : data;

            //
            // In order to make the Hi byte match the 16-bit word write
            // we use the Hi byte of the random data. This is needed
            // to ensure the same region marked as both 8-bit and 16-bit
            // in the ChipSelect test has the same data.
            //
            if ((dataBusWidth == 2)    &&
                (dataAccessWidth == 1) &&
                (address & 1)) {

                data = data >> 8;
            }

            error = m_cpu->memoryWrite(address, data);

            if (FAILED(error))
            {
                break;
            }
        }
    }
    return error;
}


//
// Perform the simple random number read-verify RAM check for the supplied region.
//
PERROR
CRamCheck::readVerifyRandom(
    const RAM_REGION *ramRegion,
    int   seed,
    bool  invert
)
{
    PERROR error = errorSuccess;

    //
    // Check if we need to perform a bank switch for this region.
    // and do that now for all the testing to be done upon it.
    //

    if (ramRegion->bankSwitch != NO_BANK_SWITCH)
    {
        error = ramRegion->bankSwitch( m_bankSwitchContext );
    }

    if (SUCCESS(error))
    {
        UINT8 dataBusWidth    = m_cpu->dataBusWidth(ramRegion->start);
        UINT8 dataAccessWidth = m_cpu->dataAccessWidth(ramRegion->start);

        randomSeed(seed);
        for (UINT32 address = ramRegion->start ; address <= ramRegion->end ; address += dataBusWidth)
        {
            UINT16 expData = (UINT16) random(s_randomSize);
            expData = (invert) ? ~expData : expData;
            UINT16 recData = 0;

            //
            // In order to make the Hi byte match the 16-bit word write
            // we use the Hi byte of the random data. This is needed
            // to ensure the same region marked as both 8-bit and 16-bit
            // in the ChipSelect test has the same data.
            //
            if ((dataBusWidth == 2)    &&
                (dataAccessWidth == 1) &&
                (address & 1)) {

                expData = expData >> 8;
            }

            error = m_cpu->memoryRead(address, &recData);

            if (FAILED(error))
            {
                break;
            }

            expData &= ramRegion->mask;
            recData &= ramRegion->mask;

            if (dataAccessWidth == 1)
            {
                CHECK_VALUE_UINT8_BREAK(error, ramRegion->location, address, expData, recData);
            }
            else if (dataAccessWidth == 2)
            {
                CHECK_VALUE_UINT16_BREAK(error, ramRegion->location, address, expData, recData);
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

