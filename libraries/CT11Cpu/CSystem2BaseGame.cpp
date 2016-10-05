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
#include "CSystem2BaseGame.h"
#include "CT11Cpu.h"

//
// Probe Head GND:
//   T11 GND Pin 20
//
// Watchdog Disable:
//   IC 5J (LS08) to GND
//
// Board Designations:
//   'c' - CPU board.
//   'v' - Video board.
//
// CPU Compatibility Notes
//   * Some sort of clock stretching circuit i used with VRAM access.
//     This is worked around by using the VBLANK interrupt instead.
//

//
// The lower 4 bits map to IRQ0-3 (CP0-3)
//
static const UINT32 s_irqEnableAddress = 0x01001600;

static const UINT32 s_irq0ClearAddress = 0x01001580;
static const UINT32 s_p2ResetAddress   = 0x010015A0; // No clear for IRQ1
static const UINT32 s_irq2ClearAddress = 0x010015C0;
static const UINT32 s_irq3ClearAddress = 0x010015E0;


//
// RAM region is the same for all games on this board set.
//
// NOTE: The VRAM sync limits IO to ~50 bytes/s so the VRAM test takes several minutes to complete.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //
                                                  // See note above about access restrictions w.r.t video RAM access
                                                  // These regions are access with special support in the CT11Cpu.
                                                  //
                                                  //                                                                       "012", "012345"
                                                  {NO_BANK_SWITCH,                      0x00000000, 0x00000FFE, 1,   0xFF, "c7K", "Prog L"}, // Program RAM 6116, Lo
                                                  {NO_BANK_SWITCH,                      0x00000001, 0x00000FFF, 1,   0xFF, "c7P", "Prog H"}, // Program RAM 6116, Hi
                                                  {CSystem2BaseGame::onBankSwitchVRAM0, 0x03002000, 0x03003FFF, 1, 0xFFFF, "v??", "V.Ram "}, // Video RAM, 16-bit VSYNC
                                                  {0}
                                                }; // end of list

//
// No write-only RAM on this platform. Yay!
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                   "012", "012345"
                                                      {NO_BANK_SWITCH, 0x01001400, 0x00FF, "c1S", "ADC in"}, // ADC data
                                                      {NO_BANK_SWITCH, 0x01001800, 0x00FF, "c5P", "SW in "}, // Control inputs
                                                      {NO_BANK_SWITCH, 0x01001800, 0x8000, "c2F", "SlfTst"}, // Self Test
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                    "012", "012345"
                                                        {NO_BANK_SWITCH, 0x01001480,         0x0001, 0x0000,  "c1S", "ADC go"}, // ADC Start
                                                        {NO_BANK_SWITCH, s_irqEnableAddress, 0x0001, 0x0000,  "c5N", "IRQ0 e"}, // IRQ0 enable
                                                        {NO_BANK_SWITCH, s_irqEnableAddress, 0x0002, 0x0000,  "c5N", "IRQ1 e"}, // IRQ1 enable
                                                        {NO_BANK_SWITCH, s_irqEnableAddress, 0x0004, 0x0000,  "c5N", "IRQ2 e"}, // IRQ2 enable
                                                        {NO_BANK_SWITCH, s_irqEnableAddress, 0x0008, 0x0000,  "c5N", "IRQ3 e"}, // IRQ3 enable
                                                        {NO_BANK_SWITCH, s_irq0ClearAddress, 0x0001, 0x0000,  "c3N", "IRQ0 c"}, // IRQ0 clear
                                                        {NO_BANK_SWITCH, s_p2ResetAddress,   0x0001, 0x0000,  "c4H", "P2 Res"}, // 6502 Reset
                                                        {NO_BANK_SWITCH, s_irq2ClearAddress, 0x0001, 0x0000,  "c2M", "IRQ2 c"}, // IRQ2 clear
                                                        {NO_BANK_SWITCH, s_irq3ClearAddress, 0x0001, 0x0000,  "c3N", "IRQ3 c"}, // IRQ3 clear
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { {NO_CUSTOM_FUNCTION}}; // end of list


CSystem2BaseGame::CSystem2BaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegion, // It's all Byte wide
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new CT11Cpu(onAddressRemap, this);
    m_cpu->idle();

    // Default
    m_interrupt = ICpu::NMI;
    m_interruptAutoVector = true;
}


