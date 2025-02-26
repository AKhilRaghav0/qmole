/*=========================================================================

  Program:   KWSys - Kitware System Library
  Module:    $RCSfile: SystemTools.hxx.in,v $

  Copyright (c) Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef cmsys_SystemTools_hxx
#define cmsys_SystemTools_hxx

#include <cmsys/ios/iosfwd>
#include <cmsys/stl/string>
#include <cmsys/stl/vector>
#include <cmsys/stl/map>

#include <cmsys/Configure.h>
#include <cmsys/String.hxx>

#include <sys/types.h>

// Required for va_list
#include <stdarg.h>
#if cmsys_STL_HAVE_STD && !defined(va_list)
// Some compilers move va_list into the std namespace and there is no way to
// tell that this has been done. Playing with things being included before or
// after stdarg.h does not solve things because we do not have control over
// what the user does. This hack solves this problem by moving va_list to our
// own namespace that is local for kwsys.
namespace std {} // Required for platforms that do not have std namespace
namespace cmsys_VA_LIST
{
  using namespace std;
  typedef va_list hack_va_list;
}
namespace cmsys
{
  typedef cmsys_VA_LIST::hack_va_list va_list;
}
#endif // va_list

#if defined( _MSC_VER )
typedef unsigned short mode_t;
#endif

/* Define these macros temporarily to keep the code readable.  */
#if !defined (KWSYS_NAMESPACE) && !cmsys_NAME_IS_KWSYS
# define kwsys_stl cmsys_stl
# define kwsys_ios cmsys_ios
#endif

namespace cmsys
{

class SystemToolsTranslationMap;
/** \class SystemToolsManager
 * \brief Use to make sure SystemTools is initialized before it is used
 * and is the last static object destroyed
 */
class cmsys_EXPORT SystemToolsManager
{
public:
  SystemToolsManager();
  ~SystemToolsManager();
};

// This instance will show up in any translation unit that uses
// SystemTools. It will make sure SystemTools is initialized 
// before it is used and is the last static object destroyed.
static SystemToolsManager SystemToolsManagerInstance;

/** \class SystemTools
 * \brief A collection of useful platform-independent system functions.
 */
class cmsys_EXPORT SystemTools
{
public:

  /** -----------------------------------------------------------------
   *               String Manipulation Routines
   *  -----------------------------------------------------------------
   */

  /**
   * Replace symbols in str that are not valid in C identifiers as
   * defined by the 1999 standard, ie. anything except [A-Za-z0-9_].
   * They are replaced with `_' and if the first character is a digit
   * then an underscore is prepended.  Note that this can produce
   * identifiers that the standard reserves (_[A-Z].* and __.*).
   */
  static kwsys_stl::string MakeCindentifier(const char* s);
  
  /**
   * Replace replace all occurences of the string in the source string.
   */
  static void ReplaceString(kwsys_stl::string& source,
                            const char* replace,
                            const char* with);

  /**
   * Return a capitalized string (i.e the first letter is uppercased,
   * all other are lowercased).
   */
  static kwsys_stl::string Capitalized(const kwsys_stl::string&);
  
  /**
   * Return a 'capitalized words' string (i.e the first letter of each word
   * is uppercased all other are left untouched though).
   */
  static kwsys_stl::string CapitalizedWords(const kwsys_stl::string&);
  
  /**
   * Return a 'uncapitalized words' string (i.e the first letter of each word
   * is lowercased all other are left untouched though).
   */
  static kwsys_stl::string UnCapitalizedWords(const kwsys_stl::string&);
  
  /**
   * Return a lower case string
   */
  static kwsys_stl::string LowerCase(const kwsys_stl::string&);
  
  /**
   * Return a lower case string
   */
  static kwsys_stl::string UpperCase(const kwsys_stl::string&);
  
  /**
   * Count char in string
   */
  static size_t CountChar(const char* str, char c);
  
