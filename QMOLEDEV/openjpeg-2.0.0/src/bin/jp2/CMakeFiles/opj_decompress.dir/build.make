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
CMAKE_COMMAND = /home/local/bin/cmake

# The command to remove a file.
RM = /home/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /home/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chris/openjpeg-2.0.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chris/openjpeg-2.0.0

# Include any dependencies generated for this target.
include src/bin/jp2/CMakeFiles/opj_decompress.dir/depend.make

# Include the progress variables for this target.
include src/bin/jp2/CMakeFiles/opj_decompress.dir/progress.make

# Include the compile flags for this target's objects.
include src/bin/jp2/CMakeFiles/opj_decompress.dir/flags.make

src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o: src/bin/jp2/CMakeFiles/opj_decompress.dir/flags.make
src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o: src/bin/jp2/opj_decompress.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chris/openjpeg-2.0.0/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/opj_decompress.dir/opj_decompress.c.o   -c /home/chris/openjpeg-2.0.0/src/bin/jp2/opj_decompress.c

src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/opj_decompress.dir/opj_decompress.c.i"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/chris/openjpeg-2.0.0/src/bin/jp2/opj_decompress.c > CMakeFiles/opj_decompress.dir/opj_decompress.c.i

src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/opj_decompress.dir/opj_decompress.c.s"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/chris/openjpeg-2.0.0/src/bin/jp2/opj_decompress.c -o CMakeFiles/opj_decompress.dir/opj_decompress.c.s

src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o.requires:
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o.requires

src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o.provides: src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o.requires
	$(MAKE) -f src/bin/jp2/CMakeFiles/opj_decompress.dir/build.make src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o.provides.build
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o.provides

src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o.provides.build: src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o

src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o: src/bin/jp2/CMakeFiles/opj_decompress.dir/flags.make
src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o: src/bin/jp2/convert.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chris/openjpeg-2.0.0/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/opj_decompress.dir/convert.c.o   -c /home/chris/openjpeg-2.0.0/src/bin/jp2/convert.c

src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/opj_decompress.dir/convert.c.i"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/chris/openjpeg-2.0.0/src/bin/jp2/convert.c > CMakeFiles/opj_decompress.dir/convert.c.i

src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/opj_decompress.dir/convert.c.s"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/chris/openjpeg-2.0.0/src/bin/jp2/convert.c -o CMakeFiles/opj_decompress.dir/convert.c.s

src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o.requires:
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o.requires

src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o.provides: src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o.requires
	$(MAKE) -f src/bin/jp2/CMakeFiles/opj_decompress.dir/build.make src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o.provides.build
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o.provides

src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o.provides.build: src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o

src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o: src/bin/jp2/CMakeFiles/opj_decompress.dir/flags.make
src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o: src/bin/jp2/index.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chris/openjpeg-2.0.0/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/opj_decompress.dir/index.c.o   -c /home/chris/openjpeg-2.0.0/src/bin/jp2/index.c

src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/opj_decompress.dir/index.c.i"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/chris/openjpeg-2.0.0/src/bin/jp2/index.c > CMakeFiles/opj_decompress.dir/index.c.i

src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/opj_decompress.dir/index.c.s"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/chris/openjpeg-2.0.0/src/bin/jp2/index.c -o CMakeFiles/opj_decompress.dir/index.c.s

src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o.requires:
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o.requires

src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o.provides: src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o.requires
	$(MAKE) -f src/bin/jp2/CMakeFiles/opj_decompress.dir/build.make src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o.provides.build
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o.provides

src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o.provides.build: src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o

src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o: src/bin/jp2/CMakeFiles/opj_decompress.dir/flags.make
src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o: src/bin/common/color.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chris/openjpeg-2.0.0/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/opj_decompress.dir/__/common/color.c.o   -c /home/chris/openjpeg-2.0.0/src/bin/common/color.c

src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/opj_decompress.dir/__/common/color.c.i"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/chris/openjpeg-2.0.0/src/bin/common/color.c > CMakeFiles/opj_decompress.dir/__/common/color.c.i

