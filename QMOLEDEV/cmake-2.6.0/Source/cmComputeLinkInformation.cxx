/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile: cmComputeLinkInformation.cxx,v $
  Language:  C++
  Date:      $Date: 2008-05-01 16:35:39 $
  Version:   $Revision: 1.24.2.6 $

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cmComputeLinkInformation.h"

#include "cmComputeLinkDepends.h"
#include "cmOrderDirectories.h"

#include "cmGlobalGenerator.h"
#include "cmLocalGenerator.h"
#include "cmMakefile.h"
#include "cmTarget.h"
#include "cmake.h"

#include <ctype.h>

//#define CM_COMPUTE_LINK_INFO_DEBUG

/*
Notes about linking on various platforms:

------------------------------------------------------------------------------

Linux, FreeBSD, Mac OS X, IRIX, Sun, Windows:

Linking to libraries using the full path works fine.

------------------------------------------------------------------------------

On AIX, more work is needed.

  The "-bnoipath" option is needed.  From "man ld":

    Note: If you specify a shared object, or an archive file
    containing a shared object, with an absolute or relative path
    name, instead of with the -lName flag, the path name is
    included in the import file ID string in the loader section of
    the output file. You can override this behavior with the
    -bnoipath option.

      noipath

        For shared objects listed on the command-line, rather than
        specified with the -l flag, use a null path component when
        listing the shared object in the loader section of the
        output file. A null path component is always used for
        shared objects specified with the -l flag. This option
        does not affect the specification of a path component by
        using a line beginning with #! in an import file. The
        default is the ipath option.

  This prevents the full path specified on the compile line from being
  compiled directly into the binary.

  By default the linker places -L paths in the embedded runtime path.
  In order to implement CMake's RPATH interface correctly, we need the
  -blibpath:Path option.  From "man ld":

      libpath:Path

        Uses Path as the library path when writing the loader section
        of the output file. Path is neither checked for validity nor
        used when searching for libraries specified by the -l flag.
        Path overrides any library paths generated when the -L flag is
        used.

        If you do not specify any -L flags, or if you specify the
        nolibpath option, the default library path information is
        written in the loader section of the output file. The default
        library path information is the value of the LIBPATH
        environment variable if it is defined, and /usr/lib:/lib,
        otherwise.

  We can pass -Wl,-blibpath:/usr/lib:/lib always to avoid the -L stuff
  and not break when the user sets LIBPATH.  Then if we want to add an
  rpath we insert it into the option before /usr/lib.

------------------------------------------------------------------------------

On HP-UX, more work is needed.  There are differences between
versions.

ld: 92453-07 linker linker ld B.10.33 990520

  Linking with a full path works okay for static and shared libraries.
  The linker seems to always put the full path to where the library
  was found in the binary whether using a full path or -lfoo syntax.
  Transitive link dependencies work just fine due to the full paths.

  It has the "-l:libfoo.sl" option.  The +nodefaultrpath is accepted
  but not documented and does not seem to do anything.  There is no
  +forceload option.

ld: 92453-07 linker ld HP Itanium(R) B.12.41  IPF/IPF

  Linking with a full path works okay for static libraries.

  Linking with a full path works okay for shared libraries.  However
  dependent (transitive) libraries of those linked directly must be
  either found with an rpath stored in the direct dependencies or
  found in -L paths as if they were specified with "-l:libfoo.sl"
  (really "-l:<soname>").  The search matches that of the dynamic
  loader but only with -L paths.  In other words, if we have an
  executable that links to shared library bar which links to shared
  library foo, the link line for the exe must contain

    /dir/with/bar/libbar.sl -L/dir/with/foo

  It does not matter whether the exe wants to link to foo directly or
  whether /dir/with/foo/libfoo.sl is listed.  The -L path must still
  be present.  It should match the runtime path computed for the
  executable taking all directly and transitively linked libraries
  into account.

  The "+nodefaultrpath" option should be used to avoid getting -L
  paths in the rpath unless we add our own rpath with +b.  This means
  that skip-build-rpath should use this option.

  See documentation in "man ld", "man dld.so", and
  http://docs.hp.com/en/B2355-90968/creatingandusinglibraries.htm

    +[no]defaultrpath
      +defaultrpath is the default.  Include any paths that are
      specified with -L in the embedded path, unless you specify the
      +b option.  If you use +b, only the path list specified by +b is
      in the embedded path.

      The +nodefaultrpath option removes all library paths that were
      specified with the -L option from the embedded path.  The linker
      searches the library paths specified by the -L option at link
      time.  At run time, the only library paths searched are those
      specified by the environment variables LD_LIBRARY_PATH and
      SHLIB_PATH, library paths specified by the +b linker option, and
      finally the default library paths.

    +rpathfirst
      This option will cause the paths specified in RPATH (embedded
      path) to be used before the paths specified in LD_LIBRARY_PATH
      or SHLIB_PATH, in searching for shared libraries.  This changes
      the default search order of LD_LIBRARY_PATH, SHLIB_PATH, and
      RPATH (embedded path).

------------------------------------------------------------------------------
Notes about dependent (transitive) shared libraries:

On non-Windows systems shared libraries may have transitive
dependencies.  In order to support LINK_INTERFACE_LIBRARIES we must
support linking to a shared library without listing all the libraries
to which it links.  Some linkers want to be able to find the
transitive dependencies (dependent libraries) of shared libraries
listed on the command line.

  - On Windows, DLLs are not directly linked, and the import libraries
    have no transitive dependencies.

  - On Mac, we need to actually list the transitive dependencies.
    Otherwise when using -isysroot for universal binaries it cannot
    find the dependent libraries.  Listing them on the command line
    tells the linker where to find them, but unfortunately also links
    the library.

  - On HP-UX, the linker wants to find the transitive dependencies of
    shared libraries in the -L paths even if the dependent libraries
    are given on the link line.

  - On AIX the transitive dependencies are not needed.

  - On SGI, the linker wants to find the transitive dependencies of
    shared libraries in the -L paths if they are not given on the link
    line.  Transitive linking can be disabled using the options

      -no_transitive_link -Wl,-no_transitive_link

    which disable it.  Both options must be given when invoking the
    linker through the compiler.

  - On Sun, the linker wants to find the transitive dependencies of
    shared libraries in the -L paths if they are not given on the link
    line.

  - On Linux, FreeBSD, and QNX:

    The linker wants to find the transitive dependencies of shared
    libraries in the "-rpath-link" paths option if they have not been
    given on the link line.  The option is like rpath but just for
    link time:

      -Wl,-rpath-link,"/path1:/path2"

For -rpath-link, we need a separate runtime path ordering pass
including just the dependent libraries that are not linked.

For -L paths on non-HP, we can do the same thing as with rpath-link
but put the results in -L paths.  The paths should be listed at the
end to avoid conflicting with user search paths (?).

For -L paths on HP, we should do a runtime path ordering pass with
all libraries, both linked and non-linked.  Even dependent
libraries that are also linked need to be listed in -L paths.

In our implementation we add all dependent libraries to the runtime
path computation.  Then the auto-generated RPATH will find everything.

------------------------------------------------------------------------------
Notes about shared libraries with not builtin soname:

Some UNIX shared libraries may be created with no builtin soname.  On
some platforms such libraries cannot be linked using the path to their
location because the linker will copy the path into the field used to
find the library at runtime.

  Apple:    ../libfoo.dylib  ==>  libfoo.dylib  # ok, uses install_name
  SGI:      ../libfoo.so     ==>  libfoo.so     # ok
  AIX:      ../libfoo.so     ==>  libfoo.so     # ok
  Linux:    ../libfoo.so     ==>  ../libfoo.so  # bad
  HP-UX:    ../libfoo.so     ==>  ../libfoo.so  # bad
  Sun:      ../libfoo.so     ==>  ../libfoo.so  # bad
  FreeBSD:  ../libfoo.so     ==>  ../libfoo.so  # bad

In order to link these libraries we need to use the old-style split
into -L.. and -lfoo options.  This should be fairly safe because most
problems with -lfoo options were related to selecting shared libraries
instead of static but in this case we want the shared lib.  Link
directory ordering needs to be done to make sure these shared
libraries are found first.  There should be very few restrictions
because this need be done only for shared libraries without soname-s.

*/

