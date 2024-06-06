# Getting Started

## Requirements

- Source SDK 2013 Multiplayer Base (Installed through Steam)
- Visual Studio 2022 (or newer)
- [Visual Studio 2013 (for the Source SDK 2013 solution)](https://archive.org/details/en_visual_studio_community_2013_with_update_5_x86_dvd_6816332)

*[&raquo; See the Source SDK 2013 documentation for more information.](https://developer.valvesoftware.com/wiki/Source_SDK_2013)*

## Getting started

1. Go into the `src/` directory and generate the Visual Studio solution.

    ```bash
    cd src
    creategameprojects.bat
    ```

2. Open the created `games.sln` solution in Visual Studio 2022 (or newer).

    > [!WARNING]
    > Despite building in Visual Studio 2022, the Source SDK 2013 solution requires Visual Studio 2013 to be installed.

3. Build the solution in `Release` mode.

4. After the build is complete make a symlink from the `game/mod_hl2mp` directory to your `steamapps/sourcemods` directory. E.g:

    ```bash
    mklink /J "C:\Program Files (x86)\Steam\steamapps\sourcemods\hl2sb" "<path to this repo>\game\mod_hl2mp"
    ```

## Extras

<details>

<summary>Attaching the debugger</summary>

1. First generate the Visual Studio solution as described above.

2. Copy the `.env.example` file in the root of this repo to `.env`.

    ```bash
    cp .env.example .env
    ```

3. Fill the `.env` file with the described values

4. Then run the `setupprojects.sh` script in `src` to modify the Visual Studio project files.

    ```bash
    cd src
    ./setupprojects.sh
    ```

    This will setup the Client Visual Studio project for debugging.

5. Start the Visual Studio solution

6. Set the `Client` project as the startup project

7. Press `F5` to start the debugger

> Alternatively for a manual approach: [check the official docs and use values like this:](https://developer.valvesoftware.com/wiki/Installing_and_Debugging_the_Source_Code)
> - Command: `C:\Program Files %28x86%29\Steam\steamapps\common\Source SDK Base 2013 Multiplayer\hl2.exe`
> - Command Arguments: `-allowdebug -dev -sw -game "C:\Program Files (x86)\Steam\steamapps\sourcemods\mod_hl2mp"`
> - Working Directory: `C:\Program Files %28x86%29\Steam\steamapps\common\Source SDK Base 2013 Multiplayer`

</details>
