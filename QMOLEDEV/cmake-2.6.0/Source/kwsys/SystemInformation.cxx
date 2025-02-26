/*=========================================================================

  Program:   BatchMake
  Module:    $RCSfile: SystemInformation.cxx,v $
  Language:  C++
  Date:      $Date: 2008-05-04 22:07:27 $
  Version:   $Revision: 1.22.2.2 $
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "kwsysPrivate.h"
#include KWSYS_HEADER(FundamentalType.h)
#include KWSYS_HEADER(stl/string)
#include KWSYS_HEADER(stl/vector)
#include KWSYS_HEADER(ios/iosfwd)
#include KWSYS_HEADER(SystemInformation.hxx)
#include KWSYS_HEADER(Process.h)
#include KWSYS_HEADER(ios/iostream)
#include KWSYS_HEADER(ios/sstream)
// Work-around CMake dependency scanning limitation.  This must
// duplicate the above list of headers.
#if 0
# include "FundamentalType.h.in"
# include "SystemInformation.hxx.in"
# include "Process.h.in"
# include "Configure.hxx.in"
# include "kwsys_stl.hxx.in"
# include "kwsys_stl_vector.in"
# include "kwsys_stl_iosfwd.in"
# include "kwsys_ios_sstream.h.in"
# include "kwsys_ios_iostream.h.in"
#endif


#ifndef WIN32
# include <sys/utsname.h> // int uname(struct utsname *buf);
#endif

#ifdef _WIN32
# include <windows.h>
#endif

#ifdef __linux
# include <sys/procfs.h>
# include <sys/types.h>
# include <unistd.h>
# include <fcntl.h>
# include <ctype.h> // int isdigit(int c);
# include <errno.h> // extern int errno;
# include <sys/time.h>
#elif __hpux
# include <sys/param.h>
# include <sys/pstat.h>
#endif

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



namespace KWSYS_NAMESPACE
{

// Create longlong
#if KWSYS_USE_LONG_LONG
  typedef long long LongLong;
#elif KWSYS_USE___INT64
  typedef __int64 LongLong;
#else
# error "No Long Long"
#endif

//  Define SystemInformationImplementation class
typedef  void (*DELAY_FUNC)(unsigned int uiMS);
  
class SystemInformationImplementation 
{

public:
  SystemInformationImplementation ();
  ~SystemInformationImplementation ();

  const char * GetVendorString();
  const char * GetVendorID();
  kwsys_stl::string GetTypeID();
  kwsys_stl::string GetFamilyID();
  kwsys_stl::string GetModelID();
  kwsys_stl::string GetSteppingCode();
  const char * GetExtendedProcessorName();
  const char * GetProcessorSerialNumber();
  int GetProcessorCacheSize();
  int GetLogicalProcessorsPerPhysical();
  float GetProcessorClockFrequency();
  int GetProcessorAPICID();
  int GetProcessorCacheXSize(long int);
  bool DoesCPUSupportFeature(long int);
  
  const char * GetOSName();
  const char * GetHostname();
  const char * GetOSRelease();
  const char * GetOSVersion();
  const char * GetOSPlatform();

  bool Is64Bits();

  unsigned int GetNumberOfLogicalCPU(); // per physical cpu
  unsigned int GetNumberOfPhysicalCPU();

  bool DoesCPUSupportCPUID();

  // Retrieve memory information in megabyte.
  unsigned long GetTotalVirtualMemory();
  unsigned long GetAvailableVirtualMemory();
  unsigned long GetTotalPhysicalMemory();
  unsigned long GetAvailablePhysicalMemory();  

  /** Run the different checks */
  void RunCPUCheck();
  void RunOSCheck();
  void RunMemoryCheck();
public:
#define VENDOR_STRING_LENGTH    (12 + 1)
#define CHIPNAME_STRING_LENGTH    (48 + 1)
#define SERIALNUMBER_STRING_LENGTH  (29 + 1)

  typedef struct tagID 
    {
    int Type;
    int Family;
    int Model;
    int Revision;
    int ExtendedFamily;
    int ExtendedModel;
    char ProcessorName[CHIPNAME_STRING_LENGTH];
    char Vendor[VENDOR_STRING_LENGTH];
    char SerialNumber[SERIALNUMBER_STRING_LENGTH];
    } ID;

  typedef struct tagCPUPowerManagement 
    {
    bool HasVoltageID;
    bool HasFrequencyID;
    bool HasTempSenseDiode;
    } CPUPowerManagement;

  typedef struct tagCPUExtendedFeatures 
    {
    bool Has3DNow;
    bool Has3DNowPlus;
    bool SupportsMP;
    bool HasMMXPlus;
    bool HasSSEMMX;
    bool SupportsHyperthreading;
    int LogicalProcessorsPerPhysical;
    int APIC_ID;
    CPUPowerManagement PowerManagement;
    } CPUExtendedFeatures;  
  
  typedef struct CPUtagFeatures 
    {
    bool HasFPU;
    bool HasTSC;
    bool HasMMX;
    bool HasSSE;
    bool HasSSEFP;
    bool HasSSE2;
    bool HasIA64;
    bool HasAPIC;
    bool HasCMOV;
    bool HasMTRR;
    bool HasACPI;
    bool HasSerial;
    bool HasThermal;
    int CPUSpeed;
    int L1CacheSize;
    int L2CacheSize;
    int L3CacheSize;
    CPUExtendedFeatures ExtendedFeatures;
    } CPUFeatures;
    
  enum Manufacturer 
    {
    AMD, Intel, NSC, UMC, Cyrix, NexGen, IDT, Rise, Transmeta, Sun, UnknownManufacturer
    };
protected:

  // Functions.
  bool RetrieveCPUFeatures();
  bool RetrieveCPUIdentity();
  bool RetrieveCPUCacheDetails();
  bool RetrieveClassicalCPUCacheDetails();
  bool RetrieveCPUClockSpeed();
  bool RetrieveClassicalCPUClockSpeed();
  bool RetrieveCPUExtendedLevelSupport(int);
  bool RetrieveExtendedCPUFeatures();
  bool RetrieveProcessorSerialNumber();
  bool RetrieveCPUPowerManagement();
  bool RetrieveClassicalCPUIdentity();
  bool RetrieveExtendedCPUIdentity();
  
  Manufacturer  ChipManufacturer;
  CPUFeatures   Features;
  ID            ChipID;
  float         CPUSpeedInMHz;
  unsigned int  NumberOfLogicalCPU;
  unsigned int  NumberOfPhysicalCPU;

  int CPUCount();
  unsigned char LogicalCPUPerPhysicalCPU();
  unsigned char GetAPICId();
  unsigned int IsHyperThreadingSupported();
  LongLong GetCyclesDifference(DELAY_FUNC, unsigned int);

  // For Linux
  int RetreiveInformationFromCpuInfoFile();
  kwsys_stl::string ExtractValueFromCpuInfoFile(kwsys_stl::string buffer,
                                          const char* word, size_t init=0);

  static void Delay (unsigned int);
  static void DelayOverhead (unsigned int);

  void FindManufacturer();

  // For Mac
  bool ParseSysCtl();
  kwsys_stl::string ExtractValueFromSysCtl(const char* word);
  kwsys_stl::string SysCtlBuffer;

  // For Solaris
  bool QuerySolarisInfo();
  kwsys_stl::string ParseValueFromKStat(const char* arguments);
  kwsys_stl::string RunProcess(kwsys_stl::vector<const char*> args);

  // Evaluate the memory information.
  int QueryMemory();
  unsigned long TotalVirtualMemory;
  unsigned long AvailableVirtualMemory;
  unsigned long TotalPhysicalMemory;
  unsigned long AvailablePhysicalMemory;

  size_t CurrentPositionInFile;

  // Operating System information
  bool QueryOSInformation();
  kwsys_stl::string OSName;
  kwsys_stl::string Hostname;
  kwsys_stl::string OSRelease;
  kwsys_stl::string OSVersion;
  kwsys_stl::string OSPlatform; 
};
  
  
  
  

SystemInformation::SystemInformation()
{
  this->Implementation = new SystemInformationImplementation;
}

SystemInformation::~SystemInformation ()
{
  delete this->Implementation;
}

const char * SystemInformation::GetVendorString()
{
  return this->Implementation->GetVendorString();
}
const char * SystemInformation::GetVendorID()
{
  return this->Implementation->GetVendorID();
}
kwsys_stl::string SystemInformation::GetTypeID()
{
  return this->Implementation->GetTypeID();
}
kwsys_stl::string SystemInformation::GetFamilyID()
{
  return this->Implementation->GetFamilyID();
}
kwsys_stl::string SystemInformation::GetModelID()
{
  return this->Implementation->GetModelID();
}
kwsys_stl::string SystemInformation::GetSteppingCode()
{
  return this->Implementation->GetSteppingCode();
}
const char * SystemInformation::GetExtendedProcessorName()
{
  return this->Implementation->GetExtendedProcessorName();
}
const char * SystemInformation::GetProcessorSerialNumber()
{
  return this->Implementation->GetProcessorSerialNumber();
}
int SystemInformation::GetProcessorCacheSize()
{
  return this->Implementation->GetProcessorCacheSize();
}
int SystemInformation::GetLogicalProcessorsPerPhysical()
{
  return this->Implementation->GetLogicalProcessorsPerPhysical();
}
float SystemInformation::GetProcessorClockFrequency()
{
  return this->Implementation->GetProcessorClockFrequency();
}
int SystemInformation::GetProcessorAPICID()
{
  return this->Implementation->GetProcessorAPICID();
}
int SystemInformation::GetProcessorCacheXSize(long int l)
{
  return this->Implementation->GetProcessorCacheXSize(l);
}
bool SystemInformation::DoesCPUSupportFeature(long int i)
{
  return this->Implementation->DoesCPUSupportFeature(i);
}

const char * SystemInformation::GetOSName()
{
  return this->Implementation->GetOSName();
}
const char * SystemInformation::GetHostname()
{
  return this->Implementation->GetHostname();
}
const char * SystemInformation::GetOSRelease()
{
  return this->Implementation->GetOSRelease();
}
const char * SystemInformation::GetOSVersion()
{
  return this->Implementation->GetOSVersion();
}
const char * SystemInformation::GetOSPlatform()
{
  return this->Implementation->GetOSPlatform();
}

bool SystemInformation::Is64Bits()
{
  return this->Implementation->Is64Bits();
}

unsigned int SystemInformation::GetNumberOfLogicalCPU() // per physical cpu
{
  return this->Implementation->GetNumberOfLogicalCPU();
}
unsigned int SystemInformation::GetNumberOfPhysicalCPU()
{
  return this->Implementation->GetNumberOfPhysicalCPU();
}

bool SystemInformation::DoesCPUSupportCPUID()
{
  return this->Implementation->DoesCPUSupportCPUID();
}

// Retrieve memory information in megabyte.
unsigned long SystemInformation::GetTotalVirtualMemory()
{
  return this->Implementation->GetTotalVirtualMemory();
}
unsigned long SystemInformation::GetAvailableVirtualMemory()
{
  return this->Implementation->GetAvailableVirtualMemory();
}
unsigned long SystemInformation::GetTotalPhysicalMemory()
{
  return this->Implementation->GetTotalPhysicalMemory();
}

unsigned long SystemInformation::GetAvailablePhysicalMemory()
{
  return this->Implementation->GetAvailablePhysicalMemory();
}

/** Run the different checks */
void SystemInformation::RunCPUCheck()
{
  this->Implementation->RunCPUCheck();
}
void SystemInformation::RunOSCheck()
{
  this->Implementation->RunOSCheck();
}
void SystemInformation::RunMemoryCheck()
{
  this->Implementation->RunMemoryCheck();
}



// --------------------------------------------------------------
// SystemInformationImplementation starts here

#if defined(_MSC_VER) && (_MSC_VER >= 1300) && !defined(_WIN64)
#define USE_ASM_INSTRUCTIONS 1
#else
#define USE_ASM_INSTRUCTIONS 0
#endif

#define STORE_TLBCACHE_INFO(x,y)  x = (x < y) ? y : x
#define TLBCACHE_INFO_UNITS      (15)
#define CLASSICAL_CPU_FREQ_LOOP    10000000
#define RDTSC_INSTRUCTION      _asm _emit 0x0f _asm _emit 0x31

#define CPUID_AWARE_COMPILER
#ifdef CPUID_AWARE_COMPILER
  #define CPUID_INSTRUCTION    cpuid
#else
  #define CPUID_INSTRUCTION    _asm _emit 0x0f _asm _emit 0xa2
#endif

#define MMX_FEATURE          0x00000001
#define MMX_PLUS_FEATURE      0x00000002
#define SSE_FEATURE          0x00000004
#define SSE2_FEATURE        0x00000008
#define AMD_3DNOW_FEATURE      0x00000010
#define AMD_3DNOW_PLUS_FEATURE    0x00000020
#define IA64_FEATURE        0x00000040
#define MP_CAPABLE          0x00000080
#define HYPERTHREAD_FEATURE      0x00000100
#define SERIALNUMBER_FEATURE    0x00000200
#define APIC_FEATURE        0x00000400
#define SSE_FP_FEATURE        0x00000800
#define SSE_MMX_FEATURE        0x00001000
#define CMOV_FEATURE        0x00002000
#define MTRR_FEATURE        0x00004000
#define L1CACHE_FEATURE        0x00008000
#define L2CACHE_FEATURE        0x00010000
#define L3CACHE_FEATURE        0x00020000
#define ACPI_FEATURE        0x00040000
#define THERMALMONITOR_FEATURE    0x00080000
#define TEMPSENSEDIODE_FEATURE    0x00100000
#define FREQUENCYID_FEATURE      0x00200000
#define VOLTAGEID_FREQUENCY      0x00400000

