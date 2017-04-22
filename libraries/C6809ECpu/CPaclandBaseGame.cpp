//
// Copyright (c) 2017, Phillip Riscombe-Burton
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
#include "CPaclandBaseGame.h"
#include "C6809ECpu.h"

//
// Notes
// -----
//
// * The Clock output from the ICT should be connected to pin 35 of CUS27 at 6D pin that's
//   been disconnected from the '49MHz / 32 = 1.536MHz' signal clock source (e.g. bend out pin 35 and connect the ICT clock to it).
//

//
// Programmatically used definitions
//


//********* FROM STAR WARS:
//// ROM bank switching
//static const UINT32 c_MPAGE_A  = 0x4684;
//static const UINT8  c_MPAGE_D  = 0x80;
//*********** END FROM STAR WARS




//NEED TO SEE IF ANY OF THESE ARE WRITE-ONLY
//AM_RAM_WRITE ?
//AM_RAM AM_SHARE?

//WHAT NEEDS BANK SWITCHING?


//********* PACLAND MAME
//static ADDRESS_MAP_START( main_map, AS_PROGRAM, 8, pacland_state )
//AM_RANGE(0x0000, 0x0fff) AM_RAM_WRITE(videoram_w) AM_SHARE("videoram")
//AM_RANGE(0x1000, 0x1fff) AM_RAM_WRITE(videoram2_w) AM_SHARE("videoram2")
//AM_RANGE(0x2000, 0x37ff) AM_RAM AM_SHARE("spriteram")

//AM_RANGE(0x3800, 0x3801) AM_WRITE(scroll0_w)
//AM_RANGE(0x3a00, 0x3a01) AM_WRITE(scroll1_w)
//AM_RANGE(0x3c00, 0x3c00) AM_WRITE(bankswitch_w)
//AM_RANGE(0x4000, 0x5fff) AM_ROMBANK("bank1")
//AM_RANGE(0x6800, 0x6bff) AM_DEVREADWRITE("namco", namco_cus30_device, namcos1_cus30_r, namcos1_cus30_w)      /* PSG device, shared RAM */
//AM_RANGE(0x7000, 0x7fff) AM_WRITE(irq_1_ctrl_w)
//AM_RANGE(0x7800, 0x7fff) AM_DEVREAD("watchdog", watchdog_timer_device, reset_r)
//AM_RANGE(0x8000, 0xffff) AM_ROM
//AM_RANGE(0x8000, 0x8fff) AM_WRITE(subreset_w)
//AM_RANGE(0x9000, 0x9fff) AM_WRITE(flipscreen_w)
//ADDRESS_MAP_END
//
//static ADDRESS_MAP_START( mcu_map, AS_PROGRAM, 8, pacland_state )
//AM_RANGE(0x0000, 0x001f) AM_DEVREADWRITE("mcu", hd63701_cpu_device, m6801_io_r, m6801_io_w)
//AM_RANGE(0x0080, 0x00ff) AM_RAM
//AM_RANGE(0x1000, 0x13ff) AM_DEVREADWRITE("namco", namco_cus30_device, namcos1_cus30_r, namcos1_cus30_w)      /* PSG device, shared RAM */
//AM_RANGE(0x2000, 0x3fff) AM_DEVWRITE("watchdog", watchdog_timer_device, reset_w)     /* watchdog? */
//AM_RANGE(0x4000, 0x7fff) AM_WRITE(irq_2_ctrl_w)
//AM_RANGE(0x8000, 0xbfff) AM_ROM
//AM_RANGE(0xc000, 0xc7ff) AM_RAM
//AM_RANGE(0xd000, 0xd003) AM_READ(input_r)
//AM_RANGE(0xf000, 0xffff) AM_ROM
//ADDRESS_MAP_END
//********* END PACLAND MAME




//
// Write-only RAM region
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { //                                       "012", "012345"
                                                {0}
                                                }; // end of list

//
// RAM region
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                "012", "012345"
                                                {NO_BANK_SWITCH, 0x0000,      0x0FFF,      1, 0xFF, "   ", "      "}, // "Vid RAM,   6116"
                                                {NO_BANK_SWITCH, 0x1000,      0x1FFF,      1, 0xFF, "   ", "      "}, // "Vid RAM 2, 6116"
                                                {NO_BANK_SWITCH, 0x2000,      0x37ff,      1, 0xFF, "   ", "      "}, // "Sprite RAM"
                                                  {0}
                                                }; // end of list


