//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//
#include "..\shared\luamanager.h"
#include "..\shared\luacachefile.h"
#include "..\shared\luasrclib.h"
#include "..\lua-5.1.5\etc\lua.hpp"
#include "luamanager.h"

#ifndef LFILESYSTEM_H
#define LFILESYSTEM_H

#ifdef _WIN32
#pragma once
#endif

int luaopen_filesystem(lua_State *L);

#endif // LFILESYSTEM_H
