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
#  Description: Find Apache Xerces-c - find Apache Xerces-c include directory and libraries.
#
#  XERCES_INCLUDE_DIR - where to find xercesc/util/XercesVersion.hpp.
#  XERCES_LIBRARY     - where to find Xerces-C libraries.
#  XERCES_FOUND       - True if Xerces-c found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#

if(UNIX)

find_path(XERCES_INCLUDE_DIR
          NAMES xercesc/util/XercesVersion.hpp
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include)

  find_library(XERCES_LIBRARY
               NAMES xerces-c
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)

elseif(WIN32)

find_path(XERCES_INCLUDE_DIR
          NAMES xercesc/util/XercesVersion.hpp
          PATH_SUFFIXES xerces)

  find_library(XERCES_LIBRARY_RELEASE
               NAMES xerces-c xerces-c_3)

  find_library(XERCES_LIBRARY_DEBUG
               NAMES xerces-cD xerces-c_3D)

  if(XERCES_LIBRARY_RELEASE AND XERCES_LIBRARY_DEBUG)
    set(XERCES_LIBRARY optimized ${XERCES_LIBRARY_RELEASE} debug ${XERCES_LIBRARY_DEBUG})
  elseif(XERCES_LIBRARY_RELEASE)
    set(XERCES_LIBRARY optimized ${XERCES_LIBRARY_RELEASE} debug ${XERCES_LIBRARY_RELEASE})
  elseif(XERCES_LIBRARY_DEBUG)
    set(XERCES_LIBRARY optimized ${XERCES_LIBRARY_DEBUG} debug ${XERCES_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Xerces DEFAULT_MSG XERCES_LIBRARY XERCES_INCLUDE_DIR)

mark_as_advanced(XERCES_INCLUDE_DIR XERCES_LIBRARY)
