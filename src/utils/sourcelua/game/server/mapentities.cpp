//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Controls the loading, parsing and creation of the entities from the BSP.
//
//=============================================================================//

#include "../../sigscan.h"
#include "mapentities.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


CSigScan sig_CreateEntityByName;
// creates an entity by string name, but does not spawn it
CBaseEntity *CreateEntityByName( const char *className, int iForceEdictIndex )
{
	BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_GLOBAL( sig_CreateEntityByName, "CreateEntityByName" );
		return NULL;
	END_CHECK_UNSUCCESSFUL_SIGNATURE();

#ifdef WIN32
	typedef CBaseEntity *( __fastcall *pfn )( const char *, int ); 
#else
	typedef CBaseEntity *( *pfn )( const char *, int );
#endif
	pfn pCreateEntityByName = (pfn)sig_CreateEntityByName.sig_addr;
	return pCreateEntityByName( className, iForceEdictIndex );
}


void siginit_mapentities( void )
{
	sig_CreateEntityByName.Init( (unsigned char*)SIG_CREATEENTITYBYNAME, MASK_CREATEENTITYBYNAME, SIZE_CREATEENTITYBYNAME );
	return;
}


