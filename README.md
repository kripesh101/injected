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

**DO NOT USE SPACES IN THE NAME OF YOUR LEVEL FOLDER**

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

### Consumable Hearts

Press `H` while in input mode to switch to Consumable Hearts editing. `Left-click` to place the heart, and `Right-click` to delete a heart.

A heart will heal the player 1 health when the player touches it.

Make sure to place the heart in places that the player can navigate to! The heart's bounding box is `15 x 13`.

### Player Spawn Position

The player spawn position is represented by a special texture in the editor:

![Player Spawn Pos Indicator](bin/assets/textures/editor/player_preview.png)

Press `P` while in input mode to switch to Player Spawn editing. Simply press `Left-click` to move the player spawn.

By default, the player spawn is located outside the tilemap boundaries, at `-32, -32`.

The player bounding box is always `16 x 16`, which is also the size of the special texture (see above). Use the red boundaries of the texture to help perfectly align your player spawn, if desired.

**Important:**
- The game automatically generates a boundary for all levels **64** pixels outside the tilemap boundary. The player spawn must be within this boundary or the player will be unable to move.
- No collision checks are done by the editor for invalid positions. Make sure to playtest your map after changing the player spawn position!

### Enemy Spawn Positions

The enemy spawn positions are represented by a special texture in the editor:

![Enemy Spawn Pos Indicator](bin/assets/textures/editor/enemy_preview.png)

The blue arrow specifies the direction faced by the enemy.

Press `E` while in input mode to switch to Enemy Spawns editing. A preview (similar to Decorations) is shown when in this mode. Press `Left-click` to place an enemy spawn, and press `Right-click` to remove an enemy spawn.

The enemy bounding box is always `16 x 16`, which is also the size of the special texture (see above). Use the red boundaries of the texture to help perfectly align your enemy spawn, if desired.

Hold `Ctrl` and scroll the mouse-wheel to rotate the enemy 90°.

**Important:**
- No collision checks are done by the editor for invalid positions. Make sure to playtest your map after changing the enemy spawn positions!

## Saving Levels

Closing the editor will automatically save the level.

`level.png` stores Level Tile Map data. Tile number data is stored in red channel of the pixels.

`walls.png` stores Walls Tile Map data. Tile number data is stored in red channel of the pixels.

`decor.txt` stores Decorations Data. Format:\
`<texture location> <x-coordinate> <y-coordinate> <rotation (in degrees)> <collision (0 or 1)>`

`player_spawn.txt` stores Player Spawn Data. Format:\
`<x-coordinate> <y-coordinate>`

`enemy_spawns.txt` stores Enemy Spawns Data. Format:\
`<x-coordinate> <y-coordinate> <rotation (in degrees)>`

`hearts.txt` stores Consumable Hearts Data. Format:\
`<x-coordinate> <y-coordinate>`

All rotations are clockwise.

# Missions Format

Missions are the campaigns you can play in "INJECTED!". By default, the vanilla "INJECTED!" mission is included with every copy of the game. 

The missions directory is `assets/missions/`. Placing a mission folder there will make it show up in the main menu, provided the required files exist.

Creating a custom mission or modifying an existing mission is very simple, as detailed below:

## Information Files

These files are used in the mission select screen in the main menu, when there is more than one mission present in the missions directory.

`title.txt` simply has the title for your mission. Keep it short!

`thumbnail.png` should be a image file of ratio **2:1**. It is simply a thumbnail or icon for your mission.

`description.txt` should include a short description of your mission. Make sure to check how the text looks in the selection menu, as there is no code present for word wrapping.

## Mission Detail File

The `mission_details.txt` file sets the outline for your mission. Each line in the file is a sequence of your mission. There are 4 types of supported sequences:

### `TRANSITION`
The specified image file will be loaded and shown to the user, until the user presses `Left-click`. The next sequence will be played after a 2 second fade. The image file should be of ratio **16:9**. The idea is to use these transitions for storytelling.

Format: `TRANSITION <image file path>`

### `MUSIC`
The specified music track will start playing (in a loop), until another `MUSIC` sequence is reached. Having multiple `MUSIC` sequences is handy if you want to use different music tracks for dialogue and gameplay, for example. Similarly, use this if you want different music tracks for each level.

This is the only way to get music working in INJECTED!, so you must have at least one `MUSIC` sequence (preferably at the start of the mission details file) if you want a track to be played during your mission.

Format: `MUSIC <music file path>`\
Supported Files: `ogg` and `wav` formats

### `AUDIO`
Very similar to `MUSIC` in the sense that a specified audio track will start playing. However, this one will not loop. It will automatically fade out during the next fade sequence. Use this for voiceover during transition or credits sequences. You can also use it to play a short audio clip at the beginning of a level.

Format: `AUDIO <audio file path>`\
Supported Files: `ogg` and `wav` formats

### `LEVEL`
A level will be loaded from the specified folder. The level will be playable until all enemies are killed, or until the player dies. If all enemies are killed, the next sequence will be played after a 2 second fade. If the player dies instead, the restart menu will show up, allowing the user to choose from 3 options:

1. Restart level
2. Return to main menu
3. Quit the game

Format: `LEVEL <level folder path>`

### `CREDITS`
Although named *credits*, this sequence type allows for any text file to be displayed until the user presses `Left-click`. This is similar to the `TRANSITION` sequence, except it simply needs a text file. Make sure to check how the text looks in-game, as there is no code present for word wrapping.

Format: `CREDITS <text file path>`

**Important:** All paths in the mission detail file are relative to the mission folder itself.

For a complete example of the `mission_detail.txt` file, please refer to the included "INJECTED!" mission.

# Testing Levels and Missions
Launching `main.exe` with the `-debug` parameter passed may be handy, as it enables a few test shortcuts:

1. Press `O` to simulate victory (all enemies will be killed)
Use this to check your mission structure without having to finish levels. 

2. Press `.` to show hitboxes for enemies and the player
Use this to check for collision issues.

3. Press `L` to simulate defeat (restart screen will be shown)
Not very useful except if you simply want to keep replaying the level for testing etc.

`-debug` parameter will also enable frametime spike logs in the console. Please ignore the warnings.
