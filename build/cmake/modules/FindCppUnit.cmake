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
#  Description: Find Terralib4 - find Terralib4 include directory and libraries.
#
#  CPPUNIT_INCLUDE_DIR - The CppUnit include directory.
#  CPPUNIT_LIBRARY  -  - The CppUnit library.
#  CPPUNIT_FOUND       - True, if CppUnit is found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#

if(UNIX)

  find_path(CPPUNIT_INCLUDE_DIR Test.h
            PATHS /usr
                  /usr/local
                  /usr/local/cppunit
            PATH_SUFFIXES include
                          cppunit
                          include/cppunit)

  find_library(CPPUNIT_LIBRARY
               NAMES cppunit
               PATHS /usr
                     /usr/local
                     /usr/local/cppunit
               PATH_SUFFIXES lib)

elseif(WIN32)

  find_path(CPPUNIT_INCLUDE_DIR Test.h
            PATH_SUFFIXES include
                          cppunit
                          include/cppunit)

  find_library(CPPUNIT_LIBRARY_RELEASE
               NAMES cppunit_dll cppunit
               PATH_SUFFIXES lib)
 
  find_library(CPPUNIT_LIBRARY_DEBUG
               NAMES cppunitd_dll cppunit
               PATH_SUFFIXES lib)
 
  if(CPPUNIT_LIBRARY_RELEASE AND CPPUNIT_LIBRARY_DEBUG)
    set(CPPUNIT_LIBRARY optimized ${CPPUNIT_LIBRARY_RELEASE} debug ${CPPUNIT_LIBRARY_DEBUG})
  elseif(CPPUNIT_LIBRARY_RELEASE)
    set(CPPUNIT_LIBRARY optimized ${CPPUNIT_LIBRARY_RELEASE} debug ${CPPUNIT_LIBRARY_RELEASE})
  elseif(CPPUNIT_LIBRARY_DEBUG)
    set(CPPUNIT_LIBRARY optimized ${CPPUNIT_LIBRARY_DEBUG} debug ${CPPUNIT_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(CppUnit DEFAULT_MSG CPPUNIT_LIBRARY CPPUNIT_INCLUDE_DIR)

mark_as_advanced(CPPUNIT_INCLUDE_DIR CPPUNIT_LIBRARY)
