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
#  TERRALIB4_INCLUDE_DIR  - where to find Terralib4 include files.
#  TERRALIB4_LIBRARIES    - where to find Terralib4 libraries.
#  TERRALIB4_FOUND        - True, if Terralib4 is found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#

find_path(TERRALIB4_INCLUDE_DIR 
          NAMES kernel/TeDefines.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include
                        terralib4
                        include/terralib4)
                        
if(TERRALIB4_INCLUDE_DIR)
  set(TERRALIB4_INCLUDE_DIR ${TERRALIB4_INCLUDE_DIR}
                            ${TERRALIB4_INCLUDE_DIR}/kernel CACHE STRING "Path to include dir" FORCE)
endif()

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

  set(TERRALIB_LIBRARIES_RELEASE ${TERRALIB4_LIBRARY_RELEASE} ${TERRALIB4_ADO_LIBRARY_RELEASE})

  set(TERRALIB_LIBRARIES_DEBUG ${TERRALIB4_LIBRARY_DEBUG} ${TERRALIB4_ADO_LIBRARY_DEBUG})

  if(TERRALIB_LIBRARIES_RELEASE AND TERRALIB_LIBRARIES_DEBUG)
    set(TERRALIB4_LIBRARIES optimized ${TERRALIB_LIBRARIES_RELEASE} debug ${TERRALIB_LIBRARIES_DEBUG})
  elseif(TERRALIB_LIBRARIES_RELEASE)
    set(TERRALIB4_LIBRARIES optimized ${TERRALIB_LIBRARIES_RELEASE} debug ${TERRALIB_LIBRARIES_RELEASE})
  elseif(TERRALIB_LIBRARIES_DEBUG)
    set(TERRALIB4_LIBRARIES optimized ${TERRALIB_LIBRARIES_DEBUG} debug ${TERRALIB_LIBRARIES_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Terralib4 DEFAULT_MSG TERRALIB4_LIBRARIES TERRALIB4_INCLUDE_DIR)

mark_as_advanced(TERRALIB4_INCLUDE_DIR TERRALIB4_LIBRARIES)