  /**
   * Remove some characters from a string.
   * Return a pointer to the new resulting string (allocated with 'new')
   */
  static char* RemoveChars(const char* str, const char *toremove);

  /**
   * Remove remove all but 0->9, A->F characters from a string.
   * Return a pointer to the new resulting string (allocated with 'new')
   */
  static char* RemoveCharsButUpperHex(const char* str);
  
  /**
   * Replace some characters by another character in a string (in-place)
   * Return a pointer to string
   */
  static char* ReplaceChars(char* str, const char *toreplace,char replacement);
  
  /**
   * Returns true if str1 starts (respectively ends) with str2
   */
  static bool StringStartsWith(const char* str1, const char* str2);
  static bool StringEndsWith(const char* str1, const char* str2);

  /**
   * Returns a pointer to the last occurence of str2 in str1
   */
  static const char* FindLastString(const char* str1, const char* str2);
  
  /**
   * Make a duplicate of the string similar to the strdup C function
   * but use new to create the 'new' string, so one can use
   * 'delete' to remove it. Returns 0 if the input is empty.
   */
  static char* DuplicateString(const char* str);
  
  /**
   * Return the string cropped to a given length by removing chars in the
   * center of the string and replacing them with an ellipsis (...)
   */
  static kwsys_stl::string CropString(const kwsys_stl::string&,size_t max_len);
  
  /** split a path by separator into an array of strings, default is /.
      If isPath is true then the string is treated like a path and if
      s starts with a / then the first element of the returned array will
      be /, so /foo/bar will be [/, foo, bar]
  */  
  static kwsys_stl::vector<String> SplitString(const char* s, char separator = '/', 
                                               bool isPath = false);
  /**
   * Perform a case-independent string comparison
   */
  static int Strucmp(const char *s1, const char *s2);

  /** 
   * Convert a string in __DATE__ or __TIMESTAMP__ format into a time_t.
   * Return false on error, true on success
   */
  static bool ConvertDateMacroString(const char *str, time_t *tmt);
  static bool ConvertTimeStampMacroString(const char *str, time_t *tmt);

  /**
   * Split a string on its newlines into multiple lines
   * Return false only if the last line stored had no newline
   */
  static bool Split(const char* s, kwsys_stl::vector<kwsys_stl::string>& l);
  static bool Split(const char* s, kwsys_stl::vector<kwsys_stl::string>& l, char separator);
  
  /** 
   * Return string with space added between capitalized words
   * (i.e. EatMyShorts becomes Eat My Shorts )
   * (note that IEatShorts becomes IEat Shorts) 
   */
  static kwsys_stl::string AddSpaceBetweenCapitalizedWords(
    const kwsys_stl::string&);

  /**
   * Append two or more strings and produce new one.
   * Programmer must 'delete []' the resulting string, which was allocated
   * with 'new'.
   * Return 0 if inputs are empty or there was an error
   */
  static char* AppendStrings(
    const char* str1, const char* str2);
  static char* AppendStrings(
    const char* str1, const char* str2, const char* str3);

  /**
   * Estimate the length of the string that will be produced
   * from printing the given format string and arguments.  The
   * returned length will always be at least as large as the string
   * that will result from printing.
   * WARNING: since va_arg is called to iterate of the argument list,
   * you will not be able to use this 'ap' anymore from the beginning.
   * It's up to you to call va_end though.
   */
  static int EstimateFormatLength(const char *format, va_list ap);

  /**
   * Escape specific characters in 'str'.
   */
  static kwsys_stl::string EscapeChars(
    const char *str, const char *chars_to_escape, char escape_char = '\\');

  /** -----------------------------------------------------------------
   *               Filename Manipulation Routines
   *  -----------------------------------------------------------------
   */

  /**
   * Replace Windows file system slashes with Unix-style slashes.
   */
  static void ConvertToUnixSlashes(kwsys_stl::string& path);
  
