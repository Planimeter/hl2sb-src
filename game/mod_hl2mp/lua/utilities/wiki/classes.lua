--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Dumps a list of all metatables in _R in wiki format for Sandpedia.
--
--===========================================================================--

-- List of classes in the Source Engine Lua API

local tMetatables = {}

for k, v in pairs( _R ) do
  -- Only print tables, everything else in _R should be a ref count
  if ( type( k ) ~= "number" and type( v ) == "table" ) then
    if ( v.__index ~= nil ) then
      table.insert( tMetatables, k )
    end
  end
end

table.sort( tMetatables )

local file = assert( io.open( "classes.txt", "wb" ) )
for i, metatable in pairs( tMetatables ) do
  file:write( "*[[" .. metatable .. "]]\r\n" )
end
assert( io.close( file ) )
