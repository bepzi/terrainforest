cmake_minimum_required(VERSION 3.0)
project(terrainforest)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

find_package(glm 0.9 REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glfw)

include_directories(libs/glad/include)
add_library(glad
  libs/glad/src/glad.c)

add_executable(terrainforest
  src/main.cpp
  src/application.cpp
  src/stage.hpp
  src/ocean.cpp
  src/vertex.hpp
  src/plane.hpp
  src/util.hpp)

target_compile_options(terrainforest PRIVATE -Wall -Wextra -Wmost -pedantic)

target_include_directories(terrainforest PRIVATE
  ${OPENGL_INCLUDE_DIR})

target_link_libraries(terrainforest
  glfw
  ${OPENGL_LIBRARIES}
  glad)