  /**
   * For windows this calls ConvertToWindowsOutputPath and for unix
   * it calls ConvertToUnixOutputPath
   */
  static kwsys_stl::string ConvertToOutputPath(const char*);

  /**
   * Convert the path to a string that can be used in a unix makefile.
   * double slashes are removed, and spaces are escaped.
   */
  static kwsys_stl::string ConvertToUnixOutputPath(const char*);

  /**
   * Convert the path to string that can be used in a windows project or
   * makefile.   Double slashes are removed if they are not at the start of
   * the string, the slashes are converted to windows style backslashes, and
   * if there are spaces in the string it is double quoted.
   */
  static kwsys_stl::string ConvertToWindowsOutputPath(const char*);

  /**
   * Return true if a file exists in the current directory.
   * If isFile = true, then make sure the file is a file and 
   * not a directory.  If isFile = false, then return true
   * if it is a file or a directory.
   */
  static bool FileExists(const char* filename, bool isFile=false);

  /**
   * Return file length
   */
  static unsigned long FileLength(const char *filename);

  /**
     Change the modification time or create a file
  */
  static bool Touch(const char* filename, bool create);
  
  /**
   *  Compare file modification times.
   *  Return true for successful comparison and false for error.
   *  When true is returned, result has -1, 0, +1 for
   *  f1 older, same, or newer than f2.
   */
  static bool FileTimeCompare(const char* f1, const char* f2,
                              int* result);

  /**
   *  Get the file extension (including ".") needed for an executable
   *  on the current platform ("" for unix, ".exe" for Windows).
   */
  static const char* GetExecutableExtension();

  /**
   *  Given a path that exists on a windows machine, return the
   *  actuall case of the path as it was created.  If the file
   *  does not exist path is returned unchanged.  This does nothing
   *  on unix but return path.
   */
  static kwsys_stl::string GetActualCaseForPath(const char* path);

  /**
   * Given the path to a program executable, get the directory part of
   * the path with the file stripped off.  If there is no directory
   * part, the empty string is returned.
   */
  static kwsys_stl::string GetProgramPath(const char*);
  static bool SplitProgramPath(const char* in_name,
                               kwsys_stl::string& dir,
                               kwsys_stl::string& file,
                               bool errorReport = true);

  /**
   *  Given argv[0] for a unix program find the full path to a running
   *  executable.  argv0 can be null for windows WinMain programs
   *  in this case GetModuleFileName will be used to find the path
   *  to the running executable.  If argv0 is not a full path,
   *  then this will try to find the full path.  If the path is not
   *  found false is returned, if found true is returned.  An error
   *  message of the attempted paths is stored in errorMsg.  
   *  exeName is the name of the executable.
   *  buildDir is a possibly null path to the build directory.
   *  installPrefix is a possibly null pointer to the install directory.
   */
  static bool FindProgramPath(const char* argv0,
                              kwsys_stl::string& pathOut,
                              kwsys_stl::string& errorMsg,
                              const char* exeName = 0,
                              const char* buildDir = 0,
                              const char* installPrefix = 0);

  /**
   * Given a path to a file or directory, convert it to a full path.
   * This collapses away relative paths relative to the cwd argument
   * (which defaults to the current working directory).  The full path
   * is returned.
   */
  static kwsys_stl::string CollapseFullPath(const char* in_relative);
  static kwsys_stl::string CollapseFullPath(const char* in_relative,
                                            const char* in_base);

  /** 
   * Get the real path for a given path, removing all symlinks.
   */
  static kwsys_stl::string GetRealPath(const char* path);

  /**
   * Split a path name into its root component and the rest of the
   * path.  The root component is one of the following:
   *    "/"   = UNIX full path
   *    "c:/" = Windows full path (can be any drive letter)
   *    "c:"  = Windows drive-letter relative path (can be any drive letter)
   *    "//"  = Network path
   *    "~"   = Home path for current user
   *    "~u"  = Home path for user 'u'
   *    ""    = Relative path
   *
   * A pointer to the rest of the path after the root component is
   * returned.  The root component is stored in the "root" string if
   * given.
   */
  static const char* SplitPathRootComponent(const char* p,
                                            kwsys_stl::string* root=0);

