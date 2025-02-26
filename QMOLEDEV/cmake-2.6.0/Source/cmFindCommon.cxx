/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile: cmFindCommon.cxx,v $
  Language:  C++
  Date:      $Date: 2008-01-17 14:02:31 $
  Version:   $Revision: 1.1 $

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cmFindCommon.h"

//----------------------------------------------------------------------------
cmFindCommon::cmFindCommon()
{
  this->FindRootPathMode = RootPathModeBoth;
  this->NoDefaultPath = false;
  this->NoCMakePath = false;
  this->NoCMakeEnvironmentPath = false;
  this->NoSystemEnvironmentPath = false;
  this->NoCMakeSystemPath = false;

  // OS X Bundle and Framework search policy.  The default is to
  // search frameworks first on apple.
#if defined(__APPLE__)
  this->SearchFrameworkFirst = true;
  this->SearchAppBundleFirst = true;
#else
  this->SearchFrameworkFirst = false;
  this->SearchAppBundleFirst = false;
#endif
  this->SearchFrameworkOnly = false;
  this->SearchFrameworkLast = false;
  this->SearchAppBundleOnly = false;
  this->SearchAppBundleLast = false;

  // Documentation components.
  this->GenericDocumentationMacPolicy =
    "On Darwin or systems supporting OS X Frameworks, the cmake variable"
    "    CMAKE_FIND_FRAMEWORK can be set to empty or one of the following:\n"
    "   \"FIRST\"  - Try to find frameworks before standard\n"
    "              libraries or headers. This is the default on Darwin.\n"
    "   \"LAST\"   - Try to find frameworks after standard\n"
    "              libraries or headers.\n"
    "   \"ONLY\"   - Only try to find frameworks.\n"
    "   \"NEVER\". - Never try to find frameworks.\n"
    "On Darwin or systems supporting OS X Application Bundles, the cmake "
    "variable CMAKE_FIND_APPBUNDLE can be set to empty or one of the "
    "following:\n"
    "   \"FIRST\"  - Try to find application bundles before standard\n"
    "              programs. This is the default on Darwin.\n"
    "   \"LAST\"   - Try to find application bundles after standard\n"
    "              programs.\n"
    "   \"ONLY\"   - Only try to find application bundles.\n"
    "   \"NEVER\". - Never try to find application bundles.\n";
  this->GenericDocumentationRootPath =
    "The CMake variable CMAKE_FIND_ROOT_PATH specifies one or more "
    "directories to be prepended to all other search directories. "
    "This effectively \"re-roots\" the entire search under given locations. "
    "By default it is empty. It is especially useful when "
    "cross-compiling to point to the root directory of the "
    "target environment and CMake will search there too. By default at first "
    "the directories listed in CMAKE_FIND_ROOT_PATH and then the non-rooted "
    "directories will be searched. "
    "The default behavior can be adjusted by setting "
    "CMAKE_FIND_ROOT_PATH_MODE_XXX.  This behavior can be manually "
    "overridden on a per-call basis. "
    "By using CMAKE_FIND_ROOT_PATH_BOTH the search order will "
    "be as described above. If NO_CMAKE_FIND_ROOT_PATH is used "
    "then CMAKE_FIND_ROOT_PATH will not be used. If ONLY_CMAKE_FIND_ROOT_PATH "
    "is used then only the re-rooted directories will be searched.\n";
  this->GenericDocumentationPathsOrder =
    "The reason the paths listed in the call to the command are searched "
    "last is that most users of CMake would expect things to be found "
    "first in the locations specified by their environment.  Projects may "
    "override this behavior by simply calling the command twice:\n"
    "   FIND_XXX(FIND_ARGS_XXX PATHS paths... NO_DEFAULT_PATH)\n"
    "   FIND_XXX(FIND_ARGS_XXX)\n"
    "Once one of these calls succeeds the result variable will be set "
    "and stored in the cache so that neither call will search again.";
}

