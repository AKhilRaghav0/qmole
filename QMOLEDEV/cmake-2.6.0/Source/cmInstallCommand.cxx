/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile: cmInstallCommand.cxx,v $
  Language:  C++
  Date:      $Date: 2008-04-23 16:14:33 $
  Version:   $Revision: 1.45.2.2 $

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "cmInstallCommand.h"

#include "cmInstallDirectoryGenerator.h"
#include "cmInstallFilesGenerator.h"
#include "cmInstallScriptGenerator.h"
#include "cmInstallTargetGenerator.h"
#include "cmInstallExportGenerator.h"
#include "cmInstallCommandArguments.h"

#include <cmsys/Glob.hxx>

static cmInstallTargetGenerator* CreateInstallTargetGenerator(cmTarget& target,
     const cmInstallCommandArguments& args, bool impLib, bool forceOpt = false)
{
  return new cmInstallTargetGenerator(target, args.GetDestination().c_str(), 
                        impLib, args.GetPermissions().c_str(), 
                        args.GetConfigurations(), args.GetComponent().c_str(),
                        args.GetOptional() || forceOpt);
}

static cmInstallFilesGenerator* CreateInstallFilesGenerator(
    const std::vector<std::string>& absFiles, 
    const cmInstallCommandArguments& args, bool programs)
{
  return new cmInstallFilesGenerator(absFiles, args.GetDestination().c_str(), 
                        programs, args.GetPermissions().c_str(), 
                        args.GetConfigurations(), args.GetComponent().c_str(),
                        args.GetRename().c_str(), args.GetOptional());
}


// cmInstallCommand
bool cmInstallCommand::InitialPass(std::vector<std::string> const& args, 
                                   cmExecutionStatus &)
{
  // Allow calling with no arguments so that arguments may be built up
  // using a variable that may be left empty.
  if(args.empty())
    {
    return true;
    }

  // Enable the install target.
  this->Makefile->GetLocalGenerator()
    ->GetGlobalGenerator()->EnableInstallTarget();

  // Switch among the command modes.
  if(args[0] == "SCRIPT")
    {
    return this->HandleScriptMode(args);
    }
  else if(args[0] == "CODE")
    {
    return this->HandleScriptMode(args);
    }
  else if(args[0] == "TARGETS")
    {
    return this->HandleTargetsMode(args);
    }
  else if(args[0] == "FILES")
    {
    return this->HandleFilesMode(args);
    }
  else if(args[0] == "PROGRAMS")
    {
    return this->HandleFilesMode(args);
    }
  else if(args[0] == "DIRECTORY")
    {
    return this->HandleDirectoryMode(args);
    }
  else if(args[0] == "EXPORT")
    {
    return this->HandleExportMode(args);
    }

  // Unknown mode.
  cmStdString e = "called with unknown mode ";
  e += args[0];
  this->SetError(e.c_str());
  return false;
}

//----------------------------------------------------------------------------
bool cmInstallCommand::HandleScriptMode(std::vector<std::string> const& args)
{
  std::string component("Unspecified");
  int componentCount = 0;
  bool doing_script = false;
  bool doing_code = false;

  // Scan the args once for COMPONENT. Only allow one.
  //
  for(size_t i=0; i < args.size(); ++i)
    {
    if(args[i] == "COMPONENT" && i+1 < args.size())
        {
        ++componentCount;
        ++i;
        component = args[i];
        }
    }

  if(componentCount>1)
    {
    this->SetError("given more than one COMPONENT for the SCRIPT or CODE "
      "signature of the INSTALL command. "
      "Use multiple INSTALL commands with one COMPONENT each.");
    return false;
    }

  // Scan the args again, this time adding install generators each time we
  // encounter a SCRIPT or CODE arg:
  //
  for(size_t i=0; i < args.size(); ++i)
    {
    if(args[i] == "SCRIPT")
      {
      doing_script = true;
      doing_code = false;
      }
    else if(args[i] == "CODE")
      {
      doing_script = false;
      doing_code = true;
      }
    else if(args[i] == "COMPONENT")
      {
      doing_script = false;
      doing_code = false;
      }
    else if(doing_script)
      {
      doing_script = false;
      std::string script = args[i];
      if(!cmSystemTools::FileIsFullPath(script.c_str()))
        {
        script = this->Makefile->GetCurrentDirectory();
        script += "/";
        script += args[i];
        }
      if(cmSystemTools::FileIsDirectory(script.c_str()))
        {
        this->SetError("given a directory as value of SCRIPT argument.");
        return false;
        }
      this->Makefile->AddInstallGenerator(
        new cmInstallScriptGenerator(script.c_str(), false,
                                     component.c_str()));
      }
    else if(doing_code)
      {
      doing_code = false;
      std::string code = args[i];
      this->Makefile->AddInstallGenerator(
        new cmInstallScriptGenerator(code.c_str(), true,
                                     component.c_str()));
      }
    }

  if(doing_script)
    {
    this->SetError("given no value for SCRIPT argument.");
    return false;
    }
  if(doing_code)
    {
    this->SetError("given no value for CODE argument.");
    return false;
    }

  //Tell the global generator about any installation component names specified.
  this->Makefile->GetLocalGenerator()->GetGlobalGenerator()
                             ->AddInstallComponent(component.c_str());

  return true;
}


