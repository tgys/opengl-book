# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /nix/store/5cl0n3viqbgfgh0zaizaag6p3s5444x6-cmake-3.19.7/bin/cmake

# The command to remove a file.
RM = /nix/store/5cl0n3viqbgfgh0zaizaag6p3s5444x6-cmake-3.19.7/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tei/projects/LearnOpenGL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tei/projects/LearnOpenGL/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/flags.make

CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp.o: CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/flags.make
CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp.o: ../src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tei/projects/LearnOpenGL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp.o"
	/nix/store/88ghxafjpqp5sqpd75r51qqg4q5d95ss-gcc-wrapper-10.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp.o -c /home/tei/projects/LearnOpenGL/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp

CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp.i"
	/nix/store/88ghxafjpqp5sqpd75r51qqg4q5d95ss-gcc-wrapper-10.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tei/projects/LearnOpenGL/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp > CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp.i

CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp.s"
	/nix/store/88ghxafjpqp5sqpd75r51qqg4q5d95ss-gcc-wrapper-10.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tei/projects/LearnOpenGL/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp -o CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp.s

# Object files for target 4.advanced_opengl__6.1.cubemaps_skybox
4_advanced_opengl__6_1_cubemaps_skybox_OBJECTS = \
"CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp.o"

# External object files for target 4.advanced_opengl__6.1.cubemaps_skybox
4_advanced_opengl__6_1_cubemaps_skybox_EXTERNAL_OBJECTS =

../bin/4.advanced_opengl/4.advanced_opengl__6.1.cubemaps_skybox: CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp.o
../bin/4.advanced_opengl/4.advanced_opengl__6.1.cubemaps_skybox: CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/build.make
../bin/4.advanced_opengl/4.advanced_opengl__6.1.cubemaps_skybox: /nix/store/a0j4adpnc5z9000lrhil0sv1xcj9y87k-glfw-3.3.4/lib/libglfw.so
../bin/4.advanced_opengl/4.advanced_opengl__6.1.cubemaps_skybox: /nix/store/5faibianv8qizafps484a9w15ds68si2-assimp-5.0.1/lib/libassimp.so
../bin/4.advanced_opengl/4.advanced_opengl__6.1.cubemaps_skybox: libSTB_IMAGE.a
../bin/4.advanced_opengl/4.advanced_opengl__6.1.cubemaps_skybox: libGLAD.a
../bin/4.advanced_opengl/4.advanced_opengl__6.1.cubemaps_skybox: CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tei/projects/LearnOpenGL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/4.advanced_opengl/4.advanced_opengl__6.1.cubemaps_skybox"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/build: ../bin/4.advanced_opengl/4.advanced_opengl__6.1.cubemaps_skybox

.PHONY : CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/build

CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/cmake_clean.cmake
.PHONY : CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/clean

CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/depend:
	cd /home/tei/projects/LearnOpenGL/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tei/projects/LearnOpenGL /home/tei/projects/LearnOpenGL /home/tei/projects/LearnOpenGL/cmake-build-debug /home/tei/projects/LearnOpenGL/cmake-build-debug /home/tei/projects/LearnOpenGL/cmake-build-debug/CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/4.advanced_opengl__6.1.cubemaps_skybox.dir/depend

