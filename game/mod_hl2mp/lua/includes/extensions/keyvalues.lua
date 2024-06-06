--========== Copyleft © 2013, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Extends the keyvalues type.
--
--===========================================================================--

local print = print

function _R.KeyValues.Print( self, i )
  i = i or 0
  local indent = ""
  for j = 1, i do
    indent = indent .. "\t"
  end
  local kv = self:GetFirstSubKey()
  while kv ~= NULL_KEYVALUES do
    local k, v = kv:GetName(), kv:GetString()
    print( indent .. k, v )
    kv:Print( i + 1 )
    kv = kv:GetNextKey()
  end
end

function _R.KeyValues.ToTable( self, bPreserveOrder )
  local i = 1
  local t = {}
  local pair = self:GetFirstSubKey()
  while pair ~= NULL_KEYVALUES do
    local k, v = pair:GetName(), pair:GetString()
    if ( not bPreserveOrder ) then
      t[ k ] = v
    else
      t[ i ] = {
        key = k,
        value = v
      }
    end
    local bKeyValueIsEmpty
    if ( not bPreserveOrder ) then
      bKeyValueIsEmpty = t[ k ] == ""
    else
      bKeyValueIsEmpty = t[ i ].value == ""
    end
    if ( bKeyValueIsEmpty and self:GetDataType( pair:GetName() ) == 0 ) then
      if ( not bPreserveOrder ) then
        t[ k ] = pair:ToTable()
      else
        t[ i ].value = pair:ToTable( true )
      end
      local isEmpty = true
      if ( not bPreserveOrder ) then
        for l, w in pairs( t[ k ] ) do
          if ( w ) then
            isEmpty = false
            break
          end
        end
      else
        for l, w in ipairs( t[ i ].value ) do
          if ( w ) then
            isEmpty = false
            break
          end
        end
      end
      if ( isEmpty ) then
        if ( not bPreserveOrder ) then
          t[ k ] = ""
        else
          t[ i ] = {
            key = k,
            value = ""
          }
        end
      end
    end
    pair = pair:GetNextKey()
    i = i + 1
  end
  return t
end