// Status Flag
#define HT_NOT_CAPABLE           0
#define HT_ENABLED               1
#define HT_DISABLED              2
#define HT_SUPPORTED_NOT_ENABLED 3
#define HT_CANNOT_DETECT         4

// EDX[28]  Bit 28 is set if HT is supported
#define HT_BIT             0x10000000   

// EAX[11:8] Bit 8-11 contains family processor ID.
#define FAMILY_ID          0x0F00
#define PENTIUM4_ID        0x0F00         
// EAX[23:20] Bit 20-23 contains extended family processor ID
#define EXT_FAMILY_ID      0x0F00000 
// EBX[23:16] Bit 16-23 in ebx contains the number of logical
#define NUM_LOGICAL_BITS   0x00FF0000  
// processors per physical processor when execute cpuid with 
// eax set to 1
// EBX[31:24] Bits 24-31 (8 bits) return the 8-bit unique 
#define INITIAL_APIC_ID_BITS  0xFF000000  
// initial APIC ID for the processor this code is running on.
// Default value = 0xff if HT is not supported


  
SystemInformationImplementation::SystemInformationImplementation()
{
  this->TotalVirtualMemory = 0;
  this->AvailableVirtualMemory = 0;
  this->TotalPhysicalMemory = 0;
  this->AvailablePhysicalMemory = 0;
  this->CurrentPositionInFile = 0;
  this->ChipManufacturer = UnknownManufacturer;
  memset(&this->Features, 0, sizeof(CPUFeatures));
  memset(&this->ChipID, 0, sizeof(ID));
  this->CPUSpeedInMHz = 0;
  this->NumberOfLogicalCPU = 0;
  this->NumberOfPhysicalCPU = 0;
  this->OSName = "";
  this->Hostname = "";
  this->OSRelease = "";
  this->OSVersion = "";
  this->OSPlatform = "";
}

SystemInformationImplementation::~SystemInformationImplementation()
{
}

void SystemInformationImplementation::RunCPUCheck()
{
#ifdef WIN32
  // Check to see if this processor supports CPUID.
  if (DoesCPUSupportCPUID()) 
    {
    // Retrieve the CPU details.
    RetrieveCPUIdentity();
    RetrieveCPUFeatures();
    if (!RetrieveCPUClockSpeed())
      {
      RetrieveClassicalCPUClockSpeed();
      }

    // Attempt to retrieve cache information.
    if (!RetrieveCPUCacheDetails()) 
      {
      RetrieveClassicalCPUCacheDetails();
      }
    // Retrieve the extended CPU details.
    if (!RetrieveExtendedCPUIdentity()) 
      {
      RetrieveClassicalCPUIdentity();
      }
    RetrieveExtendedCPUFeatures();

    // Now attempt to retrieve the serial number (if possible).
    RetrieveProcessorSerialNumber();
    }
  this->CPUCount();
#elif defined(__APPLE__)
  this->ParseSysCtl();
#elif defined (__SVR4) && defined (__sun)
  this->QuerySolarisInfo();
#else
  this->RetreiveInformationFromCpuInfoFile();
#endif
}

void SystemInformationImplementation::RunOSCheck()
{
  this->QueryOSInformation();
}
 
void SystemInformationImplementation::RunMemoryCheck()
{
#if defined(__APPLE__)
  this->ParseSysCtl();
#elif defined (__SVR4) && defined (__sun)
  this->QuerySolarisInfo();
#else
  this->QueryMemory();
#endif
}

/** Get the vendor string */
const char * SystemInformationImplementation::GetVendorString()
{
  return this->ChipID.Vendor;
}

/** Get the OS Name */
const char * SystemInformationImplementation::GetOSName()
{
  return this->OSName.c_str();
}

/** Get the hostname */
const char* SystemInformationImplementation::GetHostname()
{
  return this->Hostname.c_str();
}

/** Get the OS release */
const char* SystemInformationImplementation::GetOSRelease()
{
  return this->OSRelease.c_str();
}

/** Get the OS version */
const char* SystemInformationImplementation::GetOSVersion()
{
  return this->OSVersion.c_str();
}

/** Get the OS platform */
const char* SystemInformationImplementation::GetOSPlatform()
{
  return this->OSPlatform.c_str();
}

/** Get the vendor ID */
const char * SystemInformationImplementation::GetVendorID()
{
  // Return the vendor ID.
  switch (this->ChipManufacturer) 
    {
    case Intel:
      return "Intel Corporation";
    case AMD:
      return "Advanced Micro Devices";
    case NSC:
      return "National Semiconductor";
    case Cyrix:
      return "Cyrix Corp., VIA Inc.";
    case NexGen:
      return "NexGen Inc., Advanced Micro Devices";
    case IDT:
      return "IDT\\Centaur, Via Inc.";
    case UMC:
      return "United Microelectronics Corp.";
    case Rise:
      return "Rise";
    case Transmeta:
      return "Transmeta";
    case Sun:
      return "Sun Microelectronics";
    default:
      return "Unknown Manufacturer";
    }
}

/** Return the type ID of the CPU */
kwsys_stl::string SystemInformationImplementation::GetTypeID()
{
  kwsys_ios::ostringstream str;
  str << this->ChipID.Type;
  return str.str();
}

/** Return the family of the CPU present */
kwsys_stl::string SystemInformationImplementation::GetFamilyID()
{
  kwsys_ios::ostringstream str;
  str << this->ChipID.Family;
  return str.str();
}

// Return the model of CPU present */
kwsys_stl::string SystemInformationImplementation::GetModelID()
{
  kwsys_ios::ostringstream str;
  str << this->ChipID.Model;
  return str.str();
}

/** Return the stepping code of the CPU present. */
kwsys_stl::string SystemInformationImplementation::GetSteppingCode()
{ 
  kwsys_ios::ostringstream str;
  str << this->ChipID.Revision;
  return str.str();
}

/** Return the stepping code of the CPU present. */
const char * SystemInformationImplementation::GetExtendedProcessorName()
{
  return this->ChipID.ProcessorName;
}
  
/** Return the serial number of the processor 
 *  in hexadecimal: xxxx-xxxx-xxxx-xxxx-xxxx-xxxx. */
const char * SystemInformationImplementation::GetProcessorSerialNumber()
{
  return this->ChipID.SerialNumber;
}

/** Return the logical processors per physical */
int SystemInformationImplementation::GetLogicalProcessorsPerPhysical()
{
  return this->Features.ExtendedFeatures.LogicalProcessorsPerPhysical;
}

/** Return the processor clock frequency. */
float SystemInformationImplementation::GetProcessorClockFrequency()
{
  return this->CPUSpeedInMHz;
}

/**  Return the APIC ID. */
int SystemInformationImplementation::GetProcessorAPICID()
{
  return this->Features.ExtendedFeatures.APIC_ID;
}

/** Return the L1 cache size. */
int SystemInformationImplementation::GetProcessorCacheSize()
{
  return this->Features.L1CacheSize;
}

/** Return the chosen cache size. */
int SystemInformationImplementation::GetProcessorCacheXSize(long int dwCacheID)
{
  switch (dwCacheID) 
    {
    case L1CACHE_FEATURE:
      return this->Features.L1CacheSize;
    case L2CACHE_FEATURE:
      return this->Features.L2CacheSize;
    case L3CACHE_FEATURE:
      return this->Features.L3CacheSize;
    }
  return -1;
}

bool SystemInformationImplementation::DoesCPUSupportFeature(long int dwFeature)
{
  bool bHasFeature = false;

  // Check for MMX instructions.
  if (((dwFeature & MMX_FEATURE) != 0) && this->Features.HasMMX) bHasFeature = true;

  // Check for MMX+ instructions.
  if (((dwFeature & MMX_PLUS_FEATURE) != 0) && this->Features.ExtendedFeatures.HasMMXPlus) bHasFeature = true;

  // Check for SSE FP instructions.
  if (((dwFeature & SSE_FEATURE) != 0) && this->Features.HasSSE) bHasFeature = true;

  // Check for SSE FP instructions.
  if (((dwFeature & SSE_FP_FEATURE) != 0) && this->Features.HasSSEFP) bHasFeature = true;

  // Check for SSE MMX instructions.
  if (((dwFeature & SSE_MMX_FEATURE) != 0) && this->Features.ExtendedFeatures.HasSSEMMX) bHasFeature = true;

  // Check for SSE2 instructions.
  if (((dwFeature & SSE2_FEATURE) != 0) && this->Features.HasSSE2) bHasFeature = true;

  // Check for 3DNow! instructions.
  if (((dwFeature & AMD_3DNOW_FEATURE) != 0) && this->Features.ExtendedFeatures.Has3DNow) bHasFeature = true;

  // Check for 3DNow+ instructions.
  if (((dwFeature & AMD_3DNOW_PLUS_FEATURE) != 0) && this->Features.ExtendedFeatures.Has3DNowPlus) bHasFeature = true;

  // Check for IA64 instructions.
  if (((dwFeature & IA64_FEATURE) != 0) && this->Features.HasIA64) bHasFeature = true;

  // Check for MP capable.
  if (((dwFeature & MP_CAPABLE) != 0) && this->Features.ExtendedFeatures.SupportsMP) bHasFeature = true;

  // Check for a serial number for the processor.
  if (((dwFeature & SERIALNUMBER_FEATURE) != 0) && this->Features.HasSerial) bHasFeature = true;

  // Check for a local APIC in the processor.
  if (((dwFeature & APIC_FEATURE) != 0) && this->Features.HasAPIC) bHasFeature = true;

  // Check for CMOV instructions.
  if (((dwFeature & CMOV_FEATURE) != 0) && this->Features.HasCMOV) bHasFeature = true;

  // Check for MTRR instructions.
  if (((dwFeature & MTRR_FEATURE) != 0) && this->Features.HasMTRR) bHasFeature = true;

  // Check for L1 cache size.
  if (((dwFeature & L1CACHE_FEATURE) != 0) && (this->Features.L1CacheSize != -1)) bHasFeature = true;

  // Check for L2 cache size.
  if (((dwFeature & L2CACHE_FEATURE) != 0) && (this->Features.L2CacheSize != -1)) bHasFeature = true;

  // Check for L3 cache size.
  if (((dwFeature & L3CACHE_FEATURE) != 0) && (this->Features.L3CacheSize != -1)) bHasFeature = true;

  // Check for ACPI capability.
  if (((dwFeature & ACPI_FEATURE) != 0) && this->Features.HasACPI) bHasFeature = true;

  // Check for thermal monitor support.
  if (((dwFeature & THERMALMONITOR_FEATURE) != 0) && this->Features.HasThermal) bHasFeature = true;

  // Check for temperature sensing diode support.
  if (((dwFeature & TEMPSENSEDIODE_FEATURE) != 0) && this->Features.ExtendedFeatures.PowerManagement.HasTempSenseDiode) bHasFeature = true;

  // Check for frequency ID support.
  if (((dwFeature & FREQUENCYID_FEATURE) != 0) && this->Features.ExtendedFeatures.PowerManagement.HasFrequencyID) bHasFeature = true;

  // Check for voltage ID support.
  if (((dwFeature & VOLTAGEID_FREQUENCY) != 0) && this->Features.ExtendedFeatures.PowerManagement.HasVoltageID) bHasFeature = true;

  return bHasFeature;
}

void SystemInformationImplementation::Delay(unsigned int uiMS)
{
#ifdef WIN32
  LARGE_INTEGER Frequency, StartCounter, EndCounter;
  __int64 x;

  // Get the frequency of the high performance counter.
  if (!QueryPerformanceFrequency (&Frequency)) return;
  x = Frequency.QuadPart / 1000 * uiMS;

  // Get the starting position of the counter.
  QueryPerformanceCounter (&StartCounter);

  do {
    // Get the ending position of the counter.  
    QueryPerformanceCounter (&EndCounter);
    } while (EndCounter.QuadPart - StartCounter.QuadPart < x);
#endif
  (void)uiMS;
}

bool SystemInformationImplementation::DoesCPUSupportCPUID()
{
#if USE_ASM_INSTRUCTIONS
  // Use SEH to determine CPUID presence
    __try {
        _asm {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>> 
            mov eax, 0
      CPUID_INSTRUCTION

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
        }
    }
  __except(1) 
    {
    // Stop the class from trying to use CPUID again!
    return false;
    }

  // The cpuid instruction succeeded.
  return true;
#else
  // Assume no cpuid instruction.
  return false;
#endif
}

