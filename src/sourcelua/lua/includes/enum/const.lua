--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose:
--
--===========================================================================--

-- the command line param that tells the engine to use steam
STEAM_PARM					= "-steam"
-- the command line param to tell dedicated server to restart
-- if they are out of date
AUTO_RESTART = "-autoupdate"

-- the message a server sends when a clients steam login is expired
INVALID_STEAM_TICKET = "Invalid STEAM UserID Ticket\n"
INVALID_STEAM_LOGON = "No Steam logon\n"
INVALID_STEAM_VACBANSTATE = "VAC banned from secure server\n"
INVALID_STEAM_LOGGED_IN_ELSEWHERE = "This Steam account is being used in another location\n"

-- This is the default, see shareddefs.h for mod-specific value, which can override this
DEFAULT_TICK_INTERVAL	= (0.015)				-- 15 msec is the default
MINIMUM_TICK_INTERVAL   = (0.001)
MAXIMUM_TICK_INTERVAL	= (0.1)

-- This is the max # of players the engine can handle
ABSOLUTE_PLAYER_LIMIT = 255  -- not 256, so we can send the limit as a byte
ABSOLUTE_PLAYER_LIMIT_DW	= ( (ABSOLUTE_PLAYER_LIMIT/32) + 1 )

-- a player name may have 31 chars + 0 on the PC.
-- the 360 only allows 15 char + 0, but stick with the larger PC size for cross-platform communication
MAX_PLAYER_NAME_LENGTH		= 32

if X360 then
MAX_PLAYERS_PER_CLIENT		= XUSER_MAX_COUNT	-- Xbox 360 supports 4 players per console
else
MAX_PLAYERS_PER_CLIENT		= 1	-- One player per PC
end

MAX_MAP_NAME				= 32
MAX_NETWORKID_LENGTH		= 64  -- num chars for a network (i.e steam) ID

-- BUGBUG: Reconcile with or derive this from the engine's internal definition!
-- FIXME: I added an extra bit because I needed to make it signed
SP_MODEL_INDEX_BITS			= 11

-- How many bits to use to encode an edict.
MAX_EDICT_BITS				= 11			-- # of bits needed to represent max edicts
-- Max # of edicts in a level
MAX_EDICTS					= (2^MAX_EDICT_BITS)

-- How many bits to use to encode an server class index
MAX_SERVER_CLASS_BITS		= 9
-- Max # of networkable server classes
MAX_SERVER_CLASSES			= (2^MAX_SERVER_CLASS_BITS)

SIGNED_GUID_LEN = 32 -- Hashed CD Key (32 hex alphabetic chars + 0 terminator )

-- Used for networking ehandles.
NUM_ENT_ENTRY_BITS		= (MAX_EDICT_BITS + 1)
NUM_ENT_ENTRIES			= (2^ NUM_ENT_ENTRY_BITS)
ENT_ENTRY_MASK			= (NUM_ENT_ENTRIES - 1)
INVALID_EHANDLE_INDEX	= 0xFFFFFFFF

NUM_SERIAL_NUM_BITS		= (32 - NUM_ENT_ENTRY_BITS)


-- Networked ehandles use less bits to encode the serial number.
NUM_NETWORKED_EHANDLE_SERIAL_NUMBER_BITS	= 10
NUM_NETWORKED_EHANDLE_BITS					= (MAX_EDICT_BITS + NUM_NETWORKED_EHANDLE_SERIAL_NUMBER_BITS)
INVALID_NETWORKED_EHANDLE_VALUE				= ((2^ NUM_NETWORKED_EHANDLE_BITS) - 1)

-- This is the maximum amount of data a PackedEntity can have. Having a limit allows us
-- to use static arrays sometimes instead of allocating memory all over the place.
MAX_PACKEDENTITY_DATA	= (16384)

-- This is the maximum number of properties that can be delta'd. Must be evenly divisible by 8.
MAX_PACKEDENTITY_PROPS	= (4096)