src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/opj_decompress.dir/__/common/color.c.s"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/chris/openjpeg-2.0.0/src/bin/common/color.c -o CMakeFiles/opj_decompress.dir/__/common/color.c.s

src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o.requires:
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o.requires

src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o.provides: src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o.requires
	$(MAKE) -f src/bin/jp2/CMakeFiles/opj_decompress.dir/build.make src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o.provides.build
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o.provides

src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o.provides.build: src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o

src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o: src/bin/jp2/CMakeFiles/opj_decompress.dir/flags.make
src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o: src/bin/common/opj_getopt.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chris/openjpeg-2.0.0/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o   -c /home/chris/openjpeg-2.0.0/src/bin/common/opj_getopt.c

src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.i"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/chris/openjpeg-2.0.0/src/bin/common/opj_getopt.c > CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.i

src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.s"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/chris/openjpeg-2.0.0/src/bin/common/opj_getopt.c -o CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.s

src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o.requires:
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o.requires

src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o.provides: src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o.requires
	$(MAKE) -f src/bin/jp2/CMakeFiles/opj_decompress.dir/build.make src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o.provides.build
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o.provides

src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o.provides.build: src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o

# Object files for target opj_decompress
opj_decompress_OBJECTS = \
"CMakeFiles/opj_decompress.dir/opj_decompress.c.o" \
"CMakeFiles/opj_decompress.dir/convert.c.o" \
"CMakeFiles/opj_decompress.dir/index.c.o" \
"CMakeFiles/opj_decompress.dir/__/common/color.c.o" \
"CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o"

# External object files for target opj_decompress
opj_decompress_EXTERNAL_OBJECTS =

bin/opj_decompress: src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o
bin/opj_decompress: src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o
bin/opj_decompress: src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o
bin/opj_decompress: src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o
bin/opj_decompress: src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o
bin/opj_decompress: src/bin/jp2/CMakeFiles/opj_decompress.dir/build.make
bin/opj_decompress: bin/libopenjp2.2.0.0.dylib
bin/opj_decompress: /usr/local/lib/libpng.dylib
bin/opj_decompress: /usr/local/lib/libz.dylib
bin/opj_decompress: /usr/lib/libtiff.dylib
bin/opj_decompress: /usr/local/lib/libz.dylib
bin/opj_decompress: /usr/lib/libtiff.dylib
bin/opj_decompress: src/bin/jp2/CMakeFiles/opj_decompress.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ../../../bin/opj_decompress"
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/opj_decompress.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/bin/jp2/CMakeFiles/opj_decompress.dir/build: bin/opj_decompress
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/build

src/bin/jp2/CMakeFiles/opj_decompress.dir/requires: src/bin/jp2/CMakeFiles/opj_decompress.dir/opj_decompress.c.o.requires
src/bin/jp2/CMakeFiles/opj_decompress.dir/requires: src/bin/jp2/CMakeFiles/opj_decompress.dir/convert.c.o.requires
src/bin/jp2/CMakeFiles/opj_decompress.dir/requires: src/bin/jp2/CMakeFiles/opj_decompress.dir/index.c.o.requires
src/bin/jp2/CMakeFiles/opj_decompress.dir/requires: src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/color.c.o.requires
src/bin/jp2/CMakeFiles/opj_decompress.dir/requires: src/bin/jp2/CMakeFiles/opj_decompress.dir/__/common/opj_getopt.c.o.requires
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/requires

src/bin/jp2/CMakeFiles/opj_decompress.dir/clean:
	cd /home/chris/openjpeg-2.0.0/src/bin/jp2 && $(CMAKE_COMMAND) -P CMakeFiles/opj_decompress.dir/cmake_clean.cmake
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/clean

src/bin/jp2/CMakeFiles/opj_decompress.dir/depend:
	cd /home/chris/openjpeg-2.0.0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chris/openjpeg-2.0.0 /home/chris/openjpeg-2.0.0/src/bin/jp2 /home/chris/openjpeg-2.0.0 /home/chris/openjpeg-2.0.0/src/bin/jp2 /home/chris/openjpeg-2.0.0/src/bin/jp2/CMakeFiles/opj_decompress.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/bin/jp2/CMakeFiles/opj_decompress.dir/depend

