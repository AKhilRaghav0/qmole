/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile: cmCTestBuildAndTestHandler.cxx,v $
  Language:  C++
  Date:      $Date: 2007-09-17 14:40:57 $
  Version:   $Revision: 1.20 $

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "cmCTestBuildAndTestHandler.h"

#include "cmSystemTools.h"
#include "cmCTest.h"
#include "cmake.h"
#include "cmGlobalGenerator.h"
#include <cmsys/Process.h>
#include "cmCTestTestHandler.h"

//----------------------------------------------------------------------
cmCTestBuildAndTestHandler::cmCTestBuildAndTestHandler()
{
  this->BuildTwoConfig         = false;
  this->BuildNoClean           = false;
  this->BuildNoCMake           = false;
  this->Timeout = 0;
}

//----------------------------------------------------------------------
void cmCTestBuildAndTestHandler::Initialize()
{
  this->BuildTargets.erase(
    this->BuildTargets.begin(), this->BuildTargets.end());
  this->Superclass::Initialize();
}

//----------------------------------------------------------------------
const char* cmCTestBuildAndTestHandler::GetOutput()
{
  return this->Output.c_str();
}
//----------------------------------------------------------------------
int cmCTestBuildAndTestHandler::ProcessHandler()
{
  this->Output = "";
  std::string output;
  cmSystemTools::ResetErrorOccuredFlag();
  int retv = this->RunCMakeAndTest(&this->Output);
  cmSystemTools::ResetErrorOccuredFlag();
  return retv;
}

//----------------------------------------------------------------------
int cmCTestBuildAndTestHandler::RunCMake(std::string* outstring,
  cmOStringStream &out, std::string &cmakeOutString, std::string &cwd,
  cmake *cm)
{
  unsigned int k;
  std::vector<std::string> args;
  args.push_back(this->CTest->GetCMakeExecutable());
  args.push_back(this->SourceDir);
  if(this->BuildGenerator.size())
    {
    std::string generator = "-G";
    generator += this->BuildGenerator;
    args.push_back(generator);
    }
  
  const char* config = 0;
  if ( this->CTest->GetConfigType().size() > 0 )
    {
    config = this->CTest->GetConfigType().c_str();
    }
#ifdef CMAKE_INTDIR
  config = CMAKE_INTDIR;
#endif
  
  if ( config )
    {
    std::string btype
      = "-DCMAKE_BUILD_TYPE:STRING=" + std::string(config);
    args.push_back(btype);
    }

  for(k=0; k < this->BuildOptions.size(); ++k)
    {
    args.push_back(this->BuildOptions[k]);
    }
  if (cm->Run(args) != 0)
    {
    out << "Error: cmake execution failed\n";
    out << cmakeOutString << "\n";
    // return to the original directory
    cmSystemTools::ChangeDirectory(cwd.c_str());
    if(outstring)
      {
      *outstring = out.str();
      }
    else
      {
      cmCTestLog(this->CTest, ERROR_MESSAGE, out.str() << std::endl);
      }
    return 1;
    }
  // do another config?
  if(this->BuildTwoConfig)
    {
    if (cm->Run(args) != 0)
      {
      out << "Error: cmake execution failed\n";
      out << cmakeOutString << "\n";
      // return to the original directory
      cmSystemTools::ChangeDirectory(cwd.c_str());
      if(outstring)
        {
        *outstring = out.str();
        }
      else
        {
        cmCTestLog(this->CTest, ERROR_MESSAGE, out.str() << std::endl);
        }
      return 1;
      }
    }
  out << "======== CMake output     ======\n";
  out << cmakeOutString;
  out << "======== End CMake output ======\n";
  return 0;
}

//----------------------------------------------------------------------
void CMakeMessageCallback(const char* m, const char*, bool&, void* s)
{
  std::string* out = (std::string*)s;
  *out += m;
  *out += "\n";
}

//----------------------------------------------------------------------
void CMakeStdoutCallback(const char* m, int len, void* s)
{
  std::string* out = (std::string*)s;
  out->append(m, len);
}

