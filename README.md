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
