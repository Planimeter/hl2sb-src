//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Functions dealing with the player.
//
//===========================================================================//

#include "../../sigscan.h"
#include "player.h"

CSigScan sig_CBasePlayer_GetUserID;
int	CBasePlayer_GetUserID( CBasePlayer *pPlayer )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_METHOD( sig_CBasePlayer_GetUserID, "GetUserID" );
		return -1;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

    union {
        int	( CEmpty::*m_pfnMethod )();
        void *addr;
    } u;
    u.addr = sig_CBasePlayer_GetUserID.sig_addr;
 
	return (reinterpret_cast<CEmpty*>(pPlayer)->*u.m_pfnMethod)();
}


//==============================================

//-----------------------------------------------------------------------------
// Purpose: Create and give the named item to the player. Then return it.
//-----------------------------------------------------------------------------
CSigScan sig_CBasePlayer_GiveNamedItem;
CBaseEntity	*CBasePlayer_GiveNamedItem( CBasePlayer *pPlayer, const char *pszName, int iSubType )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_METHOD( sig_CBasePlayer_GiveNamedItem, "GiveNamedItem" );
		return NULL;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

    union {
        CBaseEntity	*( CEmpty::*m_pfnMethod )( const char *, int );
        void *addr;
    } u;
    u.addr = sig_CBasePlayer_GiveNamedItem.sig_addr;
 
	return (reinterpret_cast<CEmpty*>(pPlayer)->*u.m_pfnMethod)( pszName, iSubType );
}


void siginit_CBasePlayer( void )
{
	sig_CBasePlayer_GetUserID.Init( (unsigned char*)SIG_CBASEPLAYER_GETUSERID, MASK_CBASEPLAYER_GETUSERID, SIZE_CBASEPLAYER_GETUSERID );
	sig_CBasePlayer_GiveNamedItem.Init( (unsigned char*)SIG_CBASEPLAYER_GIVENAMEDITEM, MASK_CBASEPLAYER_GIVENAMEDITEM, SIZE_CBASEPLAYER_GIVENAMEDITEM );
	return;
}
