# terrainforest

Experiments with OpenGL and terrain generation.

## Building

Built with CMake.

Libraries needed:

 - GLM for vector/matrix operations
 - GLFW for the window
 - GLAD for using newer OpenGL features

GLAD and GLFW are already in this repo under `libs`.

``` bash
git clone --recurse-submodules <this repo's url>
cd terrainforest
mkdir build && cd build
cmake ..
# or, if you want to use clang:
# cmake -D CMAKE_CXX_COMPILER=/path/to/clang++ ..
make
./terrainforest
```

The `CMakeLists.txt` automatically generates a compilation database
file (`compile_commands.json`) in the build directory. If your tools
use it, you may want to symlink it from the build directory to the
root repo directory to get proper code completion.

## Keybindings

**WASD** (or equivalent) to move around. **Space** and **Left Shift**
move you up or down (think Minecraft Creative-mode controls).

**Escape** to quit the application.

**Ctrl+W** toggles wireframe rendering.
