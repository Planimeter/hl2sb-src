--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Extends the vgui library.
--
--===========================================================================--

if not _CLIENT then return end

require( "vgui" )

local error = error
local table = table

-- Private list of helpers
local tHelpers = {}

-------------------------------------------------------------------------------
-- Purpose: Registers a panel and creates a factory for it in the vgui
--          namespace
-- Input  : tPanel - Panel table object
--          strName - Name of the panel
--          strBaseClass - Name of the base class
-- Output :
-------------------------------------------------------------------------------
function vgui.register( tPanel, strName, strBaseClass )
  if ( vgui[ strName ] ~= nil ) then
    error( "attempt to register existing panel class \"" .. strName .. "\"", 2 )
  end
  if ( vgui[ strBaseClass ] == nil ) then
    error( "attempt to register panel class with non-existing base class", 2 )
  end
  tPanel.__classname = strName
  tPanel.__base = strBaseClass
  tHelpers[ strName ] = tPanel
  vgui[ strName ] = function( ... )
    local helper = tHelpers[ strName ]
    local panel = vgui[ helper.__base ]( ... )
    table.merge( panel:GetRefTable(), helper )
    panel.BaseClass = tHelpers[ helper.__base ]
    panel:Init( ... )
    return panel
  end
end
