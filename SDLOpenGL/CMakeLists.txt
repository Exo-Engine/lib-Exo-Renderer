cmake_minimum_required(VERSION 3.8)
project(ExoRendererSDLOpenGL CXX)

file(GLOB SOURCES
	include/*.h
	include/**/**.h
	src/*.cpp
	src/**/**.cpp
)

set(RENDERER_DIR ../renderer)

include_directories(
	include
	${RENDERER_DIR}/include)

add_library(ExoRendererSDLOpenGL SHARED ${SOURCES})