-- a client can have up to 4 customization files (logo, sounds, models, txt).
MAX_CUSTOM_FILES		= 4		-- max 4 files
MAX_CUSTOM_FILE_SIZE	= 131072

--
-- Constants shared by the engine and dlls
-- This header file included by engine files and DLL files.
-- Most came from server.h

-- CBaseEntity::m_fFlags
-- PLAYER SPECIFIC FLAGS FIRST BECAUSE WE USE ONLY A FEW BITS OF NETWORK PRECISION
FL_ONGROUND				= (2^0)	-- At rest / on the ground
FL_DUCKING				= (2^1)	-- Player flag -- Player is fully crouched
FL_WATERJUMP			= (2^2)	-- player jumping out of water
FL_ONTRAIN				= (2^3) -- Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
FL_INRAIN				= (2^4)	-- Indicates the entity is standing in rain
FL_FROZEN				= (2^5) -- Player is frozen for 3rd person camera
FL_ATCONTROLS			= (2^6) -- Player can't move, but keeps key inputs for controlling another entity
FL_CLIENT				= (2^7)	-- Is a player
FL_FAKECLIENT			= (2^8)	-- Fake client, simulated server side; don't send network messages to them

-- NOTE if you move things up, make sure to change this value
PLAYER_FLAG_BITS		= 9

-- NON-PLAYER SPECIFIC (i.e., not used by GameMovement or the client .dll ) -- Can still be applied to players, though
FL_INWATER				= (2^9)	-- In water
FL_FLY					= (2^10)	-- Changes the SV_Movestep() behavior to not need to be on ground
FL_SWIM					= (2^11)	-- Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
FL_CONVEYOR				= (2^12)
FL_NPC					= (2^13)
FL_GODMODE				= (2^14)
FL_NOTARGET				= (2^15)
FL_AIMTARGET			= (2^16)	-- set if the crosshair needs to aim onto the entity
FL_PARTIALGROUND		= (2^17)	-- not all corners are valid
FL_STATICPROP			= (2^18)	-- Eetsa static prop!
FL_GRAPHED				= (2^19) -- worldgraph has this ent listed as something that blocks a connection
FL_GRENADE				= (2^20)
FL_STEPMOVEMENT			= (2^21)	-- Changes the SV_Movestep() behavior to not do any processing
FL_DONTTOUCH			= (2^22)	-- Doesn't generate touch functions, generates Untouch() for anything it was touching when this flag was set
FL_BASEVELOCITY			= (2^23)	-- Base velocity has been applied this frame (used to convert base velocity into momentum)
FL_WORLDBRUSH			= (2^24)	-- Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
FL_OBJECT				= (2^25) -- Terrible name. This is an object that NPCs should see. Missiles, for example.
FL_KILLME				= (2^26)	-- This entity is marked for death -- will be freed by game DLL
FL_ONFIRE				= (2^27)	-- You know...
FL_DISSOLVING			= (2^28) -- We're dissolving!
FL_TRANSRAGDOLL			= (2^29) -- In the process of turning into a client side ragdoll.
FL_UNBLOCKABLE_BY_PLAYER = (2^30) -- pusher that can't be blocked by the player

-- edict->movetype values
MOVETYPE_NONE		= 0		-- never moves
MOVETYPE_ISOMETRIC	= 1		-- For players -- in TF2 commander view, etc.
MOVETYPE_WALK		= 2		-- Player only - moving on the ground
MOVETYPE_STEP		= 3		-- gravity, special edge handling -- monsters use this
MOVETYPE_FLY		= 4		-- No gravity, but still collides with stuff
MOVETYPE_FLYGRAVITY	= 5		-- flies through the air + is affected by gravity
MOVETYPE_VPHYSICS	= 6		-- uses VPHYSICS for simulation
MOVETYPE_PUSH		= 7		-- no clip to world, push and crush
MOVETYPE_NOCLIP		= 8		-- No gravity, no collisions, still do velocity/avelocity
MOVETYPE_LADDER		= 9		-- Used by players only when going onto a ladder
MOVETYPE_OBSERVER	= 10	-- Observer movement, depends on player's observer mode
MOVETYPE_CUSTOM		= 11	-- Allows the entity to describe its own physics

