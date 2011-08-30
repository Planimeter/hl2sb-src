//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//

#include "cbase.h"
#include "luabaseentity.h"
#include "luamanager.h"
#include "lbaseentity_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_NETWORKCLASS_ALIASED( LuaBaseEntity, DT_LuaBaseEntity )

BEGIN_NETWORK_TABLE( CLuaBaseEntity, DT_LuaBaseEntity )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(	CLuaBaseEntity )
END_PREDICTION_DATA()

CLuaBaseEntity::CLuaBaseEntity( void )
{
#ifdef LUA_SDK
	// We're done in CBaseEntity
	// m_nRefCount = LUA_REFNIL;
#endif
}

CLuaBaseEntity::~CLuaBaseEntity( void )
{
	// Andrew; This is actually done in CBaseEntity. I'm doing it here because
	// this is the class that initialized the reference.
#ifdef LUA_SDK
	lua_unref( L, m_nRefCount );
#endif
}

void CLuaBaseEntity::Think()
{
#ifdef LUA_SDK
	BEGIN_LUA_CALL_ENTITY_METHOD( "Think" );
	END_LUA_CALL_ENTITY_METHOD( 0, 0 );
#endif

	BaseClass::Think( );
}

