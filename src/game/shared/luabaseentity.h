//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//

#ifndef LUA_BASEENTITY_H
#define LUA_BASEENTITY_H

#include "predictable_entity.h"
#include "baseentity_shared.h"

#if defined( CLIENT_DLL )
#define CLuaBaseEntity C_LuaBaseEntity

#endif 

class CLuaBaseEntity : /* public CBaseEntity */ public CBaseAnimating
{
public:
	// DECLARE_CLASS( CLuaBaseEntity, CBaseEntity );
	DECLARE_CLASS( CLuaBaseEntity, CBaseAnimating );
	DECLARE_PREDICTABLE();
	DECLARE_NETWORKCLASS();

	CLuaBaseEntity();
	~CLuaBaseEntity();

	bool	IsScripted( void ) const { return true; }
	
	// CBaseEntity overrides.
public:
	void	Think();	

	// void	Spawn( void );
	void	Precache( void );
	void	InitScriptedEntity( void );

#ifdef CLIENT_DLL
	virtual void	OnDataChanged( DataUpdateType_t updateType );
	virtual const char *GetScriptedClassname( void );
#endif

private:
	CLuaBaseEntity( const CLuaBaseEntity & ); // not defined, not accessible

	CNetworkString( m_iScriptedClassname, 255 );
};

void RegisterScriptedBaseEntity( const char *szClassname );

#endif


