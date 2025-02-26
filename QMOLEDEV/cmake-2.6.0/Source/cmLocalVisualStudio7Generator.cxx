/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile: cmLocalVisualStudio7Generator.cxx,v $
  Language:  C++
  Date:      $Date: 2008-05-01 16:35:39 $
  Version:   $Revision: 1.217.2.5 $

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cmGlobalVisualStudio7Generator.h"
#include "cmLocalVisualStudio7Generator.h"
#include "cmXMLParser.h"
#include <cm_expat.h>
#include "cmMakefile.h"
#include "cmSystemTools.h"
#include "cmSourceFile.h"
#include "cmCacheManager.h"
#include "cmake.h"

#include "cmComputeLinkInformation.h"
#include "cmGeneratedFileStream.h"

#include <cmsys/System.h>

#include <ctype.h> // for isspace

static bool cmLVS6G_IsFAT(const char* dir);

class cmLocalVisualStudio7GeneratorInternals
{
public:
  cmLocalVisualStudio7GeneratorInternals(cmLocalVisualStudio7Generator* e):
    LocalGenerator(e) {}
  typedef cmComputeLinkInformation::ItemVector ItemVector;
  void OutputLibraries(std::ostream& fout, ItemVector const& libs);
private:
  cmLocalVisualStudio7Generator* LocalGenerator;
};

extern cmVS7FlagTable cmLocalVisualStudio7GeneratorFlagTable[];

//----------------------------------------------------------------------------
cmLocalVisualStudio7Generator::cmLocalVisualStudio7Generator()
{
  this->Version = 7;
  this->PlatformName = "Win32";
  this->ExtraFlagTable = 0;
  this->Internal = new cmLocalVisualStudio7GeneratorInternals(this);
}

cmLocalVisualStudio7Generator::~cmLocalVisualStudio7Generator()
{
  delete this->Internal;
}

void cmLocalVisualStudio7Generator::AddHelperCommands()
{
  std::set<cmStdString> lang;
  lang.insert("C");
  lang.insert("CXX");
  lang.insert("RC");
  lang.insert("IDL");
  lang.insert("DEF");
  lang.insert("Fortran");
  this->CreateCustomTargetsAndCommands(lang);
  this->FixGlobalTargets();
}

void cmLocalVisualStudio7Generator::Generate()
{
  this->WriteProjectFiles();
  this->WriteStampFiles();
}

void cmLocalVisualStudio7Generator::FixGlobalTargets()
{
  // Visual Studio .NET 2003 Service Pack 1 will not run post-build
  // commands for targets in which no sources are built.  Add dummy
  // rules to force these targets to build.
  cmTargets &tgts = this->Makefile->GetTargets();
  for(cmTargets::iterator l = tgts.begin();
      l != tgts.end(); l++)
    {
    cmTarget& tgt = l->second;
    if(tgt.GetType() == cmTarget::GLOBAL_TARGET)
      {
      std::vector<std::string> no_depends;
      cmCustomCommandLine force_command;
      force_command.push_back("cd");
      force_command.push_back(".");
      cmCustomCommandLines force_commands;
      force_commands.push_back(force_command);
      const char* no_main_dependency = 0;
      std::string force = this->Makefile->GetStartOutputDirectory();
      force += cmake::GetCMakeFilesDirectory();
      force += "/";
      force += tgt.GetName();
      force += "_force";
      this->Makefile->AddCustomCommandToOutput(force.c_str(), no_depends,
                                               no_main_dependency,
                                               force_commands, " ", 0, true);
      if(cmSourceFile* file =
         this->Makefile->GetSourceFileWithOutput(force.c_str()))
        {
        tgt.AddSourceFile(file);
        }
      }
    }
}

// TODO
// for CommandLine= need to repleace quotes with &quot
// write out configurations
void cmLocalVisualStudio7Generator::WriteProjectFiles()
{
  // If not an in source build, then create the output directory
  if(strcmp(this->Makefile->GetStartOutputDirectory(),
            this->Makefile->GetHomeDirectory()) != 0)
    {
    if(!cmSystemTools::MakeDirectory
       (this->Makefile->GetStartOutputDirectory()))
      {
      cmSystemTools::Error("Error creating directory ",
                           this->Makefile->GetStartOutputDirectory());
      }
    }

  // Get the set of targets in this directory.
  cmTargets &tgts = this->Makefile->GetTargets();

  // Create the regeneration custom rule.
  if(!this->Makefile->IsOn("CMAKE_SUPPRESS_REGENERATION"))
    {
    // Create a rule to regenerate the build system when the target
    // specification source changes.
    if(cmSourceFile* sf = this->CreateVCProjBuildRule())
      {
      // Add the rule to targets that need it.
      for(cmTargets::iterator l = tgts.begin(); l != tgts.end(); ++l)
        {
        if(l->first != CMAKE_CHECK_BUILD_SYSTEM_TARGET)
          {
          l->second.AddSourceFile(sf);
          }
        }
      }
    }

  // Create the project file for each target.
  for(cmTargets::iterator l = tgts.begin();
      l != tgts.end(); l++)
    {
    // INCLUDE_EXTERNAL_MSPROJECT command only affects the workspace
    // so don't build a projectfile for it
    if (strncmp(l->first.c_str(), "INCLUDE_EXTERNAL_MSPROJECT", 26) != 0)
      {
      this->CreateSingleVCProj(l->first.c_str(),l->second);
      }
    }
}

//----------------------------------------------------------------------------
void cmLocalVisualStudio7Generator::WriteStampFiles()
{
  // Touch a timestamp file used to determine when the project file is
  // out of date.
  std::string stampName = this->Makefile->GetStartOutputDirectory();
  stampName += cmake::GetCMakeFilesDirectory();
  cmSystemTools::MakeDirectory(stampName.c_str());
  stampName += "/";
  stampName += "generate.stamp";
  std::ofstream stamp(stampName.c_str());
  stamp << "# CMake generation timestamp file this directory.\n";

  // Create a helper file so CMake can determine when it is run
  // through the rule created by CreateVCProjBuildRule whether it
  // really needs to regenerate the project.  This file lists its own
  // dependencies.  If any file listed in it is newer than itself then
  // CMake must rerun.  Otherwise the project files are up to date and
  // the stamp file can just be touched.
  std::string depName = stampName;
  depName += ".depend";
  std::ofstream depFile(depName.c_str());
  depFile << "# CMake generation dependency list for this directory.\n";
  std::vector<std::string> const& listFiles = this->Makefile->GetListFiles();
  for(std::vector<std::string>::const_iterator lf = listFiles.begin();
      lf != listFiles.end(); ++lf)
    {
    depFile << *lf << std::endl;
    }
}

//----------------------------------------------------------------------------
void cmLocalVisualStudio7Generator
::CreateSingleVCProj(const char *lname, cmTarget &target)
{
  this->FortranProject =
    static_cast<cmGlobalVisualStudioGenerator*>(this->GlobalGenerator)
    ->TargetIsFortranOnly(target);
  // add to the list of projects
  std::string pname = lname;
  target.SetProperty("GENERATOR_FILE_NAME",lname);
  // create the dsp.cmake file
  std::string fname;
  fname = this->Makefile->GetStartOutputDirectory();
  fname += "/";
  fname += lname;
  if(this->FortranProject)
    {
    fname += ".vfproj";
    }
  else
    {
    fname += ".vcproj";
    }

  // Generate the project file and replace it atomically with
  // copy-if-different.  We use a separate timestamp so that the IDE
  // does not reload project files unnecessarily.
  cmGeneratedFileStream fout(fname.c_str());
  fout.SetCopyIfDifferent(true);
  this->WriteVCProjFile(fout,lname,target);
  if (fout.Close())
    {
    this->GlobalGenerator->FileReplacedDuringGenerate(fname);
    }
}

//----------------------------------------------------------------------------
cmSourceFile* cmLocalVisualStudio7Generator::CreateVCProjBuildRule()
{
  std::string stampName = cmake::GetCMakeFilesDirectoryPostSlash();
  stampName += "generate.stamp";
  const char* dsprule = 
    this->Makefile->GetRequiredDefinition("CMAKE_COMMAND");
  cmCustomCommandLine commandLine;
  commandLine.push_back(dsprule);
  std::string makefileIn = this->Makefile->GetStartDirectory();
  makefileIn += "/";
  makefileIn += "CMakeLists.txt";
  makefileIn = cmSystemTools::CollapseFullPath(makefileIn.c_str());
  std::string comment = "Building Custom Rule ";
  comment += makefileIn;
  std::string args;
  args = "-H";
  args += this->Convert(this->Makefile->GetHomeDirectory(), 
                        START_OUTPUT, UNCHANGED, true);
  commandLine.push_back(args);
  args = "-B";
  args +=
    this->Convert(this->Makefile->GetHomeOutputDirectory(),
                  START_OUTPUT, UNCHANGED, true);
  commandLine.push_back(args);
  commandLine.push_back("--check-stamp-file");
  commandLine.push_back(stampName.c_str());

  std::vector<std::string> const& listFiles = this->Makefile->GetListFiles();

  cmCustomCommandLines commandLines;
  commandLines.push_back(commandLine);
  const char* no_working_directory = 0;
  this->Makefile->AddCustomCommandToOutput(stampName.c_str(), listFiles,
                                           makefileIn.c_str(), commandLines,
                                           comment.c_str(),
                                           no_working_directory, true);
  if(cmSourceFile* file = this->Makefile->GetSource(makefileIn.c_str()))
    {
    return file;
    }
  else
    {
    cmSystemTools::Error("Error adding rule for ", makefileIn.c_str());
    return 0;
    }
}

