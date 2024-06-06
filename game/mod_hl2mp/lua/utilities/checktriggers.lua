--======== Copyleft © 2010-2011, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Checks the existence of trigger_changelevel*.
--
--===========================================================================--

local pTrigger = gEntList.FindEntityByClassname( NULL, "trigger_changelevel" )
local tTriggers = {}
while ( pTrigger ~= NULL ) do
  print( pTrigger )
  table.insert( tTriggers, pTrigger )
  pTrigger = gEntList.FindEntityByClassname( pTrigger, "trigger_changelevel" )
end

print( "Found " .. #tTriggers .. " trigger_changelevels!" )

for i, pTrigger in ipairs( tTriggers ) do
	pTrigger:Remove()
end

pTrigger = gEntList.FindEntityByClassname( NULL, "trigger_changelevel_scripted" )
tTriggers = {}
while ( pTrigger ~= NULL ) do
  print( pTrigger )
  table.insert( tTriggers, pTrigger )
  pTrigger = gEntList.FindEntityByClassname( pTrigger, "trigger_changelevel_scripted" )
end

print( "Found " .. #tTriggers .. " trigger_changelevel_scripted!" )
