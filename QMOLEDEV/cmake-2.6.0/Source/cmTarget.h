/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile: cmTarget.h,v $
  Language:  C++
  Date:      $Date: 2008-04-08 20:26:07 $
  Version:   $Revision: 1.109.2.3 $

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef cmTarget_h
#define cmTarget_h

#include "cmCustomCommand.h"
#include "cmPropertyMap.h"
#include "cmPolicies.h"

class cmake;
class cmMakefile;
class cmSourceFile;
class cmGlobalGenerator;
class cmComputeLinkInformation;
class cmListFileBacktrace;

struct cmTargetLinkInformationMap:
  public std::map<cmStdString, cmComputeLinkInformation*>
{
  typedef std::map<cmStdString, cmComputeLinkInformation*> derived;
  cmTargetLinkInformationMap() {}
  cmTargetLinkInformationMap(cmTargetLinkInformationMap const& r);
  ~cmTargetLinkInformationMap();
};

struct cmTargetLinkInterface
{
  // Libraries listed in the interface.
  std::vector<std::string> Libraries;

  // Shared library dependencies needed for linking on some platforms.
  std::vector<std::string> SharedDeps;
};

struct cmTargetLinkInterfaceMap:
  public std::map<cmStdString, cmTargetLinkInterface*>
{
  typedef std::map<cmStdString, cmTargetLinkInterface*> derived;
  cmTargetLinkInterfaceMap() {}
  cmTargetLinkInterfaceMap(cmTargetLinkInterfaceMap const& r);
  ~cmTargetLinkInterfaceMap();
};

class cmTargetInternals;
class cmTargetInternalPointer
{
public:
  cmTargetInternalPointer();
  cmTargetInternalPointer(cmTargetInternalPointer const& r);
  ~cmTargetInternalPointer();
  cmTargetInternalPointer& operator=(cmTargetInternalPointer const& r);
  cmTargetInternals* operator->() const { return this->Pointer; }
private:
  cmTargetInternals* Pointer;
};

/** \class cmTarget
 * \brief Represent a library or executable target loaded from a makefile.
 *
 * cmTarget represents a target loaded from
 * a makefile.
 */
class cmTarget
{
public:
  cmTarget();
  enum TargetType { EXECUTABLE, STATIC_LIBRARY,
                    SHARED_LIBRARY, MODULE_LIBRARY, UTILITY, GLOBAL_TARGET,
                    INSTALL_FILES, INSTALL_PROGRAMS, INSTALL_DIRECTORY};
  static const char* TargetTypeNames[];
  enum CustomCommandType { PRE_BUILD, PRE_LINK, POST_BUILD };

  /**
   * Return the type of target.
   */
  TargetType GetType() const
    {
    return this->TargetTypeValue;
    }

  /**
   * Set the target type
   */
  void SetType(TargetType f, const char* name);

  void MarkAsImported();

  ///! Set/Get the name of the target
  const char* GetName() const {return this->Name.c_str();}

  ///! Set the cmMakefile that owns this target
  void SetMakefile(cmMakefile *mf);
  cmMakefile *GetMakefile() const { return this->Makefile;};

  /** Get the status of policy CMP0003 when the target was created.  */
  cmPolicies::PolicyStatus GetPolicyStatusCMP0003() const
    { return this->PolicyStatusCMP0003; }

  /** Get the status of policy CMP0004 when the target was created.  */
  cmPolicies::PolicyStatus GetPolicyStatusCMP0004() const
    { return this->PolicyStatusCMP0004; }

  /**
   * Get the list of the custom commands for this target
   */
  std::vector<cmCustomCommand> &GetPreBuildCommands()
    {return this->PreBuildCommands;}
  std::vector<cmCustomCommand> &GetPreLinkCommands()
    {return this->PreLinkCommands;}
  std::vector<cmCustomCommand> &GetPostBuildCommands()
    {return this->PostBuildCommands;}

