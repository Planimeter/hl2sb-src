//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef BASEENTITY_SHARED_H
#define BASEENTITY_SHARED_H
#ifdef _WIN32
#pragma once
#endif


// saverestore.h declarations
class CBaseEntity;

#define SIZE_CBASEENTITY_ISALIVE	12
#define SIG_CBASEENTITY_ISALIVE		"\x33\xC0\x38\x81\xE0\x00\x00\x00\x0F\x94\xC0\xC3"
#define MASK_CBASEENTITY_ISALIVE	"xxxxxxxxxxxx"

bool CBaseEntity_IsAlive( CBaseEntity *pEntity );

void siginit_CBaseEntity_shared( void );

#endif // BASEENTITY_SHARED_H
