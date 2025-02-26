/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator3
  Module:    $RCSfile: cmGlobalUnixMakefileGenerator3.cxx,v $
  Language:  C++
  Date:      $Date: 2008-03-24 22:23:26 $
  Version:   $Revision: 1.126.2.1 $

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "cmGlobalUnixMakefileGenerator3.h"
#include "cmLocalUnixMakefileGenerator3.h"
#include "cmMakefile.h"
#include "cmake.h"
#include "cmGeneratedFileStream.h"
#include "cmSourceFile.h"
#include "cmTarget.h"

cmGlobalUnixMakefileGenerator3::cmGlobalUnixMakefileGenerator3()
{
  // This type of makefile always requires unix style paths
  this->ForceUnixPaths = true;
  this->FindMakeProgramFile = "CMakeUnixFindMake.cmake";
  this->ToolSupportsColor = true;
  this->ForceVerboseMakefiles = false;

#ifdef _WIN32
  this->UseLinkScript = false;
#else
  this->UseLinkScript = true;
#endif
}

void cmGlobalUnixMakefileGenerator3
::EnableLanguage(std::vector<std::string>const& languages, 
                 cmMakefile *mf, 
                 bool optional)
{
  this->cmGlobalGenerator::EnableLanguage(languages, mf, optional);
  std::string path;
  for(std::vector<std::string>::const_iterator l = languages.begin();
      l != languages.end(); ++l)
    {
    if(*l == "NONE")
      {
      continue;
      }
    const char* lang = l->c_str();
    std::string langComp = "CMAKE_";
    langComp += lang;
    langComp += "_COMPILER";
    
    if(!mf->GetDefinition(langComp.c_str()))
      {
      cmSystemTools::Error(langComp.c_str(), 
                           " not set, after EnableLanguage");
      continue;
      }
    const char* name = mf->GetRequiredDefinition(langComp.c_str()); 
    if(!cmSystemTools::FileIsFullPath(name))
      {
      path = cmSystemTools::FindProgram(name);
      }
    else
      {
      path = name;
      }
    if((path.size() == 0 || !cmSystemTools::FileExists(path.c_str())) 
        && (optional==false))
      {
      std::string message = "your ";
      message += lang;
      message += " compiler: \"";
      message +=  name;
      message += "\" was not found.   Please set ";
      message += langComp;
      message += " to a valid compiler path or name.";
      cmSystemTools::Error(message.c_str());
      path = name;
      }
    std::string doc = lang;
    doc += " compiler.";
    const char* cname = this->GetCMakeInstance()->
      GetCacheManager()->GetCacheValue(langComp.c_str());
    std::string changeVars;
    if(cname && (path != cname))
      {
      const char* cvars = 
        this->GetCMakeInstance()->GetProperty(
          "__CMAKE_DELETE_CACHE_CHANGE_VARS_");
      if(cvars)
        {
        changeVars += cvars;
        changeVars += ";";
        }
      changeVars += langComp;
      changeVars += ";";
      changeVars += cname;
      this->GetCMakeInstance()->SetProperty(
        "__CMAKE_DELETE_CACHE_CHANGE_VARS_",
        changeVars.c_str());
      }
    mf->AddCacheDefinition(langComp.c_str(), path.c_str(),
                           doc.c_str(), cmCacheManager::FILEPATH);
    }
}

///! Create a local generator appropriate to this Global Generator
cmLocalGenerator *cmGlobalUnixMakefileGenerator3::CreateLocalGenerator()
{
  cmLocalGenerator* lg = new cmLocalUnixMakefileGenerator3;
  lg->SetGlobalGenerator(this);
  return lg;
}

//----------------------------------------------------------------------------
void cmGlobalUnixMakefileGenerator3
::GetDocumentation(cmDocumentationEntry& entry) const
{
  entry.Name = this->GetName();
  entry.Brief = "Generates standard UNIX makefiles.";
  entry.Full =
    "A hierarchy of UNIX makefiles is generated into the build tree.  Any "
    "standard UNIX-style make program can build the project through the "
    "default make target.  A \"make install\" target is also provided.";
}

