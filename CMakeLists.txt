# See: https://cliutils.gitlab.io/modern-cmake/
cmake_minimum_required(VERSION 3.1...3.15)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(TerrainForest
  VERSION 0.1.0
  DESCRIPTION "Experiments with OpenGL and terrain generation"
  LANGUAGES C CXX)

# Dependencies and Libraries
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm 0.9 REQUIRED)

add_library(glad STATIC
  extern/glad/src/glad.c)
target_include_directories(glad PUBLIC
  extern/glad/include)
get_property(GLAD_INCLUDE_DIRS
  TARGET glad PROPERTY INCLUDE_DIRECTORIES)

# Executable
file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(forest "${SRC_FILES}")

target_include_directories(forest PRIVATE
  "${SDL2_INCLUDE_DIRS}"
  "${OPENGL_INCLUDE_DIR}"  # not a typo, should be DIR
  "${GLAD_INCLUDE_DIRS}"
  "${GLM_INCLUDE_DIRS}")
target_link_libraries(forest PRIVATE
  "${SDL2_LIBRARIES}"
  "${OPENGL_LIBRARIES}"
  "${CMAKE_DL_LIBS}"
  glm
  glad)

set_target_properties(forest PROPERTIES
  CXX_STANDARD 17                  # -std=c++17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  INTERPROCEDURAL_OPTIMIZATION ON) # -flto

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
    OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  target_compile_options(forest PRIVATE
    -Wall
    -Wextra
    -Wswitch
    -Wconversion
    -Wshadow
    -Wparentheses
    -Wfloat-equal
    -Wzero-as-null-pointer-constant
    -Wpedantic
    -Werror
    -fno-exceptions
    -pedantic
    -pedantic-errors)
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  target_compile_options(forest PRIVATE
    -Wmost
    -Wimplicit
    -Wmove
    -Wloop-analysis
    -Widiomatic-parentheses
    -Wpragmas
    -Wheader-hygiene)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  target_compile_options(forest PRIVATE
    -Wsuggest-attribute=pure
    -Wsuggest-attribute=const
    -Wsuggest-attribute=noreturn
    -Wsuggest-final-types
    -Wsuggest-final-methods
    -Wsuggest-override
    -Wmaybe-uninitialized)
endif()