/*struct InstallPart
{
  InstallPart(cmCommandArgumentsHelper* helper, const char* key, 
         cmCommandArgumentGroup* group);
  cmCAStringVector argVector;
  cmInstallCommandArguments args;
};*/

//----------------------------------------------------------------------------
bool cmInstallCommand::HandleTargetsMode(std::vector<std::string> const& args)
{
  // This is the TARGETS mode.
  std::vector<cmTarget*> targets;

  cmCommandArgumentsHelper argHelper;
  cmCommandArgumentGroup group;
  cmCAStringVector genericArgVector      (&argHelper,0);
  cmCAStringVector archiveArgVector      (&argHelper,"ARCHIVE",&group);
  cmCAStringVector libraryArgVector      (&argHelper,"LIBRARY",&group);
  cmCAStringVector runtimeArgVector      (&argHelper,"RUNTIME",&group);
  cmCAStringVector frameworkArgVector    (&argHelper,"FRAMEWORK",&group);
  cmCAStringVector bundleArgVector       (&argHelper,"BUNDLE",&group);
  cmCAStringVector privateHeaderArgVector(&argHelper,"PRIVATE_HEADER",&group);
  cmCAStringVector publicHeaderArgVector (&argHelper,"PUBLIC_HEADER",&group);
  cmCAStringVector resourceArgVector     (&argHelper,"RESOURCE",&group);
  genericArgVector.Follows(0);
  group.Follows(&genericArgVector);

  argHelper.Parse(&args, 0);

  // now parse the generic args (i.e. the ones not specialized on LIBRARY/
  // ARCHIVE, RUNTIME etc. (see above)
  // These generic args also contain the targets and the export stuff
  std::vector<std::string> unknownArgs;
  cmInstallCommandArguments genericArgs;
  cmCAStringVector targetList(&genericArgs.Parser, "TARGETS");
  cmCAString exports(&genericArgs.Parser,"EXPORT", &genericArgs.ArgumentGroup);
  targetList.Follows(0);
  genericArgs.ArgumentGroup.Follows(&targetList);
  genericArgs.Parse(&genericArgVector.GetVector(), &unknownArgs);
  bool success = genericArgs.Finalize();

  cmInstallCommandArguments archiveArgs;
  cmInstallCommandArguments libraryArgs;
  cmInstallCommandArguments runtimeArgs;
  cmInstallCommandArguments frameworkArgs;
  cmInstallCommandArguments bundleArgs;
  cmInstallCommandArguments privateHeaderArgs;
  cmInstallCommandArguments publicHeaderArgs;
  cmInstallCommandArguments resourceArgs;

  // now parse the args for specific parts of the target (e.g. LIBRARY, 
  // RUNTIME, ARCHIVE etc.
  archiveArgs.Parse      (&archiveArgVector.GetVector(),       &unknownArgs);
  libraryArgs.Parse      (&libraryArgVector.GetVector(),       &unknownArgs);
  runtimeArgs.Parse      (&runtimeArgVector.GetVector(),       &unknownArgs);
  frameworkArgs.Parse    (&frameworkArgVector.GetVector(),     &unknownArgs);
  bundleArgs.Parse       (&bundleArgVector.GetVector(),        &unknownArgs);
  privateHeaderArgs.Parse(&privateHeaderArgVector.GetVector(), &unknownArgs);
  publicHeaderArgs.Parse (&publicHeaderArgVector.GetVector(),  &unknownArgs);
  resourceArgs.Parse     (&resourceArgVector.GetVector(),      &unknownArgs);

  if(!unknownArgs.empty())
    {
    // Unknown argument.
    cmOStringStream e;
    e << "TARGETS given unknown argument \"" << unknownArgs[0] << "\".";
    this->SetError(e.str().c_str());
    return false;
    }

  // apply generic args
  archiveArgs.SetGenericArguments(&genericArgs);
  libraryArgs.SetGenericArguments(&genericArgs);
  runtimeArgs.SetGenericArguments(&genericArgs);
  frameworkArgs.SetGenericArguments(&genericArgs);
  bundleArgs.SetGenericArguments(&genericArgs);
  privateHeaderArgs.SetGenericArguments(&genericArgs);
  publicHeaderArgs.SetGenericArguments(&genericArgs);
  resourceArgs.SetGenericArguments(&genericArgs);

  success = success && archiveArgs.Finalize();
  success = success && libraryArgs.Finalize();
  success = success && runtimeArgs.Finalize();
  success = success && frameworkArgs.Finalize();
  success = success && bundleArgs.Finalize();
  success = success && privateHeaderArgs.Finalize();
  success = success && publicHeaderArgs.Finalize();
  success = success && resourceArgs.Finalize();

  if(!success)
    {
    return false;
    }

  // Enforce argument rules too complex to specify for the
  // general-purpose parser.
  if(archiveArgs.GetNamelinkOnly() ||
     runtimeArgs.GetNamelinkOnly() ||
     frameworkArgs.GetNamelinkOnly() ||
     bundleArgs.GetNamelinkOnly() ||
     privateHeaderArgs.GetNamelinkOnly() ||
     publicHeaderArgs.GetNamelinkOnly() ||
     resourceArgs.GetNamelinkOnly())
    {
    this->SetError(
      "TARGETS given NAMELINK_ONLY option not in LIBRARY group.  "
      "The NAMELINK_ONLY option may be specified only following LIBRARY."
      );
    return false;
    }
  if(archiveArgs.GetNamelinkSkip() ||
     runtimeArgs.GetNamelinkSkip() ||
     frameworkArgs.GetNamelinkSkip() ||
     bundleArgs.GetNamelinkSkip() ||
     privateHeaderArgs.GetNamelinkSkip() ||
     publicHeaderArgs.GetNamelinkSkip() ||
     resourceArgs.GetNamelinkSkip())
    {
    this->SetError(
      "TARGETS given NAMELINK_SKIP option not in LIBRARY group.  "
      "The NAMELINK_SKIP option may be specified only following LIBRARY."
      );
    return false;
    }
  if(libraryArgs.GetNamelinkOnly() && libraryArgs.GetNamelinkSkip())
    {
    this->SetError(
      "TARGETS given NAMELINK_ONLY and NAMELINK_SKIP.  "
      "At most one of these two options may be specified."
      );
    return false;
    }

  // Select the mode for installing symlinks to versioned shared libraries.
  cmInstallTargetGenerator::NamelinkModeType
    namelinkMode = cmInstallTargetGenerator::NamelinkModeNone;
  if(libraryArgs.GetNamelinkOnly())
    {
    namelinkMode = cmInstallTargetGenerator::NamelinkModeOnly;
    }
  else if(libraryArgs.GetNamelinkSkip())
    {
    namelinkMode = cmInstallTargetGenerator::NamelinkModeSkip;
    }

  // Check if there is something to do.
  if(targetList.GetVector().empty())
    {
    return true;
    }

  // Check whether this is a DLL platform.
  bool dll_platform = (this->Makefile->IsOn("WIN32") ||
                       this->Makefile->IsOn("CYGWIN") ||
                       this->Makefile->IsOn("MINGW"));

  for(std::vector<std::string>::const_iterator 
      targetIt=targetList.GetVector().begin();
      targetIt!=targetList.GetVector().end();
      ++targetIt)
    {
      // Lookup this target in the current directory.
      if(cmTarget* target=this->Makefile->FindTarget(targetIt->c_str()))
        {
        // Found the target.  Check its type.
        if(target->GetType() != cmTarget::EXECUTABLE &&
           target->GetType() != cmTarget::STATIC_LIBRARY &&
           target->GetType() != cmTarget::SHARED_LIBRARY &&
           target->GetType() != cmTarget::MODULE_LIBRARY)
          {
          cmOStringStream e;
          e << "TARGETS given target \"" << (*targetIt)
            << "\" which is not an executable, library, or module.";
          this->SetError(e.str().c_str());
          return false;
          }
        // Store the target in the list to be installed.
        targets.push_back(target);
        }
      else
        {
        // Did not find the target.
        cmOStringStream e;
        e << "TARGETS given target \"" << (*targetIt)
          << "\" which does not exist in this directory.";
        this->SetError(e.str().c_str());
        return false;
        }
    }

  // Generate install script code to install the given targets.
  for(std::vector<cmTarget*>::iterator ti = targets.begin();
      ti != targets.end(); ++ti)
    {
    // Handle each target type.
    cmTarget& target = *(*ti);
    cmInstallTargetGenerator* archiveGenerator = 0;
    cmInstallTargetGenerator* libraryGenerator = 0;
    cmInstallTargetGenerator* runtimeGenerator = 0;
    cmInstallTargetGenerator* frameworkGenerator = 0;
    cmInstallTargetGenerator* bundleGenerator = 0;
    cmInstallFilesGenerator* privateHeaderGenerator = 0;
    cmInstallFilesGenerator* publicHeaderGenerator = 0;
    cmInstallFilesGenerator* resourceGenerator = 0;

    // Track whether this is a namelink-only rule.
    bool namelinkOnly = false;

    switch(target.GetType())
      {
      case cmTarget::SHARED_LIBRARY:
        {
        // Shared libraries are handled differently on DLL and non-DLL
        // platforms.  All windows platforms are DLL platforms including 
        // cygwin.  Currently no other platform is a DLL platform.
        if(dll_platform)
          {
          // When in namelink only mode skip all libraries on Windows.
          if(namelinkMode == cmInstallTargetGenerator::NamelinkModeOnly)
            {
            continue;
            }

          // This is a DLL platform.
          if(!archiveArgs.GetDestination().empty())
            {
            // The import library uses the ARCHIVE properties.
            archiveGenerator = CreateInstallTargetGenerator(target, 
                                                            archiveArgs, true);
            }
          if(!runtimeArgs.GetDestination().empty())
            {
            // The DLL uses the RUNTIME properties.
            runtimeGenerator = CreateInstallTargetGenerator(target, 
                                                           runtimeArgs, false);
            }
          if ((archiveGenerator==0) && (runtimeGenerator==0))
            {
            this->SetError("Library TARGETS given no DESTINATION!");
            return false;
            }
          }
        else
          {
          // This is a non-DLL platform.
          // If it is marked with FRAMEWORK property use the FRAMEWORK set of
          // INSTALL properties. Otherwise, use the LIBRARY properties.
          if(target.IsFrameworkOnApple())
            {
            // When in namelink only mode skip frameworks.
            if(namelinkMode == cmInstallTargetGenerator::NamelinkModeOnly)
              {
              continue;
              }

            // Use the FRAMEWORK properties.
            if (!frameworkArgs.GetDestination().empty())
              {
              frameworkGenerator = CreateInstallTargetGenerator(target, 
                                                         frameworkArgs, false);
              }
            else
              {
              cmOStringStream e;
              e << "TARGETS given no FRAMEWORK DESTINATION for shared library "
                "FRAMEWORK target \"" << target.GetName() << "\".";
              this->SetError(e.str().c_str());
              return false;
              }
            }
          else
            {
            // The shared library uses the LIBRARY properties.
            if (!libraryArgs.GetDestination().empty())
              {
              libraryGenerator = CreateInstallTargetGenerator(target, 
                                                           libraryArgs, false);
              libraryGenerator->SetNamelinkMode(namelinkMode);
              namelinkOnly =
                (namelinkMode == cmInstallTargetGenerator::NamelinkModeOnly);
              }
            else
              {
              cmOStringStream e;
              e << "TARGETS given no LIBRARY DESTINATION for shared library "
                "target \"" << target.GetName() << "\".";
              this->SetError(e.str().c_str());
              return false;
              }
            }
          }
        }
        break;
      case cmTarget::STATIC_LIBRARY:
        {
        // Static libraries use ARCHIVE properties.
        if (!archiveArgs.GetDestination().empty())
          {
          archiveGenerator = CreateInstallTargetGenerator(target, archiveArgs, 
                                                          false);
          }
        else
          {
          cmOStringStream e;
          e << "TARGETS given no ARCHIVE DESTINATION for static library "
            "target \"" << target.GetName() << "\".";
          this->SetError(e.str().c_str());
          return false;
          }
        }
        break;
      case cmTarget::MODULE_LIBRARY:
        {
        // Modules use LIBRARY properties.
        if (!libraryArgs.GetDestination().empty())
          {
          libraryGenerator = CreateInstallTargetGenerator(target, libraryArgs, 
                                                          false);
          libraryGenerator->SetNamelinkMode(namelinkMode);
          namelinkOnly =
            (namelinkMode == cmInstallTargetGenerator::NamelinkModeOnly);
          }
        else
          {
          cmOStringStream e;
          e << "TARGETS given no LIBRARY DESTINATION for module target \""
            << target.GetName() << "\".";
          this->SetError(e.str().c_str());
          return false;
          }
        }
        break;
      case cmTarget::EXECUTABLE:
        {
        if(target.IsAppBundleOnApple())
          {
          // Application bundles use the BUNDLE properties.
          if (!bundleArgs.GetDestination().empty())
            {
            bundleGenerator = CreateInstallTargetGenerator(target, bundleArgs, 
                                                           false);
            }
          else if(!runtimeArgs.GetDestination().empty())
            {
            bool failure = false;
            if(this->CheckCMP0006(failure))
              {
              // For CMake 2.4 compatibility fallback to the RUNTIME
              // properties.
              bundleGenerator =
                CreateInstallTargetGenerator(target, runtimeArgs, false);
              }
            else if(failure)
              {
              return false;
              }
            }
          if(!bundleGenerator)
            {
            cmOStringStream e;
            e << "TARGETS given no BUNDLE DESTINATION for MACOSX_BUNDLE "
                 "executable target \"" << target.GetName() << "\".";
            this->SetError(e.str().c_str());
            return false;
            }
          }
        else
          {
          // Executables use the RUNTIME properties.
          if (!runtimeArgs.GetDestination().empty())
            {
            runtimeGenerator = CreateInstallTargetGenerator(target, 
                                                           runtimeArgs, false);
            }
          else
            {
            cmOStringStream e;
            e << "TARGETS given no RUNTIME DESTINATION for executable "
                 "target \"" << target.GetName() << "\".";
            this->SetError(e.str().c_str());
            return false;
            }
          }

        // On DLL platforms an executable may also have an import
        // library.  Install it to the archive destination if it
        // exists.
        if(dll_platform && !archiveArgs.GetDestination().empty() &&
           target.IsExecutableWithExports())
          {
          // The import library uses the ARCHIVE properties.
          archiveGenerator = CreateInstallTargetGenerator(target, 
                                                      archiveArgs, true, true);
          }
        }
        break;
      default:
        // This should never happen due to the above type check.
        // Ignore the case.
        break;
      }

//       if(target.GetProperty("ASSOCIATED_FILES");

  // These well-known sets of files are installed *automatically* for FRAMEWORK
  // SHARED library targets on the Mac as part of installing the FRAMEWORK.
  // For other target types or on other platforms, they are not installed
  // automatically and so we need to create install files generators for them.
  //
  bool createInstallGeneratorsForTargetFileSets = true;

  if(target.IsFrameworkOnApple())
    {
    createInstallGeneratorsForTargetFileSets = false;
    }

  if(createInstallGeneratorsForTargetFileSets && !namelinkOnly)
    {
    const char* files = target.GetProperty("PRIVATE_HEADER");
    if ((files) && (*files))
      {
      std::vector<std::string> relFiles;
      cmSystemTools::ExpandListArgument(files, relFiles);
      std::vector<std::string> absFiles;
      if (!this->MakeFilesFullPath("PRIVATE_HEADER", relFiles, absFiles))
        {
        return false;
        }

      // Create the files install generator.
      if (!privateHeaderArgs.GetDestination().empty())
        {
        privateHeaderGenerator = CreateInstallFilesGenerator(absFiles, 
                                                     privateHeaderArgs, false);
        }
      else
        {
        cmOStringStream e;
        e << "INSTALL TARGETS - target " << target.GetName() << " has "
          << "PRIVATE_HEADER files but no PRIVATE_HEADER DESTINATION.";
        cmSystemTools::Message(e.str().c_str(), "Warning");
        }
      }

    files = target.GetProperty("PUBLIC_HEADER");
    if ((files) && (*files))
      {
      std::vector<std::string> relFiles;
      cmSystemTools::ExpandListArgument(files, relFiles);
      std::vector<std::string> absFiles;
      if (!this->MakeFilesFullPath("PUBLIC_HEADER", relFiles, absFiles))
        {
        return false;
        }

      // Create the files install generator.
      if (!publicHeaderArgs.GetDestination().empty())
        {
        publicHeaderGenerator = CreateInstallFilesGenerator(absFiles, 
                                                      publicHeaderArgs, false);
        }
      else
        {
        cmOStringStream e;
        e << "INSTALL TARGETS - target " << target.GetName() << " has "
          << "PUBLIC_HEADER files but no PUBLIC_HEADER DESTINATION.";
        cmSystemTools::Message(e.str().c_str(), "Warning");
        }
      }

    files = target.GetProperty("RESOURCE");
    if ((files) && (*files))
      {
      std::vector<std::string> relFiles;
      cmSystemTools::ExpandListArgument(files, relFiles);
      std::vector<std::string> absFiles;
      if (!this->MakeFilesFullPath("RESOURCE", relFiles, absFiles))
        {
        return false;
        }

      // Create the files install generator.
      if (!resourceArgs.GetDestination().empty())
        {
        resourceGenerator = CreateInstallFilesGenerator(absFiles, 
                                                        resourceArgs, false);
        }
      else
        {
        cmOStringStream e;
        e << "INSTALL TARGETS - target " << target.GetName() << " has "
          << "RESOURCE files but no RESOURCE DESTINATION.";
        cmSystemTools::Message(e.str().c_str(), "Warning");
        }
      }
    }

    this->Makefile->AddInstallGenerator(archiveGenerator);
    this->Makefile->AddInstallGenerator(libraryGenerator);
    this->Makefile->AddInstallGenerator(runtimeGenerator);
    this->Makefile->AddInstallGenerator(frameworkGenerator);
    this->Makefile->AddInstallGenerator(bundleGenerator);
    this->Makefile->AddInstallGenerator(privateHeaderGenerator);
    this->Makefile->AddInstallGenerator(publicHeaderGenerator);
    this->Makefile->AddInstallGenerator(resourceGenerator);

    // Add this install rule to an export if one was specified and
    // this is not a namelink-only rule.
    if(!exports.GetString().empty() && !namelinkOnly)
      {
      this->Makefile->GetLocalGenerator()->GetGlobalGenerator()
        ->AddTargetToExports(exports.GetCString(), &target, 
                             archiveGenerator, runtimeGenerator,
                             libraryGenerator, frameworkGenerator,
                             bundleGenerator, publicHeaderGenerator);
      }
    }

  // Tell the global generator about any installation component names specified
  this->Makefile->GetLocalGenerator()->GetGlobalGenerator()
                     ->AddInstallComponent(archiveArgs.GetComponent().c_str());
  this->Makefile->GetLocalGenerator()->GetGlobalGenerator()
                     ->AddInstallComponent(libraryArgs.GetComponent().c_str());
  this->Makefile->GetLocalGenerator()->GetGlobalGenerator()
                     ->AddInstallComponent(runtimeArgs.GetComponent().c_str());
  this->Makefile->GetLocalGenerator()->GetGlobalGenerator()
                   ->AddInstallComponent(frameworkArgs.GetComponent().c_str());
  this->Makefile->GetLocalGenerator()->GetGlobalGenerator()
                      ->AddInstallComponent(bundleArgs.GetComponent().c_str());
  this->Makefile->GetLocalGenerator()->GetGlobalGenerator()
               ->AddInstallComponent(privateHeaderArgs.GetComponent().c_str());
  this->Makefile->GetLocalGenerator()->GetGlobalGenerator()
                ->AddInstallComponent(publicHeaderArgs.GetComponent().c_str());
  this->Makefile->GetLocalGenerator()->GetGlobalGenerator()
                    ->AddInstallComponent(resourceArgs.GetComponent().c_str());

  return true;
}

