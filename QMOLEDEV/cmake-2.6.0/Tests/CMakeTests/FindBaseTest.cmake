set(MY_SOURCE_DIR "/home/chris/cmake-2.6.0/Tests/CMakeTests")

set(_HEADER cmake_i_do_not_exist_in_the_system.h)
set(_HEADER_FULL "${MY_SOURCE_DIR}/include/${_HEADER}")

# at first check that the header isn't found without special measures
find_file(FOO_H_1 ${_HEADER})
if(FOO_H_1)
   message(FATAL_ERROR "${_HEADER} found: ${FOO_H_1}, it should not exist !")
endif(FOO_H_1)

# with this it still should not be found, since the include/ subdir is still missing
set(CMAKE_INCLUDE_PATH "${MY_SOURCE_DIR}")
find_file(FOO_H_2 ${_HEADER})
if(FOO_H_2)
   message(FATAL_ERROR "${_HEADER} found: ${FOO_H_2}, it should not exist !")
endif(FOO_H_2)

# now with the PATH_SUFFIX it should be found
find_file(FOO_H_3 NAMES ${_HEADER} PATH_SUFFIXES include )
if(NOT "${FOO_H_3}" STREQUAL "${_HEADER_FULL}")
   message(FATAL_ERROR "Did not find \"${_HEADER_FULL}\"\ngot ${FOO_H_3} instead !")
endif(NOT "${FOO_H_3}" STREQUAL "${_HEADER_FULL}")

# without PATH_SUFFIX, but with a CMAKE_INCLUDE_PATH it should not be found
set(CMAKE_INCLUDE_PATH /include)
find_file(FOO_H_4 ${_HEADER})
if(FOO_H_4)
   message(FATAL_ERROR "${_HEADER} found: ${FOO_H_4}, it should not exist !")
endif(FOO_H_4)

# when setting CMAKE_FIND_ROOT_PATH to the current source dir,
# together with the CMAKE_INCLUDE_PATH it should be found
set(CMAKE_FIND_ROOT_PATH blub "${MY_SOURCE_DIR}")
find_file(FOO_H_5 ${_HEADER})
if(NOT "${FOO_H_5}" STREQUAL "${_HEADER_FULL}")
   message(FATAL_ERROR "Did not find \"${_HEADER_FULL}\"\ngot ${FOO_H_5} instead !")
endif(NOT "${FOO_H_5}" STREQUAL "${_HEADER_FULL}")

# by explicitely disabling CMAKE_FIND_ROOT_PATH again it shouldn't be found
find_file(FOO_H_6 ${_HEADER} NO_CMAKE_FIND_ROOT_PATH)
if(FOO_H_6)
   message(FATAL_ERROR "${_HEADER} found: ${FOO_H_6}, it should not exist !")
endif(FOO_H_6)
