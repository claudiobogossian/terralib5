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
#  Description: Find the Type Library File (msado15.dll).
#
#  ADO_LIBRARY_DIR - where to find the ADO type library (msado15.dll) directory.
#  ADO_FOUND       - True if the ADO type library path is found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#

if(CMAKE_SIZEOF_VOID_P EQUAL 4)

  if(NOT "$ENV{CommonProgramFiles(x86)}")
    set(path_prefix "$ENV{CommonProgramFiles}")
  else()
    set (path_prefix "$ENV{CommonProgramFiles(x86)}")
  endif()

else()
  set (path_prefix "$ENV{CommonProgramFiles}")

endif()

find_path(ADO_LIBRARY_DIR
          NAMES msado15.dll
          PATHS ${path_prefix}"/System/ado")


include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(ADO DEFAULT_MSG ADO_LIBRARY_DIR)

mark_as_advanced(ADO_LIBRARY_DIR)
