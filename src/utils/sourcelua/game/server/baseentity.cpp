//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: The base class from which all game entities are derived.
//
//===========================================================================//

#include "../../sigscan.h"
#include "baseentity.h"
#include "string_t.h"
#include "datamap.h"
#include "variant_t.h"
#include "mathlib/vector.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: calls the appropriate message mapped function in the entity according
//			to the fired action.
// Input  : char *szInputName - input destination
//			*pActivator - entity which initiated this sequence of actions
//			*pCaller - entity from which this event is sent
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
CSigScan sig_CBaseEntity_AcceptInput;
bool CBaseEntity_AcceptInput( CBaseEntity *pEntity, const char *szInputName, CBaseEntity *pActivator, CBaseEntity *pCaller, variant_t Value, int outputID )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_METHOD( sig_CBaseEntity_AcceptInput, "AcceptInput" );
		return false;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

    union {
        bool ( CEmpty::*m_pfnMethod )( const char *, CBaseEntity *, CBaseEntity *, variant_t, int );
        void *addr;
    } u;
    u.addr = sig_CBaseEntity_AcceptInput.sig_addr;
 
	return (reinterpret_cast<CEmpty*>(pEntity)->*u.m_pfnMethod)( szInputName, pActivator, pCaller, Value, outputID );
}

CSigScan sig_CBaseEntity_GetClassname;
const char* CBaseEntity_GetClassname( CBaseEntity *pEntity )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_METHOD( sig_CBaseEntity_GetClassname, "GetClassname" );
		return NULL;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

    union {
        const char* ( CEmpty::*m_pfnMethod )();
        void *addr;
    } u;
    u.addr = sig_CBaseEntity_GetClassname.sig_addr;
 
	return (reinterpret_cast<CEmpty*>(pEntity)->*u.m_pfnMethod)();
}

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
// Purpose: Add model to level precache list
// Input  : *name - model name
// Output : int -- model index for model
//-----------------------------------------------------------------------------
CSigScan sig_CBaseEntity_PrecacheModel;
int CBaseEntity_PrecacheModel( const char *name )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_METHOD( sig_CBaseEntity_PrecacheModel, "PrecacheModel" );
		return -1;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

#ifdef WIN32
	typedef int	( __fastcall *pfn )( const char * ); 
#else
	typedef int	( *pfn )( const char * );
#endif
	pfn PrecacheModel = (pfn)sig_CBaseEntity_PrecacheModel.sig_addr;
	return PrecacheModel( name );
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
	// sig_CBaseEntity_AcceptInput.Init( (unsigned char*)SIG_CBASEENTITY_ACCEPTINPUT, MASK_CBASEENTITY_ACCEPTINPUT, SIZE_CBASEENTITY_ACCEPTINPUT );
	sig_CBaseEntity_GetClassname.Init( (unsigned char*)SIG_CBASEENTITY_GETCLASSNAME, MASK_CBASEENTITY_GETCLASSNAME, SIZE_CBASEENTITY_GETCLASSNAME );
	sig_CBaseEntity_Instance.Init( (unsigned char*)SIG_CBASEENTITY_INSTANCE, MASK_CBASEENTITY_INSTANCE, SIZE_CBASEENTITY_INSTANCE );
	sig_CBaseEntity_SetModel.Init( (unsigned char*)SIG_CBASEENTITY_SETMODEL, MASK_CBASEENTITY_SETMODEL, SIZE_CBASEENTITY_SETMODEL );
	sig_CBaseEntity_PrecacheModel.Init( (unsigned char*)SIG_CBASEENTITY_PRECACHEMODEL, MASK_CBASEENTITY_PRECACHEMODEL, SIZE_CBASEENTITY_PRECACHEMODEL );
	sig_CBaseEntity_SetAbsOrigin.Init( (unsigned char*)SIG_CBASEENTITY_SETABSORIGIN, MASK_CBASEENTITY_SETABSORIGIN, SIZE_CBASEENTITY_SETABSORIGIN );
	return;
}


