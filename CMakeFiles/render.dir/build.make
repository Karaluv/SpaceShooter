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
include CMakeFiles/render.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/render.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/render.dir/flags.make

CMakeFiles/render.dir/src/rend/render.cpp.o: CMakeFiles/render.dir/flags.make
CMakeFiles/render.dir/src/rend/render.cpp.o: src/rend/render.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home-local/student/SpaceShooter/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/render.dir/src/rend/render.cpp.o"
	/opt/rh/devtoolset-11/root/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/render.dir/src/rend/render.cpp.o -c /home-local/student/SpaceShooter/src/rend/render.cpp

CMakeFiles/render.dir/src/rend/render.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render.dir/src/rend/render.cpp.i"
	/opt/rh/devtoolset-11/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home-local/student/SpaceShooter/src/rend/render.cpp > CMakeFiles/render.dir/src/rend/render.cpp.i

CMakeFiles/render.dir/src/rend/render.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render.dir/src/rend/render.cpp.s"
	/opt/rh/devtoolset-11/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home-local/student/SpaceShooter/src/rend/render.cpp -o CMakeFiles/render.dir/src/rend/render.cpp.s

# Object files for target render
render_OBJECTS = \
"CMakeFiles/render.dir/src/rend/render.cpp.o"

# External object files for target render
render_EXTERNAL_OBJECTS =

librender.a: CMakeFiles/render.dir/src/rend/render.cpp.o
librender.a: CMakeFiles/render.dir/build.make
librender.a: CMakeFiles/render.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home-local/student/SpaceShooter/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library librender.a"
	$(CMAKE_COMMAND) -P CMakeFiles/render.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/render.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/render.dir/build: librender.a

.PHONY : CMakeFiles/render.dir/build

CMakeFiles/render.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/render.dir/cmake_clean.cmake
.PHONY : CMakeFiles/render.dir/clean

CMakeFiles/render.dir/depend:
	cd /home-local/student/SpaceShooter && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home-local/student/SpaceShooter /home-local/student/SpaceShooter /home-local/student/SpaceShooter /home-local/student/SpaceShooter /home-local/student/SpaceShooter/CMakeFiles/render.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/render.dir/depend

