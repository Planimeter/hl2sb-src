--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Definitions that are shared by the game DLL and the client DLL.
--
--===========================================================================--

TICK_INTERVAL			= (gpGlobals.interval_per_tick())


function TIME_TO_TICKS( dt )	return ( math.floor( 0.5 + (dt) / TICK_INTERVAL ) ) end
function TICKS_TO_TIME( t )		return ( TICK_INTERVAL *( t ) ) end
function ROUND_TO_TICKS( t )	return ( TICK_INTERVAL * TIME_TO_TICKS( t ) ) end
TICK_NEVER_THINK				= (-1)

ANIMATION_CYCLE_BITS		= 15
ANIMATION_CYCLE_MINFRAC		= (1.0 / (2^ANIMATION_CYCLE_BITS))

-- Height above entity position where the viewer's eye is.
--[[
VEC_VIEW			= g_pGameRules.GetViewVectors().m_vView
VEC_HULL_MIN		= g_pGameRules.GetViewVectors().m_vHullMin
VEC_HULL_MAX		= g_pGameRules.GetViewVectors().m_vHullMax

VEC_DUCK_HULL_MIN	= g_pGameRules.GetViewVectors().m_vDuckHullMin
VEC_DUCK_HULL_MAX	= g_pGameRules.GetViewVectors().m_vDuckHullMax
VEC_DUCK_VIEW		= g_pGameRules.GetViewVectors().m_vDuckView

VEC_OBS_HULL_MIN	= g_pGameRules.GetViewVectors().m_vObsHullMin
VEC_OBS_HULL_MAX	= g_pGameRules.GetViewVectors().m_vObsHullMax

VEC_DEAD_VIEWHEIGHT	= g_pGameRules.GetViewVectors().m_vDeadViewHeight
--]]


WATERJUMP_HEIGHT			= 8

MAX_CLIMB_SPEED		= 200

TIME_TO_DUCK		= 0.4
TIME_TO_DUCK_MS		= 400.0
TIME_TO_UNDUCK		= 0.2
TIME_TO_UNDUCK_MS	= 200.0

MAX_WEAPON_SLOTS		= 6		-- hud item selection slots
MAX_WEAPON_POSITIONS	= 20	-- max number of items within a slot
MAX_ITEM_TYPES			= 6		-- hud item selection slots
MAX_WEAPONS				= 48	-- Max number of weapons available

MAX_ITEMS				= 5		-- hard coded item types

WEAPON_NOCLIP			= -1	-- clip sizes set to this tell the weapon it doesn't use a clip

MAX_AMMO_TYPES	= 32		-- ???
MAX_AMMO_SLOTS  = 32		-- not really slots

HUD_PRINTNOTIFY		= 1
HUD_PRINTCONSOLE	= 2
HUD_PRINTTALK		= 3
HUD_PRINTCENTER		= 4

--===================================================================================================================
-- Close caption flags
CLOSE_CAPTION_WARNIFMISSING	= ( 2^0 )
CLOSE_CAPTION_FROMPLAYER	= ( 2^1 )
CLOSE_CAPTION_GENDER_MALE	= ( 2^2 )
CLOSE_CAPTION_GENDER_FEMALE	= ( 2^3 )

--===================================================================================================================
-- Hud Element hiding flags
HIDEHUD_WEAPONSELECTION		= ( 2^0 )	-- Hide ammo count & weapon selection
HIDEHUD_FLASHLIGHT			= ( 2^1 )
HIDEHUD_ALL					= ( 2^2 )
HIDEHUD_HEALTH				= ( 2^3 )	-- Hide health & armor / suit battery
HIDEHUD_PLAYERDEAD			= ( 2^4 )	-- Hide when local player's dead
HIDEHUD_NEEDSUIT			= ( 2^5 )	-- Hide when the local player doesn't have the HEV suit
HIDEHUD_MISCSTATUS			= ( 2^6 )	-- Hide miscellaneous status elements (trains, pickup history, death notices, etc)
HIDEHUD_CHAT				= ( 2^7 )	-- Hide all communication elements (saytext, voice icon, etc)
HIDEHUD_CROSSHAIR			= ( 2^8 )	-- Hide crosshairs
HIDEHUD_VEHICLE_CROSSHAIR	= ( 2^9 )	-- Hide vehicle crosshair
HIDEHUD_INVEHICLE			= ( 2^10 )
HIDEHUD_BONUS_PROGRESS		= ( 2^11 )	-- Hide bonus progress display (for bonus map challenges)

