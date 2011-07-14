//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose: Handles the creation of Lua Cache Files for multiplayer.
//
//===========================================================================//

#include "cbase.h"
#include "filesystem.h"
#include "lua.hpp"
#include "luacachefile.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static IZip *s_lcfFile = 0;

//-----------------------------------------------------------------------------
// Purpose: // Get a lcffile instance
// Output : IZip*
//-----------------------------------------------------------------------------
LUA_API IZip* luasrc_GetLcfFile( void )
{
	if ( !s_lcfFile )
	{
		s_lcfFile = IZip::CreateZip();
	}
	return s_lcfFile;
}

//-----------------------------------------------------------------------------
// Purpose: Add file from disk to .lcf
// Input  : *relativename - 
//			*fullpath - 
//-----------------------------------------------------------------------------
LUA_API void luasrc_AddFileToLcf( const char *relativename, const char *fullpath )
{
	s_lcfFile->AddFileToZip( relativename, fullpath );
}

LUA_API void luasrc_archivefolder (lua_State *L, const char *path)
{
	FileFindHandle_t fh;

	char searchPath[ 512 ];
	Q_snprintf( searchPath, sizeof( searchPath ), "%s\\*.lua", path );

#ifdef CLIENT_DLL
	const char *gamePath = engine->GetGameDirectory();
#else
	char gamePath[ 256 ];
	engine->GetGameDir( gamePath, 256 );
#endif

	char const *fn = g_pFullFileSystem->FindFirstEx( searchPath, "MOD", &fh );
	if ( fn )
	{
		do
		{
			if ( fn[0] != '.' )
			{
				char ext[ 10 ];
				Q_ExtractFileExtension( fn, ext, sizeof( ext ) );

				if ( !Q_stricmp( ext, "lua" ) )
				{
					char loadname[ 512 ];
					Q_snprintf( loadname, sizeof( loadname ), "%s\\%s\\%s", gamePath, path, fn );
					char archivename[ 512 ];
					Q_snprintf( archivename, sizeof( archivename ), "%s\\%s", path, fn );
					luasrc_AddFileToLcf(archivename,loadname);
				}
			}

			fn = g_pFullFileSystem->FindNext( fh );

		} while ( fn );

		g_pFullFileSystem->FindClose( fh );
	}
}
