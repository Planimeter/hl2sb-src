//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: A base class for the client-side representation of entities.
//
//			This class encompasses both entities that are created on the server
//			and networked to the client AND entities that are created on the
//			client.
//
// $NoKeywords: $
//===========================================================================//

#ifndef LC_BASEENTITY_H
#define LC_BASEENTITY_H
#ifdef _WIN32
#pragma once
#endif

int luaopen_CBaseEntity(lua_State *L);

#endif // LC_BASEENTITY_H
