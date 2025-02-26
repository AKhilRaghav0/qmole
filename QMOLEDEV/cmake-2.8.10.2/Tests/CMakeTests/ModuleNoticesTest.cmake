# Regex to match copyright/license notices.
# We require the Kitware copyright on the first line, but this can
# match any additional copyright holder notices.
set(notice_regex "
#=============================================================================
# Copyright (20[0-9][0-9]-)?20[0-9][0-9] [^\n]+(
# Copyright (20[0-9][0-9]-)?20[0-9][0-9] [^\n]+)*
#
# Distributed under the OSI-approved BSD License \\(the \"License\"\\);
# see accompanying file Copyright\\.txt for details\\.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\\.
# See the License for more information\\.
#=============================================================================
# \\(To distribute this file outside of CMake, substitute the full
#  License text for the above reference.\\)
")
string(REPLACE "\n" "\r?\n" notice_regex "${notice_regex}")
string(REPLACE "\r\r" "\r" notice_regex "${notice_regex}")

# Modules that do not require our notice.
set(notice_exceptions
  FindCUDA.cmake # MIT License, distributed here from upstream project
  )

# Load the list of modules to check.
set(dir "/home/chris/cmake-2.8.10.2/Modules")
file(GLOB all_modules RELATIVE "${dir}" "${dir}/*.cmake")
list(REMOVE_ITEM all_modules ${notice_exceptions})

# Check each module.
set(notice_missing)
foreach(module ${all_modules})
  message(STATUS "module: ${module}")
  file(READ "${dir}/${module}" module_content)
  if(NOT "${module_content}" MATCHES "${notice_regex}")
    set(notice_missing "${notice_missing}  ${module}\n")
  endif()
endforeach()

# Report the list of bad modules.
if(notice_missing)
  message(FATAL_ERROR
    "Some modules do not have a valid copyright notice:\n${notice_missing}")
endif()