HIDEHUD_BITCOUNT			= 12

--===================================================================================================================
-- suit usage bits
bits_SUIT_DEVICE_SPRINT		= 0x00000001
bits_SUIT_DEVICE_FLASHLIGHT	= 0x00000002
bits_SUIT_DEVICE_BREATHER	= 0x00000004

MAX_SUIT_DEVICES			= 3


--===================================================================================================================
-- Player Defines

-- Max number of players in a game ( see const.h for ABSOLUTE_PLAYER_LIMIT (256 ) )
-- The Source engine is really designed for 32 or less players.  If you raise this number above 32, you better know what you are doing
--  and have a good answer for a bunch of perf question related to player simulation, thinking logic, tracelines, networking overhead, etc.
-- But if you are brave or are doing something interesting, go for it...   ywb 9/22/03

--You might be wondering why these aren't multiple of 2. Well the reason is that if servers decide to have HLTV enabled we need the extra slot.
--This is ok since MAX_PLAYERS is used for code specific things like arrays and loops, but it doesn't really means that this is the max number of players allowed
--Since this is decided by the gamerules (and it can be whatever number as long as its less than MAX_PLAYERS).
MAX_PLAYERS					= 33  -- Absolute max players supported

MAX_PLACE_NAME_LENGTH		= 18

--===================================================================================================================
-- Team Defines
TEAM_INVALID			= -1
TEAM_UNASSIGNED			= 0	-- not assigned to a team
TEAM_SPECTATOR			= 1	-- spectator team
-- Start your team numbers after this
LAST_SHARED_TEAM		= TEAM_SPECTATOR

-- The first team that's game specific (i.e. not unassigned / spectator)
FIRST_GAME_TEAM			= (LAST_SHARED_TEAM+1)

MAX_TEAMS				= 32	-- Max number of teams in a game
MAX_TEAM_NAME_LENGTH	= 32	-- Max length of a team's name

-- Weapon m_iState
WEAPON_IS_ONTARGET				= 0x40

WEAPON_NOT_CARRIED				= 0	-- Weapon is on the ground
WEAPON_IS_CARRIED_BY_PLAYER		= 1	-- This client is carrying this weapon.
WEAPON_IS_ACTIVE				= 2	-- This client is carrying this weapon and it's the currently held weapon

-- -----------------------------------------
-- Skill Level
-- -----------------------------------------
SKILL_EASY		= 1
SKILL_MEDIUM	= 2
SKILL_HARD		= 3


-- Weapon flags
-- -----------------------------------------
--	Flags - NOTE: KEEP g_ItemFlags IN WEAPON_PARSE.CPP UPDATED WITH THESE
-- -----------------------------------------
ITEM_FLAG_SELECTONEMPTY		= (2^0)
ITEM_FLAG_NOAUTORELOAD		= (2^1)
ITEM_FLAG_NOAUTOSWITCHEMPTY	= (2^2)
ITEM_FLAG_LIMITINWORLD		= (2^3)
ITEM_FLAG_EXHAUSTIBLE		= (2^4)	-- A player can totally exhaust their ammo supply and lose this weapon
ITEM_FLAG_DOHITLOCATIONDMG	= (2^5)	-- This weapon take hit location into account when applying damage
ITEM_FLAG_NOAMMOPICKUPS		= (2^6)	-- Don't draw ammo pickup sprites/sounds when ammo is received
ITEM_FLAG_NOITEMPICKUP		= (2^7)	-- Don't draw weapon pickup when this weapon is picked up by the player
-- NOTE: KEEP g_ItemFlags IN WEAPON_PARSE.CPP UPDATED WITH THESE


