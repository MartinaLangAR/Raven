# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/Documents/Raven/version1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/Documents/Raven/version1

# Include any dependencies generated for this target.
include CMakeFiles/RavenLib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RavenLib.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RavenLib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RavenLib.dir/flags.make

CMakeFiles/RavenLib.dir/facedetector.cpp.o: CMakeFiles/RavenLib.dir/flags.make
CMakeFiles/RavenLib.dir/facedetector.cpp.o: facedetector.cpp
CMakeFiles/RavenLib.dir/facedetector.cpp.o: CMakeFiles/RavenLib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/Raven/version1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RavenLib.dir/facedetector.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RavenLib.dir/facedetector.cpp.o -MF CMakeFiles/RavenLib.dir/facedetector.cpp.o.d -o CMakeFiles/RavenLib.dir/facedetector.cpp.o -c /home/pi/Documents/Raven/version1/facedetector.cpp

CMakeFiles/RavenLib.dir/facedetector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RavenLib.dir/facedetector.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Documents/Raven/version1/facedetector.cpp > CMakeFiles/RavenLib.dir/facedetector.cpp.i

CMakeFiles/RavenLib.dir/facedetector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RavenLib.dir/facedetector.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Documents/Raven/version1/facedetector.cpp -o CMakeFiles/RavenLib.dir/facedetector.cpp.s

CMakeFiles/RavenLib.dir/servos.cpp.o: CMakeFiles/RavenLib.dir/flags.make
CMakeFiles/RavenLib.dir/servos.cpp.o: servos.cpp
CMakeFiles/RavenLib.dir/servos.cpp.o: CMakeFiles/RavenLib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/Raven/version1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RavenLib.dir/servos.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RavenLib.dir/servos.cpp.o -MF CMakeFiles/RavenLib.dir/servos.cpp.o.d -o CMakeFiles/RavenLib.dir/servos.cpp.o -c /home/pi/Documents/Raven/version1/servos.cpp

CMakeFiles/RavenLib.dir/servos.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RavenLib.dir/servos.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Documents/Raven/version1/servos.cpp > CMakeFiles/RavenLib.dir/servos.cpp.i

CMakeFiles/RavenLib.dir/servos.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RavenLib.dir/servos.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Documents/Raven/version1/servos.cpp -o CMakeFiles/RavenLib.dir/servos.cpp.s

# Object files for target RavenLib
RavenLib_OBJECTS = \
"CMakeFiles/RavenLib.dir/facedetector.cpp.o" \
"CMakeFiles/RavenLib.dir/servos.cpp.o"

# External object files for target RavenLib
RavenLib_EXTERNAL_OBJECTS =

libRavenLib.a: CMakeFiles/RavenLib.dir/facedetector.cpp.o
libRavenLib.a: CMakeFiles/RavenLib.dir/servos.cpp.o
libRavenLib.a: CMakeFiles/RavenLib.dir/build.make
libRavenLib.a: CMakeFiles/RavenLib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/Documents/Raven/version1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libRavenLib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/RavenLib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RavenLib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RavenLib.dir/build: libRavenLib.a
.PHONY : CMakeFiles/RavenLib.dir/build

CMakeFiles/RavenLib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RavenLib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RavenLib.dir/clean

CMakeFiles/RavenLib.dir/depend:
	cd /home/pi/Documents/Raven/version1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/Documents/Raven/version1 /home/pi/Documents/Raven/version1 /home/pi/Documents/Raven/version1 /home/pi/Documents/Raven/version1 /home/pi/Documents/Raven/version1/CMakeFiles/RavenLib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RavenLib.dir/depend

