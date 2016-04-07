#
#  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.
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
#  Description: TerraLib Core Runtime Module.
#
#  Author: Gilberto Ribeiro de Queiroz
#          Vinicius Campanha
#          Matheus Cavassan Zaglia
#

set(TERRALIB_CORE_DIR ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/core)

include_directories(${Boost_INCLUDE_DIR} ${TERRALIB_CORE_DIR} ${TERRALIB_CORE_DIR}/uri ${TERRALIB_CORE_DIR}/encoding)

if (WIN32)
  add_definitions(-DTEMODULEDLL)
endif()

file(GLOB TERRALIB_CORE_SRC_FILES ${TERRALIB_CORE_DIR}/*.cpp)
file(GLOB TERRALIB_CORE_HDR_FILES ${TERRALIB_CORE_DIR}/*.h)

file(GLOB TERRALIB_CORE_URI_SRC_FILES ${TERRALIB_CORE_DIR}/uri/*.cpp)
file(GLOB TERRALIB_CORE_URI_HDR_FILES ${TERRALIB_CORE_DIR}/uri/*.h)

file(GLOB TERRALIB_CORE_ENCODING_SRC_FILES ${TERRALIB_CORE_DIR}/encoding/*.cpp)
file(GLOB TERRALIB_CORE_ENCODING_HDR_FILES ${TERRALIB_CORE_DIR}/encoding/*.h)

file(GLOB TERRALIB_CORE_LIB_SRC_FILES ${TERRALIB_CORE_DIR}/lib/*.cpp)
file(GLOB TERRALIB_CORE_LIB_HDR_FILES ${TERRALIB_CORE_DIR}/lib/*.h)

source_group("Source Files"            FILES ${TERRALIB_CORE_SRC_FILES})
source_group("Header Files"            FILES ${TERRALIB_CORE_HDR_FILES})
source_group("Source Files\\uri"       FILES ${TERRALIB_CORE_URI_SRC_FILES})
source_group("Header Files\\uri"       FILES ${TERRALIB_CORE_URI_HDR_FILES})
source_group("Source Files\\encoding"  FILES ${TERRALIB_CORE_ENCODING_SRC_FILES})
source_group("Header Files\\encoding"  FILES ${TERRALIB_CORE_ENCODING_HDR_FILES})
source_group("Source Files\\lib"       FILES ${TERRALIB_CORE_LIB_SRC_FILES})
source_group("Header Files\\lib"       FILES ${TERRALIB_CORE_LIB_HDR_FILES})

add_library(terralib_mod_core SHARED ${TERRALIB_CORE_SRC_FILES} ${TERRALIB_CORE_HDR_FILES}
                                     ${TERRALIB_CORE_URI_SRC_FILES} ${TERRALIB_CORE_URI_HDR_FILES}
                                     ${TERRALIB_CORE_ENCODING_SRC_FILES} ${TERRALIB_CORE_ENCODING_HDR_FILES}
                                     ${TERRALIB_CORE_LIB_SRC_FILES} ${TERRALIB_CORE_LIB_HDR_FILES})

target_link_libraries(terralib_mod_core ${Boost_LOCALE_LIBRARY} ${Boost_REGEX_LIBRARY})

set_target_properties(terralib_mod_core
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

install(TARGETS terralib_mod_core
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_CORE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/core COMPONENT devel)

install(FILES ${TERRALIB_CORE_URI_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/core/uri COMPONENT devel)

install(FILES ${TERRALIB_CORE_ENCODING_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/core/encoding COMPONENT devel)
