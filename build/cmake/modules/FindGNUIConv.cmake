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
#  GNUICONV_INCLUDE_DIR - where to find iconv.h.
#  GNUICONV_LIBRARY     - where to find the GNU Iconv library.
#  GNUICONV_FOUND       - True if GNU Iconv is  found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan P. Garrido <juan@dpi.inpe.br>
#

if(UNIX)

  find_path(GNUICONV_INCLUDE_DIR
            NAMES iconv.h
            PATHS /usr
                  /usr/local
            PATH_SUFFIXES include)

  find_library(GNUICONV_LIBRARY
               NAMES iconv
               PATHS /usr
                     /usr/local
               PATH_SUFFIXES lib)
elseif(WIN32)

  find_path(GNUICONV_INCLUDE_DIR
            NAMES iconv.h
            PATH_SUFFIXES include)

  find_library(GNUICONV_LIBRARY_RELEASE
               NAMES iconv
               PATH_SUFFIXES lib)

  find_library(GNUICONV_LIBRARY_DEBUG
               NAMES iconv_d iconvd
               PATH_SUFFIXES lib)

  if(GNUICONV_LIBRARY_RELEASE AND GNUICONV_LIBRARY_DEBUG)
    set(GNUICONV_LIBRARY optimized ${GNUICONV_LIBRARY_RELEASE} debug ${GNUICONV_LIBRARY_DEBUG})
  elseif(GNUICONV_LIBRARY_RELEASE)
    set(GNUICONV_LIBRARY optimized ${GNUICONV_LIBRARY_RELEASE} debug ${GNUICONV_LIBRARY_RELEASE})
  elseif(GNUICONV_LIBRARY_DEBUG)
    set(GNUICONV_LIBRARY optimized ${GNUICONV_LIBRARY_DEBUG} debug ${GNUICONV_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GNUICONV DEFAULT_MSG GNUICONV_LIBRARY GNUICONV_INCLUDE_DIR)

mark_as_advanced(GNUICONV_INCLUDE_DIR GNUICONV_LIBRARY)