  ///! Return the list of frameworks being linked to this target
  std::vector<std::string> &GetFrameworks() {return this->Frameworks;}

  /**
   * Get the list of the source files used by this target
   */
  std::vector<cmSourceFile*> const &GetSourceFiles()
    {return this->SourceFiles;}
  void AddSourceFile(cmSourceFile* sf) { this->SourceFiles.push_back(sf); }

  /**
   * Flags for a given source file as used in this target. Typically assigned
   * via SET_TARGET_PROPERTIES when the property is a list of source files.
   */
  enum SourceFileType
  {
    SourceFileTypeNormal,
    SourceFileTypePrivateHeader, // is in "PRIVATE_HEADER" target property
    SourceFileTypePublicHeader,  // is in "PUBLIC_HEADER" target property
    SourceFileTypeResource,      // is in "RESOURCE" target property *or*
                                 // has MACOSX_PACKAGE_LOCATION=="Resources"
    SourceFileTypeMacContent     // has MACOSX_PACKAGE_LOCATION!="Resources"
  };
  struct SourceFileFlags
  {
    SourceFileFlags(): Type(SourceFileTypeNormal), MacFolder(0) {}
    SourceFileFlags(SourceFileFlags const& r):
      Type(r.Type), MacFolder(r.MacFolder) {}
    SourceFileType Type;
    const char* MacFolder; // location inside Mac content folders
  };

  /**
   * Get the flags for a given source file as used in this target
   */
  struct SourceFileFlags GetTargetSourceFileFlags(const cmSourceFile* sf);

  /**
   * Add sources to the target.
   */
  void AddSources(std::vector<std::string> const& srcs);
  cmSourceFile* AddSource(const char* src);

  /**
   * Get the list of the source files used by this target
   */
  enum LinkLibraryType {GENERAL, DEBUG, OPTIMIZED};

  //* how we identify a library, by name and type
  typedef std::pair<cmStdString, LinkLibraryType> LibraryID;

  typedef std::vector<LibraryID > LinkLibraryVectorType;
  const LinkLibraryVectorType &GetLinkLibraries() const {
  return this->LinkLibraries;}
  const LinkLibraryVectorType &GetOriginalLinkLibraries() const
    {return this->OriginalLinkLibraries;}

  /**
   * Clear the dependency information recorded for this target, if any.
   */
  void ClearDependencyInformation(cmMakefile& mf, const char* target);

  // Check to see if a library is a framework and treat it different on Mac
  bool NameResolvesToFramework(const std::string& libname);
  bool AddFramework(const std::string& lib, LinkLibraryType llt);
  void AddLinkLibrary(cmMakefile& mf,
                      const char *target, const char* lib,
                      LinkLibraryType llt);

  void AddLinkLibrary(const std::string& lib,
                      LinkLibraryType llt);

  void MergeLinkLibraries( cmMakefile& mf, const char* selfname,
                           const LinkLibraryVectorType& libs );

  const std::vector<std::string>& GetLinkDirectories();

  void AddLinkDirectory(const char* d);

  /**
   * Set the path where this target should be installed. This is relative to
   * INSTALL_PREFIX
   */
  std::string GetInstallPath() {return this->InstallPath;}
  void SetInstallPath(const char *name) {this->InstallPath = name;}

  /**
   * Set the path where this target (if it has a runtime part) should be
   * installed. This is relative to INSTALL_PREFIX
   */
  std::string GetRuntimeInstallPath() {return this->RuntimeInstallPath;}
  void SetRuntimeInstallPath(const char *name) {
    this->RuntimeInstallPath = name; }

  /**
   * Get/Set whether there is an install rule for this target.
   */
  bool GetHaveInstallRule() { return this->HaveInstallRule; }
  void SetHaveInstallRule(bool h) { this->HaveInstallRule = h; }

