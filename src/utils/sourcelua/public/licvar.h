//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#include "..\shared\luamanager.h"
#include "..\shared\luacachefile.h"
#include "..\shared\luasrclib.h"
#include "..\lua-5.1.5\etc\lua.hpp"
#include "luamanager.h"

#ifndef LICVAR_H
#define LICVAR_H
#ifdef _WIN32
#pragma once
#endif

void RemoveGlobalChangeCallbacks( void );

int luaopen_cvar(lua_State *L);


#endif // LICVAR_H