//----------------------------------------------------------------------------
bool cmInstallCommand::HandleFilesMode(std::vector<std::string> const& args)
{
  // This is the FILES mode.
  bool programs = (args[0] == "PROGRAMS");
  cmInstallCommandArguments ica;
  cmCAStringVector files(&ica.Parser, programs ? "PROGRAMS" : "FILES");
  files.Follows(0);
  ica.ArgumentGroup.Follows(&files);
  std::vector<std::string> unknownArgs;
  ica.Parse(&args, &unknownArgs);

  if(!unknownArgs.empty())
    {
    // Unknown argument.
    cmOStringStream e;
    e << args[0] << " given unknown argument \"" << unknownArgs[0] << "\".";
    this->SetError(e.str().c_str());
    return false;
    }
  
  // Check if there is something to do.
  if(files.GetVector().empty())
    {
    return true;
    }

  if(!ica.GetRename().empty() && files.GetVector().size() > 1)
    {
    // The rename option works only with one file.
    cmOStringStream e;
    e << args[0] << " given RENAME option with more than one file.";
    this->SetError(e.str().c_str());
    return false;
    }

  std::vector<std::string> absFiles;
  if (!this->MakeFilesFullPath(args[0].c_str(), files.GetVector(), absFiles))
    {
    return false;
    }

  if (!ica.Finalize())
    {
    return false;
    }

  if(ica.GetDestination().empty())
    {
    // A destination is required.
    cmOStringStream e;
    e << args[0] << " given no DESTINATION!";
    this->SetError(e.str().c_str());
    return false;
    }

  // Create the files install generator.
  this->Makefile->AddInstallGenerator(
                         CreateInstallFilesGenerator(absFiles, ica, programs));

  //Tell the global generator about any installation component names specified.
  this->Makefile->GetLocalGenerator()->GetGlobalGenerator()
                             ->AddInstallComponent(ica.GetComponent().c_str());

  return true;
}

