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
#include "CPaclandGame.h"




//NEED TO DISCUSS HOW MAME DATA FOR PACLAND BRIDGES TO THIS FORMAT!!
//
// Default set from MAME.
//
//                                          01   02   04   08   10   20   40   80  100  200  400  800 1000
/*static const UINT16 s_romData2n_1F_Lo[] = {0xf2,0x61,0x56,0xcc,0x3d,0x24,0xad,0x68,0x02,0x02,0x20,0x41,0xfc};
static const UINT16 s_romData2n_1F_Hi[] = {0xf2,0x61,0x56,0x00,0xea,0x30,0x00,0xd0,0xd9,0x00,0x3c,0xfd,0x28};
static const UINT16 s_romData2n_1HJ[]   = {0x00,0x05,0xb0,0x2c,0x0a,0x2c,0x01,0x00,0x02,0x05,0x60,0x7c,0xa7};
static const UINT16 s_romData2n_1JK[]   = {0x38,0x00,0x00,0x38,0x00,0x00,0x50,0x00,0x00,0xb7,0xbd,0xbd,0xa0};
static const UINT16 s_romData2n_1KL[]   = {0xfa,0xa6,0xbd,0x30,0xd7,0x85,0x19,0xfe,0x02,0xfd,0x4c,0x2b,0xbb};
static const UINT16 s_romData2n_1M[]    = {0x42,0x45,0x20,0x43,0x20,0xd3,0x46,0x41,0x41,0x50,0x20,0xea,0x01};*/

/*static const ROM_REGION s_romRegion[] PROGMEM =     {*/ //
                                                      /*{CPaclandBaseGame::onBankSwitchMPAGE0, 0x6000, 0x2000, s_romData2n_1F_Lo,  0xA7ED5F8A, "1F0"}, // 214 - Lo
                                                      {CPaclandBaseGame::onBankSwitchMPAGE1, 0x6000, 0x2000, s_romData2n_1F_Hi,  0xB8123618, "1F1"}, // 214 - Hi
                                                      {NO_BANK_SWITCH,                        0x8000, 0x2000, s_romData2n_1HJ,    0xf725e344, "1HJ"}, // 102
                                                      {NO_BANK_SWITCH,                        0xA000, 0x2000, s_romData2n_1JK,    0xf6da0a00, "1JK"}, // 203
                                                      {NO_BANK_SWITCH,                        0xC000, 0x2000, s_romData2n_1KL,    0x7e406703, "1KL"}, // 104
                                                      {NO_BANK_SWITCH,                        0xE000, 0x2000, s_romData2n_1M,     0xc7e51237, "1M "}, // 206*/
/*                                                      {0} };*/ // end of list

//
// Set "O" from MAME.
//
/*static const ROM_REGION s_romRegionSetO[] PROGMEM = {*/ //
                                                      /*{CPaclandBaseGame::onBankSwitchMPAGE0, 0x6000, 0x2000, s_romData2n_1F_Lo,  0x67CBA67C, "1F0"}, // 114 - Lo
                                                      {CPaclandBaseGame::onBankSwitchMPAGE1, 0x6000, 0x2000, s_romData2n_1F_Hi,  0xB8123618, "1F1"}, // 114 - Hi
                                                      {NO_BANK_SWITCH,                        0x8000, 0x2000, s_romData2n_1HJ,    0xf725e344, "1HJ"}, // 102
                                                      {NO_BANK_SWITCH,                        0xA000, 0x2000, s_romData2n_1JK,    0x3fde9ccb, "1JK"}, // 103
                                                      {NO_BANK_SWITCH,                        0xC000, 0x2000, s_romData2n_1KL,    0x7e406703, "1KL"}, // 104
                                                      {NO_BANK_SWITCH,                        0xE000, 0x2000, s_romData2n_1M,     0xc7e51237, "1M "}, // 206*/
/*                                                      {0} };*/ // end of list




IGame*
CPaclandGame::createInstance(
)
{
    return (new CPaclandGame(/*s_romRegion*/));
}


/*IGame*
CPaclandGame::createInstanceSetO(
)
{
    return (new CPaclandGame(s_romRegionSetO));
}*/

CPaclandGame::CPaclandGame(
    /*const ROM_REGION *romRegion*/
) : CPaclandBaseGame( /*romRegion*/ )
{
}








//TAKEN FROM MAME


