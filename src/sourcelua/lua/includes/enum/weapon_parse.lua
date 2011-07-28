--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Weapon data file parsing, shared by game & client dlls.
--
--===========================================================================--

-- -----------------------------------------------------------
-- Weapon sound types
-- Used to play sounds defined in the weapon's classname.txt file
-- This needs to match pWeaponSoundCategories in weapon_parse.cpp
-- ------------------------------------------------------------
EMPTY = 1
SINGLE = 2
SINGLE_NPC = 3
WPN_DOUBLE = 4 -- Can't be "DOUBLE" because windows.h uses it.
DOUBLE_NPC = 5
BURST = 6
RELOAD = 7
RELOAD_NPC = 8
MELEE_MISS = 9
MELEE_HIT = 10
MELEE_HIT_WORLD = 11
SPECIAL1 = 12
SPECIAL2 = 13
SPECIAL3 = 14
TAUNT = 15

-- Add new shoot sound types here

NUM_SHOOT_SOUND_TYPES = 16

MAX_SHOOT_SOUNDS	= 16		-- Maximum number of shoot sounds per shoot type

MAX_WEAPON_STRING	= 80
MAX_WEAPON_PREFIX	= 16
MAX_WEAPON_AMMO_NAME		= 32

WEAPON_PRINTNAME_MISSING = "!!! Missing printname on weapon"