-- should always be defined as the last item in the list
MOVETYPE_LAST		= MOVETYPE_CUSTOM

MOVETYPE_MAX_BITS	= 4

-- edict->movecollide values
MOVECOLLIDE_DEFAULT = 0

-- These ones only work for MOVETYPE_FLY + MOVETYPE_FLYGRAVITY
MOVECOLLIDE_FLY_BOUNCE = 1	-- bounces, reflects, based on elasticity of surface and object - applies friction (adjust velocity)
MOVECOLLIDE_FLY_CUSTOM = 2	-- Touch() will modify the velocity however it likes
MOVECOLLIDE_FLY_SLIDE = 3   -- slides along surfaces (no bounce) - applies friciton (adjusts velocity)

MOVECOLLIDE_COUNT = 4		-- Number of different movecollides

-- When adding new movecollide types, make sure this is correct
MOVECOLLIDE_MAX_BITS = 3

-- edict->solid values
-- NOTE: Some movetypes will cause collisions independent of SOLID_NOT/SOLID_TRIGGER when the entity moves
-- SOLID only effects OTHER entities colliding with this one when they move - UGH!

-- Solid type basically describes how the bounding volume of the object is represented
-- NOTE: SOLID_BBOX MUST BE 2, and SOLID_VPHYSICS MUST BE 6
-- NOTE: These numerical values are used in the FGD by the prop code (see prop_dynamic)
SOLID_NONE			= 0		-- no solid model
SOLID_BSP			= 1		-- a BSP tree
SOLID_BBOX			= 2		-- an AABB
SOLID_OBB			= 3		-- an OBB (not implemented yet)
SOLID_OBB_YAW		= 4		-- an OBB, constrained so that it can only yaw
SOLID_CUSTOM		= 5		-- Always call into the entity for tests
SOLID_VPHYSICS		= 6		-- solid vphysics object, get vcollide from the model and collide with that
SOLID_LAST			= 7

FSOLID_CUSTOMRAYTEST		= 0x0001	-- Ignore solid type + always call into the entity for ray tests
FSOLID_CUSTOMBOXTEST		= 0x0002	-- Ignore solid type + always call into the entity for swept box tests
FSOLID_NOT_SOLID			= 0x0004	-- Are we currently not solid?
FSOLID_TRIGGER				= 0x0008	-- This is something may be collideable but fires touch functions
										-- even when it's not collideable (when the FSOLID_NOT_SOLID flag is set)
FSOLID_NOT_STANDABLE		= 0x0010	-- You can't stand on this
FSOLID_VOLUME_CONTENTS		= 0x0020	-- Contains volumetric contents (like water)
FSOLID_FORCE_WORLD_ALIGNED	= 0x0040	-- Forces the collision rep to be world-aligned even if it's SOLID_BSP or SOLID_VPHYSICS
FSOLID_USE_TRIGGER_BOUNDS	= 0x0080	-- Uses a special trigger bounds separate from the normal OBB
FSOLID_ROOT_PARENT_ALIGNED	= 0x0100	-- Collisions are defined in root parent's local coordinate space
FSOLID_TRIGGER_TOUCH_DEBRIS	= 0x0200	-- This trigger will touch debris objects

FSOLID_MAX_BITS	= 10

-------------------------------------------------------------------------------
-- A couple of inline helper methods
-------------------------------------------------------------------------------
function IsSolid( solidType, nSolidFlags )
	return (solidType ~= SOLID_NONE) and ((nSolidFlags == nSolidFlags and 0 or FSOLID_NOT_SOLID) == 0);
end


-- m_lifeState values
LIFE_ALIVE				= 0 -- alive
LIFE_DYING				= 1 -- playing death animation or still falling off of a ledge waiting to hit ground
LIFE_DEAD				= 2 -- dead. lying still.
LIFE_RESPAWNABLE		= 3
LIFE_DISCARDBODY		= 4

