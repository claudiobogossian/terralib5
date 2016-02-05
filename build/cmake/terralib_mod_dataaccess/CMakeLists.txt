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
#  Description: Build configuration for the Data Access Module.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})

if(WIN32)
  add_definitions(-DTEDATAACCESSDLL)
endif()

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/dataaccess/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/dataaccess/*.h)

file(GLOB TERRALIB_DATASET_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/dataaccess/dataset/*.cpp)
file(GLOB TERRALIB_DATASET_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/dataaccess/dataset/*.h)

file(GLOB TERRALIB_DATASOURCE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/dataaccess/datasource/*.cpp)
file(GLOB TERRALIB_DATASOURCE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/dataaccess/datasource/*.h)

file(GLOB TERRALIB_QUERY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/dataaccess/query/*.cpp)
file(GLOB TERRALIB_QUERY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/dataaccess/query/*.h)

file(GLOB TERRALIB_UTILS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/dataaccess/utils/*.cpp)
file(GLOB TERRALIB_UTILS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/dataaccess/utils/*.h)

source_group("Source Files"              FILES ${TERRALIB_SRC_FILES})
source_group("Header Files"              FILES ${TERRALIB_HDR_FILES})
source_group("Source Files\\dataset"     FILES ${TERRALIB_DATASET_SRC_FILES})
source_group("Header Files\\dataset"     FILES ${TERRALIB_DATASET_HDR_FILES})
source_group("Source Files\\datasource"  FILES ${TERRALIB_DATASOURCE_SRC_FILES})
source_group("Header Files\\datasource"  FILES ${TERRALIB_DATASOURCE_HDR_FILES})
source_group("Source Files\\query"       FILES ${TERRALIB_QUERY_SRC_FILES})
source_group("Header Files\\query"       FILES ${TERRALIB_QUERY_HDR_FILES})
source_group("Source Files\\utils"       FILES ${TERRALIB_UTILS_SRC_FILES})
source_group("Header Files\\utils"       FILES ${TERRALIB_UTILS_HDR_FILES})

if(TERRALIB_MOD_XML_ENABLED)

  file(GLOB TERRALIB_XML_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/dataaccess/serialization/xml/*.cpp)
  file(GLOB TERRALIB_XML_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/dataaccess/serialization/xml/*.h)

  source_group("Source Files\\serialization\\xml"  FILES ${TERRALIB_XML_SRC_FILES})
  source_group("Header Files\\serialization\\xml"  FILES ${TERRALIB_XML_HDR_FILES})

  set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES}
                     ${TERRALIB_DATASET_SRC_FILES} ${TERRALIB_DATASET_HDR_FILES}
                     ${TERRALIB_DATASOURCE_SRC_FILES} ${TERRALIB_DATASOURCE_HDR_FILES}
                     ${TERRALIB_QUERY_SRC_FILES} ${TERRALIB_QUERY_HDR_FILES}
                     ${TERRALIB_UTILS_SRC_FILES} ${TERRALIB_UTILS_HDR_FILES}
                     ${TERRALIB_XML_SRC_FILES} ${TERRALIB_XML_HDR_FILES})

else()

  set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES}
                     ${TERRALIB_DATASET_SRC_FILES} ${TERRALIB_DATASET_HDR_FILES}
                     ${TERRALIB_DATASOURCE_SRC_FILES} ${TERRALIB_DATASOURCE_HDR_FILES}
                     ${TERRALIB_QUERY_SRC_FILES} ${TERRALIB_QUERY_HDR_FILES}
                     ${TERRALIB_UTILS_SRC_FILES} ${TERRALIB_UTILS_HDR_FILES})

endif()

add_library(terralib_mod_dataaccess SHARED ${TERRALIB_FILES})

set(TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_raster")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_geometry")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_datatype")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_common")

if(TERRALIB_MOD_XML_ENABLED)
  list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_xml")
endif()

list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES ${Boost_THREAD_LIBRARY})
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES ${Boost_DATE_TIME_LIBRARY})
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES ${Boost_SYSTEM_LIBRARY})

target_link_libraries(terralib_mod_dataaccess ${TERRALIB_LIBRARIES_DEPENDENCIES})

set_target_properties(terralib_mod_dataaccess
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

install(TARGETS terralib_mod_dataaccess
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/dataaccess COMPONENT devel)

install(FILES ${TERRALIB_DATASET_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/dataaccess/dataset COMPONENT devel)

install(FILES ${TERRALIB_DATASOURCE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/dataaccess/datasource COMPONENT devel)

install(FILES ${TERRALIB_QUERY_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/dataaccess/query COMPONENT devel)

install(FILES ${TERRALIB_UTILS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/dataaccess/utils COMPONENT devel)

if(TERRALIB_MOD_XML_ENABLED)
  install(FILES ${TERRALIB_XML_HDR_FILES}
          DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/dataaccess/serialization/xml COMPONENT devel)
endif()

