--======== Copyleft © 2010-2011, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Dumps a list of enumerations in _E in wiki format for Sandpedia.
--
--===========================================================================--

-- List of enumerations in the Source Engine Lua API

local enumerations = {}

for prefix, t in pairs( _E ) do
  for name, n in pairs( t ) do
    table.insert( enumerations, prefix .. "_" .. name )
  end
end

table.sort( enumerations )

local file = assert( io.open( "enumerations.txt", "wb" ) )
for i, enumeration in ipairs( enumerations ) do
  file:write( "*[[_E." .. enumeration .. "|" .. enumeration .. "]]\r\n" )
end
assert( io.close( file ) )
