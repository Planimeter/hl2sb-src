--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Implements addon functionality.
--
--===========================================================================--

local pairs = pairs
local Warning = Warning
local table = table
local filesystem = filesystem

module( "addon" )

local strAddonsFolder = "addons\\"
local tAddons = {}

-------------------------------------------------------------------------------
-- Purpose: Adds an addon to the filesystem
-- Input  : strAddonName - Name of the folder within ..\..\..\addons
--			addType - Enum corresponding to SearchPathAdd_t, determines
--			if the addon overrides files, essentially
-- Output : boolean
-------------------------------------------------------------------------------
function Add( strAddonName, addType )
  for _, addon in pairs( tAddons ) do
    if ( addon:lower() == strAddonName:lower() ) then
	  Warning( "WARNING: Addon \"" .. addon .. "\"is already added!\n" )
	  return false
	end
  end
  if ( not filesystem.FileExists( strAddonsFolder .. strAddonName .. "\\info.txt" ) ) then
    return false
  else
    filesystem.AddSearchPath( strAddonsFolder .. strAddonName, "MOD", addType )
    table.insert( tAddons, strAddonName )
    table.sort( tAddons )
	return true
  end
end

-------------------------------------------------------------------------------
-- Purpose: Returns all of the added addons
-- Input  :
-- Output : table
-------------------------------------------------------------------------------
function GetAddons()
  return tAddons
end

-------------------------------------------------------------------------------
-- Purpose: Removes an addon from the list of added addons
-- Input  : strAddonName - Name of the addon folder
-- Output : boolean
-------------------------------------------------------------------------------
function Remove( strAddonName )
  local bIsAddon = false
  for _, addon in pairs( tAddons ) do
    if ( addon:lower() == strAddonName:lower() ) then
	  bIsAddon = true
	  addon = nil
	  table.sort( tAddons )
	end
  end
  if ( not bIsAddon ) then
    Warning( "WARNING: " .. strAddonName .. " is NOT a valid addon!\n" )
	return false
  end
  filesystem.RemoveSearchPath( strAddonsFolder .. strAddonName, "MOD" )
  return true
end
