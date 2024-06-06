--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Dumps a list of all INetworkStringTables
--
--===========================================================================--

local t
local i = 0
repeat
  t = networkstringtable.GetTable( i )
  if ( t == nil ) then
    break
  end
  print( t:GetTableName() )
  i = i + 1
until t == nil