//----------------------------------------------------------------------------
bool
cmInstallCommand::HandleDirectoryMode(std::vector<std::string> const& args)
{
  bool doing_dirs = true;
  bool doing_destination = false;
  bool doing_pattern = false;
  bool doing_regex = false;
  bool doing_permissions_file = false;
  bool doing_permissions_dir = false;
  bool doing_permissions_match = false;
  bool doing_configurations = false;
  bool doing_component = false;
  bool in_match_mode = false;
  std::vector<std::string> dirs;
  const char* destination = 0;
  std::string permissions_file;
  std::string permissions_dir;
  std::vector<std::string> configurations;
  std::string component = "Unspecified";
  std::string literal_args;
  for(unsigned int i=1; i < args.size(); ++i)
    {
    if(args[i] == "DESTINATION")
      {
      if(in_match_mode)
        {
        cmOStringStream e;
        e << args[0] << " does not allow \""
          << args[i] << "\" after PATTERN or REGEX.";
        this->SetError(e.str().c_str());
        return false;
        }

      // Switch to setting the destination property.
      doing_dirs = false;
      doing_destination = true;
      doing_pattern = false;
      doing_regex = false;
      doing_permissions_file = false;
      doing_permissions_dir = false;
      doing_configurations = false;
      doing_component = false;
      }
    else if(args[i] == "PATTERN")
      {
      // Switch to a new pattern match rule.
      doing_dirs = false;
      doing_destination = false;
      doing_pattern = true;
      doing_regex = false;
      doing_permissions_file = false;
      doing_permissions_dir = false;
      doing_permissions_match = false;
      doing_configurations = false;
      doing_component = false;
      in_match_mode = true;
      }
    else if(args[i] == "REGEX")
      {
      // Switch to a new regex match rule.
      doing_dirs = false;
      doing_destination = false;
      doing_pattern = false;
      doing_regex = true;
      doing_permissions_file = false;
      doing_permissions_dir = false;
      doing_permissions_match = false;
      doing_configurations = false;
      doing_component = false;
      in_match_mode = true;
      }
    else if(args[i] == "EXCLUDE")
      {
      // Add this property to the current match rule.
      if(!in_match_mode || doing_pattern || doing_regex)
        {
        cmOStringStream e;
        e << args[0] << " does not allow \""
          << args[i] << "\" before a PATTERN or REGEX is given.";
        this->SetError(e.str().c_str());
        return false;
        }
      literal_args += " EXCLUDE";
      doing_permissions_match = false;
      }
    else if(args[i] == "PERMISSIONS")
      {
      if(!in_match_mode)
        {
        cmOStringStream e;
        e << args[0] << " does not allow \""
          << args[i] << "\" before a PATTERN or REGEX is given.";
        this->SetError(e.str().c_str());
        return false;
        }

      // Switch to setting the current match permissions property.
      literal_args += " PERMISSIONS";
      doing_permissions_match = true;
      }
    else if(args[i] == "FILE_PERMISSIONS")
      {
      if(in_match_mode)
        {
        cmOStringStream e;
        e << args[0] << " does not allow \""
          << args[i] << "\" after PATTERN or REGEX.";
        this->SetError(e.str().c_str());
        return false;
        }

      // Switch to setting the file permissions property.
      doing_dirs = false;
      doing_destination = false;
      doing_pattern = false;
      doing_regex = false;
      doing_permissions_file = true;
      doing_permissions_dir = false;
      doing_configurations = false;
      doing_component = false;
      }
    else if(args[i] == "DIRECTORY_PERMISSIONS")
      {
      if(in_match_mode)
        {
        cmOStringStream e;
        e << args[0] << " does not allow \""
          << args[i] << "\" after PATTERN or REGEX.";
        this->SetError(e.str().c_str());
        return false;
        }

      // Switch to setting the directory permissions property.
      doing_dirs = false;
      doing_destination = false;
      doing_pattern = false;
      doing_regex = false;
      doing_permissions_file = false;
      doing_permissions_dir = true;
      doing_configurations = false;
      doing_component = false;
      }
    else if(args[i] == "USE_SOURCE_PERMISSIONS")
      {
      if(in_match_mode)
        {
        cmOStringStream e;
        e << args[0] << " does not allow \""
          << args[i] << "\" after PATTERN or REGEX.";
        this->SetError(e.str().c_str());
        return false;
        }

      // Add this option literally.
      doing_dirs = false;
      doing_destination = false;
      doing_pattern = false;
      doing_regex = false;
      doing_permissions_file = false;
      doing_permissions_dir = false;
      doing_configurations = false;
      doing_component = false;
      literal_args += " USE_SOURCE_PERMISSIONS";
      }
    else if(args[i] == "FILES_MATCHING")
      {
      if(in_match_mode)
        {
        cmOStringStream e;
        e << args[0] << " does not allow \""
          << args[i] << "\" after PATTERN or REGEX.";
        this->SetError(e.str().c_str());
        return false;
        }

      // Add this option literally.
      doing_dirs = false;
      doing_destination = false;
      doing_pattern = false;
      doing_regex = false;
      doing_permissions_file = false;
      doing_permissions_dir = false;
      doing_permissions_match = false;
      doing_configurations = false;
      doing_component = false;
      literal_args += " FILES_MATCHING";
      }
    else if(args[i] == "CONFIGURATIONS")
      {
      if(in_match_mode)
        {
        cmOStringStream e;
        e << args[0] << " does not allow \""
          << args[i] << "\" after PATTERN or REGEX.";
        this->SetError(e.str().c_str());
        return false;
        }

      // Switch to setting the configurations property.
      doing_dirs = false;
      doing_destination = false;
      doing_pattern = false;
      doing_regex = false;
      doing_permissions_file = false;
      doing_permissions_dir = false;
      doing_configurations = true;
      doing_component = false;
      }
    else if(args[i] == "COMPONENT")
      {
      if(in_match_mode)
        {
        cmOStringStream e;
        e << args[0] << " does not allow \""
          << args[i] << "\" after PATTERN or REGEX.";
        this->SetError(e.str().c_str());
        return false;
        }

      // Switch to setting the component property.
      doing_dirs = false;
      doing_destination = false;
      doing_pattern = false;
      doing_regex = false;
      doing_permissions_file = false;
      doing_permissions_dir = false;
      doing_configurations = false;
      doing_component = true;
      }
    else if(doing_dirs)
      {
      // Convert this directory to a full path.
      std::string dir = args[i];
      if(!cmSystemTools::FileIsFullPath(dir.c_str()))
        {
        dir = this->Makefile->GetCurrentDirectory();
        dir += "/";
        dir += args[i];
        }

      // Make sure the name is a directory.
      if(cmSystemTools::FileExists(dir.c_str()) &&
         !cmSystemTools::FileIsDirectory(dir.c_str()))
        {
        cmOStringStream e;
        e << args[0] << " given non-directory \""
          << args[i] << "\" to install.";
        this->SetError(e.str().c_str());
        return false;
        }

      // Store the directory for installation.
      dirs.push_back(dir);
      }
    else if(doing_configurations)
      {
      configurations.push_back(args[i]);
      }
    else if(doing_destination)
      {
      destination = args[i].c_str();
      doing_destination = false;
      }
    else if(doing_pattern)
      {
      // Convert the pattern to a regular expression.  Require a
      // leading slash and trailing end-of-string in the matched
      // string to make sure the pattern matches only whole file
      // names.
      literal_args += " REGEX \"/";
      std::string regex = cmsys::Glob::PatternToRegex(args[i], false);
      cmSystemTools::ReplaceString(regex, "\\", "\\\\");
      literal_args += regex;
      literal_args += "$\"";
      doing_pattern = false;
      }
    else if(doing_regex)
      {
      literal_args += " REGEX \"";
    // Match rules are case-insensitive on some platforms.
#if defined(_WIN32) || defined(__APPLE__) || defined(__CYGWIN__)
      std::string regex = cmSystemTools::LowerCase(args[i]);
#else
      std::string regex = args[i];
#endif
      cmSystemTools::ReplaceString(regex, "\\", "\\\\");
      literal_args += regex;
      literal_args += "\"";
      doing_regex = false;
      }
    else if(doing_component)
      {
      component = args[i];
      doing_component = false;
      }
    else if(doing_permissions_file)
     {
     // Check the requested permission.
     if(!cmInstallCommandArguments::CheckPermissions(args[i],permissions_file))
        {
        cmOStringStream e;
        e << args[0] << " given invalid file permission \""
          << args[i] << "\".";
        this->SetError(e.str().c_str());
        return false;
        }
      }
    else if(doing_permissions_dir)
      {
      // Check the requested permission.
      if(!cmInstallCommandArguments::CheckPermissions(args[i],permissions_dir))
        {
        cmOStringStream e;
        e << args[0] << " given invalid directory permission \""
          << args[i] << "\".";
        this->SetError(e.str().c_str());
        return false;
        }
      }
    else if(doing_permissions_match)
      {
      // Check the requested permission.
      if(!cmInstallCommandArguments::CheckPermissions(args[i], literal_args))
        {
        cmOStringStream e;
        e << args[0] << " given invalid permission \""
          << args[i] << "\".";
        this->SetError(e.str().c_str());
        return false;
        }
      }
    else
      {
      // Unknown argument.
      cmOStringStream e;
      e << args[0] << " given unknown argument \"" << args[i] << "\".";
      this->SetError(e.str().c_str());
      return false;
      }
    }

  // Support installing an empty directory.
  if(dirs.empty() && destination)
    {
    dirs.push_back("");
    }

  // Check if there is something to do.
  if(dirs.empty())
    {
    return true;
    }
  if(!destination)
    {
    // A destination is required.
    cmOStringStream e;
    e << args[0] << " given no DESTINATION!";
    this->SetError(e.str().c_str());
    return false;
    }

  // Create the directory install generator.
  this->Makefile->AddInstallGenerator(
    new cmInstallDirectoryGenerator(dirs, destination,
                                    permissions_file.c_str(),
                                    permissions_dir.c_str(),
                                    configurations,
                                    component.c_str(),
                                    literal_args.c_str()));

  // Tell the global generator about any installation component names
  // specified.
  this->Makefile->GetLocalGenerator()->GetGlobalGenerator()
    ->AddInstallComponent(component.c_str());

  return true;
}

