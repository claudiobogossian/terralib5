/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/PlatformUtils.h

  \brief This file is a wrapper around platform specific include files.

  \ingroup common
*/

#ifndef __TERRALIB_COMMON_INTERNAL_PLATFORMUTILS_H
#define __TERRALIB_COMMON_INTERNAL_PLATFORMUTILS_H

// TerraLib
#include "Config.h"

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#include "WinUtils.h"

#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
#include "LinuxUtils.h"

#else
  #error "Platform not supported yet! Please contact terralib-team@dpi.inpe.br"
#endif

// STL
#include <vector>
#include <string>

namespace te
{
  namespace common
  {
    /*!
      \brief Returns the amount of free physical memory (bytes).

      \return The amount of free physical memory (bytes).
    */
    TECOMMONEXPORT unsigned long int GetFreePhysicalMemory();

    /*!
      \brief Returns the amount of total physical memory (bytes).

      \return The amount of total physical memory (bytes).
    */
    TECOMMONEXPORT unsigned long int GetTotalPhysicalMemory();

    /*!
      \brief Returns the amount of used virtual memory (bytes) for the current process (physical + swapped).

      \return The amount of free virtual memory (bytes).
    */
    TECOMMONEXPORT unsigned long int GetUsedVirtualMemory();

    /*!
      \brief Returns the amount of total virtual memory (bytes) that can be claimed by the current process (physical + swapped).

      \return The amount of total virtual memory (bytes).
    */
    TECOMMONEXPORT unsigned long int GetTotalVirtualMemory();

    /*!
      \brief Returns the number of physical processors.

      \return The number of physical processors.
    */
    TECOMMONEXPORT unsigned int GetPhysProcNumber();
    
    /*!
      \brief Returns the system PATH enviroment variable, decomposed into directory names, or an empty vector if no values were found.

      \param paths The decomposed PATH.
    */
    TECOMMONEXPORT void GetDecompostedPathEnvVar( std::vector< std::string >& paths );    
    
    /*!
      \brief Returns the system LD_LIBRARY_PATH enviroment variable, decomposed into directory names, or an empty vector if no values were found.

      \param paths The decomposed PATH.
    */
    TECOMMONEXPORT void GetDecompostedLDPathEnvVar( std::vector< std::string >& paths );
    
    TECOMMONEXPORT std::string GetTerraLibDir();

  }     // end namespace common
}       // end namespace te

#endif  //__TERRALIB_COMMON_INTERNAL_PLATFORMUTILS_H
