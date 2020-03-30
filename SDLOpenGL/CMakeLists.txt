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

link_libraries(SDL2 SDL2_image OpenGL GLEW)

add_library(ExoRendererSDLOpenGL SHARED ${SOURCES})