bool SystemInformationImplementation::RetrieveCPUFeatures()
{
#if USE_ASM_INSTRUCTIONS
  int localCPUFeatures = 0;
  int localCPUAdvanced = 0;


  // Use assembly to detect CPUID information...
  __try {
    _asm {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>> 
      ; eax = 1 --> eax: CPU ID - bits 31..16 - unused, bits 15..12 - type, bits 11..8 - family, bits 7..4 - model, bits 3..0 - mask revision
      ;        ebx: 31..24 - default APIC ID, 23..16 - logical processsor ID, 15..8 - CFLUSH chunk size , 7..0 - brand ID
      ;        edx: CPU feature flags
      mov eax,1
      CPUID_INSTRUCTION
      mov localCPUFeatures, edx
      mov localCPUAdvanced, ebx

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
    }
  }
  __except(1) 
    {
    return false;
    }

  // Retrieve the features of CPU present.
  this->Features.HasFPU =    ((localCPUFeatures & 0x00000001) != 0);    // FPU Present --> Bit 0
  this->Features.HasTSC =    ((localCPUFeatures & 0x00000010) != 0);    // TSC Present --> Bit 4
  this->Features.HasAPIC =    ((localCPUFeatures & 0x00000200) != 0);    // APIC Present --> Bit 9
  this->Features.HasMTRR =    ((localCPUFeatures & 0x00001000) != 0);    // MTRR Present --> Bit 12
  this->Features.HasCMOV =    ((localCPUFeatures & 0x00008000) != 0);    // CMOV Present --> Bit 15
  this->Features.HasSerial =  ((localCPUFeatures & 0x00040000) != 0);    // Serial Present --> Bit 18
  this->Features.HasACPI =    ((localCPUFeatures & 0x00400000) != 0);    // ACPI Capable --> Bit 22
  this->Features.HasMMX =    ((localCPUFeatures & 0x00800000) != 0);    // MMX Present --> Bit 23
  this->Features.HasSSE =    ((localCPUFeatures & 0x02000000) != 0);    // SSE Present --> Bit 25
  this->Features.HasSSE2 =    ((localCPUFeatures & 0x04000000) != 0);    // SSE2 Present --> Bit 26
  this->Features.HasThermal =  ((localCPUFeatures & 0x20000000) != 0);    // Thermal Monitor Present --> Bit 29
  this->Features.HasIA64 =    ((localCPUFeatures & 0x40000000) != 0);    // IA64 Present --> Bit 30

  // Retrieve extended SSE capabilities if SSE is available.
  if (this->Features.HasSSE) {
    
    // Attempt to __try some SSE FP instructions.
    __try 
      {
      // Perform: orps xmm0, xmm0
      _asm 
        {
        _emit 0x0f
        _emit 0x56
        _emit 0xc0  
        }

      // SSE FP capable processor.
      this->Features.HasSSEFP = true;
      }   
    __except(1) 
      {
      // bad instruction - processor or OS cannot handle SSE FP.
      this->Features.HasSSEFP = false;
      }
    } 
  else 
    {
    // Set the advanced SSE capabilities to not available.
    this->Features.HasSSEFP = false;
    }

  // Retrieve Intel specific extended features.
  if (this->ChipManufacturer == Intel) 
    {
    this->Features.ExtendedFeatures.SupportsHyperthreading =  ((localCPUFeatures &  0x10000000) != 0);  // Intel specific: Hyperthreading --> Bit 28
    this->Features.ExtendedFeatures.LogicalProcessorsPerPhysical = (this->Features.ExtendedFeatures.SupportsHyperthreading) ? ((localCPUAdvanced & 0x00FF0000) >> 16) : 1;
    
    if ((this->Features.ExtendedFeatures.SupportsHyperthreading) && (this->Features.HasAPIC))
      {
      // Retrieve APIC information if there is one present.
      this->Features.ExtendedFeatures.APIC_ID = ((localCPUAdvanced & 0xFF000000) >> 24);
      }
    }
#endif
  return true;
}


/** Find the manufacturer given the vendor id */
void SystemInformationImplementation::FindManufacturer()
{
  if (strcmp (this->ChipID.Vendor, "GenuineIntel") == 0)    this->ChipManufacturer = Intel;        // Intel Corp.
  else if (strcmp (this->ChipID.Vendor, "UMC UMC UMC ") == 0)  this->ChipManufacturer = UMC;          // United Microelectronics Corp.
  else if (strcmp (this->ChipID.Vendor, "AuthenticAMD") == 0)  this->ChipManufacturer = AMD;          // Advanced Micro Devices
  else if (strcmp (this->ChipID.Vendor, "AMD ISBETTER") == 0)  this->ChipManufacturer = AMD;          // Advanced Micro Devices (1994)
  else if (strcmp (this->ChipID.Vendor, "CyrixInstead") == 0)  this->ChipManufacturer = Cyrix;        // Cyrix Corp., VIA Inc.
  else if (strcmp (this->ChipID.Vendor, "NexGenDriven") == 0)  this->ChipManufacturer = NexGen;        // NexGen Inc. (now AMD)
  else if (strcmp (this->ChipID.Vendor, "CentaurHauls") == 0)  this->ChipManufacturer = IDT;          // IDT/Centaur (now VIA)
  else if (strcmp (this->ChipID.Vendor, "RiseRiseRise") == 0)  this->ChipManufacturer = Rise;        // Rise
  else if (strcmp (this->ChipID.Vendor, "GenuineTMx86") == 0)  this->ChipManufacturer = Transmeta;      // Transmeta
  else if (strcmp (this->ChipID.Vendor, "TransmetaCPU") == 0)  this->ChipManufacturer = Transmeta;      // Transmeta
  else if (strcmp (this->ChipID.Vendor, "Geode By NSC") == 0)  this->ChipManufacturer = NSC;          // National Semiconductor
  else if (strcmp (this->ChipID.Vendor, "Sun") == 0)  this->ChipManufacturer = Sun;          // Sun Microelectronics
  else                          this->ChipManufacturer = UnknownManufacturer;  // Unknown manufacturer
}

/** */
bool SystemInformationImplementation::RetrieveCPUIdentity()
{
#if USE_ASM_INSTRUCTIONS
  int localCPUVendor[3];
  int localCPUSignature;

  // Use assembly to detect CPUID information...
  __try 
    {
    _asm 
      {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>>
      ; eax = 0 --> eax: maximum value of CPUID instruction.
      ;        ebx: part 1 of 3; CPU signature.
      ;        edx: part 2 of 3; CPU signature.
      ;        ecx: part 3 of 3; CPU signature.
      mov eax, 0
      CPUID_INSTRUCTION
      mov localCPUVendor[0 * TYPE int], ebx
      mov localCPUVendor[1 * TYPE int], edx
      mov localCPUVendor[2 * TYPE int], ecx

      ; <<CPUID>> 
      ; eax = 1 --> eax: CPU ID - bits 31..16 - unused, bits 15..12 - type, bits 11..8 - family, bits 7..4 - model, bits 3..0 - mask revision
      ;        ebx: 31..24 - default APIC ID, 23..16 - logical processsor ID, 15..8 - CFLUSH chunk size , 7..0 - brand ID
      ;        edx: CPU feature flags
      mov eax,1
      CPUID_INSTRUCTION
      mov localCPUSignature, eax

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
    }
  }
  __except(1) 
    {
    return false;
    }

  // Process the returned information.
  memcpy (this->ChipID.Vendor, &(localCPUVendor[0]), sizeof (int));
  memcpy (&(this->ChipID.Vendor[4]), &(localCPUVendor[1]), sizeof (int));
  memcpy (&(this->ChipID.Vendor[8]), &(localCPUVendor[2]), sizeof (int));
  this->ChipID.Vendor[12] = '\0';

  this->FindManufacturer();

  // Retrieve the family of CPU present.
  this->ChipID.ExtendedFamily =    ((localCPUSignature & 0x0FF00000) >> 20);  // Bits 27..20 Used
  this->ChipID.ExtendedModel =    ((localCPUSignature & 0x000F0000) >> 16);  // Bits 19..16 Used
  this->ChipID.Type =        ((localCPUSignature & 0x0000F000) >> 12);  // Bits 15..12 Used
  this->ChipID.Family =        ((localCPUSignature & 0x00000F00) >> 8);    // Bits 11..8 Used
  this->ChipID.Model =        ((localCPUSignature & 0x000000F0) >> 4);    // Bits 7..4 Used
  this->ChipID.Revision =      ((localCPUSignature & 0x0000000F) >> 0);    // Bits 3..0 Used
#endif

  return true;
}

/** */
bool SystemInformationImplementation::RetrieveCPUCacheDetails()
{
#if USE_ASM_INSTRUCTIONS
  int L1Cache[4] = { 0, 0, 0, 0 };
  int L2Cache[4] = { 0, 0, 0, 0 };

  // Check to see if what we are about to do is supported...
  if (RetrieveCPUExtendedLevelSupport (0x80000005)) 
    {
    // Use assembly to retrieve the L1 cache information ...
    __try 
      {
      _asm 
        {
#ifdef CPUID_AWARE_COMPILER
         ; we must push/pop the registers <<CPUID>> writes to, as the
        ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
        ; these registers to change.
        push eax
        push ebx
        push ecx
        push edx
#endif
        ; <<CPUID>>
        ; eax = 0x80000005 --> eax: L1 cache information - Part 1 of 4.
        ;             ebx: L1 cache information - Part 2 of 4.
        ;             edx: L1 cache information - Part 3 of 4.
        ;              ecx: L1 cache information - Part 4 of 4.
        mov eax, 0x80000005
        CPUID_INSTRUCTION
        mov L1Cache[0 * TYPE int], eax
        mov L1Cache[1 * TYPE int], ebx
        mov L1Cache[2 * TYPE int], ecx
        mov L1Cache[3 * TYPE int], edx

#ifdef CPUID_AWARE_COMPILER
        pop edx
        pop ecx
        pop ebx
        pop eax
#endif
        }
      }
    __except(1) 
      {
      return false;
      }
    // Save the L1 data cache size (in KB) from ecx: bits 31..24 as well as data cache size from edx: bits 31..24.
    this->Features.L1CacheSize = ((L1Cache[2] & 0xFF000000) >> 24);
    this->Features.L1CacheSize += ((L1Cache[3] & 0xFF000000) >> 24);
    } 
  else 
    {
    // Store -1 to indicate the cache could not be queried.
    this->Features.L1CacheSize = -1;
    }

  // Check to see if what we are about to do is supported...
  if (RetrieveCPUExtendedLevelSupport (0x80000006)) 
    {
    // Use assembly to retrieve the L2 cache information ...
    __try 
      {
      _asm 
        {
#ifdef CPUID_AWARE_COMPILER
         ; we must push/pop the registers <<CPUID>> writes to, as the
        ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
        ; these registers to change.
        push eax
        push ebx
        push ecx
        push edx
#endif
        ; <<CPUID>>
        ; eax = 0x80000006 --> eax: L2 cache information - Part 1 of 4.
        ;             ebx: L2 cache information - Part 2 of 4.
        ;             edx: L2 cache information - Part 3 of 4.
        ;              ecx: L2 cache information - Part 4 of 4.
        mov eax, 0x80000006
        CPUID_INSTRUCTION
        mov L2Cache[0 * TYPE int], eax
        mov L2Cache[1 * TYPE int], ebx
        mov L2Cache[2 * TYPE int], ecx
        mov L2Cache[3 * TYPE int], edx

#ifdef CPUID_AWARE_COMPILER
        pop edx
        pop ecx
        pop ebx
        pop eax
#endif
        }
      }
    __except(1) 
      {
      return false;
      }
    // Save the L2 unified cache size (in KB) from ecx: bits 31..16.
    this->Features.L2CacheSize = ((L2Cache[2] & 0xFFFF0000) >> 16);
    } 
  else
    {
    // Store -1 to indicate the cache could not be queried.
    this->Features.L2CacheSize = -1;
    }
  
  // Define L3 as being not present as we cannot test for it.
  this->Features.L3CacheSize = -1;

#endif

  // Return failure if we cannot detect either cache with this method.
  return ((this->Features.L1CacheSize == -1) && (this->Features.L2CacheSize == -1)) ? false : true;
}