  /** Add a utility on which this project depends. A utility is an executable
   * name as would be specified to the ADD_EXECUTABLE or UTILITY_SOURCE
   * commands. It is not a full path nor does it have an extension.
   */
  void AddUtility(const char* u) { this->Utilities.insert(u);}
  ///! Get the utilities used by this target
  std::set<cmStdString>const& GetUtilities() const { return this->Utilities; }

  void AnalyzeLibDependencies( const cmMakefile& mf );

  ///! Set/Get a property of this target file
  void SetProperty(const char *prop, const char *value);
  void AppendProperty(const char* prop, const char* value);
  const char *GetProperty(const char *prop);
  const char *GetProperty(const char *prop, cmProperty::ScopeType scope);
  bool GetPropertyAsBool(const char *prop);

  bool IsImported() const {return this->IsImportedTarget;}

  /** Get the library interface dependencies.  This is the set of
      libraries from which something that links to this target may
      also receive symbols.  Returns 0 if the user has not specified
      such dependencies or for static libraries.  */
  cmTargetLinkInterface const* GetLinkInterface(const char* config);

  /** Get the directory in which this target will be built.  If the
      configuration name is given then the generator will add its
      subdirectory for that configuration.  Otherwise just the canonical
      output directory is given.  */
  std::string GetDirectory(const char* config = 0, bool implib = false);

  /** Get the location of the target in the build tree for the given
      configuration.  This location is suitable for use as the LOCATION
      target property.  */
  const char* GetLocation(const char* config);

  /** Get the target major and minor version numbers interpreted from
      the VERSION property.  Version 0 is returned if the property is
      not set or cannot be parsed.  */
  void GetTargetVersion(int& major, int& minor);

  /**
   * Trace through the source files in this target and add al source files
   * that they depend on, used by all generators
   */
  void TraceDependencies(const char* vsProjectFile);

  ///! Return the prefered linker language for this target
  const char* GetLinkerLanguage(cmGlobalGenerator*);

  ///! Return the rule variable used to create this type of target,
  //  need to add CMAKE_(LANG) for full name.
  const char* GetCreateRuleVariable();

  /** Get the full name of the target according to the settings in its
      makefile.  */
  std::string GetFullName(const char* config=0, bool implib = false);
  void GetFullNameComponents(std::string& prefix,
                             std::string& base, std::string& suffix,
                             const char* config=0, bool implib = false);

  /** Get the name of the pdb file for the target.  */
  std::string GetPDBName(const char* config=0);

  /** Get the soname of the target.  Allowed only for a shared library.  */
  std::string GetSOName(const char* config);

  /** Test for special case of a third-party shared library that has
      no soname at all.  */
  bool IsImportedSharedLibWithoutSOName(const char* config);

  /** Get the full path to the target according to the settings in its
      makefile and the configuration type.  */
  std::string GetFullPath(const char* config=0, bool implib = false,
                          bool realname = false);

  /** Get the names of the library needed to generate a build rule
      that takes into account shared library version numbers.  This
      should be called only on a library target.  */
  void GetLibraryNames(std::string& name, std::string& soName,
                       std::string& realName, std::string& impName,
                       std::string& pdbName, const char* config);

  /** Get the names of the library used to remove existing copies of
      the library from the build tree either before linking or during
      a clean step.  This should be called only on a library
      target.  */
  void GetLibraryCleanNames(std::string& staticName,
                            std::string& sharedName,
                            std::string& sharedSOName,
                            std::string& sharedRealName,
                            std::string& importName,
                            std::string& pdbName,
                            const char* config);

  /** Get the names of the executable needed to generate a build rule
      that takes into account executable version numbers.  This should
      be called only on an executable target.  */
  void GetExecutableNames(std::string& name, std::string& realName,
                          std::string& impName,
                          std::string& pdbName, const char* config);