-- entity effects
EF_BONEMERGE			= 0x001		-- Performs bone merge on client side
EF_BRIGHTLIGHT 			= 0x002		-- DLIGHT centered at entity origin
EF_DIMLIGHT 			= 0x004		-- player flashlight
EF_NOINTERP				= 0x008		-- don't interpolate the next frame
EF_NOSHADOW				= 0x010		-- Don't cast no shadow
EF_NODRAW				= 0x020		-- don't draw entity
EF_NORECEIVESHADOW		= 0x040		-- Don't receive no shadow
EF_BONEMERGE_FASTCULL	= 0x080		-- For use with EF_BONEMERGE. If this is set, then it places this ent's origin at its
									-- parent and uses the parent's bbox + the max extents of the aiment.
									-- Otherwise, it sets up the parent's bones every frame to figure out where to place
									-- the aiment, which is inefficient because it'll setup the parent's bones even if
									-- the parent is not in the PVS.
EF_ITEM_BLINK			= 0x100		-- blink an item so that the user notices it.
EF_PARENT_ANIMATES		= 0x200		-- always assume that the parent entity is animating
EF_MAX_BITS = 10

EF_PARITY_BITS	= 3
EF_PARITY_MASK  = ((2^EF_PARITY_BITS)-1)

-- How many bits does the muzzle flash parity thing get?
EF_MUZZLEFLASH_BITS = 2

-- plats
PLAT_LOW_TRIGGER	= 1

-- Trains
SF_TRAIN_WAIT_RETRIGGER	= 1
SF_TRAIN_PASSABLE		= 8		-- Train is not solid -- used to make water trains

-- view angle update types for CPlayerState::fixangle
FIXANGLE_NONE			= 0
FIXANGLE_ABSOLUTE		= 1
FIXANGLE_RELATIVE		= 2

-- Break Model Defines

BREAK_GLASS		= 0x01
BREAK_METAL		= 0x02
BREAK_FLESH		= 0x04
BREAK_WOOD		= 0x08

BREAK_SMOKE		= 0x10
BREAK_TRANS		= 0x20
BREAK_CONCRETE	= 0x40

-- If this is set, then we share a lighting origin with the last non-slave breakable sent down to the client
BREAK_SLAVE		= 0x80

-- Colliding temp entity sounds

BOUNCE_GLASS	= BREAK_GLASS
BOUNCE_METAL	= BREAK_METAL
BOUNCE_FLESH	= BREAK_FLESH
BOUNCE_WOOD		= BREAK_WOOD
BOUNCE_SHRAP	= 0x10
BOUNCE_SHELL	= 0x20
BOUNCE_CONCRETE = BREAK_CONCRETE
BOUNCE_SHOTSHELL = 0x80

-- Temp entity bounce sound types
TE_BOUNCE_NULL		= 0
TE_BOUNCE_SHELL		= 1
TE_BOUNCE_SHOTSHELL	= 2

-- Rendering constants
-- if this is changed, update common/MaterialSystem/Sprite.cpp
kRenderNormal = 1			-- src
kRenderTransColor = 2		-- c*a+dest*(1-a)
kRenderTransTexture = 3		-- src*a+dest*(1-a)
kRenderGlow = 4				-- src*a+dest -- No Z buffer checks -- Fixed size in screen space
kRenderTransAlpha = 5		-- src*srca+dest*(1-srca)
kRenderTransAdd = 6			-- src*a+dest
kRenderEnvironmental = 7	-- not drawn, used for environmental effects
kRenderTransAddFrameBlend = 8 -- use a fractional frame value to blend between animation frames
kRenderTransAlphaAdd = 9	-- src + dest*(1-a)
kRenderWorldGlow = 10		-- Same as kRenderGlow but not fixed size in screen space
kRenderNone = 11			-- Don't render.