  /**
   * Split a path name into its basic components.  The first component
   * is one of the roots returned by SplitPathRootComponent.
   * The remaining components form the path.  If there is a trailing
   * slash then the last component is the empty string.  The
   * components can be recombined as "c[0]c[1]/c[2]/.../c[n]" to
   * produce the original path.  Home directory references are
   * automatically expanded if expand_home_dir is true and this
   * platform supports them.
   */
  static void SplitPath(const char* p,
                        kwsys_stl::vector<kwsys_stl::string>& components,
                        bool expand_home_dir = true);

  /**
   * Join components of a path name into a single string.  See
   * SplitPath for the format of the components.
   */
  static kwsys_stl::string JoinPath(
    const kwsys_stl::vector<kwsys_stl::string>& components);
  static kwsys_stl::string JoinPath(
    kwsys_stl::vector<kwsys_stl::string>::const_iterator first,
    kwsys_stl::vector<kwsys_stl::string>::const_iterator last);

  /**
   * Compare a path or components of a path.
   */
  static bool ComparePath(const char* c1, const char* c2);


  /**
   * Return path of a full filename (no trailing slashes)
   */
  static kwsys_stl::string GetFilenamePath(const kwsys_stl::string&);

  /**
   * Return file name of a full filename (i.e. file name without path)
   */
  static kwsys_stl::string GetFilenameName(const kwsys_stl::string&);

  /**
   * Split a program from its arguments and handle spaces in the paths
   */
  static void SplitProgramFromArgs(
    const char* path,
    kwsys_stl::string& program, kwsys_stl::string& args);

  /**
   * Return longest file extension of a full filename (dot included)
   */
  static kwsys_stl::string GetFilenameExtension(const kwsys_stl::string&);

  /**
   * Return shortest file extension of a full filename (dot included)
   */
  static kwsys_stl::string GetFilenameLastExtension(
    const kwsys_stl::string& filename);
  
  /**
   * Return file name without extension of a full filename
   */
  static kwsys_stl::string GetFilenameWithoutExtension(
    const kwsys_stl::string&);
  
  /**
   * Return file name without its last (shortest) extension
   */
  static kwsys_stl::string GetFilenameWithoutLastExtension(
    const kwsys_stl::string&);
  
  /**
   * Return whether the path represents a full path (not relative)
   */
  static bool FileIsFullPath(const char*);
  
  /**
   * For windows return the short path for the given path,
   * Unix just a pass through
   */
  static bool GetShortPath(const char* path, kwsys_stl::string& result);
  
  /**
   * Read line from file. Make sure to get everything. Due to a buggy stream
   * library on the HP and another on Mac OS X, we need this very carefully
   * written version of getline. Returns true if any data were read before the
   * end-of-file was reached. If the has_newline argument is specified, it will
   * be true when the line read had a newline character.
   */
  static bool GetLineFromStream(kwsys_ios::istream& istr, 
                                kwsys_stl::string& line,
                                bool* has_newline=0,
                                long sizeLimit=-1);

  /**
   * Get the parent directory of the directory or file
   */
  static kwsys_stl::string GetParentDirectory(const char* fileOrDir);

  /**
   * Check if the given file or directory is in subdirectory of dir
   */
  static bool IsSubDirectory(const char* fileOrDir, const char* dir);

  /** -----------------------------------------------------------------
   *               File Manipulation Routines
   *  -----------------------------------------------------------------
   */

  /**
   * Make a new directory if it is not there.  This function
   * can make a full path even if none of the directories existed
   * prior to calling this function.  
   */
  static bool MakeDirectory(const char* path);

