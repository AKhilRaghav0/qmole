# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /home/chris/cmake-2.8.10.2/Bootstrap.cmk/cmake

# The command to remove a file.
RM = /home/chris/cmake-2.8.10.2/Bootstrap.cmk/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chris/cmake-2.8.10.2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chris/cmake-2.8.10.2

# Include any dependencies generated for this target.
include Source/kwsys/CMakeFiles/cmsysTestDynload.dir/depend.make

# Include the progress variables for this target.
include Source/kwsys/CMakeFiles/cmsysTestDynload.dir/progress.make

# Include the compile flags for this target's objects.
include Source/kwsys/CMakeFiles/cmsysTestDynload.dir/flags.make

Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o: Source/kwsys/CMakeFiles/cmsysTestDynload.dir/flags.make
Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o: Source/kwsys/testDynload.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chris/cmake-2.8.10.2/CMakeFiles $(CMAKE_PROGRESS_1)
	@echo "Building C object Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/cmsysTestDynload.dir/testDynload.c.o   -c /home/chris/cmake-2.8.10.2/Source/kwsys/testDynload.c

Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.i: cmake_force
	@echo "Preprocessing C source to CMakeFiles/cmsysTestDynload.dir/testDynload.c.i"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/chris/cmake-2.8.10.2/Source/kwsys/testDynload.c > CMakeFiles/cmsysTestDynload.dir/testDynload.c.i

Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.s: cmake_force
	@echo "Compiling C source to assembly CMakeFiles/cmsysTestDynload.dir/testDynload.c.s"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/chris/cmake-2.8.10.2/Source/kwsys/testDynload.c -o CMakeFiles/cmsysTestDynload.dir/testDynload.c.s

Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o.requires:
.PHONY : Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o.requires

Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o.provides: Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o.requires
	$(MAKE) -f Source/kwsys/CMakeFiles/cmsysTestDynload.dir/build.make Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o.provides.build
.PHONY : Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o.provides

Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o.provides.build: Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o

# Object files for target cmsysTestDynload
cmsysTestDynload_OBJECTS = \
"CMakeFiles/cmsysTestDynload.dir/testDynload.c.o"

# External object files for target cmsysTestDynload
cmsysTestDynload_EXTERNAL_OBJECTS =

Source/kwsys/libcmsysTestDynload.so: Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o
Source/kwsys/libcmsysTestDynload.so: Source/kwsys/CMakeFiles/cmsysTestDynload.dir/build.make
Source/kwsys/libcmsysTestDynload.so: Source/kwsys/CMakeFiles/cmsysTestDynload.dir/link.txt
	@echo "Linking C shared module libcmsysTestDynload.so"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cmsysTestDynload.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Source/kwsys/CMakeFiles/cmsysTestDynload.dir/build: Source/kwsys/libcmsysTestDynload.so
.PHONY : Source/kwsys/CMakeFiles/cmsysTestDynload.dir/build

Source/kwsys/CMakeFiles/cmsysTestDynload.dir/requires: Source/kwsys/CMakeFiles/cmsysTestDynload.dir/testDynload.c.o.requires
.PHONY : Source/kwsys/CMakeFiles/cmsysTestDynload.dir/requires

Source/kwsys/CMakeFiles/cmsysTestDynload.dir/clean:
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && $(CMAKE_COMMAND) -P CMakeFiles/cmsysTestDynload.dir/cmake_clean.cmake
.PHONY : Source/kwsys/CMakeFiles/cmsysTestDynload.dir/clean

Source/kwsys/CMakeFiles/cmsysTestDynload.dir/depend:
	cd /home/chris/cmake-2.8.10.2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chris/cmake-2.8.10.2 /home/chris/cmake-2.8.10.2/Source/kwsys /home/chris/cmake-2.8.10.2 /home/chris/cmake-2.8.10.2/Source/kwsys /home/chris/cmake-2.8.10.2/Source/kwsys/CMakeFiles/cmsysTestDynload.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Source/kwsys/CMakeFiles/cmsysTestDynload.dir/depend

