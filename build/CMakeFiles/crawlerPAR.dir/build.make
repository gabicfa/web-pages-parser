# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages/build"

# Include any dependencies generated for this target.
include CMakeFiles/crawlerPAR.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/crawlerPAR.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/crawlerPAR.dir/flags.make

CMakeFiles/crawlerPAR.dir/crawlerParalelo.cpp.o: CMakeFiles/crawlerPAR.dir/flags.make
CMakeFiles/crawlerPAR.dir/crawlerParalelo.cpp.o: ../crawlerParalelo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/crawlerPAR.dir/crawlerParalelo.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/crawlerPAR.dir/crawlerParalelo.cpp.o -c "/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages/crawlerParalelo.cpp"

CMakeFiles/crawlerPAR.dir/crawlerParalelo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/crawlerPAR.dir/crawlerParalelo.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages/crawlerParalelo.cpp" > CMakeFiles/crawlerPAR.dir/crawlerParalelo.cpp.i

CMakeFiles/crawlerPAR.dir/crawlerParalelo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/crawlerPAR.dir/crawlerParalelo.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages/crawlerParalelo.cpp" -o CMakeFiles/crawlerPAR.dir/crawlerParalelo.cpp.s

# Object files for target crawlerPAR
crawlerPAR_OBJECTS = \
"CMakeFiles/crawlerPAR.dir/crawlerParalelo.cpp.o"

# External object files for target crawlerPAR
crawlerPAR_EXTERNAL_OBJECTS =

crawlerPAR: CMakeFiles/crawlerPAR.dir/crawlerParalelo.cpp.o
crawlerPAR: CMakeFiles/crawlerPAR.dir/build.make
crawlerPAR: CMakeFiles/crawlerPAR.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable crawlerPAR"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/crawlerPAR.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/crawlerPAR.dir/build: crawlerPAR

.PHONY : CMakeFiles/crawlerPAR.dir/build

CMakeFiles/crawlerPAR.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/crawlerPAR.dir/cmake_clean.cmake
.PHONY : CMakeFiles/crawlerPAR.dir/clean

CMakeFiles/crawlerPAR.dir/depend:
	cd "/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages" "/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages" "/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages/build" "/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages/build" "/Users/gabrielaalmeida/Documents/Insper/7 semestre/SuperComp/ParsingWebPages/build/CMakeFiles/crawlerPAR.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/crawlerPAR.dir/depend
