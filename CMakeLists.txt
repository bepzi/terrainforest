# See: https://cliutils.gitlab.io/modern-cmake/
cmake_minimum_required(VERSION 3.1...3.15)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(TerrainForest
  VERSION 0.1.0
  DESCRIPTION "Experiments with OpenGL and terrain generation"
  LANGUAGES C CXX)

add_executable(forest
  src/main.cpp
  src/application.cpp
  src/ocean.cpp)

# OpenGL
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

# GLM
find_package(glm 0.9 REQUIRED)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/glfw)

# GLAD
add_library(glad libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)
target_include_directories(forest PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(forest glfw glad ${OPENGL_LIBRARIES})

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
    # -fno-exceptions
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

set(CMAKE_EXPORT_COMPILE_COMMANDS true)
