/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/common/PlatformUtils.cpp

  \brief This file contains several utility functions when dealing with Linux specific API.
*/

// TerraLib
#include "../BuildConfig.h"
#include "../Defines.h"
#include "Exception.h"
#include "PlatformUtils.h"
#include "StringUtils.h"
#include "Translator.h"

// STL
#include <fstream>

// O.S. Specific
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#include <windows.h>
#include <winbase.h>

#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_AIX || TE_PLATFORM == TE_PLATFORMCODE_FREEBSD || TE_PLATFORM == TE_PLATFORMCODE_OPENBSD
#include <cstring>
#include <errno.h>
#include <dirent.h>
#include <malloc.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <sys/types.h>  
#include <unistd.h>

#elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>  
#include <sys/sysctl.h>
#include <mach/mach.h> 

#else
  #error "Unsuported plataform for physical memory checking"
#endif

#include <cstdio>
#include <cstdlib>

// Boost
#include <boost/filesystem.hpp>

unsigned long int te::common::GetFreePhysicalMemory()
{
      unsigned long int freemem = 0;

#if TE_PLATFORM == TE_PLATFORMCODE_FREEBSD || TE_PLATFORM == TE_PLATFORMCODE_OPENBSD || TE_PLATFORM == TE_PLATFORMCODE_APPLE
      int64_t usermem = 0;

      std::size_t usermem_len = sizeof(usermem);

      int mib[2] = { CTL_HW, HW_USERMEM };
        
      if(sysctl(mib, 2, &usermem, &usermem_len, NULL, 0) == 0)
      {
        freemem = static_cast<unsigned long int>(usermem);
      }
      else
      {
        throw Exception("Could not get free physical memory!");
      }

#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
      freemem = static_cast<unsigned long int>( sysconf(_SC_PAGESIZE) * sysconf(_SC_AVPHYS_PAGES) );

#elif TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      LPMEMORYSTATUS status_buffer = new MEMORYSTATUS;
      GlobalMemoryStatus(status_buffer);
      freemem = static_cast<unsigned long int>(status_buffer->dwAvailPhys);
      delete status_buffer;
#else
  #error "Unsuported plataform for physical memory checking"
#endif

      return freemem;
}

unsigned long int te::common::GetTotalPhysicalMemory()
{
  unsigned long int totalmem = 0;

  #if TE_PLATFORM == TE_PLATFORMCODE_FREEBSD || TE_PLATFORM == TE_PLATFORMCODE_OPENBSD || TE_PLATFORM == TE_PLATFORMCODE_APPLE
    #ifdef HW_MEMSIZE /* OSX. --------------------- */
      int64_t physmem = 0;
      int mib[2] = { CTL_HW, HW_MEMSIZE };
    #elif defined(HW_PHYSMEM) /* FreeBSD. ----------------- */
      unsigned int physmem = 0;
      int mib[2] = { CTL_HW, HW_PHYSMEM };
    #elif defined(HW_PHYSMEM64) /* DragonFly BSD. ----------- */
      int64_t physmem = 0;
      int mib[2] = { CTL_HW, HW_PHYSMEM64 };
    #elif defined(HW_REALMEM) /* FreeBSD. ----------------- */
      unsigned int physmem = 0;
      int mib[2] = { CTL_HW, HW_REALMEM };
    #endif

    std::size_t physmem_len = sizeof(physmem);

    if(sysctl(mib, 2, &physmem, &physmem_len, NULL, 0) == 0)
    {
      totalmem = static_cast<unsigned long int>(physmem); 
    }
    else
    {
      throw Exception("Could not get total physical memory!");
    }
  #elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
    totalmem = static_cast<unsigned long int>( sysconf(_SC_PAGESIZE) * sysconf(_SC_PHYS_PAGES) );

  #elif TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
    LPMEMORYSTATUS status_buffer = new MEMORYSTATUS;
    GlobalMemoryStatus(status_buffer);
    totalmem = static_cast<unsigned long int>(status_buffer->dwTotalPhys);
    delete status_buffer;
  #else
  #error "Unsuported plataform for physical memory checking"
  #endif

  return totalmem;
}

