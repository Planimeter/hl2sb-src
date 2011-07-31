//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#ifndef MOUNTSTEAMCONTENT_H
#define MOUNTSTEAMCONTENT_H
#ifdef _WIN32
#pragma once
#endif

//-----------------------------------------------------------------------------
// Mounts content directly through Steam instead of the engine filesystem
//-----------------------------------------------------------------------------
bool Steam_MountSteamContent( int nExtraAppId = -1 );

void MountUserContent();

#endif // MOUNTSTEAMCONTENT_H
