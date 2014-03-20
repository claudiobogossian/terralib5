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
#  Description: Find GEOS (Geometry Engine Open Source) C Library - find GEOS C include directory and library.
#
#  GEOSC_INCLUDE_DIR - where to find geos_c.h.
#  GEOSC_LIBRARY     - where to find geos_c library.
#  GEOSC_FOUND        - True if GEOSC found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan P. Garrido <juan@dpi.inpe.br>
#

find_path(GEOSC_INCLUDE_DIR NAMES geos_c.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include)

if(UNIX)

  find_library(GEOSC_LIBRARY
               NAMES geos_c
               PATHS /usr
                     /usr/lib
               PATH_SUFFIXES lib)

elseif(WIN32)

  find_library(GEOSC_LIBRARY_RELEASE
               NAMES geos_c_i geosc_i geos_c geosc
               PATH_SUFFIXES lib)

  find_library(GEOSC_LIBRARY_DEBUG
               NAMES geos_c_i_d geosc_i_d geosc_id geos_c_d geosc_d geoscd
               PATH_SUFFIXES lib)

  if(GEOSC_LIBRARY_RELEASE AND GEOSC_LIBRARY_DEBUG)
    set(GEOSC_LIBRARY optimized ${GEOSC_LIBRARY_RELEASE} debug ${GEOSC_LIBRARY_DEBUG})
  elseif(GEOSC_LIBRARY_RELEASE)
    set(GEOSC_LIBRARY optimized ${GEOSC_LIBRARY_RELEASE} debug ${GEOSC_LIBRARY_RELEASE})
  elseif(GEOSC_LIBRARY_DEBUG)
    set(GEOSC_LIBRARY optimized ${GEOSC_LIBRARY_DEBUG} debug ${GEOSC_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GEOSC DEFAULT_MSG GEOSC_LIBRARY GEOSC_INCLUDE_DIR)

mark_as_advanced(GEOSC_INCLUDE_DIR GEOSC_LIBRARY)