//----------------------------------------------------------------------------
void cmGlobalUnixMakefileGenerator3::Generate() 
{
  // first do superclass method
  this->cmGlobalGenerator::Generate();

  // initialize progress
  unsigned int i;
  unsigned long total = 0;
  for (i = 0; i < this->LocalGenerators.size(); ++i)
    {
    cmLocalUnixMakefileGenerator3 *lg = 
      static_cast<cmLocalUnixMakefileGenerator3 *>(this->LocalGenerators[i]);
    total += lg->GetNumberOfProgressActions();
    }

  // write each target's progress.make this loop is done twice. Bascially the
  // Generate pass counts all the actions, the first loop below determines
  // how many actions have progress updates for each target and writes to
  // corrrect variable values for everything except the all targets. The
  // second loop actually writes out correct values for the all targets as
  // well. This is because the all targets require more information that is
  // computed in the first loop.
  unsigned long current = 0;
  for (i = 0; i < this->LocalGenerators.size(); ++i)
    {
    cmLocalUnixMakefileGenerator3 *lg = 
      static_cast<cmLocalUnixMakefileGenerator3 *>(this->LocalGenerators[i]);
    lg->WriteProgressVariables(total,current);
    }
  for (i = 0; i < this->LocalGenerators.size(); ++i)
    {
    cmLocalUnixMakefileGenerator3 *lg = 
      static_cast<cmLocalUnixMakefileGenerator3 *>(this->LocalGenerators[i]);
    lg->WriteAllProgressVariable();
    }
  
  // write the main makefile
  this->WriteMainMakefile2();
  this->WriteMainCMakefile();
}

void cmGlobalUnixMakefileGenerator3::WriteMainMakefile2()
{
  // Open the output file.  This should not be copy-if-different
  // because the check-build-system step compares the makefile time to
  // see if the build system must be regenerated.
  std::string makefileName = 
    this->GetCMakeInstance()->GetHomeOutputDirectory();
  makefileName += cmake::GetCMakeFilesDirectory();
  makefileName += "/Makefile2";
  cmGeneratedFileStream makefileStream(makefileName.c_str());
  if(!makefileStream)
    {
    return;
    }
 
  // get a local generator for some useful methods
  cmLocalUnixMakefileGenerator3 *lg = 
    static_cast<cmLocalUnixMakefileGenerator3 *>(this->LocalGenerators[0]);
    
  // Write the do not edit header.
  lg->WriteDisclaimer(makefileStream);

  // Write the main entry point target.  This must be the VERY first
  // target so that make with no arguments will run it.
  // Just depend on the all target to drive the build.
  std::vector<std::string> depends;
  std::vector<std::string> no_commands;
  depends.push_back("all");

  // Write the rule.
  lg->WriteMakeRule(makefileStream,
                    "Default target executed when no arguments are "
                    "given to make.",
                    "default_target",
                    depends,
                    no_commands, true);

  depends.clear();

  // The all and preinstall rules might never have any dependencies
  // added to them.
  if(this->EmptyRuleHackDepends != "")
    {
    depends.push_back(this->EmptyRuleHackDepends);
    }

  // Write and empty all:
  lg->WriteMakeRule(makefileStream, 
                    "The main recursive all target", "all", 
                    depends, no_commands, true);

  // Write an empty preinstall:
  lg->WriteMakeRule(makefileStream,
                    "The main recursive preinstall target", "preinstall",
                    depends, no_commands, true);

  // Write out the "special" stuff
  lg->WriteSpecialTargetsTop(makefileStream);
  
  // write the target convenience rules
  unsigned int i;
  for (i = 0; i < this->LocalGenerators.size(); ++i)
    {
    lg = 
      static_cast<cmLocalUnixMakefileGenerator3 *>(this->LocalGenerators[i]);
    this->WriteConvenienceRules2(makefileStream,lg);
    }

  lg = static_cast<cmLocalUnixMakefileGenerator3 *>(this->LocalGenerators[0]);
  lg->WriteSpecialTargetsBottom(makefileStream);
}


