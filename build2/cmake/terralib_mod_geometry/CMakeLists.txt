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
#  Description: Build configuration for the Geometry Module.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})
include_directories(${GEOS_INCLUDE_DIR})

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/geometry/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/geometry/*.h)

if(TERRALIB_MOD_XML_ENABLED)
  file(GLOB TERRALIB_XML_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/geometry/serialization/xml/*.cpp)
  file(GLOB TERRALIB_XML_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/geometry/serialization/xml/*.h)
  set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES} ${TERRALIB_XML_SRC_FILES} ${TERRALIB_XML_HDR_FILES})
else()
  set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES})
endif()

add_library(terralib_mod_geometry SHARED ${TERRALIB_FILES})

set(TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_common")

list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_datatype")

if(TERRALIB_MOD_XML_ENABLED)
  list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_xml")
endif()

if(TERRALIB_MOD_SRS_ENABLED)
  list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_srs")
endif()

list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES ${GEOS_LIBRARY})

list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES ${Boost_THREAD_LIBRARY})

target_link_libraries(terralib_mod_geometry ${TERRALIB_LIBRARIES_DEPENDENCIES})

set_target_properties(terralib_mod_geometry
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

install(TARGETS terralib_mod_geometry
        EXPORT terralib-targets
        RUNTIME DESTINATION bin COMPONENT runtime
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES}
        DESTINATION include/terralib/geometry COMPONENT devel)

if(TERRALIB_MOD_XML_ENABLED)
  install(FILES ${TERRALIB_XML_HDR_FILES}
          DESTINATION include/terralib/geometry/serialization/xml COMPONENT devel)
endif()

export(TARGETS terralib_mod_geometry APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
