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

class CLuaBaseEntity : public CBaseEntity
{
public:
	DECLARE_CLASS( CLuaBaseEntity, CBaseEntity );
	DECLARE_PREDICTABLE();
	DECLARE_NETWORKCLASS();

	CLuaBaseEntity();
	virtual ~CLuaBaseEntity();

	// CBaseEntity overrides.
public:
	virtual void	Think();	

private:
	CLuaBaseEntity( const CLuaBaseEntity & ); // not defined, not accessible
};

#endif


