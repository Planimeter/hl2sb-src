//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose: Handles the creation of Lua Cache Files for multiplayer. This code
// is currently extremely experimental.
//
//===========================================================================//

#include "cbase.h"
#include "filesystem.h"
#include "luamanager.h"
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

#ifdef CLIENT_DLL

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
			DevMsg( "LCF: found \"%s\"!\n", pFilename );
			DevMsg( "LCF: unpacking Lua cache file...\n" );
			break;
		}
	}
}

#else

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


#endif

extern void lcf_open (lua_State *L) {
#ifndef CLIENT_DLL
  /* open lib into global table */
  luaL_register(L, "_G", lcf_funcs);
  lua_pop(L, 1);
#else
  // force create this directory incase it doesn't exist
  filesystem->CreateDirHierarchy( LUA_PATH_CACHE, "MOD");
#endif
}

extern void lcf_recursivedeletefile( const char *current ) {
	FileFindHandle_t fh;

	char path[ 512 ];
	if ( current[ 0 ] )
	{
        Q_snprintf( path, sizeof( path ), "%s/*.*", current );
	}
	else
	{
		Q_snprintf( path, sizeof( path ), "*.*" );
	}

	Q_FixSlashes( path );

	char const *fn = g_pFullFileSystem->FindFirstEx( path, "MOD", &fh );
	if ( fn )
	{
		do
		{
			if ( fn[0] != '.' )
			{
				if ( g_pFullFileSystem->FindIsDirectory( fh ) )
				{
					char nextdir[ 512 ];
					if ( current[ 0 ] )
					{
						Q_snprintf( nextdir, sizeof( nextdir ), "%s/%s", current, fn );
					}
					else
					{
						Q_snprintf( nextdir, sizeof( nextdir ), "%s", fn );
					}

					lcf_recursivedeletefile( nextdir );
				}
				else
				{
					char relative[ 512 ];
					if ( current[ 0 ] )
					{
						Q_snprintf( relative, sizeof( relative ), "%s/%s", current, fn );
					}
					else
					{
						Q_snprintf( relative, sizeof( relative ), "%s", fn );
					}
					DevMsg( "Deleting '%s/%s'...\n", current, fn );

					Q_FixSlashes( relative );
					g_pFullFileSystem->RemoveFile( relative, "MOD" );
				}
			}

			fn = g_pFullFileSystem->FindNext( fh );

		} while ( fn );

		g_pFullFileSystem->FindClose( fh );
	}
}

extern void lcf_close (lua_State *L) {
#ifndef CLIENT_DLL
	m_LcfDatabase.PurgeAndDeleteElements();
#else
	lcf_recursivedeletefile( LUA_PATH_CACHE );
#endif
}

#ifndef CLIENT_DLL

extern void lcf_preparecachefile (void) {
	DevMsg( "LCF: preparing Lua cache file...\n" );
	IZip *pZip = luasrc_GetLcfFile();
	int c = m_LcfDatabase.Count();
	for ( int i = 0; i < c; i++ )
	{
		pZip->AddFileToZip( m_LcfDatabase.GetElementName( i ), m_LcfDatabase[ i ] ); 
	}
	// force create this directory incase it doesn't exist
	filesystem->CreateDirHierarchy( "cache\\lua", "MOD");
	FileHandle_t fh = g_pFullFileSystem->Open( "cache\\lua\\cache.lcf", "wb", "MOD" );
	if ( FILESYSTEM_INVALID_HANDLE != fh )
	{
		pZip->SaveToDisk( fh );
	}
	g_pFullFileSystem->Close( fh );

	INetworkStringTable *downloadables = networkstringtable->FindTable( "downloadables" );
	downloadables->AddString( true, "cache\\lua\\cache.lcf", -1 );
	IZip::ReleaseZip( pZip );
}

CON_COMMAND(dumpluacachefile, "Dump the contents of the Lua cache file database to the console.")
{
	int c = m_LcfDatabase.Count();
	for ( int i = 0; i < c; i++ )
	{
		Msg( "%s: %s\n", m_LcfDatabase.GetElementName( i ), m_LcfDatabase[ i ] );
	}
}

#endif