void cmLocalVisualStudio7Generator::WriteConfigurations(std::ostream& fout,
                                                        const char *libName,
                                                        cmTarget &target)
{
  std::vector<std::string> *configs =
    static_cast<cmGlobalVisualStudio7Generator *>
    (this->GlobalGenerator)->GetConfigurations();

  fout << "\t<Configurations>\n";
  for( std::vector<std::string>::iterator i = configs->begin();
       i != configs->end(); ++i)
    {
    this->WriteConfiguration(fout, i->c_str(), libName, target);
    }
  fout << "\t</Configurations>\n";
}
cmVS7FlagTable cmLocalVisualStudio7GeneratorFortranFlagTable[] =
{ 
  {"Preprocess", "fpp", "Run Preprocessor on files", "preprocessYes", 0}, 
  {"SuppressStartupBanner", "nologo", "SuppressStartupBanner", "true", 0},
  {"DebugInformationFormat", "Zi", "full debug", "debugEnabled", 0},
  {"DebugInformationFormat", "debug:full", "full debug", "debugEnabled", 0},
  {"DebugInformationFormat", "Z7", "c7 compat", "debugOldStyleInfo", 0},
  {"DebugInformationFormat", "Zd", "line numbers", "debugLineInfoOnly", 0},
  {"Optimization", "Od", "disable optimization", "optimizeDisabled", 0},
  {"Optimization", "O1", "min space", "optimizeMinSpace", 0},
  {"Optimization", "O3", "full optimize", "optimizeFull", 0},
  {"GlobalOptimizations", "Og", "global optimize", "true", 0},
  {"InlineFunctionExpansion", "Ob0", "", "expandDisable", 0},
  {"InlineFunctionExpansion", "Ob1", "", "expandOnlyInline", 0},
  {"FavorSizeOrSpeed", "Os", "", "favorSize", 0},
  {"OmitFramePointers", "Oy-", "", "false", 0},
  {"OptimizeForProcessor", "GB", "", "procOptimizeBlended", 0},
  {"OptimizeForProcessor", "G5", "", "procOptimizePentium", 0},
  {"OptimizeForProcessor", "G6", "", "procOptimizePentiumProThruIII", 0},
  {"UseProcessorExtensions", "QzxK", "", "codeForStreamingSIMD", 0},
  {"OptimizeForProcessor", "QaxN", "", "codeForPentium4", 0},
  {"OptimizeForProcessor", "QaxB", "", "codeForPentiumM", 0},
  {"OptimizeForProcessor", "QaxP", "", "codeForCodeNamedPrescott", 0},
  {"OptimizeForProcessor", "QaxT", "", "codeForCore2Duo", 0},
  {"OptimizeForProcessor", "QxK", "", "codeExclusivelyStreamingSIMD", 0},
  {"OptimizeForProcessor", "QxN", "", "codeExclusivelyPentium4", 0},
  {"OptimizeForProcessor", "QxB", "", "codeExclusivelyPentiumM", 0},
  {"OptimizeForProcessor", "QxP", "", "codeExclusivelyCodeNamedPrescott", 0},
  {"OptimizeForProcessor", "QxT", "", "codeExclusivelyCore2Duo", 0},
  {"OptimizeForProcessor", "QxO", "", "codeExclusivelyCore2StreamingSIMD", 0},
  {"OptimizeForProcessor", "QxS", "", "codeExclusivelyCore2StreamingSIMD4", 0},

  {"ModulePath", "module:", "", "", 
   cmVS7FlagTable::UserValueRequired},
  {"LoopUnrolling", "Qunroll:", "", "", 
   cmVS7FlagTable::UserValueRequired},
  {"AutoParallelThreshold", "Qpar-threshold:", "", "", 
   cmVS7FlagTable::UserValueRequired},
  {"HeapArrays", "heap-arrays:", "", "", 
   cmVS7FlagTable::UserValueRequired},
  {"ObjectText", "bintext:", "", "", 
   cmVS7FlagTable::UserValueRequired},
  {"Parallelization", "Qparallel", "", "true", 0},
  {"PrefetchInsertion", "Qprefetch-", "", "false", 0},
  {"BufferedIO", "assume:buffered_io", "", "true", 0},
  {"CallingConvention", "iface:stdcall", "", "callConventionStdCall", 0},
  {"CallingConvention", "iface:cref", "", "callConventionCRef", 0},
  {"CallingConvention", "iface:stdref", "", "callConventionStdRef", 0},
  {"CallingConvention", "iface:stdcall", "", "callConventionStdCall", 0},
  {"CallingConvention", "iface:cvf", "", "callConventionCVF", 0},
  {"EnableRecursion", "recursive", "", "true", 0},
  {"ReentrantCode", "reentrancy", "", "true", 0},
  // done up to Language
  {0,0,0,0,0}
};
// fill the table here currently the comment field is not used for
// anything other than documentation NOTE: Make sure the longer
// commandFlag comes FIRST!
cmVS7FlagTable cmLocalVisualStudio7GeneratorFlagTable[] =
{
  // option flags (some flags map to the same option)
  {"BasicRuntimeChecks", "GZ", "Stack frame checks", "1", 0},
  {"BasicRuntimeChecks", "RTCsu",
   "Both stack and uninitialized checks", "3", 0},
  {"BasicRuntimeChecks", "RTCs", "Stack frame checks", "1", 0},
  {"BasicRuntimeChecks", "RTCu", "Uninitialized Variables ", "2", 0},
  {"BasicRuntimeChecks", "RTC1",
   "Both stack and uninitialized checks", "3", 0},
  {"DebugInformationFormat", "Z7", "debug format", "1", 0},
  {"DebugInformationFormat", "Zd", "debug format", "2", 0},
  {"DebugInformationFormat", "Zi", "debug format", "3", 0},
  {"DebugInformationFormat", "ZI", "debug format", "4", 0},
  {"EnableEnhancedInstructionSet", "arch:SSE2",
   "Use sse2 instructions", "2", 0},
  {"EnableEnhancedInstructionSet", "arch:SSE",
   "Use sse instructions",   "1", 0},
  {"FavorSizeOrSpeed",  "Ot", "Favor fast code",  "1", 0},
  {"FavorSizeOrSpeed",  "Os", "Favor small code", "2", 0},
  {"CompileAs", "TC", "Compile as c code",        "1", 0},
  {"CompileAs", "TP", "Compile as c++ code",      "2", 0},
  {"Optimization", "Od", "Non Debug",        "0", 0},
  {"Optimization", "O1", "Min Size",         "1", 0},
  {"Optimization", "O2", "Max Speed",        "2", 0},
  {"Optimization", "Ox", "Max Optimization", "3", 0},
  {"OptimizeForProcessor", "GB", "Blended processor mode", "0", 0},
  {"OptimizeForProcessor", "G5", "Pentium",                "1", 0},
  {"OptimizeForProcessor", "G6", "PPro PII PIII",          "2", 0},
  {"OptimizeForProcessor", "G7", "Pentium 4 or Athlon",    "3", 0},
  {"InlineFunctionExpansion", "Ob0", "no inlines",              "0", 0},
  {"InlineFunctionExpansion", "Ob1", "when inline keyword",     "1", 0},
  {"InlineFunctionExpansion", "Ob2", "any time you can inline", "2", 0},
  {"RuntimeLibrary", "MTd", "Multithreded debug",     "1", 0},
  {"RuntimeLibrary", "MT", "Multithreded", "0", 0},
  {"RuntimeLibrary", "MDd", "Multithreded dll debug", "3", 0},
  {"RuntimeLibrary", "MD", "Multithreded dll",        "2", 0},
  {"RuntimeLibrary", "MLd", "Sinble Thread debug",    "5", 0},
  {"RuntimeLibrary", "ML", "Sinble Thread",           "4", 0},
  {"StructMemberAlignment", "Zp16", "struct align 16 byte ",   "5", 0},
  {"StructMemberAlignment", "Zp1", "struct align 1 byte ",     "1", 0},
  {"StructMemberAlignment", "Zp2", "struct align 2 byte ",     "2", 0},
  {"StructMemberAlignment", "Zp4", "struct align 4 byte ",     "3", 0},
  {"StructMemberAlignment", "Zp8", "struct align 8 byte ",     "4", 0},
  {"WarningLevel", "W1", "Warning level", "1", 0},
  {"WarningLevel", "W2", "Warning level", "2", 0},
  {"WarningLevel", "W3", "Warning level", "3", 0},
  {"WarningLevel", "W4", "Warning level", "4", 0},

  // Precompiled header and related options.  Note that the
  // UsePrecompiledHeader entries are marked as "Continue" so that the
  // corresponding PrecompiledHeaderThrough entry can be found.
  {"UsePrecompiledHeader", "Yc", "Create Precompiled Header", "1",
   cmVS7FlagTable::UserValueIgnored | cmVS7FlagTable::Continue},
  {"PrecompiledHeaderThrough", "Yc", "Precompiled Header Name", "",
   cmVS7FlagTable::UserValueRequired},
  {"PrecompiledHeaderFile", "Fp", "Generated Precompiled Header", "",
   cmVS7FlagTable::UserValue},
  // The YX and Yu options are in a per-global-generator table because
  // their values differ based on the VS IDE version.
  {"ForcedIncludeFiles", "FI", "Forced include files", "",
   cmVS7FlagTable::UserValueRequired},

  // boolean flags
  {"BufferSecurityCheck", "GS", "Buffer security check", "TRUE", 0},
  {"BufferSecurityCheck", "GS-", "Turn off Buffer security check", "FALSE", 0},
  {"Detect64BitPortabilityProblems", "Wp64", 
   "Detect 64-bit Portability Problems", "TRUE", 0},
  {"EnableFiberSafeOptimization", "GT", "Enable Fiber-safe Optimizations",
   "TRUE", 0},
  {"EnableFunctionLevelLinking", "Gy",
   "EnableFunctionLevelLinking", "TRUE", 0},
  {"EnableIntrinsicFunctions", "Oi", "EnableIntrinsicFunctions", "TRUE", 0},
  {"GlobalOptimizations", "Og", "Global Optimize", "TRUE", 0},
  {"ImproveFloatingPointConsistency", "Op",
   "ImproveFloatingPointConsistency", "TRUE", 0},
  {"MinimalRebuild", "Gm", "minimal rebuild", "TRUE", 0},
  {"OmitFramePointers", "Oy", "OmitFramePointers", "TRUE", 0},
  {"OptimizeForWindowsApplication", "GA", "Optimize for windows", "TRUE", 0},
  {"RuntimeTypeInfo", "GR",
   "Turn on Run time type information for c++", "TRUE", 0},
  {"RuntimeTypeInfo", "GR-",
   "Turn off Run time type information for c++", "FALSE", 0},
  {"SmallerTypeCheck", "RTCc", "smaller type check", "TRUE", 0},
  {"SuppressStartupBanner", "nologo", "SuppressStartupBanner", "TRUE", 0},
  {"WarnAsError", "WX", "Treat warnings as errors", "TRUE", 0},
  {0,0,0,0,0}
};



cmVS7FlagTable cmLocalVisualStudio7GeneratorLinkFlagTable[] =
{
  // option flags (some flags map to the same option)
  {"GenerateManifest", "MANIFEST:NO",
   "disable manifest generation", "FALSE", 0},
  {"GenerateManifest", "MANIFEST", "enable manifest generation", "TRUE", 0},
  {"LinkIncremental", "INCREMENTAL:NO", "link incremental", "1", 0},
  {"LinkIncremental", "INCREMENTAL:YES", "link incremental", "2", 0},
  {"IgnoreDefaultLibraryNames", "NODEFAULTLIB:", "default libs to ignore", "",
  cmVS7FlagTable::UserValue | cmVS7FlagTable::SemicolonAppendable},
  {"IgnoreAllDefaultLibraries", "NODEFAULTLIB", "ignore all default libs",
   "TRUE", 0},
  {"ModuleDefinitionFile", "DEF:", "add an export def file", "",
   cmVS7FlagTable::UserValue},
  {0,0,0,0,0}
};

