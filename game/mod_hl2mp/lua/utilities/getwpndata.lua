--======== Copyleft © 2010-2011, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Dump weapon data from WeaponInfo_t.
--
--===========================================================================--

for k, v in pairs( UTIL.PlayerByIndex( 1 ):GetActiveWeapon():GetWpnData() ) do
  if ( type( v ) == "table" ) then
    print( k .. ":" )
	for l, w in pairs( v ) do
	  print( "\t" .. l, w )
	end
  else
    print( k, v )
  end
end
