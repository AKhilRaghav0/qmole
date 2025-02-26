/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile: cmSystemTools.h,v $
  Language:  C++
  Date:      $Date: 2008-04-21 00:44:53 $
  Version:   $Revision: 1.150.2.1 $

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef cmSystemTools_h
#define cmSystemTools_h

#include "cmStandardIncludes.h"

#include <cmsys/SystemTools.hxx>
#include <cmsys/Process.h>

class cmSystemToolsFileTime;

/** \class cmSystemTools
 * \brief A collection of useful functions for CMake.
 *
 * cmSystemTools is a class that provides helper functions
 * for the CMake build system.
 */
class cmSystemTools: public cmsys::SystemTools
{
public:
  typedef cmsys::SystemTools Superclass;
  
  /** Expand out any arguements in the vector that have ; separated
   *  strings into multiple arguements.  A new vector is created 
   *  containing the expanded versions of all arguments in argsIn.
   */
  static void ExpandList(std::vector<std::string> const& argsIn,
                         std::vector<std::string>& argsOut);
  static void ExpandListArgument(const std::string& arg,
                                 std::vector<std::string>& argsOut,
                                 bool emptyArgs=false);

  /**
   * Look for and replace registry values in a string
   */
  static void ExpandRegistryValues(std::string& source);

  /**
   * Platform independent escape spaces, unix uses backslash,
   * windows double quotes the string.
   */
  static std::string EscapeSpaces(const char* str);

  ///! Escape quotes in a string.
  static std::string EscapeQuotes(const char* str);
  
  /**
   * Given a string, replace any escape sequences with the corresponding
   * characters.
   */
  static std::string RemoveEscapes(const char*);
  
  typedef  void (*ErrorCallback)(const char*, const char*, bool&, void*);
  /**
   *  Set the function used by GUI's to display error messages
   *  Function gets passed: message as a const char*, 
   *  title as a const char*, and a reference to bool that when
   *  set to false, will disable furthur messages (cancel).
   */
  static void SetErrorCallback(ErrorCallback f, void* clientData=0);

  /**
   * Display an error message.
   */
  static void Error(const char* m, const char* m2=0,
                    const char* m3=0, const char* m4=0);

  /**
   * Display a message.
   */
  static void Message(const char* m, const char* title=0);

  ///! Send a string to stdout
  static void Stdout(const char* s);
  static void Stdout(const char* s, int length);
  typedef  void (*StdoutCallback)(const char*, int length, void*);
  static void SetStdoutCallback(StdoutCallback, void* clientData=0);

  ///! Return true if there was an error at any point.
  static bool GetErrorOccuredFlag() 
    {
      return cmSystemTools::s_ErrorOccured || 
        cmSystemTools::s_FatalErrorOccured;
    }
  ///! If this is set to true, cmake stops processing commands.
  static void SetFatalErrorOccured()
    {
      cmSystemTools::s_FatalErrorOccured = true;
    }
  static void SetErrorOccured()
    {
      cmSystemTools::s_ErrorOccured = true;
    }
 ///! Return true if there was an error at any point.
  static bool GetFatalErrorOccured() 
    {
      return cmSystemTools::s_FatalErrorOccured;
    }

  ///! Set the error occured flag and fatal error back to false
  static void ResetErrorOccuredFlag()
    {
      cmSystemTools::s_FatalErrorOccured = false;
      cmSystemTools::s_ErrorOccured = false;
    }
  
  /** 
   * does a string indicate a true or on value ? This is not the same
   * as ifdef. 
   */ 
  static bool IsOn(const char* val);
  
  /** 
   * does a string indicate a false or off value ? Note that this is
   * not the same as !IsOn(...) because there are a number of
   * ambiguous values such as "/usr/local/bin" a path will result in
   * IsON and IsOff both returning false. Note that the special path
   * NOTFOUND, *-NOTFOUND or IGNORE will cause IsOff to return true. 
   */
  static bool IsOff(const char* val);

  ///! Return true if value is NOTFOUND or ends in -NOTFOUND.
  static bool IsNOTFOUND(const char* value);
  ///! Return true if the path is a framework
  static bool IsPathToFramework(const char* value);
  
  static bool DoesFileExistWithExtensions(
    const char *name,
    const std::vector<std::string>& sourceExts);

  static void Glob(const char *directory, const char *regexp,
                   std::vector<std::string>& files);
  static void GlobDirs(const char *fullPath, std::vector<std::string>& files);

  /**
   * Try to find a list of files that match the "simple" globbing
   * expression. At this point in time the globbing expressions have
   * to be in form: /directory/partial_file_name*. The * character has
   * to be at the end of the string and it does not support ?
   * []... The optional argument type specifies what kind of files you
   * want to find. 0 means all files, -1 means directories, 1 means
   * files only. This method returns true if search was succesfull.
   */
  static bool SimpleGlob(const cmStdString& glob, 
                         std::vector<cmStdString>& files, 
                         int type = 0);
  
