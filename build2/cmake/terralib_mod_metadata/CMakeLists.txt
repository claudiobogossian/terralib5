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
#  Description: Build configuration for the Symbology Encoding Module.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})

file(GLOB ROOT_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/metadata/*.cpp)
file(GLOB ROOT_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/metadata/*.h)

file(GLOB ISO_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/metadata/iso19115/*.cpp)
file(GLOB ISO_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/metadata/iso19115/*.h)

source_group("Source Files"            FILES ${ROOT_SRC_FILES})
source_group("Header Files"            FILES ${ROOT_HDR_FILES})
source_group("Source Files\\iso19115"  FILES ${ISO_SRC_FILES})
source_group("Header Files\\iso19115"  FILES ${ISO_HDR_FILES})

if(TERRALIB_MOD_XML_ENABLED)
  file(GLOB TERRALIB_XML_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/metadata/serialization/xml/*.cpp)
  file(GLOB TERRALIB_XML_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/metadata/serialization/xml/*.h)
  source_group("Source Files\\serialization\\xml"  FILES ${TERRALIB_XML_SRC_FILES})
  source_group("Header Files\\serialization\\xml"  FILES ${TERRALIB_XML_HDR_FILES})
  set(TERRALIB_FILES ${ROOT_SRC_FILES} ${ROOT_HDR_FILES} ${ISO_SRC_FILES} ${ISO_HDR_FILES} ${TERRALIB_XML_SRC_FILES} ${TERRALIB_XML_HDR_FILES})
else()
  set(TERRALIB_FILES ${ROOT_SRC_FILES} ${ROOT_HDR_FILES} ${ISO_SRC_FILES} ${ISO_HDR_FILES})
endif()

add_library(terralib_mod_metadata SHARED ${TERRALIB_FILES})

if(TERRALIB_MOD_XML_ENABLED)
  target_link_libraries(terralib_mod_metadata terralib_mod_dataaccess
                                              terralib_mod_datatype
                                              terralib_mod_geometry
                                              terralib_mod_xml
                                              terralib_mod_common)
else()
  target_link_libraries(terralib_mod_metadata terralib_mod_dataaccess
                                              terralib_mod_datatype
                                              terralib_mod_geometry
                                              terralib_mod_common)
endif()

set_target_properties(terralib_mod_metadata
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                      SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

install(TARGETS terralib_mod_metadata
        EXPORT terralib-targets
        RUNTIME DESTINATION bin COMPONENT runtime
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT runtime)

install(FILES ${ROOT_HDR_FILES}
        DESTINATION include/terralib/metadata COMPONENT devel)

install(FILES ${ISO_HDR_FILES}
        DESTINATION include/terralib/metadata/iso19115 COMPONENT devel)

if(TERRALIB_MOD_XML_ENABLED)
  install(FILES ${TERRALIB_XML_HDR_FILES}
          DESTINATION include/terralib/metadata/serialization/xml COMPONENT devel)
endif()

export(TARGETS terralib_mod_metadata APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