//----------------------------------------------------------------------------
class cmLocalVisualStudio7GeneratorOptions
{
public:
  // Construct an options table for a given tool.
  enum Tool
  {
    Compiler,
    Linker,
    FortranCompiler
  };
  cmLocalVisualStudio7GeneratorOptions(cmLocalVisualStudio7Generator* lg,
                                       int version,
                                       Tool tool,
                                       cmVS7FlagTable const* extraTable = 0);

  // Store options from command line flags.
  void Parse(const char* flags);

  // Fix the ExceptionHandling option to default to off.
  void FixExceptionHandlingDefault();

  // Store options for verbose builds.
  void SetVerboseMakefile(bool verbose);

  // Store definitions and flags.
  void AddDefine(const std::string& define);
  void AddDefines(const char* defines);
  void AddFlag(const char* flag, const char* value);

  // Check for specific options.
  bool UsingUnicode();

  bool IsDebug();
  // Write options to output.
  void OutputPreprocessorDefinitions(std::ostream& fout,
                                     const char* prefix,
                                     const char* suffix);
  void OutputFlagMap(std::ostream& fout, const char* indent);
  void OutputAdditionalOptions(std::ostream& fout,
                               const char* prefix,
                               const char* suffix);

private:
  cmLocalVisualStudio7Generator* LocalGenerator;
  int Version;

  // create a map of xml tags to the values they should have in the output
  // for example, "BufferSecurityCheck" = "TRUE"
  // first fill this table with the values for the configuration
  // Debug, Release, etc,
  // Then parse the command line flags specified in CMAKE_CXX_FLAGS
  // and CMAKE_C_FLAGS
  // and overwrite or add new values to this map
  std::map<cmStdString, cmStdString> FlagMap;

  // Preprocessor definitions.
  std::vector<std::string> Defines;

  // Unrecognized flags that get no special handling.
  cmStdString FlagString;

  Tool CurrentTool;
  bool DoingDefine;
  cmVS7FlagTable const* FlagTable;
  cmVS7FlagTable const* ExtraFlagTable;
  void HandleFlag(const char* flag);
  bool CheckFlagTable(cmVS7FlagTable const* table, const char* flag,
                      bool& flag_handled);
};

void cmLocalVisualStudio7Generator::WriteConfiguration(std::ostream& fout,
                                                       const char* configName,
                                                       const char *libName,
                                                       cmTarget &target)
{
  const char* mfcFlag = this->Makefile->GetDefinition("CMAKE_MFC_FLAG");
  if(!mfcFlag)
    {
    mfcFlag = "0";
    }
  fout << "\t\t<Configuration\n"
       << "\t\t\tName=\"" << configName << "|" << this->PlatformName << "\"\n"
       << "\t\t\tOutputDirectory=\"" << configName << "\"\n";
  // This is an internal type to Visual Studio, it seems that:
  // 4 == static library
  // 2 == dll
  // 1 == executable
  // 10 == utility
  const char* configType = "10";
  const char* projectType = 0;
  switch(target.GetType())
    {
    case cmTarget::STATIC_LIBRARY:
      projectType = "typeStaticLibrary";
      configType = "4";
      break;
    case cmTarget::SHARED_LIBRARY:
    case cmTarget::MODULE_LIBRARY:
      projectType = "typeDynamicLibrary";
      configType = "2";
      break;
    case cmTarget::EXECUTABLE:
      configType = "1";
      break;
    case cmTarget::UTILITY:
    case cmTarget::GLOBAL_TARGET:
      configType = "10";
    default:
      break;
    }
  if(this->FortranProject && projectType)
    {
    configType = projectType;
    }
  std::string flags;
  if(strcmp(configType, "10") != 0)
    {
    const char* linkLanguage = 
      target.GetLinkerLanguage(this->GetGlobalGenerator());
    if(!linkLanguage)
      {
      cmSystemTools::Error
        ("CMake can not determine linker language for target:",
         target.GetName());
      return;
      }
    if(strcmp(linkLanguage, "C") == 0 || strcmp(linkLanguage, "CXX") == 0
      || strcmp(linkLanguage, "Fortran") == 0)
      {
      std::string baseFlagVar = "CMAKE_";
      baseFlagVar += linkLanguage;
      baseFlagVar += "_FLAGS";
      flags = this->Makefile->GetRequiredDefinition(baseFlagVar.c_str());
      std::string flagVar = baseFlagVar + std::string("_") +
        cmSystemTools::UpperCase(configName);
      flags += " ";
      flags += this->Makefile->GetRequiredDefinition(flagVar.c_str());
      }
    // set the correct language
    if(strcmp(linkLanguage, "C") == 0)
      {
      flags += " /TC ";
      }
    if(strcmp(linkLanguage, "CXX") == 0)
      {
      flags += " /TP ";
      }
    }

  // Add the target-specific flags.
  if(const char* targetFlags = target.GetProperty("COMPILE_FLAGS"))
    {
    flags += " ";
    flags += targetFlags;
    }

  std::string configUpper = cmSystemTools::UpperCase(configName);
  std::string defPropName = "COMPILE_DEFINITIONS_";
  defPropName += configUpper;

  // Get preprocessor definitions for this directory.
  std::string defineFlags = this->Makefile->GetDefineFlags();
  Options::Tool t = Options::Compiler;
  if(this->FortranProject)
    {
    t = Options::FortranCompiler;
    }
  Options targetOptions(this, this->Version, t, this->ExtraFlagTable);
  targetOptions.FixExceptionHandlingDefault();
  targetOptions.Parse(flags.c_str());
  targetOptions.Parse(defineFlags.c_str());
  targetOptions.AddDefines
    (this->Makefile->GetProperty("COMPILE_DEFINITIONS"));
  targetOptions.AddDefines(target.GetProperty("COMPILE_DEFINITIONS"));
  targetOptions.AddDefines(this->Makefile->GetProperty(defPropName.c_str()));
  targetOptions.AddDefines(target.GetProperty(defPropName.c_str()));
  targetOptions.SetVerboseMakefile(
    this->Makefile->IsOn("CMAKE_VERBOSE_MAKEFILE"));

  // Add a definition for the configuration name.
  std::string configDefine = "CMAKE_INTDIR=\"";
  configDefine += configName;
  configDefine += "\"";
  targetOptions.AddDefine(configDefine);

  // Add the export symbol definition for shared library objects.
  if(const char* exportMacro = target.GetExportMacro())
    {
    targetOptions.AddDefine(exportMacro);
    }

  // The intermediate directory name consists of a directory for the
  // target and a subdirectory for the configuration name.
  std::string intermediateDir = this->GetTargetDirectory(target);
  intermediateDir += "/";
  intermediateDir += configName;
  fout << "\t\t\tIntermediateDirectory=\""
       << this->ConvertToXMLOutputPath(intermediateDir.c_str())
       << "\"\n"
       << "\t\t\tConfigurationType=\"" << configType << "\"\n"
       << "\t\t\tUseOfMFC=\"" << mfcFlag << "\"\n"
       << "\t\t\tATLMinimizesCRunTimeLibraryUsage=\"FALSE\"\n";

  // If unicode is enabled change the character set to unicode, if not
  // then default to MBCS.
  if(targetOptions.UsingUnicode())
    {
    fout << "\t\t\tCharacterSet=\"1\">\n";
    }
  else
    {
    fout << "\t\t\tCharacterSet=\"2\">\n";
    }
  const char* tool = "VCCLCompilerTool";
  if(this->FortranProject)
    {
    tool = "VFFortranCompilerTool";
    }
  fout << "\t\t\t<Tool\n"
       << "\t\t\t\tName=\"" << tool << "\"\n";
  if(this->FortranProject)
    {
    const char* target_mod_dir =
      target.GetProperty("Fortran_MODULE_DIRECTORY"); 
    std::string modDir;
    if(target_mod_dir)
      {
      modDir = this->Convert(target_mod_dir,
                             cmLocalGenerator::START_OUTPUT,
                             cmLocalGenerator::UNCHANGED);
      }
    else
      {
      modDir = ".";
      }
    fout << "\t\t\t\tModulePath=\"" 
         << this->ConvertToXMLOutputPath(modDir.c_str()) << "\\$(ConfigurationName)\"\n";
    }
  targetOptions.OutputAdditionalOptions(fout, "\t\t\t\t", "\n");
  fout << "\t\t\t\tAdditionalIncludeDirectories=\"";
  std::vector<std::string> includes;
  this->GetIncludeDirectories(includes);
  std::vector<std::string>::iterator i = includes.begin();
  for(;i != includes.end(); ++i)
    {
    // output the include path
    std::string ipath = this->ConvertToXMLOutputPath(i->c_str());
    fout << ipath << ";";
    // if this is fortran then output the include with 
    // a ConfigurationName on the end of it.
    if(this->FortranProject)
      {
      ipath = i->c_str();
      ipath += "/$(ConfigurationName)";
      ipath = this->ConvertToXMLOutputPath(ipath.c_str());
      fout << ipath << ";";
      }
    }
  fout << "\"\n";
  targetOptions.OutputFlagMap(fout, "\t\t\t\t");
  targetOptions.OutputPreprocessorDefinitions(fout, "\t\t\t\t", "\n");
  fout << "\t\t\t\tAssemblerListingLocation=\"" << configName << "\"\n";
  fout << "\t\t\t\tObjectFile=\"$(IntDir)\\\"\n";
  if(target.GetType() == cmTarget::EXECUTABLE ||
     target.GetType() == cmTarget::STATIC_LIBRARY ||
     target.GetType() == cmTarget::SHARED_LIBRARY ||
     target.GetType() == cmTarget::MODULE_LIBRARY)
    {
    // We need to specify a program database file name even for
    // non-debug configurations because VS still creates .idb files.
    fout <<  "\t\t\t\tProgramDataBaseFileName=\""
         << target.GetDirectory(configName) << "/"
         << target.GetPDBName(configName) << "\"\n";
    }
  fout << "/>\n";  // end of <Tool Name=VCCLCompilerTool
  tool = "VCCustomBuildTool";
  if(this->FortranProject)
    {
    tool = "VFCustomBuildTool";
    }
  fout << "\t\t\t<Tool\n\t\t\t\tName=\"" << tool << "\"/>\n";
  tool = "VCResourceCompilerTool";
  if(this->FortranProject)
    {
    tool = "VFResourceCompilerTool";
    }
  fout << "\t\t\t<Tool\n\t\t\t\tName=\"" << tool << "\"\n"
       << "\t\t\t\tAdditionalIncludeDirectories=\"";
  for(i = includes.begin();i != includes.end(); ++i)
    {
    std::string ipath = this->ConvertToXMLOutputPath(i->c_str());
    fout << ipath << ";";
    }
  // add the -D flags to the RC tool
  fout << "\"";
  targetOptions.OutputPreprocessorDefinitions(fout, "\n\t\t\t\t", "");
  fout << "/>\n";
  tool = "VCMIDLTool";
  if(this->FortranProject)
    {
    tool = "VFMIDLTool";
    }
  fout << "\t\t\t<Tool\n\t\t\t\tName=\"" << tool << "\"\n";
  targetOptions.OutputPreprocessorDefinitions(fout, "\t\t\t\t", "\n");
  fout << "\t\t\t\tMkTypLibCompatible=\"FALSE\"\n";
  if( this->PlatformName == "x64" )
    {
    fout << "\t\t\t\tTargetEnvironment=\"3\"\n";
    }
  else if( this->PlatformName == "ia64" )
    {
    fout << "\t\t\t\tTargetEnvironment=\"2\"\n";
    }
  else
    {
    fout << "\t\t\t\tTargetEnvironment=\"1\"\n";
    }
  fout << "\t\t\t\tGenerateStublessProxies=\"TRUE\"\n";
  fout << "\t\t\t\tTypeLibraryName=\"$(InputName).tlb\"\n";
  fout << "\t\t\t\tOutputDirectory=\"$(IntDir)\"\n";
  fout << "\t\t\t\tHeaderFileName=\"$(InputName).h\"\n";
  fout << "\t\t\t\tDLLDataFileName=\"\"\n";
  fout << "\t\t\t\tInterfaceIdentifierFileName=\"$(InputName)_i.c\"\n";
  fout << "\t\t\t\tProxyFileName=\"$(InputName)_p.c\"/>\n";
  // end of <Tool Name=VCMIDLTool

  // Check if we need the FAT32 workaround.
  if ( this->Version >= 8 )
    {
    // Check the filesystem type where the target will be written.
    if(cmLVS6G_IsFAT(target.GetDirectory(configName).c_str()))
      {
      // Add a flag telling the manifest tool to use a workaround
      // for FAT32 file systems, which can cause an empty manifest
      // to be embedded into the resulting executable.  See CMake
      // bug #2617. 
      const char* tool  = "VCManifestTool";
      if(this->FortranProject)
        {
        tool = "VFManifestTool";
        }
      fout << "\t\t\t<Tool\n\t\t\t\tName=\"" << tool << "\"\n"
           << "\t\t\t\tUseFAT32Workaround=\"true\"\n"
           << "\t\t\t/>\n";
      }
    }

  this->OutputTargetRules(fout, configName, target, libName);
  this->OutputBuildTool(fout, configName, target, targetOptions.IsDebug());
  fout << "\t\t</Configuration>\n";
}

