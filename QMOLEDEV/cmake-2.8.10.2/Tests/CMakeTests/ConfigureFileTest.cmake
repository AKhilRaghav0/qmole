set(DirInput-RESULT 1)
set(DirInput-STDERR "is a directory")
set(DirOutput-RESULT 0)
set(DirOutput-STDERR "DirOutput test file")
set(Relative-RESULT 0)
set(Relative-STDERR "Relative test file")
set(BadArg-RESULT 1)
set(BadArg-STDERR "called with incorrect number of arguments")
set(NewLineStyle-NoArg-RESULT 1)
set(NewLineStyle-NoArg-STDERR "NEWLINE_STYLE must set a style:")
set(NewLineStyle-WrongArg-RESULT 1)
set(NewLineStyle-WrongArg-STDERR "NEWLINE_STYLE sets an unknown style")
set(NewLineStyle-ValidArg-RESULT 0)
set(NewLineStyle-ValidArg-STDERR )
set(NewLineStyle-COPYONLY-RESULT 1)
set(NewLineStyle-COPYONLY-STDERR "COPYONLY could not be used in combination")

include("/home/chris/cmake-2.8.10.2/Tests/CMakeTests/CheckCMakeTest.cmake")
check_cmake_test(ConfigureFile
  DirInput
  DirOutput
  Relative
  BadArg
  NewLineStyle-NoArg
  NewLineStyle-WrongArg
  NewLineStyle-ValidArg
  NewLineStyle-COPYONLY
  )