/** */
bool SystemInformationImplementation::RetrieveClassicalCPUCacheDetails()
{
#if USE_ASM_INSTRUCTIONS
  int TLBCode = -1, TLBData = -1, L1Code = -1, L1Data = -1, L1Trace = -1, L2Unified = -1, L3Unified = -1;
  int TLBCacheData[4] = { 0, 0, 0, 0 };
  int TLBPassCounter = 0;
  int TLBCacheUnit = 0;


  do {
    // Use assembly to retrieve the L2 cache information ...
    __try {
      _asm {
#ifdef CPUID_AWARE_COMPILER
         ; we must push/pop the registers <<CPUID>> writes to, as the
        ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
        ; these registers to change.
        push eax
        push ebx
        push ecx
        push edx
#endif
        ; <<CPUID>>
        ; eax = 2 --> eax: TLB and cache information - Part 1 of 4.
        ;        ebx: TLB and cache information - Part 2 of 4.
        ;        ecx: TLB and cache information - Part 3 of 4.
        ;        edx: TLB and cache information - Part 4 of 4.
        mov eax, 2
        CPUID_INSTRUCTION
        mov TLBCacheData[0 * TYPE int], eax
        mov TLBCacheData[1 * TYPE int], ebx
        mov TLBCacheData[2 * TYPE int], ecx
        mov TLBCacheData[3 * TYPE int], edx

#ifdef CPUID_AWARE_COMPILER
        pop edx
        pop ecx
        pop ebx
        pop eax
#endif
        }
      }
    __except(1)
      {
      return false;
      }

    int bob = ((TLBCacheData[0] & 0x00FF0000) >> 16);
    (void)bob;
    // Process the returned TLB and cache information.
    for (int nCounter = 0; nCounter < TLBCACHE_INFO_UNITS; nCounter ++) 
      {
      // First of all - decide which unit we are dealing with.
      switch (nCounter) 
        {
        // eax: bits 8..15 : bits 16..23 : bits 24..31
        case 0: TLBCacheUnit = ((TLBCacheData[0] & 0x0000FF00) >> 8); break;
        case 1: TLBCacheUnit = ((TLBCacheData[0] & 0x00FF0000) >> 16); break;
        case 2: TLBCacheUnit = ((TLBCacheData[0] & 0xFF000000) >> 24); break;

        // ebx: bits 0..7 : bits 8..15 : bits 16..23 : bits 24..31
        case 3: TLBCacheUnit = ((TLBCacheData[1] & 0x000000FF) >> 0); break;
        case 4: TLBCacheUnit = ((TLBCacheData[1] & 0x0000FF00) >> 8); break;
        case 5: TLBCacheUnit = ((TLBCacheData[1] & 0x00FF0000) >> 16); break;
        case 6: TLBCacheUnit = ((TLBCacheData[1] & 0xFF000000) >> 24); break;

        // ecx: bits 0..7 : bits 8..15 : bits 16..23 : bits 24..31
        case 7: TLBCacheUnit = ((TLBCacheData[2] & 0x000000FF) >> 0); break;
        case 8: TLBCacheUnit = ((TLBCacheData[2] & 0x0000FF00) >> 8); break;
        case 9: TLBCacheUnit = ((TLBCacheData[2] & 0x00FF0000) >> 16); break;
        case 10: TLBCacheUnit = ((TLBCacheData[2] & 0xFF000000) >> 24); break;

        // edx: bits 0..7 : bits 8..15 : bits 16..23 : bits 24..31
        case 11: TLBCacheUnit = ((TLBCacheData[3] & 0x000000FF) >> 0); break;
        case 12: TLBCacheUnit = ((TLBCacheData[3] & 0x0000FF00) >> 8); break;
        case 13: TLBCacheUnit = ((TLBCacheData[3] & 0x00FF0000) >> 16); break;
        case 14: TLBCacheUnit = ((TLBCacheData[3] & 0xFF000000) >> 24); break;

        // Default case - an error has occured.
        default: return false;
        }

      // Now process the resulting unit to see what it means....
      switch (TLBCacheUnit) 
        {
        case 0x00: break;
        case 0x01: STORE_TLBCACHE_INFO (TLBCode, 4); break;
        case 0x02: STORE_TLBCACHE_INFO (TLBCode, 4096); break;
        case 0x03: STORE_TLBCACHE_INFO (TLBData, 4); break;
        case 0x04: STORE_TLBCACHE_INFO (TLBData, 4096); break;
        case 0x06: STORE_TLBCACHE_INFO (L1Code, 8); break;
        case 0x08: STORE_TLBCACHE_INFO (L1Code, 16); break;
        case 0x0a: STORE_TLBCACHE_INFO (L1Data, 8); break;
        case 0x0c: STORE_TLBCACHE_INFO (L1Data, 16); break;
        case 0x10: STORE_TLBCACHE_INFO (L1Data, 16); break;      // <-- FIXME: IA-64 Only
        case 0x15: STORE_TLBCACHE_INFO (L1Code, 16); break;      // <-- FIXME: IA-64 Only
        case 0x1a: STORE_TLBCACHE_INFO (L2Unified, 96); break;    // <-- FIXME: IA-64 Only
        case 0x22: STORE_TLBCACHE_INFO (L3Unified, 512); break;
        case 0x23: STORE_TLBCACHE_INFO (L3Unified, 1024); break;
        case 0x25: STORE_TLBCACHE_INFO (L3Unified, 2048); break;
        case 0x29: STORE_TLBCACHE_INFO (L3Unified, 4096); break;
        case 0x39: STORE_TLBCACHE_INFO (L2Unified, 128); break;
        case 0x3c: STORE_TLBCACHE_INFO (L2Unified, 256); break;
        case 0x40: STORE_TLBCACHE_INFO (L2Unified, 0); break;    // <-- FIXME: No integrated L2 cache (P6 core) or L3 cache (P4 core).
        case 0x41: STORE_TLBCACHE_INFO (L2Unified, 128); break;
        case 0x42: STORE_TLBCACHE_INFO (L2Unified, 256); break;
        case 0x43: STORE_TLBCACHE_INFO (L2Unified, 512); break;
        case 0x44: STORE_TLBCACHE_INFO (L2Unified, 1024); break;
        case 0x45: STORE_TLBCACHE_INFO (L2Unified, 2048); break;
        case 0x50: STORE_TLBCACHE_INFO (TLBCode, 4096); break;
        case 0x51: STORE_TLBCACHE_INFO (TLBCode, 4096); break;
        case 0x52: STORE_TLBCACHE_INFO (TLBCode, 4096); break;
        case 0x5b: STORE_TLBCACHE_INFO (TLBData, 4096); break;
        case 0x5c: STORE_TLBCACHE_INFO (TLBData, 4096); break;
        case 0x5d: STORE_TLBCACHE_INFO (TLBData, 4096); break;
        case 0x66: STORE_TLBCACHE_INFO (L1Data, 8); break;
        case 0x67: STORE_TLBCACHE_INFO (L1Data, 16); break;
        case 0x68: STORE_TLBCACHE_INFO (L1Data, 32); break;
        case 0x70: STORE_TLBCACHE_INFO (L1Trace, 12); break;
        case 0x71: STORE_TLBCACHE_INFO (L1Trace, 16); break;
        case 0x72: STORE_TLBCACHE_INFO (L1Trace, 32); break;
        case 0x77: STORE_TLBCACHE_INFO (L1Code, 16); break;      // <-- FIXME: IA-64 Only
        case 0x79: STORE_TLBCACHE_INFO (L2Unified, 128); break;
        case 0x7a: STORE_TLBCACHE_INFO (L2Unified, 256); break;
        case 0x7b: STORE_TLBCACHE_INFO (L2Unified, 512); break;
        case 0x7c: STORE_TLBCACHE_INFO (L2Unified, 1024); break;
        case 0x7e: STORE_TLBCACHE_INFO (L2Unified, 256); break;
        case 0x81: STORE_TLBCACHE_INFO (L2Unified, 128); break;
        case 0x82: STORE_TLBCACHE_INFO (L2Unified, 256); break;
        case 0x83: STORE_TLBCACHE_INFO (L2Unified, 512); break;
        case 0x84: STORE_TLBCACHE_INFO (L2Unified, 1024); break;
        case 0x85: STORE_TLBCACHE_INFO (L2Unified, 2048); break;
        case 0x88: STORE_TLBCACHE_INFO (L3Unified, 2048); break;  // <-- FIXME: IA-64 Only
        case 0x89: STORE_TLBCACHE_INFO (L3Unified, 4096); break;  // <-- FIXME: IA-64 Only
        case 0x8a: STORE_TLBCACHE_INFO (L3Unified, 8192); break;  // <-- FIXME: IA-64 Only
        case 0x8d: STORE_TLBCACHE_INFO (L3Unified, 3096); break;  // <-- FIXME: IA-64 Only
        case 0x90: STORE_TLBCACHE_INFO (TLBCode, 262144); break;  // <-- FIXME: IA-64 Only
        case 0x96: STORE_TLBCACHE_INFO (TLBCode, 262144); break;  // <-- FIXME: IA-64 Only
        case 0x9b: STORE_TLBCACHE_INFO (TLBCode, 262144); break;  // <-- FIXME: IA-64 Only
        
        // Default case - an error has occured.
        default: return false;
        }
      }

    // Increment the TLB pass counter.
    TLBPassCounter ++;
    } while ((TLBCacheData[0] & 0x000000FF) > TLBPassCounter);

  // Ok - we now have the maximum TLB, L1, L2, and L3 sizes...
  if ((L1Code == -1) && (L1Data == -1) && (L1Trace == -1)) 
    {
    this->Features.L1CacheSize = -1;
    }
  else if ((L1Code == -1) && (L1Data == -1) && (L1Trace != -1)) 
    {
    this->Features.L1CacheSize = L1Trace;
    }
  else if ((L1Code != -1) && (L1Data == -1)) 
    {
    this->Features.L1CacheSize = L1Code;
    }
  else if ((L1Code == -1) && (L1Data != -1)) 
    {
    this->Features.L1CacheSize = L1Data;
    }
  else if ((L1Code != -1) && (L1Data != -1)) 
    {
    this->Features.L1CacheSize = L1Code + L1Data;
    }
  else 
    {
    this->Features.L1CacheSize = -1;
    }

  // Ok - we now have the maximum TLB, L1, L2, and L3 sizes...
  if (L2Unified == -1) 
    {
    this->Features.L2CacheSize = -1;
    }
  else 
    {
    this->Features.L2CacheSize = L2Unified;
    }

  // Ok - we now have the maximum TLB, L1, L2, and L3 sizes...
  if (L3Unified == -1) 
    {
    this->Features.L3CacheSize = -1;
    }
  else 
    {
    this->Features.L3CacheSize = L3Unified;
    }

#endif
  return true;
}

/** */
bool SystemInformationImplementation::RetrieveCPUClockSpeed()
{
#if _WIN32
  // First of all we check to see if the RDTSC (0x0F, 0x31) instruction is supported.
  if (!this->Features.HasTSC) 
    {
    return false;
    }

  unsigned int uiRepetitions = 1;
  unsigned int uiMSecPerRepetition = 50;
  __int64  i64Total = 0;
  __int64 i64Overhead = 0;

  for (unsigned int nCounter = 0; nCounter < uiRepetitions; nCounter ++) 
    {
    i64Total += GetCyclesDifference (SystemInformationImplementation::Delay,
                                     uiMSecPerRepetition);
    i64Overhead +=
      GetCyclesDifference (SystemInformationImplementation::DelayOverhead,
                           uiMSecPerRepetition);
    }

  // Calculate the MHz speed.
  i64Total -= i64Overhead;
  i64Total /= uiRepetitions;
  i64Total /= uiMSecPerRepetition;
  i64Total /= 1000;

  // Save the CPU speed.
  this->CPUSpeedInMHz = (float) i64Total;

  return true;
#else
  return false;
#endif
}

/** */
bool SystemInformationImplementation::RetrieveClassicalCPUClockSpeed()
{
#if USE_ASM_INSTRUCTIONS
  LARGE_INTEGER liStart, liEnd, liCountsPerSecond;
  double dFrequency, dDifference;

  // Attempt to get a starting tick count.
  QueryPerformanceCounter (&liStart);

  __try 
    {
    _asm 
      {
      mov eax, 0x80000000
      mov ebx, CLASSICAL_CPU_FREQ_LOOP
      Timer_Loop: 
      bsf ecx,eax
      dec ebx
      jnz Timer_Loop
      }  
    }
  __except(1) 
    {
    return false;
    }

  // Attempt to get a starting tick count.
  QueryPerformanceCounter (&liEnd);

  // Get the difference...  NB: This is in seconds....
  QueryPerformanceFrequency (&liCountsPerSecond);
  dDifference = (((double) liEnd.QuadPart - (double) liStart.QuadPart) / (double) liCountsPerSecond.QuadPart);

  // Calculate the clock speed.
  if (this->ChipID.Family == 3) 
    {
    // 80386 processors....  Loop time is 115 cycles!
    dFrequency = (((CLASSICAL_CPU_FREQ_LOOP * 115) / dDifference) / 1048576);
    } 
  else if (this->ChipID.Family == 4) 
    {
    // 80486 processors....  Loop time is 47 cycles!
    dFrequency = (((CLASSICAL_CPU_FREQ_LOOP * 47) / dDifference) / 1048576);
    } 
  else if (this->ChipID.Family == 5) 
    {
    // Pentium processors....  Loop time is 43 cycles!
    dFrequency = (((CLASSICAL_CPU_FREQ_LOOP * 43) / dDifference) / 1048576);
    }
  
  // Save the clock speed.
  this->Features.CPUSpeed = (int) dFrequency;
#else
  return true;
#endif
}

/** */
bool SystemInformationImplementation::RetrieveCPUExtendedLevelSupport(int CPULevelToCheck)
{
  int MaxCPUExtendedLevel = 0;

  // The extended CPUID is supported by various vendors starting with the following CPU models: 
  //
  //    Manufacturer & Chip Name      |    Family     Model    Revision
  //
  //    AMD K6, K6-2                  |       5       6      x
  //    Cyrix GXm, Cyrix III "Joshua" |       5       4      x
  //    IDT C6-2                      |       5       8      x
  //    VIA Cyrix III                 |       6       5      x
  //    Transmeta Crusoe              |       5       x      x
  //    Intel Pentium 4               |       f       x      x
  //

  // We check to see if a supported processor is present...
  if (this->ChipManufacturer == AMD) 
    {
    if (this->ChipID.Family < 5) return false;
    if ((this->ChipID.Family == 5) && (this->ChipID.Model < 6)) return false;
    } 
  else if (this->ChipManufacturer == Cyrix) 
    {
    if (this->ChipID.Family < 5) return false;
    if ((this->ChipID.Family == 5) && (this->ChipID.Model < 4)) return false;
    if ((this->ChipID.Family == 6) && (this->ChipID.Model < 5)) return false;
    } 
  else if (this->ChipManufacturer == IDT) 
    {
    if (this->ChipID.Family < 5) return false;
    if ((this->ChipID.Family == 5) && (this->ChipID.Model < 8)) return false;
    } 
  else if (this->ChipManufacturer == Transmeta) 
    {
    if (this->ChipID.Family < 5) return false;
    } 
  else if (this->ChipManufacturer == Intel) 
    {
    if (this->ChipID.Family < 0xf)
      {
      return false;
      }
    }
    
#if USE_ASM_INSTRUCTIONS

  // Use assembly to detect CPUID information...
  __try {
    _asm {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>> 
      ; eax = 0x80000000 --> eax: maximum supported extended level
      mov eax,0x80000000
      CPUID_INSTRUCTION
      mov MaxCPUExtendedLevel, eax

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
    }
  }
  __except(1) 
    {
    return false;
    }
#endif

  // Now we have to check the level wanted vs level returned...
  int nLevelWanted = (CPULevelToCheck & 0x7FFFFFFF);
  int nLevelReturn = (MaxCPUExtendedLevel & 0x7FFFFFFF);

  // Check to see if the level provided is supported...
  if (nLevelWanted > nLevelReturn)
    {
    return false;
    }

  return true;
}

