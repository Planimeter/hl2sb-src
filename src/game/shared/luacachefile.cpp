//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose: Handles the creation of Lua Cache Files for multiplayer.
//
//===========================================================================//

#include "cbase.h"
#include "filesystem.h"
#include "lua.hpp"
#include "luacachefile.h"
#ifdef CLIENT_DLL
#include "networkstringtable_clientdll.h"
#else
#include "networkstringtable_gamedll.h"
#endif

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

LUA_API void luasrc_parsefromdownloadables ()
{
	// Andrew; Do we even need this anymore? We're not going to be listing .lua
	// files in downloadables anyway.
	// INetworkStringTable *downloadables = networkstringtable->FindTable( "downloadables" );
}

LUA_API void luasrc_ExtractLcf ()
{
	INetworkStringTable *downloadables = networkstringtable->FindTable( "downloadables" );
	const char *pFilename = NULL;
	for ( int i=0; i<downloadables->GetNumStrings(); i++ )
	{
		pFilename = downloadables->GetString( i );
		char ext[ 10 ];
		Q_ExtractFileExtension( pFilename, ext, sizeof( ext ) );

		if ( !Q_stricmp( ext, "lcf" ) )
		{
			// Andrew; extract the .lcf here.
			break;
		}
	}
}
