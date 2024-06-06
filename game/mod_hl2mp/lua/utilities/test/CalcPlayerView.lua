--======== Copyleft © 2010-2011, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Tests the implementation of CalcPlayerView.
--
--===========================================================================--

local sin = math.sin
local curtime = gpGlobals.curtime

hook.add( "CalcPlayerView", "CalcPlayerView", function( pPlayer, eyeOrigin, eyeAngles, fov )
  -- Give a subtle breathing effect.
  eyeAngles = eyeAngles + QAngle( sin( curtime() ) / 2, 0, 0 )

  -- FIXME: Field of view is just plain broken here. Use pPlayer:SetFOV() to
  -- get around this.

  -- Although ideally you should just return nil for values you don't modify,
  -- if we return nil for the first value, nothing will change at all. This is
  -- due to our hook system's mechanics.
  return eyeOrigin, eyeAngles, fov
end )