/** */
bool SystemInformationImplementation::RetrieveExtendedCPUFeatures()
{

  // Check that we are not using an Intel processor as it does not support this.
  if (this->ChipManufacturer == Intel) 
    {
    return false;
    }

  // Check to see if what we are about to do is supported...
  if (!RetrieveCPUExtendedLevelSupport (0x80000001)) 
    {
    return false;
    }
#if USE_ASM_INSTRUCTIONS
  int localCPUExtendedFeatures = 0;

  // Use assembly to detect CPUID information...
  __try 
    {
    _asm 
      {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>> 
      ; eax = 0x80000001 --> eax: CPU ID - bits 31..16 - unused, bits 15..12 - type, bits 11..8 - family, bits 7..4 - model, bits 3..0 - mask revision
      ;             ebx: 31..24 - default APIC ID, 23..16 - logical processsor ID, 15..8 - CFLUSH chunk size , 7..0 - brand ID
      ;             edx: CPU feature flags
      mov eax,0x80000001
      CPUID_INSTRUCTION
      mov localCPUExtendedFeatures, edx

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
      }
    }
  __except(1) 
    {
    return false;
    }

  // Retrieve the extended features of CPU present.
  this->Features.ExtendedFeatures.Has3DNow = ((localCPUExtendedFeatures & 0x80000000) != 0);  // 3DNow Present --> Bit 31.
  this->Features.ExtendedFeatures.Has3DNowPlus = ((localCPUExtendedFeatures & 0x40000000) != 0);  // 3DNow+ Present -- > Bit 30.
  this->Features.ExtendedFeatures.HasSSEMMX = ((localCPUExtendedFeatures & 0x00400000) != 0);  // SSE MMX Present --> Bit 22.
  this->Features.ExtendedFeatures.SupportsMP = ((localCPUExtendedFeatures & 0x00080000) != 0);  // MP Capable -- > Bit 19.
  
  // Retrieve AMD specific extended features.
  if (this->ChipManufacturer == AMD) 
    {
    this->Features.ExtendedFeatures.HasMMXPlus = ((localCPUExtendedFeatures &  0x00400000) != 0);  // AMD specific: MMX-SSE --> Bit 22
    }

  // Retrieve Cyrix specific extended features.
  if (this->ChipManufacturer == Cyrix) 
    {
    this->Features.ExtendedFeatures.HasMMXPlus = ((localCPUExtendedFeatures &  0x01000000) != 0);  // Cyrix specific: Extended MMX --> Bit 24
    }
#endif

  return true;
}

/** */
bool SystemInformationImplementation::RetrieveProcessorSerialNumber()
{
  // Check to see if the processor supports the processor serial number.
  if (!this->Features.HasSerial)
    {
    return false;
    }

#if USE_ASM_INSTRUCTIONS
  int SerialNumber[3];


    // Use assembly to detect CPUID information...
  __try {
    _asm {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>>
      ; eax = 3 --> ebx: top 32 bits are the processor signature bits --> NB: Transmeta only ?!?
      ;        ecx: middle 32 bits are the processor signature bits
      ;        edx: bottom 32 bits are the processor signature bits
      mov eax, 3
      CPUID_INSTRUCTION
      mov SerialNumber[0 * TYPE int], ebx
      mov SerialNumber[1 * TYPE int], ecx
      mov SerialNumber[2 * TYPE int], edx

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
    }
  }
  __except(1) 
    {
    return false;
    }

  // Process the returned information.
  sprintf (this->ChipID.SerialNumber, "%.2x%.2x-%.2x%.2x-%.2x%.2x-%.2x%.2x-%.2x%.2x-%.2x%.2x",
       ((SerialNumber[0] & 0xff000000) >> 24),
       ((SerialNumber[0] & 0x00ff0000) >> 16),
       ((SerialNumber[0] & 0x0000ff00) >> 8),
       ((SerialNumber[0] & 0x000000ff) >> 0),
       ((SerialNumber[1] & 0xff000000) >> 24),
       ((SerialNumber[1] & 0x00ff0000) >> 16),
       ((SerialNumber[1] & 0x0000ff00) >> 8),
       ((SerialNumber[1] & 0x000000ff) >> 0),
       ((SerialNumber[2] & 0xff000000) >> 24),
       ((SerialNumber[2] & 0x00ff0000) >> 16),
       ((SerialNumber[2] & 0x0000ff00) >> 8),
       ((SerialNumber[2] & 0x000000ff) >> 0));
#endif

  return true;
}

/** */
bool SystemInformationImplementation::RetrieveCPUPowerManagement()
{  
  // Check to see if what we are about to do is supported...
  if (!RetrieveCPUExtendedLevelSupport (0x80000007)) 
    {
    this->Features.ExtendedFeatures.PowerManagement.HasFrequencyID = false;
    this->Features.ExtendedFeatures.PowerManagement.HasVoltageID = false;
    this->Features.ExtendedFeatures.PowerManagement.HasTempSenseDiode = false;
    return false;
    }

#if USE_ASM_INSTRUCTIONS
  int localCPUPowerManagement = 0;


  // Use assembly to detect CPUID information...
  __try {
    _asm {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>> 
      ; eax = 0x80000007 --> edx: get processor power management
      mov eax,0x80000007
      CPUID_INSTRUCTION
      mov localCPUPowerManagement, edx
      
#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
    }
  }
  __except(1) 
    {
    return false;
    }

  // Check for the power management capabilities of the CPU.
  this->Features.ExtendedFeatures.PowerManagement.HasTempSenseDiode =  ((localCPUPowerManagement & 0x00000001) != 0);
  this->Features.ExtendedFeatures.PowerManagement.HasFrequencyID =    ((localCPUPowerManagement & 0x00000002) != 0);
  this->Features.ExtendedFeatures.PowerManagement.HasVoltageID =    ((localCPUPowerManagement & 0x00000004) != 0);

#endif

  return true;
}

/** */
bool SystemInformationImplementation::RetrieveExtendedCPUIdentity()
{
  // Check to see if what we are about to do is supported...
  if (!RetrieveCPUExtendedLevelSupport(0x80000002)) return false;
  if (!RetrieveCPUExtendedLevelSupport(0x80000003)) return false;
  if (!RetrieveCPUExtendedLevelSupport(0x80000004)) return false;
   
#if USE_ASM_INSTRUCTIONS
  int ProcessorNameStartPos = 0;
  int CPUExtendedIdentity[12];

  // Use assembly to detect CPUID information...
  __try {
    _asm {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>> 
      ; eax = 0x80000002 --> eax, ebx, ecx, edx: get processor name string (part 1)
      mov eax,0x80000002
      CPUID_INSTRUCTION
      mov CPUExtendedIdentity[0 * TYPE int], eax
      mov CPUExtendedIdentity[1 * TYPE int], ebx
      mov CPUExtendedIdentity[2 * TYPE int], ecx
      mov CPUExtendedIdentity[3 * TYPE int], edx

      ; <<CPUID>> 
      ; eax = 0x80000003 --> eax, ebx, ecx, edx: get processor name string (part 2)
      mov eax,0x80000003
      CPUID_INSTRUCTION
      mov CPUExtendedIdentity[4 * TYPE int], eax
      mov CPUExtendedIdentity[5 * TYPE int], ebx
      mov CPUExtendedIdentity[6 * TYPE int], ecx
      mov CPUExtendedIdentity[7 * TYPE int], edx

      ; <<CPUID>> 
      ; eax = 0x80000004 --> eax, ebx, ecx, edx: get processor name string (part 3)
      mov eax,0x80000004
      CPUID_INSTRUCTION
      mov CPUExtendedIdentity[8 * TYPE int], eax
      mov CPUExtendedIdentity[9 * TYPE int], ebx
      mov CPUExtendedIdentity[10 * TYPE int], ecx
      mov CPUExtendedIdentity[11 * TYPE int], edx

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
    }
  }
  __except(1) 
    {
    return false;
    }

  // Process the returned information.
  memcpy (this->ChipID.ProcessorName, &(CPUExtendedIdentity[0]), sizeof (int));
  memcpy (&(this->ChipID.ProcessorName[4]), &(CPUExtendedIdentity[1]), sizeof (int));
  memcpy (&(this->ChipID.ProcessorName[8]), &(CPUExtendedIdentity[2]), sizeof (int));
  memcpy (&(this->ChipID.ProcessorName[12]), &(CPUExtendedIdentity[3]), sizeof (int));
  memcpy (&(this->ChipID.ProcessorName[16]), &(CPUExtendedIdentity[4]), sizeof (int));
  memcpy (&(this->ChipID.ProcessorName[20]), &(CPUExtendedIdentity[5]), sizeof (int));
  memcpy (&(this->ChipID.ProcessorName[24]), &(CPUExtendedIdentity[6]), sizeof (int));
  memcpy (&(this->ChipID.ProcessorName[28]), &(CPUExtendedIdentity[7]), sizeof (int));
  memcpy (&(this->ChipID.ProcessorName[32]), &(CPUExtendedIdentity[8]), sizeof (int));
  memcpy (&(this->ChipID.ProcessorName[36]), &(CPUExtendedIdentity[9]), sizeof (int));
  memcpy (&(this->ChipID.ProcessorName[40]), &(CPUExtendedIdentity[10]), sizeof (int));
  memcpy (&(this->ChipID.ProcessorName[44]), &(CPUExtendedIdentity[11]), sizeof (int));
  this->ChipID.ProcessorName[48] = '\0';

  // Because some manufacturers have leading white space - we have to post-process the name.
  if (this->ChipManufacturer == Intel) 
    {
    for (int nCounter = 0; nCounter < CHIPNAME_STRING_LENGTH; nCounter ++) 
      {
      // There will either be NULL (\0) or spaces ( ) as the leading characters.
      if ((this->ChipID.ProcessorName[nCounter] != '\0') && (this->ChipID.ProcessorName[nCounter] != ' ')) 
        {
        // We have found the starting position of the name.
        ProcessorNameStartPos = nCounter;
        // Terminate the loop.
        break;
        }
      }

    // Check to see if there is any white space at the start.
    if (ProcessorNameStartPos == 0) 
      {
      return true;
      }

    // Now move the name forward so that there is no white space.
    memmove(this->ChipID.ProcessorName, &(this->ChipID.ProcessorName[ProcessorNameStartPos]), (CHIPNAME_STRING_LENGTH - ProcessorNameStartPos));
   }
#endif

  return true;
}

