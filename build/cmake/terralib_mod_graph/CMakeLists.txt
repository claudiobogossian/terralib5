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
#  Description: Build configuration for the GRAPH Module.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})

if(WIN32)
  add_definitions(-DTEGRAPHDLL)
endif()

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/*.h)

file(GLOB TERRALIB_BUILDER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/builder/*.cpp)
file(GLOB TERRALIB_BUILDER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/builder/*.h)

file(GLOB TERRALIB_CACHE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/cache/*.cpp)
file(GLOB TERRALIB_CACHE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/cache/*.h)

file(GLOB TERRALIB_CORE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/core/*.cpp)
file(GLOB TERRALIB_CORE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/core/*.h)

file(GLOB TERRALIB_DRIVERS_DATASOURCE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/drivers/datasource/*.cpp)
file(GLOB TERRALIB_DRIVERS_DATASOURCE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/drivers/datasource/*.h)

file(GLOB TERRALIB_FUNCTIONS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/functions/*.cpp)
file(GLOB TERRALIB_FUNCTIONS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/functions/*.h)

file(GLOB TERRALIB_GRAPHS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/graphs/*.cpp)
file(GLOB TERRALIB_GRAPHS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/graphs/*.h)

file(GLOB TERRALIB_ITERATOR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/iterator/*.cpp)
file(GLOB TERRALIB_ITERATOR_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/iterator/*.h)

file(GLOB TERRALIB_LOADER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/loader/*.cpp)
file(GLOB TERRALIB_LOADER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/loader/*.h)

file(GLOB TERRALIB_MAPTOOLS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/maptools/*.cpp)
file(GLOB TERRALIB_MAPTOOLS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/graph/maptools/*.h)

source_group("Source Files"                       FILES ${TERRALIB_SRC_FILES})
source_group("Header Files"                       FILES ${TERRALIB_HDR_FILES})
source_group("Source Files\\builder"              FILES ${TERRALIB_BUILDER_SRC_FILES})
source_group("Header Files\\builder"              FILES ${TERRALIB_BUILDER_HDR_FILES})
source_group("Source Files\\cache"                FILES ${TERRALIB_CACHE_SRC_FILES})
source_group("Header Files\\cache"                FILES ${TERRALIB_CACHE_HDR_FILES})
source_group("Source Files\\core"                 FILES ${TERRALIB_CORE_SRC_FILES})
source_group("Header Files\\core"                 FILES ${TERRALIB_CORE_HDR_FILES})
source_group("Source Files\\drivers\\datasource"  FILES ${TERRALIB_DRIVERS_DATASOURCE_SRC_FILES})
source_group("Header Files\\drivers\\datasource"  FILES ${TERRALIB_DRIVERS_DATASOURCE_HDR_FILES})
source_group("Source Files\\functions"            FILES ${TERRALIB_FUNCTIONS_SRC_FILES})
source_group("Header Files\\functions"            FILES ${TERRALIB_FUNCTIONS_HDR_FILES})
source_group("Source Files\\graphs"               FILES ${TERRALIB_GRAPHS_SRC_FILES})
source_group("Header Files\\graphs"               FILES ${TERRALIB_GRAPHS_HDR_FILES})
source_group("Source Files\\iterator"             FILES ${TERRALIB_ITERATOR_SRC_FILES})
source_group("Header Files\\iterator"             FILES ${TERRALIB_ITERATOR_HDR_FILES})
source_group("Source Files\\loader"               FILES ${TERRALIB_LOADER_SRC_FILES})
source_group("Header Files\\loader"               FILES ${TERRALIB_LOADER_HDR_FILES})
source_group("Source Files\\maptools"             FILES ${TERRALIB_MAPTOOLS_SRC_FILES})
source_group("Header Files\\maptools"             FILES ${TERRALIB_MAPTOOLS_HDR_FILES})


set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES}
                   ${TERRALIB_BUILDER_SRC_FILES} ${TERRALIB_BUILDER_HDR_FILES}
                   ${TERRALIB_CACHE_SRC_FILES} ${TERRALIB_CACHE_HDR_FILES}
                   ${TERRALIB_CORE_SRC_FILES} ${TERRALIB_CORE_HDR_FILES}
                   ${TERRALIB_DRIVERS_DATASOURCE_SRC_FILES} ${TERRALIB_DRIVERS_DATASOURCE_HDR_FILES}
                   ${TERRALIB_FUNCTIONS_SRC_FILES} ${TERRALIB_FUNCTIONS_HDR_FILES}
                   ${TERRALIB_GRAPHS_SRC_FILES} ${TERRALIB_GRAPHS_HDR_FILES}
                   ${TERRALIB_ITERATOR_SRC_FILES} ${TERRALIB_ITERATOR_HDR_FILES}
                   ${TERRALIB_LOADER_SRC_FILES} ${TERRALIB_LOADER_HDR_FILES}
                   ${TERRALIB_MAPTOOLS_SRC_FILES} ${TERRALIB_MAPTOOLS_HDR_FILES})

add_library(terralib_mod_graph SHARED ${TERRALIB_FILES})

set(TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_raster")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_dataaccess")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_datatype")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_geometry")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_maptools")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_memory")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_plugin")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_raster")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_srs")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_symbology")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_common")


list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES ${Boost_FILESYSTEM_LIBRARY})
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES ${Boost_SYSTEM_LIBRARY})

target_link_libraries(terralib_mod_graph ${TERRALIB_LIBRARIES_DEPENDENCIES})

set_target_properties(terralib_mod_graph
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

install(TARGETS terralib_mod_graph
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/graph COMPONENT devel)

install(FILES ${TERRALIB_BUILDER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/graph/builder COMPONENT devel)

install(FILES ${TERRALIB_CACHE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/graph/cache COMPONENT devel)

install(FILES ${TERRALIB_CORE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/graph/core COMPONENT devel)

install(FILES ${TERRALIB_DRIVERS_DATASOURCE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/graph/drivers/datasource COMPONENT devel)

install(FILES ${TERRALIB_FUNCTIONS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/graph/functions COMPONENT devel)

install(FILES ${TERRALIB_GRAPHS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/graph/graphs COMPONENT devel)

install(FILES ${TERRALIB_ITERATOR_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/graph/iterator COMPONENT devel)

install(FILES ${TERRALIB_LOADER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/graph/loader COMPONENT devel)

install(FILES ${TERRALIB_MAPTOOLS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/graph/maptools COMPONENT devel)

