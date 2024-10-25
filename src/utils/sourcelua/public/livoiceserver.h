//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: This module defines the IVoiceServer interface, which is used by
//			game code to control which clients are listening to which other
//			clients' voice streams.
//
// $NoKeywords: $
//=============================================================================//

#include "..\shared\luamanager.h"
#include "..\shared\luacachefile.h"
#include "..\shared\luasrclib.h"
#include "..\lua-5.1.5\etc\lua.hpp"
#include "luamanager.h"

#ifndef LIVOICESERVER_H
#define LIVOICESERVER_H


int luaopen_g_pVoiceServer(lua_State *L);


#endif