//----------------------------------------------------------------------------
std::string
cmLocalVisualStudio7Generator
::GetBuildTypeLinkerFlags(std::string rootLinkerFlags, const char* configName)
{
  std::string configTypeUpper = cmSystemTools::UpperCase(configName);
  std::string extraLinkOptionsBuildTypeDef = 
    rootLinkerFlags + "_" + configTypeUpper;

  std::string extraLinkOptionsBuildType =
    this->Makefile->GetRequiredDefinition
    (extraLinkOptionsBuildTypeDef.c_str());

  return extraLinkOptionsBuildType;
}

void cmLocalVisualStudio7Generator::OutputBuildTool(std::ostream& fout,
                                                    const char* configName,
                                                    cmTarget &target,
                                                    bool isDebug)
{
  std::string temp;
  std::string extraLinkOptions;
  if(target.GetType() == cmTarget::EXECUTABLE)
    {
    extraLinkOptions = 
      this->Makefile->GetRequiredDefinition("CMAKE_EXE_LINKER_FLAGS") 
      + std::string(" ") 
      + GetBuildTypeLinkerFlags("CMAKE_EXE_LINKER_FLAGS", configName);
    }
  if(target.GetType() == cmTarget::SHARED_LIBRARY)
    {
    extraLinkOptions = 
      this->Makefile->GetRequiredDefinition("CMAKE_SHARED_LINKER_FLAGS") 
      + std::string(" ") 
      + GetBuildTypeLinkerFlags("CMAKE_SHARED_LINKER_FLAGS", configName);
    }
  if(target.GetType() == cmTarget::MODULE_LIBRARY)
    {
    extraLinkOptions = 
      this->Makefile->GetRequiredDefinition("CMAKE_MODULE_LINKER_FLAGS") 
      + std::string(" ") 
      + GetBuildTypeLinkerFlags("CMAKE_MODULE_LINKER_FLAGS", configName);
    }

  const char* targetLinkFlags = target.GetProperty("LINK_FLAGS");
  if(targetLinkFlags)
    {
    extraLinkOptions += " ";
    extraLinkOptions += targetLinkFlags;
    }
  std::string configTypeUpper = cmSystemTools::UpperCase(configName);
  std::string linkFlagsConfig = "LINK_FLAGS_";
  linkFlagsConfig += configTypeUpper;
  targetLinkFlags = target.GetProperty(linkFlagsConfig.c_str());
  if(targetLinkFlags)
    {
    extraLinkOptions += " ";
    extraLinkOptions += targetLinkFlags;
    }
  Options linkOptions(this, this->Version, Options::Linker);
  linkOptions.Parse(extraLinkOptions.c_str());
  switch(target.GetType())
    {
    case cmTarget::STATIC_LIBRARY:
    {
    std::string targetNameFull = target.GetFullName(configName);
    std::string libpath = target.GetDirectory(configName);
    libpath += "/";
    libpath += targetNameFull;
    const char* tool = "VCLibrarianTool";
    if(this->FortranProject)
      {
      tool = "VFLibrarianTool";
      }
    fout << "\t\t\t<Tool\n"
         << "\t\t\t\tName=\"" << tool << "\"\n";
    if(const char* libflags = target.GetProperty("STATIC_LIBRARY_FLAGS"))
      {
      fout << "\t\t\t\tAdditionalOptions=\"" << libflags << "\"\n";
      }
    fout << "\t\t\t\tOutputFile=\""
         << this->ConvertToXMLOutputPathSingle(libpath.c_str()) << "\"/>\n";
    break;
    }
    case cmTarget::SHARED_LIBRARY:
    case cmTarget::MODULE_LIBRARY:
    {
    std::string targetName;
    std::string targetNameSO;
    std::string targetNameFull;
    std::string targetNameImport;
    std::string targetNamePDB;
    target.GetLibraryNames(targetName, targetNameSO, targetNameFull,
                           targetNameImport, targetNamePDB, configName);

    // Compute the link library and directory information.
    cmComputeLinkInformation* pcli = target.GetLinkInformation(configName);
    if(!pcli)
      {
      return;
      }
    cmComputeLinkInformation& cli = *pcli;
    const char* linkLanguage = cli.GetLinkLanguage();

    // Compute the variable name to lookup standard libraries for this
    // language.
    std::string standardLibsVar = "CMAKE_";
    standardLibsVar += linkLanguage;
    standardLibsVar += "_STANDARD_LIBRARIES";
    const char* tool = "VCLinkerTool";
    if(this->FortranProject)
      {
      tool = "VFLinkerTool";
      }
    fout << "\t\t\t<Tool\n"
         << "\t\t\t\tName=\"" << tool << "\"\n";
    linkOptions.OutputAdditionalOptions(fout, "\t\t\t\t", "\n");
    // Use the NOINHERIT macro to avoid getting VS project default
    // libraries which may be set by the user to something bad.
    fout << "\t\t\t\tAdditionalDependencies=\"$(NOINHERIT) "
         << this->Makefile->GetSafeDefinition(standardLibsVar.c_str())
         << " ";
    this->Internal->OutputLibraries(fout, cli.GetItems());
    fout << "\"\n";
    temp = target.GetDirectory(configName);
    temp += "/";
    temp += targetNameFull;
    fout << "\t\t\t\tOutputFile=\""
         << this->ConvertToXMLOutputPathSingle(temp.c_str()) << "\"\n";
    this->WriteTargetVersionAttribute(fout, target);
    linkOptions.OutputFlagMap(fout, "\t\t\t\t");
    fout << "\t\t\t\tAdditionalLibraryDirectories=\"";
    this->OutputLibraryDirectories(fout, cli.GetDirectories());
    fout << "\"\n";
    this->OutputModuleDefinitionFile(fout, target);
    temp = target.GetDirectory(configName);
    temp += "/";
    temp += targetNamePDB;
    fout << "\t\t\t\tProgramDataBaseFile=\"" <<
      this->ConvertToXMLOutputPathSingle(temp.c_str()) << "\"\n";
    if(isDebug)
      {
      fout << "\t\t\t\tGenerateDebugInformation=\"TRUE\"\n";
      }
    std::string stackVar = "CMAKE_";
    stackVar += linkLanguage;
    stackVar += "_STACK_SIZE";
    const char* stackVal = this->Makefile->GetDefinition(stackVar.c_str());
    if(stackVal)
      {
      fout << "\t\t\t\tStackReserveSize=\"" << stackVal  << "\"\n";
      }
    temp = target.GetDirectory(configName, true);
    temp += "/";
    temp += targetNameImport;
    fout << "\t\t\t\tImportLibrary=\""
         << this->ConvertToXMLOutputPathSingle(temp.c_str()) << "\"/>\n";
    }
    break;
    case cmTarget::EXECUTABLE:
    {
    std::string targetName;
    std::string targetNameFull;
    std::string targetNameImport;
    std::string targetNamePDB;
    target.GetExecutableNames(targetName, targetNameFull,
                              targetNameImport, targetNamePDB, configName);

    // Compute the link library and directory information.
    cmComputeLinkInformation* pcli = target.GetLinkInformation(configName);
    if(!pcli)
      {
      return;
      }
    cmComputeLinkInformation& cli = *pcli;
    const char* linkLanguage = cli.GetLinkLanguage();

    // Compute the variable name to lookup standard libraries for this
    // language.
    std::string standardLibsVar = "CMAKE_";
    standardLibsVar += linkLanguage;
    standardLibsVar += "_STANDARD_LIBRARIES";
    const char* tool = "VCLinkerTool";
    if(this->FortranProject)
      {
      tool = "VFLinkerTool";
      }
    fout << "\t\t\t<Tool\n"
         << "\t\t\t\tName=\"" << tool << "\"\n";
    linkOptions.OutputAdditionalOptions(fout, "\t\t\t\t", "\n");
    // Use the NOINHERIT macro to avoid getting VS project default
    // libraries which may be set by the user to something bad.
    fout << "\t\t\t\tAdditionalDependencies=\"$(NOINHERIT) "
         << this->Makefile->GetSafeDefinition(standardLibsVar.c_str())
         << " ";
    this->Internal->OutputLibraries(fout, cli.GetItems());
    fout << "\"\n";
    temp = target.GetDirectory(configName);
    temp += "/";
    temp += targetNameFull;
    fout << "\t\t\t\tOutputFile=\"" 
         << this->ConvertToXMLOutputPathSingle(temp.c_str()) << "\"\n";
    this->WriteTargetVersionAttribute(fout, target);
    linkOptions.OutputFlagMap(fout, "\t\t\t\t");
    fout << "\t\t\t\tAdditionalLibraryDirectories=\"";
    this->OutputLibraryDirectories(fout, cli.GetDirectories());
    fout << "\"\n";
    fout << "\t\t\t\tProgramDataBaseFile=\""
         << target.GetDirectory(configName) << "/" << targetNamePDB
         << "\"\n";
    if(isDebug)
      {
      fout << "\t\t\t\tGenerateDebugInformation=\"TRUE\"\n";
      }
    if ( target.GetPropertyAsBool("WIN32_EXECUTABLE") )
      {
      fout << "\t\t\t\tSubSystem=\"2\"\n";
      }
    else
      {
      fout << "\t\t\t\tSubSystem=\"1\"\n";
      }
    std::string stackVar = "CMAKE_";
    stackVar += linkLanguage;
    stackVar += "_STACK_SIZE";
    const char* stackVal = this->Makefile->GetDefinition(stackVar.c_str());
    if(stackVal)
      {
      fout << "\t\t\t\tStackReserveSize=\"" << stackVal << "\"";
      }
    temp = target.GetDirectory(configName, true);
    temp += "/";
    temp += targetNameImport;
    fout << "\t\t\t\tImportLibrary=\""
         << this->ConvertToXMLOutputPathSingle(temp.c_str()) << "\"/>\n";
    break;
    }
    case cmTarget::UTILITY:
    case cmTarget::GLOBAL_TARGET:
      break;
    }
}

