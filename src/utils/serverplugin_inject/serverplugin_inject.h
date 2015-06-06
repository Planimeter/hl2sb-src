//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

extern volatile bool g_bInitializedAsPlugin;
extern volatile bool g_bExit;
extern char g_PluginFolder[ MAX_PATH ];

extern HANDLE hThread;
extern HMODULE hmPlugin;
extern HWND hMainWindow;

void DllInit();
void DllShutdown();
