//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "../../sigscan.h"

#include "baseentity_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CSigScan sig_CBaseEntity_IsAlive;
bool CBaseEntity_IsAlive( CBaseEntity *pEntity )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_METHOD( sig_CBaseEntity_IsAlive, "IsAlive" );
		return false;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

    union {
        bool ( CEmpty::*m_pfnMethod )();
        void *addr;
    } u;
    u.addr = sig_CBaseEntity_IsAlive.sig_addr;
 
	return (reinterpret_cast<CEmpty*>(pEntity)->*u.m_pfnMethod)();
}


void siginit_CBaseEntity_shared( void )
{
	sig_CBaseEntity_IsAlive.Init( (unsigned char*)SIG_CBASEENTITY_ISALIVE, MASK_CBASEENTITY_ISALIVE, SIZE_CBASEENTITY_ISALIVE );
	return;
}