//----------------------------------------------------------------------------
void
cmLocalVisualStudio7Generator
::WriteTargetVersionAttribute(std::ostream& fout, cmTarget& target)
{
  int major;
  int minor;
  target.GetTargetVersion(major, minor);
  fout << "\t\t\t\tVersion=\"" << major << "." << minor << "\"\n";
}

void cmLocalVisualStudio7Generator
::OutputModuleDefinitionFile(std::ostream& fout,
                             cmTarget &target)
{
  std::vector<cmSourceFile*> const& classes = target.GetSourceFiles();
  for(std::vector<cmSourceFile*>::const_iterator i = classes.begin();
      i != classes.end(); i++)
    {
    cmSourceFile* sf = *i;
    if(cmSystemTools::UpperCase(sf->GetExtension()) == "DEF")
      {
      fout << "\t\t\t\tModuleDefinitionFile=\""
           << this->ConvertToXMLOutputPath(sf->GetFullPath().c_str())
           << "\"\n";
      return;
      }
    }

}

//----------------------------------------------------------------------------
void
cmLocalVisualStudio7GeneratorInternals
::OutputLibraries(std::ostream& fout, ItemVector const& libs)
{
  cmLocalVisualStudio7Generator* lg = this->LocalGenerator;
  for(ItemVector::const_iterator l = libs.begin(); l != libs.end(); ++l)
    {
    if(l->IsPath)
      {
      std::string rel = lg->Convert(l->Value.c_str(),
                                    cmLocalGenerator::START_OUTPUT,
                                    cmLocalGenerator::UNCHANGED);
      fout << lg->ConvertToXMLOutputPath(rel.c_str()) << " ";
      }
    else
      {
      fout << l->Value << " ";
      }
    }
}

//----------------------------------------------------------------------------
void
cmLocalVisualStudio7Generator
::OutputLibraryDirectories(std::ostream& fout,
                           std::vector<std::string> const& dirs)
{
  const char* comma = "";
  for(std::vector<std::string>::const_iterator d = dirs.begin();
      d != dirs.end(); ++d)
    {
    // Remove any trailing slash and skip empty paths.
    std::string dir = *d;
    if(dir[dir.size()-1] == '/')
      {
      dir = dir.substr(0, dir.size()-1);
      }
    if(dir.empty())
      {
      continue;
      }

    // Switch to a relative path specification if it is shorter.
    if(cmSystemTools::FileIsFullPath(dir.c_str()))
      {
      std::string rel = this->Convert(dir.c_str(), START_OUTPUT, UNCHANGED);
      if(rel.size() < dir.size())
        {
        dir = rel;
        }
      }

    // First search a configuration-specific subdirectory and then the
    // original directory.
    fout << comma << this->ConvertToXMLOutputPath((dir+"/$(OutDir)").c_str())
         << "," << this->ConvertToXMLOutputPath(dir.c_str());
    comma = ",";
    }
}

void cmLocalVisualStudio7Generator::WriteVCProjFile(std::ostream& fout,
                                                    const char *libName,
                                                    cmTarget &target)
{
  // get the configurations
  std::vector<std::string> *configs =
    static_cast<cmGlobalVisualStudio7Generator *>
    (this->GlobalGenerator)->GetConfigurations();

  // We may be modifying the source groups temporarily, so make a copy.
  std::vector<cmSourceGroup> sourceGroups = this->Makefile->GetSourceGroups();

  // get the classes from the source lists then add them to the groups
  std::vector<cmSourceFile*>const & classes = target.GetSourceFiles();
  for(std::vector<cmSourceFile*>::const_iterator i = classes.begin();
      i != classes.end(); i++)
    {
    // Add the file to the list of sources.
    std::string source = (*i)->GetFullPath();
    if(cmSystemTools::UpperCase((*i)->GetExtension()) == "DEF")
      {
      this->ModuleDefinitionFile = (*i)->GetFullPath();
      }
    cmSourceGroup& sourceGroup =
      this->Makefile->FindSourceGroup(source.c_str(), sourceGroups);
    sourceGroup.AssignSource(*i);
    }

  // Compute which sources need unique object computation.
  this->ComputeObjectNameRequirements(sourceGroups);

  // open the project
  this->WriteProjectStart(fout, libName, target, sourceGroups);
  // write the configuration information
  this->WriteConfigurations(fout, libName, target);

  fout << "\t<Files>\n";


  // Loop through every source group.
  for(unsigned int i = 0; i < sourceGroups.size(); ++i)
    {
    cmSourceGroup sg = sourceGroups[i];
    this->WriteGroup(&sg, target, fout, libName, configs);
    }

  //}

  fout << "\t</Files>\n";

  // Write the VCProj file's footer.
  this->WriteVCProjFooter(fout);
}

struct cmLVS7GFileConfig
{
  std::string ObjectName;
  std::string CompileFlags;
  std::string CompileDefs;
  std::string CompileDefsConfig;
  std::string AdditionalDeps;
  bool ExcludedFromBuild;
};

class cmLocalVisualStudio7GeneratorFCInfo
{
public:
  cmLocalVisualStudio7GeneratorFCInfo(cmLocalVisualStudio7Generator* lg,
                                      cmTarget& target,
                                      cmSourceFile const& sf,
                                      std::vector<std::string>* configs,
                                      std::string::size_type dir_len);
  std::map<cmStdString, cmLVS7GFileConfig> FileConfigMap;
};

cmLocalVisualStudio7GeneratorFCInfo
::cmLocalVisualStudio7GeneratorFCInfo(cmLocalVisualStudio7Generator* lg,
                                      cmTarget& target,
                                      cmSourceFile const& sf,
                                      std::vector<std::string>* configs,
                                      std::string::size_type dir_len)
{
  std::string objectName;
  if(lg->NeedObjectName.find(&sf) != lg->NeedObjectName.end())
    {
    objectName = lg->GetObjectFileNameWithoutTarget(sf, dir_len);
    }

  // Compute per-source, per-config information.
  for(std::vector<std::string>::iterator i = configs->begin();
      i != configs->end(); ++i)
    {
    std::string configUpper = cmSystemTools::UpperCase(*i);
    cmLVS7GFileConfig fc;
    bool needfc = false;
    if(!objectName.empty())
      {
      fc.ObjectName = objectName;
      needfc = true;
      }
    if(const char* cflags = sf.GetProperty("COMPILE_FLAGS"))
      {
      fc.CompileFlags = cflags;
      needfc = true;
      }
    if(const char* cdefs = sf.GetProperty("COMPILE_DEFINITIONS"))
      {
      fc.CompileDefs = cdefs;
      needfc = true;
      }
    std::string defPropName = "COMPILE_DEFINITIONS_";
    defPropName += configUpper;
    if(const char* ccdefs = sf.GetProperty(defPropName.c_str()))
      {
      fc.CompileDefsConfig = ccdefs;
      needfc = true;
      }

    // Check for extra object-file dependencies.
    if(const char* deps = sf.GetProperty("OBJECT_DEPENDS"))
      {
      std::vector<std::string> depends;
      cmSystemTools::ExpandListArgument(deps, depends);
      const char* sep = "";
      for(std::vector<std::string>::iterator j = depends.begin();
          j != depends.end(); ++j)
        {
        fc.AdditionalDeps += sep;
        fc.AdditionalDeps += lg->ConvertToXMLOutputPath(j->c_str());
        sep = ";";
        needfc = true;
        }
      }

    const char* lang =
      lg->GlobalGenerator->GetLanguageFromExtension
      (sf.GetExtension().c_str());
    const char* sourceLang = lg->GetSourceFileLanguage(sf);
    const char* linkLanguage = target.GetLinkerLanguage
      (lg->GetGlobalGenerator());
    bool needForceLang = false;
    // source file does not match its extension language
    if(lang && sourceLang && strcmp(lang, sourceLang) != 0)
      {
      needForceLang = true;
      lang = sourceLang;
      }
    // If lang is set, the compiler will generate code automatically.
    // If HEADER_FILE_ONLY is set, we must suppress this generation in
    // the project file
    fc.ExcludedFromBuild =
      (lang && sf.GetPropertyAsBool("HEADER_FILE_ONLY"));
    if(fc.ExcludedFromBuild)
      {
      needfc = true;
      }

    // if the source file does not match the linker language
    // then force c or c++
    if(needForceLang || (linkLanguage && lang
                         && strcmp(lang, linkLanguage) != 0))
      {
      if(strcmp(lang, "CXX") == 0)
        {
        // force a C++ file type
        fc.CompileFlags += " /TP ";
        needfc = true;
        }
      else if(strcmp(lang, "C") == 0)
        {
        // force to c
        fc.CompileFlags += " /TC ";
        needfc = true;
        }
      }

    if(needfc)
      {
      this->FileConfigMap[*i] = fc;
      }
    }
}

