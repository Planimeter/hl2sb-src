//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Interfaces between the client.dll and engine
//
//===========================================================================//

#ifndef LCDLL_INT_H
#define LCDLL_INT_H
#ifdef _WIN32
#pragma once
#endif

int luaopen_engine(lua_State *L);

#endif // LCDLL_INT_H
