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
#  Description: Find PostgreSQL libpq include directory and library.
#
#  PostgreSQL_INCLUDE_DIRS - where to find libpq-fe.h.
#  PostgreSQL_LIBRARIES    - where to find pq library.
#  LIBPQ_FOUND             - True if libpq found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#

if(UNIX)

  find_path(PostgreSQL_INCLUDE_DIRS
            NAMES libpq-fe.h
            PATHS /usr/local/pgsql
                  /usr/local
                  /usr
            PATH_SUFFIXES include)

  find_library(PostgreSQL_LIBRARIES
               NAMES pq
               PATHS /usr/local/pgsql
                     /usr/local
                     /usr
               PATH_SUFFIXES lib)

elseif(WIN32)

  find_path(PostgreSQL_INCLUDE_DIRS
            NAMES libpq-fe.h
            PATH_SUFFIXES include)

  find_library(PostgreSQL_LIBRARY_RELEASE 
               NAMES libpqdll libpq pq
               PATH_SUFFIXES lib)

  find_library(PostgreSQL_LIBRARY_DEBUG
               NAMES libpqddll libpqd pqd
               PATH_SUFFIXES lib)

  if(PostgreSQL_LIBRARY_RELEASE AND PostgreSQL_LIBRARY_DEBUG)
    set(PostgreSQL_LIBRARIES optimized ${PostgreSQL_LIBRARY_RELEASE} debug ${PostgreSQL_LIBRARY_DEBUG})
  elseif(PostgreSQL_LIBRARY_RELEASE)
    set(PostgreSQL_LIBRARIES optimized ${PostgreSQL_LIBRARY_RELEASE} debug ${PostgreSQL_LIBRARY_RELEASE})
  elseif(PostgreSQL_LIBRARY_DEBUG)
    set(PostgreSQL_LIBRARIES optimized ${PostgreSQL_LIBRARY_DEBUG} debug ${PostgreSQL_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBPQ DEFAULT_MSG PostgreSQL_LIBRARIES PostgreSQL_INCLUDE_DIRS)

mark_as_advanced(PostgreSQL_INCLUDE_DIRS PostgreSQL_LIBRARIES)