//----------------------------------------------------------------------
int cmCTestBuildAndTestHandler::RunCMakeAndTest(std::string* outstring)
{
  unsigned int k;
  std::string cmakeOutString;
  cmSystemTools::SetErrorCallback(CMakeMessageCallback, &cmakeOutString);
  cmSystemTools::SetStdoutCallback(CMakeStdoutCallback, &cmakeOutString);
  cmOStringStream out;

  // if the generator and make program are not specified then it is an error
  if (!this->BuildGenerator.size() || !this->BuildMakeProgram.size())
    {
    if(outstring)
      {
      *outstring =
        "--build-and-test requires that both the generator and makeprogram "
        "be provided using the --build-generator and --build-makeprogram "
        "command line options. ";
      }
    return 1;
    }

  if ( this->CTest->GetConfigType().size() == 0 &&
       this->ConfigSample.size())
    {
    // use the config sample to set the ConfigType
    std::string fullPath;
    std::string resultingConfig;
    std::vector<std::string> extraPaths;
    std::vector<std::string> failed;
    fullPath = 
      cmCTestTestHandler::FindExecutable(this->CTest, 
                                         this->ConfigSample.c_str(),
                                         resultingConfig,
                                         extraPaths,
                                         failed);
    if (fullPath.size() && resultingConfig.size())
      {
      this->CTest->SetConfigType(resultingConfig.c_str());
      }
    out << "Using config sample with results: "
        << fullPath << " and " << resultingConfig << std::endl;
    }
  
  // we need to honor the timeout specified, the timeout include cmake, build
  // and test time
  double clock_start = cmSystemTools::GetTime();

  // make sure the binary dir is there
  std::string cwd = cmSystemTools::GetCurrentWorkingDirectory();
  out << "Internal cmake changing into directory: "
    << this->BinaryDir << std::endl;
  if (!cmSystemTools::FileIsDirectory(this->BinaryDir.c_str()))
    {
    cmSystemTools::MakeDirectory(this->BinaryDir.c_str());
    }
  cmSystemTools::ChangeDirectory(this->BinaryDir.c_str());

  // should we cmake?
  cmake cm;
  cm.SetGlobalGenerator(cm.CreateGlobalGenerator(
      this->BuildGenerator.c_str()));

  if(!this->BuildNoCMake)
    {
    // do the cmake step, no timeout here since it is not a sub process
    if (this->RunCMake(outstring,out,cmakeOutString,cwd,&cm))
      {
      return 1;
      }
    }

  // do the build
  std::vector<std::string>::iterator tarIt;
  if ( this->BuildTargets.size() == 0 )
    {
    this->BuildTargets.push_back("");
    }
  for ( tarIt = this->BuildTargets.begin(); 
        tarIt != this->BuildTargets.end(); ++ tarIt )
    {
    double remainingTime = 0;
    if (this->Timeout)
      {
      remainingTime = this->Timeout - cmSystemTools::GetTime() + clock_start;
      if (remainingTime <= 0)
        {
        if(outstring)
          {
          *outstring = "--build-and-test timeout exceeded. ";
          }
        return 1;
        }
      }
    std::string output;
    const char* config = 0;
    if ( this->CTest->GetConfigType().size() > 0 )
      {
      config = this->CTest->GetConfigType().c_str();
      }
#ifdef CMAKE_INTDIR
    config = CMAKE_INTDIR;
#endif
    if(!config)
      {
      config = "Debug";
      }
    int retVal = cm.GetGlobalGenerator()->Build(
      this->SourceDir.c_str(), this->BinaryDir.c_str(),
      this->BuildProject.c_str(), tarIt->c_str(),
      &output, this->BuildMakeProgram.c_str(),
      config,
      !this->BuildNoClean, 
      false, remainingTime);
    out << output;
    // if the build failed then return
    if (retVal)
      {
      if(outstring)
        {
        *outstring =  out.str();
        }
      return 1;
      }
    }
  if(outstring)
    {
    *outstring =  out.str();
    }

  // if no test was specified then we are done
  if (!this->TestCommand.size())
    {
    return 0;
    }

  // now run the compiled test if we can find it
  // store the final location in fullPath
  std::string fullPath;
  std::string resultingConfig;
  std::vector<std::string> extraPaths;
  // if this->ExecutableDirectory is set try that as well
  if (this->ExecutableDirectory.size())
    {
    std::string tempPath = this->ExecutableDirectory;
    tempPath += "/";
    tempPath += this->TestCommand;
    extraPaths.push_back(tempPath);
    }
  std::vector<std::string> failed;
  fullPath = 
    cmCTestTestHandler::FindExecutable(this->CTest, 
                                       this->TestCommand.c_str(),
                                       resultingConfig,
                                       extraPaths,
                                       failed);
  
  if(!cmSystemTools::FileExists(fullPath.c_str()))
    {
    out << "Could not find path to executable, perhaps it was not built: "
      << this->TestCommand << "\n";
    out << "tried to find it in these places:\n";
    out << fullPath.c_str() << "\n";
    for(unsigned int i=0; i < failed.size(); ++i)
      {
      out << failed[i] << "\n";
      }
    if(outstring)
      {
      *outstring =  out.str();
      }
    else
      {
      cmCTestLog(this->CTest, ERROR_MESSAGE, out.str());
      }
    // return to the original directory
    cmSystemTools::ChangeDirectory(cwd.c_str());
    return 1;
    }

  std::vector<const char*> testCommand;
  testCommand.push_back(fullPath.c_str());
  for(k=0; k < this->TestCommandArgs.size(); ++k)
    {
    testCommand.push_back(this->TestCommandArgs[k].c_str());
    }
  testCommand.push_back(0);
  std::string outs;
  int retval = 0;
  // run the test from the this->BuildRunDir if set
  if(this->BuildRunDir.size())
    {
    out << "Run test in directory: " << this->BuildRunDir << "\n";
    cmSystemTools::ChangeDirectory(this->BuildRunDir.c_str());
    }
  out << "Running test executable: " << fullPath << " ";
  for(k=0; k < this->TestCommandArgs.size(); ++k)
    {
    out << this->TestCommandArgs[k] << " ";
    }
  out << "\n";

  // how much time is remaining
  double remainingTime = 0;
  if (this->Timeout)
    {
    remainingTime = this->Timeout - cmSystemTools::GetTime() + clock_start;
    if (remainingTime <= 0)
      {
      if(outstring)
        {
        *outstring = "--build-and-test timeout exceeded. ";
        }
      return 1;
      }
    }
  
  int runTestRes = this->CTest->RunTest(testCommand, &outs, &retval, 0, 
                                        remainingTime);

  if(runTestRes != cmsysProcess_State_Exited || retval != 0)
    {
    out << "Failed to run test command: " << testCommand[0] << "\n";
    retval = 1;
    }

  out << outs << "\n";
  if(outstring)
    {
    *outstring = out.str();
    }
  else
    {
    cmCTestLog(this->CTest, OUTPUT, out.str() << std::endl);
    }
  return retval;
}

