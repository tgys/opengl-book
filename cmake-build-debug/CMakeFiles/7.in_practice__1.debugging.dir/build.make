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
include CMakeFiles/7.in_practice__1.debugging.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/7.in_practice__1.debugging.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/7.in_practice__1.debugging.dir/flags.make

CMakeFiles/7.in_practice__1.debugging.dir/src/7.in_practice/1.debugging/debugging.cpp.o: CMakeFiles/7.in_practice__1.debugging.dir/flags.make
CMakeFiles/7.in_practice__1.debugging.dir/src/7.in_practice/1.debugging/debugging.cpp.o: ../src/7.in_practice/1.debugging/debugging.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tei/projects/LearnOpenGL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/7.in_practice__1.debugging.dir/src/7.in_practice/1.debugging/debugging.cpp.o"
	/nix/store/88ghxafjpqp5sqpd75r51qqg4q5d95ss-gcc-wrapper-10.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/7.in_practice__1.debugging.dir/src/7.in_practice/1.debugging/debugging.cpp.o -c /home/tei/projects/LearnOpenGL/src/7.in_practice/1.debugging/debugging.cpp

CMakeFiles/7.in_practice__1.debugging.dir/src/7.in_practice/1.debugging/debugging.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/7.in_practice__1.debugging.dir/src/7.in_practice/1.debugging/debugging.cpp.i"
	/nix/store/88ghxafjpqp5sqpd75r51qqg4q5d95ss-gcc-wrapper-10.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tei/projects/LearnOpenGL/src/7.in_practice/1.debugging/debugging.cpp > CMakeFiles/7.in_practice__1.debugging.dir/src/7.in_practice/1.debugging/debugging.cpp.i

CMakeFiles/7.in_practice__1.debugging.dir/src/7.in_practice/1.debugging/debugging.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/7.in_practice__1.debugging.dir/src/7.in_practice/1.debugging/debugging.cpp.s"
	/nix/store/88ghxafjpqp5sqpd75r51qqg4q5d95ss-gcc-wrapper-10.3.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tei/projects/LearnOpenGL/src/7.in_practice/1.debugging/debugging.cpp -o CMakeFiles/7.in_practice__1.debugging.dir/src/7.in_practice/1.debugging/debugging.cpp.s

# Object files for target 7.in_practice__1.debugging
7_in_practice__1_debugging_OBJECTS = \
"CMakeFiles/7.in_practice__1.debugging.dir/src/7.in_practice/1.debugging/debugging.cpp.o"

# External object files for target 7.in_practice__1.debugging
7_in_practice__1_debugging_EXTERNAL_OBJECTS =

../bin/7.in_practice/7.in_practice__1.debugging: CMakeFiles/7.in_practice__1.debugging.dir/src/7.in_practice/1.debugging/debugging.cpp.o
../bin/7.in_practice/7.in_practice__1.debugging: CMakeFiles/7.in_practice__1.debugging.dir/build.make
../bin/7.in_practice/7.in_practice__1.debugging: /nix/store/a0j4adpnc5z9000lrhil0sv1xcj9y87k-glfw-3.3.4/lib/libglfw.so
../bin/7.in_practice/7.in_practice__1.debugging: /nix/store/5faibianv8qizafps484a9w15ds68si2-assimp-5.0.1/lib/libassimp.so
../bin/7.in_practice/7.in_practice__1.debugging: libSTB_IMAGE.a
../bin/7.in_practice/7.in_practice__1.debugging: libGLAD.a
../bin/7.in_practice/7.in_practice__1.debugging: CMakeFiles/7.in_practice__1.debugging.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tei/projects/LearnOpenGL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/7.in_practice/7.in_practice__1.debugging"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/7.in_practice__1.debugging.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/7.in_practice__1.debugging.dir/build: ../bin/7.in_practice/7.in_practice__1.debugging

.PHONY : CMakeFiles/7.in_practice__1.debugging.dir/build

CMakeFiles/7.in_practice__1.debugging.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/7.in_practice__1.debugging.dir/cmake_clean.cmake
.PHONY : CMakeFiles/7.in_practice__1.debugging.dir/clean

CMakeFiles/7.in_practice__1.debugging.dir/depend:
	cd /home/tei/projects/LearnOpenGL/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tei/projects/LearnOpenGL /home/tei/projects/LearnOpenGL /home/tei/projects/LearnOpenGL/cmake-build-debug /home/tei/projects/LearnOpenGL/cmake-build-debug /home/tei/projects/LearnOpenGL/cmake-build-debug/CMakeFiles/7.in_practice__1.debugging.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/7.in_practice__1.debugging.dir/depend

