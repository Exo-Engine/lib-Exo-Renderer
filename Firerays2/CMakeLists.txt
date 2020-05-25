cmake_minimum_required(VERSION 3.8)
project(ExoRendererFirerays2 CXX)

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

option(RR_EMBED_KERNELS "Embed CL kernels into binary module" ON)
add_subdirectory(third_party/RadeonRays_SDK)

link_libraries(SDL2 SDL2_image OpenGL GL GLEW RadeonRays OpenCL)

add_library(ExoRendererFirerays2 SHARED ${SOURCES})
