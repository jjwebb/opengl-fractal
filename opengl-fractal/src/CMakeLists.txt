
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)

file(GLOB_RECURSE SRCS ./*.cpp)
file(GLOB_RECURSE HDRS ./*.hpp)

#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)
#add_executable(fractal main.cpp IndexBuffer.cpp Renderer.cpp Shader.cpp Texture.cpp VertexArray.cpp VertexBuffer.cpp)

add_executable(fractal  ${SRCS} ${HDRS})
#${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
target_include_directories(fractal PRIVATE ${GLM_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)
#find_library(glm ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)
#add_library(glm STATIC IMPORTED ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)
target_compile_definitions(fractal PRIVATE GLEW_NO_GLU GLEW_STATIC)
#target_compile_features(fractal PRIVATE cxx_std_17)
if(UNIX)
	target_link_libraries(fractal OpenGL::GL GLEW::GLEW ${GLFW_STATIC_LIBRARIES} pthread glm)
else()
	target_link_libraries(fractal OpenGL::GL glew32s glfw ${GLFW_STATIC_LIBRARIES} glm)
endif()
#target_link_libraries(fractal ${GLFW_STATIC_LIBRARIES})