//----------------------------------------------------------------------------
void cmGlobalUnixMakefileGenerator3::WriteMainCMakefile()
{
  // Open the output file.  This should not be copy-if-different
  // because the check-build-system step compares the makefile time to
  // see if the build system must be regenerated.
  std::string cmakefileName = 
    this->GetCMakeInstance()->GetHomeOutputDirectory();
  cmakefileName += cmake::GetCMakeFilesDirectory();
  cmakefileName += "/Makefile.cmake";
  cmGeneratedFileStream cmakefileStream(cmakefileName.c_str());
  if(!cmakefileStream)
    {
    return;
    }

  std::string makefileName = 
    this->GetCMakeInstance()->GetHomeOutputDirectory();
  makefileName += "/Makefile";
  
  // get a local generator for some useful methods
  cmLocalUnixMakefileGenerator3 *lg = 
    static_cast<cmLocalUnixMakefileGenerator3 *>(this->LocalGenerators[0]);
    
  // Write the do not edit header.
  lg->WriteDisclaimer(cmakefileStream);

  // Save the generator name
  cmakefileStream
    << "# The generator used is:\n"
    << "SET(CMAKE_DEPENDS_GENERATOR \"" << this->GetName() << "\")\n\n";

  // for each cmMakefile get its list of dependencies
  std::vector<std::string> lfiles;
  for (unsigned int i = 0; i < this->LocalGenerators.size(); ++i)
    {
    lg = 
      static_cast<cmLocalUnixMakefileGenerator3 *>(this->LocalGenerators[i]);
  
    // Get the list of files contributing to this generation step.
    lfiles.insert(lfiles.end(),lg->GetMakefile()->GetListFiles().begin(),
                  lg->GetMakefile()->GetListFiles().end());
    }
  // Sort the list and remove duplicates.
  std::sort(lfiles.begin(), lfiles.end(), std::less<std::string>());
  std::vector<std::string>::iterator new_end = 
    std::unique(lfiles.begin(),lfiles.end());
  lfiles.erase(new_end, lfiles.end());

  // reset lg to the first makefile
  lg = static_cast<cmLocalUnixMakefileGenerator3 *>(this->LocalGenerators[0]);

  // Build the path to the cache file.
  std::string cache = this->GetCMakeInstance()->GetHomeOutputDirectory();
  cache += "/CMakeCache.txt";

  // Save the list to the cmake file.
  cmakefileStream
    << "# The top level Makefile was generated from the following files:\n"
    << "SET(CMAKE_MAKEFILE_DEPENDS\n"
    << "  \"" 
    << lg->Convert(cache.c_str(),
                   cmLocalGenerator::START_OUTPUT).c_str() << "\"\n";
  for(std::vector<std::string>::const_iterator i = lfiles.begin();
      i !=  lfiles.end(); ++i)
    {
    cmakefileStream
      << "  \"" 
      << lg->Convert(i->c_str(), cmLocalGenerator::START_OUTPUT).c_str()
      << "\"\n";
    }
  cmakefileStream
    << "  )\n\n";

  // Build the path to the cache check file.
  std::string check = this->GetCMakeInstance()->GetHomeOutputDirectory();
  check += cmake::GetCMakeFilesDirectory();
  check += "/cmake.check_cache";

  // Set the corresponding makefile in the cmake file.
  cmakefileStream
    << "# The corresponding makefile is:\n"
    << "SET(CMAKE_MAKEFILE_OUTPUTS\n"
    << "  \"" 
    << lg->Convert(makefileName.c_str(),
                   cmLocalGenerator::START_OUTPUT).c_str() << "\"\n"
    << "  \"" 
    << lg->Convert(check.c_str(),
                   cmLocalGenerator::START_OUTPUT).c_str() << "\"\n";

  // add in all the directory information files
  std::string tmpStr;
  for (unsigned int i = 0; i < this->LocalGenerators.size(); ++i)
    {
    lg = 
      static_cast<cmLocalUnixMakefileGenerator3 *>(this->LocalGenerators[i]);
    tmpStr = lg->GetMakefile()->GetStartOutputDirectory();
    tmpStr += cmake::GetCMakeFilesDirectory();
    tmpStr += "/CMakeDirectoryInformation.cmake";
    cmakefileStream << "  \"" << 
      lg->Convert(tmpStr.c_str(),cmLocalGenerator::HOME_OUTPUT).c_str() 
                    << "\"\n";
    const std::vector<std::string>& outfiles = 
      lg->GetMakefile()->GetOutputFiles();
    for(std::vector<std::string>::const_iterator k= outfiles.begin();
        k != outfiles.end(); ++k)
      {
      cmakefileStream << "  \"" << 
        lg->Convert(k->c_str(),cmLocalGenerator::HOME_OUTPUT).c_str() 
                      << "\"\n";
      }
    }
  cmakefileStream << "  )\n\n";

  this->WriteMainCMakefileLanguageRules(cmakefileStream, 
                                        this->LocalGenerators);
}