-- Humans only have left and right hands, though we might have aliens with more
--  than two, sigh
MAX_VIEWMODELS			= 2

MAX_BEAM_ENTS			= 10

TRACER_TYPE_DEFAULT		= 0x00000001
TRACER_TYPE_GUNSHIP		= 0x00000002
TRACER_TYPE_STRIDER		= 0x00000004 -- Here ya go, Jay!
TRACER_TYPE_GAUSS		= 0x00000008
TRACER_TYPE_WATERBULLET	= 0x00000010

MUZZLEFLASH_TYPE_DEFAULT	= 0x00000001
MUZZLEFLASH_TYPE_GUNSHIP	= 0x00000002
MUZZLEFLASH_TYPE_STRIDER	= 0x00000004

-- Muzzle flash definitions (for the flags field of the "MuzzleFlash" DispatchEffect)
MUZZLEFLASH_AR2				= 0
MUZZLEFLASH_SHOTGUN			= 1
MUZZLEFLASH_SMG1			= 2
MUZZLEFLASH_SMG2			= 3
MUZZLEFLASH_PISTOL			= 4
MUZZLEFLASH_COMBINE			= 5
MUZZLEFLASH_357				= 6
MUZZLEFLASH_RPG				= 7
MUZZLEFLASH_COMBINE_TURRET	= 8

MUZZLEFLASH_FIRSTPERSON		= 0x100

-- Tracer Flags
TRACER_FLAG_WHIZ			= 0x0001
TRACER_FLAG_USEATTACHMENT	= 0x0002

TRACER_DONT_USE_ATTACHMENT	= -1

-- Entity Dissolve types
ENTITY_DISSOLVE_NORMAL = 0
ENTITY_DISSOLVE_ELECTRICAL = 1
ENTITY_DISSOLVE_ELECTRICAL_LIGHT = 2
ENTITY_DISSOLVE_CORE = 3

-- NOTE: Be sure to up the bits if you make more dissolve types
ENTITY_DISSOLVE_BITS = 3

-- ---------------------------
--  Hit Group standards
-- ---------------------------
HITGROUP_GENERIC	= 0
HITGROUP_HEAD		= 1
HITGROUP_CHEST		= 2
HITGROUP_STOMACH	= 3
HITGROUP_LEFTARM	= 4
HITGROUP_RIGHTARM	= 5
HITGROUP_LEFTLEG	= 6
HITGROUP_RIGHTLEG	= 7
HITGROUP_GEAR		= 10			-- alerts NPC, but doesn't do damage or bleed (1/100th damage)

--
-- Enumerations for setting player animation.
--
PLAYER_IDLE = 1
PLAYER_WALK = 2
PLAYER_JUMP = 3
PLAYER_SUPERJUMP = 4
PLAYER_DIE = 5
PLAYER_ATTACK1 = 6
PLAYER_IN_VEHICLE = 7

-- TF Player animations
PLAYER_RELOAD = 8
PLAYER_START_AIMING = 9
PLAYER_LEAVE_AIMING = 10

-- HL2 has 600 gravity by default
-- NOTE: The discrete ticks can have quantization error, so these numbers are biased a little to
-- make the heights more exact
PLAYER_FATAL_FALL_SPEED		= 1024 -- approx 60 feet
PLAYER_MAX_SAFE_FALL_SPEED	= 580 -- approx 20 feet
PLAYER_LAND_ON_FLOATING_OBJECT	= 200 -- Can go another 200 units without getting hurt
PLAYER_MIN_BOUNCE_SPEED		= 200
PLAYER_FALL_PUNCH_THRESHOLD = 350 -- won't punch player's screen/make scrape noise unless player falling at least this fast.
DAMAGE_FOR_FALL_SPEED		= 100.0 / ( PLAYER_FATAL_FALL_SPEED - PLAYER_MAX_SAFE_FALL_SPEED ) -- damage per unit per second.


AUTOAIM_2DEGREES  = 0.0348994967025
AUTOAIM_5DEGREES  = 0.08715574274766
AUTOAIM_8DEGREES  = 0.1391731009601
AUTOAIM_10DEGREES = 0.1736481776669
AUTOAIM_20DEGREES = 0.3490658503989

