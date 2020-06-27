cmake_minimum_required(VERSION 3.8)
project(ExoRendererSDLOpenGL CXX)

set(RENDERER_DIR ../renderer)

file(GLOB SOURCES
	${RENDERER_DIR}/include/*.h
	${RENDERER_DIR}/include/**/**.h
	${RENDERER_DIR}/src/*.cpp
	${RENDERER_DIR}/src/**/**.cpp
	include/*.h
	include/**/**.h
	src/*.cpp
	src/**/**.cpp
)

include_directories(
	include
	${RENDERER_DIR}/include)

find_package(SDL2 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)

link_libraries(SDL2::SDL2 SDL2::SDL2main SDL2::SDL2_image glm ${OPENGL_gl_LIBRARY} GLEW::GLEW)
add_library(ExoRendererSDLOpenGL SHARED ${SOURCES})
