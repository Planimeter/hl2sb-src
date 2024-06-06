--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: builds an intended command to send to the server
--
--===========================================================================--

require( "concommand" )

local function IN_BuildDown( pPlayer, pCmd, args )
	if ( gBuildMenuInterface ) then
		gBuildMenuInterface:ShowPanel( true )
	end
end

concommand.Create( "+buildmenu", IN_BuildDown, nil )

local function IN_BuildUp( pPlayer, pCmd, args )
	if ( gBuildMenuInterface ) then
		gBuildMenuInterface:ShowPanel( false )
	end
end

concommand.Create( "-buildmenu", IN_BuildUp, nil )