//
// Input region is the same for all versions.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                                      "012", "012345"
                                                      /*{NO_BANK_SWITCH, 0x4300,    0xFF,       "9J ", "IN0   "}, // Input port 0
                                                      {NO_BANK_SWITCH, 0x4320,    0xFF,       "9H ", "IN1   "}, // Input port 1
                                                      {NO_BANK_SWITCH, 0x4340,    0xFF,       "9E ", "OPT0  "}, // DIP Switch 10D
                                                      {NO_BANK_SWITCH, 0x4360,    0xFF,       "9F ", "OPT1  "}, // DIP Switch 10E/F
                                                      {NO_BANK_SWITCH, c_ADC_A,   0xFF,       "9K ", "ADC   "}, // Yoke analog inputs
                                                      {NO_BANK_SWITCH, c_MBRUN_A, c_MBRUN_D,  "9H ", "MB RUN"}, // MATHRUN
                                                      {NO_BANK_SWITCH, c_REH_A,   0xFF,       "4J ", "REH   "}, // REH Hi - Quotient Hi
                                                      {NO_BANK_SWITCH, c_REL_A,   0xFF,       "4K ", "REL   "}, // REH Lo - Quotient Lo*/
                                                      {0}
                                                    }; // end of list


//******** PACLAND MAME
//static INPUT_PORTS_START( pacland )
//PORT_START("DSWA")
//PORT_SERVICE_DIPLOC( 0x80, IP_ACTIVE_LOW, "SWA:1" )
//PORT_DIPNAME( 0x60, 0x60, DEF_STR( Lives ) )        PORT_DIPLOCATION("SWA:3,2")
//PORT_DIPSETTING(    0x40, "2" )
//PORT_DIPSETTING(    0x60, "3" )
//PORT_DIPSETTING(    0x20, "4" )
//PORT_DIPSETTING(    0x00, "5" )
//PORT_DIPNAME( 0x18, 0x18, DEF_STR( Coin_A ) )       PORT_DIPLOCATION("SWA:5,4")
//PORT_DIPSETTING(    0x00, DEF_STR( 3C_1C ) )
//PORT_DIPSETTING(    0x08, DEF_STR( 2C_1C ) )
//PORT_DIPSETTING(    0x18, DEF_STR( 1C_1C ) )
//PORT_DIPSETTING(    0x10, DEF_STR( 1C_2C ) )
//PORT_DIPNAME( 0x04, 0x04, DEF_STR( Demo_Sounds ) )  PORT_DIPLOCATION("SWA:6")
//PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
//PORT_DIPSETTING(    0x04, DEF_STR( On ) )
//PORT_DIPNAME( 0x03, 0x03, DEF_STR( Coin_B ) )       PORT_DIPLOCATION("SWA:8,7")
//PORT_DIPSETTING(    0x00, DEF_STR( 3C_1C ) )
//PORT_DIPSETTING(    0x01, DEF_STR( 2C_1C ) )
//PORT_DIPSETTING(    0x03, DEF_STR( 1C_1C ) )
//PORT_DIPSETTING(    0x02, DEF_STR( 1C_2C ) )
//
//PORT_START("DSWB")
//PORT_DIPNAME( 0xe0, 0xe0, DEF_STR( Bonus_Life ) )   PORT_DIPLOCATION("SWB:3,2,1")
//PORT_DIPSETTING(    0xe0, "30K 80K 130K 300K 500K 1M" )     // "A"
//PORT_DIPSETTING(    0x80, "30K 80K every 100K" )            // "D"
//PORT_DIPSETTING(    0x40, "30K 80K 150K" )                  // "F"
//PORT_DIPSETTING(    0xc0, "30K 100K 200K 400K 600K 1M" )    // "B"
//PORT_DIPSETTING(    0xa0, "40K 100K 180K 300K 500K 1M" )    // "C"
//PORT_DIPSETTING(    0x20, "40K 100K 200K" )                 // "G"
//PORT_DIPSETTING(    0x00, "40K" )                           // "H"
//PORT_DIPSETTING(    0x60, "50K 150K every 200K" )           // "E"
//PORT_DIPNAME( 0x18, 0x18, DEF_STR( Difficulty ) )   PORT_DIPLOCATION("SWB:5,4")
//PORT_DIPSETTING(    0x10, "B (Easy)" )
//PORT_DIPSETTING(    0x18, "A (Average)" )
//PORT_DIPSETTING(    0x08, "C (Hard)" )
//PORT_DIPSETTING(    0x00, "D (Very Hard)" )
//PORT_DIPNAME( 0x04, 0x04, "Round Select" )          PORT_DIPLOCATION("SWB:6")
//PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
//PORT_DIPSETTING(    0x00, DEF_STR( On ) )
//PORT_DIPNAME( 0x02, 0x02, "Freeze" )                PORT_DIPLOCATION("SWB:7")
//PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
//PORT_DIPSETTING(    0x00, DEF_STR( On ) )
//PORT_DIPNAME( 0x01, 0x01, "Trip Select" )           PORT_DIPLOCATION("SWB:8")
//PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
//PORT_DIPSETTING(    0x01, DEF_STR( On ) )
//
//PORT_START("IN0")   /* Memory Mapped Port */
//PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_COCKTAIL  PORT_NAME("P2 Right")
//PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_SERVICE1 )
//PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_COIN1 )
//PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_COIN2 )
//PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_START1 )
//PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_START2 )
//PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_SERVICE )    // service mode again
//PORT_DIPNAME( 0x80, 0x80, DEF_STR( Cabinet ) )
//PORT_DIPSETTING(    0x80, DEF_STR( Upright ) )
//PORT_DIPSETTING(    0x00, DEF_STR( Cocktail ) )
//
//PORT_START("IN1")   /* Memory Mapped Port */
//PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_UNUSED )
//PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_UNUSED )
//PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_UNUSED )
//PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNUSED )
//PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNUSED ) // IPT_JOYSTICK_DOWN according to schematics
//PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNUSED ) // IPT_JOYSTICK_UP according to schematics
//PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNUSED ) // IPT_JOYSTICK_DOWN according to schematics
//PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNUSED ) PORT_COCKTAIL   // IPT_JOYSTICK_UP according to schematics
//
//PORT_START("IN2")   /* MCU Input Port */
//PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_SPECIAL ) PORT_COCKTAIL  /* OUT:coin lockout */
//PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_SPECIAL )    /* OUT:coin counter 1 */
//PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SPECIAL )    /* OUT:coin counter 2 */
//PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("P1 Jump")
//PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("P1 Left")
//PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("P1 Right")
//PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_COCKTAIL PORT_NAME("P2 Jump")
//PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_COCKTAIL PORT_NAME("P2 Left")
//INPUT_PORTS_END
//******** END PACLAND MAME





