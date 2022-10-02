cmake_minimum_required(VERSION 3.0.0)
project(SpaceShooter VERSION 0.1.0)
find_package(OpenGL REQUIRED)

# Compile external dependencies
add_subdirectory(external)

# On Visual 2005 and above, this mocan set the debug working directory
# cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)

include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	src/rend/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_library(render_libs
	src/rend/render.cpp
	src/rend/render_engine.cpp
)

add_library(render_engine_libs

	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/objloader.cpp
	common/objloader.hpp
	common/vboindexer.cpp
	common/vboindexer.hpp
	common/quaternion_utils.cpp
	common/quaternion_utils.hpp

	res/StandardShading.vertexshader
	res/StandardShading.fragmentshader
)

add_executable(game_core
	src/game_core.cpp)

target_link_libraries(game_core
	${ALL_LIBS}
	ANTTWEAKBAR_116_OGLCORE_GLFW
	render_libs
	render_engine_libs
)

set_target_properties(game_core PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_custom_command(
	TARGET game_core POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/res
	${CMAKE_CURRENT_BINARY_DIR}/res)

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*")
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$")