void cmGlobalUnixMakefileGenerator3
::WriteMainCMakefileLanguageRules(cmGeneratedFileStream& cmakefileStream,
                                  std::vector<cmLocalGenerator *> &lGenerators
                                  )
{
  cmLocalUnixMakefileGenerator3 *lg;

  // now list all the target info files
  cmakefileStream
    << "# Dependency information for all targets:\n";
  cmakefileStream
    << "SET(CMAKE_DEPEND_INFO_FILES\n";
  for (unsigned int i = 0; i < lGenerators.size(); ++i)
    {
    lg = static_cast<cmLocalUnixMakefileGenerator3 *>(lGenerators[i]);
    // for all of out targets
    for (cmTargets::iterator l = lg->GetMakefile()->GetTargets().begin();
         l != lg->GetMakefile()->GetTargets().end(); l++)
      {
      if((l->second.GetType() == cmTarget::EXECUTABLE) ||
         (l->second.GetType() == cmTarget::STATIC_LIBRARY) ||
         (l->second.GetType() == cmTarget::SHARED_LIBRARY) ||
         (l->second.GetType() == cmTarget::MODULE_LIBRARY) ||
         (l->second.GetType() == cmTarget::UTILITY))
        {
        std::string tname = lg->GetRelativeTargetDirectory(l->second);
        tname += "/DependInfo.cmake";
        cmSystemTools::ConvertToUnixSlashes(tname);
        cmakefileStream << "  \"" << tname.c_str() << "\"\n";
        }
      }
    }
  cmakefileStream << "  )\n";
}

//----------------------------------------------------------------------------
void
cmGlobalUnixMakefileGenerator3
::WriteDirectoryRule2(std::ostream& ruleFileStream,
                      cmLocalUnixMakefileGenerator3* lg,
                      const char* pass, bool check_all,
                      bool check_relink)
{
  // Get the relative path to the subdirectory from the top.
  std::string makeTarget = lg->GetMakefile()->GetStartOutputDirectory();
  makeTarget += "/";
  makeTarget += pass;

  // The directory-level rule should depend on the target-level rules
  // for all targets in the directory.
  std::vector<std::string> depends;
  for(cmTargets::iterator l = lg->GetMakefile()->GetTargets().begin();
      l != lg->GetMakefile()->GetTargets().end(); ++l)
    {
    if((l->second.GetType() == cmTarget::EXECUTABLE) ||
       (l->second.GetType() == cmTarget::STATIC_LIBRARY) ||
       (l->second.GetType() == cmTarget::SHARED_LIBRARY) ||
       (l->second.GetType() == cmTarget::MODULE_LIBRARY) ||
       (l->second.GetType() == cmTarget::UTILITY))
      {
      // Add this to the list of depends rules in this directory.
      if((!check_all || !l->second.GetPropertyAsBool("EXCLUDE_FROM_ALL")) &&
         (!check_relink || l->second.NeedRelinkBeforeInstall()))
        {
        std::string tname = lg->GetRelativeTargetDirectory(l->second);
        tname += "/";
        tname += pass;
        depends.push_back(tname);
        }
      }
    }

  // The directory-level rule should depend on the directory-level
  // rules of the subdirectories.
  for(std::vector<cmLocalGenerator*>::iterator sdi = 
        lg->GetChildren().begin(); sdi != lg->GetChildren().end(); ++sdi)
    {
    cmLocalUnixMakefileGenerator3* slg =
      static_cast<cmLocalUnixMakefileGenerator3*>(*sdi);
    std::string subdir = slg->GetMakefile()->GetStartOutputDirectory();
    subdir += "/";
    subdir += pass;
    depends.push_back(subdir);
    }

  // Work-around for makes that drop rules that have no dependencies
  // or commands.
  if(depends.empty() && this->EmptyRuleHackDepends != "")
    {
    depends.push_back(this->EmptyRuleHackDepends);
    }

  // Write the rule.
  std::string doc = "Convenience name for \"";
  doc += pass;
  doc += "\" pass in the directory.";
  std::vector<std::string> no_commands;
  lg->WriteMakeRule(ruleFileStream, doc.c_str(),
                    makeTarget.c_str(), depends, no_commands, true);
}

//----------------------------------------------------------------------------
void
cmGlobalUnixMakefileGenerator3
::WriteDirectoryRules2(std::ostream& ruleFileStream,
                       cmLocalUnixMakefileGenerator3* lg)
{
  // Only subdirectories need these rules.
  if(!lg->GetParent())
    {
    return;
    }

  // Begin the directory-level rules section.
  std::string dir = lg->GetMakefile()->GetStartOutputDirectory();
  dir = lg->Convert(dir.c_str(), cmLocalGenerator::HOME_OUTPUT,
                    cmLocalGenerator::MAKEFILE);
  lg->WriteDivider(ruleFileStream);
  ruleFileStream
    << "# Directory level rules for directory "
    << dir << "\n\n";

  // Write directory-level rules for "all".
  this->WriteDirectoryRule2(ruleFileStream, lg, "all", true, false);

  // Write directory-level rules for "clean".
  this->WriteDirectoryRule2(ruleFileStream, lg, "clean", false, false);

  // Write directory-level rules for "preinstall".
  this->WriteDirectoryRule2(ruleFileStream, lg, "preinstall", true, true);
}


