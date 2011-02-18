//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "../../sigscan.h"
#include "basecombatcharacter.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: 
// Output : CBaseCombatWeapon
//-----------------------------------------------------------------------------
CSigScan sig_CBaseCombatCharacter_GetActiveWeapon;
CBaseCombatWeapon *CBaseCombatCharacter_GetActiveWeapon( CBaseCombatCharacter *pPlayer )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_METHOD( sig_CBaseCombatCharacter_GetActiveWeapon, "GetActiveWeapon" );
		return NULL;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

    union {
        CBaseCombatWeapon *( CEmpty::*m_pfnMethod )();
        void *addr;
    } u;
    u.addr = sig_CBaseCombatCharacter_GetActiveWeapon.sig_addr;
 
	return (reinterpret_cast<CEmpty*>(pPlayer)->*u.m_pfnMethod)();
}


void siginit_CBaseCombatCharacter( void )
{
	sig_CBaseCombatCharacter_GetActiveWeapon.Init( (unsigned char*)SIG_CBASECOMBATCHARACTER_GETACTIVEWEAPON, MASK_CBASECOMBATCHARACTER_GETACTIVEWEAPON, SIZE_CBASECOMBATCHARACTER_GETACTIVEWEAPON );
	return;
}