//----------------------------------------------------------------------------
bool cmInstallCommand::HandleExportMode(std::vector<std::string> const& args)
{
  // This is the EXPORT mode.
  cmInstallCommandArguments ica;
  cmCAString exp(&ica.Parser, "EXPORT");
  cmCAString name_space(&ica.Parser, "NAMESPACE", &ica.ArgumentGroup);
  cmCAString filename(&ica.Parser, "FILE", &ica.ArgumentGroup);
  exp.Follows(0);

  ica.ArgumentGroup.Follows(&exp);
  std::vector<std::string> unknownArgs;
  ica.Parse(&args, &unknownArgs);

  if (!unknownArgs.empty())
    {
    // Unknown argument.
    cmOStringStream e;
    e << args[0] << " given unknown argument \"" << unknownArgs[0] << "\".";
    this->SetError(e.str().c_str());
    return false;
    }

  if (!ica.Finalize())
    {
    return false;
    }

  // Make sure there is a destination.
  if(ica.GetDestination().empty())
    {
    // A destination is required.
    cmOStringStream e;
    e << args[0] << " given no DESTINATION!";
    this->SetError(e.str().c_str());
    return false;
    }

  // Check the file name.
  std::string fname = filename.GetString();
  if(fname.find_first_of(":/\\") != fname.npos)
    {
    cmOStringStream e;
    e << args[0] << " given invalid export file name \"" << fname << "\".  "
      << "The FILE argument may not contain a path.  "
      << "Specify the path in the DESTINATION argument.";
    this->SetError(e.str().c_str());
    return false;
    }

  // Check the file extension.
  if(!fname.empty() &&
     cmSystemTools::GetFilenameLastExtension(fname) != ".cmake")
    {
    cmOStringStream e;
    e << args[0] << " given invalid export file name \"" << fname << "\".  "
      << "The FILE argument must specify a name ending in \".cmake\".";
    this->SetError(e.str().c_str());
    return false;
    }

  // Construct the file name.
  if(fname.empty())
    {
    fname = exp.GetString();
    fname += ".cmake";

    if(fname.find_first_of(":/\\") != fname.npos)
      {
      cmOStringStream e;
      e << args[0] << " given export name \"" << exp.GetString() << "\".  "
        << "This name cannot be safely converted to a file name.  "
        << "Specify a different export name or use the FILE option to set "
        << "a file name explicitly.";
      this->SetError(e.str().c_str());
      return false;
      }
    }

  // Create the export install generator.
  cmInstallExportGenerator* exportGenerator =
    new cmInstallExportGenerator(
      exp.GetCString(), ica.GetDestination().c_str(),
      ica.GetPermissions().c_str(), ica.GetConfigurations(),
      ica.GetComponent().c_str(), fname.c_str(),
      name_space.GetCString(), this->Makefile);
  this->Makefile->AddInstallGenerator(exportGenerator);

  return true;
}