std::string cmGlobalUnixMakefileGenerator3
::GenerateBuildCommand(const char* makeProgram, const char *projectName, 
                       const char* additionalOptions, const char *targetName,
                       const char* config, bool ignoreErrors, bool fast)
{
  // Project name and config are not used yet.
  (void)projectName;
  (void)config;

  std::string makeCommand = 
    cmSystemTools::ConvertToUnixOutputPath(makeProgram);
  
  // Since we have full control over the invocation of nmake, let us
  // make it quiet.
  if ( strcmp(this->GetName(), "NMake Makefiles") == 0 )
    {
    makeCommand += " /NOLOGO ";
    }
  if ( ignoreErrors )
    {
    makeCommand += " -i";
    }
  if ( additionalOptions )
    {
    makeCommand += " ";
    makeCommand += additionalOptions;
    }
  if ( targetName && strlen(targetName))
    {
    cmLocalUnixMakefileGenerator3 *lg;
    if (this->LocalGenerators.size())
      {
      lg = static_cast<cmLocalUnixMakefileGenerator3 *>
        (this->LocalGenerators[0]);
      }
    else
      {
      lg = static_cast<cmLocalUnixMakefileGenerator3 *>
        (this->CreateLocalGenerator());
      // set the Start directories
      lg->GetMakefile()->SetStartDirectory
        (this->CMakeInstance->GetStartDirectory());
      lg->GetMakefile()->SetStartOutputDirectory
        (this->CMakeInstance->GetStartOutputDirectory());
      lg->GetMakefile()->MakeStartDirectoriesCurrent();
      }
    
    makeCommand += " \"";
    std::string tname = targetName;
    if(fast)
      {
      tname += "/fast";
      }
    tname = lg->Convert(tname.c_str(),cmLocalGenerator::HOME_OUTPUT,
                        cmLocalGenerator::MAKEFILE);
    makeCommand += tname.c_str();
    makeCommand += "\"";
    if (!this->LocalGenerators.size())
      {
      delete lg;
      }
    }
  return makeCommand;
}

//----------------------------------------------------------------------------
void
cmGlobalUnixMakefileGenerator3
::WriteConvenienceRules(std::ostream& ruleFileStream, 
                        std::set<cmStdString> &emitted)
{
  std::vector<std::string> depends;  
  std::vector<std::string> commands;

  depends.push_back("cmake_check_build_system");
  
  // write the target convenience rules
  unsigned int i;
  cmLocalUnixMakefileGenerator3 *lg;
  for (i = 0; i < this->LocalGenerators.size(); ++i)
    {
    lg = static_cast<cmLocalUnixMakefileGenerator3 *>
      (this->LocalGenerators[i]);
    // for each target Generate the rule files for each target.
    cmTargets& targets = lg->GetMakefile()->GetTargets();
    for(cmTargets::iterator t = targets.begin(); t != targets.end(); ++t)
      {
      // Don't emit the same rule twice (e.g. two targets with the same
      // simple name)
      if(t->second.GetName() &&
         strlen(t->second.GetName()) &&
         emitted.insert(t->second.GetName()).second &&
         // Handle user targets here.  Global targets are handled in
         // the local generator on a per-directory basis.
         ((t->second.GetType() == cmTarget::EXECUTABLE) ||
          (t->second.GetType() == cmTarget::STATIC_LIBRARY) ||
          (t->second.GetType() == cmTarget::SHARED_LIBRARY) ||
          (t->second.GetType() == cmTarget::MODULE_LIBRARY) ||
          (t->second.GetType() == cmTarget::UTILITY)))
        {
        // Add a rule to build the target by name.
        lg->WriteDivider(ruleFileStream);
        ruleFileStream
          << "# Target rules for targets named "
          << t->second.GetName() << "\n\n";
        
        // Write the rule.
        commands.clear();
        std::string tmp = cmake::GetCMakeFilesDirectoryPostSlash();
        tmp += "Makefile2";
        commands.push_back(lg->GetRecursiveMakeCall
                            (tmp.c_str(),t->second.GetName()));
        depends.clear();
        depends.push_back("cmake_check_build_system");
        lg->WriteMakeRule(ruleFileStream, 
                          "Build rule for target.",
                          t->second.GetName(), depends, commands,
                          true);
        
        // Add a fast rule to build the target
        std::string localName = lg->GetRelativeTargetDirectory(t->second);
        std::string makefileName;
        makefileName = localName;
        makefileName += "/build.make";          
        depends.clear();
        commands.clear();
        std::string makeTargetName = localName;
        makeTargetName += "/build";
        localName = t->second.GetName();
        localName += "/fast";
        commands.push_back(lg->GetRecursiveMakeCall
                            (makefileName.c_str(), makeTargetName.c_str()));
        lg->WriteMakeRule(ruleFileStream, "fast build rule for target.",
                          localName.c_str(), depends, commands, true);

        // Add a local name for the rule to relink the target before
        // installation.
        if(t->second.NeedRelinkBeforeInstall())
          {
          makeTargetName = lg->GetRelativeTargetDirectory(t->second);
          makeTargetName += "/preinstall";
          localName = t->second.GetName();
          localName += "/preinstall";
          depends.clear();
          commands.clear();
          commands.push_back(lg->GetRecursiveMakeCall
                             (makefileName.c_str(), makeTargetName.c_str()));
          lg->WriteMakeRule(ruleFileStream,
                            "Manual pre-install relink rule for target.",
                            localName.c_str(), depends, commands, true);
          }
        }
      }
    }
}


