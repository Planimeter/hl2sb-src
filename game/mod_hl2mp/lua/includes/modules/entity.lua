--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Scripted entity implementation.
--
--===========================================================================--

_BASE_ENTITY_CLASS = "prop_scripted"

local table = table
local Warning = dbg.Warning

module( "entity" )

local tEntities = {}

-------------------------------------------------------------------------------
-- Purpose: Returns an entity table
-- Input  : strName - Name of the entity
-- Output : table
-------------------------------------------------------------------------------
function get( strClassname )
  local tEntity = tEntities[ strClassname ]
  if ( not tEntity ) then
    return nil
  end
  tEntity = table.copy( tEntity )
  if ( tEntity.__base ~= strClassname ) then
    local tBaseEntity = get( tEntity.__base )
    if ( not tBaseEntity ) then
      Warning( "WARNING: Attempted to initialize entity \"" .. strClassname .. "\" with non-existing base class!\n" )
    else
      return table.inherit( tEntity, tBaseEntity )
    end
  end
  return tEntity
end

-------------------------------------------------------------------------------
-- Purpose: Returns all registered entities
-- Input  :
-- Output : table
-------------------------------------------------------------------------------
function getentities()
  return tEntities
end

-------------------------------------------------------------------------------
-- Purpose: Registers an entity
-- Input  : tEntity - Entity table
--          strClassname - Name of the entity
--          bReload - Whether or not we're reloading this entity data
-- Output :
-------------------------------------------------------------------------------
function register( tEntity, strClassname, bReload )
  if ( get( strClassname ) ~= nil and bReload ~= true ) then
    return
  end
  tEntities[ strClassname ] = tEntity
end
