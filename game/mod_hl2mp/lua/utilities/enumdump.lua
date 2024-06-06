--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Dumps a list of all enumeration libraries in _E
--
--===========================================================================--

local function kpairs( t, f )
  local a = {}
  for n in pairs( t ) do
  	table.insert( a, n )
  end
  table.sort( a, f )
  local i = 0
  local iter = function()
    i = i + 1
    if ( a[ i ] == nil ) then
      return nil
    else
      return a[ i ], t[ a[ i ] ]
    end
  end
  return iter
end

local function vpairs( t, f )
  local a = {}
  for _, n in pairs( t ) do
  	table.insert( a, n )
  end
  table.sort( a, f )
  local i = 0
  local iter = function()
    i = i + 1
    if ( a[ i ] == nil ) then
      return nil
    else
      local k = nil
      for lookup, v in pairs( t ) do
      	if ( v == a[ i ] ) then
      	  k = lookup
      	end
      end
      return k, a[ i ]
    end
  end
  return iter
end

for name, lib in kpairs( _E ) do
  print( "_E." .. name .. ": " .. tostring( lib ) )
  for enum, v in vpairs( lib ) do
  	print( "  _E." .. name .. "." .. enum .. ": " .. v )
  end
end