//----------------------------------------------------------------------------
void
cmGlobalUnixMakefileGenerator3
::WriteConvenienceRules2(std::ostream& ruleFileStream, 
                         cmLocalUnixMakefileGenerator3 *lg)
{
  std::vector<std::string> depends;  
  std::vector<std::string> commands;
  std::string localName;
  std::string makeTargetName;


  // write the directory level rules for this local gen
  this->WriteDirectoryRules2(ruleFileStream,lg);
  
  depends.push_back("cmake_check_build_system");

  // for each target Generate the rule files for each target.
  cmTargets& targets = lg->GetMakefile()->GetTargets();
  for(cmTargets::iterator t = targets.begin(); t != targets.end(); ++t)
    {
    if (t->second.GetName()
     && strlen(t->second.GetName())
     && ((t->second.GetType() == cmTarget::EXECUTABLE)
        || (t->second.GetType() == cmTarget::STATIC_LIBRARY)
        || (t->second.GetType() == cmTarget::SHARED_LIBRARY)
        || (t->second.GetType() == cmTarget::MODULE_LIBRARY)
        || (t->second.GetType() == cmTarget::UTILITY)))
      {
      std::string makefileName;
      // Add a rule to build the target by name.
      localName = lg->GetRelativeTargetDirectory(t->second);
      makefileName = localName;
      makefileName += "/build.make";
      
      bool needRequiresStep = this->NeedRequiresStep(t->second);
      
      lg->WriteDivider(ruleFileStream);
      ruleFileStream
        << "# Target rules for target "
        << localName << "\n\n";
    
      commands.clear();        
      makeTargetName = localName;
      makeTargetName += "/depend";
      commands.push_back(lg->GetRecursiveMakeCall
                         (makefileName.c_str(),makeTargetName.c_str()));

      // add requires if we need it for this generator
      if (needRequiresStep)
        {
        makeTargetName = localName;
        makeTargetName += "/requires";
        commands.push_back(lg->GetRecursiveMakeCall
                           (makefileName.c_str(),makeTargetName.c_str()));
        }
      makeTargetName = localName;
      makeTargetName += "/build";
      commands.push_back(lg->GetRecursiveMakeCall
                          (makefileName.c_str(),makeTargetName.c_str()));
      
      // Write the rule.
      localName += "/all";
      depends.clear();

      std::string progressDir =
        lg->GetMakefile()->GetHomeOutputDirectory();
      progressDir += cmake::GetCMakeFilesDirectory();
        {
        cmOStringStream progCmd;
        progCmd << "$(CMAKE_COMMAND) -E cmake_progress_report "; 
        // all target counts
        progCmd << lg->Convert(progressDir.c_str(),
                                cmLocalGenerator::FULL,
                                cmLocalGenerator::SHELL);
        progCmd << " ";
        std::vector<int> &progFiles = lg->ProgressFiles[t->first];
        for (std::vector<int>::iterator i = progFiles.begin();
              i != progFiles.end(); ++i)
          {
          progCmd << " " << *i;
          }
        commands.push_back(progCmd.str());
        }
      progressDir = "Built target ";
      progressDir += t->first;
      lg->AppendEcho(commands,progressDir.c_str());
      
      this->AppendGlobalTargetDepends(depends,t->second);
      lg->WriteMakeRule(ruleFileStream, "All Build rule for target.",
                        localName.c_str(), depends, commands, true);
      
      // add the all/all dependency
      if(!this->IsExcluded(this->LocalGenerators[0], t->second))
        {
        depends.clear();
        depends.push_back(localName);
        commands.clear();
        lg->WriteMakeRule(ruleFileStream, "Include target in all.",
                          "all", depends, commands, true);
        }
      
      // Write the rule.
      commands.clear();
      progressDir = lg->GetMakefile()->GetHomeOutputDirectory();
      progressDir += cmake::GetCMakeFilesDirectory();
      
      {
      // TODO: Convert the total progress count to a make variable.
      cmOStringStream progCmd;
      progCmd << "$(CMAKE_COMMAND) -E cmake_progress_start "; 
      // # in target
      progCmd << lg->Convert(progressDir.c_str(),
                              cmLocalGenerator::FULL,
                              cmLocalGenerator::SHELL);
      //
      std::set<cmStdString> emitted;
      progCmd << " " 
              << this->GetTargetTotalNumberOfActions(t->second,
                                                      emitted);
      commands.push_back(progCmd.str());
      }
      std::string tmp = cmake::GetCMakeFilesDirectoryPostSlash();
      tmp += "Makefile2";
      commands.push_back(lg->GetRecursiveMakeCall
                          (tmp.c_str(),localName.c_str()));
      {
      cmOStringStream progCmd;
      progCmd << "$(CMAKE_COMMAND) -E cmake_progress_start "; // # 0
      progCmd << lg->Convert(progressDir.c_str(),
                              cmLocalGenerator::FULL,
                              cmLocalGenerator::SHELL);
      progCmd << " 0";
      commands.push_back(progCmd.str());
      }
      depends.clear();
      depends.push_back("cmake_check_build_system");
      localName = lg->GetRelativeTargetDirectory(t->second);
      localName += "/rule";
      lg->WriteMakeRule(ruleFileStream, 
                        "Build rule for subdir invocation for target.",
                        localName.c_str(), depends, commands, true);
      
      // Add a target with the canonical name (no prefix, suffix or path).
      commands.clear();
      depends.clear();
      depends.push_back(localName);
      lg->WriteMakeRule(ruleFileStream, "Convenience name for target.",
                        t->second.GetName(), depends, commands, true);
      
      // Add rules to prepare the target for installation.
      if(t->second.NeedRelinkBeforeInstall())
        {
        localName = lg->GetRelativeTargetDirectory(t->second);
        localName += "/preinstall";
        depends.clear();
        commands.clear();
        commands.push_back(lg->GetRecursiveMakeCall
                            (makefileName.c_str(), localName.c_str()));
        lg->WriteMakeRule(ruleFileStream, 
                          "Pre-install relink rule for target.",
                          localName.c_str(), depends, commands, true);

        if(!this->IsExcluded(this->LocalGenerators[0], t->second))
          {
          depends.clear();
          depends.push_back(localName);
          commands.clear();
          lg->WriteMakeRule(ruleFileStream, "Prepare target for install.",
                            "preinstall", depends, commands, true);
          }
        }
      
      // add the clean rule
      localName = lg->GetRelativeTargetDirectory(t->second);
      makeTargetName = localName;
      makeTargetName += "/clean";
      depends.clear();
      commands.clear();
      commands.push_back(lg->GetRecursiveMakeCall
                          (makefileName.c_str(), makeTargetName.c_str()));
      lg->WriteMakeRule(ruleFileStream, "clean rule for target.",
                        makeTargetName.c_str(), depends, commands, true);
      commands.clear();
      depends.push_back(makeTargetName);
      lg->WriteMakeRule(ruleFileStream, "clean rule for target.",
                        "clean", depends, commands, true);
      }
    }
}

