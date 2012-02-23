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
#include "utldict.h"

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

	DevMsg( "Unpacking Lua cache file\n" );
}

static CUtlDict< char *, int > m_LcfDatabase;

static int luasrc_sendfile (lua_State *L) {
  lua_Debug ar1;
  lua_getstack(L, 1, &ar1);
  lua_getinfo(L, "f", &ar1);
  lua_Debug ar2;
  lua_getinfo(L, ">S", &ar2);
  int iLength = Q_strlen( ar2.source );
  char source[MAX_PATH];
  Q_StrRight( ar2.source, iLength-1, source, sizeof( source ) );
  Q_StripFilename( source );
  char filename[MAX_PATH];
  Q_snprintf( filename, sizeof( filename ), "%s\\%s", source, luaL_checkstring(L, 1) );
  m_LcfDatabase.Insert( luaL_checkstring(L, 1), strdup( filename ) );
  return 0;
}


static const luaL_Reg lcf_funcs[] = {
  {"sendfile", luasrc_sendfile},
  {NULL, NULL}
};


extern void lcf_open (lua_State *L) {
  /* open lib into global table */
  luaL_register(L, "_G", lcf_funcs);
  lua_pop(L, 1);
}

extern void lcf_close (lua_State *L) {
	m_LcfDatabase.PurgeAndDeleteElements();
}

#ifndef CLIENT_DLL

CON_COMMAND(dumpluacachefile, "Dump the contents of the Lua cache file database to the console.")
{
	int c = m_LcfDatabase.Count();
	for ( int i = 0; i < c; i++ )
	{
		Msg( "%s: %s\n", m_LcfDatabase.GetElementName( i ), m_LcfDatabase[ i ] );
	}
}

#endif