unsigned long int te::common::GetUsedVirtualMemory()
{
      unsigned long int usedmem = 0;
      
#if TE_PLATFORM == TE_PLATFORMCODE_FREEBSD || TE_PLATFORM == TE_PLATFORMCODE_OPENBSD
      struct rusage rusageinfo;
      getrusage( RUSAGE_SELF, &rusageinfo );
      usedmem = static_cast<unsigned long int>(1024 * rusageinfo.ru_maxrss);

#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
      std::string pid, comm, state, ppid, pgrp, session, tty_nr, 
                  tpgid, flags, minflt, cminflt, majflt, cmajflt,
                  utime, stime, cutime, cstime, priority, nice,
                  stringO, itrealvalue, starttime;
      
      std::ifstream stat_stream("/proc/self/stat", std::ios_base::in); 
      if( !stat_stream.is_open() )
      {
        throw Exception("Could not get the used virtual memory!");
      }
        
      stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr 
                  >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt 
                  >> utime >> stime >> cutime >> cstime >> priority >> nice 
                  >> stringO >> itrealvalue >> starttime >> usedmem;    

#elif TE_PLATFORM == TE_PLATFORMCODE_AIX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if ( task_info( mach_task_self( ), MACH_TASK_BASIC_INFO,
                   (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
        throw;
    usedmem = (unsigned long int)info.resident_size;

#elif TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      LPMEMORYSTATUS status_buffer = new MEMORYSTATUS;
      GlobalMemoryStatus( status_buffer );
      usedmem = static_cast<unsigned long int>(status_buffer->dwTotalVirtual - status_buffer->dwAvailVirtual);
      delete status_buffer;

#else
  #error "Unsuported plataform for virtual memory checking"
#endif

      return usedmem;
}


unsigned long int te::common::GetTotalVirtualMemory()
{
      unsigned long int totalmem = 0;

#if (TE_PLATFORM == TE_PLATFORMCODE_FREEBSD) || (TE_PLATFORM == TE_PLATFORMCODE_OPENBSD) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE) || (TE_PLATFORM == TE_PLATFORMCODE_LINUX)
      struct rlimit info;
        
      if( getrlimit( RLIMIT_AS, &info ) == 0 )
      {
        totalmem = (unsigned long int)info.rlim_max;
      }

#elif TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      LPMEMORYSTATUS status_buffer = new MEMORYSTATUS;
      GlobalMemoryStatus( status_buffer );
      totalmem = (unsigned long int) status_buffer->dwTotalVirtual;
      delete status_buffer;

#else
  #error "Unsuported plataform for virtual memory checking"
#endif

      return totalmem;
}

unsigned int te::common::GetPhysProcNumber()
{
      unsigned int procnmb = 0;
      
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      SYSTEM_INFO siSysInfo;
      GetSystemInfo(&siSysInfo);
      procnmb = static_cast<unsigned int>(siSysInfo.dwNumberOfProcessors);

#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_AIX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
      procnmb = static_cast<unsigned int>(sysconf(_SC_NPROCESSORS_ONLN));

#else
  #error "ERROR: Unsupported platform"
#endif    

      return procnmb;
}
    
void te::common::GetDecompostedPathEnvVar( std::vector< std::string >& paths )
{
      paths.clear();
      
      char* varValuePtr = getenv("PATH");
      
      std::string separator;
      #if (TE_PLATFORM == TE_PLATFORMCODE_FREEBSD) || (TE_PLATFORM == TE_PLATFORMCODE_OPENBSD) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE) || (TE_PLATFORM == TE_PLATFORMCODE_LINUX)
        separator = ":";
      #elif TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        separator = ";";
      #else
        #error "Unsuported plataform for virtual memory checking"
      #endif
      
      if( varValuePtr )
      {
        Tokenize( std::string( varValuePtr ), paths, separator );
      }
}
    
void te::common::GetDecompostedLDPathEnvVar( std::vector< std::string >& paths )
{
      paths.clear();
      
      char* varValuePtr = getenv("LD_LIBRARY_PATH");
      
      std::string separator;
      #if (TE_PLATFORM == TE_PLATFORMCODE_FREEBSD) || (TE_PLATFORM == TE_PLATFORMCODE_OPENBSD) || (TE_PLATFORM == TE_PLATFORMCODE_APPLE) || (TE_PLATFORM == TE_PLATFORMCODE_LINUX)
        separator = ":";
      #elif TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        separator = ";";
      #else
        #error "Unsuported plataform for virtual memory checking"
      #endif
      
      if( varValuePtr )
      {
        Tokenize( std::string( varValuePtr ), paths, separator );
      }
}

std::string te::common::FindInTerraLibPath(const std::string& p)
{
// 1st: look in the neighborhood of the executable
  boost::filesystem::path tl_path = boost::filesystem::current_path();
  
  boost::filesystem::path eval_path = tl_path / p;
  
  if(boost::filesystem::exists(eval_path))
    return eval_path.string();
  
  tl_path /= "..";
  
  eval_path = tl_path / p;
  
  if(boost::filesystem::exists(eval_path))
    return eval_path.string();

// 2rd: look for an environment variable defined by macro TERRALIB_DIR_VAR_NAME
  const char* te_env = getenv(TERRALIB_DIR_VAR_NAME);
  
  if(te_env != 0)
  {
    tl_path = te_env;
    
    eval_path = tl_path / p;
    
    if(boost::filesystem::exists(eval_path))
      return eval_path.string();
  }
  
// 3th: look into install prefix-path
  tl_path = TERRALIB_INSTALL_PREFIX_PATH;
  
  eval_path = tl_path / p;
  
  if(boost::filesystem::exists(eval_path))
    return eval_path.string();

// 4nd: look into the codebase path
  tl_path = TERRALIB_CODEBASE_PATH;
  
  eval_path = tl_path / p;
  
  if(boost::filesystem::exists(eval_path))
    return eval_path.string();
  

  return "";
}