//----------------------------------------------------------------------------
cmFindCommon::~cmFindCommon()
{
}

//----------------------------------------------------------------------------
void cmFindCommon::SelectDefaultRootPathMode()
{
  // Use both by default.
  this->FindRootPathMode = RootPathModeBoth;

  // Check the policy variable for this find command type.
  std::string findRootPathVar = "CMAKE_FIND_ROOT_PATH_MODE_";
  findRootPathVar += this->CMakePathName;
  std::string rootPathMode =
    this->Makefile->GetSafeDefinition(findRootPathVar.c_str());
  if (rootPathMode=="NEVER")
    {
    this->FindRootPathMode = RootPathModeNoRootPath;
    }
  else if (rootPathMode=="ONLY")
    {
    this->FindRootPathMode = RootPathModeOnlyRootPath;
    }
  else if (rootPathMode=="BOTH")
    {
    this->FindRootPathMode = RootPathModeBoth;
    }
}

//----------------------------------------------------------------------------
void cmFindCommon::SelectDefaultMacMode()
{
  std::string ff = this->Makefile->GetSafeDefinition("CMAKE_FIND_FRAMEWORK");
  if(ff == "NEVER")
    {
    this->SearchFrameworkLast = false;
    this->SearchFrameworkFirst = false;
    this->SearchFrameworkOnly = false;
    }
  else if(ff == "ONLY")
    {
    this->SearchFrameworkLast = false;
    this->SearchFrameworkFirst = false;
    this->SearchFrameworkOnly = true;
    }
  else if(ff == "FIRST")
    {
    this->SearchFrameworkLast = false;
    this->SearchFrameworkFirst = true;
    this->SearchFrameworkOnly = false;
    }
  else if(ff == "LAST")
    {
    this->SearchFrameworkLast = true;
    this->SearchFrameworkFirst = false;
    this->SearchFrameworkOnly = false;
    }

  std::string fab = this->Makefile->GetSafeDefinition("CMAKE_FIND_APPBUNDLE");
  if(fab == "NEVER")
    {
    this->SearchAppBundleLast = false;
    this->SearchAppBundleFirst = false;
    this->SearchAppBundleOnly = false;
    }
  else if(fab == "ONLY")
    {
    this->SearchAppBundleLast = false;
    this->SearchAppBundleFirst = false;
    this->SearchAppBundleOnly = true;
    }
  else if(fab == "FIRST")
    {
    this->SearchAppBundleLast = false;
    this->SearchAppBundleFirst = true;
    this->SearchAppBundleOnly = false;
    }
  else if(fab == "LAST")
    {
    this->SearchAppBundleLast = true;
    this->SearchAppBundleFirst = false;
    this->SearchAppBundleOnly = false;
    }
}

//----------------------------------------------------------------------------
void cmFindCommon::RerootPaths(std::vector<std::string>& paths)
{
#if 0
  for(std::vector<std::string>::const_iterator i = paths.begin();
      i != paths.end(); ++i)
    {
    fprintf(stderr, "[%s]\n", i->c_str());
    }
#endif

  // Short-circuit if there is nothing to do.
  if(this->FindRootPathMode == RootPathModeNoRootPath)
    {
    return;
    }
  const char* rootPath =
    this->Makefile->GetDefinition("CMAKE_FIND_ROOT_PATH");
  if((rootPath == 0) || (strlen(rootPath) == 0))
    {
    return;
    }

  // Construct the list of path roots with no trailing slashes.
  std::vector<std::string> roots;
  cmSystemTools::ExpandListArgument(rootPath, roots);
  for(std::vector<std::string>::iterator ri = roots.begin();
      ri != roots.end(); ++ri)
    {
    cmSystemTools::ConvertToUnixSlashes(*ri);
    }

  // Copy the original set of unrooted paths.
  std::vector<std::string> unrootedPaths = paths;
  paths.clear();

  for(std::vector<std::string>::const_iterator ri = roots.begin();
      ri != roots.end(); ++ri)
    {
    for(std::vector<std::string>::const_iterator ui = unrootedPaths.begin();
        ui != unrootedPaths.end(); ++ui)
      {
      // Place the unrooted path under the current root if it is not
      // already inside.  Skip the unrooted path if it is relative to
      // a user home directory or is empty.
      std::string rootedDir;
      if(cmSystemTools::IsSubDirectory(ui->c_str(), ri->c_str()))
        {
        rootedDir = *ui;
        }
      else if(!ui->empty() && (*ui)[0] != '~')
        {
        // Start with the new root.
        rootedDir = *ri;
        rootedDir += "/";

        // Append the original path with its old root removed.
        rootedDir += cmSystemTools::SplitPathRootComponent(ui->c_str());
        }

      // Store the new path.
      paths.push_back(rootedDir);
      }
    }

  // If searching both rooted and unrooted paths add the original
  // paths again.
  if(this->FindRootPathMode == RootPathModeBoth)
    {
    paths.insert(paths.end(), unrootedPaths.begin(), unrootedPaths.end());
    }
}

