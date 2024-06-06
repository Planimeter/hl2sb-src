--======== Copyleft © 2010-2011, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Dumps a list of libraries in _G in wiki format for Sandpedia.
--
--===========================================================================--

-- List of libraries in the Source Engine Lua API

local blacklist = {
  -- Source Engine Lua API enumerations
  "_E",
  -- Source Engine Lua API table objects
  "_GAMEMODE",
  -- Source Engine Lua API registry pseudo-index
  "_R",

  -- Lua global pseudo-index
  "_G",

  -- Lua Standard Libraries
  "coroutine",
  "debug",
  "io",
  "math",
  "os",
  "package",
  "string",
  "table"
}

local bBlacklisted = false
local libraries = {}

for library, t in pairs( _G ) do
  for _, v in pairs( blacklist ) do
    if ( library == v ) then
      bBlacklisted = true
    end
  end
  if ( not bBlacklisted and type( t ) == "table" ) then
    table.insert( libraries, library )
  end
  bBlacklisted = false
end

table.sort( libraries )

local file = assert( io.open( "libraries.txt", "wb") )
for i, library in pairs( libraries ) do
  file:write( "*[[" .. library .. "]]\r\n" )
end
assert( io.close( file ) )