  /**
   * Copy the source file to the destination file only
   * if the two files differ.  
   */
  static bool CopyFileIfDifferent(const char* source,
                                  const char* destination);
  
  /**
   * Compare the contents of two files.  Return true if different
   */
  static bool FilesDiffer(const char* source, const char* destination);
  
  /**
   * Return true if the two files are the same file
   */
  static bool SameFile(const char* file1, const char* file2);

  /**
   * Copy a file
   */
  static bool CopyFileAlways(const char* source, const char* destination);

  /**
   * Copy a file.  If the "always" argument is true the file is always
   * copied.  If it is false, the file is copied only if it is new or
   * has changed.
   */
  static bool CopyAFile(const char* source, const char* destination,
                        bool always = true);

  /**
   * Copy content directory to another directory with all files and
   * subdirectories.  If the "always" argument is true all files are
   * always copied.  If it is false, only files that have changed or
   * are new are copied.
   */
  static bool CopyADirectory(const char* source, const char* destination,
                             bool always = true);
  
  /**
   * Remove a file
   */
  static bool RemoveFile(const char* source);
  
  /**
   * Remove a directory
   */
  static bool RemoveADirectory(const char* source);

  /**
   * Get the maximum full file path length
   */
  static size_t GetMaximumFilePathLength();

  /**
   * Find a file in the system PATH, with optional extra paths
   */
  static kwsys_stl::string FindFile(
    const char* name,
    const kwsys_stl::vector<kwsys_stl::string>& path = 
    kwsys_stl::vector<kwsys_stl::string>(),
    bool no_system_path = false);

  /**
   * Find a directory in the system PATH, with optional extra paths
   */
  static kwsys_stl::string FindDirectory(
    const char* name,
    const kwsys_stl::vector<kwsys_stl::string>& path = 
    kwsys_stl::vector<kwsys_stl::string>(),
    bool no_system_path = false);

  /**
   * Find an executable in the system PATH, with optional extra paths
   */
  static kwsys_stl::string FindProgram(
    const char* name,
    const kwsys_stl::vector<kwsys_stl::string>& path = 
    kwsys_stl::vector<kwsys_stl::string>(),
    bool no_system_path = false);
  static kwsys_stl::string FindProgram(
    const kwsys_stl::vector<kwsys_stl::string>& names,
    const kwsys_stl::vector<kwsys_stl::string>& path = 
    kwsys_stl::vector<kwsys_stl::string>(),
    bool no_system_path = false);

  /**
   * Find a library in the system PATH, with optional extra paths
   */
  static kwsys_stl::string FindLibrary(
    const char* name,
    const kwsys_stl::vector<kwsys_stl::string>& path);
  
  /**
   * Return true if the file is a directory
   */
  static bool FileIsDirectory(const char* name);
  
  /**
   * Return true if the file is a symlink
   */
  static bool FileIsSymlink(const char* name);
  
  /**
   * Return true if the file has a given signature (first set of bytes)
   */
  static bool FileHasSignature(
    const char* filename, const char *signature, long offset = 0);

  /**
   * Attempt to detect and return the type of a file.
   * Up to 'length' bytes are read from the file, if more than 'percent_bin' %
   * of the bytes are non-textual elements, the file is considered binary,
   * otherwise textual. Textual elements are bytes in the ASCII [0x20, 0x7E]
   * range, but also \n, \r, \t.
   * The algorithm is simplistic, and should probably check for usual file
   * extensions, 'magic' signature, unicode, etc.
   */
  enum FileTypeEnum 
  { 
    FileTypeUnknown,
    FileTypeBinary,
    FileTypeText
  };
  static SystemTools::FileTypeEnum DetectFileType(
    const char* filename, 
    unsigned long length = 256, 
    double percent_bin = 0.05);

  /**
   * Create a symbolic link if the platform supports it.  Returns whether
   * creation succeded.
   */
  static bool CreateSymlink(const char* origName, const char* newName);

