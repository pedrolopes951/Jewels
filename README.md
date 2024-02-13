# Jewel Match Game

## Overview
This project is a match-three puzzle game where players swap jewels to match three or more of the same kind in a row or column. The objective is to score as many points as possible within a set time limit of 1 min.

## Features
- Grid-based match-three puzzle mechanics.
- Time-limited gameplay.
- Scoring system based on the number of jewels matched.
- Restart and exit options post-gameplay.

## Future Features
- Sound based clicking and matching
- Tracking of max points per player
- Improve "Gravity" motion of jewels that get matched
...

## How to Play
1. Run the game executable.
2. Press ENTER at the setup screen to start the game.
3. Swap adjacent jewels to form a line of three or more matching jewels.
4. Score as many points as possible before the timer runs out.
5. At the game over screen, press 'R' to restart or 'X' to exit the game.

## Requirements
- SDL2 library for rendering, window management, and event handling.
- SDL2_ttf for text rendering. 
- SDL2_image for image loading.

## Installation
This project was built using Visual Studio IDE 2022 for the x86 Windows platform. To compile, open the provided solution file in Visual Studio and build the solution. Ensure that SDL2, SDL2_ttf, and SDL2_image libraries are properly linked.

## Compilation and Running
The project uses C++14 standards. No additional external libraries were used besides SDL2, SDL2_ttf, and SDL2_image. After opening the solution in Visual Studio, simply build the solution to compile the project.
Be sure to build for x86 platform, and that the SDL2 libs like (SDL2, SDL2_ttf, and SDL2_image) are install in root dir i,e: c:\SDL2 ...

## Configuration
No additional configuration is required for basic gameplay. Settings such as the time limit or difficulty adjustments are hardcoded and can be modified in the source code if necessary.

## Credits (Contact)
- Pedro Lopes (pedro.miguel.lopes951@gmail.com)
- SDL2: [https://www.libsdl.org/index.php](https://www.libsdl.org/index.php)