  ///! Copy a file.
  static bool cmCopyFile(const char* source, const char* destination);
  static bool CopyFileIfDifferent(const char* source, 
    const char* destination);

  ///! Compute the md5sum of a file
  static bool ComputeFileMD5(const char* source, char* md5out);

  /** Compute the md5sum of a string.  */
  static std::string ComputeStringMD5(const char* input);

  /**
   * Run an executable command and put the stdout in output.
   * A temporary file is created in the binaryDir for storing the
   * output because windows does not have popen.
   *
   * If verbose is false, no user-viewable output from the program
   * being run will be generated.
   *
   * If timeout is specified, the command will be terminated after
   * timeout expires.
   */
  static bool RunCommand(const char* command, std::string& output, 
                         const char* directory = 0,
                         bool verbose = true, int timeout = 0);
  static bool RunCommand(const char* command, std::string& output,
                         int &retVal, const char* directory = 0, 
                         bool verbose = true, int timeout = 0);  
  /**
   * Run a single executable command and put the stdout and stderr 
   * in output.
   *
   * If verbose is false, no user-viewable output from the program
   * being run will be generated.
   *
   * If timeout is specified, the command will be terminated after
   * timeout expires. Timeout is specified in seconds.
   *
   * Argument retVal should be a pointer to the location where the
   * exit code will be stored. If the retVal is not specified and 
   * the program exits with a code other than 0, then the this 
   * function will return false.
   *
   * If the command has spaces in the path the caller MUST call
   * cmSystemTools::ConvertToRunCommandPath on the command before passing
   * it into this function or it will not work.  The command must be correctly
   * escaped for this to with spaces.  
   */
  static bool RunSingleCommand(const char* command, std::string* output = 0,
                               int* retVal = 0, const char* dir = 0, 
                               bool verbose = true,
                               double timeout = 0.0);
  /** 
   * In this version of RunSingleCommand, command[0] should be
   * the command to run, and each argument to the command should
   * be in comand[1]...command[command.size()]
   */
  static bool RunSingleCommand(std::vector<cmStdString> const& command,
                               std::string* output = 0,
                               int* retVal = 0, const char* dir = 0, 
                               bool verbose = true,
                               double timeout = 0.0);

  /**
   * Parse arguments out of a single string command
   */
  static std::vector<cmStdString> ParseArguments(const char* command);

  /** Parse arguments out of a windows command line string.  */
  static void ParseWindowsCommandLine(const char* command,
                                      std::vector<std::string>& args);

  /** Compute an escaped version of the given argument for use in a
      windows shell.  See kwsys/System.h.in for details.  */
  static std::string EscapeWindowsShellArgument(const char* arg,
                                                int shell_flags);

  static void EnableMessages() { s_DisableMessages = false; }
  static void DisableMessages() { s_DisableMessages = true; }
  static void DisableRunCommandOutput() {s_DisableRunCommandOutput = true; }
  static void EnableRunCommandOutput() {s_DisableRunCommandOutput = false; }
  static bool GetRunCommandOutput() { return s_DisableRunCommandOutput; }

  /**
   * Come constants for different file formats.
   */
  enum FileFormat {
    NO_FILE_FORMAT = 0,
    C_FILE_FORMAT,
    CXX_FILE_FORMAT,
    FORTRAN_FILE_FORMAT,
    JAVA_FILE_FORMAT,
    HEADER_FILE_FORMAT,
    RESOURCE_FILE_FORMAT,
    DEFINITION_FILE_FORMAT,
    STATIC_LIBRARY_FILE_FORMAT,
    SHARED_LIBRARY_FILE_FORMAT,
    MODULE_FILE_FORMAT,
    OBJECT_FILE_FORMAT,
    UNKNOWN_FILE_FORMAT
  };

  /**
   * Determine the file type based on the extension
   */
  static FileFormat GetFileFormat(const char* ext);

  /**
   * On Windows 9x we need a comspec (command.com) substitute to run
   * programs correctly. This string has to be constant available
   * through the running of program. This method does not create a copy.
   */
  static void SetWindows9xComspecSubstitute(const char*);
  static const char* GetWindows9xComspecSubstitute();

  /** Windows if this is true, the CreateProcess in RunCommand will
   *  not show new consol windows when running programs.   
   */
  static void SetRunCommandHideConsole(bool v){s_RunCommandHideConsole = v;}
  static bool GetRunCommandHideConsole(){ return s_RunCommandHideConsole;}
  /** Call cmSystemTools::Error with the message m, plus the
   * result of strerror(errno)
   */
  static void ReportLastSystemError(const char* m);
  
  /** a general output handler for cmsysProcess  */
  static int WaitForLine(cmsysProcess* process, std::string& line,
                         double timeout,
                         std::vector<char>& out,
                         std::vector<char>& err);
    
