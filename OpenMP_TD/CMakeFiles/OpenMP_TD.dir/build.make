# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.26.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.26.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD"

# Include any dependencies generated for this target.
include CMakeFiles/OpenMP_TD.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/OpenMP_TD.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/OpenMP_TD.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/OpenMP_TD.dir/flags.make

CMakeFiles/OpenMP_TD.dir/main.cpp.o: CMakeFiles/OpenMP_TD.dir/flags.make
CMakeFiles/OpenMP_TD.dir/main.cpp.o: main.cpp
CMakeFiles/OpenMP_TD.dir/main.cpp.o: CMakeFiles/OpenMP_TD.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/OpenMP_TD.dir/main.cpp.o"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/OpenMP_TD.dir/main.cpp.o -MF CMakeFiles/OpenMP_TD.dir/main.cpp.o.d -o CMakeFiles/OpenMP_TD.dir/main.cpp.o -c "/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD/main.cpp"

CMakeFiles/OpenMP_TD.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OpenMP_TD.dir/main.cpp.i"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD/main.cpp" > CMakeFiles/OpenMP_TD.dir/main.cpp.i

CMakeFiles/OpenMP_TD.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OpenMP_TD.dir/main.cpp.s"
	/usr/local/opt/llvm/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD/main.cpp" -o CMakeFiles/OpenMP_TD.dir/main.cpp.s

# Object files for target OpenMP_TD
OpenMP_TD_OBJECTS = \
"CMakeFiles/OpenMP_TD.dir/main.cpp.o"

# External object files for target OpenMP_TD
OpenMP_TD_EXTERNAL_OBJECTS =

OpenMP_TD: CMakeFiles/OpenMP_TD.dir/main.cpp.o
OpenMP_TD: CMakeFiles/OpenMP_TD.dir/build.make
OpenMP_TD: CMakeFiles/OpenMP_TD.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable OpenMP_TD"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OpenMP_TD.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/OpenMP_TD.dir/build: OpenMP_TD
.PHONY : CMakeFiles/OpenMP_TD.dir/build

CMakeFiles/OpenMP_TD.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/OpenMP_TD.dir/cmake_clean.cmake
.PHONY : CMakeFiles/OpenMP_TD.dir/clean

CMakeFiles/OpenMP_TD.dir/depend:
	cd "/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD" "/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD" "/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD" "/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD" "/Users/yaroeugenepascal/Documents/Documents/DE3/Multi threading/OpenMP_TD/CMakeFiles/OpenMP_TD.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/OpenMP_TD.dir/depend

