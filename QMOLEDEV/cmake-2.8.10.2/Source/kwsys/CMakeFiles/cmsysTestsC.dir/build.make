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
include Source/kwsys/CMakeFiles/cmsysTestsC.dir/depend.make

# Include the progress variables for this target.
include Source/kwsys/CMakeFiles/cmsysTestsC.dir/progress.make

# Include the compile flags for this target's objects.
include Source/kwsys/CMakeFiles/cmsysTestsC.dir/flags.make

Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o: Source/kwsys/CMakeFiles/cmsysTestsC.dir/flags.make
Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o: Source/kwsys/cmsysTestsC.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chris/cmake-2.8.10.2/CMakeFiles $(CMAKE_PROGRESS_1)
	@echo "Building C object Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o   -c /home/chris/cmake-2.8.10.2/Source/kwsys/cmsysTestsC.c

Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.i: cmake_force
	@echo "Preprocessing C source to CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.i"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/chris/cmake-2.8.10.2/Source/kwsys/cmsysTestsC.c > CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.i

Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.s: cmake_force
	@echo "Compiling C source to assembly CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.s"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/chris/cmake-2.8.10.2/Source/kwsys/cmsysTestsC.c -o CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.s

Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o.requires:
.PHONY : Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o.requires

Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o.provides: Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o.requires
	$(MAKE) -f Source/kwsys/CMakeFiles/cmsysTestsC.dir/build.make Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o.provides.build
.PHONY : Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o.provides

Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o.provides.build: Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o

Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o: Source/kwsys/CMakeFiles/cmsysTestsC.dir/flags.make
Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o: Source/kwsys/testEncode.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chris/cmake-2.8.10.2/CMakeFiles $(CMAKE_PROGRESS_2)
	@echo "Building C object Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/cmsysTestsC.dir/testEncode.c.o   -c /home/chris/cmake-2.8.10.2/Source/kwsys/testEncode.c

Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.i: cmake_force
	@echo "Preprocessing C source to CMakeFiles/cmsysTestsC.dir/testEncode.c.i"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/chris/cmake-2.8.10.2/Source/kwsys/testEncode.c > CMakeFiles/cmsysTestsC.dir/testEncode.c.i

Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.s: cmake_force
	@echo "Compiling C source to assembly CMakeFiles/cmsysTestsC.dir/testEncode.c.s"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/chris/cmake-2.8.10.2/Source/kwsys/testEncode.c -o CMakeFiles/cmsysTestsC.dir/testEncode.c.s

Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o.requires:
.PHONY : Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o.requires

Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o.provides: Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o.requires
	$(MAKE) -f Source/kwsys/CMakeFiles/cmsysTestsC.dir/build.make Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o.provides.build
.PHONY : Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o.provides

Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o.provides.build: Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o

Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o: Source/kwsys/CMakeFiles/cmsysTestsC.dir/flags.make
Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o: Source/kwsys/testTerminal.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chris/cmake-2.8.10.2/CMakeFiles $(CMAKE_PROGRESS_3)
	@echo "Building C object Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/cmsysTestsC.dir/testTerminal.c.o   -c /home/chris/cmake-2.8.10.2/Source/kwsys/testTerminal.c

Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.i: cmake_force
	@echo "Preprocessing C source to CMakeFiles/cmsysTestsC.dir/testTerminal.c.i"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/chris/cmake-2.8.10.2/Source/kwsys/testTerminal.c > CMakeFiles/cmsysTestsC.dir/testTerminal.c.i

Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.s: cmake_force
	@echo "Compiling C source to assembly CMakeFiles/cmsysTestsC.dir/testTerminal.c.s"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/chris/cmake-2.8.10.2/Source/kwsys/testTerminal.c -o CMakeFiles/cmsysTestsC.dir/testTerminal.c.s

Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o.requires:
.PHONY : Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o.requires

Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o.provides: Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o.requires
	$(MAKE) -f Source/kwsys/CMakeFiles/cmsysTestsC.dir/build.make Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o.provides.build
.PHONY : Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o.provides

Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o.provides.build: Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o

# Object files for target cmsysTestsC
cmsysTestsC_OBJECTS = \
"CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o" \
"CMakeFiles/cmsysTestsC.dir/testEncode.c.o" \
"CMakeFiles/cmsysTestsC.dir/testTerminal.c.o"

# External object files for target cmsysTestsC
cmsysTestsC_EXTERNAL_OBJECTS =

Source/kwsys/cmsysTestsC: Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o
Source/kwsys/cmsysTestsC: Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o
Source/kwsys/cmsysTestsC: Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o
Source/kwsys/cmsysTestsC: Source/kwsys/CMakeFiles/cmsysTestsC.dir/build.make
Source/kwsys/cmsysTestsC: Source/kwsys/libcmsys_c.a
Source/kwsys/cmsysTestsC: Source/kwsys/CMakeFiles/cmsysTestsC.dir/link.txt
	@echo "Linking C executable cmsysTestsC"
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cmsysTestsC.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Source/kwsys/CMakeFiles/cmsysTestsC.dir/build: Source/kwsys/cmsysTestsC
.PHONY : Source/kwsys/CMakeFiles/cmsysTestsC.dir/build

Source/kwsys/CMakeFiles/cmsysTestsC.dir/requires: Source/kwsys/CMakeFiles/cmsysTestsC.dir/cmsysTestsC.c.o.requires
Source/kwsys/CMakeFiles/cmsysTestsC.dir/requires: Source/kwsys/CMakeFiles/cmsysTestsC.dir/testEncode.c.o.requires
Source/kwsys/CMakeFiles/cmsysTestsC.dir/requires: Source/kwsys/CMakeFiles/cmsysTestsC.dir/testTerminal.c.o.requires
.PHONY : Source/kwsys/CMakeFiles/cmsysTestsC.dir/requires

Source/kwsys/CMakeFiles/cmsysTestsC.dir/clean:
	cd /home/chris/cmake-2.8.10.2/Source/kwsys && $(CMAKE_COMMAND) -P CMakeFiles/cmsysTestsC.dir/cmake_clean.cmake
.PHONY : Source/kwsys/CMakeFiles/cmsysTestsC.dir/clean

Source/kwsys/CMakeFiles/cmsysTestsC.dir/depend:
	cd /home/chris/cmake-2.8.10.2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chris/cmake-2.8.10.2 /home/chris/cmake-2.8.10.2/Source/kwsys /home/chris/cmake-2.8.10.2 /home/chris/cmake-2.8.10.2/Source/kwsys /home/chris/cmake-2.8.10.2/Source/kwsys/CMakeFiles/cmsysTestsC.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Source/kwsys/CMakeFiles/cmsysTestsC.dir/depend