void cmLocalVisualStudio7Generator
::WriteGroup(const cmSourceGroup *sg, cmTarget& target,
             std::ostream &fout, const char *libName, 
             std::vector<std::string> *configs)
{
  const std::vector<const cmSourceFile *> &sourceFiles =
    sg->GetSourceFiles();
  // If the group is empty, don't write it at all.
  if(sourceFiles.empty() && sg->GetGroupChildren().empty())
    {
    return;
    }

  // If the group has a name, write the header.
  std::string name = sg->GetName();
  if(name != "")
    {
    this->WriteVCProjBeginGroup(fout, name.c_str(), "");
    }

  // Compute the maximum length of a configuration name.
  std::string::size_type config_len_max = 0;
  for(std::vector<std::string>::iterator i = configs->begin();
      i != configs->end(); ++i)
    {
    if(i->size() > config_len_max)
      {
      config_len_max = i->size();
      }
    }

  // Compute the maximum length of the full path to the intermediate
  // files directory for any configuration.  This is used to construct
  // object file names that do not produce paths that are too long.
  std::string::size_type dir_len = 0;
  dir_len += strlen(this->Makefile->GetCurrentOutputDirectory());
  dir_len += 1;
  dir_len += this->GetTargetDirectory(target).size();
  dir_len += 1;
  dir_len += config_len_max;
  dir_len += 1;

  // Loop through each source in the source group.
  std::string objectName;
  for(std::vector<const cmSourceFile *>::const_iterator sf =
        sourceFiles.begin(); sf != sourceFiles.end(); ++sf)
    {
    std::string source = (*sf)->GetFullPath();
    FCInfo fcinfo(this, target, *(*sf), configs, dir_len);

    if (source != libName || target.GetType() == cmTarget::UTILITY ||
      target.GetType() == cmTarget::GLOBAL_TARGET )
      {
      fout << "\t\t\t<File\n";
      std::string d = this->ConvertToXMLOutputPathSingle(source.c_str());
      // Tell MS-Dev what the source is.  If the compiler knows how to
      // build it, then it will.
      fout << "\t\t\t\tRelativePath=\"" << d << "\">\n";
      if(cmCustomCommand const* command = (*sf)->GetCustomCommand())
        {
        this->WriteCustomRule(fout, source.c_str(), *command, fcinfo);
        }
      else if(!fcinfo.FileConfigMap.empty())
        {
        const char* aCompilerTool = "VCCLCompilerTool";
        std::string ext = (*sf)->GetExtension();
        ext = cmSystemTools::LowerCase(ext);
        if(ext == "idl")
          {
          aCompilerTool = "VCMIDLTool";
          if(this->FortranProject)
            {
            aCompilerTool = "VFMIDLTool";
            }
          }
        if(ext == "rc")
          {
          aCompilerTool = "VCResourceCompilerTool";  
          if(this->FortranProject)
            {
            aCompilerTool = "VFResourceCompilerTool";
            }
          }
        if(ext == "def")
          {
          aCompilerTool = "VCCustomBuildTool";
          if(this->FortranProject)
            {
            aCompilerTool = "VFCustomBuildTool";
            }
          }
        for(std::map<cmStdString, cmLVS7GFileConfig>::const_iterator
              fci = fcinfo.FileConfigMap.begin();
            fci != fcinfo.FileConfigMap.end(); ++fci)
          {
          cmLVS7GFileConfig const& fc = fci->second;
          fout << "\t\t\t\t<FileConfiguration\n"
               << "\t\t\t\t\tName=\""  << fci->first
               << "|" << this->PlatformName << "\"";
          if(fc.ExcludedFromBuild)
            {
            fout << " ExcludedFromBuild=\"true\"";
            }
          fout << ">\n";
          fout << "\t\t\t\t\t<Tool\n"
               << "\t\t\t\t\tName=\"" << aCompilerTool << "\"\n";
          if(!fc.CompileFlags.empty() ||
             !fc.CompileDefs.empty() ||
             !fc.CompileDefsConfig.empty())
            {
            Options fileOptions(this, this->Version, Options::Compiler,
                                this->ExtraFlagTable);
            fileOptions.Parse(fc.CompileFlags.c_str());
            fileOptions.AddDefines(fc.CompileDefs.c_str());
            fileOptions.AddDefines(fc.CompileDefsConfig.c_str());
            fileOptions.OutputAdditionalOptions(fout, "\t\t\t\t\t", "\n");
            fileOptions.OutputFlagMap(fout, "\t\t\t\t\t");
            fileOptions.OutputPreprocessorDefinitions(fout,
                                                      "\t\t\t\t\t", "\n");
            }
          if(!fc.AdditionalDeps.empty())
            {
            fout << "\t\t\t\t\tAdditionalDependencies=\""
                 << fc.AdditionalDeps.c_str() << "\"\n";
            }
          if(!fc.ObjectName.empty())
            {
            fout << "\t\t\t\t\tObjectFile=\"$(IntDir)/"
                 << fc.ObjectName.c_str() << "\"\n";
            }
          fout << "\t\t\t\t\t/>\n"
               << "\t\t\t\t</FileConfiguration>\n";
          }
        }
      fout << "\t\t\t</File>\n";
      }
    }

  std::vector<cmSourceGroup> children  = sg->GetGroupChildren();

  for(unsigned int i=0;i<children.size();++i)
    {
    this->WriteGroup(&children[i], target, fout, libName, configs);
    }

  // If the group has a name, write the footer.
  if(name != "")
    {
    this->WriteVCProjEndGroup(fout);
    }
}

void cmLocalVisualStudio7Generator::
WriteCustomRule(std::ostream& fout,
                const char* source,
                const cmCustomCommand& command,
                FCInfo& fcinfo)
{
  std::string comment = this->ConstructComment(command);
  
  // Write the rule for each configuration.
  std::vector<std::string>::iterator i;
  std::vector<std::string> *configs =
    static_cast<cmGlobalVisualStudio7Generator *>
    (this->GlobalGenerator)->GetConfigurations();
  const char* compileTool = "VCCLCompilerTool";
  if(this->FortranProject)
    {
    compileTool = "VFCLCompilerTool";
    }
  const char* customTool = "VCCustomBuildTool";
  if(this->FortranProject)
    {
    customTool = "VFCustomBuildTool";
    }
  for(i = configs->begin(); i != configs->end(); ++i)
    {
    cmLVS7GFileConfig const& fc = fcinfo.FileConfigMap[*i];
    fout << "\t\t\t\t<FileConfiguration\n";
    fout << "\t\t\t\t\tName=\"" << *i << "|" << this->PlatformName << "\">\n";
    if(!fc.CompileFlags.empty())
      {
      fout << "\t\t\t\t\t<Tool\n"
           << "\t\t\t\t\tName=\"" << compileTool << "\"\n"
           << "\t\t\t\t\tAdditionalOptions=\""
           << this->EscapeForXML(fc.CompileFlags.c_str()) << "\"/>\n";
      }

    std::string script = 
      this->ConstructScript(command.GetCommandLines(),
                            command.GetWorkingDirectory(),
                            i->c_str(),
                            command.GetEscapeOldStyle(),
                            command.GetEscapeAllowMakeVars());
    fout << "\t\t\t\t\t<Tool\n"
         << "\t\t\t\t\tName=\"" << customTool << "\"\n"
         << "\t\t\t\t\tDescription=\"" 
         << this->EscapeForXML(comment.c_str()) << "\"\n"
         << "\t\t\t\t\tCommandLine=\"" 
         << this->EscapeForXML(script.c_str()) << "\"\n"
         << "\t\t\t\t\tAdditionalDependencies=\"";
    if(command.GetDepends().empty())
      {
      // There are no real dependencies.  Produce an artificial one to
      // make sure the rule runs reliably.
      if(!cmSystemTools::FileExists(source))
        {
        std::ofstream depout(source);
        depout << "Artificial dependency for a custom command.\n";
        }
      fout << this->ConvertToXMLOutputPath(source);
      }
    else
      {
      // Write out the dependencies for the rule.
      for(std::vector<std::string>::const_iterator d = 
          command.GetDepends().begin();
          d != command.GetDepends().end(); 
          ++d)
        {
        // Get the real name of the dependency in case it is a CMake target.
        std::string dep = this->GetRealDependency(d->c_str(), i->c_str());
        fout << this->ConvertToXMLOutputPath(dep.c_str())
             << ";";
        }
      }
    fout << "\"\n";
    fout << "\t\t\t\t\tOutputs=\"";
    if(command.GetOutputs().empty())
      {
      fout << source << "_force";
      }
    else
      {
      // Write a rule for the output generated by this command.
      const char* sep = "";
      for(std::vector<std::string>::const_iterator o = 
          command.GetOutputs().begin(); 
          o != command.GetOutputs().end(); 
          ++o)
        {
        fout << sep << this->ConvertToXMLOutputPathSingle(o->c_str());
        sep = ";";
        }
      }
    fout << "\"/>\n";
    fout << "\t\t\t\t</FileConfiguration>\n";
    }
}


void cmLocalVisualStudio7Generator::WriteVCProjBeginGroup(std::ostream& fout,
                                                          const char* group,
                                                          const char* )
{
  fout << "\t\t<Filter\n"
       << "\t\t\tName=\"" << group << "\"\n"
       << "\t\t\tFilter=\"\">\n";
}


void cmLocalVisualStudio7Generator::WriteVCProjEndGroup(std::ostream& fout)
{
  fout << "\t\t</Filter>\n";
}