//
// Output region is the same for all versions.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                                "012", "012345"
                                                        /*{NO_BANK_SWITCH, c_ADCSTART0_A, 0x00,      0x00,  "9K ", "ADCS0 "}, // ADC start channel 0 (pitch,  J)
                                                        {NO_BANK_SWITCH, c_ADCSTART1_A, 0x00,      0x00,  "9K ", "ADCS1 "}, // ADC start channel 0 (yaw,    K)
                                                        {NO_BANK_SWITCH, c_ADCSTART2_A, 0x00,      0x00,  "9K ", "ADCS2 "}, // ADC start channel 0 (thrust, 9)
                                                        {NO_BANK_SWITCH, c_MPAGE_A,     c_MPAGE_D, 0x00,  "9LM", "MPAGE "}, // MPAGE ROM bank switch
                                                        {NO_BANK_SWITCH, c_MW0_A,       0xFF,      0x00,  "   ", "MW0-PA"}, // MW0 - MP Address MPA2-MPA9, run
                                                        {NO_BANK_SWITCH, c_MW1_A,       0x01,      0x00,  "3D ", "MW1-BI"}, // MW1 - MP Block Index BIC8
                                                        {NO_BANK_SWITCH, c_MW2_A,       0xFF,      0x00,  "   ", "MW2-BI"}, // MW2 - MP Block Index BIC0-BIC7
                                                        {NO_BANK_SWITCH, c_DVSRH_A,     0xFF,      0x00,  "45P", "DVSRH "}, // DVSRH - Divisor Hi, Q clear, load div.
                                                        {NO_BANK_SWITCH, c_DVSRL_A,     0xFF,      0x00,  "6PL", "DVSRL "}, // DVSRL - Divisor Lo, start
                                                        {NO_BANK_SWITCH, c_DVDDH_A,     0xFF,      0x00,  "4L ", "DVDDH "}, // DVDDH - Dividend Hi
                                                        {NO_BANK_SWITCH, c_DVDDL_A,     0xFF,      0x00,  "5L ", "DVDDL "}, // DVDDL - Dividend Lo*/
                                                        {0}
                                                      }; // end of list

