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
#  Description: Find Proj4 - find Proj4 include directory and libraries.
#
#  PROJ4_INCLUDE_DIR - where to find proj_api.h.
#  PROJ4_LIBRARY     - where to find proj libraries.
#  PROJ4_FOUND       - True if Proj4 found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#

if(UNIX)

  find_path(PROJ4_INCLUDE_DIR 
            NAMES proj_api.h
            PATHS /usr
                  /usr/local
            PATH_SUFFIXES include)
		  
  find_library(PROJ4_LIBRARY
               NAMES proj
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)
elseif(WIN32)

  find_path(PROJ4_INCLUDE_DIR
            NAMES proj_api.h
            PATH_SUFFIXES proj4)
		  
  find_library(PROJ4_LIBRARY_RELEASE
               NAMES proj_i proj)

  find_library(PROJ4_LIBRARY_DEBUG
               NAMES proj_i_d proj_d proj_i)

  if(PROJ4_LIBRARY_RELEASE AND PROJ4_LIBRARY_DEBUG)
    set(PROJ4_LIBRARY optimized ${PROJ4_LIBRARY_RELEASE} debug ${PROJ4_LIBRARY_DEBUG})
  elseif(PROJ4_LIBRARY_RELEASE)
    set(PROJ4_LIBRARY optimized ${PROJ4_LIBRARY_RELEASE} debug ${PROJ4_LIBRARY_RELEASE})
  elseif(PROJ4_LIBRARY_DEBUG)
    set(PROJ4_LIBRARY optimized ${PROJ4_LIBRARY_DEBUG} debug ${PROJ4_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Proj4 DEFAULT_MSG PROJ4_LIBRARY PROJ4_INCLUDE_DIR)

mark_as_advanced(PROJ4_INCLUDE_DIR PROJ4_LIBRARY)
