# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = "/Users/jim/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/221.5591.52/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/jim/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/221.5591.52/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jim/Downloads/NESP32-master/NESP32-Single-Game

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jim/Downloads/NESP32-master/NESP32-Single-Game/cmake-build-ili9341

# Utility rule file for Production.

# Include any custom commands dependencies for this target.
include CMakeFiles/Production.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Production.dir/progress.make

CMakeFiles/Production:
	cd /Users/jim/Downloads/NESP32-master/NESP32-Single-Game && platformio -c clion run -eILI9341

Production: CMakeFiles/Production
Production: CMakeFiles/Production.dir/build.make
.PHONY : Production

# Rule to build all files generated by this target.
CMakeFiles/Production.dir/build: Production
.PHONY : CMakeFiles/Production.dir/build

CMakeFiles/Production.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Production.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Production.dir/clean

CMakeFiles/Production.dir/depend:
	cd /Users/jim/Downloads/NESP32-master/NESP32-Single-Game/cmake-build-ili9341 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jim/Downloads/NESP32-master/NESP32-Single-Game /Users/jim/Downloads/NESP32-master/NESP32-Single-Game /Users/jim/Downloads/NESP32-master/NESP32-Single-Game/cmake-build-ili9341 /Users/jim/Downloads/NESP32-master/NESP32-Single-Game/cmake-build-ili9341 /Users/jim/Downloads/NESP32-master/NESP32-Single-Game/cmake-build-ili9341/CMakeFiles/Production.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Production.dir/depend
