--======== Copyleft © 2010-2013, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Tests the KeyValues implementation.
--
--===========================================================================--

local gamePath
if _CLIENT then
  gamePath = engine.GetGameDirectory();
else
  gamePath = engine.GetGameDir();
end

local pMainFile = KeyValues( "gameinfo.txt" );
if ( pMainFile:LoadFromFile( gamePath .. "/gameinfo.txt", "MOD" ) ) then
  print( "KeyValues: ")
  pMainFile:Print( 1 )

  --[[
  print( "KeyValues -> Table: ")
  local tMainFile = pMainFile:ToTable()
  table.print( tMainFile, false, 1 )

  print( "Table -> KeyValues: ")
  local pNewMainFile = table.tokeyvalues( tMainFile, "gameinfo.txt" )
  pNewMainFile:Print( 1 )
  pNewMainFile:deleteThis()
  ]]

  print( "KeyValues -> Table (Order preserved): ")
  local tMainFile = pMainFile:ToTable( true )
  table.print( tMainFile, true, 1 )

  print( "Table (Order preserved) -> KeyValues: ")
  local pNewMainFile = table.tokeyvalues( tMainFile, "gameinfo.txt", true )
  pNewMainFile:Print( 1 )
  pNewMainFile:deleteThis()
end
pMainFile:deleteThis();
