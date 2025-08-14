# Lite2D Engine (C++/SDL2)

Minimal 2D engine with modular subsystems: rendering, input (action-based), basic physics (AABB vs static), camera follow, and sprite animations. Includes a small demo scene.

## Requirements
- CMake 3.16+
- C++17 compiler
- SDL2, SDL2_image (dev packages)

On Debian/Ubuntu:
```bash
sudo apt-get update -y && sudo apt-get install -y cmake g++ libsdl2-dev libsdl2-image-dev
```

## Build
```bash
cmake -S . -B build
cmake --build build -j
```

## Run
```bash
./build/Lite2DEngine
```
If running in a headless container/CI, you can use SDL's dummy video driver:
```bash
SDL_VIDEODRIVER=dummy ./build/Lite2DEngine
```

## Controls
- A/D: Move left/right
- Space: Jump
- Esc: Quit

## Project Structure
- `include/Engine/` headers for subsystems
- `src/` implementations and `main.cpp`
- `assets/` placeholder for future textures, backgrounds, and levels

## Notes
- Fixed time-step at 60 FPS, accumulator-based loop
- Simple AABB vs static bodies with positional resolution
- Camera follows the first entity (player)
- Example animation uses a generated 4-frame sprite sheet
