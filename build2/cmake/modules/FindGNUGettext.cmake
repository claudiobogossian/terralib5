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
#  Description: Find GNU Gettext - find the GNU Gettext include directory and libraries.
#
#  GETTEXT_INCLUDE_DIR - where to find proj_api.h.
#  GETTEXT_LIBRARY     - where to find proj libraries.
#  GETTEXT_FOUND       - True if GNU Gettext found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan P. Garrido <juan@dpi.inpe.br>
#

find_path(GETTEXT_INCLUDE_DIR proj_api.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include)

if(UNIX)

  find_library(GETTEXT_LIBRARY
               NAMES proj
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)
elseif(WIN32)

  find_library(GETTEXT_LIBRARY_RELEASE
               NAMES proj_i proj
               PATH_SUFFIXES lib)

  find_library(GETTEXT_LIBRARY_DEBUG
               NAMES proj_i_d proj_d
               PATH_SUFFIXES lib)

  if(GETTEXT_LIBRARY_RELEASE AND GETTEXT_LIBRARY_DEBUG)
    set(GETTEXT_LIBRARY optimized ${GETTEXT_LIBRARY_RELEASE} debug ${GETTEXT_LIBRARY_DEBUG})
  elseif(GETTEXT_LIBRARY_RELEASE)
    set(GETTEXT_LIBRARY optimized ${GETTEXT_LIBRARY_RELEASE} debug ${GETTEXT_LIBRARY_RELEASE})
  elseif(GETTEXT_LIBRARY_DEBUG)
    set(GETTEXT_LIBRARY optimized ${GETTEXT_LIBRARY_DEBUG} debug ${GETTEXT_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Gettext DEFAULT_MSG GETTEXT_LIBRARY GETTEXT_INCLUDE_DIR)

mark_as_advanced(GETTEXT_INCLUDE_DIR GETTEXT_LIBRARY)
