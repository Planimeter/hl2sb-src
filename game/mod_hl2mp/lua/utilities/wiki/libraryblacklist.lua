--========== Copyleft © 2013, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Dumps a list of libraries from Lua to be used as a blacklist.
--
--===========================================================================--

local libraries = {}

for library, v in pairs( _G ) do
  if ( type( v ) == "table" and library ~= "args" ) then
    table.insert( libraries, library )
  end
end

table.sort( libraries )

local file = assert( io.open( "libraries.txt", "wb" ) )
file:write( "local blacklist = {\r\n" )
for i, library in ipairs( libraries ) do
  file:write( "  \"" .. library .. "\"" .. ( ( i ~= #libraries ) and "," or "" )  .. "\r\n" )
end
file:write( "}\r\n" )
assert( io.close( file ) )