  /** Get the names of the executable used to remove existing copies
      of the executable from the build tree either before linking or
      during a clean step.  This should be called only on an
      executable target.  */
  void GetExecutableCleanNames(std::string& name, std::string& realName,
                               std::string& impName,
                               std::string& pdbName, const char* config);

  /** Add the target output files to the global generator manifest.  */
  void GenerateTargetManifest(const char* config);

  /**
   * Compute whether this target must be relinked before installing.
   */
  bool NeedRelinkBeforeInstall();

  bool HaveBuildTreeRPATH();
  bool HaveInstallTreeRPATH();

  /** Return true if builtin chrpath will work for this target */
  bool IsChrpathUsed();

  std::string GetInstallNameDirForBuildTree(const char* config,
                                            bool for_xcode = false);
  std::string GetInstallNameDirForInstallTree(const char* config,
                                              bool for_xcode = false);

  cmComputeLinkInformation* GetLinkInformation(const char* config);

  // Get the properties
  cmPropertyMap &GetProperties() { return this->Properties; };

  // Define the properties
  static void DefineProperties(cmake *cm);

  // Compute the OBJECT_FILES property only when requested
  void ComputeObjectFiles();

  /** Get the macro to define when building sources in this target.
      If no macro should be defined null is returned.  */
  const char* GetExportMacro();

  // Compute the set of languages compiled by the target.  This is
  // computed every time it is called because the languages can change
  // when source file properties are changed and we do not have enough
  // information to forward these property changes to the targets
  // until we have per-target object file properties.
  void GetLanguages(std::set<cmStdString>& languages) const;

  /** Return whether this target is an executable with symbol exports
      enabled.  */
  bool IsExecutableWithExports();

  /** Return whether this target is a shared library Framework on
      Apple.  */
  bool IsFrameworkOnApple();

  /** Return whether this target is an executable Bundle on Apple.  */
  bool IsAppBundleOnApple();

  /** Return the framework version string.  Undefined if
      IsFrameworkOnApple returns false.  */
  std::string GetFrameworkVersion();

  /** Get a backtrace from the creation of the target.  */
  cmListFileBacktrace const& GetBacktrace() const;

private:
  /**
   * A list of direct dependencies. Use in conjunction with DependencyMap.
   */
  typedef std::vector< LibraryID > DependencyList;

  /**
   * This map holds the dependency graph. map[x] returns a set of
   * direct dependencies of x. Note that the direct depenencies are
   * ordered. This is necessary to handle direct dependencies that
   * themselves have no dependency information.
   */
  typedef std::map< LibraryID, DependencyList > DependencyMap;

  /**
   * Inserts \a dep at the end of the dependency list of \a lib.
   */
  void InsertDependency( DependencyMap& depMap,
                         const LibraryID& lib,
                         const LibraryID& dep);

  /*
   * Deletes \a dep from the dependency list of \a lib.
   */
  void DeleteDependency( DependencyMap& depMap,
                         const LibraryID& lib,
                         const LibraryID& dep);

  /**
   * Emits the library \a lib and all its dependencies into link_line.
   * \a emitted keeps track of the libraries that have been emitted to
   * avoid duplicates--it is more efficient than searching
   * link_line. \a visited is used detect cycles. Note that \a
   * link_line is in reverse order, in that the dependencies of a
   * library are listed before the library itself.
   */
  void Emit( const LibraryID lib,
             const DependencyMap& dep_map,
             std::set<LibraryID>& emitted,
             std::set<LibraryID>& visited,
             DependencyList& link_line);

  /**
   * Finds the dependencies for \a lib and inserts them into \a
   * dep_map.
   */
  void GatherDependencies( const cmMakefile& mf,
                           const LibraryID& lib,
                           DependencyMap& dep_map);

  const char* GetSuffixVariableInternal(TargetType type, bool implib);
  const char* GetPrefixVariableInternal(TargetType type, bool implib);
  std::string GetFullNameInternal(TargetType type, const char* config,
                                  bool implib);
  void GetFullNameInternal(TargetType type, const char* config, bool implib,
                           std::string& outPrefix, std::string& outBase,
                           std::string& outSuffix);

