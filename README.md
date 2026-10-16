# PhysicsSim: Spring and Ball

A real-time spring–mass physics simulation written in **C** with **[raylib](https://www.raylib.com/)**.

Several weights hang from springs side by side, each with a different mass. They're released at the springs' natural length and bounce under gravity, spring force, and damping. Each system shows its live **velocity** (m/s) and **spring tension** (N) above its anchor.

---

## Quick Start

You need a C compiler, raylib, and `pkg-config`.

### macOS (Homebrew)

```bash
brew install raylib pkg-config
git clone https://github.com/Bttaleb/PhysicsSim---Spring-and-Ball.git
cd PhysicsSim---Spring-and-Ball
cc PhysicsSim.c -o sim $(pkg-config --libs --cflags raylib) && ./sim
```

### Linux

Install raylib from your package manager if it has it (for example `sudo pacman -S raylib` on Arch, or `sudo apt install libraylib-dev` on recent Debian/Ubuntu). Otherwise [build raylib from source](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).

```bash
git clone https://github.com/Bttaleb/PhysicsSim---Spring-and-Ball.git
cd PhysicsSim---Spring-and-Ball
cc PhysicsSim.c -o sim $(pkg-config --libs --cflags raylib) -lm && ./sim
```

### Windows (MSYS2, UCRT64 shell)

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-raylib mingw-w64-ucrt-x86_64-pkgconf git
git clone https://github.com/Bttaleb/PhysicsSim---Spring-and-Ball.git
cd PhysicsSim---Spring-and-Ball
gcc PhysicsSim.c -o sim.exe $(pkg-config --libs --cflags raylib) && ./sim.exe
```

Close the window or press **Esc** to quit.

> **What does the compile command do?** `pkg-config --libs --cflags raylib` prints the flags that tell the compiler where raylib's headers and library live. `&&` runs the program only if the compile succeeded.

### Editor setup (optional)

If your editor underlines raylib functions in red even though the project compiles, its language server (clangd) can't find `raylib.h`. The included `compile_flags.txt` points it at `/opt/homebrew/include` (Homebrew on Apple Silicon). Change that path to wherever raylib's headers are on your machine. `pkg-config --cflags raylib` will tell you.

---

## Tweaking the Simulation

Everything is controlled by constants at the top of `PhysicsSim.c`. Change one, recompile, and run:

| Constant / value | What it changes |
| --- | --- |
| `systemCount` | How many spring systems are on screen (up to `MAX_SYSTEMS`) |
| `MASS_PER_SYSTEM` | Mass added per system. System *i* weighs `(i + 1) × MASS_PER_SYSTEM` kg |
| `RADIUS_PER_SYSTEM` | Drawn size per system, so heavier balls look bigger |
| `.stiffness` (in `main`) | Spring stiffness *k*: higher means faster, shallower bounces |
| `.damping` (in `main`) | Energy loss: `0` bounces forever, large values settle without bouncing |
| `PIXELS_PER_METER` | Screen scale (default: 1 m = 100 px) |

**Things to try:**
- Set damping to `0` and watch the bounce never fade.
- Make one system much heavier. It hangs lower **and** bounces slower.
- Increase the count to 10 and check whether the heaviest weights stay on screen.

---

## How It Works

Every frame (a fixed **1/60 s** step), each system goes through the same chain:

```
stretch      = (ball.y − anchor.y) − restLength
springForce  = −stiffness × stretch            Hooke's law
dampingForce = −damping × velocity             opposes motion
acceleration = gravity + (springForce + dampingForce) / mass
velocity    += acceleration × dt
position    += velocity × dt
```

- **Semi-implicit Euler integration:** velocity is updated *before* position, which keeps springs stable.
- **Fixed time step:** the result doesn't depend on frame rate, so the simulation behaves the same on every machine.
- **Real-world units:** the physics uses real gravity (9.8 m/s²), converted to pixels with `PIXELS_PER_METER`. Displayed values are converted back to m/s and newtons.
- **Screen coordinates:** in raylib, **y increases downward**, so gravity is positive and a stretched spring's upward pull is negative.

### Checking the physics

For an undamped spring, the period of one bounce is

```
T = 2π √(mass / stiffness)
```

so quadrupling the mass only **doubles** the period. Set damping to 0 and time the bounces to compare.

---

## Code Structure

| Piece | Role |
| --- | --- |
| `Ball` | A moving weight: position and velocity (state), mass and radius (parameters) |
| `Spring` | Anchor point, rest length, stiffness, damping |
| `SpringSystem` | One ball paired with its spring |
| `spring_force()` | Hooke's law, written once and shared by update and draw |
| `update_system()` | Advances one system by one time step |
| `draw_system()` | Draws one system and its live readouts |
| `main()` | Sets up systems evenly across the screen, then runs the update → draw loop |

---

## Roadmap

- [ ] **String mode:** a string that pulls but never pushes (goes slack), and **snaps** when tension exceeds its breaking strength
- [ ] **UI controls** to pick spring/string and change mass, stiffness, and damping at runtime
- [ ] Measure the bounce period in the simulation and compare it with the formula

---

## Built With

- [C](https://en.wikipedia.org/wiki/C_(programming_language))
- [raylib](https://www.raylib.com/): a simple library for games and graphics
