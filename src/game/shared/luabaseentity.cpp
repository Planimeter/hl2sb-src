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
#ifdef CLIENT_DLL
	RecvPropString( RECVINFO( m_iScriptedClassname ) ),
#else
	SendPropString( SENDINFO( m_iScriptedClassname ) ),
#endif
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(	CLuaBaseEntity )
END_PREDICTION_DATA()

#ifdef CLIENT_DLL
static C_BaseEntity *CCLuaBaseEntityFactory( void )
{
	return static_cast< C_BaseEntity * >( new CLuaBaseEntity );
};
#endif

#ifndef CLIENT_DLL
static CUtlDict< CEntityFactory<CLuaBaseEntity>*, unsigned short > m_EntityFactoryDatabase;
#endif

void RegisterScriptedBaseEntity( const char *className )
{
#ifdef CLIENT_DLL
	GetClassMap().Add( className, "CLuaBaseEntity", sizeof( CLuaBaseEntity ),
		&CCLuaBaseEntityFactory );
#else
	if ( EntityFactoryDictionary()->FindFactory( className ) )
	{
		return;
	}

	unsigned short lookup = m_EntityFactoryDatabase.Find( className );
	if ( lookup != m_EntityFactoryDatabase.InvalidIndex() )
	{
		return;
	}

	CEntityFactory<CLuaBaseEntity> *pFactory = new CEntityFactory<CLuaBaseEntity>( className );

	lookup = m_EntityFactoryDatabase.Insert( className, pFactory );
	Assert( lookup != m_EntityFactoryDatabase.InvalidIndex() );
#endif
}

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

void CLuaBaseEntity::InitScriptedEntity( void )
{
#if defined ( LUA_SDK )
#ifndef CLIENT_DLL
	// Let the instance reinitialize itself for the client.
	if ( m_nRefCount != LUA_REFNIL )
		return;
#endif

	char className[ 255 ];
#if defined ( CLIENT_DLL )
	if ( strlen( GetScriptedClassname() ) > 0 )
		Q_strncpy( className, GetScriptedClassname(), sizeof( className ) );
	else
		Q_strncpy( className, GetClassname(), sizeof( className ) );
#else
	Q_strncpy( m_iScriptedClassname.GetForModify(), GetClassname(), sizeof( className ) );
 	Q_strncpy( className, GetClassname(), sizeof( className ) );
#endif
 	Q_strlower( className );
	SetClassname( className );

	lua_getglobal( L, "entity" );
	if ( lua_istable( L, -1 ) )
	{
		lua_getfield( L, -1, "Get" );
		if ( lua_isfunction( L, -1 ) )
		{
			lua_remove( L, -2 );
			lua_pushstring( L, className );
			luasrc_pcall( L, 1, 1, 0 );
		}
		else
		{
			lua_pop( L, 2 );
		}
	}
	else
	{
		lua_pop( L, 1 );
	}

	m_nRefCount = luaL_ref( L, LUA_REGISTRYINDEX );

	BEGIN_LUA_CALL_ENTITY_METHOD( "Initialize" );
	END_LUA_CALL_ENTITY_METHOD( 0, 0 );
#endif
}

#ifdef CLIENT_DLL
void CLuaBaseEntity::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );

	if ( updateType == DATA_UPDATE_CREATED )
	{
		if ( m_iScriptedClassname.Get() )
		{
			SetClassname( m_iScriptedClassname.Get() );
			InitScriptedEntity();

#ifdef LUA_SDK
			BEGIN_LUA_CALL_ENTITY_METHOD( "Precache" );
			END_LUA_CALL_ENTITY_METHOD( 0, 0 );
#endif
		}
	}
}

const char *CLuaBaseEntity::GetScriptedClassname( void )
{
	if ( m_iScriptedClassname.Get() )
		return m_iScriptedClassname.Get();
	return BaseClass::GetClassname();
}
#endif

void CLuaBaseEntity::Precache( void )
{
	BaseClass::Precache();

	InitScriptedEntity();
}

void CLuaBaseEntity::Think()
{
#ifdef LUA_SDK
	BEGIN_LUA_CALL_ENTITY_METHOD( "Think" );
	END_LUA_CALL_ENTITY_METHOD( 0, 0 );
#endif

	BaseClass::Think( );
}

