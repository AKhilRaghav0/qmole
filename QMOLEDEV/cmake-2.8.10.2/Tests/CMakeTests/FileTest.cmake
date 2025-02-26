set(Copy-BadArg-RESULT 1)
set(Copy-BadArg-STDERR "unknown argument \"BADARG\"")
set(Copy-BadPerm-RESULT 1)
set(Copy-BadPerm-STDERR "COPY given invalid permission \"BADPERM\"")
set(Copy-BadRegex-RESULT 1)
set(Copy-BadRegex-STDERR "could not compile REGEX")
set(Copy-EarlyArg-RESULT 1)
set(Copy-EarlyArg-STDERR "option PERMISSIONS may not appear before")
set(Copy-LateArg-RESULT 1)
set(Copy-LateArg-STDERR "option FILE_PERMISSIONS may not appear after")
set(Copy-NoDest-RESULT 1)
set(Copy-NoDest-STDERR "given no DESTINATION")
set(Copy-NoFile-RESULT 1)
set(Copy-NoFile-STDERR "COPY cannot find.*/does_not_exist\\.txt")
set(MD5-NoFile-RESULT 1)
set(MD5-NoFile-STDERR "file MD5 failed to read file")
set(MD5-BadArg1-RESULT 1)
set(MD5-BadArg1-STDERR "file must be called with at least two arguments")
set(MD5-BadArg2-RESULT 1)
set(MD5-BadArg2-STDERR "file MD5 requires a file name and output variable")
set(MD5-BadArg4-RESULT 1)
set(MD5-BadArg4-STDERR "file MD5 requires a file name and output variable")
set(MD5-Works-RESULT 0)
set(MD5-Works-STDERR "10d20ddb981a6202b84aa1ce1cb7fce3")
set(SHA1-Works-RESULT 0)
set(SHA1-Works-STDERR "83f093e04289b21a9415f408ad50be8b57ad2f34")
set(SHA224-Works-RESULT 0)
set(SHA224-Works-STDERR "e995a7789922c4ef9279d94e763c8375934180a51baa7147bc48edf7")
set(SHA256-Works-RESULT 0)
set(SHA256-Works-STDERR "d1c5915d8b71150726a1eef75a29ec6bea8fd1bef6b7299ef8048760b0402025")
set(SHA384-Works-RESULT 0)
set(SHA384-Works-STDERR "1de9560b4e030e02051ea408200ffc55d70c97ac64ebf822461a5c786f495c36df43259b14483bc8d364f0106f4971ee")
set(SHA512-Works-RESULT 0)
set(SHA512-Works-STDERR "3982a1b4e651768bec70ab1fb97045cb7a659f4ba7203d501c52ab2e803071f9d5fd272022df15f27727fc67f8cd022e710e29010b2a9c0b467c111e2f6abf51")

include("/home/chris/cmake-2.8.10.2/Tests/CMakeTests/CheckCMakeTest.cmake")
check_cmake_test(File
  Copy-BadArg
  Copy-BadPerm
  Copy-BadRegex
  Copy-EarlyArg
  Copy-LateArg
  Copy-NoDest
  Copy-NoFile
  MD5-NoFile
  MD5-BadArg1
  MD5-BadArg2
  MD5-BadArg4
  MD5-Works
  SHA1-Works
  SHA224-Works
  SHA256-Works
  SHA384-Works
  SHA512-Works
  )

# Also execute each test listed in FileTestScript.cmake:
#
set(scriptname "/home/chris/cmake-2.8.10.2/Tests/CMakeTests/FileTestScript.cmake")
set(number_of_tests_expected 62)

include("/home/chris/cmake-2.8.10.2/Tests/CMakeTests/ExecuteScriptTests.cmake")
execute_all_script_tests(${scriptname} number_of_tests_executed)

# And verify that number_of_tests_executed is at least as many as we know
# about as of this writing...
#
message(STATUS "scriptname='${scriptname}'")
message(STATUS "number_of_tests_executed='${number_of_tests_executed}'")
message(STATUS "number_of_tests_expected='${number_of_tests_expected}'")

if(number_of_tests_executed LESS number_of_tests_expected)
  message(FATAL_ERROR "error: some test cases were skipped")
endif()