CSystem2BaseGame::~CSystem2BaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// Simple test that the VBLANK interrupt is generated.
//
PERROR
CSystem2BaseGame::interruptCheck(
)
{
    PERROR error = errorSuccess;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "OK:";

    // Enable the VBLANK interrupt
    error = m_cpu->memoryWrite(s_irqEnableAddress, 0x8);
    if (FAILED(error))
    {
        goto Exit;
    }

    for (int i = 0 ; i < 4 ; i++)
    {
        // Reset the VBLANK interrupt.
        error = m_cpu->memoryWrite(s_irq3ClearAddress, 0x0);
        if (FAILED(error))
        {
            goto Exit;
        }

        // Wait for VBLANK
        error = m_cpu->waitForInterrupt(ICpu::IRQ3, true, 300);
        if (FAILED(error))
        {
            goto Exit;
        }

        // Reset the VBLANK interrupt.
        error = m_cpu->memoryWrite(s_irq3ClearAddress, 0x0);
        if (FAILED(error))
        {
            goto Exit;
        }

        // Confirm VBLANK inactive
        error = m_cpu->waitForInterrupt(ICpu::IRQ3, false, 0);
        if (FAILED(error))
        {
            goto Exit;
        }

        // Wait for VBLANK
        error = m_cpu->waitForInterrupt(ICpu::IRQ3, true, 300);
        if (FAILED(error))
        {
            goto Exit;
        }
    }

Exit:
    return error;
}


//
// The paged program ROMs are mapped into two small blocks. In order to
// properly check those ROMs we remap the "virtual" address supplied
// in the ROM description into the paged area.
//
// NOTE: Since this function makes calls to write on the CPU, it is
//       recursive.
//
PERROR
CSystem2BaseGame::onAddressRemap(
    void   *cSystem2BaseGame,
    UINT32  addressIn,
    UINT32 *addressOut
)
{
    CSystem2BaseGame *thisGame   = (CSystem2BaseGame *) cSystem2BaseGame;
    ICpu             *cpu        = thisGame->m_cpu;
    UINT32            cpuAddress = addressIn;
    PERROR            error      = errorSuccess;

    // Check for one of the remapped program ROM's.
    if ((cpuAddress & 0x00F00000) != 0)
    {
        static UINT32 curPad0 = ~0;
        static UINT32 curPad1 = ~0;
        UINT32 pad = 0;

        cpuAddress = 0;

        //
        // PAD Calculation
        // ---------------
        //

        // DAL -> PAD -> ROM mapping
        //
        // DAL15 -> PAD5 ->  CS1-B
        // DAL14 -> PAD4 ->  CS0-A
        // DAL13 -> PAD3 ->  A13 (LA14)
        // DAL12 -> PAD2 ->  A12 (LA13)
        // DAL11 -> PAD1 -> !A15 (LA16) <- extended
        // DAL10 -> PAD0 -> !A14 (LA15)

        // Set PAD 4 & 5 based on ROM selected.
        pad |= ( addressIn & 0x00300000) >> (20 - 4);

        // Set PAD 2 & 3 based on LA 14 & 13
        pad |= ( addressIn & 0x00006000) >> (13 - 2);

        // Set PAD 0 & 1 based on LA 16 & 15
        pad |= (~addressIn & 0x00018000) >> (15 - 0);

        //
        // CPU Address Calculation
        // -----------------------
        //

        // Shave off LA0->LA12
        cpuAddress |= (addressIn & 0xFF001FFF);

        //
        // Select a page block to use.
        // Either would work OK but using both validates
        // the page register circuits more thoroughly.
        //
        if (addressIn & 0x00200000)
        {
            // Check if the PAD has changed to gate update.
            if (pad != curPad0)
            {
                // 16-bit write
                (void) cpu->memoryWrite(0x01001400, (pad << 10));
                curPad0 = pad;
            }

            // Add in the page address.
            cpuAddress |= 0x4000;
        }
        else
        {
            // Check if the PAD has changed to gate update.
            if (pad != curPad1)
            {
                // 16-bit write
                (void) cpu->memoryWrite(0x01001402, (pad << 10));
                curPad1 = pad;
            }

            // Add in the page address.
            cpuAddress |= 0x6000;
        }
    }

    // Check for VBLANK synchronization
    if ((cpuAddress & 0x02000000) != 0)
    {
        //
        // The VBLANK interrupt is already enabled
        // by the VRAM bank switch.
        //

        // Reset the VBLANK interrupt.
        error = cpu->memoryWrite(s_irq3ClearAddress, 0x0);
        if (FAILED(error))
        {
            goto Exit;
        }

        // Wait for VBLANK;
        error = cpu->waitForInterrupt(ICpu::IRQ3, true, 300);
        if (FAILED(error))
        {
            goto Exit;
        }

        // Strip the flag we just processed.
        cpuAddress &= ~0x02000000;

    }

Exit:

    *addressOut = cpuAddress;

    return error;
}


//
// Enable the VBLANK interrupt when the VRAM is selected.
// In future the slapstic programming for the video banking
// would go here.
//
PERROR
CSystem2BaseGame::onBankSwitchVRAM0(
    void   *cSystem2BaseGame
)
{
    PERROR           error     = errorSuccess;
    CSystem2BaseGame *thisGame  = (CSystem2BaseGame *) cSystem2BaseGame;
    ICpu             *cpu       = thisGame->m_cpu;

    // Enable the VBLANK interrupt for VRAM access sync.
    error = cpu->memoryWrite(s_irqEnableAddress, 0x8);
    if (FAILED(error))
    {
        goto Exit;
    }

Exit:
    return error;
}