// look for custom rules on a target and collect them together
void cmLocalVisualStudio7Generator
::OutputTargetRules(std::ostream& fout,
                    const char* configName,
                    cmTarget &target,
                    const char * /*libName*/)
{
  if (target.GetType() > cmTarget::GLOBAL_TARGET)
    {
    return;
    }
  const char* tool = "VCPreBuildEventTool";
  if(this->FortranProject)
    {
    tool = "VFPreBuildEventTool";
    }
  // add the pre build rules
  fout << "\t\t\t<Tool\n\t\t\t\tName=\"" << tool << "\"";
  bool init = false;
  for (std::vector<cmCustomCommand>::const_iterator cr =
         target.GetPreBuildCommands().begin();
       cr != target.GetPreBuildCommands().end(); ++cr)
    {
    if(!init)
      {
      const char* comment = cr->GetComment();
      if(comment && *comment)
        {
        fout << "\nDescription=\""
             << this->EscapeForXML(comment) << "\"";
        }
      fout << "\nCommandLine=\"";
      init = true;
      }
    else
      {
      fout << this->EscapeForXML("\n");
      }
    std::string script =
      this->ConstructScript(cr->GetCommandLines(),
                            cr->GetWorkingDirectory(),
                            configName,
                            cr->GetEscapeOldStyle(),
                            cr->GetEscapeAllowMakeVars());
    fout << this->EscapeForXML(script.c_str()).c_str();
    }
  if (init)
    {
    fout << "\"";
    }
  fout << "/>\n";

  // add the pre Link rules
  tool = "VCPreLinkEventTool";
  if(this->FortranProject)
    {
    tool = "VFPreLinkEventTool";
    }
  fout << "\t\t\t<Tool\n\t\t\t\tName=\"" << tool << "\"";
  init = false;
  for (std::vector<cmCustomCommand>::const_iterator cr =
         target.GetPreLinkCommands().begin();
       cr != target.GetPreLinkCommands().end(); ++cr)
    {
    if(!init)
      {
      const char* comment = cr->GetComment();
      if(comment && *comment)
        {
        fout << "\nDescription=\""
             << this->EscapeForXML(comment) << "\"";
        }
      fout << "\nCommandLine=\"";
      init = true;
      }
    else
      {
      fout << this->EscapeForXML("\n");
      }
    std::string script =
      this->ConstructScript(cr->GetCommandLines(),
                            cr->GetWorkingDirectory(),
                            configName,
                            cr->GetEscapeOldStyle(),
                            cr->GetEscapeAllowMakeVars());
    fout << this->EscapeForXML(script.c_str()).c_str();
    }
  if (init)
    {
    fout << "\"";
    }
  fout << "/>\n";

  // add the PostBuild rules
  tool = "VCPostBuildEventTool";
  if(this->FortranProject)
    {
    tool = "VFPostBuildEventTool";
    }
  fout << "\t\t\t<Tool\n\t\t\t\tName=\"" << tool << "\"";
  init = false;
  for (std::vector<cmCustomCommand>::const_iterator cr =
         target.GetPostBuildCommands().begin();
       cr != target.GetPostBuildCommands().end(); ++cr)
    {
    if(!init)
      {
      const char* comment = cr->GetComment();
      if(comment && *comment)
        {
        fout << "\nDescription=\""
             << this->EscapeForXML(comment) << "\"";
        }
      fout << "\nCommandLine=\"";
      init = true;
      }
    else
      {
      fout << this->EscapeForXML("\n");
      }
    std::string script =
      this->ConstructScript(cr->GetCommandLines(),
                            cr->GetWorkingDirectory(),
                            configName,
                            cr->GetEscapeOldStyle(),
                            cr->GetEscapeAllowMakeVars());
    fout << this->EscapeForXML(script.c_str()).c_str();
    }
  if (init)
    {
    fout << "\"";
    }
  fout << "/>\n";
}

void
cmLocalVisualStudio7Generator
::WriteProjectStartFortran(std::ostream& fout,
                           const char *libName,
                           cmTarget & target)
{
  
  cmGlobalVisualStudio7Generator* gg =
    static_cast<cmGlobalVisualStudio7Generator *>(this->GlobalGenerator);
  fout << "<?xml version=\"1.0\" encoding = \"Windows-1252\"?>\n"
       << "<VisualStudioProject\n"
       << "\tProjectCreator=\"Intel Fortran\"\n"
       << "\tVersion=\"9.10\"\n";
  const char* keyword = target.GetProperty("VS_KEYWORD");
  if(!keyword)
    {
    keyword = "Console Application";
    } 
  const char* projectType = 0;
  switch(target.GetType())
    {
    case cmTarget::STATIC_LIBRARY:
      projectType = "typeStaticLibrary";
      if(keyword)
        {
        keyword = "Static Library";
        }
      break;
    case cmTarget::SHARED_LIBRARY:
    case cmTarget::MODULE_LIBRARY:
      projectType = "typeDynamicLibrary";
      if(!keyword)
        {
        keyword = "Dll";
        }
      break;
    case cmTarget::EXECUTABLE:
      if(!keyword)
        {
        keyword = "Console Application";
        }
      projectType = 0;
      break;
    case cmTarget::UTILITY:
    case cmTarget::GLOBAL_TARGET:
    default:
      break;
    }
  if(projectType)
    {
    fout << "\tProjectType=\"" << projectType << "\">\n";
    }
  fout<< "\tKeyword=\"" << keyword << "\">\n" 
       << "\tProjectGUID=\"{" << gg->GetGUID(libName) << "}\">\n"
       << "\t<Platforms>\n"
       << "\t\t<Platform\n\t\t\tName=\"" << this->PlatformName << "\"/>\n"
       << "\t</Platforms>\n";
}


void
cmLocalVisualStudio7Generator::WriteProjectStart(std::ostream& fout,
                                                 const char *libName,
                                                 cmTarget & target,
                                                 std::vector<cmSourceGroup> &)
{
  if(this->FortranProject)
    {
    this->WriteProjectStartFortran(fout, libName, target);
    return;
    }
  fout << "<?xml version=\"1.0\" encoding = \"Windows-1252\"?>\n"
       << "<VisualStudioProject\n"
       << "\tProjectType=\"Visual C++\"\n";
  if(this->Version == 71)
    {
    fout << "\tVersion=\"7.10\"\n";
    }
  else
    {
    fout <<  "\tVersion=\"" << this->Version << ".00\"\n";
    }
  const char* projLabel = target.GetProperty("PROJECT_LABEL");
  if(!projLabel)
    {
    projLabel = libName;
    }
  const char* keyword = target.GetProperty("VS_KEYWORD");
  if(!keyword)
    {
    keyword = "Win32Proj";
    }
  cmGlobalVisualStudio7Generator* gg =
    static_cast<cmGlobalVisualStudio7Generator *>(this->GlobalGenerator);
  fout << "\tName=\"" << projLabel << "\"\n";
  if(this->Version >= 8)
    {
    fout << "\tProjectGUID=\"{" << gg->GetGUID(libName) << "}\"\n";
    }
  fout << "\tSccProjectName=\"\"\n"
       << "\tSccLocalPath=\"\"\n"
       << "\tKeyword=\"" << keyword << "\">\n"
       << "\t<Platforms>\n"
       << "\t\t<Platform\n\t\t\tName=\"" << this->PlatformName << "\"/>\n"
       << "\t</Platforms>\n";
}


void cmLocalVisualStudio7Generator::WriteVCProjFooter(std::ostream& fout)
{
  fout << "\t<Globals>\n"
       << "\t</Globals>\n"
       << "</VisualStudioProject>\n";
}

std::string cmLocalVisualStudio7GeneratorEscapeForXML(const char* s)
{
  std::string ret = s;
  cmSystemTools::ReplaceString(ret, "&", "&amp;");
  cmSystemTools::ReplaceString(ret, "\"", "&quot;");
  cmSystemTools::ReplaceString(ret, "<", "&lt;");
  cmSystemTools::ReplaceString(ret, ">", "&gt;");
  cmSystemTools::ReplaceString(ret, "\n", "&#x0D;&#x0A;");
  return ret;
}

std::string cmLocalVisualStudio7Generator::EscapeForXML(const char* s)
{
  return cmLocalVisualStudio7GeneratorEscapeForXML(s);
}

std::string cmLocalVisualStudio7Generator
::ConvertToXMLOutputPath(const char* path)
{
  std::string ret = this->ConvertToOptionallyRelativeOutputPath(path);
  cmSystemTools::ReplaceString(ret, "&", "&amp;");
  cmSystemTools::ReplaceString(ret, "\"", "&quot;");
  cmSystemTools::ReplaceString(ret, "<", "&lt;");
  cmSystemTools::ReplaceString(ret, ">", "&gt;");
  return ret;
}

std::string cmLocalVisualStudio7Generator
::ConvertToXMLOutputPathSingle(const char* path)
{
  std::string ret = this->ConvertToOptionallyRelativeOutputPath(path);
  cmSystemTools::ReplaceString(ret, "\"", "");
  cmSystemTools::ReplaceString(ret, "&", "&amp;");
  cmSystemTools::ReplaceString(ret, "<", "&lt;");
  cmSystemTools::ReplaceString(ret, ">", "&gt;");
  return ret;
}


// This class is used to parse an existing vs 7 project
// and extract the GUID 
class cmVS7XMLParser : public cmXMLParser
{
public:
  virtual void EndElement(const char* /* name */)
    {
    }
  virtual void StartElement(const char* name, const char** atts)
    {
      // once the GUID is found do nothing
      if(this->GUID.size())
        {
        return;
        }
      int i =0;
      if(strcmp("VisualStudioProject", name) == 0)
        {
        while(atts[i])
          {
          if(strcmp(atts[i], "ProjectGUID") == 0)
            { 
            if(atts[i+1])
              {
              this->GUID =  atts[i+1];
              this->GUID = this->GUID.substr(1, this->GUID.size()-2);
              }
            else
              {
              this->GUID = "";
              }
            return;
            }
          ++i;
          }
        } 
    }
  int InitializeParser()
    {
      int ret = cmXMLParser::InitializeParser();
      if(ret == 0)
        {
        return ret;
        }
      // visual studio projects have a strange encoding, but it is 
      // really utf-8
      XML_SetEncoding(static_cast<XML_Parser>(this->Parser), "utf-8");
      return 1;
    }
  std::string GUID;
};

void cmLocalVisualStudio7Generator::ReadAndStoreExternalGUID(
  const char* name,
  const char* path)
{
  cmVS7XMLParser parser;
  parser.ParseFile(path);
  // if we can not find a GUID then create one
  if(parser.GUID.size() == 0)
    {
    cmGlobalVisualStudio7Generator* gg =
      static_cast<cmGlobalVisualStudio7Generator *>(this->GlobalGenerator);
    gg->CreateGUID(name);
    return;
    }
  std::string guidStoreName = name;
  guidStoreName += "_GUID_CMAKE";
  // save the GUID in the cache
  this->GlobalGenerator->GetCMakeInstance()->
    AddCacheEntry(guidStoreName.c_str(),
                  parser.GUID.c_str(),
                  "Stored GUID",
                  cmCacheManager::INTERNAL);
}


