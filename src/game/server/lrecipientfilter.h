//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef LRECIPIENTFILTER_H
#define LRECIPIENTFILTER_H
#ifdef _WIN32
#pragma once
#endif

/* type for CRecipientFilter functions */
typedef CRecipientFilter lua_CRecipientFilter;



/*
** access functions (stack -> C)
*/

LUA_API lua_CRecipientFilter     *(lua_torecipientfilter) (lua_State *L, int idx);


/*
** push functions (C -> stack)
*/
LUA_API void  (lua_pushrecipientfilter) (lua_State *L, lua_CRecipientFilter *filter);



LUALIB_API lua_CRecipientFilter *(luaL_checkrecipientfilter) (lua_State *L, int narg);


int luaopen_CRecipientFilter(lua_State *L);

#endif // LRECIPIENTFILTER_H
