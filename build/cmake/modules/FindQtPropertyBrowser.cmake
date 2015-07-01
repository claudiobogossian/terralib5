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
#  Description: Find Qt Property Browser - find Qt Property Browser include directory and libraries.
#
#  QTPROPERTYBROWSER_INCLUDE_DIR - where to find qtpropertybrowser.h.
#  QTPROPERTYBROWSER_LIBRARY     - where to find QtPropertyBrowser libraries.
#  QTPROPERTYBROWSER_FOUND       - True if QtPropertyBrowser found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#

if(UNIX)

  find_path(QTPROPERTYBROWSER_INCLUDE_DIR
            NAMES QtPropertyBrowser/qtpropertybrowser.h
            PATHS /usr
                  /usr/local
                  /usr/local/qtpropertybrowser
            PATH_SUFFIXES include)

find_library(QTPROPERTYBROWSER_LIBRARY
             NAMES qt_property_browser
             PATHS /usr
                   /usr/local
                   /usr/local/qtpropertybrowser
             PATH_SUFFIXES lib)

elseif(WIN32)

  find_path(QTPROPERTYBROWSER_INCLUDE_DIR
            NAMES QtPropertyBrowser/qtpropertybrowser.h
            PATH_SUFFIXES include)

  find_library(QTPROPERTYBROWSER_LIBRARY_RELEASE
               NAMES qt_property_browser
               PATH_SUFFIXES lib)

  find_library(QTPROPERTYBROWSER_LIBRARY_DEBUG
               NAMES qt_property_browserd
               PATH_SUFFIXES lib)

  if(QTPROPERTYBROWSER_LIBRARY_RELEASE AND QTPROPERTYBROWSER_LIBRARY_DEBUG)
    set(QTPROPERTYBROWSER_LIBRARY optimized ${QTPROPERTYBROWSER_LIBRARY_RELEASE} debug ${QTPROPERTYBROWSER_LIBRARY_DEBUG})
  elseif(QTPROPERTYBROWSER_LIBRARY_RELEASE)
    set(QTPROPERTYBROWSER_LIBRARY optimized ${QTPROPERTYBROWSER_LIBRARY_RELEASE} debug ${QTPROPERTYBROWSER_LIBRARY_RELEASE})
  elseif(PROJ4_LIBRARY_DEBUG)
    set(QTPROPERTYBROWSER_LIBRARY optimized ${QTPROPERTYBROWSER_LIBRARY_DEBUG} debug ${QTPROPERTYBROWSER_LIBRARY_DEBUG})
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(QtPropertyBrowser DEFAULT_MSG QTPROPERTYBROWSER_INCLUDE_DIR QTPROPERTYBROWSER_LIBRARY)

mark_as_advanced(QTPROPERTYBROWSER_INCLUDE_DIR QTPROPERTYBROWSER_LIBRARY)
