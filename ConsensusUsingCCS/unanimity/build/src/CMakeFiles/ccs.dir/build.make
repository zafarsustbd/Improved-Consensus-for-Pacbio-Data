# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rathishdas/CompBio/newccs/unanimity

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rathishdas/CompBio/newccs/unanimity/build

# Include any dependencies generated for this target.
include src/CMakeFiles/ccs.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/ccs.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/ccs.dir/flags.make

src/CMakeFiles/ccs.dir/main/ccs.cpp.o: src/CMakeFiles/ccs.dir/flags.make
src/CMakeFiles/ccs.dir/main/ccs.cpp.o: ../src/main/ccs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rathishdas/CompBio/newccs/unanimity/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/ccs.dir/main/ccs.cpp.o"
	cd /home/rathishdas/CompBio/newccs/unanimity/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ccs.dir/main/ccs.cpp.o -c /home/rathishdas/CompBio/newccs/unanimity/src/main/ccs.cpp

src/CMakeFiles/ccs.dir/main/ccs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ccs.dir/main/ccs.cpp.i"
	cd /home/rathishdas/CompBio/newccs/unanimity/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rathishdas/CompBio/newccs/unanimity/src/main/ccs.cpp > CMakeFiles/ccs.dir/main/ccs.cpp.i

src/CMakeFiles/ccs.dir/main/ccs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ccs.dir/main/ccs.cpp.s"
	cd /home/rathishdas/CompBio/newccs/unanimity/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rathishdas/CompBio/newccs/unanimity/src/main/ccs.cpp -o CMakeFiles/ccs.dir/main/ccs.cpp.s

src/CMakeFiles/ccs.dir/main/ccs.cpp.o.requires:

.PHONY : src/CMakeFiles/ccs.dir/main/ccs.cpp.o.requires

src/CMakeFiles/ccs.dir/main/ccs.cpp.o.provides: src/CMakeFiles/ccs.dir/main/ccs.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/ccs.dir/build.make src/CMakeFiles/ccs.dir/main/ccs.cpp.o.provides.build
.PHONY : src/CMakeFiles/ccs.dir/main/ccs.cpp.o.provides

src/CMakeFiles/ccs.dir/main/ccs.cpp.o.provides.build: src/CMakeFiles/ccs.dir/main/ccs.cpp.o


# Object files for target ccs
ccs_OBJECTS = \
"CMakeFiles/ccs.dir/main/ccs.cpp.o"

# External object files for target ccs
ccs_EXTERNAL_OBJECTS =

ccs: src/CMakeFiles/ccs.dir/main/ccs.cpp.o
ccs: src/CMakeFiles/ccs.dir/build.make
ccs: src/libcc2.a
ccs: src/libunanimity.a
ccs: /usr/lib/x86_64-linux-gnu/libz.a
ccs: external/pbbam/build/external/htslib/libhts.a
ccs: external/pbbam/build/lib/libpbbam.a
ccs: external/pbbam/build/external/htslib/libhts.a
ccs: /usr/lib/x86_64-linux-gnu/libz.a
ccs: external/pbcopper/build/lib/libpbcopper.a
ccs: external/pbbam/build/lib/libpbbam.a
ccs: src/CMakeFiles/ccs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rathishdas/CompBio/newccs/unanimity/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../ccs"
	cd /home/rathishdas/CompBio/newccs/unanimity/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ccs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/ccs.dir/build: ccs

.PHONY : src/CMakeFiles/ccs.dir/build

src/CMakeFiles/ccs.dir/requires: src/CMakeFiles/ccs.dir/main/ccs.cpp.o.requires

.PHONY : src/CMakeFiles/ccs.dir/requires

src/CMakeFiles/ccs.dir/clean:
	cd /home/rathishdas/CompBio/newccs/unanimity/build/src && $(CMAKE_COMMAND) -P CMakeFiles/ccs.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/ccs.dir/clean

src/CMakeFiles/ccs.dir/depend:
	cd /home/rathishdas/CompBio/newccs/unanimity/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rathishdas/CompBio/newccs/unanimity /home/rathishdas/CompBio/newccs/unanimity/src /home/rathishdas/CompBio/newccs/unanimity/build /home/rathishdas/CompBio/newccs/unanimity/build/src /home/rathishdas/CompBio/newccs/unanimity/build/src/CMakeFiles/ccs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/ccs.dir/depend

