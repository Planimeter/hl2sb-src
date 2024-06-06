--========== Copyleft © 2013, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Dumps a list of globals from Lua to be used as a blacklist.
--
--===========================================================================--

local globals = {}

for global, v in pairs( _G ) do
  if ( type( v ) ~= "number" and type( v ) ~= "table" ) then
    table.insert( globals, global )
  end
end

table.sort( globals )

local file = assert( io.open( "globals.txt", "wb" ) )
file:write( "local blacklist = {\r\n" )
for i, global in ipairs( globals ) do
  file:write( "  \"" .. global .. "\"" .. ( ( i ~= #globals ) and "," or "" )  .. "\r\n" )
end
file:write( "}\r\n" )
assert( io.close( file ) )
