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
include Utilities/cmcurl/CMakeFiles/LIBCURL.dir/depend.make

# Include the progress variables for this target.
include Utilities/cmcurl/CMakeFiles/LIBCURL.dir/progress.make

# Include the compile flags for this target's objects.
include Utilities/cmcurl/CMakeFiles/LIBCURL.dir/flags.make

Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o: Utilities/cmcurl/CMakeFiles/LIBCURL.dir/flags.make
Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o: Utilities/cmcurl/Testing/curltest.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chris/cmake-2.8.10.2/CMakeFiles $(CMAKE_PROGRESS_1)
	@echo "Building C object Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o"
	cd /home/chris/cmake-2.8.10.2/Utilities/cmcurl && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/LIBCURL.dir/Testing/curltest.c.o   -c /home/chris/cmake-2.8.10.2/Utilities/cmcurl/Testing/curltest.c

Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.i: cmake_force
	@echo "Preprocessing C source to CMakeFiles/LIBCURL.dir/Testing/curltest.c.i"
	cd /home/chris/cmake-2.8.10.2/Utilities/cmcurl && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/chris/cmake-2.8.10.2/Utilities/cmcurl/Testing/curltest.c > CMakeFiles/LIBCURL.dir/Testing/curltest.c.i

Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.s: cmake_force
	@echo "Compiling C source to assembly CMakeFiles/LIBCURL.dir/Testing/curltest.c.s"
	cd /home/chris/cmake-2.8.10.2/Utilities/cmcurl && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/chris/cmake-2.8.10.2/Utilities/cmcurl/Testing/curltest.c -o CMakeFiles/LIBCURL.dir/Testing/curltest.c.s

Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o.requires:
.PHONY : Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o.requires

Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o.provides: Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o.requires
	$(MAKE) -f Utilities/cmcurl/CMakeFiles/LIBCURL.dir/build.make Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o.provides.build
.PHONY : Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o.provides

Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o.provides.build: Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o

# Object files for target LIBCURL
LIBCURL_OBJECTS = \
"CMakeFiles/LIBCURL.dir/Testing/curltest.c.o"

# External object files for target LIBCURL
LIBCURL_EXTERNAL_OBJECTS =

Utilities/cmcurl/LIBCURL: Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o
Utilities/cmcurl/LIBCURL: Utilities/cmcurl/CMakeFiles/LIBCURL.dir/build.make
Utilities/cmcurl/LIBCURL: Utilities/cmcurl/libcmcurl.a
Utilities/cmcurl/LIBCURL: Utilities/cmzlib/libcmzlib.a
Utilities/cmcurl/LIBCURL: Utilities/cmcurl/CMakeFiles/LIBCURL.dir/link.txt
	@echo "Linking C executable LIBCURL"
	cd /home/chris/cmake-2.8.10.2/Utilities/cmcurl && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LIBCURL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Utilities/cmcurl/CMakeFiles/LIBCURL.dir/build: Utilities/cmcurl/LIBCURL
.PHONY : Utilities/cmcurl/CMakeFiles/LIBCURL.dir/build

Utilities/cmcurl/CMakeFiles/LIBCURL.dir/requires: Utilities/cmcurl/CMakeFiles/LIBCURL.dir/Testing/curltest.c.o.requires
.PHONY : Utilities/cmcurl/CMakeFiles/LIBCURL.dir/requires

Utilities/cmcurl/CMakeFiles/LIBCURL.dir/clean:
	cd /home/chris/cmake-2.8.10.2/Utilities/cmcurl && $(CMAKE_COMMAND) -P CMakeFiles/LIBCURL.dir/cmake_clean.cmake
.PHONY : Utilities/cmcurl/CMakeFiles/LIBCURL.dir/clean

Utilities/cmcurl/CMakeFiles/LIBCURL.dir/depend:
	cd /home/chris/cmake-2.8.10.2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chris/cmake-2.8.10.2 /home/chris/cmake-2.8.10.2/Utilities/cmcurl /home/chris/cmake-2.8.10.2 /home/chris/cmake-2.8.10.2/Utilities/cmcurl /home/chris/cmake-2.8.10.2/Utilities/cmcurl/CMakeFiles/LIBCURL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Utilities/cmcurl/CMakeFiles/LIBCURL.dir/depend