  /** Split a string on its newlines into multiple lines.  Returns
      false only if the last line stored had no newline.  */
  static bool Split(const char* s, std::vector<cmStdString>& l);  
  static void SetForceUnixPaths(bool v)
    {
      s_ForceUnixPaths = v;
    }
  static bool GetForceUnixPaths()
    {
      return s_ForceUnixPaths;
    }

  // ConvertToOutputPath use s_ForceUnixPaths
  static std::string ConvertToOutputPath(const char* path);
  static void ConvertToOutputSlashes(std::string& path);

  // ConvertToRunCommandPath does not use s_ForceUnixPaths and should
  // be used when RunCommand is called from cmake, because the 
  // running cmake needs paths to be in its format
  static std::string ConvertToRunCommandPath(const char* path);
  //! Check if the first string ends with the second one.
  static bool StringEndsWith(const char* str1, const char* str2);
  
  /** compute the relative path from local to remote.  local must 
      be a directory.  remote can be a file or a directory.  
      Both remote and local must be full paths.  Basically, if
      you are in directory local and you want to access the file in remote
      what is the relative path to do that.  For example:
      /a/b/c/d to /a/b/c1/d1 -> ../../c1/d1
      from /usr/src to /usr/src/test/blah/foo.cpp -> test/blah/foo.cpp
  */
  static std::string RelativePath(const char* local, const char* remote);

  /** Put a string into the environment
      of the form var=value */
  static bool PutEnv(const char* value);

#ifdef CMAKE_BUILD_WITH_CMAKE
  /** Remove an environment variable */
  static bool UnsetEnv(const char* value);

  /** Get the list of all environment variables */
  static std::vector<std::string> GetEnvironmentVariables();
#endif

  /** Setup the environment to enable VS 8 IDE output.  */
  static void EnableVSConsoleOutput();

  /** Make string XML safe */
  static std::string MakeXMLSafe(const char* str);

  /** Create tar */
  static bool ListTar(const char* outFileName,
                      std::vector<cmStdString>& files, 
                      bool gzip, bool verbose);
  static bool CreateTar(const char* outFileName,
                        const std::vector<cmStdString>& files, bool gzip,
                        bool verbose);
  static bool ExtractTar(const char* inFileName,
                         const std::vector<cmStdString>& files, bool gzip, 
                         bool verbose);
  // This should be called first thing in main
  // it will keep child processes from inheriting the
  // stdin and stdout of this process.  This is important
  // if you want to be able to kill child processes and
  // not get stuck waiting for all the output on the pipes.
  static void DoNotInheritStdPipes();

  /** Copy the file create/access/modify times from the file named by
      the first argument to that named by the second.  */
  static bool CopyFileTime(const char* fromFile, const char* toFile);

  /** Save and restore file times.  */
  static cmSystemToolsFileTime* FileTimeNew();
  static void FileTimeDelete(cmSystemToolsFileTime*);
  static bool FileTimeGet(const char* fname, cmSystemToolsFileTime* t);
  static bool FileTimeSet(const char* fname, cmSystemToolsFileTime* t);

  /** Find the directory containing the running executable.  Save it
   in a global location to be queried by GetExecutableDirectory
   later.  */
  static void FindExecutableDirectory(const char* argv0);

  /** Get the directory containing the currently running executable.  */
  static const char* GetExecutableDirectory();

#if defined(CMAKE_BUILD_WITH_CMAKE)
  /** Echo a message in color using KWSys's Terminal cprintf.  */
  static void MakefileColorEcho(int color, const char* message,
                                bool newLine, bool enabled);
#endif

  /** Try to guess the soname of a shared library.  */
  static bool GuessLibrarySOName(std::string const& fullPath,
                                 std::string& soname);

  /** Try to set the RPATH in an ELF binary.  */
  static bool ChangeRPath(std::string const& file,
                          std::string const& oldRPath,
                          std::string const& newRPath,
                          std::string* emsg = 0);

  /** Try to remove the RPATH from an ELF binary.  */
  static bool RemoveRPath(std::string const& file, std::string* emsg = 0);

  /** Check whether the RPATH in an ELF binary contains the path
      given.  */
  static bool CheckRPath(std::string const& file,
                         std::string const& newRPath);

private:
  static bool s_ForceUnixPaths;
  static bool s_RunCommandHideConsole;
  static bool s_ErrorOccured;
  static bool s_FatalErrorOccured;
  static bool s_DisableMessages;
  static bool s_DisableRunCommandOutput;
  static ErrorCallback s_ErrorCallback;
  static StdoutCallback s_StdoutCallback;
  static void* s_ErrorCallbackClientData;
  static void* s_StdoutCallbackClientData;

  static std::string s_Windows9xComspecSubstitute;
};

#endif
