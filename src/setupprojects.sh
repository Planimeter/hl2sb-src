#!/bin/bash

if [ -f ../.env ]; then
    while IFS= read -r line; do
        if [[ $line == *"="* && $line != "#"* ]]; then
            eval "export $line"
        fi
    done < ../.env
else
    echo "The .env file does not exist!"
    exit 1
fi

if [ -z "$STEAM_MOD_DIR" ]; then
    echo "STEAM_MOD_DIR is not set in the .env file!"
    exit 1
fi

if [ -z "$STEAM_SDK_DIR" ]; then
    echo "STEAM_SDK_DIR is not set in the .env file!"
    exit 1
fi

PROJECT_FILE="game/client/client_hl2mp.vcxproj.user"

# Create the file, remove it if it already exists
if [ -f $PROJECT_FILE ]; then
    rm $PROJECT_FILE
fi

cat <<EOL > $PROJECT_FILE
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="Current" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
    <PropertyGroup Condition="'\$(Configuration)|\$(Platform)'=='Debug|Win32'">
        <LocalDebuggerCommand>$STEAM_SDK_DIR\\hl2.exe</LocalDebuggerCommand>
        <LocalDebuggerCommandArguments>-allowdebug -dev -sw -game "$STEAM_MOD_DIR"</LocalDebuggerCommandArguments>
        <LocalDebuggerWorkingDirectory>$STEAM_SDK_DIR</LocalDebuggerWorkingDirectory>
        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
    </PropertyGroup>
</Project>
EOL