//
// Custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = { //                                               "0123456789"
                                                            /*{CPaclandBaseGame::testADC,                     "Test ADC  "},*/
                                                            /*{CPaclandBaseGame::test10,                      "MX Test 10"},*/
                                                            /*{CPaclandBaseGame::test11,                      "MX Test 11"},
                                                            {CPaclandBaseGame::test12,                      "MX Test 12"},
                                                            {CPaclandBaseGame::test13,                      "MX Test 13"},
                                                            {CPaclandBaseGame::test14,                      "MX Test 14"},
                                                            {CPaclandBaseGame::test15,                      "MX Test 15"},
                                                            {CPaclandBaseGame::test16,                      "MX Test 16"},
                                                            {CPaclandBaseGame::test17,                      "MX Test 17"},
                                                            {CPaclandBaseGame::test18,                      "MX Test 18"},
                                                            {CPaclandBaseGame::test19,                      "MX Test 19"},
                                                            {CPaclandBaseGame::test20,                      "MX Test 20"},
                                                            {CPaclandBaseGame::test21,                      "DV Test 21"},
                                                            {CPaclandBaseGame::test22,                      "DV Test 22"},
                                                            {CPaclandBaseGame::test23,                      "DV Test 23"},
                                                            {CPaclandBaseGame::test24,                      "DV Test 24"},
                                                            {CPaclandBaseGame::test25,                      "DV Test 25"},
                                                            {CPaclandBaseGame::testRepeatLastMatrixProgram, "Repeat MX "},*/
                                                            {CPaclandBaseGame::testClockPulse,              "Clk Pulse "},
                                                            /*{CPaclandBaseGame::testCapture,                 "Capture   "},*/
                                                            {NO_CUSTOM_FUNCTION}}; // end of list




//Do these params need to be in a particular order?
CPaclandBaseGame::CPaclandBaseGame(
    const ROM_REGION    *romRegion
) : CGame( romRegion,
           s_ramRegion,
           s_ramRegion, // It's all Byte wide
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction ),
    m_clockPulseCount(0),
    m_lastMatrixProgramAddress(0)
{
    m_cpu = new C6809ECpu(0);
    m_cpu->idle();

  /*  // A timer is on the INT pin (vector game thus no VBALNK).
    m_interrupt = ICpu::IRQ0;

    // The interrupt uses an external ROM vector.
    m_interruptAutoVector = false;*/
}


CPaclandBaseGame::~CPaclandBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// The program memory is banked.
//
/*PERROR
CPaclandBaseGame::onBankSwitchMPAGE0(
    void *cPaclandBaseGame
)
{
    CPaclandBaseGame *thisGame  = (CPaclandBaseGame *) cPaclandBaseGame;
    ICpu              *cpu       = thisGame->m_cpu;

    return cpu->memoryWrite(c_MPAGE_A, 0x00);
}


PERROR
CPaclandBaseGame::onBankSwitchMPAGE1(
    void *cPaclandBaseGame
)
{
    CPaclandBaseGame *thisGame  = (CPaclandBaseGame *) cPaclandBaseGame;
    ICpu              *cpu       = thisGame->m_cpu;

    return cpu->memoryWrite(c_MPAGE_A, c_MPAGE_D);
}*/


