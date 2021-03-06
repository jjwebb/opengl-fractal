cmake_minimum_required(VERSION 3.10)

project(opengl-fractal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/opengl-fractal)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(UNIX)
	find_package(PkgConfig REQUIRED)
	find_package(GLEW REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	include_directories(${GLFW_INCLUDE_DIRS})
else()
	add_subdirectory(Dependencies/GLFW/glfw)
	include_directories(Dependencies/GLFW/glfw/include)
	include_directories(Dependencies/glew-2.1.0/include)
	link_directories(Dependencies/glew-2.1.0/lib/Release/x64)
endif()

find_package(OpenGL REQUIRED)

find_package(glm REQUIRED PATHS ${PROJECT_SOURCE_DIR}/opengl-fractal/src/vendor/glm)

add_subdirectory(opengl-fractal/src)