//----------------------------------------------------------------------------
int cmGlobalUnixMakefileGenerator3
::GetTargetTotalNumberOfActions(cmTarget & target,
                                std::set<cmStdString> &emitted)
{
  // do not double count
  int result = 0;

  if(emitted.insert(target.GetName()).second)
    {
    cmLocalUnixMakefileGenerator3 *lg = 
      static_cast<cmLocalUnixMakefileGenerator3 *>
      (target.GetMakefile()->GetLocalGenerator());
    result = static_cast<int>(lg->ProgressFiles[target.GetName()].size());
    
    TargetDependSet & depends = this->GetTargetDirectDepends(target);
    
    TargetDependSet::iterator i;
    for (i = depends.begin(); i != depends.end(); ++i)
      {
      result += this->GetTargetTotalNumberOfActions(**i, emitted);
      }
    }
  
  return result;
}

unsigned long cmGlobalUnixMakefileGenerator3
::GetNumberOfProgressActionsInAll(cmLocalUnixMakefileGenerator3 *lg)
{
  unsigned long result = 0;
  std::set<cmStdString> emitted;
  std::set<cmTarget *>& targets = this->LocalGeneratorToTargetMap[lg];
  for(std::set<cmTarget *>::iterator t = targets.begin();
      t != targets.end(); ++t)
    {
    result += this->GetTargetTotalNumberOfActions(**t,emitted);
    }
  return result;
}


