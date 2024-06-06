--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Hook implementation.
--
--===========================================================================--

local pairs = pairs
local Warning = dbg.Warning
local tostring = tostring
local pcall = pcall
local unpack = unpack

module( "hook" )

local tHooks = {}
local tReturns = {}

-------------------------------------------------------------------------------
-- Purpose: Adds a hook to the given GameRules function
-- Input  : strEventName - Name of the internal GameRules method
--     	    strHookName - Name of the hook
--          pFn - pointer to function
-- Output :
-------------------------------------------------------------------------------
function add( strEventName, strHookName, pFn )
  tHooks[ strEventName ] = tHooks[ strEventName ] or {}
  tHooks[ strEventName ][ strHookName ] = pFn
end

-------------------------------------------------------------------------------
-- Purpose: Called by the engine to call a GameRules hook
-- Input  : strEventName - Name of the internal GameRules method
--          tGamemode - Table of the current gamemode
-- Output :
-------------------------------------------------------------------------------
function call( strEventName, tGamemode, ... )
  local tHooks = tHooks[ strEventName ]
  if ( tHooks ~= nil ) then
    for k, v in pairs( tHooks ) do
      if ( v == nil ) then
        Warning( "Hook '" .. tostring( k ) .. "' (" .. tostring( strEventName ) .. ") tried to call a nil function!\n" )
        tHooks[ k ] = nil
        break
      else
        tReturns = { pcall( v, ... ) }
        if ( tReturns[ 1 ] == false ) then
          Warning( "Hook '" .. tostring( k ) .. "' (" .. tostring( strEventName ) .. ") Failed: " .. tostring( tReturns[ 2 ] ) .. "\n" )
          tHooks[ k ] = nil
        elseif ( tReturns[ 2 ] ~= nil ) then
          return unpack( tReturns, 2 )
        end
      end
    end
  end
  if ( tGamemode ~= nil ) then
    local fn = tGamemode[ strEventName ]
    if ( fn == nil ) then
      return nil
    else
      tReturns = { pcall( fn, tGamemode, ... ) }
      if ( tReturns[ 1 ] == false ) then
        Warning( "ERROR: GAMEMODE: '" .. tostring( strEventName ) .. "' Failed: " .. tostring( tReturns[ 2 ] ) .. "\n" )
        tGamemode[ strEventName ] = nil
        return nil
      end
      return unpack( tReturns, 2 )
    end
  end
end

-------------------------------------------------------------------------------
-- Purpose: Returns all of the registered hooks or only hooks pertaining to a
--          specific event
-- Input  : strEventName - Name of the internal GameRules method
-- Output : table
-------------------------------------------------------------------------------
function gethooks( strEventName )
  if ( strEventName ) then
    return tHooks[ strEventName ]
  end
  return tHooks
end

-------------------------------------------------------------------------------
-- Purpose: Removes a hook from the list of registered hooks
-- Input  : strEventName - Name of the internal GameRules method
--          strHookName - Name of the hook
-- Output :
-------------------------------------------------------------------------------
function remove( strEventName, strHookName )
  if ( tHooks[ strEventName ][ strHookName ] ) then
    tHooks[ strEventName ][ strHookName ] = nil
  end
end
