--======== Copyleft © 2010-2011, Team Sandbox, Some rights reserved. ========--
--
-- Purpose: Tests the FileHandle_t implementation.
--
--===========================================================================--

require( "dbg" )
require( "filesystem" )

local Warning = dbg.Warning

local hFile = filesystem.Open( "gamecontent.txt", "r" )
local size = filesystem.Size( hFile )
print( "hFile = " .. tostring( hFile ) )
local bytesRead, pOutput = filesystem.Read( size, hFile )
print( "bytes: " .. bytesRead )
print( "output:\n" .. pOutput )
print( "Closing hFile, handle should become invalid!" )
filesystem.Close( hFile )
print( "hFile = " .. tostring( hFile ) )

print( "Testing error handling..." )
local status, strError = pcall( function()
	print( "filesystem.IsOk( hFile ) = " .. tostring( filesystem.IsOk( hFile ) ) )
end )

if ( status == false ) then
	Warning( strError .. "\n" )
else
	error( "WARNING: test did not fail as expected on line 32!", 2 )
end

print( "Testing error handling with non-filehandle type...")
local status, strError = pcall( function()
	print( "filesystem.IsOk( hFile ) = " .. tostring( filesystem.IsOk( -1 ) ) )
end )

if ( status == false ) then
	Warning( strError .. "\n" )
else
	error( "WARNING: test did not fail as expected on line 47!", 2 )
end
