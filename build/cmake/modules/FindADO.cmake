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
#  Description: Find the ADO Type Library Files.
#
#  ADO_LIBRARY_DIRS - where to find the ADO type library (msado15.dll) directory.
#  ADO_FOUND     -    True, if the ADO type library path is found.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#


# Get the architecture

set(arch "x86")

if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
  set(arch "x64")
elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
  set(arch "x64")
elseif("$ENV{LIB}" MATCHES "(amd64|ia64)")
  set(arch "x64")
endif()

set(_PREFIX_ENV_VAR "ProgramFiles")

if ("$ENV{ProgramW6432}" STREQUAL "$ENV{ProgramFiles}")
  if(NOT "${arch}" STREQUAL "x64")
    set(_PREFIX_ENV_VAR "ProgramFiles(x86)")
  endif()
else()
  if("${arch}" STREQUAL "x64")
    set(_PREFIX_ENV_VAR "ProgramW6432")
  endif()
endif()

if(NOT "$ENV{${_PREFIX_ENV_VAR}}" STREQUAL "")
  file(TO_CMAKE_PATH "$ENV{${_PREFIX_ENV_VAR}}" path_prefix)
elseif(NOT "$ENV{SystemDrive}" STREQUAL "")
  set(path_prefix "$ENV{SystemDrive}/Program Files")
else()
  set(path_prefix "C:/Program Files")
endif()

find_path(ADO_TYPE_LIBRARY_DIR
          NAMES msado15.dll msado26.tlb msadox.dll
          PATHS ${path_prefix}"/Common Files/System/ado")

find_path(ADO_OLEDB_DIR
          NAMES oledb32.dll
          PATHS ${path_prefix}"/Common Files/System/Ole DB")

set(ADO_LIBRARY_DIRS ${ADO_TYPE_LIBRARY_DIR} ${ADO_OLEDB_DIR})

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(ADO DEFAULT_MSG ADO_LIBRARY_DIRS)

mark_as_advanced(ADO_LIBRARY_DIRS)
