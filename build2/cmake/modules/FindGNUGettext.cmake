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
#  GNUGETTEXT_INCLUDE_DIR - where to find libintl.h.
#  GNUGETTEXT_LIBRARY     - where to find the GNU gettext library.
#  GNUGETTEXT_FOUND       - True if the GNU Gettext is found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan P. Garrido <juan@dpi.inpe.br>
#

if(UNIX)

  find_path(GNUGETTEXT_INCLUDE_DIR libintl.h
            PATHS /usr
                  /usr/local
            PATH_SUFFIXES include)

  find_library(GNUGETTEXT_LIBRARY
               NAMES gettextlib
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)
elseif(WIN32)

  find_path(GNUGETTEXT_INCLUDE_DIR
            NAMES libintl.h
            PATH_SUFFIXES include)

  find_library(GNUGETTEXT_LIBRARY_RELEASE
               NAMES gettextlib gettextpo
               PATH_SUFFIXES lib)

  find_library(GNUGETTEXT_LIBRARY_DEBUG
               NAMES gettextlib_d gettextlibd gettextpo_d gettextpod gettextpo
               PATH_SUFFIXES lib)

  if(GNUGETTEXT_LIBRARY_RELEASE AND GNUGETTEXT_LIBRARY_DEBUG)
    set(GNUGETTEXT_LIBRARY optimized ${GNUGETTEXT_LIBRARY_RELEASE} debug ${GNUGETTEXT_LIBRARY_DEBUG})
  elseif(GNUGETTEXT_LIBRARY_RELEASE)
    set(GNUGETTEXT_LIBRARY optimized ${GNUGETTEXT_LIBRARY_RELEASE} debug ${GNUGETTEXT_LIBRARY_RELEASE})
  elseif(GNUGETTEXT_LIBRARY_DEBUG)
    set(GNUGETTEXT_LIBRARY optimized ${GNUGETTEXT_LIBRARY_DEBUG} debug ${GNUGETTEXT_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GNUGETTEXT DEFAULT_MSG GNUGETTEXT_LIBRARY GNUGETTEXT_INCLUDE_DIR)

mark_as_advanced(GNUGETTEXT_INCLUDE_DIR GNUGETTEXT_LIBRARY)
