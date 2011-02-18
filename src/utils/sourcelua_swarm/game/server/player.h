//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#ifndef PLAYER_H
#define PLAYER_H
#ifdef _WIN32
#pragma once
#endif

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class CBaseEntity;


class CBasePlayer;
#define SIZE_CBASEPLAYER_GETUSERID	18
#define SIG_CBASEPLAYER_GETUSERID	"\x8B\x41\x18\x8D\x0D\x68\x39\x5D\x10\x8B\x11\x50\x8B\x42\x3C\xFF\xD0\xC3"
#define MASK_CBASEPLAYER_GETUSERID	"xxx??????xxxxxxxxx"

int						CBasePlayer_GetUserID( CBasePlayer *pPlayer );


#define SIZE_CBASEPLAYER_GIVENAMEDITEM	32
#define SIG_CBASEPLAYER_GIVENAMEDITEM	"\x53\x55\x8B\x6C\x24\x10\x56\x8B\x74\x24\x10\x55\x56\x8B\xD9\xE8\x1C\x33\xEF\xFF" \
										"\x85\xC0\x74\x08\x5E\x5D\x33\xC0\x5B\xC2\x08\x00"
#define MASK_CBASEPLAYER_GIVENAMEDITEM	"xxxxxxxxxxxxxxx?????xxxxxxxxxxxx"

CBaseEntity				*CBasePlayer_GiveNamedItem( CBasePlayer *pPlayer, const char *pszName, int iSubType = 0 );

void siginit_CBasePlayer( void );

#endif // PLAYER_H