bool cmInstallCommand::MakeFilesFullPath(const char* modeName,
                                      const std::vector<std::string>& relFiles,
                                      std::vector<std::string>& absFiles)
{
  for(std::vector<std::string>::const_iterator fileIt = relFiles.begin();
      fileIt != relFiles.end();
      ++fileIt)
    {
    std::string file = (*fileIt);
    if(!cmSystemTools::FileIsFullPath(file.c_str()))
      {
      file = this->Makefile->GetCurrentDirectory();
      file += "/";
      file += *fileIt;
      }

    // Make sure the file is not a directory.
    if(cmSystemTools::FileIsDirectory(file.c_str()))
      {
      cmOStringStream e;
      e << modeName << " given directory \"" << (*fileIt) << "\" to install.";
      this->SetError(e.str().c_str());
      return false;
      }
    // Store the file for installation.
    absFiles.push_back(file);
    }
  return true;
}

//----------------------------------------------------------------------------
bool cmInstallCommand::CheckCMP0006(bool& failure)
{
  switch(this->Makefile->GetPolicyStatus(cmPolicies::CMP0006))
    {
    case cmPolicies::WARN:
      {
      this->Makefile->IssueMessage(
        cmake::AUTHOR_WARNING,
        this->Makefile->GetPolicies()->GetPolicyWarning(cmPolicies::CMP0006)
        );
      }
    case cmPolicies::OLD:
      // OLD behavior is to allow compatibility
      return true;
    case cmPolicies::NEW:
      // NEW behavior is to disallow compatibility
      break;
    case cmPolicies::REQUIRED_IF_USED:
    case cmPolicies::REQUIRED_ALWAYS:
      failure = true;
      this->Makefile->IssueMessage(
        cmake::FATAL_ERROR,
        this->Makefile->GetPolicies()
        ->GetRequiredPolicyError(cmPolicies::CMP0006)
        );
      break;
    }
  return false;
}
