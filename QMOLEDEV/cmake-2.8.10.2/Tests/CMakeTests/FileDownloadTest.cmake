set(url "file:///home/chris/cmake-2.8.10.2/Tests/CMakeTests/FileDownloadInput.png")
set(dir "/home/chris/cmake-2.8.10.2/Tests/CMakeTests/downloads")

message(STATUS "FileDownload:1")
file(DOWNLOAD
  ${url}
  ${dir}/file1.png
  TIMEOUT 2
  )

message(STATUS "FileDownload:2")
file(DOWNLOAD
  ${url}
  ${dir}/file2.png
  TIMEOUT 2
  SHOW_PROGRESS
  )

# Two calls in a row, exactly the same arguments.
# Since downloaded file should exist already for 2nd call,
# the 2nd call will short-circuit and return early...
#
if(EXISTS ${dir}/file3.png)
  file(REMOVE ${dir}/file3.png)
endif()

message(STATUS "FileDownload:3")
file(DOWNLOAD
  ${url}
  ${dir}/file3.png
  TIMEOUT 2
  EXPECTED_MD5 d16778650db435bda3a8c3435c3ff5d1
  )

message(STATUS "FileDownload:4")
file(DOWNLOAD
  ${url}
  ${dir}/file3.png
  TIMEOUT 2
  STATUS status
  EXPECTED_HASH SHA1=50c614fc28b39c1281d0517bb6d5858b4359c9b7
  )

message(STATUS "FileDownload:5")
file(DOWNLOAD
  ${url}
  ${dir}/file3.png
  TIMEOUT 2
  STATUS status
  EXPECTED_HASH SHA224=73cd5f442b04e8320e4f907f8e1b21d4befff98b5bd77bc32526ea68
  )

message(STATUS "FileDownload:6")
file(DOWNLOAD
  ${url}
  ${dir}/file3.png
  TIMEOUT 2
  STATUS status
  EXPECTED_HASH SHA256=2e067f6c09cbc7cd619c8fbcc44eb64cd6b45a95e4cddb3a585eee1f731c4da9
  )

message(STATUS "FileDownload:7")
file(DOWNLOAD
  ${url}
  ${dir}/file3.png
  TIMEOUT 2
  STATUS status
  EXPECTED_HASH SHA384=398bf41902a7251c30e522b307e3e41e3fb617c765b3feaa99b2f7d063894708ad399267ccc25d877437a10e5e890d35
  )

message(STATUS "FileDownload:8")
file(DOWNLOAD
  ${url}
  ${dir}/file3.png
  TIMEOUT 2
  STATUS status
  EXPECTED_HASH SHA512=c51854d21052713968b849c2b4263cf54be03bc3a7e9847a6c71c6c8d1d13cd805fe1b9fa95f9ba1d0a5631513974f6fae21e34ab5b171d94bad48df5f073e48
  )
message(STATUS "FileDownload:9")
file(DOWNLOAD
  ${url}
  ${dir}/file3.png
  TIMEOUT 2
  STATUS status
  EXPECTED_HASH MD5=d16778650db435bda3a8c3435c3ff5d1
  )

message(STATUS "FileDownload:10")
file(DOWNLOAD
  ${url}
  ${dir}/file3.png
  TIMEOUT 2
  STATUS status
  EXPECTED_MD5 d16778650db435bda3a8c3435c3ff5d1
  )
message(STATUS "${status}")
