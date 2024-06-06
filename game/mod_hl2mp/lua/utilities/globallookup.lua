--======== Copyleft © 2010-2011, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Dump the size of _G.
--
--===========================================================================--

local n = 0
for k, v in pairs( _G ) do
  n = n + 1
end

print( n )
