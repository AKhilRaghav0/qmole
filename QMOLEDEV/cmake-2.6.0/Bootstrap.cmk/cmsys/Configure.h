/*
 * Generated by /home/chris/cmake-2.6.0/bootstrap
 * Version:     $Revision: 1.113 $
 *
 * Source directory: /home/chris/cmake-2.6.0
 * Binary directory: /home/chris/cmake-2.6.0/Bootstrap.cmk
 *
 * C compiler:   /usr/bin/gcc
 * C flags:      
 *
 * C++ compiler: g++
 * C++ flags:    
 *
 * Make:         gmake
 *
 * Sources:
 *   cmake    cmakemain   cmakewizard    cmCommandArgumentLexer   cmCommandArgumentParser   cmCommandArgumentParserHelper   cmDepends   cmDependsC   cmDocumentationFormatter   cmDocumentationFormatterText   cmPolicies   cmProperty   cmPropertyMap   cmPropertyDefinition   cmPropertyDefinitionMap   cmMakeDepend   cmMakefile   cmExportFileGenerator   cmExportInstallFileGenerator   cmInstallDirectoryGenerator   cmGeneratedFileStream   cmGlobalGenerator   cmLocalGenerator   cmInstallGenerator   cmInstallExportGenerator   cmInstallFilesGenerator   cmInstallScriptGenerator   cmInstallTargetGenerator   cmSourceFile   cmSourceFileLocation   cmSystemTools   cmVersion   cmFileTimeComparison   cmGlobalUnixMakefileGenerator3   cmLocalUnixMakefileGenerator3   cmMakefileExecutableTargetGenerator   cmMakefileLibraryTargetGenerator   cmMakefileTargetGenerator   cmMakefileUtilityTargetGenerator   cmBootstrapCommands   cmCommands   cmTarget   cmTest   cmCustomCommand   cmDocumentVariables   cmCacheManager   cmListFileCache   cmComputeLinkDepends   cmComputeLinkInformation   cmOrderDirectories   cmComputeTargetDepends   cmComputeComponentGraph   cmExprLexer   cmExprParser   cmExprParserHelper    cmListFileLexer   
 * kwSys Sources:
 *   Directory   Glob   RegularExpression   SystemTools     ProcessUNIX     String     System 
 */

/*=========================================================================

  Program:   KWSys - Kitware System Library
  Module:    $RCSfile: Configure.h.in,v $

  Copyright (c) Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef cmsys_Configure_h
#define cmsys_Configure_h

/* If we are building a kwsys .c or .cxx file, let it use the kwsys
   namespace.  When not building a kwsys source file these macros are
   temporarily defined inside the headers that use them.  */
#if defined(KWSYS_NAMESPACE)
# define kwsys_ns(x) cmsys##x
# define kwsysEXPORT cmsys_EXPORT
#endif

/* If we are building a kwsys .c or .cxx file, suppress the Microsoft
   deprecation warnings.  */
#if defined(KWSYS_NAMESPACE)
# ifndef _CRT_NONSTDC_NO_DEPRECATE
#  define _CRT_NONSTDC_NO_DEPRECATE
# endif
# ifndef _CRT_SECURE_NO_DEPRECATE
#  define _CRT_SECURE_NO_DEPRECATE
# endif
# ifndef _SCL_SECURE_NO_DEPRECATE
#  define _SCL_SECURE_NO_DEPRECATE
# endif
#endif

/* Whether Large File Support is requested.  */
#define cmsys_LFS_REQUESTED 0

/* Whether Large File Support is available.  */
#if cmsys_LFS_REQUESTED
# define cmsys_LFS_AVAILABLE 0
#endif

/* Setup Large File Support if requested.  */
#if cmsys_LFS_REQUESTED
  /* Since LFS is requested this header must be included before system
     headers whether or not LFS is available. */
# if 0 && (defined(_SYS_TYPES_H) || defined(_SYS_TYPES_INCLUDED))
#  error "cmsys/Configure.h must be included before sys/types.h"
# endif
  /* Enable the large file API if it is available.  */
# if cmsys_LFS_AVAILABLE && \
     !defined(cmsys_LFS_NO_DEFINES)
#  if !defined(_LARGEFILE_SOURCE) && \
      !defined(cmsys_LFS_NO_DEFINE_LARGEFILE_SOURCE)
#   define _LARGEFILE_SOURCE
#  endif
#  if !defined(_LARGEFILE64_SOURCE) && \
      !defined(cmsys_LFS_NO_DEFINE_LARGEFILE64_SOURCE)
#   define _LARGEFILE64_SOURCE
#  endif
#  if !defined(_LARGE_FILES) && \
      !defined(cmsys_LFS_NO_DEFINE_LARGE_FILES)
#   define _LARGE_FILES
#  endif
#  if !defined(_FILE_OFFSET_BITS) && \
      !defined(cmsys_LFS_NO_DEFINE_FILE_OFFSET_BITS)
#   define _FILE_OFFSET_BITS 64
#  endif
#  if 0 && (defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS < 64)
#   error "_FILE_OFFSET_BITS must be defined to at least 64"
#  endif
# endif
#endif

/* Setup the export macro.  */
#if defined(_WIN32) && 0
# if defined(cmsys_EXPORTS)
#  define cmsys_EXPORT __declspec(dllexport)
# else
#  define cmsys_EXPORT __declspec(dllimport)
# endif
#else
# define cmsys_EXPORT
#endif

/* Enable warnings that are off by default but are useful.  */
#if !defined(cmsys_NO_WARNING_ENABLE)
# if defined(_MSC_VER)
#  pragma warning ( default : 4263 ) /* no override, call convention differs */
# endif
#endif

/* Disable warnings that are on by default but occur in valid code.  */
#if !defined(cmsys_NO_WARNING_DISABLE)
# if defined(_MSC_VER)
#  pragma warning (disable: 4097) /* typedef is synonym for class */
#  pragma warning (disable: 4127) /* conditional expression is constant */
#  pragma warning (disable: 4244) /* possible loss in conversion */
#  pragma warning (disable: 4251) /* missing DLL-interface */
#  pragma warning (disable: 4305) /* truncation from type1 to type2 */
#  pragma warning (disable: 4309) /* truncation of constant value */
#  pragma warning (disable: 4514) /* unreferenced inline function */
#  pragma warning (disable: 4706) /* assignment in conditional expression */
#  pragma warning (disable: 4710) /* function not inlined */
#  pragma warning (disable: 4786) /* identifier truncated in debug info */
# endif
#endif

/* MSVC 6.0 in release mode will warn about code it produces with its
   optimizer.  Disable the warnings specifically for this
   configuration.  Real warnings will be revealed by a debug build or
   by other compilers.  */
#if !defined(cmsys_NO_WARNING_DISABLE_BOGUS)
# if defined(_MSC_VER) && (_MSC_VER < 1300) && defined(NDEBUG)
#  pragma warning ( disable : 4701 ) /* Variable may be used uninitialized.  */
#  pragma warning ( disable : 4702 ) /* Unreachable code.  */
# endif
#endif

#endif
