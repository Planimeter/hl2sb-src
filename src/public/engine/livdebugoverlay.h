//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
//
//  cdll_int.h
//
// 4-23-98  
// JOHN:  client dll interface declarations
//

#ifndef LIVDEBUGOVERLAY_H
#define LIVDEBUGOVERLAY_H

#ifdef _WIN32
#pragma once
#endif

int luaopen_debugoverlay(lua_State *L);


#endif // LIVDEBUGOVERLAY_H
