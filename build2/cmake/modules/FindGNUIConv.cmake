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
#  Description: Find GNU IConv - find the GNU IConv include directory and libraries.
#
#  ICONV_INCLUDE_DIR - where to find .
#  ICONV_LIBRARY     - where to find the GNU iconv library.
#  ICONV_FOUND       - True if GNU iconv is  found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan P. Garrido <juan@dpi.inpe.br>
#

find_path(ICONV_INCLUDE_DIR intl.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include)

if(UNIX)

  find_library(ICONV_LIBRARY
               NAMES iconv
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)
elseif(WIN32)

  find_library(ICONV_LIBRARY_RELEASE
               NAMES iconv
               PATH_SUFFIXES lib)

  find_library(ICONV_LIBRARY_DEBUG
               NAMES iconv_d
               PATH_SUFFIXES lib)

  if(ICONV_LIBRARY_RELEASE AND ICONV_LIBRARY_DEBUG)
    set(ICONV_LIBRARY optimized ${ICONV_LIBRARY_RELEASE} debug ${ICONV_LIBRARY_DEBUG})
  elseif(ICONV_LIBRARY_RELEASE)
    set(ICONV_LIBRARY optimized ${ICONV_LIBRARY_RELEASE} debug ${ICONV_LIBRARY_RELEASE})
  elseif(ICONV_LIBRARY_DEBUG)
    set(ICONV_LIBRARY optimized ${ICONV_LIBRARY_DEBUG} debug ${ICONV_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(ICONV DEFAULT_MSG ICONV_LIBRARY ICONV_INCLUDE_DIR)

mark_as_advanced(ICONV_INCLUDE_DIR ICONV_LIBRARY)