void cmLocalVisualStudio7Generator::ConfigureFinalPass()
{
  cmLocalGenerator::ConfigureFinalPass();
  cmTargets &tgts = this->Makefile->GetTargets();

  cmGlobalVisualStudio7Generator* gg =
    static_cast<cmGlobalVisualStudio7Generator *>(this->GlobalGenerator);
  for(cmTargets::iterator l = tgts.begin(); l != tgts.end(); l++)
    {
    if (strncmp(l->first.c_str(), "INCLUDE_EXTERNAL_MSPROJECT", 26) == 0)
      {
      cmCustomCommand cc = l->second.GetPostBuildCommands()[0];
      const cmCustomCommandLines& cmds = cc.GetCommandLines();
      std::string project_name = cmds[0][0];
      this->ReadAndStoreExternalGUID(project_name.c_str(),
                                     cmds[0][1].c_str());
      }
    else
      {
      gg->CreateGUID(l->first.c_str());
      }
    }

}

//----------------------------------------------------------------------------
std::string cmLocalVisualStudio7Generator
::GetTargetDirectory(cmTarget const& target) const
{
  std::string dir;
  dir += target.GetName();
  dir += ".dir";
  return dir;
}

//----------------------------------------------------------------------------
cmLocalVisualStudio7GeneratorOptions
::cmLocalVisualStudio7GeneratorOptions(cmLocalVisualStudio7Generator* lg,
                                       int version,
                                       Tool tool,
                                       cmVS7FlagTable const* extraTable):
  LocalGenerator(lg), Version(version), CurrentTool(tool),
  DoingDefine(false), FlagTable(0), ExtraFlagTable(extraTable)
{
  // Choose the flag table for the requested tool.
  switch(tool)
    {
    case Compiler:
      this->FlagTable = cmLocalVisualStudio7GeneratorFlagTable; break;
    case Linker:
      this->FlagTable = cmLocalVisualStudio7GeneratorLinkFlagTable; break;
    case FortranCompiler:
      this->FlagTable = cmLocalVisualStudio7GeneratorFortranFlagTable;
    default: break;
    }
}

//----------------------------------------------------------------------------
void cmLocalVisualStudio7GeneratorOptions::FixExceptionHandlingDefault()
{
  // Exception handling is on by default because the platform file has
  // "/EHsc" in the flags.  Normally, that will override this
  // initialization to off, but the user has the option of removing
  // the flag to disable exception handling.  When the user does
  // remove the flag we need to override the IDE default of on.
  switch (this->Version)
    {
    case 7:
    case 71:
      this->FlagMap["ExceptionHandling"] = "FALSE";
    break;

    default:
      this->FlagMap["ExceptionHandling"] = "0";
    break;
    }
}

//----------------------------------------------------------------------------
void cmLocalVisualStudio7GeneratorOptions::SetVerboseMakefile(bool verbose)
{
  // If verbose makefiles have been requested and the /nologo option
  // was not given explicitly in the flags we want to add an attribute
  // to the generated project to disable logo suppression.  Otherwise
  // the GUI default is to enable suppression.
  if(verbose &&
     this->FlagMap.find("SuppressStartupBanner") == this->FlagMap.end())
    {
    this->FlagMap["SuppressStartupBanner"] = "FALSE";
    }
}

//----------------------------------------------------------------------------
void cmLocalVisualStudio7GeneratorOptions::AddDefine(const std::string& def)
{
  this->Defines.push_back(def);
}

//----------------------------------------------------------------------------
void cmLocalVisualStudio7GeneratorOptions::AddDefines(const char* defines)
{
  if(defines)
    {
    // Expand the list of definitions.
    cmSystemTools::ExpandListArgument(defines, this->Defines);
    }
}

//----------------------------------------------------------------------------
void cmLocalVisualStudio7GeneratorOptions::AddFlag(const char* flag,
                                                   const char* value)
{
  this->FlagMap[flag] = value;
}


bool cmLocalVisualStudio7GeneratorOptions::IsDebug()
{
  return this->FlagMap.find("DebugInformationFormat") != this->FlagMap.end();
}

//----------------------------------------------------------------------------
bool cmLocalVisualStudio7GeneratorOptions::UsingUnicode()
{
  // Look for the a _UNICODE definition.
  for(std::vector<std::string>::const_iterator di = this->Defines.begin();
      di != this->Defines.end(); ++di)
    {
    if(*di == "_UNICODE")
      {
      return true;
      }
    }
  return false;
}

//----------------------------------------------------------------------------
void cmLocalVisualStudio7GeneratorOptions::Parse(const char* flags)
{
  // Parse the input string as a windows command line since the string
  // is intended for writing directly into the build files.
  std::vector<std::string> args;
  cmSystemTools::ParseWindowsCommandLine(flags, args);

  // Process flags that need to be represented specially in the IDE
  // project file.
  for(std::vector<std::string>::iterator ai = args.begin();
      ai != args.end(); ++ai)
    {
    this->HandleFlag(ai->c_str());
    }
}

//----------------------------------------------------------------------------
void cmLocalVisualStudio7GeneratorOptions::HandleFlag(const char* flag)
{
  // If the last option was -D then this option is the definition.
  if(this->DoingDefine)
    {
    this->DoingDefine = false;
    this->Defines.push_back(flag);
    return;
    }

  // Look for known arguments.
  if(flag[0] == '-' || flag[0] == '/')
    {
    // Look for preprocessor definitions.
    if(this->CurrentTool == Compiler && flag[1] == 'D')
      {
      if(flag[2] == '\0')
        {
        // The next argument will have the definition.
        this->DoingDefine = true;
        }
      else
        {
        // Store this definition.
        this->Defines.push_back(flag+2);
        }
      return;
      }

    // Look through the available flag tables.
    bool flag_handled = false;
    if(this->FlagTable &&
       this->CheckFlagTable(this->FlagTable, flag, flag_handled))
      {
      return;
      }
    if(this->ExtraFlagTable &&
       this->CheckFlagTable(this->ExtraFlagTable, flag, flag_handled))
      {
      return;
      }

    // If any map entry handled the flag we are done.
    if(flag_handled)
      {
      return;
      }
    }
  // This option is not known.  Store it in the output flags.
  this->FlagString += " ";
  this->FlagString +=
    cmSystemTools::EscapeWindowsShellArgument(flag,
                                              cmsysSystem_Shell_Flag_VSIDE);
}

//----------------------------------------------------------------------------
bool
cmLocalVisualStudio7GeneratorOptions
::CheckFlagTable(cmVS7FlagTable const* table, const char* flag,
                 bool& flag_handled)
{
  // Look for an entry in the flag table matching this flag.
  for(cmVS7FlagTable const* entry = table; entry->IDEName; ++entry)
    {
    bool entry_found = false;
    if(entry->special & cmVS7FlagTable::UserValue)
      {
      // This flag table entry accepts a user-specified value.  If
      // the entry specifies UserRequired we must match only if a
      // non-empty value is given.
      int n = static_cast<int>(strlen(entry->commandFlag));
      if(strncmp(flag+1, entry->commandFlag, n) == 0 &&
         (!(entry->special & cmVS7FlagTable::UserRequired) ||
          static_cast<int>(strlen(flag+1)) > n))
        {
        if(entry->special & cmVS7FlagTable::UserIgnored)
          {
          // Ignore the user-specified value.
          this->FlagMap[entry->IDEName] = entry->value;
          }
        else if(entry->special & cmVS7FlagTable::SemicolonAppendable)
          {
          const char *new_value = flag+1+n;
          
          std::map<cmStdString,cmStdString>::iterator itr;
          itr = this->FlagMap.find(entry->IDEName);
          if(itr != this->FlagMap.end())
            {
            // Append to old value (if present) with semicolons;
            itr->second += ";";
            itr->second += new_value;
            }
          else
            {
            this->FlagMap[entry->IDEName] = new_value;
            }
          }
        else
          {
          // Use the user-specified value.
          this->FlagMap[entry->IDEName] = flag+1+n;
          }
        entry_found = true;
        }
      }
    else if(strcmp(flag+1, entry->commandFlag) == 0)
      {
      // This flag table entry provides a fixed value.
      this->FlagMap[entry->IDEName] = entry->value;
      entry_found = true;
      }
    
    // If the flag has been handled by an entry not requesting a
    // search continuation we are done.
    if(entry_found && !(entry->special & cmVS7FlagTable::Continue))
      {
      return true;
      }
    
    // If the entry was found the flag has been handled.
    flag_handled = flag_handled || entry_found;
    }
  
  return false;
}

//----------------------------------------------------------------------------
void
cmLocalVisualStudio7GeneratorOptions
::OutputPreprocessorDefinitions(std::ostream& fout,
                                const char* prefix,
                                const char* suffix)
{
  if(this->Defines.empty())
    {
    return;
    }

  fout << prefix <<  "PreprocessorDefinitions=\"";
  const char* comma = "";
  for(std::vector<std::string>::const_iterator di = this->Defines.begin();
      di != this->Defines.end(); ++di)
    {
    // Escape the definition for the compiler.
    std::string define =
      this->LocalGenerator->EscapeForShell(di->c_str(), true);

    // Escape this flag for the IDE.
    define = cmLocalVisualStudio7GeneratorEscapeForXML(define.c_str());

    // Store the flag in the project file.
    fout << comma << define;
    comma = ",";
    }
  fout << "\"" << suffix;
}

//----------------------------------------------------------------------------
void
cmLocalVisualStudio7GeneratorOptions
::OutputFlagMap(std::ostream& fout, const char* indent)
{
  for(std::map<cmStdString, cmStdString>::iterator m = this->FlagMap.begin();
      m != this->FlagMap.end(); ++m)
    {
    fout << indent << m->first << "=\"" << m->second << "\"\n";
    }
}

//----------------------------------------------------------------------------
void
cmLocalVisualStudio7GeneratorOptions
::OutputAdditionalOptions(std::ostream& fout,
                          const char* prefix,
                          const char* suffix)
{
  if(!this->FlagString.empty())
    {
    fout << prefix << "AdditionalOptions=\"";
    fout <<
      cmLocalVisualStudio7GeneratorEscapeForXML(this->FlagString.c_str());
    fout << "\"" << suffix;
    }
}
void cmLocalVisualStudio7Generator::
GetTargetObjectFileDirectories(cmTarget* target,
                               std::vector<std::string>& 
                               dirs)
{
  std::string dir = this->Makefile->GetCurrentOutputDirectory();
  dir += "/";
  dir += this->GetTargetDirectory(*target);
  dir += "/";
  dir += this->GetGlobalGenerator()->GetCMakeCFGInitDirectory();
  dirs.push_back(dir);
}

//----------------------------------------------------------------------------
#include <windows.h>
static bool cmLVS6G_IsFAT(const char* dir)
{
  if(dir[0] && dir[1] == ':')
    {
    char volRoot[4] = "_:/";
    volRoot[0] = dir[0];
    char fsName[16];
    if(GetVolumeInformation(volRoot, 0, 0, 0, 0, 0, fsName, 16) &&
       strstr(fsName, "FAT") != 0)
      {
      return true;
      }
    }
  return false;
}
