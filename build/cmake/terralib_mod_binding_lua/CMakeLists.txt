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
#  Description: CMake script for the TerraLib Java binding.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

include(${SWIG_USE_FILE})

if(WIN32)
  add_definitions(-DLUA_BUILD_AS_DLL)
endif()

file(GLOB TERRALIB_SWIG_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/terralib/binding/swig/TerraLibLUA.i)

set (CMAKE_SWIG_FLAGS "-c++")

set_source_files_properties(${TERRALIB_SWIG_SCRIPT_FILE} PROPERTIES CPLUSPLUS ON CMAKE_SWIG_FLAGS "-includeall")

include_directories (
  "${TERRALIB_ABSOLUTE_ROOT_DIR}/src"
  "${CMAKE_BINARY_DIR}"
  "${Boost_INCLUDE_DIR}"
  "${LUA_INCLUDE_DIR}"
)

set (CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/src" CACHE PATH "Directory for swig output.")

SWIG_ADD_MODULE(terralib_mod_binding_lua lua ${TERRALIB_SWIG_SCRIPT_FILE})

SWIG_LINK_LIBRARIES (terralib_mod_binding_lua terralib_mod_common
                                              terralib_mod_datatype
                                              terralib_mod_srs
                                              terralib_mod_geometry
                                              terralib_mod_raster
                                              terralib_mod_dataaccess
                                              terralib_mod_plugin
                                              terralib_mod_st
											  terralib_mod_cellspace
											  terralib_mod_maptools
											  terralib_mod_qt_apf
                                              ${Boost_FILESYSTEM_LIBRARY}
                                              ${Boost_SYSTEM_LIBRARY}
                                              ${Boost_THREAD_LIBRARY}
                                              ${LUA_LIBRARIES})

install(TARGETS terralib_mod_binding_lua
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime)
