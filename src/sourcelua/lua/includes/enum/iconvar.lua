--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose:
--
--===========================================================================--

-------------------------------------------------------------------------------
-- ConVar flags
-------------------------------------------------------------------------------
-- The default, no flags at all
FCVAR_NONE				= 0

-- Command to ConVars and ConCommands
-- ConVar Systems
FCVAR_UNREGISTERED		= (2^0)	-- If this is set, don't add to linked list, etc.
FCVAR_DEVELOPMENTONLY	= (2^1)	-- Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
FCVAR_GAMEDLL			= (2^2)	-- defined by the game DLL
FCVAR_CLIENTDLL			= (2^3)  -- defined by the client DLL
FCVAR_HIDDEN			= (2^4)	-- Hidden. Doesn't appear in find or autocomplete. Like DEVELOPMENTONLY, but can't be compiled out.

-- ConVar only
FCVAR_PROTECTED			= (2^5)  -- It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
FCVAR_SPONLY			= (2^6)  -- This cvar cannot be changed by clients connected to a multiplayer server.
FCVAR_ARCHIVE			= (2^7)	-- set to cause it to be saved to vars.rc
FCVAR_NOTIFY			= (2^8)	-- notifies players when changed
FCVAR_USERINFO			= (2^9)	-- changes the client's info string
FCVAR_CHEAT				= (2^14) -- Only useable in singleplayer / debug / multiplayer & sv_cheats

FCVAR_PRINTABLEONLY		= (2^10)  -- This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
FCVAR_UNLOGGED			= (2^11)  -- If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
FCVAR_NEVER_AS_STRING	= (2^12)  -- never try to print that cvar

-- It's a ConVar that's shared between the client and the server.
-- At signon, the values of all such ConVars are sent from the server to the client (skipped for local
--  client, of course )
-- If a change is requested it must come from the console (i.e., no remote client changes)
-- If a value is changed while a server is active, it's replicated to all connected clients
FCVAR_REPLICATED		= (2^13)	-- server setting enforced on clients, TODO rename to FCAR_SERVER at some time
FCVAR_DEMO				= (2^16)  -- record this cvar when starting a demo file
FCVAR_DONTRECORD		= (2^17)  -- don't record these command in demofiles

FCVAR_NOT_CONNECTED		= (2^22)	-- cvar cannot be changed by a client that is connected to a server

FCVAR_ARCHIVE_XBOX		= (2^24) -- cvar written to config.cfg on the Xbox

FCVAR_SERVER_CAN_EXECUTE	= (2^28)-- the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
FCVAR_SERVER_CANNOT_QUERY	= (2^29)-- If this is set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
FCVAR_CLIENTCMD_CAN_EXECUTE	= (2^30)	-- IVEngineClient::ClientCmd is allowed to execute this command.
											-- Note: IVEngineClient::ClientCmd_Unrestricted can run any client command.

-- FCVAR_AVAILABLE			(2^15)
-- FCVAR_AVAILABLE			(2^18)
-- FCVAR_AVAILABLE			(2^19)
-- FCVAR_AVAILABLE			(2^20)
-- FCVAR_AVAILABLE			(2^21)
-- FCVAR_AVAILABLE			(2^23)
-- FCVAR_AVAILABLE			(2^25)
-- FCVAR_AVAILABLE			(2^26)
-- FCVAR_AVAILABLE			(2^27)
-- FCVAR_AVAILABLE			(2^31)