AUTOAIM_SCALE_DEFAULT		= 1.0
AUTOAIM_SCALE_DIRECT_ONLY	= 0.0

-- instant damage

-- For a means of resolving these consts into debug string text, see function
-- CTakeDamageInfo::DebugGetDamageTypeString(unsigned int DamageType, char *outbuf, unsigned int outbuflength )
DMG_GENERIC			= 0			-- generic damage was done
DMG_CRUSH			= (2^ 0)	-- crushed by falling or moving object.
										-- NOTE: It's assumed crush damage is occurring as a result of physics collision, so no extra physics force is generated by crush damage.
										-- DON'T use DMG_CRUSH when damaging entities unless it's the result of a physics collision. You probably want DMG_CLUB instead.
DMG_BULLET			= (2^ 1)	-- shot
DMG_SLASH			= (2^ 2)	-- cut, clawed, stabbed
DMG_BURN			= (2^ 3)	-- heat burned
DMG_VEHICLE			= (2^ 4)	-- hit by a vehicle
DMG_FALL			= (2^ 5)	-- fell too far
DMG_BLAST			= (2^ 6)	-- explosive blast damage
DMG_CLUB			= (2^ 7)	-- crowbar, punch, headbutt
DMG_SHOCK			= (2^ 8)	-- electric shock
DMG_SONIC			= (2^ 9)	-- sound pulse shockwave
DMG_ENERGYBEAM		= (2^ 10)	-- laser or other high energy beam
DMG_PREVENT_PHYSICS_FORCE		= (2^ 11)	-- Prevent a physics force
DMG_NEVERGIB		= (2^ 12)	-- with this bit OR'd in, no damage type will be able to gib victims upon death
DMG_ALWAYSGIB		= (2^ 13)	-- with this bit OR'd in, any damage type can be made to gib victims upon death.
DMG_DROWN			= (2^ 14)	-- Drowning


DMG_PARALYZE		= (2^ 15)	-- slows affected creature down
DMG_NERVEGAS		= (2^ 16)	-- nerve toxins, very bad
DMG_POISON			= (2^ 17)	-- blood poisoning - heals over time like drowning damage
DMG_RADIATION		= (2^ 18)	-- radiation exposure
DMG_DROWNRECOVER	= (2^ 19)	-- drowning recovery
DMG_ACID			= (2^ 20)	-- toxic chemicals or acid burns
DMG_SLOWBURN		= (2^ 21)	-- in an oven

DMG_REMOVENORAGDOLL	= (2^22)	-- with this bit OR'd in, no ragdoll will be created, and the target will be quietly removed.
										-- use this to kill an entity that you've already got a server-side ragdoll for

DMG_PHYSGUN			= (2^23)	-- Hit by manipulator. Usually doesn't do any damage.
DMG_PLASMA			= (2^24)	-- Shot by Cremator
DMG_AIRBOAT			= (2^25)	-- Hit by the airboat's gun

DMG_DISSOLVE		= (2^26)	-- Dissolving!
DMG_BLAST_SURFACE	= (2^27)	-- A blast on the surface of water that cannot harm things underwater
DMG_DIRECT			= (2^28)
DMG_BUCKSHOT		= (2^29)	-- not quite a bullet. Little, rounder, different.

-- NOTE: DO NOT ADD ANY MORE CUSTOM DMG_ TYPES. MODS USE THE DMG_LASTGENERICFLAG BELOW, AND
--		 IF YOU ADD NEW DMG_ TYPES, THEIR TYPES WILL BE HOSED. WE NEED A BETTER SOLUTION.

-- TODO: keep this up to date so all the mod-specific flags don't overlap anything.
DMG_LASTGENERICFLAG	= DMG_BUCKSHOT



-- settings for m_takedamage
DAMAGE_NO				= 0
DAMAGE_EVENTS_ONLY		= 1		-- Call damage functions, but don't modify health
DAMAGE_YES				= 2
DAMAGE_AIM				= 3

