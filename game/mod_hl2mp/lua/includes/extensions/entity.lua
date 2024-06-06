--========== Copyleft © 2013, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Extends the entity type.
--
--===========================================================================--

local type = type

function ToBaseEntity( pEntity )
  if ( not pEntity or type( pEntity ) ~= "entity" ) then
    return NULL;
  end

  local success, hEntity = pcall( _R.CBaseEntity.GetBaseEntity, pEntity )
  if ( not success ) then
    hEntity = NULL
  end
if _DEBUG then
  assert( hEntity ~= NULL );
end

  return hEntity;
end
