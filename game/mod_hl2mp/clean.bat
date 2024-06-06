@echo off
rd "cache" "DownloadLists" "downloads" "lua_cache" "maps\graphs" "maps\soundcache" "materials\VGUI" "materialsrc" "SAVE" "screenshots" /s /q
del "bin\client.pdb" "bin\Server.pdb" "cfg\config.cfg" "demoheader.tmp" "modelsounds.cache" "serverconfig.vdf" "stats.txt" "textwindow_temp.html" "voice_ban.dt" /f /s /q
pause