//----------------------------------------------------------------------------
bool cmFindCommon::CheckCommonArgument(std::string const& arg)
{
  if(arg == "NO_DEFAULT_PATH")
    {
    this->NoDefaultPath = true;
    }
  else if(arg == "NO_CMAKE_ENVIRONMENT_PATH")
    {
    this->NoCMakeEnvironmentPath = true;
    }
  else if(arg == "NO_CMAKE_PATH")
    {
    this->NoCMakePath = true;
    }
  else if(arg == "NO_SYSTEM_ENVIRONMENT_PATH")
    {
    this->NoSystemEnvironmentPath = true;
    }
  else if(arg == "NO_CMAKE_SYSTEM_PATH")
    {
    this->NoCMakeSystemPath = true;
    }
  else if(arg == "NO_CMAKE_FIND_ROOT_PATH")
    {
    this->FindRootPathMode = RootPathModeNoRootPath;
    }
  else if(arg == "ONLY_CMAKE_FIND_ROOT_PATH")
    {
    this->FindRootPathMode = RootPathModeOnlyRootPath;
    }
  else if(arg == "CMAKE_FIND_ROOT_PATH_BOTH")
    {
    this->FindRootPathMode = RootPathModeBoth;
    }
  else
    {
    // The argument is not one of the above.
    return false;
    }

  // The argument is one of the above.
  return true;
}

//----------------------------------------------------------------------------
void cmFindCommon::AddPathSuffix(std::string const& arg)
{
  std::string suffix = arg;

  // Strip leading and trailing slashes.
  if(suffix.empty())
    {
    return;
    }
  if(suffix[0] == '/')
    {
    suffix = suffix.substr(1, suffix.npos);
    }
  if(suffix.empty())
    {
    return;
    }
  if(suffix[suffix.size()-1] == '/')
    {
    suffix = suffix.substr(0, suffix.size()-1);
    }
  if(suffix.empty())
    {
    return;
    }

  // Store the suffix.
  this->SearchPathSuffixes.push_back(suffix);
}

//----------------------------------------------------------------------------
void cmFindCommon::GetAppBundlePaths(std::vector<std::string>& paths)
{
  if(this->NoDefaultPath)
    {
    return;
    }
  std::vector<std::string> tmp;

  // first environment variables
  if(!this->NoCMakeEnvironmentPath)
    {
    cmSystemTools::GetPath(tmp, "CMAKE_APPBUNDLE_PATH");
    this->AddPathsInternal(paths, tmp, EnvPath);
    tmp.clear();
    }

  // add cmake variables
  if(!this->NoCMakePath)
    {
    if(const char* path =
       this->Makefile->GetDefinition("CMAKE_APPBUNDLE_PATH"))
      {
      cmSystemTools::ExpandListArgument(path, tmp);
      this->AddPathsInternal(paths, tmp, CMakePath);
      tmp.clear();
      }
    }

  // add cmake system variables
  if(!this->NoCMakeSystemPath)
    {
    if(const char* path =
       this->Makefile->GetDefinition("CMAKE_SYSTEM_APPBUNDLE_PATH"))
      {
      cmSystemTools::ExpandListArgument(path, tmp);
      this->AddPathsInternal(paths, tmp, CMakePath);
      tmp.clear();
      }
    }
}

