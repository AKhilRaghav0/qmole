CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# This is a template for the CTest script for this system

SET(CTEST_SITE                          "Chriss-iPad")
SET(CTEST_BUILD_NAME                    "Darwin-g++")

# ---
SET(CTEST_SOURCE_DIRECTORY              "/home/chris/cmake-2.6.0")
SET(CTEST_BINARY_DIRECTORY              "/home/chris/cmake-2.6.0")
SET(CTEST_UPDATE_COMMAND                "")
SET(CTEST_UPDATE_OPTIONS                "")
SET(CTEST_CMAKE_GENERATOR               "Unix Makefiles")
SET(CTEST_BUILD_CONFIGURATION           "Release")
#SET(CTEST_MEMORYCHECK_COMMAND           "MEMORYCHECK_COMMAND-NOTFOUND")
#SET(CTEST_MEMORYCHECK_SUPPRESSIONS_FILE "")
#SET(CTEST_MEMORYCHECK_COMMAND_OPTIONS   "")
#SET(CTEST_COVERAGE_COMMAND              "/usr/bin/gcov")
SET(CTEST_NOTES_FILES                   "${CTEST_SCRIPT_DIRECTORY}/${CTEST_SCRIPT_NAME}")

#CTEST_EMPTY_BINARY_DIRECTORY(${CTEST_BINARY_DIRECTORY})

SET(CTEST_DROP_METHOD "http")

CTEST_START(Experimental TRACK Weekly)
CTEST_UPDATE(SOURCE "${CTEST_SOURCE_DIRECTORY}")
CTEST_CONFIGURE(BUILD "${CTEST_BINARY_DIRECTORY}")
CTEST_READ_CUSTOM_FILES("${CTEST_BINARY_DIRECTORY}")
CTEST_BUILD(BUILD "${CTEST_BINARY_DIRECTORY}")
CTEST_TEST(BUILD "${CTEST_BINARY_DIRECTORY}")
#CTEST_MEMCHECK(BUILD "${CTEST_BINARY_DIRECTORY}")
#CTEST_COVERAGE(BUILD "${CTEST_BINARY_DIRECTORY}")
CTEST_SUBMIT()
