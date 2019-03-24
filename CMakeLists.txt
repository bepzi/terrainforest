cmake_minimum_required(VERSION 3.0)
project(terrainforest)

# Sources are specified later in the file
add_executable(terrainforest "")

set(DEFAULT_CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# Compiler options
set_property(TARGET terrainforest PROPERTY CXX_STANDARD 17)
set_property(TARGET terrainforest PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET terrainforest PROPERTY CXX_EXTENSIONS OFF)

target_compile_options(terrainforest PRIVATE
  -Wall
  -Wextra
  -Wswitch
  -Wparentheses
  -Wconversion
  -Wshadow
  -Wfloat-equal
  -Wmissing-noreturn
  -pedantic)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
  target_compile_options(terrainforest PRIVATE
    -Wmost
    -Wheader-hygiene
    -Widiomatic-parentheses
    -Wmove
    -Wloop-analysis)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  target_compile_options(terrainforest PRIVATE
    -Wsuggest-attribute=pure
    -Wsuggest-attribute=const
    -Wsuggest-attribute=noreturn
    -Wsuggest-final-types
    -Wsuggest-final-methods
    -Wsuggest-override
    -Wmaybe-uninitialized)
endif()

# Libraries

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

target_include_directories(terrainforest PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(terrainforest glfw glad ${OPENGL_LIBRARIES})

# Target sources
target_sources(terrainforest PRIVATE
  src/main.cpp
  src/application.cpp
  src/ocean.cpp)