//----------------------------------------------------------------------------
void cmFindCommon::GetFrameworkPaths(std::vector<std::string>& paths)
{
  if(this->NoDefaultPath)
    {
    return;
    }
  std::vector<std::string> tmp;

  // first environment variables
  if(!this->NoCMakeEnvironmentPath)
    {
    cmSystemTools::GetPath(tmp, "CMAKE_FRAMEWORK_PATH");
    this->AddPathsInternal(paths, tmp, EnvPath);
    tmp.clear();
    }

  // add cmake variables
  if(!this->NoCMakePath)
    {
    if(const char* path =
       this->Makefile->GetDefinition("CMAKE_FRAMEWORK_PATH"))
      {
      cmSystemTools::ExpandListArgument(path, tmp);
      this->AddPathsInternal(paths, tmp, CMakePath);
      tmp.clear();
      }
    }

  // add cmake system variables
  if(!this->NoCMakeSystemPath)
    {
    if(const char* path =
       this->Makefile->GetDefinition("CMAKE_SYSTEM_FRAMEWORK_PATH"))
      {
      cmSystemTools::ExpandListArgument(path, tmp);
      this->AddPathsInternal(paths, tmp, CMakePath);
      tmp.clear();
      }
    }
}

//----------------------------------------------------------------------------
void cmFindCommon::AddCMakePath(std::vector<std::string>& out_paths,
                                const char* variable,
                                std::set<cmStdString>* emmitted)
{
  // Get a path from a CMake variable.
  if(const char* varPath = this->Makefile->GetDefinition(variable))
    {
    std::vector<std::string> tmp;
    cmSystemTools::ExpandListArgument(varPath, tmp);

    // Relative paths are interpreted with respect to the current
    // source directory.
    this->AddPathsInternal(out_paths, tmp, CMakePath, emmitted);
    }
}

//----------------------------------------------------------------------------
void cmFindCommon::AddEnvPath(std::vector<std::string>& out_paths,
                              const char* variable,
                              std::set<cmStdString>* emmitted)
{
  // Get a path from the environment.
  std::vector<std::string> tmp;
  cmSystemTools::GetPath(tmp, variable);

  // Relative paths are interpreted with respect to the current
  // working directory.
  this->AddPathsInternal(out_paths, tmp, EnvPath, emmitted);
}

//----------------------------------------------------------------------------
void cmFindCommon::AddPathsInternal(std::vector<std::string>& out_paths,
                                    std::vector<std::string> const& in_paths,
                                    PathType pathType,
                                    std::set<cmStdString>* emmitted)
{
  for(std::vector<std::string>::const_iterator i = in_paths.begin();
      i != in_paths.end(); ++i)
    {
    this->AddPathInternal(out_paths, *i, pathType, emmitted);
    }
}

//----------------------------------------------------------------------------
void cmFindCommon::AddPathInternal(std::vector<std::string>& out_paths,
                                   std::string const& in_path,
                                   PathType pathType,
                                   std::set<cmStdString>* emmitted)
{
  if(in_path.empty())
    {
    return;
    }

  // Select the base path with which to interpret relative paths.
  const char* relbase = 0;
  if(pathType == CMakePath)
    {
    relbase = this->Makefile->GetCurrentDirectory();
    }

  // Convert to clean full path.
  std::string fullPath =
    cmSystemTools::CollapseFullPath(in_path.c_str(), relbase);

  // Insert the path if has not already been emmitted.
  if(!emmitted || emmitted->insert(fullPath).second)
    {
    out_paths.push_back(fullPath.c_str());
    }
}
