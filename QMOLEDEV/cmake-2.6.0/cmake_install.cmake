# Install script for directory: /home/chris/cmake-2.6.0

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/chris/cmake-2.6.0/Source/kwsys/cmake_install.cmake")
  INCLUDE("/home/chris/cmake-2.6.0/Utilities/cmzlib/cmake_install.cmake")
  INCLUDE("/home/chris/cmake-2.6.0/Utilities/cmcurl/cmake_install.cmake")
  INCLUDE("/home/chris/cmake-2.6.0/Utilities/cmtar/cmake_install.cmake")
  INCLUDE("/home/chris/cmake-2.6.0/Utilities/cmcompress/cmake_install.cmake")
  INCLUDE("/home/chris/cmake-2.6.0/Utilities/cmexpat/cmake_install.cmake")
  INCLUDE("/home/chris/cmake-2.6.0/Utilities/cmxmlrpc/cmake_install.cmake")
  INCLUDE("/home/chris/cmake-2.6.0/Source/CursesDialog/form/cmake_install.cmake")
  INCLUDE("/home/chris/cmake-2.6.0/Source/cmake_install.cmake")
  INCLUDE("/home/chris/cmake-2.6.0/Modules/cmake_install.cmake")
  INCLUDE("/home/chris/cmake-2.6.0/Templates/cmake_install.cmake")
  INCLUDE("/home/chris/cmake-2.6.0/Utilities/cmake_install.cmake")
  INCLUDE("/home/chris/cmake-2.6.0/Tests/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/chris/cmake-2.6.0/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/chris/cmake-2.6.0/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