//----------------------------------------------------------------------------
cmComputeLinkInformation
::cmComputeLinkInformation(cmTarget* target, const char* config)
{
  // Store context information.
  this->Target = target;
  this->Makefile = this->Target->GetMakefile();
  this->LocalGenerator = this->Makefile->GetLocalGenerator();
  this->GlobalGenerator = this->LocalGenerator->GetGlobalGenerator();
  this->CMakeInstance = this->GlobalGenerator->GetCMakeInstance();

  // The configuration being linked.
  this->Config = config;

  // Allocate internals.
  this->OrderLinkerSearchPath =
    new cmOrderDirectories(this->GlobalGenerator, target->GetName(),
                           "linker search path");
  this->OrderRuntimeSearchPath =
    new cmOrderDirectories(this->GlobalGenerator, target->GetName(),
                           "runtime search path");
  this->OrderDependentRPath = 0;

  // Get the language used for linking this target.
  this->LinkLanguage =
    this->Target->GetLinkerLanguage(this->GlobalGenerator);
  if(!this->LinkLanguage)
    {
    // The Compute method will do nothing, so skip the rest of the
    // initialization.
    return;
    }

  // Check whether we should use an import library for linking a target.
  this->UseImportLibrary =
    this->Makefile->GetDefinition("CMAKE_IMPORT_LIBRARY_SUFFIX")?true:false;

  // On platforms without import libraries there may be a special flag
  // to use when creating a plugin (module) that obtains symbols from
  // the program that will load it.
  this->LoaderFlag = 0;
  if(!this->UseImportLibrary &&
     this->Target->GetType() == cmTarget::MODULE_LIBRARY)
    {
    std::string loader_flag_var = "CMAKE_SHARED_MODULE_LOADER_";
    loader_flag_var += this->LinkLanguage;
    loader_flag_var += "_FLAG";
    this->LoaderFlag = this->Makefile->GetDefinition(loader_flag_var.c_str());
    }

  // Get options needed to link libraries.
  this->LibLinkFlag =
    this->Makefile->GetSafeDefinition("CMAKE_LINK_LIBRARY_FLAG");
  this->LibLinkFileFlag =
    this->Makefile->GetSafeDefinition("CMAKE_LINK_LIBRARY_FILE_FLAG");
  this->LibLinkSuffix =
    this->Makefile->GetSafeDefinition("CMAKE_LINK_LIBRARY_SUFFIX");

  // Get options needed to specify RPATHs.
  this->RuntimeUseChrpath = false;
  if(this->Target->GetType() != cmTarget::STATIC_LIBRARY)
    {
    const char* tType =
      ((this->Target->GetType() == cmTarget::EXECUTABLE)?
       "EXECUTABLE" : "SHARED_LIBRARY");
    std::string rtVar = "CMAKE_";
    rtVar += tType;
    rtVar += "_RUNTIME_";
    rtVar += this->LinkLanguage;
    rtVar += "_FLAG";
    std::string rtSepVar = rtVar + "_SEP";
    this->RuntimeFlag = this->Makefile->GetSafeDefinition(rtVar.c_str());
    this->RuntimeSep = this->Makefile->GetSafeDefinition(rtSepVar.c_str());
    this->RuntimeAlways =
      (this->Makefile->
       GetSafeDefinition("CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH"));
    this->RuntimeUseChrpath = this->Target->IsChrpathUsed();

    // Get options needed to help find dependent libraries.
    std::string rlVar = "CMAKE_";
    rlVar += tType;
    rlVar += "_RPATH_LINK_";
    rlVar += this->LinkLanguage;
    rlVar += "_FLAG";
    this->RPathLinkFlag = this->Makefile->GetSafeDefinition(rlVar.c_str());
    }

  // Check if we need to include the runtime search path at link time.
  {
  std::string var = "CMAKE_SHARED_LIBRARY_LINK_";
  var += this->LinkLanguage;
  var += "_WITH_RUNTIME_PATH";
  this->LinkWithRuntimePath = this->Makefile->IsOn(var.c_str());
  }

  // Check the platform policy for missing soname case.
  this->NoSONameUsesPath =
    this->Makefile->IsOn("CMAKE_PLATFORM_USES_PATH_WHEN_NO_SONAME");

  // Get link type information.
  this->ComputeLinkTypeInfo();

  // Setup the link item parser.
  this->ComputeItemParserInfo();

  // Setup framework support.
  this->ComputeFrameworkInfo();

  // Choose a mode for dealing with shared library dependencies.
  this->SharedDependencyMode = SharedDepModeNone;
  if(this->Makefile->IsOn("CMAKE_LINK_DEPENDENT_LIBRARY_FILES"))
    {
    this->SharedDependencyMode = SharedDepModeLink;
    }
  else if(this->Makefile->IsOn("CMAKE_LINK_DEPENDENT_LIBRARY_DIRS"))
    {
    this->SharedDependencyMode = SharedDepModeLibDir;
    }
  else if(!this->RPathLinkFlag.empty())
    {
    this->SharedDependencyMode = SharedDepModeDir;
    this->OrderDependentRPath =
      new cmOrderDirectories(this->GlobalGenerator, target->GetName(),
                             "dependent library path");
    }

  // Get the implicit link directories for this platform.
  if(const char* implicitLinks =
     (this->Makefile->GetDefinition
      ("CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES")))
    {
    std::vector<std::string> implicitLinkVec;
    cmSystemTools::ExpandListArgument(implicitLinks, implicitLinkVec);
    for(std::vector<std::string>::const_iterator
          i = implicitLinkVec.begin();
        i != implicitLinkVec.end(); ++i)
      {
      this->ImplicitLinkDirs.insert(*i);
      }
    }

  // Add the search path entries requested by the user to path ordering.
  this->OrderLinkerSearchPath
    ->AddUserDirectories(this->Target->GetLinkDirectories());
  this->OrderRuntimeSearchPath
    ->AddUserDirectories(this->Target->GetLinkDirectories());
  this->OrderLinkerSearchPath
    ->SetImplicitDirectories(this->ImplicitLinkDirs);
  this->OrderRuntimeSearchPath
    ->SetImplicitDirectories(this->ImplicitLinkDirs);
  if(this->OrderDependentRPath)
    {
    this->OrderDependentRPath
      ->SetImplicitDirectories(this->ImplicitLinkDirs);
    }

  // Decide whether to enable compatible library search path mode.
  // There exists code that effectively does
  //
  //    /path/to/libA.so -lB
  //
  // where -lB is meant to link to /path/to/libB.so.  This is broken
  // because it specified -lB without specifying a link directory (-L)
  // in which to search for B.  This worked in CMake 2.4 and below
  // because -L/path/to would be added by the -L/-l split for A.  In
  // order to support such projects we need to add the directories
  // containing libraries linked with a full path to the -L path.
  this->OldLinkDirMode =
    this->Target->GetPolicyStatusCMP0003() != cmPolicies::NEW;
  if(this->OldLinkDirMode)
    {
    // Construct a mask to not bother with this behavior for link
    // directories already specified by the user.
    std::vector<std::string> const& dirs = this->Target->GetLinkDirectories();
    for(std::vector<std::string>::const_iterator di = dirs.begin();
        di != dirs.end(); ++di)
      {
      this->OldLinkDirMask.insert(*di);
      }
    }
}

