//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//
#include "..\shared\luamanager.h"
#include "..\shared\luacachefile.h"
#include "..\shared\luasrclib.h"
#include "..\lua-5.1.5\etc\lua.hpp"
#include "luamanager.h"

#ifndef LGLOBALVARS_BASE_H
#define LGLOBALVARS_BASE_H

#ifdef _WIN32
#pragma once
#endif

int luaopen_gpGlobals(lua_State *L);

#endif // LGLOBALVARS_BASE_H
