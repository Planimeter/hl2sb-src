--======== Copyleft © 2010-2011, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Dumps an individual library in wiki format for Sandpedia.
--
--===========================================================================--

local curdir = "wiki/libraries/" .. ( _CLIENT and "client" or "server" )
filesystem.CreateDirHierarchy( curdir, "MOD" )

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
  -- Only print tables
  if ( not bBlacklisted and type( t ) == "table" ) then
    table.insert( libraries, library )
  end
  bBlacklisted = false
end

for _, library in pairs( libraries ) do
  local fields = {}

  for field, _ in pairs( _G[ library ] ) do
    table.insert( fields, field )
  end

  table.sort( fields )

  local curfilename = curdir .. "/" .. library .. ".txt"
  local curfile = filesystem.Open( curfilename, "wb" )
  local lines = {}
  for i, field in pairs( fields ) do
    table.insert( lines, "*[[" .. library .. "." .. field .. "|" .. field .. "]]" )
  end
  filesystem.Write( table.concat( lines, "\r\n" ) .. "\r\n", curfile )
  filesystem.Close( curfile )

  local cl_filename = "wiki/libraries/client/" .. library .. ".txt"
  local sv_filename = "wiki/libraries/server/" .. library .. ".txt"
  if ( filesystem.FileExists( cl_filename, "MOD" ) and
       filesystem.FileExists( sv_filename, "MOD" ) ) then
    curfile = filesystem.Open( cl_filename, "rb" )
    local _, cl_file = filesystem.Read( filesystem.Size( curfile ), curfile )
    filesystem.Close( curfile )
    local cl_lines = {}
    for line in string.gmatch( cl_file, "(.-)\r\n" ) do
      table.insert( cl_lines, line )
    end

    curfile = filesystem.Open( sv_filename, "rb" )
    local _, sv_file = filesystem.Read( filesystem.Size( curfile ), curfile )
    filesystem.Close( curfile )
    local sv_lines = {}
    for line in string.gmatch( sv_file, "(.-)\r\n" ) do
      table.insert( sv_lines, line )
    end

    local sh_lines = {}
    for k, cl_line in pairs( cl_lines ) do
      for l, sv_line in pairs( sv_lines ) do
        if ( cl_line == sv_line ) then
          table.insert( sh_lines, cl_line )
          cl_lines[ k ] = nil
          sv_lines[ l ] = nil
        end
      end
    end

    curfile = filesystem.Open( "wiki/libraries/" .. library .. ".txt", "wb" )
    lines = {}
    table.insert( lines, "<table width=\"100%\">" )
    table.insert( lines, "<tr>" )
    if ( #sh_lines > 0 ) then
      table.insert( lines, "<td valign=\"top\">" )
      table.insert( lines, "==Shared==" )
      for _, line in pairs( sh_lines ) do
        table.insert( lines, line )
      end
      table.insert( lines, "</td>" )
    end
    if ( #cl_lines > 0 ) then
      table.insert( lines, "<td valign=\"top\">" )
      table.insert( lines, "==Client==" )
      for _, line in pairs( cl_lines ) do
        table.insert( lines, line )
      end
      table.insert( lines, "</td>" )
    end
    if ( #sv_lines > 0 ) then
      table.insert( lines, "<td valign=\"top\">" )
      table.insert( lines, "==Server==" )
      for _, line in pairs( sv_lines ) do
        table.insert( lines, line )
      end
      table.insert( lines, "</td>" )
    end
    table.insert( lines, "</tr>" )
    table.insert( lines, "</table>" )
    filesystem.Write( table.concat( lines, "\r\n" ) .. "\r\n", curfile )
    filesystem.Close( curfile )
  end
end