  /**
   * Read the contents of a symbolic link.  Returns whether reading
   * succeded.
   */
  static bool ReadSymlink(const char* newName, kwsys_stl::string& origName);

  /**
   * Try to locate the file 'filename' in the directory 'dir'.
   * If 'filename' is a fully qualified filename, the basename of the file is
   * used to check for its existence in 'dir'.
   * If 'dir' is not a directory, GetFilenamePath() is called on 'dir' to
   * get its directory first (thus, you can pass a filename as 'dir', as
   * a convenience).
   * 'filename_found' is assigned the fully qualified name/path of the file
   * if it is found (not touched otherwise).
   * If 'try_filename_dirs' is true, try to find the file using the
   * components of its path, i.e. if we are looking for c:/foo/bar/bill.txt, 
   * first look for bill.txt in 'dir', then in 'dir'/bar, then in 'dir'/foo/bar
   * etc.
   * Return true if the file was found, false otherwise.
   */
  static bool LocateFileInDir(const char *filename, 
                              const char *dir, 
                              kwsys_stl::string& filename_found,
                              int try_filename_dirs = 0);
  
  /** 
   * Check if the given file exists in one of the parent directory of the
   * given file or directory and if it does, return the name of the file.
   * Toplevel specifies the top-most directory to where it will look.
   */
  static kwsys_stl::string FileExistsInParentDirectories(const char* fname,
    const char* directory, const char* toplevel);

  /** compute the relative path from local to remote.  local must 
      be a directory.  remote can be a file or a directory.  
      Both remote and local must be full paths.  Basically, if
      you are in directory local and you want to access the file in remote
      what is the relative path to do that.  For example:
      /a/b/c/d to /a/b/c1/d1 -> ../../c1/d1
      from /usr/src to /usr/src/test/blah/foo.cpp -> test/blah/foo.cpp
  */
  static kwsys_stl::string RelativePath(const char* local, const char* remote);

  /**
   * Return file's modified time
   */
  static long int ModifiedTime(const char* filename);

  /**
   * Return file's creation time (Win32: works only for NTFS, not FAT)
   */
  static long int CreationTime(const char* filename);

  /**
   * Get and set permissions of the file.
   */
  static bool GetPermissions(const char* file, mode_t& mode);
  static bool SetPermissions(const char* file, mode_t mode);

  /** -----------------------------------------------------------------
   *               Time Manipulation Routines
   *  -----------------------------------------------------------------
   */

  /**
   * Get current time as a double. On certain platforms this will
   * return higher resolution than seconds:
   * (1) gettimeofday() -- resolution in microseconds
   * (2) ftime() -- resolution in milliseconds
   * (3) time() -- resolution in seconds
   */
  static double GetTime();

  /**
   * Get current date/time
   */
  static kwsys_stl::string GetCurrentDateTime(const char* format);

  /** -----------------------------------------------------------------
   *               Registry Manipulation Routines
   *  -----------------------------------------------------------------
   */

  /**
   * Read a registry value
   */
  static bool ReadRegistryValue(const char *key, kwsys_stl::string &value);

  /**
   * Write a registry value
   */
  static bool WriteRegistryValue(const char *key, const char *value);

  /**
   * Delete a registry value
   */
  static bool DeleteRegistryValue(const char *key);

  /** -----------------------------------------------------------------
   *               Environment Manipulation Routines
   *  -----------------------------------------------------------------
   */

  /**
   *  Add the paths from the environment variable PATH to the
   *  string vector passed in.  If env is set then the value
   *  of env will be used instead of PATH.
   */
  static void GetPath(kwsys_stl::vector<kwsys_stl::string>& path,
                      const char* env=0);

  /**
   * Read an environment variable
   */
  static const char* GetEnv(const char* key);
  static bool GetEnv(const char* key, kwsys_stl::string& result);

  /**
   * Get current working directory CWD
   */
  static kwsys_stl::string GetCurrentWorkingDirectory(bool collapse =true);

