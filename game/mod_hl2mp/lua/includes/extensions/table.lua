--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Extends the table library.
--
--===========================================================================--

require( "table" )

local setmetatable = setmetatable
local getmetatable = getmetatable
local pairs = pairs
local type = type
local print = print
local KeyValues = KeyValues

function table.copy( t, tRecursive )
  if ( t == nil ) then
    return nil
  end
  local __copy = {}
  setmetatable( __copy, getmetatable( t ) )
  for i, v in pairs( t ) do
    if ( type( v ) ~= "table" ) then
      __copy[ i ] = v
    else
      tRecursive = tRecursive or {}
      tRecursive[ t ] = __copy
      if ( tRecursive[ v ] ) then
        __copy[ i ] = tRecursive[ v ]
      else
        __copy[ i ] = table.copy( v, tRecursive )
      end
    end
  end
  return __copy
end

function table.hasvalue( t, val )
  for _, v in pairs( t ) do
    if ( v == val ) then
      return true
    end
  end
  return false
end

function table.inherit( t, BaseClass )
  for k, v in pairs( BaseClass ) do
    if ( t[ k ] == nil ) then
      t[ k ] = v
    end
  end
  t.BaseClass = BaseClass
  return t
end

function table.merge( dest, src )
  if ( type( dest ) ~= "table" ) then
    error( "bad argument #1 to 'merge' (table expected, got " .. type( dest ) .. ")", 2 )
  end
  if ( type( src ) ~= "table" ) then
    error( "bad argument #2 to 'merge' (table expected, got " .. type( src ) .. ")", 2 )
  end
  for k, v in pairs( src ) do
    if ( type( dest[ k ] ) == "table" and type( v ) == "table" ) then
      table.merge( dest[ k ], v )
    else
      dest[ k ] = v
    end
	end
end

function table.print( t, bOrdered, i )
  i = i or 0
  local indent = ""
  for j = 1, i do
    indent = indent .. "\t"
  end
  if ( not bOrdered ) then
    for k, v in pairs( t ) do
      if ( type( v ) == "table" ) then
        print( indent .. k )
        table.print( v, false, i + 1 )
      else
        print( indent .. k, v )
      end
    end
  else
    for j, pair in ipairs( t ) do
      if ( type( pair.value ) == "table" ) then
        print( indent .. pair.key )
        table.print( pair.value, true, i + 1 )
      else
        print( indent .. pair.key, pair.value )
      end
    end
  end
end

function table.tokeyvalues( t, setName, bOrdered )
  local pKV = KeyValues( setName )
  if ( not bOrdered ) then
    for k, v in pairs( t ) do
      if ( type( v ) == "table" ) then
        pKV:AddSubKey( table.tokeyvalues( v, k ) )
      elseif ( type( v ) == "string" ) then
        pKV:SetString( k, v )
      elseif ( type( v ) == "number" ) then
        pKV:SetFloat( k, v )
      elseif ( type( v ) == "color" ) then
        pKV:SetColor( k, v )
      else
        pKV:SetString( k, tostring( v ) )
      end
    end
  else
    for i, pair in ipairs( t ) do
      if ( type( pair.value ) == "table" ) then
        pKV:AddSubKey( table.tokeyvalues( pair.value, pair.key, true ) )
      else
        local pKey = pKV:CreateNewKey()
        pKey:SetName( pair.key )
        pKey:SetStringValue( tostring( pair.value ) )
      end
    end
  end
  return pKV
end
