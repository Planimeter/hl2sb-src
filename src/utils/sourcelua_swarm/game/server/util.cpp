//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Utility code.
//
// $NoKeywords: $
//=============================================================================//

#include "../../sigscan.h"
#include "util.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// returns a CBaseEntity pointer to a player by index.  Only returns if the player is spawned and connected
// otherwise returns NULL
// Index is 1 based
CSigScan sig_UTIL_PlayerByIndex;
CBasePlayer	*UTIL_PlayerByIndex( int playerIndex )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_GLOBAL( sig_UTIL_PlayerByIndex, "UTIL_PlayerByIndex" );
		return NULL;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

#ifdef WIN32
	typedef CBasePlayer	*( __fastcall *pfn )( int ); 
#else
	typedef CBasePlayer	*( *pfn )( int );
#endif
	pfn pUTIL_PlayerByIndex = (pfn)sig_UTIL_PlayerByIndex.sig_addr;
	CBasePlayer *pPlayer = pUTIL_PlayerByIndex( playerIndex );
	return pPlayer;
}

CSigScan sig_UTIL_ClientPrintAll;
void UTIL_ClientPrintAll( int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_GLOBAL( sig_UTIL_ClientPrintAll, "UTIL_ClientPrintAll" );
		return;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

#ifdef WIN32
	typedef void ( __fastcall *pfn )( int, const char * ); 
#else
	typedef void ( *pfn )( int, const char * );
#endif
	pfn pUTIL_ClientPrintAll = (pfn)sig_UTIL_ClientPrintAll.sig_addr;
	pUTIL_ClientPrintAll( msg_dest, msg_name );

	return;
}

CSigScan sig_UTIL_Remove;
void UTIL_Remove( CBaseEntity *oldObj )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_GLOBAL( sig_UTIL_Remove, "UTIL_Remove" );
		return;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

#ifdef WIN32
	typedef void ( __fastcall *pfn )( CBaseEntity * ); 
#else
	typedef void ( *pfn )( CBaseEntity * );
#endif
	pfn pUTIL_Remove = (pfn)sig_UTIL_Remove.sig_addr;
	pUTIL_Remove( oldObj );

	return;
}

//-----------------------------------------------------------------------------
// Purpose: Spawns an entity into the game, initializing it with the map ent data block
// Input  : *pEntity - the newly created entity
//			*mapData - pointer a block of entity map data
// Output : -1 if the entity was not successfully created; 0 on success
//-----------------------------------------------------------------------------
CSigScan sig_DispatchSpawn;
int DispatchSpawn( CBaseEntity *pEntity )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_GLOBAL( sig_DispatchSpawn, "DispatchSpawn" );
		return 0;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

#ifdef WIN32
	typedef int ( __fastcall *pfn )( CBaseEntity * ); 
#else
	typedef int ( *pfn )( CBaseEntity * );
#endif
	pfn pDispatchSpawn = (pfn)sig_DispatchSpawn.sig_addr;
	return pDispatchSpawn( pEntity );
}


void siginit_UTIL( void )
{
	sig_UTIL_PlayerByIndex.Init( (unsigned char*)SIG_UTIL_PLAYERBYINDEX, MASK_UTIL_PLAYERBYINDEX, SIZE_UTIL_PLAYERBYINDEX );
	sig_UTIL_ClientPrintAll.Init( (unsigned char*)SIG_UTIL_CLIENTPRINTALL, MASK_UTIL_CLIENTPRINTALL, SIZE_UTIL_CLIENTPRINTALL );
	sig_UTIL_Remove.Init( (unsigned char*)SIG_UTIL_REMOVE, MASK_UTIL_REMOVE, SIZE_UTIL_REMOVE );
	sig_DispatchSpawn.Init( (unsigned char*)SIG_DISPATCHSPAWN, MASK_DISPATCHSPAWN, SIZE_DISPATCHSPAWN );
	return;
}




