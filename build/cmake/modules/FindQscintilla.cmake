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
#  Description: Find Qscintilla
#
#  Qscintilla_INCLUDE_DIR - where to find Qsci/qsciscintilla.h.
#  Qscintilla_LIBRARY     - where to find qscintilla library.
#  Qscintilla_FOUND       - True if Qscintilla found.
#
#  Author: Frederico Bedê <frederico.bede@funcate.org.br>
#

find_path(Qscintilla_INCLUDE_DIR 
          NAMES Qsci/qsciscintilla.h
)

if(UNIX)
  find_library(Qscintilla_LIBRARY
               NAMES qscintilla2
)
elseif(WIN32)
  find_library(Qscintilla_LIBRARY_DEBUG
               NAMES qscintilla2d)

  find_library(Qscintilla_LIBRARY_RELEASE
               NAMES qscintilla2)

  if(Qscintilla_LIBRARY_RELEASE AND Qscintilla_LIBRARY_DEBUG)
    set(Qscintilla_LIBRARY "optimized;${Qscintilla_LIBRARY_RELEASE};debug;${Qscintilla_LIBRARY_DEBUG}")
  elseif(Qscintilla_LIBRARY_RELEASE)
    set(Qscintilla_LIBRARY "optimized;${Qscintilla_LIBRARY_RELEASE};debug;${Qscintilla_LIBRARY_RELEASE}")
  elseif(Qscintilla_LIBRARY_DEBUG)
    set(Qscintilla_LIBRARY "optimized;${Qscintilla_LIBRARY_DEBUG};debug;${Qscintilla_LIBRARY_DEBUG}")
  endif()

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Qscintilla DEFAULT_MSG Qscintilla_LIBRARY Qscintilla_INCLUDE_DIR)

mark_as_advanced(Qscintilla_INCLUDE_DIR Qscintilla_LIBRARY)
