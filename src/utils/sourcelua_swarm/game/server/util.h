//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Misc utility code.
//
// $NoKeywords: $
//=============================================================================//

#ifndef UTIL_H
#define UTIL_H
#ifdef _WIN32
#pragma once
#endif


#include "tier0/memdbgon.h"

class CBaseEntity;
class CBasePlayer;

//-----------------------------------------------------------------------------
// Purpose: Gets a player pointer by 1-based index
//			If player is not yet spawned or connected, returns NULL
// Input  : playerIndex - index of the player - first player is index 1
//-----------------------------------------------------------------------------

// NOTENOTE: Use UTIL_GetLocalPlayer instead of UTIL_PlayerByIndex IF you're in single player
// and you want the player.
#define SIZE_UTIL_PLAYERBYINDEX	66
#define SIG_UTIL_PLAYERBYINDEX	"\x8B\x44\x24\x04\x56\x33\xF6\x85\xC0\x7E\x3B\x8B\x0D\x60\x39\x5D\x10\x3B\x41\x14" \
								"\x7F\x30\x8B\x0D\x68\x39\x5D\x10\x8B\x11\x50\x8B\x42\x4C\xFF\xD0\x85\xC0\x74\x1E" \
								"\x8B\x08\xD1\xE9\xF6\xC1\x01\x75\x15\x8B\x40\x0C\x85\xC0\x74\x0A\x8B\x10\x8B\xC8" \
								"\x8B\x42\x14\x5E\xFF\xE0"
#define MASK_UTIL_PLAYERBYINDEX	"xxxxxxxxxxx??????xxxxx??????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

CBasePlayer	*UTIL_PlayerByIndex( int playerIndex );

// marks the entity for deletion so it will get removed next frame
#define SIZE_UTIL_REMOVE	20
#define SIG_UTIL_REMOVE		"\x8B\x44\x24\x04\x85\xC0\x74\x0C\x83\xC0\x0C\x89\x44\x24\x04\xE9\x7C\xFF\xFF\xFF"
#define MASK_UTIL_REMOVE	"xxxxxxxxxxxxxxxxxxxx"

void UTIL_Remove( CBaseEntity *oldObj );

#define SIZE_UTIL_CLIENTPRINTALL	96
#define SIG_UTIL_CLIENTPRINTALL		"\x83\xEC\x20\x8D\x0C\x24\xE8\x95\xC0\xFB\xFF\x8D\x0C\x24\xC7\x04\x24\x94\x73\x41" \
									"\x10\xE8\xB6\xC5\xFB\xFF\x8D\x0C\x24\xC7\x04\x24\xD4\x16\x48\x10\xE8\x07\xBF\xFB" \
									"\xFF\x8B\x44\x24\x38\x8B\x4C\x24\x34\x8B\x54\x24\x30\x50\x8B\x44\x24\x30\x51\x8B" \
									"\x4C\x24\x30\x52\x8B\x54\x24\x30\x50\x51\x52\x8D\x44\x24\x18\x50\xE8\x2F\xF0\xFF" \
									"\xFF\x83\xC4\x1C\x8D\x0C\x24\xE8\x74\xC0\xFB\xFF\x83\xC4\x20\xC3"
#define MASK_UTIL_CLIENTPRINTALL	"xxxxxx?????xxx????????????xxx????????????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????" \
									"?xxxxxx?????xxxx"

// prints a message to each client
void			UTIL_ClientPrintAll( int msg_dest, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL );

#define SIZE_DISPATCHSPAWN	95
#define SIG_DISPATCHSPAWN	"\x53\x55\x56\x8B\x74\x24\x10\x85\xF6\x57\x0F\x84\x56\x01\x00\x00\x8B\x1D\x78\xDF" \
							"\x61\x10\x8B\x03\x8B\x50\x64\x8B\xCB\xFF\xD2\x8B\x06\x8B\x50\x08\x8B\xCE\xFF\xD2" \
							"\x8B\x0D\x78\xDF\x61\x10\x8B\x28\x8B\x01\x8B\x90\x80\x00\x00\x00\x6A\x00\x6A\x03" \
							"\xFF\xD2\x88\x44\x24\x14\x8B\x06\x8B\x90\xC0\x00\x00\x00\x8B\xCE\xFF\xD2\x8B\xF8" \
							"\x85\xFF\x8B\xCE\x75\x09\x8B\x06\x8B\x50\x54\xFF\xD2\xEB\x18"
#define MASK_DISPATCHSPAWN	"xxxxxxxxxxxxxxxx??????xxxxxxxxxxxxxxxxxx??????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
							"xxxxxxxxxxxxxxx"

void siginit_UTIL( void );

#endif // UTIL_H