/** */
bool SystemInformationImplementation::RetrieveClassicalCPUIdentity()
{
  // Start by decided which manufacturer we are using....
  switch (this->ChipManufacturer) 
    {
    case Intel:
      // Check the family / model / revision to determine the CPU ID.
      switch (this->ChipID.Family) {
        case 3:
          sprintf (this->ChipID.ProcessorName, "Newer i80386 family"); 
          break;
        case 4:
          switch (this->ChipID.Model) {
            case 0: sprintf (this->ChipID.ProcessorName,"i80486DX-25/33"); break;
            case 1: sprintf (this->ChipID.ProcessorName,"i80486DX-50"); break;
            case 2: sprintf (this->ChipID.ProcessorName,"i80486SX"); break;
            case 3: sprintf (this->ChipID.ProcessorName,"i80486DX2"); break;
            case 4: sprintf (this->ChipID.ProcessorName,"i80486SL"); break;
            case 5: sprintf (this->ChipID.ProcessorName,"i80486SX2"); break;
            case 7: sprintf (this->ChipID.ProcessorName,"i80486DX2 WriteBack"); break;
            case 8: sprintf (this->ChipID.ProcessorName,"i80486DX4"); break;
            case 9: sprintf (this->ChipID.ProcessorName,"i80486DX4 WriteBack"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown 80486 family"); return false;
            }
          break;
        case 5:
          switch (this->ChipID.Model) 
            {
            case 0: sprintf (this->ChipID.ProcessorName,"P5 A-Step"); break;
            case 1: sprintf (this->ChipID.ProcessorName,"P5"); break;
            case 2: sprintf (this->ChipID.ProcessorName,"P54C"); break;
            case 3: sprintf (this->ChipID.ProcessorName,"P24T OverDrive"); break;
            case 4: sprintf (this->ChipID.ProcessorName,"P55C"); break;
            case 7: sprintf (this->ChipID.ProcessorName,"P54C"); break;
            case 8: sprintf (this->ChipID.ProcessorName,"P55C (0.25micron)"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown Pentium family"); return false;
            }
          break;
        case 6:
          switch (this->ChipID.Model) 
            {
            case 0: sprintf (this->ChipID.ProcessorName,"P6 A-Step"); break;
            case 1: sprintf (this->ChipID.ProcessorName,"P6"); break;
            case 3: sprintf (this->ChipID.ProcessorName,"Pentium II (0.28 micron)"); break;
            case 5: sprintf (this->ChipID.ProcessorName,"Pentium II (0.25 micron)"); break;
            case 6: sprintf (this->ChipID.ProcessorName,"Pentium II With On-Die L2 Cache"); break;
            case 7: sprintf (this->ChipID.ProcessorName,"Pentium III (0.25 micron)"); break;
            case 8: sprintf (this->ChipID.ProcessorName,"Pentium III (0.18 micron) With 256 KB On-Die L2 Cache "); break;
            case 0xa: sprintf (this->ChipID.ProcessorName,"Pentium III (0.18 micron) With 1 Or 2 MB On-Die L2 Cache "); break;
            case 0xb: sprintf (this->ChipID.ProcessorName,"Pentium III (0.13 micron) With 256 Or 512 KB On-Die L2 Cache "); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown P6 family"); return false;
            }
          break;
        case 7:
          sprintf (this->ChipID.ProcessorName,"Intel Merced (IA-64)");
          break;
        case 0xf:
          // Check the extended family bits...
          switch (this->ChipID.ExtendedFamily) 
            {
            case 0:
              switch (this->ChipID.Model) 
                {
                case 0: sprintf (this->ChipID.ProcessorName,"Pentium IV (0.18 micron)"); break;
                case 1: sprintf (this->ChipID.ProcessorName,"Pentium IV (0.18 micron)"); break;
                case 2: sprintf (this->ChipID.ProcessorName,"Pentium IV (0.13 micron)"); break;
                default: sprintf (this->ChipID.ProcessorName,"Unknown Pentium 4 family"); return false;
                }
              break;
            case 1:
              sprintf (this->ChipID.ProcessorName,"Intel McKinley (IA-64)");
              break;
            default:
              sprintf (this->ChipID.ProcessorName,"Pentium");
            }
          break;
        default:
          sprintf (this->ChipID.ProcessorName,"Unknown Intel family");
          return false;
        }
      break;

    case AMD:
      // Check the family / model / revision to determine the CPU ID.
      switch (this->ChipID.Family) 
        {
        case 4:
          switch (this->ChipID.Model) 
            {
            case 3: sprintf (this->ChipID.ProcessorName,"80486DX2"); break;
            case 7: sprintf (this->ChipID.ProcessorName,"80486DX2 WriteBack"); break;
            case 8: sprintf (this->ChipID.ProcessorName,"80486DX4"); break;
            case 9: sprintf (this->ChipID.ProcessorName,"80486DX4 WriteBack"); break;
            case 0xe: sprintf (this->ChipID.ProcessorName,"5x86"); break;
            case 0xf: sprintf (this->ChipID.ProcessorName,"5x86WB"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown 80486 family"); return false;
            }
          break;
        case 5:
          switch (this->ChipID.Model) 
            {
            case 0: sprintf (this->ChipID.ProcessorName,"SSA5 (PR75, PR90, PR100)"); break;
            case 1: sprintf (this->ChipID.ProcessorName,"5k86 (PR120, PR133)"); break;
            case 2: sprintf (this->ChipID.ProcessorName,"5k86 (PR166)"); break;
            case 3: sprintf (this->ChipID.ProcessorName,"5k86 (PR200)"); break;
            case 6: sprintf (this->ChipID.ProcessorName,"K6 (0.30 micron)"); break;
            case 7: sprintf (this->ChipID.ProcessorName,"K6 (0.25 micron)"); break;
            case 8: sprintf (this->ChipID.ProcessorName,"K6-2"); break;
            case 9: sprintf (this->ChipID.ProcessorName,"K6-III"); break;
            case 0xd: sprintf (this->ChipID.ProcessorName,"K6-2+ or K6-III+ (0.18 micron)"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown 80586 family"); return false;
            }
          break;
        case 6:
          switch (this->ChipID.Model) 
            {
            case 1: sprintf (this->ChipID.ProcessorName,"Athlon- (0.25 micron)"); break;
            case 2: sprintf (this->ChipID.ProcessorName,"Athlon- (0.18 micron)"); break;
            case 3: sprintf (this->ChipID.ProcessorName,"Duron- (SF core)"); break;
            case 4: sprintf (this->ChipID.ProcessorName,"Athlon- (Thunderbird core)"); break;
            case 6: sprintf (this->ChipID.ProcessorName,"Athlon- (Palomino core)"); break;
            case 7: sprintf (this->ChipID.ProcessorName,"Duron- (Morgan core)"); break;
            case 8: 
              if (this->Features.ExtendedFeatures.SupportsMP)
                sprintf (this->ChipID.ProcessorName,"Athlon - MP (Thoroughbred core)"); 
              else sprintf (this->ChipID.ProcessorName,"Athlon - XP (Thoroughbred core)");
              break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown K7 family"); return false;
            }
          break;
        default:
          sprintf (this->ChipID.ProcessorName,"Unknown AMD family");
          return false;
        }
      break;

    case Transmeta:
      switch (this->ChipID.Family) 
        {  
        case 5:
          switch (this->ChipID.Model) 
            {
            case 4: sprintf (this->ChipID.ProcessorName,"Crusoe TM3x00 and TM5x00"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown Crusoe family"); return false;
            }
          break;
        default:
          sprintf (this->ChipID.ProcessorName,"Unknown Transmeta family");
          return false;
        }
      break;

    case Rise:
      switch (this->ChipID.Family) 
        {  
        case 5:
          switch (this->ChipID.Model) 
            {
            case 0: sprintf (this->ChipID.ProcessorName,"mP6 (0.25 micron)"); break;
            case 2: sprintf (this->ChipID.ProcessorName,"mP6 (0.18 micron)"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown Rise family"); return false;
            }
          break;
        default:
          sprintf (this->ChipID.ProcessorName,"Unknown Rise family");
          return false;
        }
      break;

    case UMC:
      switch (this->ChipID.Family) 
        {  
        case 4:
          switch (this->ChipID.Model) 
            {
            case 1: sprintf (this->ChipID.ProcessorName,"U5D"); break;
            case 2: sprintf (this->ChipID.ProcessorName,"U5S"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown UMC family"); return false;
            }
          break;
        default:
          sprintf (this->ChipID.ProcessorName,"Unknown UMC family");
          return false;
        }
      break;

    case IDT:
      switch (this->ChipID.Family) 
        {  
        case 5:
          switch (this->ChipID.Model) 
            {
            case 4: sprintf (this->ChipID.ProcessorName,"C6"); break;
            case 8: sprintf (this->ChipID.ProcessorName,"C2"); break;
            case 9: sprintf (this->ChipID.ProcessorName,"C3"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown IDT\\Centaur family"); return false;
            }
          break;
        case 6:
          switch (this->ChipID.Model) 
            {
            case 6: sprintf (this->ChipID.ProcessorName,"VIA Cyrix III - Samuel"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown IDT\\Centaur family"); return false;
            }
          break;
        default:
          sprintf (this->ChipID.ProcessorName,"Unknown IDT\\Centaur family");
          return false;
        }
      break;

    case Cyrix:
      switch (this->ChipID.Family) 
        {  
        case 4:
          switch (this->ChipID.Model) 
            {
            case 4: sprintf (this->ChipID.ProcessorName,"MediaGX GX, GXm"); break;
            case 9: sprintf (this->ChipID.ProcessorName,"5x86"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown Cx5x86 family"); return false;
            }
          break;
        case 5:
          switch (this->ChipID.Model) 
            {
            case 2: sprintf (this->ChipID.ProcessorName,"Cx6x86"); break;
            case 4: sprintf (this->ChipID.ProcessorName,"MediaGX GXm"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown Cx6x86 family"); return false;
            }
          break;
        case 6:
          switch (this->ChipID.Model) 
            {
            case 0: sprintf (this->ChipID.ProcessorName,"6x86MX"); break;
            case 5: sprintf (this->ChipID.ProcessorName,"Cyrix M2 Core"); break;
            case 6: sprintf (this->ChipID.ProcessorName,"WinChip C5A Core"); break;
            case 7: sprintf (this->ChipID.ProcessorName,"WinChip C5B\\C5C Core"); break;
            case 8: sprintf (this->ChipID.ProcessorName,"WinChip C5C-T Core"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown 6x86MX\\Cyrix III family"); return false;
            }
          break;
        default:
          sprintf (this->ChipID.ProcessorName,"Unknown Cyrix family");
          return false;
        }
      break;

    case NexGen:
      switch (this->ChipID.Family) 
        {  
        case 5:
          switch (this->ChipID.Model) 
            {
            case 0: sprintf (this->ChipID.ProcessorName,"Nx586 or Nx586FPU"); break;
            default: sprintf (this->ChipID.ProcessorName,"Unknown NexGen family"); return false;
            }
          break;
        default:
          sprintf (this->ChipID.ProcessorName,"Unknown NexGen family");
          return false;
        }
      break;

    case NSC:
      sprintf (this->ChipID.ProcessorName,"Cx486SLC \\ DLC \\ Cx486S A-Step");
      break;
    default:
      sprintf (this->ChipID.ProcessorName,"Unknown family"); // We cannot identify the processor.
      return false;
    }

  return true;
}

/** Extract a value from the CPUInfo file */
kwsys_stl::string SystemInformationImplementation::ExtractValueFromCpuInfoFile(kwsys_stl::string buffer,const char* word,size_t init)
{
  size_t pos = buffer.find(word,init);
  if(pos != buffer.npos)
    {
    this->CurrentPositionInFile = pos;
    pos = buffer.find(":",pos);
    size_t pos2 = buffer.find("\n",pos);
    if(pos!=buffer.npos && pos2!=buffer.npos)
      {
      return buffer.substr(pos+2,pos2-pos-2);
      }
    }
  this->CurrentPositionInFile = buffer.npos;
  return "";
}

/** Query for the cpu status */
int SystemInformationImplementation::RetreiveInformationFromCpuInfoFile()
{
  this->NumberOfLogicalCPU = 0;
  this->NumberOfPhysicalCPU = 0;
  kwsys_stl::string buffer;

  FILE *fd = fopen("/proc/cpuinfo", "r" );
  if ( !fd ) 
    {
    kwsys_ios::cout << "Problem opening /proc/cpuinfo" << kwsys_stl::endl;
    return 0;
    }
  
  size_t fileSize = 0;
  while(!feof(fd))
    {
    buffer += fgetc(fd);
    fileSize++;
    }
  fclose( fd );
  
  buffer.resize(fileSize-2);

  // Number of CPUs
  size_t pos = buffer.find("processor\t");
  while(pos != buffer.npos)
    {
    this->NumberOfLogicalCPU++;
    this->NumberOfPhysicalCPU++;
    pos = buffer.find("processor\t",pos+1);
    }

  // Count the number of physical ids that are the same
  int currentId = -1;
  kwsys_stl::string idc = this->ExtractValueFromCpuInfoFile(buffer,"physical id");

  while(this->CurrentPositionInFile != buffer.npos)
    {
    int id = atoi(idc.c_str());
    if(id == currentId)
      {
      this->NumberOfPhysicalCPU--;
      }
    currentId = id;
    idc = this->ExtractValueFromCpuInfoFile(buffer,"physical id",this->CurrentPositionInFile+1);
    }

   if(this->NumberOfPhysicalCPU>0)
     {
     this->NumberOfLogicalCPU /= this->NumberOfPhysicalCPU;
     }

  // CPU speed (checking only the first proc
  kwsys_stl::string CPUSpeed = this->ExtractValueFromCpuInfoFile(buffer,"cpu MHz");
  this->CPUSpeedInMHz = (float)atof(CPUSpeed.c_str());

  // Chip family
  this->ChipID.Family = atoi(this->ExtractValueFromCpuInfoFile(buffer,"cpu family").c_str());
 
  // Chip Vendor
  strcpy(this->ChipID.Vendor,this->ExtractValueFromCpuInfoFile(buffer,"vendor_id").c_str());
  this->FindManufacturer();
  
  // Chip Model
  this->ChipID.Model = atoi(this->ExtractValueFromCpuInfoFile(buffer,"model").c_str());
  this->RetrieveClassicalCPUIdentity();

  // L1 Cache size
  kwsys_stl::string cacheSize = this->ExtractValueFromCpuInfoFile(buffer,"cache size");
  pos = cacheSize.find(" KB");
  if(pos!=cacheSize.npos)
    {
    cacheSize = cacheSize.substr(0,pos);
    }
  this->Features.L1CacheSize = atoi(cacheSize.c_str());


  return 1;
}

/** Query for the memory status */
int SystemInformationImplementation::QueryMemory()
{
  this->TotalVirtualMemory = 0;
  this->TotalPhysicalMemory = 0;
  this->AvailableVirtualMemory = 0;
  this->AvailablePhysicalMemory = 0;
#ifdef __CYGWIN__
  return 0;
#elif _WIN32
#if  _MSC_VER < 1300
  MEMORYSTATUS ms;
  GlobalMemoryStatus(&ms);
#define MEM_VAL(value) dw##value
#else
  MEMORYSTATUSEX ms;
  GlobalMemoryStatusEx(&ms);
#define MEM_VAL(value) ull##value
#endif
  unsigned long tv = ms.MEM_VAL(TotalVirtual);
  unsigned long tp = ms.MEM_VAL(TotalPhys);
  unsigned long av = ms.MEM_VAL(AvailVirtual);
  unsigned long ap = ms.MEM_VAL(AvailPhys);
  this->TotalVirtualMemory = tv>>10>>10;
  this->TotalPhysicalMemory = tp>>10>>10;
  this->AvailableVirtualMemory = av>>10>>10;
  this->AvailablePhysicalMemory = ap>>10>>10;
  return 1;
#elif __linux
  unsigned long tv=0;
  unsigned long tp=0;
  unsigned long av=0;
  unsigned long ap=0;
  
  char buffer[1024]; // for skipping unused lines
  
  int linuxMajor = 0;
  int linuxMinor = 0;
  
  // Find the Linux kernel version first
  struct utsname unameInfo;
  int errorFlag = uname(&unameInfo);
  if( errorFlag!=0 )
    {
    kwsys_ios::cout << "Problem calling uname(): " << strerror(errno) << kwsys_stl::endl;
    return 0;
    }
 
  if( unameInfo.release!=0 && strlen(unameInfo.release)>=3 )
    {
    // release looks like "2.6.3-15mdk-i686-up-4GB"
    char majorChar=unameInfo.release[0];
    char minorChar=unameInfo.release[2];
    
    if( isdigit(majorChar) )
      {
      linuxMajor=majorChar-'0';
      }
    
    if( isdigit(minorChar) )
      {
      linuxMinor=minorChar-'0';
      }
    }
  
  FILE *fd = fopen("/proc/meminfo", "r" );
  if ( !fd ) 
    {
    kwsys_ios::cout << "Problem opening /proc/meminfo" << kwsys_stl::endl;
    return 0;
    }
  
  if( linuxMajor>=3 || ( (linuxMajor>=2) && (linuxMinor>=6) ) )
    {
    // new /proc/meminfo format since kernel 2.6.x
    // Rigorously, this test should check from the developping version 2.5.x
    // that introduced the new format...
    
    long freeMem;
    long buffersMem;
    long cachedMem;
    
    fscanf(fd,"MemTotal:%ld kB\n", &this->TotalPhysicalMemory);
    fscanf(fd,"MemFree:%ld kB\n", &freeMem);
    fscanf(fd,"Buffers:%ld kB\n", &buffersMem);
    fscanf(fd,"Cached:%ld kB\n", &cachedMem);
    
    this->TotalPhysicalMemory /= 1024;
    this->AvailablePhysicalMemory = freeMem+cachedMem+buffersMem;
    this->AvailablePhysicalMemory /= 1024;
    
    // Skip SwapCached, Active, Inactive, HighTotal, HighFree, LowTotal
    // and LowFree.
    int i=0;
    while(i<7)
      {
      fgets(buffer, sizeof(buffer), fd); // skip a line
      ++i;
      }
    
    fscanf(fd,"SwapTotal:%ld kB\n", &this->TotalVirtualMemory);
    fscanf(fd,"SwapFree:%ld kB\n", &this->AvailableVirtualMemory);

    this->TotalVirtualMemory /= 1024;
    this->AvailableVirtualMemory /= 1024;
    }
  else
    {
    // /proc/meminfo format for kernel older than 2.6.x
    
    unsigned long temp;
    unsigned long cachedMem;
    unsigned long buffersMem;
    fgets(buffer, sizeof(buffer), fd); // Skip "total: used:..."
    
    fscanf(fd, "Mem: %lu %lu %lu %lu %lu %lu\n",
         &tp, &temp, &ap, &temp, &buffersMem, &cachedMem);
    fscanf(fd, "Swap: %lu %lu %lu\n", &tv, &temp, &av);
    
    this->TotalVirtualMemory = tv>>10>>10;
    this->TotalPhysicalMemory = tp>>10>>10;
    this->AvailableVirtualMemory = av>>10>>10;
    this->AvailablePhysicalMemory = (ap+buffersMem+cachedMem)>>10>>10;
    }
  fclose( fd );
  return 1;
#elif __hpux
  unsigned long tv=0;
  unsigned long tp=0;
  unsigned long av=0;
  unsigned long ap=0;
  struct pst_static pst;
  struct pst_dynamic pdy;
     
  unsigned long ps = 0;
  if (pstat_getstatic(&pst, sizeof(pst), (size_t) 1, 0) != -1)
    {
    ps = pst.page_size;
    tp =  pst.physical_memory *ps;
    tv = (pst.physical_memory + pst.pst_maxmem) * ps;
    if (pstat_getdynamic(&pdy, sizeof(pdy), (size_t) 1, 0) != -1)
      {
      ap = tp - pdy.psd_rm * ps;
      av = tv - pdy.psd_vm;
      this->TotalVirtualMemory = tv>>10>>10;
      this->TotalPhysicalMemory = tp>>10>>10;
      this->AvailableVirtualMemory = av>>10>>10;
      this->AvailablePhysicalMemory = ap>>10>>10;
      return 1;
      }
    }
  return 0;
#else
  return 0;
#endif


}

/** */
unsigned long SystemInformationImplementation::GetTotalVirtualMemory() 
{ 
  return this->TotalVirtualMemory; 
}

/** */
unsigned long SystemInformationImplementation::GetAvailableVirtualMemory() 
{ 
  return this->AvailableVirtualMemory; 
}

unsigned long SystemInformationImplementation::GetTotalPhysicalMemory() 
{ 
  return this->TotalPhysicalMemory; 
}

/** */
unsigned long SystemInformationImplementation::GetAvailablePhysicalMemory() 
{ 
  return this->AvailablePhysicalMemory; 
}

/** Get Cycle differences */
LongLong SystemInformationImplementation::GetCyclesDifference (DELAY_FUNC DelayFunction,
                                                  unsigned int uiParameter)
{
#if USE_ASM_INSTRUCTIONS

  unsigned int edx1, eax1;
  unsigned int edx2, eax2;

  // Calculate the frequency of the CPU instructions.
  __try {
    _asm {
      push uiParameter ; push parameter param
      mov ebx, DelayFunction ; store func in ebx

      RDTSC_INSTRUCTION

      mov esi, eax ; esi = eax
      mov edi, edx ; edi = edx

      call ebx ; call the delay functions

      RDTSC_INSTRUCTION

      pop ebx

      mov edx2, edx      ; edx2 = edx
      mov eax2, eax      ; eax2 = eax

      mov edx1, edi      ; edx2 = edi
      mov eax1, esi      ; eax2 = esi
    }
  }
  __except(1) 
    {
    return -1;
    }

  return ((((__int64) edx2 << 32) + eax2) - (((__int64) edx1 << 32) + eax1));

#else
  (void)DelayFunction;
  (void)uiParameter;
  return -1;
#endif
}

/** Compute the delay overhead */
void SystemInformationImplementation::DelayOverhead(unsigned int uiMS)
{
#if _WIN32
  LARGE_INTEGER Frequency, StartCounter, EndCounter;
  __int64 x;

  // Get the frequency of the high performance counter.
  if(!QueryPerformanceFrequency (&Frequency)) 
    {
    return;
    }
  x = Frequency.QuadPart / 1000 * uiMS;

  // Get the starting position of the counter.
  QueryPerformanceCounter (&StartCounter);
  
  do {
    // Get the ending position of the counter.  
    QueryPerformanceCounter (&EndCounter);
  } while (EndCounter.QuadPart - StartCounter.QuadPart == x);
#endif
  (void)uiMS;
}

/** Return the number of logical CPU per physical CPUs Works only for windows */
unsigned char SystemInformationImplementation::LogicalCPUPerPhysicalCPU(void)
{
  unsigned int Regebx = 0;
#if USE_ASM_INSTRUCTIONS
  if (!this->IsHyperThreadingSupported()) 
    {
    return (unsigned char) 1;  // HT not supported
    }
  __asm
    {
    mov eax, 1
    cpuid
    mov Regebx, ebx
    }
#endif
  return (unsigned char) ((Regebx & NUM_LOGICAL_BITS) >> 16);
}

/** Works only for windows */
unsigned int SystemInformationImplementation::IsHyperThreadingSupported()
{
#if USE_ASM_INSTRUCTIONS
  unsigned int Regedx    = 0,
             Regeax      = 0,
             VendorId[3] = {0, 0, 0};
  __try    // Verify cpuid instruction is supported
    {
      __asm
      {
        xor eax, eax          // call cpuid with eax = 0
            cpuid                 // Get vendor id string
        mov VendorId, ebx
        mov VendorId + 4, edx
        mov VendorId + 8, ecx
        
        mov eax, 1            // call cpuid with eax = 1
        cpuid
        mov Regeax, eax      // eax contains family processor type
        mov Regedx, edx      // edx has info about the availability of hyper-Threading
      }
    }
  __except (EXCEPTION_EXECUTE_HANDLER)
    {
    return(0);                   // cpuid is unavailable
    }

  if (((Regeax & FAMILY_ID) == PENTIUM4_ID) || (Regeax & EXT_FAMILY_ID))
    {
    if (VendorId[0] == 'uneG')
      {
      if (VendorId[1] == 'Ieni')
        {
        if (VendorId[2] == 'letn')
          {
          return(Regedx & HT_BIT);    // Genuine Intel with hyper-Threading technology
          }
        }
      }
    }
#endif

  return 0;    // Not genuine Intel processor
}

/** Return the APIC Id. Works only for windows. */
unsigned char SystemInformationImplementation::GetAPICId()
{
  unsigned int Regebx = 0;
#if USE_ASM_INSTRUCTIONS
  if (!this->IsHyperThreadingSupported()) 
    {
    return (unsigned char) -1;  // HT not supported
    } // Logical processor = 1
  __asm
    {
    mov eax, 1
    cpuid
    mov Regebx, ebx
    }
#endif
  return (unsigned char) ((Regebx & INITIAL_APIC_ID_BITS) >> 24);
}

/** Count the number of CPUs. Works only on windows. */
int SystemInformationImplementation::CPUCount()
{
#if _WIN32
  unsigned char StatusFlag  = 0;
  SYSTEM_INFO info;

  this->NumberOfPhysicalCPU = 0;
  this->NumberOfLogicalCPU = 0;
  info.dwNumberOfProcessors = 0;
  GetSystemInfo (&info);

  // Number of physical processors in a non-Intel system
  // or in a 32-bit Intel system with Hyper-Threading technology disabled
  this->NumberOfPhysicalCPU = (unsigned char) info.dwNumberOfProcessors;  

  if (this->IsHyperThreadingSupported())
    {
    unsigned char HT_Enabled = 0;
    this->NumberOfLogicalCPU = this->LogicalCPUPerPhysicalCPU();
    if (this->NumberOfLogicalCPU >= 1)    // >1 Doesn't mean HT is enabled in the BIOS
      {
      HANDLE hCurrentProcessHandle;
#ifndef _WIN64
# define DWORD_PTR DWORD
#endif
      DWORD_PTR  dwProcessAffinity;
      DWORD_PTR  dwSystemAffinity;
      DWORD  dwAffinityMask;

      // Calculate the appropriate  shifts and mask based on the 
      // number of logical processors.
      unsigned int i = 1;
      unsigned char PHY_ID_MASK  = 0xFF;
      //unsigned char PHY_ID_SHIFT = 0;

      while (i < this->NumberOfLogicalCPU)
        {
        i *= 2;
         PHY_ID_MASK  <<= 1;
         // PHY_ID_SHIFT++;
        }
      
      hCurrentProcessHandle = GetCurrentProcess();
      GetProcessAffinityMask(hCurrentProcessHandle, &dwProcessAffinity,
                                                  &dwSystemAffinity);

      // Check if available process affinity mask is equal to the
      // available system affinity mask
      if (dwProcessAffinity != dwSystemAffinity)
        {
        StatusFlag = HT_CANNOT_DETECT;
        this->NumberOfPhysicalCPU = (unsigned char)-1;
        return StatusFlag;
        }

      dwAffinityMask = 1;
      while (dwAffinityMask != 0 && dwAffinityMask <= dwProcessAffinity)
        {
        // Check if this CPU is available
        if (dwAffinityMask & dwProcessAffinity)
          {
          if (SetProcessAffinityMask(hCurrentProcessHandle,
                                     dwAffinityMask))
            {
            unsigned char APIC_ID, LOG_ID;
            Sleep(0); // Give OS time to switch CPU

            APIC_ID = GetAPICId();
            LOG_ID  = APIC_ID & ~PHY_ID_MASK;
 
            if (LOG_ID != 0) 
              {
              HT_Enabled = 1;
              }
            }
          }
        dwAffinityMask = dwAffinityMask << 1;
        }
      // Reset the processor affinity
      SetProcessAffinityMask(hCurrentProcessHandle, dwProcessAffinity);
            
      if (this->NumberOfLogicalCPU == 1)  // Normal P4 : HT is disabled in hardware
        {
        StatusFlag = HT_DISABLED;
        }
      else
        {
        if (HT_Enabled)
          {
          // Total physical processors in a Hyper-Threading enabled system.
          this->NumberOfPhysicalCPU /= (this->NumberOfLogicalCPU);
          StatusFlag = HT_ENABLED;
          }
        else 
          {
          StatusFlag = HT_SUPPORTED_NOT_ENABLED;
          }
        }
      }
    }
  else
    {
    // Processors do not have Hyper-Threading technology
    StatusFlag = HT_NOT_CAPABLE;
    this->NumberOfLogicalCPU = 1;
    }
  return StatusFlag;
#else
  return 0;
#endif
}

/** Return the number of logical CPUs on the system */
unsigned int SystemInformationImplementation::GetNumberOfLogicalCPU()
{
  return this->NumberOfLogicalCPU;
}

/** Return the number of physical CPUs on the system */
unsigned int SystemInformationImplementation::GetNumberOfPhysicalCPU()
{
  return this->NumberOfPhysicalCPU;
}

/** For Mac we Parse the sysctl -a output */
bool SystemInformationImplementation::ParseSysCtl()
{
  // Extract the arguments from the command line
  kwsys_stl::vector<const char*> args;
  args.push_back("sysctl");
  args.push_back("-a");
  args.push_back(0);

  this->SysCtlBuffer = this->RunProcess(args);
   
  // Parse values for Mac
  this->TotalPhysicalMemory = atoi(this->ExtractValueFromSysCtl("hw.memsize:").c_str())/(1024*1024);
  this->TotalVirtualMemory = 0;
  this->AvailablePhysicalMemory = 0;
  this->AvailableVirtualMemory = 0;

  this->NumberOfPhysicalCPU = atoi(this->ExtractValueFromSysCtl("hw.physicalcpu:").c_str());
  this->NumberOfLogicalCPU = atoi(this->ExtractValueFromSysCtl("hw.logicalcpu:").c_str());
  
  if(this->NumberOfPhysicalCPU!=0)
    {
    this->NumberOfLogicalCPU /= this->NumberOfPhysicalCPU;
    }

  this->CPUSpeedInMHz = atoi(this->ExtractValueFromSysCtl("hw.cpufrequency:").c_str()); 
  this->CPUSpeedInMHz /= 1000000;

  // Chip family
  this->ChipID.Family = atoi(this->ExtractValueFromSysCtl("machdep.cpu.family:").c_str()); 
 
  // Chip Vendor
  strcpy(this->ChipID.Vendor,this->ExtractValueFromSysCtl("machdep.cpu.vendor:").c_str());
  this->FindManufacturer();
  
  // Chip Model
  this->ChipID.Model = atoi(this->ExtractValueFromSysCtl("machdep.cpu.model:").c_str());
  this->RetrieveClassicalCPUIdentity();

  // Cache size
  this->Features.L1CacheSize = atoi(this->ExtractValueFromSysCtl("hw.l1icachesize:").c_str());  
  this->Features.L2CacheSize = atoi(this->ExtractValueFromSysCtl("hw.l2cachesize:").c_str());  

  return true;
}

/** Extract a value from sysctl command */
kwsys_stl::string SystemInformationImplementation::ExtractValueFromSysCtl(const char* word)
{
  size_t pos = this->SysCtlBuffer.find(word);
  if(pos != this->SysCtlBuffer.npos)
    {
    pos = this->SysCtlBuffer.find(": ",pos);
    size_t pos2 = this->SysCtlBuffer.find("\n",pos);
    if(pos!=this->SysCtlBuffer.npos && pos2!=this->SysCtlBuffer.npos)
      {
      return this->SysCtlBuffer.substr(pos+2,pos2-pos-2);
      }
    }
  return "";
}

/** Run a given process */
kwsys_stl::string SystemInformationImplementation::RunProcess(kwsys_stl::vector<const char*> args)
{ 
  kwsys_stl::string buffer = "";

  // Run the application
  kwsysProcess* gp = kwsysProcess_New();
  kwsysProcess_SetCommand(gp, &*args.begin());
  kwsysProcess_SetOption(gp,kwsysProcess_Option_HideWindow,1);

  kwsysProcess_Execute(gp);

  char* data = NULL;
  int length;
  double timeout = 255;

  while(kwsysProcess_WaitForData(gp,&data,&length,&timeout)) // wait for 1s
    {
    for(int i=0;i<length;i++)
      {
      buffer += data[i];
      }
    }
  kwsysProcess_WaitForExit(gp, 0);

  int result = 0;
  switch(kwsysProcess_GetState(gp))
    {
    case kwsysProcess_State_Exited:
      {
      result = kwsysProcess_GetExitValue(gp);
      } break;
    case kwsysProcess_State_Error:
      {
      kwsys_ios::cerr << "Error: Could not run " << args[0] << ":\n";
      kwsys_ios::cerr << kwsysProcess_GetErrorString(gp) << "\n";
      } break;
    case kwsysProcess_State_Exception:
      {
      kwsys_ios::cerr << "Error: " << args[0]
                << " terminated with an exception: "
                << kwsysProcess_GetExceptionString(gp) << "\n";
      } break;
    case kwsysProcess_State_Starting:
    case kwsysProcess_State_Executing:
    case kwsysProcess_State_Expired:
    case kwsysProcess_State_Killed:
      {
      // Should not get here.
      kwsys_ios::cerr << "Unexpected ending state after running " << args[0]
                << kwsys_stl::endl;
      } break;
    }
  kwsysProcess_Delete(gp);
  if(result)
    {
    kwsys_ios::cerr << "Error " << args[0] << " returned :" << result << "\n";
    }
  return buffer;
}
  

kwsys_stl::string SystemInformationImplementation::ParseValueFromKStat(const char* arguments)
{
  kwsys_stl::vector<const char*> args;
  args.clear();
  args.push_back("kstat");
  args.push_back("-p");
  
  kwsys_stl::string command = arguments;
  size_t start = command.npos;
  size_t pos = command.find(' ',0);
  while(pos!=command.npos)
    {
    bool inQuotes = false;
    // Check if we are between quotes
    size_t b0 = command.find('"',0);
    size_t b1 = command.find('"',b0+1);
    while(b0 != command.npos && b1 != command.npos && b1>b0)
      {
      if(pos>b0 && pos<b1)
        {
        inQuotes = true;
        break;
        }
      b0 = command.find('"',b1+1);
      b1 = command.find('"',b0+1);
      }
    
    if(!inQuotes)
      {
      kwsys_stl::string arg = command.substr(start+1,pos-start-1);

      // Remove the quotes if any
      size_t quotes = arg.find('"');
      while(quotes != arg.npos)
        {
        arg.erase(quotes,1);
        quotes = arg.find('"');
        }
      args.push_back(arg.c_str());  
      start = pos;
      }
    pos = command.find(' ',pos+1);
    }
  kwsys_stl::string lastArg = command.substr(start+1,command.size()-start-1);
  args.push_back(lastArg.c_str());

  args.push_back(0);

  kwsys_stl::string buffer = this->RunProcess(args);

  kwsys_stl::string value = "";
  for(size_t i=buffer.size()-1;i>0;i--)
    {
    if(buffer[i] == ' ' || buffer[i] == '\t')
      {
      break;
      }   
    if(buffer[i] != '\n' && buffer[i] != '\r')
      {
      kwsys_stl::string val = value;
      value = buffer[i];
      value += val;
      }          
    }
  return value;
}

/** Querying for system information from Solaris */
bool SystemInformationImplementation::QuerySolarisInfo()
{
  // Parse values
  this->NumberOfPhysicalCPU = atoi(this->ParseValueFromKStat("-n systethis->misc -s ncpus").c_str());
  this->NumberOfLogicalCPU = this->NumberOfPhysicalCPU;
  
  if(this->NumberOfPhysicalCPU!=0)
    {
    this->NumberOfLogicalCPU /= this->NumberOfPhysicalCPU;
    }

  this->CPUSpeedInMHz = atoi(this->ParseValueFromKStat("-s clock_MHz").c_str());

  // Chip family
  this->ChipID.Family = 0; 
 
  // Chip Vendor
  strcpy(this->ChipID.Vendor,"Sun");
  this->FindManufacturer();
  
  // Chip Model
  sprintf(this->ChipID.ProcessorName,"%s",this->ParseValueFromKStat("-s cpu_type").c_str());
  this->ChipID.Model = 0;

  // Cache size
  this->Features.L1CacheSize = 0; 
  this->Features.L2CacheSize = 0;  

  char* tail;
  unsigned long totalMemory =
       strtoul(this->ParseValueFromKStat("-s physmem").c_str(),&tail,0);
  this->TotalPhysicalMemory = totalMemory/1024;
  this->TotalPhysicalMemory *= 8192;
  this->TotalPhysicalMemory /= 1024;

  // Undefined values (for now at least)
  this->TotalVirtualMemory = 0;
  this->AvailablePhysicalMemory = 0;
  this->AvailableVirtualMemory = 0;

  return true;
}

/** Query the operating system information */
bool SystemInformationImplementation::QueryOSInformation()
{
#if _WIN32

  this->OSName = "Windows";

  OSVERSIONINFOEX osvi;
  BOOL bIsWindows64Bit;
  BOOL bOsVersionInfoEx;
  char operatingSystem[256];

  // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
  ZeroMemory (&osvi, sizeof (OSVERSIONINFOEX));
  osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFOEX);
  bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi);
  if (!bOsVersionInfoEx) 
    {
    osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
    if (!GetVersionEx ((OSVERSIONINFO *) &osvi)) 
      {
      return false;
      }
    }

  switch (osvi.dwPlatformId) 
    {
    case VER_PLATFORM_WIN32_NT:
      // Test for the product.
      if (osvi.dwMajorVersion <= 4) 
        {
        this->OSRelease = "NT";
        }
      if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) 
        {
        this->OSRelease = "2000";
        }
      if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) 
        {
        this->OSRelease = "XP";
        }
#ifdef VER_NT_WORKSTATION
      // Test for product type.
      if (bOsVersionInfoEx) 
        {
        if (osvi.wProductType == VER_NT_WORKSTATION) 
          {
// VER_SUITE_PERSONAL may not be defined
#ifdef VER_SUITE_PERSONAL
          if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
            {
            this->OSRelease += " Personal";
            }
          else 
            {
            this->OSRelease += " Professional";
            }
#endif
          } 
        else if (osvi.wProductType == VER_NT_SERVER)
          {
          // Check for .NET Server instead of Windows XP.
          if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) 
            {
            this->OSRelease = ".NET";
            }

          // Continue with the type detection.
          if (osvi.wSuiteMask & VER_SUITE_DATACENTER) 
            {
            this->OSRelease += " DataCenter Server";
            }
          else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE) 
            {
            this->OSRelease += " Advanced Server";
            }
          else 
            {
            this->OSRelease += " Server";
            }
          }

        sprintf (operatingSystem, "%s(Build %d)", osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
        this->OSVersion = operatingSystem; 
        }
      else 
#endif        // VER_NT_WORKSTATION
        {
        HKEY hKey;
        char szProductType[80];
        DWORD dwBufLen;

        // Query the registry to retrieve information.
        RegOpenKeyEx (HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_QUERY_VALUE, &hKey);
        RegQueryValueEx (hKey, "ProductType", NULL, NULL, (LPBYTE) szProductType, &dwBufLen);
        RegCloseKey (hKey);

        if (lstrcmpi ("WINNT", szProductType) == 0)
          {
          this->OSRelease += " Professional";
          }
        if (lstrcmpi ("LANMANNT", szProductType) == 0)
          {
          // Decide between Windows 2000 Advanced Server and Windows .NET Enterprise Server.
          if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
            {
            this->OSRelease += " Standard Server";
            }
          else 
            {
            this->OSRelease += " Server";
            }
          }
        if (lstrcmpi ("SERVERNT", szProductType) == 0)
          {
          // Decide between Windows 2000 Advanced Server and Windows .NET Enterprise Server.
          if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
            {
            this->OSRelease += " Enterprise Server";
            }
          else 
            {
            this->OSRelease += " Advanced Server";
            }
          }
         }

      // Display version, service pack (if any), and build number.
      if (osvi.dwMajorVersion <= 4) 
        {
        // NB: NT 4.0 and earlier.
        sprintf (operatingSystem, "version %d.%d %s (Build %d)",
                 osvi.dwMajorVersion,
                 osvi.dwMinorVersion,
                 osvi.szCSDVersion,
                 osvi.dwBuildNumber & 0xFFFF);
        this->OSVersion = operatingSystem;
        } 
      else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) 
        {
        // Windows XP and .NET server.
        typedef BOOL (CALLBACK* LPFNPROC) (HANDLE, BOOL *);
        HINSTANCE hKernelDLL; 
        LPFNPROC DLLProc;
        
        // Load the Kernel32 DLL.
        hKernelDLL = LoadLibrary ("kernel32");
        if (hKernelDLL != NULL)  { 
          // Only XP and .NET Server support IsWOW64Process so... Load dynamically!
          DLLProc = (LPFNPROC) GetProcAddress (hKernelDLL, "IsWow64Process"); 
         
          // If the function address is valid, call the function.
          if (DLLProc != NULL) (DLLProc) (GetCurrentProcess (), &bIsWindows64Bit);
          else bIsWindows64Bit = false;
         
          // Free the DLL module.
          FreeLibrary (hKernelDLL); 
          } 
        } 
      else 
        { 
        // Windows 2000 and everything else.
        sprintf (operatingSystem,"%s(Build %d)", osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
        this->OSVersion = operatingSystem;
        }
      break;

    case VER_PLATFORM_WIN32_WINDOWS:
      // Test for the product.
      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) 
        {
        this->OSRelease = "95";
        if(osvi.szCSDVersion[1] == 'C') 
          {
          this->OSRelease += "OSR 2.5";
          }
        else if(osvi.szCSDVersion[1] == 'B') 
          {
          this->OSRelease += "OSR 2";
          }
      } 

      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) 
        {
        this->OSRelease = "98";
        if (osvi.szCSDVersion[1] == 'A' ) 
          {
          this->OSRelease += "SE";
          }
        } 

      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) 
        {
        this->OSRelease = "Me";
        } 
      break;

    case VER_PLATFORM_WIN32s:
      this->OSRelease = "Win32s";
      break;

    default:
      this->OSRelease = "Unknown";
      break;
  }

  // Get the hostname
  WORD wVersionRequested;
  WSADATA wsaData;
  char name[255];
  wVersionRequested = MAKEWORD(2,0);

  if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
    {
    gethostname(name,sizeof(name));
    WSACleanup( );
    }
  this->Hostname = name;

#else

  struct utsname unameInfo;
  int errorFlag = uname(&unameInfo);
  if(errorFlag == 0)
    {
    this->OSName = unameInfo.sysname;
    this->Hostname = unameInfo.nodename;
    this->OSRelease = unameInfo.release;
    this->OSVersion = unameInfo.version;
    this->OSPlatform = unameInfo.machine;
    }
#endif

  return true;

}

/** Return true if the machine is 64 bits */
bool SystemInformationImplementation::Is64Bits()
{
  return (sizeof(void*) == 8);
}

} // namespace @KWSYS_NAMESPACE@