//----------------------------------------------------------------------
int cmCTestBuildAndTestHandler::ProcessCommandLineArguments(
  const std::string& currentArg, size_t& idx,
  const std::vector<std::string>& allArgs)
{
  // --build-and-test options
  if(currentArg.find("--build-and-test",0) == 0 && idx < allArgs.size() - 1)
    {
    if(idx+2 < allArgs.size())
      {
      idx++;
      this->SourceDir = allArgs[idx];
      idx++;
      this->BinaryDir = allArgs[idx];
      // dir must exist before CollapseFullPath is called
      cmSystemTools::MakeDirectory(this->BinaryDir.c_str());
      this->BinaryDir
        = cmSystemTools::CollapseFullPath(this->BinaryDir.c_str());
      this->SourceDir
        = cmSystemTools::CollapseFullPath(this->SourceDir.c_str());
      }
    else
      {
      cmCTestLog(this->CTest, ERROR_MESSAGE,
        "--build-and-test must have source and binary dir" << std::endl);
      return 0;
      }
    }
  if(currentArg.find("--build-target",0) == 0 && idx < allArgs.size() - 1)
    {
    idx++;
    this->BuildTargets.push_back(allArgs[idx]);
    }
  if(currentArg.find("--build-nocmake",0) == 0)
    {
    this->BuildNoCMake = true;
    }
  if(currentArg.find("--build-run-dir",0) == 0 && idx < allArgs.size() - 1)
    {
    idx++;
    this->BuildRunDir = allArgs[idx];
    }
  if(currentArg.find("--build-two-config",0) == 0)
    {
    this->BuildTwoConfig = true;
    }
  if(currentArg.find("--build-exe-dir",0) == 0 && idx < allArgs.size() - 1)
    {
    idx++;
    this->ExecutableDirectory = allArgs[idx];
    }
  if(currentArg.find("--test-timeout",0) == 0 && idx < allArgs.size() - 1)
    {
    idx++;
    this->Timeout = atof(allArgs[idx].c_str());
    }
  if(currentArg.find("--build-generator",0) == 0 && idx < allArgs.size() - 1)
    {
    idx++;
    this->BuildGenerator = allArgs[idx];
    }
  if(currentArg.find("--build-project",0) == 0 && idx < allArgs.size() - 1)
    {
    idx++;
    this->BuildProject = allArgs[idx];
    }
  if(currentArg.find("--build-makeprogram",0) == 0 &&
    idx < allArgs.size() - 1)
    {
    idx++;
    this->BuildMakeProgram = allArgs[idx];
    }
  if(currentArg.find("--build-config-sample",0) == 0 &&
    idx < allArgs.size() - 1)
    {
    idx++;
    this->ConfigSample = allArgs[idx];
    }
  if(currentArg.find("--build-noclean",0) == 0)
    {
    this->BuildNoClean = true;
    }
  if(currentArg.find("--build-options",0) == 0 && idx < allArgs.size() - 1)
    {
    ++idx;
    bool done = false;
    while(idx < allArgs.size() && !done)
      {
      this->BuildOptions.push_back(allArgs[idx]);
      if(idx+1 < allArgs.size()
         && (allArgs[idx+1] == "--build-target" ||
             allArgs[idx+1] == "--test-command"))
        {
        done = true;
        }
      else
        {
        ++idx;
        }
      }
    }
  if(currentArg.find("--test-command",0) == 0 && idx < allArgs.size() - 1)
    {
    ++idx;
    this->TestCommand = allArgs[idx];
    while(idx+1 < allArgs.size())
      {
      ++idx;
      this->TestCommandArgs.push_back(allArgs[idx]);
      }
    }
  return 1;
}

