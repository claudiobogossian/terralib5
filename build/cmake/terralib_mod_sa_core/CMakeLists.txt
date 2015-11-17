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
#  Description: Build configuration for the Vector Processing Core Module.
#
#  Author: Eric Silva Abreu <eric@dpi.inpe.br>
#

include_directories(${Boost_INCLUDE_DIR})

if(WIN32)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -DTESADLL)
endif()

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/sa/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/sa/*.h)

source_group("Source Files"  FILES ${TERRALIB_SRC_FILES})
source_group("Header Files"  FILES ${TERRALIB_HDR_FILES})

file(GLOB TERRALIB_CORE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/sa/core/*.cpp)
file(GLOB TERRALIB_CORE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/sa/core/*.h)

source_group("Source Files\\core"  FILES ${TERRALIB_CORE_SRC_FILES})
source_group("Header Files\\core"  FILES ${TERRALIB_CORE_HDR_FILES})

set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES} 
                   ${TERRALIB_CORE_SRC_FILES} ${TERRALIB_CORE_HDR_FILES})

add_library(terralib_mod_sa_core SHARED ${TERRALIB_FILES})

target_link_libraries(terralib_mod_sa_core terralib_mod_plugin
                                           terralib_mod_dataaccess
                                           terralib_mod_geometry
                                           terralib_mod_datatype
                                           terralib_mod_common
                                           terralib_mod_graph
                                           terralib_mod_raster)

set_target_properties(terralib_mod_sa_core
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

install(TARGETS terralib_mod_sa_core
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/sa COMPONENT devel)

export(TARGETS terralib_mod_sa_core APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