  /**
   * Change directory the the directory specified
   */
  static int ChangeDirectory(const char* dir);

  /**
   * Get the result of strerror(errno)
   */
  static kwsys_stl::string GetLastSystemError();

  /**
   * When building DEBUG with MSVC, this enables a hook that prevents
   * error dialogs from popping up if the program is being run from
   * DART.
   */
  static void EnableMSVCDebugHook();

  /**
   * Get the width of the terminal window. The code may or may not work, so
   * make sure you have some resonable defaults prepared if the code returns
   * some bogus size.
   */
  static int GetTerminalWidth();

  /**
   * Add an entry in the path translation table.
   */
  static void AddTranslationPath(const char * dir, const char * refdir);

  /**
   * If dir is different after CollapseFullPath is called,
   * Then insert it into the path translation table
   */
  static void AddKeepPath(const char* dir);

  /**
   * Update path by going through the Path Translation table;
   */
  static void CheckTranslationPath(kwsys_stl::string & path);

  /**
   * Delay the execution for a specified amount of time specified
   * in miliseconds
   */
  static void Delay(unsigned int msec);

  /**
   * Get the operating system name and version
   * This is implemented for Win32 only for the moment
   */
  static kwsys_stl::string GetOperatingSystemNameAndVersion();

  /**
   * Convert windows-style arguments given as a command-line string
   * into more traditional argc/argv arguments.
   * Note that argv[0] will be assigned the executable name using
   * the ::GetModuleFileName function.
   */
  static void ConvertWindowsCommandLineToUnixArguments(
    const char *cmd_line, int *argc, char ***argv);

  /** -----------------------------------------------------------------
   *               URL Manipulation Routines
   *  -----------------------------------------------------------------
   */

  /**
   * Parse a character string :
   *       protocol://dataglom
   * and fill protocol as appropriate.
   * Return false if the URL does not have the required form, true otherwise.
   */
   static bool ParseURLProtocol( const kwsys_stl::string& URL,
                                 kwsys_stl::string& protocol,
                                 kwsys_stl::string& dataglom );

  /**
   * Parse a string (a URL without protocol prefix) with the form:
   *  protocol://[[username[':'password]'@']hostname[':'dataport]]'/'[datapath]
   * and fill protocol, username, password, hostname, dataport, and datapath
   * when values are found.
   * Return true if the string matches the format; false otherwise.
   */
  static bool ParseURL( const kwsys_stl::string& URL,
                        kwsys_stl::string& protocol, 
                        kwsys_stl::string& username, 
                        kwsys_stl::string& password, 
                        kwsys_stl::string& hostname, 
                        kwsys_stl::string& dataport, 
                        kwsys_stl::string& datapath );

private:
  /**
   * Allocate the stl map that serve as the Path Translation table.
   */
  static void ClassInitialize();

  /**
   * Deallocate the stl map that serve as the Path Translation table.
   */
  static void ClassFinalize();

  /**
   * This method prevents warning on SGI
   */
  SystemToolsManager* GetSystemToolsManager()
    {
    return &SystemToolsManagerInstance;
    }

  /**
   * Find a filename (file or directory) in the system PATH, with
   * optional extra paths.
   */
  static kwsys_stl::string FindName(
    const char* name,
    const kwsys_stl::vector<kwsys_stl::string>& path = 
    kwsys_stl::vector<kwsys_stl::string>(),
    bool no_system_path = false);


  /**
   * Path translation table from dir to refdir
   * Each time 'dir' will be found it will be replace by 'refdir'
   */
  static SystemToolsTranslationMap *TranslationMap;
  static SystemToolsTranslationMap *LongPathMap;
  friend class SystemToolsManager;
};

} // namespace cmsys

/* Undefine temporary macros.  */
#if !defined (KWSYS_NAMESPACE) && !cmsys_NAME_IS_KWSYS
# undef kwsys_stl
# undef kwsys_ios
#endif

#endif
