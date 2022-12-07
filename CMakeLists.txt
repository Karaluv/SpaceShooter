cmake_minimum_required(VERSION 3.0.0)
project(SpaceShooter VERSION 0.1.0)
find_package(OpenGL REQUIRED)

# cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP
include(CTest)

add_subdirectory(external)

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
	external/stb/
	src/rend/
	src/serg/

	src/andrei/

	src/addlibs/

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

add_library(mynum
src/addlibs/MyNum.cpp
src/addlibs/MyNum.hpp
 "src/serg/Player_Actions.h")

add_library(mydict
src/addlibs/MyDict.cpp
src/addlibs/MyDict.hpp
 "src/serg/Player_Actions.h")

add_library(mycontrol
src/addlibs/MyControl.cpp
src/addlibs/MyControl.hpp
 "src/serg/Player_Actions.h")

add_library(mymatrix
src/addlibs/matrix.cpp
src/addlibs/matrix.hpp
 "src/serg/Player_Actions.h")

set(ADD_LIBS
	mynum
	mydict
	mycontrol
	mymatrix
)
# render engine

add_library(OBJLoader
	src/rend/objloader.cpp
 "src/serg/Player_Actions.h")



add_library(render_engine

	src/rend/render_engine.cpp

	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/vboindexer.cpp
	common/vboindexer.hpp
	common/quaternion_utils.cpp
	common/quaternion_utils.hpp

	res/StandardShading.GLSL
	res/StandardShading.GLSL

	src/rend/render_object.hpp
	src/rend/render_mesh.hpp
	src/rend/render_camera.hpp

 "src/serg/Player_Actions.h"

	
	src/rend/cube_map.cpp
	src/rend/cub_map.hpp
)


add_library(render
	src/rend/render.cpp
 "src/serg/Player_Actions.h")


target_link_libraries(render_engine
	${ALL_LIBS}
	OBJLoader
)

target_link_libraries(render
	render_engine
)

# sergey

add_library(serg_libs
	src/serg/serg_main.cpp
 "src/serg/Player_Actions.h")

#andrei

add_library(andrei_libs
	src/andrei/andrei_main.cpp
 "src/serg/Player_Actions.h")

# link libraries

add_executable(game_core
	src/game_core.cpp "src/serg/Player_Actions.h")

target_link_libraries(game_core
	${ALL_LIBS}
	${ADD_LIBS}
	ANTTWEAKBAR_116_OGLCORE_GLFW
	render
	render_engine
	
	
	serg_libs
	andrei_libs
)

set_target_properties(game_core PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_custom_command(
	TARGET game_core POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/res
	${CMAKE_CURRENT_BINARY_DIR}/res)

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*")
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$")
