--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Extends the weapon type.
--
--===========================================================================--

_R.CBaseCombatWeapon.__WeaponSound = _R.CBaseCombatWeapon.WeaponSound
local WeaponSound = _R.CBaseCombatWeapon.WeaponSound
local gpGlobals = gpGlobals

-- HACKHACK: We override this here, because for some reason scripted weapons
-- don't properly play weapon sounds in singleplayer
function _R.CBaseCombatWeapon.WeaponSound( self, sound_type, soundtime )
  if ( _CLIENT ) then
    return
  end
  
  -- Only the player fires this way so we can cast
  local pPlayer = self:GetOwner();

  -- if ( gpGlobals.maxClients() > 1 ) then
  --   WeaponSound( self, sound_type, soundtime );
  -- else
    local shootsound = self:GetShootSound( sound_type );
    if ( ToBaseEntity( pPlayer ) ~= NULL ) then
      pPlayer:EmitSound( shootsound ); 
    else
      self:EmitSound( shootsound ); 
    end
  -- end
end
