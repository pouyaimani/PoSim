# PoSim – POS Terminal Emulator

A professional POS (Point of Sale) terminal emulator written in **C++17** using **LVGL** for UI rendering.  
Perfect for embedded system developers or anyone interested in building a payment terminal UI simulation.

---

## Tech Stack

- **C++17**
- **LVGL (Light and Versatile Graphics Library)**
- **CMake**
- Modular architecture with GUI, core logic, and transaction controller separation

---

## Project Structure

```
PoSim/
├── src/
│   ├── main.cpp
│   ├── gui/
│   ├── controller/
│   └── core/
├── include/
│   ├── gui/
│   ├── controller/
│   └── core/
├── assets/
├── test/
├── CMakeLists.txt
└── README.md
```

---

## Build Instructions

```bash
mkdir build
cd build
cmake ..
make
./posim
```

> Make sure LVGL is correctly included or linked. Simulator mode is supported.

---

## Commit Message Convention

This project follows the [Conventional Commits](https://www.conventionalcommits.org/) standard.

### Types

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation only changes
- `style`: Code formatting (no logic change)
- `refactor`: Code refactoring (no behavior change)
- `test`: Adding or updating tests
- `chore`: Build or CI config, tooling, etc.

### Examples

```bash
feat(gui): add welcome screen layout
fix(controller): handle null pointer in transaction flow
refactor(core): split logic into smaller modules
docs: update build instructions in README
```

---

## License

MIT
