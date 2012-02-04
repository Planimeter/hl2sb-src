//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef LC_RECIPIENTFILTER_H
#define LC_RECIPIENTFILTER_H
#ifdef _WIN32
#pragma once
#endif

/* type for CRecipientFilter functions */
typedef C_RecipientFilter lua_CRecipientFilter;



/*
** access functions (stack -> C)
*/

LUA_API lua_CRecipientFilter     &(lua_torecipientfilter) (lua_State *L, int idx);


/*
** push functions (C -> stack)
*/
LUA_API void  (lua_pushrecipientfilter) (lua_State *L, lua_CRecipientFilter &filter);



LUALIB_API lua_CRecipientFilter &(luaL_checkrecipientfilter) (lua_State *L, int narg);


LUALIB_API int luaopen_CRecipientFilter(lua_State *L);

#endif // LC_RECIPIENTFILTER_H
