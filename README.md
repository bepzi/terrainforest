<h1 align="center">TerrainForest</h1>

**WIP:** Experiments with OpenGL and terrain generation.

## Building

Built with [Modern CMake](https://cliutils.gitlab.io/modern-cmake/).

``` bash
git clone --recurse-submodules <this repo>
cd <this repo>
cmake -S . -B build
cmake --build build
./build/forest
```

The `CMakeLists.txt` automatically generates a compilation database
file (`compile_commands.json`) in the build directory. If your tools
use it, you may want to symlink it from the build directory to the
root repo directory to get proper code completion.

<!-- ## Keybindings -->

<!-- **WASD** (or equivalent) to move around. **Space** and **Left Shift** -->
<!-- move you up or down (think Minecraft Creative-mode controls). -->

<!-- **Escape** to quit the application. -->

<!-- **Ctrl+W** toggles wireframe rendering. -->
