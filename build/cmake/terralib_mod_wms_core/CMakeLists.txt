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
#  Description: Build configuration for the Web Map Service(WMS) Core Module.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})
include_directories(${GDAL_INCLUDE_DIR})

if(WIN32)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -DTEWMSDLL)
endif()

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/wms/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/wms/*.h)

set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES})

add_library(terralib_mod_wms_core SHARED ${TERRALIB_FILES})

target_link_libraries(terralib_mod_wms_core terralib_mod_common
                                       terralib_mod_dataaccess
                                       terralib_mod_gdal
                                       terralib_mod_maptools)

set_target_properties(terralib_mod_wms_core
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 INSTALL_NAME_DIR "@executable_path/../lib")

install(TARGETS terralib_mod_wms_core
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/wms COMPONENT devel)

export(TARGETS terralib_mod_wms_core APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
