/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile: cmDepends.h,v $
  Language:  C++
  Date:      $Date: 2007-12-28 16:49:59 $
  Version:   $Revision: 1.14 $

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef cmDepends_h
#define cmDepends_h

#include "cmStandardIncludes.h"

class cmFileTimeComparison;
class cmLocalGenerator;

/** \class cmDepends
 * \brief Dependency scanner superclass.
 *
 * This class is responsible for maintaining a .depends.make file in
 * the build tree corresponding to an object file.  Subclasses help it
 * maintain dependencies for particular languages.
 */
class cmDepends
{
public:
  /** Instances need to know the build directory name and the relative
      path from the build directory to the target file.  */
  cmDepends();
  
  /** at what level will the compile be done from */
  void SetCompileDirectory(const char *dir) {this->CompileDirectory = dir;};

  /** Set the local generator for the directory in which we are
      scanning dependencies.  This is not a full local generator; it
      has been setup to do relative path conversions for the current
      directory.  */
  void SetLocalGenerator(cmLocalGenerator* lg) { this->LocalGenerator = lg; }

  /** Set the specific language to be scanned.  */
  void SetLanguage(const char* lang) { this->Language = lang; }

  /** Set the target build directory.  */
  void SetTargetDirectory(const char* dir) { this->TargetDirectory = dir; }

  /** should this be verbose in its output */
  void SetVerbose(bool verb) { this->Verbose = verb; }
    
  /** Virtual destructor to cleanup subclasses properly.  */
  virtual ~cmDepends();

  /** Write dependencies for the target file.  */
  bool Write(std::ostream &makeDepends, std::ostream &internalDepends);
  
  /** Check dependencies for the target file.  Returns true if
      dependencies are okay and false if they must be generated.  If
      they must be generated Clear has already been called to wipe out
      the old dependencies.  */
  bool Check(const char *makeFile, const char* internalFile);

  /** Clear dependencies for the target file so they will be regenerated.  */
  void Clear(const char *file);

  /** Set the file comparison object */
  void SetFileComparison(cmFileTimeComparison* fc) { 
    this->FileComparison = fc; }

protected:

  // Write dependencies for the target file to the given stream.
  // Return true for success and false for failure.
  virtual bool WriteDependencies(const char *src, const char* obj,
    std::ostream& makeDepends, std::ostream& internalDepends);

  // Check dependencies for the target file in the given stream.
  // Return false if dependencies must be regenerated and true
  // otherwise.
  virtual bool CheckDependencies(std::istream& internalDepends);

  // Finalize the dependency information for the target.
  virtual bool Finalize(std::ostream& makeDepends,
                        std::ostream& internalDepends);

  // The directory in which the build rule for the target file is executed.
  std::string CompileDirectory;

  // The local generator.
  cmLocalGenerator* LocalGenerator;

  // Flag for verbose output.
  bool Verbose;
  cmFileTimeComparison* FileComparison;

  std::string Language;

  // The full path to the target's build directory.
  std::string TargetDirectory;

  size_t MaxPath;
  char* Dependee;
  char* Depender;

private:
  cmDepends(cmDepends const&); // Purposely not implemented.
  void operator=(cmDepends const&); // Purposely not implemented.
};

#endif
