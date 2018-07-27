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
#include "CGameCallback.h"


static const SELECTOR s_selectorGame[] = { //"0123456789abcdef"
                                            {"Bus Idle",        CGameCallback::onSelectBusIdle,        (void*) &CGameCallback::game, false},
                                            {"Bus Check",       CGameCallback::onSelectBusCheck,       (void*) &CGameCallback::game, false},
                                            {"ROM Check All",   CGameCallback::onSelectRomCheckAll,    (void*) &CGameCallback::game, false},
                                            {"RAM Check All",   CGameCallback::onSelectRamCheckAll,    (void*) &CGameCallback::game, false},
                                            {"RAM Check All RA",CGameCallback::onSelectRamCheckAllRA,  (void*) &CGameCallback::game, false},
                                            {"RAM Check All CS",CGameCallback::onSelectRamCheckAllCS,  (void*) &CGameCallback::game, false},
                                            {"Interupt Check",  CGameCallback::onSelectInterruptCheck, (void*) &CGameCallback::game, false},
                                            {"Input Read",      CGameCallback::onSelectInputRead,      (void*) &CGameCallback::game, true},
                                            {"Output Write",    CGameCallback::onSelectOutputWrite,    (void*) &CGameCallback::game, true},
                                            {"ROM Check",       CGameCallback::onSelectRomCheck,       (void*) &CGameCallback::game, true},
                                            {"ROM CRC",         CGameCallback::onSelectRomCrc,         (void*) &CGameCallback::game, true},
                                            {"ROM Read",        CGameCallback::onSelectRomRead,        (void*) &CGameCallback::game, true},
                                            {"RAM Check",       CGameCallback::onSelectRamCheck,       (void*) &CGameCallback::game, true},
                                            {"RAM Check RA",    CGameCallback::onSelectRamCheckRA,     (void*) &CGameCallback::game, true},
                                            {"RAM Check Ad",    CGameCallback::onSelectRamCheckAd,     (void*) &CGameCallback::game, true},
                                            {"RAM Write-Read",  CGameCallback::onSelectRamWriteRead,   (void*) &CGameCallback::game, true},
                                            {"ROM Read All",    CGameCallback::onSelectRomReadAll,     (void*) &CGameCallback::game, false},
                                            {"RAM Write All AD",CGameCallback::onSelectRamWriteAllAD,  (void*) &CGameCallback::game, false},
                                            {"RAM Write All Lo",CGameCallback::onSelectRamWriteAllLo,  (void*) &CGameCallback::game, false},
                                            {"RAM Write All Hi",CGameCallback::onSelectRamWriteAllHi,  (void*) &CGameCallback::game, false},
                                            {"RAM Read All",    CGameCallback::onSelectRamReadAll,     (void*) &CGameCallback::game, false},
                                            {"Custom",          CGameCallback::onSelectCustom,         (void*) &CGameCallback::game, true},
                                            { 0, 0 }
                                         };

const SELECTOR *CGameCallback::selectorGame = s_selectorGame;

static const SELECTOR s_selectorGeneric[] = { //"0123456789abcdef"
                                               {"Bus Idle",        CGameCallback::onSelectBusIdle,        (void*) &CGameCallback::game, false},
                                               {"Bus Check",       CGameCallback::onSelectBusCheck,       (void*) &CGameCallback::game, false},
                                               {"ROM CRC",         CGameCallback::onSelectRomCrc,         (void*) &CGameCallback::game, true},
                                               {"ROM Read",        CGameCallback::onSelectRomRead,        (void*) &CGameCallback::game, true},
                                               {"RAM Check",       CGameCallback::onSelectRamCheck,       (void*) &CGameCallback::game, true},
                                               {"RAM Check RA",    CGameCallback::onSelectRamCheckRA,     (void*) &CGameCallback::game, true},
                                               {"RAM Check Ad",    CGameCallback::onSelectRamCheckAd,     (void*) &CGameCallback::game, true},
                                               {"RAM Write-Read",  CGameCallback::onSelectRamWriteRead,   (void*) &CGameCallback::game, true},
                                               { 0, 0 }
                                            };

const SELECTOR *CGameCallback::selectorGeneric = s_selectorGeneric;

static const SELECTOR s_selectorSoakTest[] = { //"0123456789abcdef"
                                                {"ROM Check All",   CGameCallback::onSelectRomCheckAll,    (void*) &CGameCallback::game, false},
                                                {"RAM Check All",   CGameCallback::onSelectRamCheckAll,    (void*) &CGameCallback::game, false},
                                                {"RAM Check All RA",CGameCallback::onSelectRamCheckAllRA,  (void*) &CGameCallback::game, false},
                                                {"RAM Check All CS",CGameCallback::onSelectRamCheckAllCS,  (void*) &CGameCallback::game, false},
                                                {"Interupt Check",  CGameCallback::onSelectInterruptCheck, (void*) &CGameCallback::game, false},
                                                {"RAM Write All AD",CGameCallback::onSelectRamWriteAllAD,  (void*) &CGameCallback::game, false},
                                                {"RAM Write All Lo",CGameCallback::onSelectRamWriteAllLo,  (void*) &CGameCallback::game, false},
                                                {"RAM Write All Hi",CGameCallback::onSelectRamWriteAllHi,  (void*) &CGameCallback::game, false},
                                                { 0, 0 }
                                             };

const SELECTOR *CGameCallback::selectorSoakTest = s_selectorSoakTest;

IGame *CGameCallback::game;


PERROR
CGameCallback::onSelectBusIdle(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->busIdle();
}

PERROR
CGameCallback::onSelectBusCheck(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->busCheck();
}

PERROR
CGameCallback::onSelectRomCheckAll(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->romCheckAll();
}

PERROR
CGameCallback::onSelectRamCheckAll(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->ramCheckAll();
}


PERROR
CGameCallback::onSelectRamCheckAllCS(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->ramCheckAllChipSelect();
}

PERROR
CGameCallback::onSelectRamCheckAllRA(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->ramCheckAllRandomAccess();
}

PERROR
CGameCallback::onSelectInterruptCheck(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->interruptCheck();
}

PERROR
CGameCallback::onSelectInputRead(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->inputRead( key );
}

PERROR
CGameCallback::onSelectOutputWrite(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->outputWrite( key );
}

PERROR
CGameCallback::onSelectRomCheck(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->romCheck( key );
}

PERROR
CGameCallback::onSelectRomCrc(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->romCrc( key );
}

PERROR
CGameCallback::onSelectRomRead(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->romRead( key );
}

PERROR
CGameCallback::onSelectRamCheck(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->ramCheck( key );
}

PERROR
CGameCallback::onSelectRamCheckRA(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->ramCheckRandomAccess( key );
}

PERROR
CGameCallback::onSelectRamCheckAd(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->ramCheckAddress( key );
}

PERROR
CGameCallback::onSelectRamWriteRead(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->ramWriteRead( key );
}

PERROR
CGameCallback::onSelectRomReadAll(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->romReadAll( key );
}

PERROR
CGameCallback::onSelectRamWriteAllAD(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->ramWriteAllAD( key );
}

PERROR
CGameCallback::onSelectRamWriteAllLo(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->ramWriteAllLo( key );
}

PERROR
CGameCallback::onSelectRamWriteAllHi(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->ramWriteAllHi( key );
}

PERROR
CGameCallback::onSelectRamReadAll(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->ramReadAll( key );
}

PERROR
CGameCallback::onSelectCustom(
    void *iGame,
    int  key
)
{
    IGame *game = *((IGame **) iGame);

    return game->custom( key );
}