/*PERROR
CPaclandBaseGame::waitForMathRunLo(
)
{
    PERROR error = errorSuccess;
    UINT16 recData;

    // Poll the math run flag
    for (int x = 0; x < 128 ; x++)
    {
        CHECK_CPU_READ_EXIT(error, m_cpu, c_MBRUN_A, &recData);

        if ((recData & c_MBRUN_D) == 0x00)
        {
            break;
        }
    }

    // Math Run is still active
    if (!((recData & c_MBRUN_D) == 0x00))
    {
        error = errorCustom;
        error->code = ERROR_FAILED;
        error->description = "E: MATH RUN Hi";
    }

Exit:
    return error;
}


PERROR
CPaclandBaseGame::testMatrix(
    void   *context,
    int    srcDataLength,
    UINT32 *srcDataAddress,
    UINT16 *srcData,
    int    expDataLength,
    UINT32 *expDataAddress,
    UINT16 *expData,
    UINT16 programAddress
)
{
    CPaclandBaseGame *thisGame = (CPaclandBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT16 recData;
    UINT16 recResult = 0;

    // Make sure the matrix processor is idle
    error = thisGame->waitForMathRunLo();
    if (FAILED(error))
    {
        goto Exit;
    }

    // Load the source data words
    for (int x = 0 ; x < srcDataLength ; x++)
    {
        CHECK_CPU_WRITE_EXIT(error, cpu, c_MBRAM_A + (srcDataAddress[x] << 1) | 0, (srcData[x] >> 8) & 0xFF);
        CHECK_CPU_WRITE_EXIT(error, cpu, c_MBRAM_A + (srcDataAddress[x] << 1) | 1, (srcData[x] >> 0) & 0xFF);
    }

    // Make sure the result words are set to something so we know something happened
    for (int x = 0 ; x < expDataLength ; x++)
    {
        CHECK_CPU_WRITE_EXIT(error, cpu, c_MBRAM_A + (expDataAddress[x] << 1) | 0, 0x12);
        CHECK_CPU_WRITE_EXIT(error, cpu, c_MBRAM_A + (expDataAddress[x] << 1) | 1, 0x34);
    }

    // Write the program address to start
    CHECK_CPU_WRITE_EXIT(error, cpu, c_MW0_A, (programAddress >> 2) & 0xFF);
    thisGame->m_lastMatrixProgramAddress = programAddress;
    thisGame->m_clockPulseCount = 0;

    // Wait for the matrix processor to become idle again.
    error = thisGame->waitForMathRunLo();
    if (FAILED(error))
    {
        goto Exit;
    }

    // Check all the result words
    for (int x = 0 ; x < expDataLength ; x++)
    {
        recResult = 0;

        CHECK_CPU_READ_EXIT(error, cpu, c_MBRAM_A + (expDataAddress[x] << 1) | 0, &recData);
        recResult |= (recData << 8);
        CHECK_CPU_READ_EXIT(error, cpu, c_MBRAM_A + (expDataAddress[x] << 1) | 1, &recData);
        recResult |= (recData << 0);

        CHECK_UINT16_VALUE_EXIT(error, "MX", recResult, expData[x]);
    }

Exit:
    return error;
}


PERROR
CPaclandBaseGame::testDivider(
    void   *context,
    UINT16 dividend,
    UINT16 divisor,
    UINT16 quotient
)
{
    CPaclandBaseGame *thisGame = (CPaclandBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;
    UINT16 recData;
    UINT16 recQuotient = 0;

    // Load dividend
    CHECK_CPU_WRITE_EXIT(error, cpu, c_DVDDH_A, (dividend >> 8) & 0xFF);
    CHECK_CPU_WRITE_EXIT(error, cpu, c_DVDDL_A, (dividend >> 0) & 0xFF);

    // Load divisor
    CHECK_CPU_WRITE_EXIT(error, cpu, c_DVSRH_A, (divisor >> 8) & 0xFF);
    CHECK_CPU_WRITE_EXIT(error, cpu, c_DVSRL_A, (divisor >> 0) & 0xFF);
    thisGame->m_clockPulseCount = 0;

    // Wait for a few clocks.
    // There is no indication to the CPU that the divide is actually complete,
    // therefore we'll just execute a dummy read to run the master clock.
    // There's a counter at 8R loaded with 0001 that counts up at 3MHz and pops
    // out a ripple signal, thus ~16 3MHz clocks maybe?
    // 16 CPU reads (at 1 x 1.5MHz cycle each) should be more than enough to
    // complete the operation.
    //
    for (int x = 0 ; x < 16 ; x++)
    {
        CHECK_CPU_READ_EXIT(error, cpu, 0xFFFF, &recData);
    }

    // Read quotient
    CHECK_CPU_READ_EXIT(error, cpu, c_REH_A, &recData);
    recQuotient |= (recData << 8);
    CHECK_CPU_READ_EXIT(error, cpu, c_REL_A, &recData);
    recQuotient |= (recData << 0);

    // Check the result is what we expect
    CHECK_UINT16_VALUE_EXIT(error, "DV", recQuotient, quotient);

Exit:
    return error;
}


PERROR
CPaclandBaseGame::testADC(
    void   *context
)
{
    CPaclandBaseGame *thisGame = (CPaclandBaseGame *) context;
    C6809ECpu *cpu = (C6809ECpu *) thisGame->m_cpu;
    PERROR error = errorCustom;
    UINT16 data[4] = {0,0,0,0};

    for (UINT32 channel = 0 ; channel < 4 ; channel++)
    {
        // Start the conversion
        CHECK_CPU_WRITE_EXIT(error, cpu, (c_ADCSTART0_A + channel), 0);

        // Wait for a few clocks.
        // There is no indication to the CPU when the conversion is actually complete.
        // The datasheet states that the maximum conversion time is 116us @ 640KHz so
        // we'll convert 116uS into the equivalent number of 12MHz clock pulses:
        //  12MHz === 83.33ns
        //  116us/83.33ns = 1,392 clock pulses.
        //
        for (int x = 0 ; x < 1392 ; x++)
        {
            cpu->clockPulse();
        }

        CHECK_CPU_READ_EXIT(error, cpu, c_ADC_A, &data[channel]);
    }

    error->code = ERROR_SUCCESS;
    error->description = "OK: ";

    for (int byte = 0 ; byte < 4 ; byte++)
    {
        STRING_UINT8_HEX(error->description, data[byte]);
    }

Exit:

    return error;
}


PERROR
CPaclandBaseGame::test10(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0F  };
    UINT16 srcData[]    = {0x5555};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               0, (UINT32*) NULL, (UINT16*) NULL,
                               0x15C);
}


PERROR
CPaclandBaseGame::test11(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0F  };
    UINT16 srcData[]    = {0xAAAA};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               0, (UINT32*) NULL, (UINT16*) NULL,
                               0x160);
}


PERROR
CPaclandBaseGame::test12(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0F  };
    UINT16 srcData[]    = {0x5555};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               0, (UINT32*) NULL, (UINT16*) NULL,
                               0x164);
}


PERROR
CPaclandBaseGame::test13(
    void   *context
)
{
    return testMatrix(context, 0, (UINT32*) NULL, (UINT16*) NULL,
                               0, (UINT32*) NULL, (UINT16*) NULL,
                               0x168);
}


PERROR
CPaclandBaseGame::test14(
    void   *context
)
{
    return testMatrix(context, 0, (UINT32*) NULL, (UINT16*) NULL,
                               0, (UINT32*) NULL, (UINT16*) NULL,
                               0x16C);
}


PERROR
CPaclandBaseGame::test15(
    void   *context
)
{
    UINT32 srcAddress[] = {0x00  };
    UINT16 srcData[]    = {0x5555};
    UINT32 expAddress[] = {0x01  };
    UINT16 expData[]    = {0x5555};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               ARRAYSIZE(expAddress), expAddress, expData,
                               0x170); // -> 171
}


PERROR
CPaclandBaseGame::test16(
    void   *context
)
{
    UINT32 srcAddress[] = {0x00  };
    UINT16 srcData[]    = {0xAAAA};
    UINT32 expAddress[] = {0x01  };
    UINT16 expData[]    = {0xAAAA};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               ARRAYSIZE(expAddress), expAddress, expData,
                               0x170); // -> 171
}


PERROR
CPaclandBaseGame::test17(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0C,   0x0D,   0x0E  };
    UINT16 srcData[]    = {0x5555, 0x0000, 0x4000};
    UINT32 expAddress[] = {0x00  };
    UINT16 expData[]    = {0x5555};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               ARRAYSIZE(expAddress), expAddress, expData,
                               0x174); // -> 177
}


PERROR
CPaclandBaseGame::test18(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0C,   0x0D,   0x0E  };
    UINT16 srcData[]    = {0x0000, 0x5555, 0xC000};
    UINT32 expAddress[] = {0x00  };
    UINT16 expData[]    = {0x5555};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               ARRAYSIZE(expAddress), expAddress, expData,
                               0x174); // -> 177
}


PERROR
CPaclandBaseGame::test19(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0C,   0x0D,   0x0E  };
    UINT16 srcData[]    = {0x2AAA, 0x0000, 0x4000};
    UINT32 expAddress[] = {0x00  };
    UINT16 expData[]    = {0x2AAA};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               ARRAYSIZE(expAddress), expAddress, expData,
                               0x174); // -> 177
}


PERROR
CPaclandBaseGame::test20(
    void   *context
)
{
    UINT32 srcAddress[] = {0x0C,   0x0D,   0x0E  };
    UINT16 srcData[]    = {0x0000, 0x2AAA, 0xC000};
    UINT32 expAddress[] = {0x00  };
    UINT16 expData[]    = {0x2AAA};

    return testMatrix(context, ARRAYSIZE(srcAddress), srcAddress, srcData,
                               ARRAYSIZE(expAddress), expAddress, expData,
                               0x174); // -> 177
}


PERROR
CPaclandBaseGame::test21(
    void   *context
)
{
    return testDivider(context, 0x4000, 0x4000, 0x4000);
}


PERROR
CPaclandBaseGame::test22(
    void   *context
)
{
    return testDivider(context, 0x5555, 0x4000, 0x5555);
}


PERROR
CPaclandBaseGame::test23(
    void   *context
)
{
    return testDivider(context, 0x2AAA, 0x4000, 0x2AAA);
}


PERROR
CPaclandBaseGame::test24(
    void   *context
)
{
    return testDivider(context, 0x2AAA, 0x2AAA, 0x4000);
}


PERROR
CPaclandBaseGame::test25(
    void   *context
)
{
    return testDivider(context, 0x5555, 0x5555, 0x4000);
}*/


