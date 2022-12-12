# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake3

# The command to remove a file.
RM = /usr/bin/cmake3 -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home-local/student/SpaceShooter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home-local/student/SpaceShooter

# Include any dependencies generated for this target.
include CMakeFiles/game_core.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/game_core.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/game_core.dir/flags.make

CMakeFiles/game_core.dir/src/game_core.cpp.o: CMakeFiles/game_core.dir/flags.make
CMakeFiles/game_core.dir/src/game_core.cpp.o: src/game_core.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home-local/student/SpaceShooter/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/game_core.dir/src/game_core.cpp.o"
	/opt/rh/devtoolset-11/root/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/game_core.dir/src/game_core.cpp.o -c /home-local/student/SpaceShooter/src/game_core.cpp

CMakeFiles/game_core.dir/src/game_core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/game_core.dir/src/game_core.cpp.i"
	/opt/rh/devtoolset-11/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home-local/student/SpaceShooter/src/game_core.cpp > CMakeFiles/game_core.dir/src/game_core.cpp.i

CMakeFiles/game_core.dir/src/game_core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/game_core.dir/src/game_core.cpp.s"
	/opt/rh/devtoolset-11/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home-local/student/SpaceShooter/src/game_core.cpp -o CMakeFiles/game_core.dir/src/game_core.cpp.s

# Object files for target game_core
game_core_OBJECTS = \
"CMakeFiles/game_core.dir/src/game_core.cpp.o"

# External object files for target game_core
game_core_EXTERNAL_OBJECTS =

game_core: CMakeFiles/game_core.dir/src/game_core.cpp.o
game_core: CMakeFiles/game_core.dir/build.make
game_core: /usr/lib64/libGL.so
game_core: /usr/lib64/libGLU.so
game_core: external/glfw-3.1.2/src/libglfw3.a
game_core: external/libGLEW_1130.a
game_core: libmynum.a
game_core: libmydict.a
game_core: libmycontrol.a
game_core: libmymatrix.a
game_core: external/libANTTWEAKBAR_116_OGLCORE_GLFW.a
game_core: librender.a
game_core: librender_engine.a
game_core: libserg_libs.a
game_core: libandrei_libs.a
game_core: external/glfw-3.1.2/src/libglfw3.a
game_core: external/libGLEW_1130.a
game_core: /usr/lib64/librt.so
game_core: /usr/lib64/libm.so
game_core: /usr/lib64/libX11.so
game_core: /usr/lib64/libXrandr.so
game_core: /usr/lib64/libXinerama.so
game_core: /usr/lib64/libXi.so
game_core: /usr/lib64/libXxf86vm.so
game_core: /usr/lib64/libXcursor.so
game_core: /usr/lib64/librt.so
game_core: /usr/lib64/libm.so
game_core: /usr/lib64/libX11.so
game_core: /usr/lib64/libXrandr.so
game_core: /usr/lib64/libXinerama.so
game_core: /usr/lib64/libXi.so
game_core: /usr/lib64/libXxf86vm.so
game_core: /usr/lib64/libXcursor.so
game_core: /usr/lib64/libGL.so
game_core: /usr/lib64/libGLU.so
game_core: libOBJLoader.a
game_core: CMakeFiles/game_core.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home-local/student/SpaceShooter/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable game_core"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/game_core.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake3 -E copy_directory /home-local/student/SpaceShooter/res /home-local/student/SpaceShooter/res

# Rule to build all files generated by this target.
CMakeFiles/game_core.dir/build: game_core

.PHONY : CMakeFiles/game_core.dir/build

CMakeFiles/game_core.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/game_core.dir/cmake_clean.cmake
.PHONY : CMakeFiles/game_core.dir/clean

CMakeFiles/game_core.dir/depend:
	cd /home-local/student/SpaceShooter && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home-local/student/SpaceShooter /home-local/student/SpaceShooter /home-local/student/SpaceShooter /home-local/student/SpaceShooter /home-local/student/SpaceShooter/CMakeFiles/game_core.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/game_core.dir/depend

