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
#  Description: TerraLib XML Module.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})

if(TERRALIB_XERCES_ENABLED)
  include_directories(${XERCES_INCLUDE_DIR})
endif()

if(WIN32)
  add_definitions(-DTEXMLDLL)
endif()

file(GLOB TERRALIB_MOD_XML_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/xml/*.cpp)
file(GLOB TERRALIB_MOD_XML_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/xml/*.h)

if(TERRALIB_XERCES_ENABLED)
  file(GLOB TERRALIB_MOD_XERCES_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/xerces/*.cpp)
  file(GLOB TERRALIB_MOD_XERCES_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/xerces/*.h)
endif()

add_library(terralib_mod_xml SHARED ${TERRALIB_MOD_XML_SRC_FILES} ${TERRALIB_MOD_XML_HDR_FILES}
                                    ${TERRALIB_MOD_XERCES_SRC_FILES} ${TERRALIB_MOD_XERCES_HDR_FILES})

if(TERRALIB_XERCES_ENABLED)
    set(LINKED_LIBRARIES ${LINKED_LIBRARIES} ${XERCES_LIBRARY})
endif()

target_link_libraries(terralib_mod_xml terralib_mod_common
                                       ${LINKED_LIBRARIES})

set_target_properties(terralib_mod_xml
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

install(TARGETS terralib_mod_xml
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_MOD_XML_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/xml COMPONENT devel)
