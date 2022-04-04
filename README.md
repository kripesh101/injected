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

Default Level Directory: `assets/editor_default_level/`

## Custom Level Folder

Optionally, you can specify the level you wish to create/modify by passing the path to level folder.

Command: `main.exe -editor assets/levels/test_level_1/`.\
**The `/` at the end is compulsory.**

## Editor Controls

Moving around the view is done using the `WASD` keys. Hold `Shift` to speed up the movement. Use the scroll-wheel to adjust the zoom level.

### Editing Tiles

Press the `~` Key (Tilde Key) to switch to Input mode. In this mode, you can choose which Tile Map you want to modify.

Press `T` or `L` to switch to Level Tile Map.\
Press `W` to switch to Walls Tile Map.

Then, enter the tile number of the texture you want to use. Finally, press `Enter` to confirm your changes. You can always press `Esc` to cancel any ongoing input.

Now, you can `Left-click` on the map to paint the tiles. To paint other tiles, you can use the input mode again. Alternatively, pressing `Alt` and scrolling with the mouse will allow you to cycle through the tile numbers. To delete tiles, you have two options:

1. Set tile number to 0 (corresponds to an empty tile) or,
2. `Right-click` on existing tiles

You can also middle-click on any existing tile to choose that specific tile's number as the current tile number.

### Decorations

Press `D` while in the input mode to switch to Decorations editing.

Decorations are sprite objects on the level that can optionally have collision enabled on them. They can be placed anywhere on the map (even outside the tilemap boundaries).

In the editor, they are shown as a preview. `Left-click` to place the decoration, and `Right-click` to delete a decoration.

Press `C` to toggle collisions. You can hold `Q` at anytime to dim decorations that have collisions disabled.

Holding `Alt` and scrolling the mouse wheel will cycle through all the available decorations.

Similarly, Holding `Ctrl` and scrolling the mouse wheel will rotate the decoration.\
Hold `Shift` while doing this to have much finer control over the rotation.\
You can press `R` to reset the rotation if needed. 

**There are a few things to be mindful of when doing fine rotations:**

1. Bounding boxes in SFML are always *axis-aligned* (always a rectangle with sides parallel to the x and y axes). This means rotations like 45° will have very large bounding boxes and feel janky for the player to navigate around.

2. Only 90° rotations will rotate the bounding boxes properly. Other rotations will grow the bounding box in weird ways.

3. Keep rotations for collidable decorations at **multiples of 90°** (and around ± 5°) for the best player experience.

4. Rotations have no additional impact on the rendering of decorations. Feel free do any rotations with non-collidable decorations!

### Player Spawn Position

The player spawn position is represented by a special texture in the editor:

![Player Spawn Pos Indicator](bin/assets/textures/editor/player_preview.png)

Press `P` while in input mode to switch to Player Spawn editing. Simply press `Left-click` to move the player spawn.

By default, the player spawn is located outside the tilemap boundaries, at `-32, -32`.

The player bounding box is always `16 x 16`, which is also the size of the special texture (see above). Use the red boundaries of the texture to help perfectly align your player spawn, if desired.

**Important:**
- The game automatically generates a boundary for all levels **64** pixels outside the tilemap boundary. The player spawn must be within this boundary or the player will be unable to move.
- No collision checks are done by the editor for invalid positions. Make sure to playtest your map after changing the spawn position!

## Saving Levels

Closing the editor will automatically save the level.

`level.png` stores Level Tile Map data. Tile number data is stored in red channel of the pixels.

`walls.png` stores Walls Tile Map data. Tile number data is stored in red channel of the pixels.

`decor.txt` stores Decorations Data. Format:\
`<texture location> <x-coordinate> <y-coordinate> <rotation (in degrees)> <collision (0 or 1)>`