  void GetLibraryNamesInternal(std::string& name,
                               std::string& soName,
                               std::string& realName,
                               std::string& impName,
                               std::string& pdbName,
                               TargetType type,
                               const char* config);
  void GetExecutableNamesInternal(std::string& name,
                                  std::string& realName,
                                  std::string& impName,
                                  std::string& pdbName,
                                  TargetType type,
                                  const char* config);

  // Use a makefile variable to set a default for the given property.
  // If the variable is not defined use the given default instead.
  void SetPropertyDefault(const char* property, const char* default_value);

  // Get the full path to the target output directory.
  std::string GetOutputDir(bool implib);
  std::string const& ComputeBaseOutputDir(bool implib);

  const char* ImportedGetLocation(const char* config);
  const char* NormalGetLocation(const char* config);

  std::string GetFullNameImported(const char* config, bool implib);

  std::string ImportedGetFullPath(const char* config, bool implib);
  std::string NormalGetFullPath(const char* config, bool implib,
                                bool realname);

  /** Get the real name of the target.  Allowed only for non-imported
      targets.  When a library or executable file is versioned this is
      the full versioned name.  If the target is not versioned this is
      the same as GetFullName.  */
  std::string NormalGetRealName(const char* config);

private:
  std::string Name;
  std::vector<cmCustomCommand> PreBuildCommands;
  std::vector<cmCustomCommand> PreLinkCommands;
  std::vector<cmCustomCommand> PostBuildCommands;
  TargetType TargetTypeValue;
  std::vector<cmSourceFile*> SourceFiles;
  LinkLibraryVectorType LinkLibraries;
  LinkLibraryVectorType PrevLinkedLibraries;
  bool LinkLibrariesAnalyzed;
  std::vector<std::string> Frameworks;
  std::vector<std::string> LinkDirectories;
  std::set<cmStdString> LinkDirectoriesEmmitted;
  bool HaveInstallRule;
  std::string InstallPath;
  std::string RuntimeInstallPath;
  std::string BaseOutputDir;
  std::string BaseOutputDirImplib;
  std::string Location;
  std::string ExportMacro;
  std::set<cmStdString> Utilities;
  bool RecordDependencies;
  cmPropertyMap Properties;
  LinkLibraryVectorType OriginalLinkLibraries;
  bool DLLPlatform;
  bool IsImportedTarget;

  // Cache import information from properties for each configuration.
  struct ImportInfo
  {
    bool NoSOName;
    std::string Location;
    std::string SOName;
    std::string ImportLibrary;
    cmTargetLinkInterface LinkInterface;
  };
  typedef std::map<cmStdString, ImportInfo> ImportInfoMapType;
  ImportInfoMapType ImportInfoMap;
  ImportInfo const* GetImportInfo(const char* config);
  void ComputeImportInfo(std::string const& desired_config, ImportInfo& info);

  cmTargetLinkInformationMap LinkInformation;

  // Link interface.
  cmTargetLinkInterface* ComputeLinkInterface(const char* config);
  cmTargetLinkInterfaceMap LinkInterface;

  // The cmMakefile instance that owns this target.  This should
  // always be set.
  cmMakefile* Makefile;

  // Policy status recorded when target was created.
  cmPolicies::PolicyStatus PolicyStatusCMP0003;
  cmPolicies::PolicyStatus PolicyStatusCMP0004;

  // Internal representation details.
  friend class cmTargetInternals;
  cmTargetInternalPointer Internal;

  void ConstructSourceFileFlags();
};

typedef std::map<cmStdString,cmTarget> cmTargets;

class cmTargetSet: public std::set<cmStdString> {};
class cmTargetManifest: public std::map<cmStdString, cmTargetSet> {};

#endif
