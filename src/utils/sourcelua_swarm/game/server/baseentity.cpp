//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: The base class from which all game entities are derived.
//
//===========================================================================//

#include "../../sigscan.h"
#include "baseentity.h"
#include "mathlib/vector.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CSigScan sig_CBaseEntity_Instance;
CBaseEntity* CBaseEntity_Instance( edict_t *pent )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_METHOD( sig_CBaseEntity_Instance, "Instance" );
		return NULL;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

#ifdef WIN32
	typedef CBaseEntity* ( __fastcall *pfn )( edict_t * ); 
#else
	typedef CBaseEntity* ( *pfn )( edict_t * );
#endif
	pfn Instance = (pfn)sig_CBaseEntity_Instance.sig_addr;
	return Instance( pent );
}

//-----------------------------------------------------------------------------
// Purpose: Sets the model, validates that it's of the appropriate type
// Input  : *szModelName - 
//-----------------------------------------------------------------------------
CSigScan sig_CBaseEntity_SetModel;
void CBaseEntity_SetModel( CBaseEntity *pEntity, const char *szModelName )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_METHOD( sig_CBaseEntity_SetModel, "SetModel" );
		return;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

    union {
        void ( CEmpty::*m_pfnMethod )( const char * );
        void *addr;
    } u;
    u.addr = sig_CBaseEntity_SetModel.sig_addr;
 
	(reinterpret_cast<CEmpty*>(pEntity)->*u.m_pfnMethod)( szModelName );

	return;
}


//-----------------------------------------------------------------------------
// These methods recompute local versions as well as set abs versions
//-----------------------------------------------------------------------------
CSigScan sig_CBaseEntity_SetAbsOrigin;
void CBaseEntity_SetAbsOrigin( CBaseEntity *pEntity, const Vector& absOrigin )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_METHOD( sig_CBaseEntity_SetAbsOrigin, "SetAbsOrigin" );
		return;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

    union {
        void ( CEmpty::*m_pfnMethod )( const Vector& );
        void *addr;
    } u;
    u.addr = sig_CBaseEntity_SetAbsOrigin.sig_addr;
 
	(reinterpret_cast<CEmpty*>(pEntity)->*u.m_pfnMethod)( absOrigin );

	return;
}


void siginit_CBaseEntity( void )
{
	sig_CBaseEntity_Instance.Init( (unsigned char*)SIG_CBASEENTITY_INSTANCE, MASK_CBASEENTITY_INSTANCE, SIZE_CBASEENTITY_INSTANCE );
	sig_CBaseEntity_SetModel.Init( (unsigned char*)SIG_CBASEENTITY_SETMODEL, MASK_CBASEENTITY_SETMODEL, SIZE_CBASEENTITY_SETMODEL );
	sig_CBaseEntity_SetAbsOrigin.Init( (unsigned char*)SIG_CBASEENTITY_SETABSORIGIN, MASK_CBASEENTITY_SETABSORIGIN, SIZE_CBASEENTITY_SETABSORIGIN );
	return;
}


