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
#  Description: Find Log4cxx - find Log4cxx include directory and library.
#
#  LOG4CXX_INCLUDE_DIR - where to find log4cxx/log4cxx.h.
#  LOG4CXX_LIBRARY     - where to find the log4cxx library.
#  LOG4CXX_FOUND       - True if Log4cxx is found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan P. Garrido <juan@dpi.inpe.br>
#

if(UNIX)

  find_path(LOG4CXX_INCLUDE_DIR
            NAMES log4cxx/log4cxx.h
            PATHS /usr
                  /usr/local
            PATH_SUFFIXES include)

  find_library(LOG4CXX_LIBRARY
               NAMES log4cxx
               PATHS /usr
                     /usr/lib
               PATH_SUFFIXES lib)

elseif(WIN32)

  find_path(LOG4CXX_INCLUDE_DIR
            NAMES log4cxx/log4cxx.h
            PATH_SUFFIXES include)

  find_library(LOG4CXX_LIBRARY_RELEASE
               NAMES log4cxx
               PATH_SUFFIXES lib)

  find_library(LOG4CXX_LIBRARY_DEBUG
               NAMES log4cxx_d
               PATH_SUFFIXES lib)

  if(LOG4CXX_LIBRARY_RELEASE AND LOG4CXX_LIBRARY_DEBUG)
    set(LOG4CXX_LIBRARY optimized ${LOG4CXX_LIBRARY_RELEASE} debug ${LOG4CXX_LIBRARY_DEBUG})
  elseif(LOG4CXX_LIBRARY_RELEASE)
    set(LOG4CXX_LIBRARY optimized ${LOG4CXX_LIBRARY_RELEASE} debug ${LOG4CXX_LIBRARY_RELEASE})
  elseif(LOG4CXX_LIBRARY_DEBUG)
    set(LOG4CXX_LIBRARY optimized ${LOG4CXX_LIBRARY_DEBUG} debug ${LOG4CXX_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LOG4CXX DEFAULT_MSG LOG4CXX_LIBRARY LOG4CXX_INCLUDE_DIR)

mark_as_advanced(LOG4CXX_INCLUDE_DIR LOG4CXX_LIBRARY)
