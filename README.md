# Cosmic Horizons

> Navigate the cosmos, dodge asteroids, and shoot your way through space.

**Cosmic Horizons** is a 3D first-person space shooter built with C++20 and [Raylib](https://www.raylib.com/). Pilot your spaceship through an asteroid field, destroy targets to rack up points, and survive as long as you can.

## Screenshots

| Main Menu | Gameplay |
|:---------:|:--------:|
| ![Main Menu](docs/img/main_menu.png) | ![Gameplay 1](docs/img/gameplay_1.png) |
| ![Gameover Menu](docs/img/gameover_menu.png) | ![Gameplay 2](docs/img/gameplay_2.png) |

## Features

- **6DOF spaceship movement** — pitch, yaw, and roll through open space
- **Asteroid shooting** — aim with the crosshair and fire with raycasting-based hit detection
- **Score & accuracy tracking** — monitor your performance in real time
- **Lives system** — 3 lives per run; collide with an asteroid and you lose one
- **Procedural asteroids** — randomly generated shapes, sizes, and trajectories
- **Starfield background** — parallax star rendering for immersion
- **Sound effects** — shooting and destruction audio feedback

## Controls

| Action | Input |
|--------|-------|
| Aim / Look | Mouse |
| Move Up | `W` |
| Move Down | `S` |
| Roll Left | `A` |
| Roll Right | `D` |
| Shoot | Left Click |
| Quit | `ESC` |

## Building

### Prerequisites

- **CMake** 3.28 or higher
- A **C++20** compatible compiler (GCC 13+, Clang 16+, MSVC 2022+)
- **Git** (for submodules)

### Steps

```bash
# Clone with submodules
git clone --recursive https://github.com/pablobh2147/cosmic-horizons.git
cd cosmic-horizons

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run
./build/CosmicHorizons
```

> **Note:** Tested on Linux. Since the only external dependency is Raylib (included as a submodule), it should work on Windows and macOS as well.

## Dependencies

| Library | Purpose | Included as |
|---------|---------|-------------|
| [Raylib](https://github.com/raysan5/raylib) | Rendering, input, audio | Git submodule |
| [GLM](https://github.com/g-truc/glm) | Math (vectors, matrices, quaternions) | Git submodule |

## License

This project is licensed under the **GNU General Public License v3.0** — see the [LICENSE](LICENSE) file for details.

## Credits

See [CREDITS.md](CREDITS.md) for full attribution of assets and contributors.
