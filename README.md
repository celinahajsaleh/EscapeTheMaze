# Escape the maze

This is a simple 3D maze game built in C++ using OpenGL (GLUT) for rendering, Box2D for 2D physics, and SDL2/SDL_mixer for audio.
Control a ball through procedurally generated mazes and reach the exit to advance to the next level.

## Gameplay
- Controls: use the Arrow keys or `h`/`j`/`k`/`l` (Vim style) to move. Hold `i` to show instructions. Press `ESC` to quit.
- Level progression: each time you complete a maze the next level increases both rows and columns by 2 (e.g. 11×11 → 13×13 → 15×15...).



## Build & Run (on macOS)
Prerequisites (install via Homebrew if needed):

```bash
brew install cmake pkg-config sdl2 sdl2_mixer
```

Build and run from the project root:

```bash
cd build
cmake ..
make
./main
```


## Project Layout
- `src/` — C++ source files (Game, Maze, Physics, Camera, Input, Audio, etc.)
- `assets/` — sound assets 
- `build/` — CMake build directory 

