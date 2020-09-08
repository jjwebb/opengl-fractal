cmake_minimum_required(VERSION 3.10)

project(opengl-fractal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/opengl-fractal)
set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)

find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)

find_package(glm REQUIRED PATHS ${PROJECT_SOURCE_DIR}/opengl-fractal/src/vendor/glm)

#include_directories(${GLEW_INCLUDE_DIRS})

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})


add_subdirectory(opengl-fractal/src)


