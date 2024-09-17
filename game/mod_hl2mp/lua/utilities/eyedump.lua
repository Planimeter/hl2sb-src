--======== Copyleft © 2010-2011, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Dump our eye origin and angles.
--
--===========================================================================--

hook.Add( "CalcPlayerView", "DumpPlayerView", function( pPlayer, eyeOrigin, eyeAngles, fov )
  print( "eyeOrigin: " .. tostring( eyeOrigin ) )
  print( "eyeAngles: " .. tostring( eyeAngles ) )
  print( "fov: " .. tostring( fov ) )
  hook.Remove( "CalcPlayerView", "CalcPlayerView" )
end )