//----------------------------------------------------------------------------
cmComputeLinkInformation::~cmComputeLinkInformation()
{
  delete this->OrderLinkerSearchPath;
  delete this->OrderRuntimeSearchPath;
  delete this->OrderDependentRPath;
}

//----------------------------------------------------------------------------
cmComputeLinkInformation::ItemVector const&
cmComputeLinkInformation::GetItems()
{
  return this->Items;
}

//----------------------------------------------------------------------------
std::vector<std::string> const& cmComputeLinkInformation::GetDirectories()
{
  return this->OrderLinkerSearchPath->GetOrderedDirectories();
}

//----------------------------------------------------------------------------
std::string cmComputeLinkInformation::GetRPathLinkString()
{
  // If there is no separate linker runtime search flag (-rpath-link)
  // there is no reason to compute a string.
  if(!this->OrderDependentRPath)
    {
    return "";
    }

  // Construct the linker runtime search path.
  std::string rpath_link;
  const char* sep = "";
  std::vector<std::string> const& dirs =
    this->OrderDependentRPath->GetOrderedDirectories();
  for(std::vector<std::string>::const_iterator di = dirs.begin();
      di != dirs.end(); ++di)
    {
    rpath_link += sep;
    sep = ":";
    rpath_link += *di;
    }
  return rpath_link;
}

//----------------------------------------------------------------------------
std::vector<std::string> const& cmComputeLinkInformation::GetDepends()
{
  return this->Depends;
}

//----------------------------------------------------------------------------
std::vector<std::string> const& cmComputeLinkInformation::GetFrameworkPaths()
{
  return this->FrameworkPaths;
}

//----------------------------------------------------------------------------
std::set<cmTarget*> const&
cmComputeLinkInformation::GetSharedLibrariesLinked()
{
  return this->SharedLibrariesLinked;
}

