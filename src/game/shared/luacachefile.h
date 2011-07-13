//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#ifndef LUACACHEFILE_H
#define LUACACHEFILE_H
#ifdef _WIN32
#pragma once
#endif

#include "zip_utils.h"

// Embedded pack/pak file
IZip				*luasrc_GetLcfFile( void );
void				luasrc_AddFileToLcf( IZip *lcf, const char *pRelativeName, const char *fullpath );

void  (luasrc_archivefolder) (lua_State *L, const char *path);

#endif // LUACACHEFILE_H