-- Spectator Movement modes
OBS_MODE_NONE = 0		-- not in spectator mode
OBS_MODE_DEATHCAM = 1	-- special mode for death cam animation
OBS_MODE_FREEZECAM = 2	-- zooms to a target, and freeze-frames on them
OBS_MODE_FIXED = 3		-- view from a fixed camera position
OBS_MODE_IN_EYE = 4	-- follow a player in first person view
OBS_MODE_CHASE = 5		-- follow a player in third person view
OBS_MODE_ROAMING = 6	-- free roaming

NUM_OBSERVER_MODES = 7

LAST_PLAYER_OBSERVERMODE	= OBS_MODE_ROAMING

-- Force Camera Restrictions with mp_forcecamera
OBS_ALLOW_ALL = 0	-- allow all modes, all targets
OBS_ALLOW_TEAM = 1	-- allow only own team & first person, no PIP
OBS_ALLOW_NONE = 2	-- don't allow any spectating after death (fixed & fade to black)

OBS_ALLOW_NUM_MODES = 3

TYPE_TEXT = 0	-- just display this plain text
TYPE_INDEX = 1	-- lookup text & title in stringtable
TYPE_URL = 2	-- show this URL
TYPE_FILE = 3	-- show this local file

-- VGui Screen Flags
VGUI_SCREEN_ACTIVE = 0x1
VGUI_SCREEN_VISIBLE_TO_TEAMMATES = 0x2
VGUI_SCREEN_ATTACHED_TO_VIEWMODEL = 0x4
VGUI_SCREEN_TRANSPARENT = 0x8
VGUI_SCREEN_ONLY_USABLE_BY_OWNER = 0x10

VGUI_SCREEN_MAX_BITS = 5

USE_OFF = 0
USE_ON = 1
USE_SET = 2
USE_TOGGLE = 3

-- basic team colors
COLOR_RED		= Color(255, 64, 64, 255)
COLOR_BLUE		= Color(153, 204, 255, 255)
COLOR_YELLOW	= Color(255, 178, 0, 255)
COLOR_GREEN		= Color(153, 255, 153, 255)
COLOR_GREY		= Color(204, 204, 204, 255)

-- All NPCs need this data
DONT_BLEED = -1

BLOOD_COLOR_RED = 0
BLOOD_COLOR_YELLOW = 1
BLOOD_COLOR_GREEN = 2
BLOOD_COLOR_MECH = 3

BLOOD_COLOR_ANTLION = 1		-- FIXME: Move to Base HL2
BLOOD_COLOR_ZOMBIE = 2		-- FIXME: Move to Base HL2
BLOOD_COLOR_ANTLION_WORKER = 3

-------------------------------------------------------------------------------
-- Vehicles may have more than one passenger.
-- This enum may be expanded by derived classes
-------------------------------------------------------------------------------
VEHICLE_ROLE_NONE = -1

VEHICLE_ROLE_DRIVER = 0		-- Only one driver

LAST_SHARED_VEHICLE_ROLE = 1

-------------------------------------------------------------------------------
-- Water splash effect flags
-------------------------------------------------------------------------------
FX_WATER_IN_SLIME = 0x1


-- Shared think context stuff
MAX_CONTEXT_LENGTH		= 32
NO_THINK_CONTEXT	= -1

-- entity flags, CBaseEntity::m_iEFlags
EFL_KILLME	=				(2^0)	-- This entity is marked for death -- This allows the game to actually delete ents at a safe time
EFL_DORMANT	=				(2^1)	-- Entity is dormant, no updates to client
EFL_NOCLIP_ACTIVE =			(2^2)	-- Lets us know when the noclip command is active.
EFL_SETTING_UP_BONES =		(2^3)	-- Set while a model is setting up its bones.
EFL_KEEP_ON_RECREATE_ENTITIES = (2^4) -- This is a special entity that should not be deleted when we restart entities only

--Tony; BUG?? I noticed this today while performing stealz on flag 16! look at the definition of the flag above...
EFL_HAS_PLAYER_CHILD=		(2^4)	-- One of the child entities is a player.