//----------------------------------------------------------------------------
void
cmGlobalUnixMakefileGenerator3
::AppendGlobalTargetDepends(std::vector<std::string>& depends,
                            cmTarget& target)
{
  TargetDependSet const& depends_set = this->GetTargetDirectDepends(target);
  for(TargetDependSet::const_iterator i = depends_set.begin();
      i != depends_set.end(); ++i)
    {
    // Create the target-level dependency.
    cmTarget const* dep = *i;
    cmLocalUnixMakefileGenerator3* lg3 =
      static_cast<cmLocalUnixMakefileGenerator3*>
      (dep->GetMakefile()->GetLocalGenerator());
    std::string tgtName = lg3->GetRelativeTargetDirectory(*dep);
    tgtName += "/all";
    depends.push_back(tgtName);
    }
}

//----------------------------------------------------------------------------
void cmGlobalUnixMakefileGenerator3::WriteHelpRule
(std::ostream& ruleFileStream, cmLocalUnixMakefileGenerator3 *lg)
{
  // add the help target
  std::string path;
  std::vector<std::string> no_depends;
  std::vector<std::string> commands;
  lg->AppendEcho(commands,"The following are some of the valid targets "
                 "for this Makefile:");
  lg->AppendEcho(commands,"... all (the default if no target is provided)");
  lg->AppendEcho(commands,"... clean");
  lg->AppendEcho(commands,"... depend");
  
  // Keep track of targets already listed.
  std::set<cmStdString> emittedTargets;

  // for each local generator
  unsigned int i;
  cmLocalUnixMakefileGenerator3 *lg2;
  for (i = 0; i < this->LocalGenerators.size(); ++i)
    {
    lg2 = 
      static_cast<cmLocalUnixMakefileGenerator3 *>(this->LocalGenerators[i]);
    // for the passed in makefile or if this is the top Makefile wripte out
    // the targets
    if (lg2 == lg || !lg->GetParent())
      {
      // for each target Generate the rule files for each target.
      cmTargets& targets = lg2->GetMakefile()->GetTargets();
      for(cmTargets::iterator t = targets.begin(); t != targets.end(); ++t)
        {
        if((t->second.GetType() == cmTarget::EXECUTABLE) ||
           (t->second.GetType() == cmTarget::STATIC_LIBRARY) ||
           (t->second.GetType() == cmTarget::SHARED_LIBRARY) ||
           (t->second.GetType() == cmTarget::MODULE_LIBRARY) ||
           (t->second.GetType() == cmTarget::GLOBAL_TARGET) ||
           (t->second.GetType() == cmTarget::UTILITY))
          {
          if(emittedTargets.insert(t->second.GetName()).second)
            {
            path = "... ";
            path += t->second.GetName();
            lg->AppendEcho(commands,path.c_str());
            }
          }
        }
      std::vector<cmStdString> const& localHelp = lg->GetLocalHelp();
      for(std::vector<cmStdString>::const_iterator o = localHelp.begin();
          o != localHelp.end(); ++o)
        {
        path = "... ";
        path += *o;
        lg->AppendEcho(commands, path.c_str());
        }
      }
    }
  lg->WriteMakeRule(ruleFileStream, "Help Target",
                    "help",
                    no_depends, commands, true);
  ruleFileStream << "\n\n";
}


bool cmGlobalUnixMakefileGenerator3
::NeedRequiresStep(cmTarget const& target)
{
  std::set<cmStdString> languages;
  target.GetLanguages(languages);
  for(std::set<cmStdString>::const_iterator l = languages.begin();
      l != languages.end(); ++l)
    {
    std::string var = "CMAKE_NEEDS_REQUIRES_STEP_";
    var += *l;
    var += "_FLAG";
    if(target.GetMakefile()->GetDefinition(var.c_str()))
      {
      return true;
      }
    }
  return false;
}