/*PERROR
CPaclandBaseGame::testRepeatLastMatrixProgram(
    void   *context
)
{
    CPaclandBaseGame *thisGame = (CPaclandBaseGame *) context;
    ICpu *cpu = thisGame->m_cpu;
    PERROR error = errorSuccess;

    if (thisGame->m_lastMatrixProgramAddress == 0)
    {
        error = errorUnexpected;
        goto Exit;
    }

    // Write the program address to start
    CHECK_CPU_WRITE_EXIT(error, cpu, c_MW0_A, (thisGame->m_lastMatrixProgramAddress >> 2) & 0xFF);
    thisGame->m_clockPulseCount = 0;

Exit:
    return error;
}*/


PERROR
CPaclandBaseGame::testClockPulse(
    void   *context
)
{
    CPaclandBaseGame *thisGame = (CPaclandBaseGame *) context;
    C6809ECpu *cpu = (C6809ECpu *) thisGame->m_cpu;
    PERROR error = errorCustom;

    cpu->clockPulse();
    thisGame->m_clockPulseCount++;

    errorCustom->code = ERROR_SUCCESS;
    errorCustom->description = "OK: Count ";
    errorCustom->description += String(thisGame->m_clockPulseCount, DEC);

    return error;
}






/*
//
// Prototype signal capture
//
#include "PinMap.h"

//
// External capture input on J14 AUX pin 1.
//
static const CONNECTION s_AUX1_i = {1, "AUX1"};

//
// Prototype Signal Capture
// ------------------------
// Capture the clocked input of aux pin 1 and display the hex result.
// The 4 hex digits covers 32 clocks.
// See PaclandCaptures.txt for reference results.
//

PERROR
CPaclandBaseGame::testCapture(
    void   *context
)
{
    CPaclandBaseGame *thisGame = (CPaclandBaseGame *) context;
    C6809ECpu *cpu = (C6809ECpu *) thisGame->m_cpu;
    PERROR error = errorCustom;
    UINT8 capture[4] = {0,0,0,0};

    ::pinMode(g_pinMap8Aux[s_AUX1_i.pin], INPUT);

    for (int byte = 0 ; byte < 4 ; byte++)
    {
        for (int bit = 0 ; bit < 8 ; bit++)
        {
            cpu->clockPulse();
            thisGame->m_clockPulseCount++;

            int value = ::digitalRead(g_pinMap8Aux[s_AUX1_i.pin]);

            capture[byte] = capture[byte] << 1;
            if (value == HIGH)
            {
                capture[byte] |= 1;
            }
        }
    }

    error->code = ERROR_SUCCESS;
    error->description = "OK: ";

    for (int byte = 0 ; byte < 4 ; byte++)
    {
        STRING_UINT8_HEX(error->description, capture[byte]);
    }

    return error;
}*/