EFL_DIRTY_SHADOWUPDATE =	(2^5)	-- Client only- need shadow manager to update the shadow...
EFL_NOTIFY =				(2^6)	-- Another entity is watching events on this entity (used by teleport)

-- The default behavior in ShouldTransmit is to not send an entity if it doesn't
-- have a model. Certain entities want to be sent anyway because all the drawing logic
-- is in the client DLL. They can set this flag and the engine will transmit them even
-- if they don't have a model.
EFL_FORCE_CHECK_TRANSMIT =	(2^7)

EFL_BOT_FROZEN =			(2^8)	-- This is set on bots that are frozen.
EFL_SERVER_ONLY =			(2^9)	-- Non-networked entity.
EFL_NO_AUTO_EDICT_ATTACH =	(2^10) -- Don't attach the edict; we're doing it explicitly

-- Some dirty bits with respect to abs computations
EFL_DIRTY_ABSTRANSFORM =	(2^11)
EFL_DIRTY_ABSVELOCITY =		(2^12)
EFL_DIRTY_ABSANGVELOCITY =	(2^13)
EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS	= (2^14)
EFL_DIRTY_SPATIAL_PARTITION = (2^15)
EFL_PLUGIN_BASED_BOT		= (2^16)		--this is set on plugin bots, so that if any games include their own bot code, they won't affect plugin bots.
EFL_IN_SKYBOX =				(2^17)	-- This is set if the entity detects that it's in the skybox.
										-- This forces it to pass the "in PVS" for transmission.
EFL_USE_PARTITION_WHEN_NOT_SOLID = (2^18)	-- Entities with this flag set show up in the partition even when not solid
EFL_TOUCHING_FLUID =		(2^19)	-- Used to determine if an entity is floating

-- FIXME: Not really sure where I should add this...
EFL_IS_BEING_LIFTED_BY_BARNACLE = (2^20)
EFL_NO_ROTORWASH_PUSH =		(2^21)		-- I shouldn't be pushed by the rotorwash
EFL_NO_THINK_FUNCTION =		(2^22)
EFL_NO_GAME_PHYSICS_SIMULATION = (2^23)

EFL_CHECK_UNTOUCH =			(2^24)
EFL_DONTBLOCKLOS =			(2^25)			-- I shouldn't block NPC line-of-sight
EFL_DONTWALKON =			(2^26)			-- NPC;s should not walk on this entity
EFL_NO_DISSOLVE =			(2^27)			-- These guys shouldn't dissolve
EFL_NO_MEGAPHYSCANNON_RAGDOLL = (2^28)		-- Mega physcannon can't ragdoll these guys.
EFL_NO_WATER_VELOCITY_CHANGE  =	(2^29)		-- Don't adjust this entity's velocity when transitioning into water
EFL_NO_PHYSCANNON_INTERACTION =	(2^30)		-- Physcannon can't pick these up or punt them
EFL_NO_DAMAGE_FORCES =		(2^31)			-- Doesn't accept forces from physics damage

-------------------------------------------------------------------------------
-- EFFECTS
-------------------------------------------------------------------------------
FX_BLOODSPRAY_DROPS	= 0x01
FX_BLOODSPRAY_GORE	= 0x02
FX_BLOODSPRAY_CLOUD	= 0x04
FX_BLOODSPRAY_ALL	= 0xFF

-------------------------------------------------------------------------------
MAX_SCREEN_OVERLAYS		= 10

-- These are the types of data that hang off of CBaseEntities and the flag bits used to mark their presence
GROUNDLINK = 0
TOUCHLINK = 1
STEPSIMULATION = 2
MODELWIDTHSCALE = 3
POSITIONWATCHER = 4
PHYSICSPUSHLIST = 5
VPHYSICSUPDATEAI = 6
VPHYSICSWATCHER = 7

-- Must be last and <= 32
NUM_DATAOBJECT_TYPES = 8

