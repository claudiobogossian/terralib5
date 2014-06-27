#
#  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of the TerraLib - a Framework for building GIS enabled applications.
#
#  TerraLib is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  TerraLib is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with TerraLib. See COPYING. If not, write to
#  TerraLib Team at <terralib-team@terralib.org>.
#
#
#  Description: Find Terralib4 - find Terralib4 include directory and libraries.
#
#  TERRALIB4_INCLUDE_DIRS        - where to find the Terralib4 include files.
#  TERRALIB4_KERNEL_INCLUDE_DIR  - where to find Terralib4 kernel include files.
#  TERRALIB4_ADO_INCLUDE_DIR     - where to find Terralib4 ado include files.
#  TERRALIB4_UTILS_INCLUDE_DIR   - where to find Terralib4 utils include files.
#  TERRALIB4_LIBRARIES           - where to find Terralib4 libraries.
#  TERRALIB4_FOUND               - True, if Terralib4 is found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#

find_path(TERRALIB4_KERNEL_INCLUDE_DIR 
          NAMES TeDefines.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include
                        terralib4
                        include/terralib4/kernel)

find_path(TERRALIB4_ADO_INCLUDE_DIR 
          NAMES TeAdoDB.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include
                        terralib4
                        include/terralib4/drivers/ado)

find_path(TERRALIB4_UTILS_INCLUDE_DIR 
          NAMES TeUtilsDefines.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include
                        terralib4
                        include/terralib4/utils)

set(TERRALIB4_INCLUDE_DIRS ${TERRALIB4_KERNEL_INCLUDE_DIR}
                           ${TERRALIB4_ADO_INCLUDE_DIR}
                           ${TERRALIB4_UTILS_INCLUDE_DIR})

if(WIN32)
  find_library(TERRALIB4_LIBRARY_RELEASE
               NAMES terralib4 terralib
               PATH_SUFFIXES lib)

  find_library(TERRALIB4_ADO_LIBRARY_RELEASE
               NAMES terralib4_ado terralib_ado
               PATH_SUFFIXES lib)

  find_library(TERRALIB4_LIBRARY_DEBUG
               NAMES terralib4d terralibd terralib_d
               PATH_SUFFIXES lib)

  find_library(TERRALIB4_ADO_LIBRARY_DEBUG
               NAMES terralib4_adod terralib4_ado_d terralib_adod terralib_ado_d
               PATH_SUFFIXES lib)

  if(TERRALIB4_LIBRARY_RELEASE AND TERRALIB4_LIBRARY_DEBUG)
    set(TERRALIB_LIBRARY optimized ${TERRALIB4_LIBRARY_RELEASE} debug ${TERRALIB4_LIBRARY_DEBUG})
  elseif(TERRALIB4_LIBRARY_RELEASE)
    set(TERRALIB_LIBRARY optimized ${TERRALIB4_LIBRARY_RELEASE} debug ${TERRALIB4_LIBRARY_RELEASE})
  elseif(TERRALIB4_LIBRARY_DEBUG)
    set(TERRALIB_LIBRARY optimized ${TERRALIB4_LIBRARY_DEBUG} debug ${TERRALIB4_LIBRARY_DEBUG})
  endif()
  
  if(TERRALIB4_ADO_LIBRARY_RELEASE AND TERRALIB4_ADO_LIBRARY_DEBUG)
    set(TERRALIB_ADO_LIBRARY optimized ${TERRALIB4_ADO_LIBRARY_RELEASE} debug ${TERRALIB4_ADO_LIBRARY_DEBUG})
  elseif(TERRALIB4_ADO_LIBRARY_RELEASE)
    set(TERRALIB_ADO_LIBRARY optimized ${TERRALIB4_ADO_LIBRARY_RELEASE} debug ${TERRALIB4_ADO_LIBRARY_RELEASE})
  elseif(TERRALIB4_ADO_LIBRARY_DEBUG)
    set(TERRALIB_ADO_LIBRARY optimized ${TERRALIB4_ADO_LIBRARY_DEBUG} debug ${TERRALIB4_ADO_LIBRARY_DEBUG})
  endif()

  set(TERRALIB4_LIBRARIES ${TERRALIB_LIBRARY} ${TERRALIB_ADO_LIBRARY})

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Terralib4 DEFAULT_MSG TERRALIB4_LIBRARIES TERRALIB4_INCLUDE_DIRS)

mark_as_advanced(TERRALIB4_INCLUDE_DIRS TERRALIB4_KERNEL_INCLUDE_DIR TERRALIB4_ADO_INCLUDE_DIR TERRALIB4_UTILS_INCLUDE_DIR TERRALIB4_LIBRARIES)