//----------------------------------------------------------------------------
bool cmComputeLinkInformation::Compute()
{
  // Skip targets that do not link.
  if(!(this->Target->GetType() == cmTarget::EXECUTABLE ||
       this->Target->GetType() == cmTarget::SHARED_LIBRARY ||
       this->Target->GetType() == cmTarget::MODULE_LIBRARY ||
       this->Target->GetType() == cmTarget::STATIC_LIBRARY))
    {
    return false;
    }

  // We require a link language for the target.
  if(!this->LinkLanguage)
    {
    cmSystemTools::
      Error("CMake can not determine linker language for target:",
            this->Target->GetName());
    return false;
    }

  // Compute the ordered link line items.
  cmComputeLinkDepends cld(this->Target, this->Config);
  cld.SetOldLinkDirMode(this->OldLinkDirMode);
  cld.SetSharedRegex(this->SharedRegexString);
  cmComputeLinkDepends::EntryVector const& linkEntries = cld.Compute();

  // Add the link line items.
  for(cmComputeLinkDepends::EntryVector::const_iterator
        lei = linkEntries.begin();
      lei != linkEntries.end(); ++lei)
    {
    if(lei->IsSharedDep)
      {
      this->AddSharedDepItem(lei->Item, lei->Target);
      }
    else
      {
      this->AddItem(lei->Item, lei->Target);
      }
    }

  // Restore the target link type so the correct system runtime
  // libraries are found.
  const char* lss = this->Target->GetProperty("LINK_SEARCH_END_STATIC");
  if(cmSystemTools::IsOn(lss))
    {
    this->SetCurrentLinkType(LinkStatic);
    }
  else
    {
    this->SetCurrentLinkType(this->StartLinkType);
    }

  // Finish listing compatibility paths.
  if(this->OldLinkDirMode)
    {
    // For CMake 2.4 bug-compatibility we need to consider the output
    // directories of targets linked in another configuration as link
    // directories.
    std::set<cmTarget*> const& wrongItems = cld.GetOldWrongConfigItems();
    for(std::set<cmTarget*>::const_iterator i = wrongItems.begin();
        i != wrongItems.end(); ++i)
      {
      cmTarget* tgt = *i;
      bool implib =
        (this->UseImportLibrary &&
         (tgt->GetType() == cmTarget::SHARED_LIBRARY));
      std::string lib = tgt->GetFullPath(this->Config , implib, true);
      this->OldLinkDirItems.push_back(lib);
      }
    }

  // Finish setting up linker search directories.
  if(!this->FinishLinkerSearchDirectories())
    {
    return false;
    }

  return true;
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::AddItem(std::string const& item, cmTarget* tgt)
{
  // Compute the proper name to use to link this library.
  const char* config = this->Config;
  bool impexe = (tgt && tgt->IsExecutableWithExports());
  if(impexe && !this->UseImportLibrary && !this->LoaderFlag)
    {
    // Skip linking to executables on platforms with no import
    // libraries or loader flags.
    return;
    }

  if(tgt && (tgt->GetType() == cmTarget::STATIC_LIBRARY ||
             tgt->GetType() == cmTarget::SHARED_LIBRARY ||
             tgt->GetType() == cmTarget::MODULE_LIBRARY ||
             impexe))
    {
    // This is a CMake target.  Ask the target for its real name.
    if(impexe && this->LoaderFlag)
      {
      // This link item is an executable that may provide symbols
      // used by this target.  A special flag is needed on this
      // platform.  Add it now.
      std::string linkItem;
      linkItem = this->LoaderFlag;
      std::string exe = tgt->GetFullPath(config, this->UseImportLibrary,
                                         true);
      linkItem += exe;
      this->Items.push_back(Item(linkItem, true));
      this->Depends.push_back(exe);
      }
    else
      {
      // Decide whether to use an import library.
      bool implib =
        (this->UseImportLibrary &&
         (impexe || tgt->GetType() == cmTarget::SHARED_LIBRARY));

      // Pass the full path to the target file.
      std::string lib = tgt->GetFullPath(config, implib, true);
      this->Depends.push_back(lib);

      this->AddTargetItem(lib, tgt);
      this->AddLibraryRuntimeInfo(lib, tgt);
      }
    }
  else
    {
    // This is not a CMake target.  Use the name given.
    if(cmSystemTools::FileIsFullPath(item.c_str()))
      {
      if(cmSystemTools::FileIsDirectory(item.c_str()))
        {
        // This is a directory.
        this->AddDirectoryItem(item);
        }
      else
        {
        // Use the full path given to the library file.
        this->Depends.push_back(item);
        this->AddFullItem(item);
        this->AddLibraryRuntimeInfo(item);
        }
      }
    else
      {
      // This is a library or option specified by the user.
      this->AddUserItem(item);
      }
    }
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::AddSharedDepItem(std::string const& item,
                                                cmTarget* tgt)
{
  // If dropping shared library dependencies, ignore them.
  if(this->SharedDependencyMode == SharedDepModeNone)
    {
    return;
    }

  // The user may have incorrectly named an item.  Skip items that are
  // not full paths to shared libraries.
  if(tgt)
    {
    // The target will provide a full path.  Make sure it is a shared
    // library.
    if(tgt->GetType() != cmTarget::SHARED_LIBRARY)
      {
      return;
      }
    }
  else
    {
    // Skip items that are not full paths.  We will not be able to
    // reliably specify them.
    if(!cmSystemTools::FileIsFullPath(item.c_str()))
      {
      return;
      }

    // Get the name of the library from the file name.
    std::string file = cmSystemTools::GetFilenameName(item);
    if(!this->ExtractSharedLibraryName.find(file.c_str()))
      {
      // This is not the name of a shared library.
      return;
      }
    }

  // If in linking mode, just link to the shared library.
  if(this->SharedDependencyMode == SharedDepModeLink)
    {
    this->AddItem(item, tgt);
    return;
    }

  // Get a full path to the dependent shared library.
  // Add it to the runtime path computation so that the target being
  // linked will be able to find it.
  std::string lib;
  if(tgt)
    {
    lib = tgt->GetFullPath(this->Config, this->UseImportLibrary);
    this->AddLibraryRuntimeInfo(lib, tgt);
    }
  else
    {
    lib = item;
    this->AddLibraryRuntimeInfo(lib);
    }

  // Check if we need to include the dependent shared library in other
  // path ordering.
  cmOrderDirectories* order = 0;
  if(this->SharedDependencyMode == SharedDepModeLibDir &&
     !this->LinkWithRuntimePath /* AddLibraryRuntimeInfo adds it */)
    {
    // Add the item to the linker search path.
    order = this->OrderLinkerSearchPath;
    }
  else if(this->SharedDependencyMode == SharedDepModeDir)
    {
    // Add the item to the separate dependent library search path.
    order = this->OrderDependentRPath;
    }
  if(order)
    {
    if(tgt)
      {
      std::string soName = tgt->GetSOName(this->Config);
      const char* soname = soName.empty()? 0 : soName.c_str();
      order->AddRuntimeLibrary(lib, soname);
      }
    else
      {
      order->AddRuntimeLibrary(lib);
      }
    }
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::ComputeLinkTypeInfo()
{
  // Check whether archives may actually be shared libraries.
  this->ArchivesMayBeShared =
    this->CMakeInstance->GetPropertyAsBool(
      "TARGET_ARCHIVES_MAY_BE_SHARED_LIBS");

  // First assume we cannot do link type stuff.
  this->LinkTypeEnabled = false;

  // Lookup link type selection flags.
  const char* static_link_type_flag = 0;
  const char* shared_link_type_flag = 0;
  const char* target_type_str = 0;
  switch(this->Target->GetType())
    {
    case cmTarget::EXECUTABLE:     target_type_str = "EXE"; break;
    case cmTarget::SHARED_LIBRARY: target_type_str = "SHARED_LIBRARY"; break;
    case cmTarget::MODULE_LIBRARY: target_type_str = "SHARED_MODULE"; break;
    default: break;
    }
  if(target_type_str)
    {
    std::string static_link_type_flag_var = "CMAKE_";
    static_link_type_flag_var += target_type_str;
    static_link_type_flag_var += "_LINK_STATIC_";
    static_link_type_flag_var += this->LinkLanguage;
    static_link_type_flag_var += "_FLAGS";
    static_link_type_flag =
      this->Makefile->GetDefinition(static_link_type_flag_var.c_str());

    std::string shared_link_type_flag_var = "CMAKE_";
    shared_link_type_flag_var += target_type_str;
    shared_link_type_flag_var += "_LINK_DYNAMIC_";
    shared_link_type_flag_var += this->LinkLanguage;
    shared_link_type_flag_var += "_FLAGS";
    shared_link_type_flag =
      this->Makefile->GetDefinition(shared_link_type_flag_var.c_str());
    }

  // We can support link type switching only if all needed flags are
  // known.
  if(static_link_type_flag && *static_link_type_flag &&
     shared_link_type_flag && *shared_link_type_flag)
    {
    this->LinkTypeEnabled = true;
    this->StaticLinkTypeFlag = static_link_type_flag;
    this->SharedLinkTypeFlag = shared_link_type_flag;
    }

  // TODO: Lookup the starting link type from the target (is it being
  // linked statically?).
  this->StartLinkType = LinkShared;
  this->CurrentLinkType = this->StartLinkType;
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::ComputeItemParserInfo()
{
  // Get possible library name prefixes.
  cmMakefile* mf = this->Makefile;
  this->AddLinkPrefix(mf->GetDefinition("CMAKE_STATIC_LIBRARY_PREFIX"));
  this->AddLinkPrefix(mf->GetDefinition("CMAKE_SHARED_LIBRARY_PREFIX"));

  // Import library names should be matched and treated as shared
  // libraries for the purposes of linking.
  this->AddLinkExtension(mf->GetDefinition("CMAKE_IMPORT_LIBRARY_SUFFIX"),
                         LinkShared);
  this->AddLinkExtension(mf->GetDefinition("CMAKE_STATIC_LIBRARY_SUFFIX"),
                         LinkStatic);
  this->AddLinkExtension(mf->GetDefinition("CMAKE_SHARED_LIBRARY_SUFFIX"),
                         LinkShared);
  this->AddLinkExtension(mf->GetDefinition("CMAKE_LINK_LIBRARY_SUFFIX"),
                         LinkUnknown);
  if(const char* linkSuffixes =
     mf->GetDefinition("CMAKE_EXTRA_LINK_EXTENSIONS"))
    {
    std::vector<std::string> linkSuffixVec;
    cmSystemTools::ExpandListArgument(linkSuffixes, linkSuffixVec);
    for(std::vector<std::string>::iterator i = linkSuffixVec.begin();
        i != linkSuffixVec.end(); ++i)
      {
      this->AddLinkExtension(i->c_str(), LinkUnknown);
      }
    }

  // Compute a regex to match link extensions.
  std::string libext = this->CreateExtensionRegex(this->LinkExtensions);

  // Create regex to remove any library extension.
  std::string reg("(.*)");
  reg += libext;
  this->OrderLinkerSearchPath->SetLinkExtensionInfo(this->LinkExtensions,
                                                    reg);

  // Create a regex to match a library name.  Match index 1 will be
  // the prefix if it exists and empty otherwise.  Match index 2 will
  // be the library name.  Match index 3 will be the library
  // extension.
  reg = "^(";
  for(std::set<cmStdString>::iterator p = this->LinkPrefixes.begin();
      p != this->LinkPrefixes.end(); ++p)
    {
    reg += *p;
    reg += "|";
    }
  reg += ")";
  reg += "([^/]*)";

  // Create a regex to match any library name.
  std::string reg_any = reg;
  reg_any += libext;
#ifdef CM_COMPUTE_LINK_INFO_DEBUG
  fprintf(stderr, "any regex [%s]\n", reg_any.c_str());
#endif
  this->ExtractAnyLibraryName.compile(reg_any.c_str());

  // Create a regex to match static library names.
  if(!this->StaticLinkExtensions.empty())
    {
    std::string reg_static = reg;
    reg_static += this->CreateExtensionRegex(this->StaticLinkExtensions);
#ifdef CM_COMPUTE_LINK_INFO_DEBUG
  fprintf(stderr, "static regex [%s]\n", reg_static.c_str());
#endif
    this->ExtractStaticLibraryName.compile(reg_static.c_str());
    }

  // Create a regex to match shared library names.
  if(!this->SharedLinkExtensions.empty())
    {
    std::string reg_shared = reg;
    this->SharedRegexString =
      this->CreateExtensionRegex(this->SharedLinkExtensions);
    reg_shared += this->SharedRegexString;
#ifdef CM_COMPUTE_LINK_INFO_DEBUG
  fprintf(stderr, "shared regex [%s]\n", reg_shared.c_str());
#endif
    this->ExtractSharedLibraryName.compile(reg_shared.c_str());
    }
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::AddLinkPrefix(const char* p)
{
  if(p)
    {
    this->LinkPrefixes.insert(p);
    }
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::AddLinkExtension(const char* e, LinkType type)
{
  if(e && *e)
    {
    if(type == LinkStatic)
      {
      this->StaticLinkExtensions.push_back(e);
      }
    if(type == LinkShared)
      {
      this->SharedLinkExtensions.push_back(e);
      }
    this->LinkExtensions.push_back(e);
    }
}

//----------------------------------------------------------------------------
std::string
cmComputeLinkInformation
::CreateExtensionRegex(std::vector<std::string> const& exts)
{
  // Build a list of extension choices.
  std::string libext = "(";
  const char* sep = "";
  for(std::vector<std::string>::const_iterator i = exts.begin();
      i != exts.end(); ++i)
    {
    // Separate this choice from the previous one.
    libext += sep;
    sep = "|";

    // Store this extension choice with the "." escaped.
    libext += "\\";
#if defined(_WIN32) && !defined(__CYGWIN__)
    libext += this->NoCaseExpression(i->c_str());
#else
    libext += *i;
#endif
    }

  // Finish the list.
  libext += ")$";
  return libext;
}

//----------------------------------------------------------------------------
std::string cmComputeLinkInformation::NoCaseExpression(const char* str)
{
  std::string ret;
  const char* s = str;
  while(*s)
    {
    if(*s == '.')
      {
      ret += *s;
      }
    else
      {
      ret += "[";
      ret += tolower(*s);
      ret += toupper(*s);
      ret += "]";
      }
    s++;
    }
  return ret;
}

//-------------------------------------------------------------------
void cmComputeLinkInformation::SetCurrentLinkType(LinkType lt)
{
  // If we are changing the current link type add the flag to tell the
  // linker about it.
  if(this->CurrentLinkType != lt)
    {
    this->CurrentLinkType = lt;

    if(this->LinkTypeEnabled)
      {
      switch(this->CurrentLinkType)
        {
        case LinkStatic:
          this->Items.push_back(Item(this->StaticLinkTypeFlag, false));
          break;
        case LinkShared:
          this->Items.push_back(Item(this->SharedLinkTypeFlag, false));
          break;
        default:
          break;
        }
      }
    }
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::AddTargetItem(std::string const& item,
                                             cmTarget* target)
{
  // This is called to handle a link item that is a full path to a target.
  // If the target is not a static library make sure the link type is
  // shared.  This is because dynamic-mode linking can handle both
  // shared and static libraries but static-mode can handle only
  // static libraries.  If a previous user item changed the link type
  // to static we need to make sure it is back to shared.
  if(target->GetType() != cmTarget::STATIC_LIBRARY)
    {
    this->SetCurrentLinkType(LinkShared);
    }

  // Keep track of shared library targets linked.
  if(target->GetType() == cmTarget::SHARED_LIBRARY)
    {
    this->SharedLibrariesLinked.insert(target);
    }

  // Handle case of an imported shared library with no soname.
  if(this->NoSONameUsesPath &&
     target->IsImportedSharedLibWithoutSOName(this->Config))
    {
    this->AddSharedLibNoSOName(item);
    return;
    }

  // If this platform wants a flag before the full path, add it.
  if(!this->LibLinkFileFlag.empty())
    {
    this->Items.push_back(Item(this->LibLinkFileFlag, false));
    }

  // For compatibility with CMake 2.4 include the item's directory in
  // the linker search path.
  if(this->OldLinkDirMode && !target->IsFrameworkOnApple() &&
     this->OldLinkDirMask.find(cmSystemTools::GetFilenamePath(item)) ==
     this->OldLinkDirMask.end())
    {
    this->OldLinkDirItems.push_back(item);
    }

  // Now add the full path to the library.
  this->Items.push_back(Item(item, true));
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::AddFullItem(std::string const& item)
{
  // Check for the implicit link directory special case.
  if(this->CheckImplicitDirItem(item))
    {
    return;
    }

  // Check for case of shared library with no builtin soname.
  if(this->NoSONameUsesPath && this->CheckSharedLibNoSOName(item))
    {
    return;
    }

  // This is called to handle a link item that is a full path.
  // If the target is not a static library make sure the link type is
  // shared.  This is because dynamic-mode linking can handle both
  // shared and static libraries but static-mode can handle only
  // static libraries.  If a previous user item changed the link type
  // to static we need to make sure it is back to shared.
  if(this->LinkTypeEnabled)
    {
    std::string name = cmSystemTools::GetFilenameName(item);
    if(this->ExtractSharedLibraryName.find(name))
      {
      this->SetCurrentLinkType(LinkShared);
      }
    else if(!this->ExtractStaticLibraryName.find(item))
      {
      // We cannot determine the type.  Assume it is the target's
      // default type.
      this->SetCurrentLinkType(this->StartLinkType);
      }
    }

  // For compatibility with CMake 2.4 include the item's directory in
  // the linker search path.
  if(this->OldLinkDirMode &&
     this->OldLinkDirMask.find(cmSystemTools::GetFilenamePath(item)) ==
     this->OldLinkDirMask.end())
    {
    this->OldLinkDirItems.push_back(item);
    }

  // If this platform wants a flag before the full path, add it.
  if(!this->LibLinkFileFlag.empty())
    {
    this->Items.push_back(Item(this->LibLinkFileFlag, false));
    }

  // Now add the full path to the library.
  this->Items.push_back(Item(item, true));
}

//----------------------------------------------------------------------------
bool cmComputeLinkInformation::CheckImplicitDirItem(std::string const& item)
{
  // We only switch to a pathless item if the link type may be
  // enforced.  Fortunately only platforms that support link types
  // seem to have magic per-architecture implicit link directories.
  if(!this->LinkTypeEnabled)
    {
    return false;
    }

  // Check if this item is in an implicit link directory.
  std::string dir = cmSystemTools::GetFilenamePath(item);
  if(this->ImplicitLinkDirs.find(dir) == this->ImplicitLinkDirs.end())
    {
    // Only libraries in implicit link directories are converted to
    // pathless items.
    return false;
    }

  // Only apply the policy below if the library file is one that can
  // be found by the linker.
  std::string file = cmSystemTools::GetFilenameName(item);
  if(!this->ExtractAnyLibraryName.find(file))
    {
    return false;
    }

  // Many system linkers support multiple architectures by
  // automatically selecting the implicit linker search path for the
  // current architecture.  If the library appears in an implicit link
  // directory then just report the file name without the directory
  // portion.  This will allow the system linker to locate the proper
  // library for the architecture at link time.
  this->AddUserItem(file);
  return true;
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::AddUserItem(std::string const& item)
{
  // This is called to handle a link item that does not match a CMake
  // target and is not a full path.  We check here if it looks like a
  // library file name to automatically request the proper link type
  // from the linker.  For example:
  //
  //   foo       ==>  -lfoo
  //   libfoo.a  ==>  -Wl,-Bstatic -lfoo
  std::string lib;

  // Parse out the prefix, base, and suffix components of the
  // library name.  If the name matches that of a shared or static
  // library then set the link type accordingly.
  //
  // Search for shared library names first because some platforms
  // have shared libraries with names that match the static library
  // pattern.  For example cygwin and msys use the convention
  // libfoo.dll.a for import libraries and libfoo.a for static
  // libraries.  On AIX a library with the name libfoo.a can be
  // shared!
  if(this->ExtractSharedLibraryName.find(item))
    {
    // This matches a shared library file name.
#ifdef CM_COMPUTE_LINK_INFO_DEBUG
    fprintf(stderr, "shared regex matched [%s] [%s] [%s]\n",
            this->ExtractSharedLibraryName.match(1).c_str(),
            this->ExtractSharedLibraryName.match(2).c_str(),
            this->ExtractSharedLibraryName.match(3).c_str());
#endif
    // Set the link type to shared.
    this->SetCurrentLinkType(LinkShared);

    // Use just the library name so the linker will search.
    lib = this->ExtractSharedLibraryName.match(2);
    }
  else if(this->ExtractStaticLibraryName.find(item))
    {
    // This matches a static library file name.
#ifdef CM_COMPUTE_LINK_INFO_DEBUG
    fprintf(stderr, "static regex matched [%s] [%s] [%s]\n",
            this->ExtractStaticLibraryName.match(1).c_str(),
            this->ExtractStaticLibraryName.match(2).c_str(),
            this->ExtractStaticLibraryName.match(3).c_str());
#endif
    // Set the link type to static.
    this->SetCurrentLinkType(LinkStatic);

    // Use just the library name so the linker will search.
    lib = this->ExtractStaticLibraryName.match(2);
    }
  else if(this->ExtractAnyLibraryName.find(item))
    {
    // This matches a library file name.
#ifdef CM_COMPUTE_LINK_INFO_DEBUG
    fprintf(stderr, "any regex matched [%s] [%s] [%s]\n",
            this->ExtractAnyLibraryName.match(1).c_str(),
            this->ExtractAnyLibraryName.match(2).c_str(),
            this->ExtractAnyLibraryName.match(3).c_str());
#endif
    // Restore the target link type since this item does not specify
    // one.
    this->SetCurrentLinkType(this->StartLinkType);

    // Use just the library name so the linker will search.
    lib = this->ExtractAnyLibraryName.match(2);
    }
  else if(item[0] == '-' || item[0] == '$' || item[0] == '`')
    {
    // if this is a -l option then we might need to warn about
    // CMP0003 so put it in OldUserFlagItems, if it is not a -l
    // or -Wl,-l (-framework -pthread), then allow it without a 
    // CMP0003 as -L will not affect those other linker flags
    if(item.find("-l") == 0 ||  item.find("-Wl,-l") == 0)
      {
      // This is a linker option provided by the user.
      this->OldUserFlagItems.push_back(item);
      }

    // Restore the target link type since this item does not specify
    // one.
    this->SetCurrentLinkType(this->StartLinkType);

    // Use the item verbatim.
    this->Items.push_back(Item(item, false));
    return;
    }
  else
    {
    // This is a name specified by the user.
    this->OldUserFlagItems.push_back(item);

    // We must ask the linker to search for a library with this name.
    // Restore the target link type since this item does not specify
    // one.
    this->SetCurrentLinkType(this->StartLinkType);
    lib = item;
    }

  // Create an option to ask the linker to search for the library.
  std::string out = this->LibLinkFlag;
  out += lib;
  out += this->LibLinkSuffix;
  this->Items.push_back(Item(out, false));

  // Here we could try to find the library the linker will find and
  // add a runtime information entry for it.  It would probably not be
  // reliable and we want to encourage use of full paths for library
  // specification.
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::AddFrameworkItem(std::string const& item)
{
  // Try to separate the framework name and path.
  if(!this->SplitFramework.find(item.c_str()))
    {
    cmOStringStream e;
    e << "Could not parse framework path \"" << item << "\" "
      << "linked by target " << this->Target->GetName() << ".";
    cmSystemTools::Error(e.str().c_str());
    return;
    }

  // Add the directory portion to the framework search path.
  this->AddFrameworkPath(this->SplitFramework.match(1));

  // Add the item using the -framework option.
  std::string fw = "-framework ";
  fw += this->SplitFramework.match(2);
  this->Items.push_back(Item(fw, false));
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::AddDirectoryItem(std::string const& item)
{
#ifdef __APPLE__
  if(cmSystemTools::IsPathToFramework(item.c_str()))
    {
    this->AddFrameworkItem(item);
    }
  else
#endif
    {
    this->DropDirectoryItem(item);
    }
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::DropDirectoryItem(std::string const& item)
{
  // A full path to a directory was found as a link item.  Warn the
  // user.
  cmOStringStream e;
  e << "WARNING: Target \"" << this->Target->GetName()
    << "\" requests linking to directory \"" << item << "\".  "
    << "Targets may link only to libraries.  "
    << "CMake is dropping the item.";
  cmSystemTools::Message(e.str().c_str());
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::ComputeFrameworkInfo()
{
  // Avoid adding system framework paths.  See "man ld" on OS X.
  this->FrameworkPathsEmmitted.insert("/Library/Frameworks");
  this->FrameworkPathsEmmitted.insert("/Network/Library/Frameworks");
  this->FrameworkPathsEmmitted.insert("/System/Library/Frameworks");

  // Regular expression to extract a framework path and name.
  this->SplitFramework.compile("(.*)/(.*)\\.framework$");
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::AddFrameworkPath(std::string const& p)
{
  if(this->FrameworkPathsEmmitted.insert(p).second)
    {
    this->FrameworkPaths.push_back(p);
    }
}

//----------------------------------------------------------------------------
bool cmComputeLinkInformation::CheckSharedLibNoSOName(std::string const& item)
{
  // This platform will use the path to a library as its soname if the
  // library is given via path and was not built with an soname.  If
  // this is a shared library that might be the case.
  std::string file = cmSystemTools::GetFilenameName(item);
  if(this->ExtractSharedLibraryName.find(file))
    {
    // If we can guess the soname fairly reliably then assume the
    // library has one.  Otherwise assume the library has no builtin
    // soname.
    std::string soname;
    if(!cmSystemTools::GuessLibrarySOName(item, soname))
      {
      this->AddSharedLibNoSOName(item);
      return true;
      }
    }
  return false;
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::AddSharedLibNoSOName(std::string const& item)
{
  // We have a full path to a shared library with no soname.  We need
  // to ask the linker to locate the item because otherwise the path
  // we give to it will be embedded in the target linked.  Then at
  // runtime the dynamic linker will search for the library using the
  // path instead of just the name.
  std::string file = cmSystemTools::GetFilenameName(item);
  this->AddUserItem(file);

  // Make sure the link directory ordering will find the library.
  this->OrderLinkerSearchPath->AddLinkLibrary(item);
}

//----------------------------------------------------------------------------
bool cmComputeLinkInformation::FinishLinkerSearchDirectories()
{
  // Support broken projects if necessary.
  if(this->OldLinkDirItems.empty() || this->OldUserFlagItems.empty() ||
     !this->OldLinkDirMode)
    {
    return true;
    }

  // Enforce policy constraints.
  switch(this->Target->GetPolicyStatusCMP0003())
    {
    case cmPolicies::WARN:
      if(!this->CMakeInstance->GetPropertyAsBool("CMP0003-WARNING-GIVEN"))
        {
        this->CMakeInstance->SetProperty("CMP0003-WARNING-GIVEN", "1");
        cmOStringStream w;
        this->PrintLinkPolicyDiagnosis(w);
        this->CMakeInstance->IssueMessage(cmake::AUTHOR_WARNING, w.str(),
                                          this->Target->GetBacktrace());
        }
    case cmPolicies::OLD:
      // OLD behavior is to add the paths containing libraries with
      // known full paths as link directories.
      break;
    case cmPolicies::NEW:
      // Should never happen due to assignment of OldLinkDirMode
      return true;
    case cmPolicies::REQUIRED_IF_USED:
    case cmPolicies::REQUIRED_ALWAYS:
      {
      cmOStringStream e;
      e << (this->Makefile->GetPolicies()->
            GetRequiredPolicyError(cmPolicies::CMP0003)) << "\n";
      this->PrintLinkPolicyDiagnosis(e);
      this->CMakeInstance->IssueMessage(cmake::FATAL_ERROR, e.str(),
                                        this->Target->GetBacktrace());
      return false;
      }
    }

  // Add the link directories for full path items.
  for(std::vector<std::string>::const_iterator
        i = this->OldLinkDirItems.begin();
      i != this->OldLinkDirItems.end(); ++i)
    {
    this->OrderLinkerSearchPath->AddLinkLibrary(*i);
    }
  return true;
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::PrintLinkPolicyDiagnosis(std::ostream& os)
{
  // Tell the user what to do.
  os << "Policy CMP0003 should be set before this line.  "
     << "Add code such as\n"
     << "  if(COMMAND cmake_policy)\n"
     << "    cmake_policy(SET CMP0003 NEW)\n"
     << "  endif(COMMAND cmake_policy)\n"
     << "as early as possible but after the most recent call to "
     << "cmake_minimum_required or cmake_policy(VERSION).  ";

  // List the items that might need the old-style paths.
  os << "This warning appears because target \""
     << this->Target->GetName() << "\" "
     << "links to some libraries for which the linker must search:\n";
  {
  // Format the list of unknown items to be as short as possible while
  // still fitting in the allowed width (a true solution would be the
  // bin packing problem if we were allowed to change the order).
  std::string::size_type max_size = 76;
  std::string line;
  const char* sep = "  ";
  for(std::vector<std::string>::const_iterator
        i = this->OldUserFlagItems.begin();
      i != this->OldUserFlagItems.end(); ++i)
    {
    // If the addition of another item will exceed the limit then
    // output the current line and reset it.  Note that the separator
    // is either " " or ", " which is always 2 characters.
    if(!line.empty() && (line.size() + i->size() + 2) > max_size)
      {
      os << line << "\n";
      sep = "  ";
      line = "";
      }
    line += sep;
    line += *i;
    // Convert to the other separator.
    sep = ", ";
    }
  if(!line.empty())
    {
    os << line << "\n";
    }
  }

  // List the paths old behavior is adding.
  os << "and other libraries with known full path:\n";
  std::set<cmStdString> emitted;
  for(std::vector<std::string>::const_iterator
        i = this->OldLinkDirItems.begin();
      i != this->OldLinkDirItems.end(); ++i)
    {
    if(emitted.insert(cmSystemTools::GetFilenamePath(*i)).second)
      {
      os << "  " << *i << "\n";
      }
    }

  // Explain.
  os << "CMake is adding directories in the second list to the linker "
     << "search path in case they are needed to find libraries from the "
     << "first list (for backwards compatibility with CMake 2.4).  "
     << "Set policy CMP0003 to OLD or NEW to enable or disable this "
     << "behavior explicitly.  "
     << "Run \"cmake --help-policy CMP0003\" for more information.";
}

//----------------------------------------------------------------------------
std::vector<std::string> const&
cmComputeLinkInformation::GetRuntimeSearchPath()
{
  return this->OrderRuntimeSearchPath->GetOrderedDirectories();
}

//----------------------------------------------------------------------------
void
cmComputeLinkInformation::AddLibraryRuntimeInfo(std::string const& fullPath,
                                                cmTarget* target)
{
  // Skip targets that are not shared libraries (modules cannot be linked).
  if(target->GetType() != cmTarget::SHARED_LIBRARY)
    {
    return;
    }

  // Try to get the soname of the library.  Only files with this name
  // could possibly conflict.
  std::string soName = target->GetSOName(this->Config);
  const char* soname = soName.empty()? 0 : soName.c_str();

  // Include this library in the runtime path ordering.
  this->OrderRuntimeSearchPath->AddRuntimeLibrary(fullPath, soname);
  if(this->LinkWithRuntimePath)
    {
    this->OrderLinkerSearchPath->AddRuntimeLibrary(fullPath, soname);
    }
}

//----------------------------------------------------------------------------
void
cmComputeLinkInformation::AddLibraryRuntimeInfo(std::string const& fullPath)
{
  // Get the name of the library from the file name.
  std::string file = cmSystemTools::GetFilenameName(fullPath);
  if(!this->ExtractSharedLibraryName.find(file.c_str()))
    {
    // On some platforms (AIX) a shared library may look static.
    if(this->ArchivesMayBeShared)
      {
      if(!this->ExtractStaticLibraryName.find(file.c_str()))
        {
        // This is not the name of a shared library or archive.
        return;
        }
      }
    else
      {
      // This is not the name of a shared library.
      return;
      }
    }

  // Include this library in the runtime path ordering.
  this->OrderRuntimeSearchPath->AddRuntimeLibrary(fullPath);
  if(this->LinkWithRuntimePath)
    {
    this->OrderLinkerSearchPath->AddRuntimeLibrary(fullPath);
    }
}

//----------------------------------------------------------------------------
void cmComputeLinkInformation::GetRPath(std::vector<std::string>& runtimeDirs,
                                        bool for_install)
{
  // Select whether to generate runtime search directories.
  bool outputRuntime =
    !this->Makefile->IsOn("CMAKE_SKIP_RPATH") && !this->RuntimeFlag.empty();

  // Select whether to generate an rpath for the install tree or the
  // build tree.
  bool linking_for_install =
    (for_install ||
     this->Target->GetPropertyAsBool("BUILD_WITH_INSTALL_RPATH"));
  bool use_install_rpath =
    (outputRuntime && this->Target->HaveInstallTreeRPATH() &&
     linking_for_install);
  bool use_build_rpath =
    (outputRuntime && this->Target->HaveBuildTreeRPATH() &&
     !linking_for_install);
  bool use_link_rpath =
    outputRuntime && linking_for_install &&
    this->Target->GetPropertyAsBool("INSTALL_RPATH_USE_LINK_PATH");

  // Construct the RPATH.
  if(use_install_rpath)
    {
    const char* install_rpath = this->Target->GetProperty("INSTALL_RPATH");
    cmSystemTools::ExpandListArgument(install_rpath, runtimeDirs);
    }
  if(use_build_rpath || use_link_rpath)
    {
    std::vector<std::string> const& rdirs = this->GetRuntimeSearchPath();
    for(std::vector<std::string>::const_iterator ri = rdirs.begin();
        ri != rdirs.end(); ++ri)
      {
      // Put this directory in the rpath if using build-tree rpath
      // support or if using the link path as an rpath.
      if(use_build_rpath)
        {
        runtimeDirs.push_back(*ri);
        }
      else if(use_link_rpath)
        {
        // Do not add any path inside the source or build tree.
        const char* topSourceDir = this->Makefile->GetHomeDirectory();
        const char* topBinaryDir = this->Makefile->GetHomeOutputDirectory();
        if(!cmSystemTools::ComparePath(ri->c_str(), topSourceDir) &&
           !cmSystemTools::ComparePath(ri->c_str(), topBinaryDir) &&
           !cmSystemTools::IsSubDirectory(ri->c_str(), topSourceDir) &&
           !cmSystemTools::IsSubDirectory(ri->c_str(), topBinaryDir))
          {
          runtimeDirs.push_back(*ri);
          }
        }
      }
    }

  // Add runtime paths required by the platform to always be
  // present.  This is done even when skipping rpath support.
  cmSystemTools::ExpandListArgument(this->RuntimeAlways.c_str(), runtimeDirs);
}

//----------------------------------------------------------------------------
std::string cmComputeLinkInformation::GetRPathString(bool for_install)
{
  // Get the directories to use.
  std::vector<std::string> runtimeDirs;
  this->GetRPath(runtimeDirs, for_install);

  // Concatenate the paths.
  std::string rpath;
  const char* sep = "";
  for(std::vector<std::string>::const_iterator ri = runtimeDirs.begin();
      ri != runtimeDirs.end(); ++ri)
    {
    // Separate from previous path.
    rpath += sep;
    sep = this->GetRuntimeSep().c_str();

    // Add this path.
    rpath += *ri;
    }

  // If the rpath will be replaced at install time make sure it is
  // long enough now.
  if(!for_install && this->RuntimeUseChrpath)
    {
    std::string::size_type minLength = this->GetChrpathString().length();
    while(rpath.length() < minLength)
      {
      rpath += this->GetRuntimeSep();
      }
    }

  return rpath;
}

//----------------------------------------------------------------------------
std::string cmComputeLinkInformation::GetChrpathString()
{
  if(!this->RuntimeUseChrpath)
    {
    return "";
    }

  return this->GetRPathString(true);
}
