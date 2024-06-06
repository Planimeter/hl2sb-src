--======== Copyleft © 2010-2011, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Tests the implementation of CEffectData.
--
--===========================================================================--

hook.add( "PlayerTraceAttack", "PlayerTraceTeslaHitboxesAttack", function( pPlayer, info, vecDir, ptr )
  local pEntity = ptr.m_pEnt
  if ( pEntity ~= NULL ) then
    -- This is taken straight out of CNPC_Hunter::TeslaThink()
	local data = CEffectData();
	if ( _CLIENT ) then
	  data.m_hEntity = pEntity
	else
	  data.m_nEntIndex = pEntity:entindex();
	end
	data.m_flMagnitude = 3;
	data.m_flScale = 0.5;
	DispatchEffect( "TeslaHitboxes", data );
  end
end )