kRenderFxNone = 0
kRenderFxPulseSlow = 1
kRenderFxPulseFast = 2
kRenderFxPulseSlowWide = 3
kRenderFxPulseFastWide = 4
kRenderFxFadeSlow = 5
kRenderFxFadeFast = 6
kRenderFxSolidSlow = 7
kRenderFxSolidFast = 8
kRenderFxStrobeSlow = 9
kRenderFxStrobeFast = 10
kRenderFxStrobeFaster = 11
kRenderFxFlickerSlow = 12
kRenderFxFlickerFast = 13
kRenderFxNoDissipation = 14
kRenderFxDistort = 15		-- Distort/scale/translate flicker
kRenderFxHologram = 16		-- kRenderFxDistort + distance fade
kRenderFxExplode = 17		-- Scale up really big!
kRenderFxGlowShell = 18		-- Glowing Shell
kRenderFxClampMinScale = 19	-- Keep this sprite from getting very small (SPRITES only!)
kRenderFxEnvRain = 20		-- for environmental rendermode, make rain
kRenderFxEnvSnow = 21		--  "        "            "    , make snow
kRenderFxSpotlight = 22		-- TEST CODE for experimental spotlight
kRenderFxRagdoll = 23		-- HACKHACK: TEST CODE for signalling death of a ragdoll character
kRenderFxPulseFastWider = 24
kRenderFxMax = 25

COLLISION_GROUP_NONE  = 0
COLLISION_GROUP_DEBRIS = 1			-- Collides with nothing but world and static stuff
COLLISION_GROUP_DEBRIS_TRIGGER = 2  -- Same as debris, but hits triggers
COLLISION_GROUP_INTERACTIVE_DEBRIS = 3	-- Collides with everything except other interactive debris or debris
COLLISION_GROUP_INTERACTIVE = 4		-- Collides with everything except interactive debris or debris
COLLISION_GROUP_PLAYER = 5
COLLISION_GROUP_BREAKABLE_GLASS = 6
COLLISION_GROUP_VEHICLE = 7
COLLISION_GROUP_PLAYER_MOVEMENT = 8  -- For HL2, same as Collision_Group_Player, for
									-- TF2, this filters out other players and CBaseObjects
COLLISION_GROUP_NPC = 9				-- Generic NPC group
COLLISION_GROUP_IN_VEHICLE = 10		-- for any entity inside a vehicle
COLLISION_GROUP_WEAPON = 11			-- for any weapons that need collision detection
COLLISION_GROUP_VEHICLE_CLIP = 12	-- vehicle clip brush to restrict vehicle movement
COLLISION_GROUP_PROJECTILE = 13		-- Projectiles!
COLLISION_GROUP_DOOR_BLOCKER = 14	-- Blocks entities not permitted to get near moving doors
COLLISION_GROUP_PASSABLE_DOOR = 15	-- Doors that the player shouldn't collide with
COLLISION_GROUP_DISSOLVING = 16		-- Things that are dissolving are in this group
COLLISION_GROUP_PUSHAWAY = 17		-- Nonsolid on client and server, pushaway in player code

COLLISION_GROUP_NPC_ACTOR = 18		-- Used so NPCs in scripts ignore the player.
COLLISION_GROUP_NPC_SCRIPTED = 19	-- USed for NPCs in scripts that should not collide with each other

LAST_SHARED_COLLISION_GROUP = 20

SOUND_NORMAL_CLIP_DIST	= 1000.0

-- How many networked area portals do we allow?
MAX_AREA_STATE_BYTES		= 32
MAX_AREA_PORTAL_STATE_BYTES = 24

-- user message max payload size (note, this value is used by the engine, so MODs cannot change it)
MAX_USER_MSG_DATA = 255
MAX_ENTITY_MSG_DATA = 255

--Tony; added for IPlayerInfo V3.
--Putting all standard possible stances, but GetStance in CBasePlayer will only return standing or ducking by default -
--up to the mod to specify the others, or override what GetStance returns.
PINFO_STANCE_STANDING = 0
PINFO_STANCE_DUCKING = 1

PINFO_STANCE_SPRINTING = 2
PINFO_STANCE_PRONE = 3
