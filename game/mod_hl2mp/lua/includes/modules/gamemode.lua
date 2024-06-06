--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Gamemode handling.
--
--===========================================================================--

_BASE_GAMEMODE = "deathmatch"

require( "hook" )

local hook = hook
local table = table
local print = print
local _BASE_GAMEMODE = _BASE_GAMEMODE
local _G = _G

module( "gamemode" )

local tGamemodes = {}

-------------------------------------------------------------------------------
-- Purpose: Calls a gamemode function
-- Input  : strEventName - Name of the internal GameRules method
-- Output :
-------------------------------------------------------------------------------
function call( strEventName, ... )
  if ( _G._GAMEMODE and _G._GAMEMODE[ strEventName ] == nil ) then
    return false
  end
  return hook.call( strEventName, _G._GAMEMODE, ... )
end

-------------------------------------------------------------------------------
-- Purpose: Returns a gamemode table object
-- Input  : strName - Name of the gamemode
-- Output : table
-------------------------------------------------------------------------------
function get( strName )
  return tGamemodes[ strName ]
end

-------------------------------------------------------------------------------
-- Purpose: Registers a gamemode
-- Input  : tGamemode - Gamemode table object
--          strName - Name of the gamemode
--          strBaseClass - Name of the base class
-- Output :
-------------------------------------------------------------------------------
function register( tGamemode, strName, strBaseClass )
  if ( get( strName ) ~= nil and _G._GAMEMODE ~= nil ) then
    tGamemode = table.inherit( tGamemode, _G._GAMEMODE )
  end
  if ( strName ~= _BASE_GAMEMODE ) then
    tGamemode = table.inherit( tGamemode, get( strBaseClass ) )
  end
  tGamemodes[ strName ] = tGamemode
end
