--======== Copyleft © 2010-2011, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Dumps an individual class in wiki format for Sandpedia.
--
--===========================================================================--

local curdir = "wiki/classes/" .. ( _CLIENT and "client" or "server" )
filesystem.CreateDirHierarchy( curdir, "MOD" )

local classes = {}

for k, v in pairs( _R ) do
  -- Only print tables, everything else in _R should be a ref count
  if ( type( k ) ~= "number" and type( v ) == "table" ) then
    if ( v.__index ~= nil and k ~= "FILE*" ) then
      table.insert( classes, k )
    end
  end
end

for _, class in pairs( classes ) do
  local fields = {}

  for field, _ in pairs( _R[ class ] ) do
    table.insert( fields, field )
  end

  table.sort( fields )

  local curfilename = curdir .. "/" .. class .. ".txt"
  local curfile = filesystem.Open( curfilename, "wb" )
  local lines = {}
  for i, field in pairs( fields ) do
    table.insert( lines, "*[[" .. class .. "." .. field .. "|" .. field .. "]]" )
  end
  filesystem.Write( table.concat( lines, "\r\n" ) .. "\r\n", curfile )
  filesystem.Close( curfile )

  local cl_filename = "wiki/classes/client/" .. class .. ".txt"
  local sv_filename = "wiki/classes/server/" .. class .. ".txt"
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

    curfile = filesystem.Open( "wiki/classes/" .. class .. ".txt", "wb" )
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
