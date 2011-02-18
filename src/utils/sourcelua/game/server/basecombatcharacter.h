//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Base combat character with no AI
//
// $NoKeywords: $
//=============================================================================//

#ifndef BASECOMBATCHARACTER_H
#define BASECOMBATCHARACTER_H

#ifdef _WIN32
#pragma once
#endif

class CBaseCombatWeapon;


//-----------------------------------------------------------------------------
// Purpose: This should contain all of the combat entry points / functionality 
// that are common between NPCs and players
//-----------------------------------------------------------------------------
class CBaseCombatCharacter;

// Weapons..
#define SIZE_CBASECOMBATCHARACTER_GETACTIVEWEAPON	46
#define SIG_CBASECOMBATCHARACTER_GETACTIVEWEAPON	"\x8B\x81\xD8\x07\x00\x00\x83\xF8\xFF\x74\x20\x8B\x15\x7C\xD1\x54\x10\x8B\xC8\x81" \
													"\xE1\xFF\x0F\x00\x00\xC1\xE1\x04\x8D\x4C\x11\x04\xC1\xE8\x0C\x39\x41\x04\x75\x03" \
													"\x8B\x01\xC3\x33\xC0\xC3"
#define MASK_CBASECOMBATCHARACTER_GETACTIVEWEAPON	"??????xxxxx??????xxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

CBaseCombatWeapon*	CBaseCombatCharacter_GetActiveWeapon( CBaseCombatCharacter *pPlayer );

void siginit_CBaseCombatCharacter( void );

#endif // BASECOMBATCHARACTER_H
