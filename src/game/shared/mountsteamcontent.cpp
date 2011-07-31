//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose:
//
//===========================================================================//

#include "cbase.h"
#include "mountsteamcontent.h"
// Andrew; grab only what we need from Open Steamworks.
#include "SteamTypes.h"
#include "ISteam006.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

bool Steam_MountSteamContent( int nExtraAppId )
{
	CreateInterfaceFn CreateInterface = Sys_GetFactory( "Steam.dll" );
	if( !CreateInterface )
		return false;

	int nStatus = 0;

	IAppSystem* pAppSystem = (IAppSystem *)CreateInterface( "SteamDLLAppsystem001", &nStatus );
	if( !pAppSystem )
		return false;

	ISteam006* pSteam006 = (ISteam006 *)pAppSystem->QueryInterface( STEAM_INTERFACE_VERSION_006 );
	if( !pSteam006 )
		return false;

	TSteamError Error;

	TSteamApp App;
	App.szName = new char[ 255 ];
	App.uMaxNameChars = sizeof( App.szName );
	App.szLatestVersionLabel = new char[ 255 ];
	App.uMaxLatestVersionLabelChars = sizeof( App.szLatestVersionLabel );
	App.szCurrentVersionLabel = new char[ 255 ];
	App.uMaxCurrentVersionLabelChars = sizeof( App.uMaxCurrentVersionLabelChars );
	App.szInstallDirName = new char[ 255 ];
	App.uMaxInstallDirNameChars = sizeof( App.szInstallDirName );
	App.szUnkString = new char[ 255 ];

	if( pSteam006->EnumerateApp( nExtraAppId, &App, &Error ) != 1 || Error.eSteamError != eSteamErrorNone )
	{
		delete[] App.szName;
		delete[] App.szLatestVersionLabel;
		delete[] App.szCurrentVersionLabel;
		delete[] App.szInstallDirName;
		delete[] App.szUnkString;

		return false;
	}

	for( unsigned int n = 0; n < App.uNumDependencies; n++ )
	{
		TSteamAppDependencyInfo Info;

		if( pSteam006->EnumerateAppDependency( nExtraAppId, n, &Info, &Error ) != 1 || Error.eSteamError != eSteamErrorNone )
			continue;

		if( !pSteam006->MountFilesystem( Info.AppId, Info.szMountName, &Error ) || Error.eSteamError != eSteamErrorNone )
			Warning( "%s\n", Error.szDesc );
	}

	delete[] App.szName;
	delete[] App.szLatestVersionLabel;
	delete[] App.szCurrentVersionLabel;
	delete[] App.szInstallDirName;
	delete[] App.szUnkString;

	return true;
}
