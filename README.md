# INJECTED!

A Hotline Miami-inspired top-down shooter.

# Setup Instructions

## Setting up GCC

Version of GCC used: 7.3.0 MinGW

[Link for Windows](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/7.3.0/threads-posix/dwarf/i686-7.3.0-release-posix-dwarf-rt_v5-rev0.7z/download)

Extract to wherever you like, then add the `bin` folder to PATH. [See the "Prerequisites" part of this article for instructions.](https://code.visualstudio.com/docs/cpp/config-mingw#_prerequisites). **Skip STEP 3.**

This version of GCC properly works with SFML, so it's strongly recommended to do this even if you already have GCC installed.

## Setting up SFML

Version of SFML Used: 2.5.1

[Direct Link to SFML](https://www.sfml-dev.org/files/SFML-2.5.1-windows-gcc-7.3.0-mingw-32-bit.zip). This version is specifically built for the version of GCC provided above.

Extract the ZIP, and copy the contents of lib and bin to the appropriate directories in the project folder.

## Building

To build, simply run `mingw32-make` in the root directory of this project. To run the application, navigate to the `bin` folder, and run `main.exe`.

You can also open this project in Visual Studio Code, and Press `Ctrl+Shift+B` to build and run.

# Editor Info

To launch the INJECTED! Level Editor, open the compiled `main.exe` by passing `-editor` argument.\
Command: `main.exe -editor`

Default Level Directory: `assets/levels/default/`

## Custom Level Folder

Optionally, you can specify the level you wish to create/modify by passing the path to level folder.

Command: `main.exe -editor assets/levels/test_level_1/`.\
Note: The level folder must exist or the level will not be saved. Also, **the `/` at the end is compulsory.**\

## Editor Controls

Moving around the view is done using the `WASD` keys. Hold `Shift` to speed up the movement. Use the scroll whell to adjust the zoom level.

### Editing Tiles

Press the `~` Key (Tilde Key) to switch to Input mode. In this mode, you can choose which Tile Map you want to modify.

Press `T` or `L` to switch to Level Tile Map.\
Press `W` to switch to Walls Tile Map.

Then, enter the tile number of the texture you want to use. Finally, press `Enter` to confirm your changes. You can always press `Esc` to cancel any ongoing input.

Now, you can click on the map to paint the tiles. To paint other tiles, you can use the input mode again. Alternatively, pressing `Alt` and scrolling with the mouse will allow you to cycle through the tile numbers.

You can also press middle-click on any existing tile to choose that specfic tile's number as the current tile number.

## Saving Levels

Closing the editor will automatically save the level.