//ROM_START( pacland )
//ROM_REGION( 0x20000, "maincpu", 0 )
//ROM_LOAD( "pl5_01b.8b",     0x08000, 0x4000, CRC(b0ea7631) SHA1(424afa6f397310c7af39c9e8b580aa9ccd42c39c) )
//ROM_LOAD( "pl5_02.8d",      0x0C000, 0x4000, CRC(d903e84e) SHA1(25338726227bfbec65847879aac5228a6a307db4) )
///* all the following are banked at 0x4000-0x5fff */
//ROM_LOAD( "pl1_3.8e",       0x10000, 0x4000, CRC(aa9fa739) SHA1(7b1f7857eb5f68e166b1f8988c82051aaf05df48) )
//ROM_LOAD( "pl1_4.8f",       0x14000, 0x4000, CRC(2b895a90) SHA1(820f8873c6a5a736089406d0f03d491dfb82d00d) )
//ROM_LOAD( "pl1_5.8h",       0x18000, 0x4000, CRC(7af66200) SHA1(f44161ded1633e9801b7a9cd84d481e53823f5d9) )
//ROM_LOAD( "pl3_6.8j",       0x1c000, 0x4000, CRC(2ffe3319) SHA1(c2540321cd5a1fe29ecb077abdf8f997893192e9) )
//
//ROM_REGION( 0x10000, "mcu", 0 )
//ROM_LOAD( "pl1_7.3e",       0x8000, 0x2000, CRC(8c5becae) SHA1(14d67136395c4c64472980a69648ce2d479ae67f) ) /* sub program for the mcu */
//ROM_LOAD( "cus60-60a1.mcu", 0xf000, 0x1000, CRC(076ea82a) SHA1(22b5e62e26390d7d5cacc0503c7aa5ed524204df) ) /* Internal code from the MCU */
//
//ROM_REGION( 0x02000, "gfx1", 0 )
//ROM_LOAD( "pl2_12.6n",      0x00000, 0x2000, CRC(a63c8726) SHA1(b15903fa2267375280af03af0a7157e1b0bcb86d) ) /* chars */
//
//ROM_REGION( 0x02000, "gfx2", 0 )
//ROM_LOAD( "pl4_13.6t",      0x00000, 0x2000, CRC(3ae582fd) SHA1(696b2cfadb6b071de8e43d20cd65b37713ca3b30) )
//
//ROM_REGION( 0x10000, "gfx3", 0 )
//ROM_LOAD( "pl1-9.6f",       0x00000, 0x4000, CRC(f5d5962b) SHA1(8d008a9bc06dc562c241955d9c551647b5c1f4e9) ) /* sprites */
//ROM_LOAD( "pl1-8.6e",       0x04000, 0x4000, CRC(a2ebfa4a) SHA1(4a2a2b43a23a7a46266751415d1bde118143429c) )
//ROM_LOAD( "pl1-10.7e",      0x08000, 0x4000, CRC(c7cf1904) SHA1(7ca8ed20ee32eb8609ac96b4e4fcb3b6027b598a) )
//ROM_LOAD( "pl1-11.7f",      0x0c000, 0x4000, CRC(6621361a) SHA1(4efa40adba803006e86d5e12514983d4132b5efb) )
//
//ROM_REGION( 0x1400, "proms", 0 )
//ROM_LOAD( "pl1-2.1t",       0x0000, 0x0400, CRC(472885de) SHA1(8d552c90b8d5bc6ad6c60934c00f4303cd180ce7) )  /* red and green component */
//ROM_LOAD( "pl1-1.1r",       0x0400, 0x0400, CRC(a78ebdaf) SHA1(8ea215701eb5e1a2a329ef92c19fc69b18fc28c7) )  /* blue component */
//ROM_LOAD( "pl1-5.5t",       0x0800, 0x0400, CRC(4b7ee712) SHA1(dd0ec4c632d8b160f7b54d8f18fcf4ef1508d832) )  /* foreground lookup table */
//ROM_LOAD( "pl1-4.4n",       0x0c00, 0x0400, CRC(3a7be418) SHA1(475cdc68205e3acce83fe79b00b74c6a7e28dde4) )  /* background lookup table */
//ROM_LOAD( "pl1-3.6l",       0x1000, 0x0400, CRC(80558da8) SHA1(7e1483467817295f36d1e2bdb32934c4f2617d52) )  /* sprites lookup table */
//ROM_END
//
//ROM_START( paclandj )
//ROM_REGION( 0x20000, "maincpu", 0 )
//ROM_LOAD( "pl6_01.8b",      0x08000, 0x4000, CRC(4c96e11c) SHA1(c136dc3877155b7a600173c876f6a53394d9260d) )
//ROM_LOAD( "pl6_02.8d",      0x0C000, 0x4000, CRC(8cf5bd8d) SHA1(0771ca1ab5db58f5632583a5e6e84660e8ab727d) )
///* all the following are banked at 0x4000-0x5fff */
//ROM_LOAD( "pl1_3.8e",       0x10000, 0x4000, CRC(aa9fa739) SHA1(7b1f7857eb5f68e166b1f8988c82051aaf05df48) )
//ROM_LOAD( "pl1_4.8f",       0x14000, 0x4000, CRC(2b895a90) SHA1(820f8873c6a5a736089406d0f03d491dfb82d00d) )
//ROM_LOAD( "pl1_5.8h",       0x18000, 0x4000, CRC(7af66200) SHA1(f44161ded1633e9801b7a9cd84d481e53823f5d9) )
//ROM_LOAD( "pl1_6.8j",       0x1c000, 0x4000, CRC(b01e59a9) SHA1(e5b093852d33a4d09969d111fa6e42e964aa4dac) )
//
//ROM_REGION( 0x10000, "mcu", 0 )
//ROM_LOAD( "pl1_7.3e",       0x8000, 0x2000, CRC(8c5becae) SHA1(14d67136395c4c64472980a69648ce2d479ae67f) ) /* sub program for the mcu */
//ROM_LOAD( "cus60-60a1.mcu", 0xf000, 0x1000, CRC(076ea82a) SHA1(22b5e62e26390d7d5cacc0503c7aa5ed524204df) ) /* Internal code from the MCU */
//
//ROM_REGION( 0x02000, "gfx1", 0 )
//ROM_LOAD( "pl6_12.6n",      0x00000, 0x2000, CRC(c8cb61ab) SHA1(ec33d64949a8c011430e889f55f54816b33c4218) ) /* chars */
//
//ROM_REGION( 0x02000, "gfx2", 0 )
//ROM_LOAD( "pl1_13.6t",      0x00000, 0x2000, CRC(6c5ed9ae) SHA1(db919c9254289179e98ba5d2ed8c66d67ae95f35) )
//
//ROM_REGION( 0x10000, "gfx3", 0 )
//ROM_LOAD( "pl1_9b.6f",      0x00000, 0x4000, CRC(80768a87) SHA1(1572f309e810d9eb007a1c8b2aa8463027c146ca) ) /* sprites */
//ROM_LOAD( "pl1_8.6e",       0x04000, 0x4000, CRC(2b20e46d) SHA1(9f78952ae94fef6a83a15de35d5fefdf71e78488) )
//ROM_LOAD( "pl1_10b.7e",     0x08000, 0x4000, CRC(ffd9d66e) SHA1(9a6e9ad500fcb7a67cb3c45d029c2aa7636a64f9) )
//ROM_LOAD( "pl1_11.7f",      0x0c000, 0x4000, CRC(c59775d8) SHA1(034281c8101719d79043df31ef845fd28c0c69c0) )
//
//ROM_REGION( 0x1400, "proms", 0 )
//ROM_LOAD( "pl1-2.1t",       0x0000, 0x0400, CRC(472885de) SHA1(8d552c90b8d5bc6ad6c60934c00f4303cd180ce7) )  /* red and green component */
//ROM_LOAD( "pl1-1.1r",       0x0400, 0x0400, CRC(a78ebdaf) SHA1(8ea215701eb5e1a2a329ef92c19fc69b18fc28c7) )  /* blue component */
//ROM_LOAD( "pl1-5.5t",       0x0800, 0x0400, CRC(4b7ee712) SHA1(dd0ec4c632d8b160f7b54d8f18fcf4ef1508d832) )  /* foreground lookup table */
//ROM_LOAD( "pl1-4.4n",       0x0c00, 0x0400, CRC(3a7be418) SHA1(475cdc68205e3acce83fe79b00b74c6a7e28dde4) )  /* background lookup table */
//ROM_LOAD( "pl1-3.6l",       0x1000, 0x0400, CRC(80558da8) SHA1(7e1483467817295f36d1e2bdb32934c4f2617d52) )  /* sprites lookup table */
//ROM_END
//
//ROM_START( paclandjo )
//ROM_REGION( 0x20000, "maincpu", 0 )
//ROM_LOAD( "pl1_1.8b",       0x08000, 0x4000, CRC(f729fb94) SHA1(332ff2e4aae67eb8ed0f52048097f74323a176f8) )
//ROM_LOAD( "pl1_2.8d",       0x0C000, 0x4000, CRC(5c66eb6f) SHA1(376233f51e655df8922886c1e808a2f37ccae5d4) )
///* all the following are banked at 0x4000-0x5fff */
//ROM_LOAD( "pl1_3.8e",       0x10000, 0x4000, CRC(aa9fa739) SHA1(7b1f7857eb5f68e166b1f8988c82051aaf05df48) )
//ROM_LOAD( "pl1_4.8f",       0x14000, 0x4000, CRC(2b895a90) SHA1(820f8873c6a5a736089406d0f03d491dfb82d00d) )
//ROM_LOAD( "pl1_5.8h",       0x18000, 0x4000, CRC(7af66200) SHA1(f44161ded1633e9801b7a9cd84d481e53823f5d9) )
//ROM_LOAD( "pl1_6.8j",       0x1c000, 0x4000, CRC(b01e59a9) SHA1(e5b093852d33a4d09969d111fa6e42e964aa4dac) )
//
//ROM_REGION( 0x10000, "mcu", 0 )
//ROM_LOAD( "pl1_7.3e",       0x8000, 0x2000, CRC(8c5becae) SHA1(14d67136395c4c64472980a69648ce2d479ae67f) ) /* sub program for the mcu */
//ROM_LOAD( "cus60-60a1.mcu", 0xf000, 0x1000, CRC(076ea82a) SHA1(22b5e62e26390d7d5cacc0503c7aa5ed524204df) ) /* Internal code from the MCU */
//
//ROM_REGION( 0x02000, "gfx1", 0 )
//ROM_LOAD( "pl1_12.6n",      0x00000, 0x2000, CRC(c159fbce) SHA1(b0326c85b7df407f3e94c38a5971f911968d7b27) ) /* chars */
//
//ROM_REGION( 0x02000, "gfx2", 0 )
//ROM_LOAD( "pl1_13.6t",      0x00000, 0x2000, CRC(6c5ed9ae) SHA1(db919c9254289179e98ba5d2ed8c66d67ae95f35) )
//
//ROM_REGION( 0x10000, "gfx3", 0 )
//ROM_LOAD( "pl1_9b.6f",      0x00000, 0x4000, CRC(80768a87) SHA1(1572f309e810d9eb007a1c8b2aa8463027c146ca) ) /* sprites */
//ROM_LOAD( "pl1_8.6e",       0x04000, 0x4000, CRC(2b20e46d) SHA1(9f78952ae94fef6a83a15de35d5fefdf71e78488) )
//ROM_LOAD( "pl1_10b.7e",     0x08000, 0x4000, CRC(ffd9d66e) SHA1(9a6e9ad500fcb7a67cb3c45d029c2aa7636a64f9) )
//ROM_LOAD( "pl1_11.7f",      0x0c000, 0x4000, CRC(c59775d8) SHA1(034281c8101719d79043df31ef845fd28c0c69c0) )
//
//ROM_REGION( 0x1400, "proms", 0 )
//ROM_LOAD( "pl1-2.1t",       0x0000, 0x0400, CRC(472885de) SHA1(8d552c90b8d5bc6ad6c60934c00f4303cd180ce7) )  /* red and green component */
//ROM_LOAD( "pl1-1.1r",       0x0400, 0x0400, CRC(a78ebdaf) SHA1(8ea215701eb5e1a2a329ef92c19fc69b18fc28c7) )  /* blue component */
//ROM_LOAD( "pl1-5.5t",       0x0800, 0x0400, CRC(4b7ee712) SHA1(dd0ec4c632d8b160f7b54d8f18fcf4ef1508d832) )  /* foreground lookup table */
//ROM_LOAD( "pl1-4.4n",       0x0c00, 0x0400, CRC(3a7be418) SHA1(475cdc68205e3acce83fe79b00b74c6a7e28dde4) )  /* background lookup table */
//ROM_LOAD( "pl1-3.6l",       0x1000, 0x0400, CRC(80558da8) SHA1(7e1483467817295f36d1e2bdb32934c4f2617d52) )  /* sprites lookup table */
//ROM_END
//
//ROM_START( paclandjo2 )
//ROM_REGION( 0x20000, "maincpu", 0 )
//ROM_LOAD( "pl1_1.8b",       0x08000, 0x4000, CRC(f729fb94) SHA1(332ff2e4aae67eb8ed0f52048097f74323a176f8) )
//ROM_LOAD( "pl1_2.8d",       0x0C000, 0x4000, CRC(5c66eb6f) SHA1(376233f51e655df8922886c1e808a2f37ccae5d4) )
///* all the following are banked at 0x4000-0x5fff */
//ROM_LOAD( "pl1_3.8e",       0x10000, 0x4000, CRC(aa9fa739) SHA1(7b1f7857eb5f68e166b1f8988c82051aaf05df48) )
//ROM_LOAD( "pl1_4.8f",       0x14000, 0x4000, CRC(2b895a90) SHA1(820f8873c6a5a736089406d0f03d491dfb82d00d) )
//ROM_LOAD( "pl1_5.8h",       0x18000, 0x4000, CRC(7af66200) SHA1(f44161ded1633e9801b7a9cd84d481e53823f5d9) )
//ROM_LOAD( "pl1_6.8j",       0x1c000, 0x4000, CRC(b01e59a9) SHA1(e5b093852d33a4d09969d111fa6e42e964aa4dac) )
//
//ROM_REGION( 0x10000, "mcu", 0 )
//ROM_LOAD( "pl1_7.3e",       0x8000, 0x2000, CRC(8c5becae) SHA1(14d67136395c4c64472980a69648ce2d479ae67f) ) /* sub program for the mcu */
//ROM_LOAD( "cus60-60a1.mcu", 0xf000, 0x1000, CRC(076ea82a) SHA1(22b5e62e26390d7d5cacc0503c7aa5ed524204df) ) /* Internal code from the MCU */
//
//ROM_REGION( 0x02000, "gfx1", 0 )
//ROM_LOAD( "pl1_12.6n",      0x00000, 0x2000, CRC(c159fbce) SHA1(b0326c85b7df407f3e94c38a5971f911968d7b27) ) /* chars */
//
//ROM_REGION( 0x02000, "gfx2", 0 )
//ROM_LOAD( "pl1_13.6t",      0x00000, 0x2000, CRC(6c5ed9ae) SHA1(db919c9254289179e98ba5d2ed8c66d67ae95f35) )
//
//ROM_REGION( 0x10000, "gfx3", 0 )
//ROM_LOAD( "pl1_9.6f",       0x00000, 0x4000, CRC(80768a87) SHA1(1572f309e810d9eb007a1c8b2aa8463027c146ca) ) /* sprites */
//ROM_LOAD( "pl1_8.6e",       0x04000, 0x4000, CRC(2b20e46d) SHA1(9f78952ae94fef6a83a15de35d5fefdf71e78488) )
//ROM_LOAD( "pl1_10.7e",      0x08000, 0x4000, CRC(c62660e8) SHA1(ff922c26f32264b35fa2b07c64097a331437dd64) )
//ROM_LOAD( "pl1_11.7f",      0x0c000, 0x4000, CRC(c59775d8) SHA1(034281c8101719d79043df31ef845fd28c0c69c0) )
//
//ROM_REGION( 0x1400, "proms", 0 )
//ROM_LOAD( "pl1-2.1t",       0x0000, 0x0400, CRC(472885de) SHA1(8d552c90b8d5bc6ad6c60934c00f4303cd180ce7) )  /* red and green component */
//ROM_LOAD( "pl1-1.1r",       0x0400, 0x0400, CRC(a78ebdaf) SHA1(8ea215701eb5e1a2a329ef92c19fc69b18fc28c7) )  /* blue component */
//ROM_LOAD( "pl1-5.5t",       0x0800, 0x0400, CRC(4b7ee712) SHA1(dd0ec4c632d8b160f7b54d8f18fcf4ef1508d832) )  /* foreground lookup table */
//ROM_LOAD( "pl1-4.4n",       0x0c00, 0x0400, CRC(3a7be418) SHA1(475cdc68205e3acce83fe79b00b74c6a7e28dde4) )  /* background lookup table */
//ROM_LOAD( "pl1-3.6l",       0x1000, 0x0400, CRC(80558da8) SHA1(7e1483467817295f36d1e2bdb32934c4f2617d52) )  /* sprites lookup table */
//ROM_END
//
//ROM_START( paclandm )
//ROM_REGION( 0x20000, "maincpu", 0 )
//ROM_LOAD( "pl1-1",          0x08000, 0x4000, CRC(a938ae99) SHA1(bf12097d8c69685cb7af763f9b9617c767aaed2f) )
//ROM_LOAD( "pl1-2",          0x0C000, 0x4000, CRC(3fe43bb5) SHA1(14e6144d06ff2fd786f383f36f1b8238ac364849) )
///* all the following are banked at 0x4000-0x5fff */
//ROM_LOAD( "pl1_3.8e",       0x10000, 0x4000, CRC(aa9fa739) SHA1(7b1f7857eb5f68e166b1f8988c82051aaf05df48) )
//ROM_LOAD( "pl1_4.8f",       0x14000, 0x4000, CRC(2b895a90) SHA1(820f8873c6a5a736089406d0f03d491dfb82d00d) )
//ROM_LOAD( "pl1_5.8h",       0x18000, 0x4000, CRC(7af66200) SHA1(f44161ded1633e9801b7a9cd84d481e53823f5d9) )
//ROM_LOAD( "pl1_6.8j",       0x1c000, 0x4000, CRC(b01e59a9) SHA1(e5b093852d33a4d09969d111fa6e42e964aa4dac) )
//
//ROM_REGION( 0x10000, "mcu", 0 )
//ROM_LOAD( "pl1_7.3e",       0x8000, 0x2000, CRC(8c5becae) SHA1(14d67136395c4c64472980a69648ce2d479ae67f) ) /* sub program for the mcu */
//ROM_LOAD( "cus60-60a1.mcu", 0xf000, 0x1000, CRC(076ea82a) SHA1(22b5e62e26390d7d5cacc0503c7aa5ed524204df) ) /* Internal code from the MCU */
//
//ROM_REGION( 0x02000, "gfx1", 0 )
//ROM_LOAD( "pl1_12.6n",      0x00000, 0x2000, CRC(c159fbce) SHA1(b0326c85b7df407f3e94c38a5971f911968d7b27) ) /* chars */
//
//ROM_REGION( 0x02000, "gfx2", 0 )
//ROM_LOAD( "pl1_13.6t",      0x00000, 0x2000, CRC(6c5ed9ae) SHA1(db919c9254289179e98ba5d2ed8c66d67ae95f35) )
//
//ROM_REGION( 0x10000, "gfx3", 0 )
//ROM_LOAD( "pl1-9.6f",       0x00000, 0x4000, CRC(f5d5962b) SHA1(8d008a9bc06dc562c241955d9c551647b5c1f4e9) ) /* sprites */
//ROM_LOAD( "pl1-8.6e",       0x04000, 0x4000, CRC(a2ebfa4a) SHA1(4a2a2b43a23a7a46266751415d1bde118143429c) )
//ROM_LOAD( "pl1-10.7e",      0x08000, 0x4000, CRC(c7cf1904) SHA1(7ca8ed20ee32eb8609ac96b4e4fcb3b6027b598a) )
//ROM_LOAD( "pl1-11.7f",      0x0c000, 0x4000, CRC(6621361a) SHA1(4efa40adba803006e86d5e12514983d4132b5efb) )
//
//ROM_REGION( 0x1400, "proms", 0 )
//ROM_LOAD( "pl1-2.1t",       0x0000, 0x0400, CRC(472885de) SHA1(8d552c90b8d5bc6ad6c60934c00f4303cd180ce7) )  /* red and green component */
//ROM_LOAD( "pl1-1.1r",       0x0400, 0x0400, CRC(a78ebdaf) SHA1(8ea215701eb5e1a2a329ef92c19fc69b18fc28c7) )  /* blue component */
//ROM_LOAD( "pl1-5.5t",       0x0800, 0x0400, CRC(4b7ee712) SHA1(dd0ec4c632d8b160f7b54d8f18fcf4ef1508d832) )  /* foreground lookup table */
//ROM_LOAD( "pl1-4.4n",       0x0c00, 0x0400, CRC(3a7be418) SHA1(475cdc68205e3acce83fe79b00b74c6a7e28dde4) )  /* background lookup table */
//ROM_LOAD( "pl1-3.6l",       0x1000, 0x0400, CRC(80558da8) SHA1(7e1483467817295f36d1e2bdb32934c4f2617d52) )  /* sprites lookup table */
//ROM_END

