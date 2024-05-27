//
// Copyright (c) 2016, Paul R. Swan
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
#include "CIoCheck.h"
#include "zutil.h"


CIoCheck::CIoCheck(
    ICpu *cpu,
    const INPUT_REGION  inputRegion[],
    const OUTPUT_REGION outputRegion[],
    void *bankSwitchContext
) : m_cpu(cpu),
    m_inputRegion(inputRegion),
    m_outputRegion(outputRegion),
    m_bankSwitchContext(bankSwitchContext)
{
};


//
// Read the inputs defined by the supplied region.
//
PERROR
CIoCheck::input(
    const INPUT_REGION *inputRegion
)
{
    PERROR error = errorSuccess;

    //
    // Check if we need to perform a bank switch for this region.
    // and do that now for all the testing to be done upon it.
    //
    if (inputRegion->bankSwitch != NO_BANK_SWITCH)
    {
        error = inputRegion->bankSwitch(m_bankSwitchContext);
    }

    if (SUCCESS(error))
    {
        UINT8 dataAccessWidth = m_cpu->dataAccessWidth(inputRegion->address);
        UINT16 recData = 0;

        error = m_cpu->memoryRead( inputRegion->address,
                                   &recData );

        errorCustom->description = "OK:";

        if (dataAccessWidth == 1)
        {
            STRING_UINT8_HEX(errorCustom->description, (recData & inputRegion->mask) );
        }
        else if (dataAccessWidth == 2)
        {
            STRING_UINT16_HEX(errorCustom->description, (recData & inputRegion->mask) );
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

    return error;
}


//
// Write the outputs defined by the supplied region.
//
PERROR
CIoCheck::output(
    const OUTPUT_REGION *outputRegion,
    bool  on
)
{
    PERROR error = errorSuccess;

    //
    // Check if we need to perform a bank switch for this region.
    // and do that now for all the testing to be done upon it.
    //
    if (outputRegion->bankSwitch != NO_BANK_SWITCH)
    {
        error = outputRegion->bankSwitch(m_bankSwitchContext);
    }

    if (SUCCESS(error))
    {
        UINT8 dataAccessWidth = m_cpu->dataAccessWidth(outputRegion->address);
        UINT16 outData;

        //
        // If we're wanting to write active(on) then just use the mask.
        // else use the clear of the mask. This is a toggle.
        //
        if (on)
        {
            errorCustom->description = "OK:On ";

            outData = outputRegion->invertMask ^ outputRegion->activeMask;
        }
        else
        {
            errorCustom->description = "OK:Off";

            outData = outputRegion->invertMask;
        }

        error = m_cpu->memoryWrite( outputRegion->address,
                                    outData );


        if (dataAccessWidth == 1)
        {
            STRING_UINT8_HEX(errorCustom->description, outData);
        }
        else if (dataAccessWidth == 2)
        {
            STRING_UINT16_HEX(errorCustom->description, outData);
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

    return error;
}

