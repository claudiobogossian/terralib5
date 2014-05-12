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
#  Description: Find GEOS (Geometry Engine Open Source) - find GEOS include directory and library.
#
#  GEOS_INCLUDE_DIR - where to find geos.h.
#  GEOS_LIBRARY     - where to find geos library.
#  GEOS_FOUND        - True if GEOS found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#

if(UNIX)

  find_path(GEOS_INCLUDE_DIR
            NAMES geos.h
			      PATHS /usr
			            /usr/local
			      PATH_SUFFIXES include)
			  
  find_library(GEOS_LIBRARY
               NAMES geos
               PATHS /usr
               /usr/lib
               PATH_SUFFIXES lib)

elseif(WIN32)

  find_path(GEOS_INCLUDE_DIR
            NAMES geos.h
            PATH_SUFFIXES geos)
            
  find_library(GEOS_LIBRARY_RELEASE 
               NAMES geos_i geos)

  find_library(GEOS_LIBRARY_DEBUG
               NAMES geos_i_d geos_d geosd)

  if(GEOS_LIBRARY_RELEASE AND GEOS_LIBRARY_DEBUG)
    set(GEOS_LIBRARY optimized ${GEOS_LIBRARY_RELEASE} debug ${GEOS_LIBRARY_DEBUG})
  elseif(GEOS_LIBRARY_RELEASE)
    set(GEOS_LIBRARY optimized ${GEOS_LIBRARY_RELEASE} debug ${GEOS_LIBRARY_RELEASE})
  elseif(GEOS_LIBRARY_DEBUG)
    set(GEOS_LIBRARY optimized ${GEOS_LIBRARY_DEBUG} debug ${GEOS_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GEOS DEFAULT_MSG GEOS_LIBRARY GEOS_INCLUDE_DIR)

mark_as_advanced(GEOS_INCLUDE_DIR GEOS_LIBRARY)