-------------------------------------------------------------------------------
-- Bullet firing information
-------------------------------------------------------------------------------
FIRE_BULLETS_FIRST_SHOT_ACCURATE = 0x1		-- Pop the first shot with perfect accuracy
FIRE_BULLETS_DONT_HIT_UNDERWATER = 0x2		-- If the shot hits its target underwater, don't damage it
FIRE_BULLETS_ALLOW_WATER_SURFACE_IMPACTS = 0x4	-- If the shot hits water surface, still call DoImpactEffect
FIRE_BULLETS_TEMPORARY_DANGER_SOUND = 0x8		-- Danger sounds added from this impact can be stomped immediately if another is queued


MAX_ACTORS_IN_SCENE = 16

-------------------------------------------------------------------------------
-- Multiplayer specific defines
-------------------------------------------------------------------------------
MAX_CONTROL_POINTS			= 8
MAX_CONTROL_POINT_GROUPS	= 8

-- Maximum number of points that a control point may need owned to be cappable
MAX_PREVIOUS_POINTS			= 3

-- The maximum number of teams the control point system knows how to deal with
MAX_CONTROL_POINT_TEAMS		= 8

-- Maximum length of the cap layout string
MAX_CAPLAYOUT_LENGTH		= 32

-- Maximum length of the current round printname
MAX_ROUND_NAME				= 32

-- Maximum length of the current round name
MAX_ROUND_IMAGE_NAME		= 64

-- Score added to the team score for a round win
TEAMPLAY_ROUND_WIN_SCORE	= 1

CP_WARN_NORMAL = 0
CP_WARN_FINALCAP = 1
CP_WARN_NO_ANNOUNCEMENTS = 2

-- YWB:  3/12/2007
-- Changing the following for Prediction Error checking (See gamemovement.cpp for overview) will to 1 or 2 enables the system, 0 turns it off
-- Level 1 enables it, but doesn't force "full precision" networking, so you can still get lots of errors in position/velocity/etc.
-- Level 2 enables it but also forces origins/angles to be sent full precision, so other fields can be error / tolerance checked
-- NOTE:  This stuff only works on a listen server since it punches a hole from the client .dll to server .dll!!!
PREDICTION_ERROR_CHECK_LEVEL = 0

-------------------------------------------------------------------------------
-- Round timer states
-------------------------------------------------------------------------------
RT_STATE_SETUP = 1	-- Timer is in setup mode
RT_STATE_NORMAL = 2	-- Timer is in normal mode

SIMULATION_TIME_WINDOW_BITS = 8

-------------------------------------------------------------------------------
-- Commentary Mode
-------------------------------------------------------------------------------
-- The player's method of starting / stopping commentary
COMMENTARY_BUTTONS		= (IN_USE)

-- Teamplay Roundbased Game rules shared
TPRBGAMEINFO_GAMESTATE = 1					--gets the state of the current game (waiting for players, setup, active, overtime, stalemate, roundreset)
TPRBGAMEINFO_RESERVED1 = 2
TPRBGAMEINFO_RESERVED2 = 3
TPRBGAMEINFO_RESERVED3 = 4
TPRBGAMEINFO_RESERVED4 = 5
TPRBGAMEINFO_RESERVED5 = 6
TPRBGAMEINFO_RESERVED6 = 7
TPRBGAMEINFO_RESERVED7 = 8
TPRBGAMEINFO_RESERVED8 = 9

TPRBGAMEINFO_LASTGAMEINFO = 10
-- Mark it off so valvegame_plugin_def.h ignores it, if both headers are included in a plugin.
TPRBGAMEINFO_x = 1

--Tony; (t)eam(p)lay(r)ound(b)ased gamerules -- Game Info values
TPRB_STATE_WAITING				= (2^0)
TPRB_STATE_SETUP				= (2^1)
TPRB_STATE_ACTIVE				= (2^2)
TPRB_STATE_ROUNDWON				= (2^3)
TPRB_STATE_OVERTIME				= (2^4)
TPRB_STATE_STALEMATE			= (2^5)
TPRB_STATE_ROUNDRESET			= (2^6)
TPRB_STATE_WAITINGREADYSTART	= (2^7)
