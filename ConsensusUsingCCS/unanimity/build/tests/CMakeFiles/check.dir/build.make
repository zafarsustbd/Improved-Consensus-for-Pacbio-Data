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

# Utility rule file for check.

# Include the progress variables for this target.
include tests/CMakeFiles/check.dir/progress.make

tests/CMakeFiles/check: ccs
	cd /home/rathishdas/CompBio/newccs/unanimity && /home/rathishdas/CompBio/newccs/unanimity/tools/check-formatting --all
	cd /home/rathishdas/CompBio/newccs/unanimity && /home/rathishdas/CompBio/newccs/unanimity/build/tests/test_unanimity --gtest_output=xml:/home/rathishdas/CompBio/newccs/unanimity/build/uny-unit.xml
	cd /home/rathishdas/CompBio/newccs/unanimity && __PBTEST_CCS_EXE=/home/rathishdas/CompBio/newccs/unanimity/build/ccs /home/rathishdas/CompBio/newccs/unanimity/scripts/cram --xunit-file=/home/rathishdas/CompBio/newccs/unanimity/build/uny-cram.xml /home/rathishdas/CompBio/newccs/unanimity/tests/cram/*.t
	cd /home/rathishdas/CompBio/newccs/unanimity && __PBTEST_CCS_EXE=/home/rathishdas/CompBio/newccs/unanimity/build/ccs /home/rathishdas/CompBio/newccs/unanimity/tests/python/test_tool_contract.py

check: tests/CMakeFiles/check
check: tests/CMakeFiles/check.dir/build.make

.PHONY : check

# Rule to build all files generated by this target.
tests/CMakeFiles/check.dir/build: check

.PHONY : tests/CMakeFiles/check.dir/build

tests/CMakeFiles/check.dir/clean:
	cd /home/rathishdas/CompBio/newccs/unanimity/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/check.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/check.dir/clean

tests/CMakeFiles/check.dir/depend:
	cd /home/rathishdas/CompBio/newccs/unanimity/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rathishdas/CompBio/newccs/unanimity /home/rathishdas/CompBio/newccs/unanimity/tests /home/rathishdas/CompBio/newccs/unanimity/build /home/rathishdas/CompBio/newccs/unanimity/build/tests /home/rathishdas/CompBio/newccs/unanimity/build/tests/CMakeFiles/check.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/check.dir/depend

