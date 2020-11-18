
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)

file(GLOB_RECURSE SRCS ./*.cpp)
file(GLOB_RECURSE HDRS ./*.hpp)

#Add an icon to our Windows binary
if(UNIX)
	add_executable(fractal  ${SRCS} ${HDRS})
else()
	add_executable(fractal  ${SRCS} ${HDRS} icon.rc)
endif()

target_include_directories(fractal PRIVATE ${GLM_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)

target_compile_definitions(fractal PRIVATE GLEW_NO_GLU GLEW_STATIC)

if(UNIX)
	target_link_libraries(fractal OpenGL::GL GLEW::GLEW ${GLFW_STATIC_LIBRARIES} pthread glm)
else()
	target_link_libraries(fractal OpenGL::GL glew32s glfw ${GLFW_STATIC_LIBRARIES} glm)
endif()


