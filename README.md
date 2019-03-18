# terrainforest

Experiments with OpenGL and terrain generation.

## Building

Built with CMake.

Libraries needed:

 - GLM for vector/matrix operations
 - GLFW for the window
 - GLAD for using newer OpenGL features

GLAD and GLFW are already in the repo under `libs`.

``` bash
git clone --recurse-submodules <this repo's url>
cd terrainforest
mkdir build && cd build
cmake ..
make
./terrainforest
```

The `CMakeLists.txt` automatically generates a compilation database
file (`compile_commands.json`) in the build directory. If you tools
use it, you may want to symlink it from the root repo directory to get
proper code completion.
