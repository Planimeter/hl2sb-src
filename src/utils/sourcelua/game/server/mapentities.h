//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef MAPENTITIES_H
#define MAPENTITIES_H
#ifdef _WIN32
#pragma once
#endif

class CBaseEntity;

#define SIZE_CREATEENTITYBYNAME	54
#define SIG_CREATEENTITYBYNAME	"\x56\x8B\x74\x24\x0C\x83\xFE\xFF\x57\x8B\x7C\x24\x0C\x74\x27\x8B\x0D\x68\x39\x5D" \
								"\x10\x8B\x01\x8B\x50\x54\x56\xFF\xD2\x85\xC0\xA3\x54\x7D\x5B\x10\x75\x10\x56\x57" \
								"\x68\x4C\xAD\x44\x10\xFF\x15\xFC\x51\x3E\x10\x83\xC4\x0C"
#define MASK_CREATEENTITYBYNAME	"xxxxxxxxxxxxxxx??????xxxxxxxxxx?????xxxx???????????xxx"

void siginit_mapentities( void );

#endif // MAPENTITIES_H
