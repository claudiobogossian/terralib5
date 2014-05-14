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
#  Description: Build configuration for the ST Module.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})

if(WIN32)
  add_definitions(-DTESTDLL)
endif(WIN32)

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/*.h)

source_group("Source Files"  FILES ${TERRALIB_SRC_FILES})
source_group("Header Files"  FILES ${TERRALIB_HDR_FILES})

file(GLOB TERRALIB_CORE_COVERAGE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/core/coverage/*.cpp)
file(GLOB TERRALIB_CORE_COVERAGE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/core/coverage/*.h)

source_group("Source Files\\core\\coverage"  FILES ${TERRALIB_CORE_COVERAGE_SRC_FILES})
source_group("Header Files\\core\\coverage"  FILES ${TERRALIB_CORE_COVERAGE_HDR_FILES})

file(GLOB TERRALIB_CORE_EVENT_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/core/event/*.cpp)
file(GLOB TERRALIB_CORE_EVENT_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/core/event/*.h)

file(GLOB TERRALIB_CORE_INTERPOLATOR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/core/interpolator/*.cpp)
file(GLOB TERRALIB_CORE_INTERPOLATOR_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/core/interpolator/*.h)

file(GLOB TERRALIB_CORE_OBSERVATION_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/core/observation/*.cpp)
file(GLOB TERRALIB_CORE_OBSERVATION_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/core/observation/*.h)

file(GLOB TERRALIB_CORE_TIMESERIES_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/core/timeseries/*.cpp)
file(GLOB TERRALIB_CORE_TIMESERIES_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/core/timeseries/*.h)

file(GLOB TERRALIB_CORE_TRAJECTORY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/core/trajectory/*.cpp)
file(GLOB TERRALIB_CORE_TRAJECTORY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/core/trajectory/*.h)

file(GLOB TERRALIB_LOADER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib//st/loader/*.cpp)
file(GLOB TERRALIB_LOADER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/st/loader/*.h)

source_group("Source Files\\core\\event"         FILES ${TERRALIB_CORE_EVENT_SRC_FILES})
source_group("Header Files\\core\\event"         FILES ${TERRALIB_CORE_EVENT_HDR_FILES})
source_group("Source Files\\core\\interpolator"  FILES ${TERRALIB_CORE_INTERPOLATOR_SRC_FILES})
source_group("Header Files\\core\\interpolator"  FILES ${TERRALIB_CORE_INTERPOLATOR_HDR_FILES})
source_group("Source Files\\core\\observation"   FILES ${TERRALIB_CORE_OBSERVATION_SRC_FILES})
source_group("Header Files\\core\\observation"   FILES ${TERRALIB_CORE_OBSERVATION_HDR_FILES})
source_group("Source Files\\core\\timeseries"    FILES ${TERRALIB_CORE_TIMESERIES_SRC_FILES})
source_group("Header Files\\core\\timeseries"    FILES ${TERRALIB_CORE_TIMESERIES_HDR_FILES})
source_group("Source Files\\core\\trajectory"    FILES ${TERRALIB_CORE_TRAJECTORY_SRC_FILES})
source_group("Header Files\\core\\trajectory"    FILES ${TERRALIB_CORE_TRAJECTORY_HDR_FILES})
source_group("Source Files\\loader"              FILES ${TERRALIB_LOADER_SRC_FILES})
source_group("Header Files\\loader"              FILES ${TERRALIB_LOADER_HDR_FILES})

set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES}
                   ${TERRALIB_CORE_COVERAGE_SRC_FILES} ${TERRALIB_CORE_COVERAGE_HDR_FILES}
                   ${TERRALIB_CORE_EVENT_SRC_FILES} ${TERRALIB_CORE_EVENT_HDR_FILES}
                   ${TERRALIB_CORE_INTERPOLATOR_SRC_FILES} ${TERRALIB_CORE_INTERPOLATOR_HDR_FILES}
                   ${TERRALIB_CORE_OBSERVATION_SRC_FILES} ${TERRALIB_CORE_OBSERVATION_HDR_FILES}
                   ${TERRALIB_CORE_TIMESERIES_SRC_FILES} ${TERRALIB_CORE_TIMESERIES_HDR_FILES}
                   ${TERRALIB_CORE_TRAJECTORY_SRC_FILES} ${TERRALIB_CORE_TRAJECTORY_HDR_FILES}
                   ${TERRALIB_LOADER_SRC_FILES} ${TERRALIB_LOADER_HDR_FILES})
                   

add_library(terralib_mod_st SHARED ${TERRALIB_FILES})

set(TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_dataaccess")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_geometry")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_datatype")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_raster")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_stmemory")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_plugin")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_common")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES ${Boost_DATE_TIME_LIBRARY})

target_link_libraries(terralib_mod_st ${TERRALIB_LIBRARIES_DEPENDENCIES})

set_target_properties(terralib_mod_st
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

install(TARGETS terralib_mod_st
        EXPORT terralib-targets
        RUNTIME DESTINATION bin COMPONENT runtime
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES}
        DESTINATION include/terralib/st COMPONENT devel)

install(FILES ${TERRALIB_CORE_COVERAGE_HDR_FILES}
        DESTINATION include/terralib/st/core/coverage COMPONENT devel)

install(FILES ${TERRALIB_CORE_EVENT_HDR_FILES}
        DESTINATION include/terralib/st/core/event COMPONENT devel)

install(FILES ${TERRALIB_CORE_INTERPOLATOR_HDR_FILES}
        DESTINATION include/terralib/st/core/interpolator COMPONENT devel)

install(FILES ${TERRALIB_CORE_OBSERVATION_HDR_FILES}
        DESTINATION include/terralib/st/core/observation COMPONENT devel)

install(FILES ${TERRALIB_CORE_TIMESERIES_HDR_FILES}
        DESTINATION include/terralib/st/core/timeseries COMPONENT devel)

install(FILES ${TERRALIB_CORE_TRAJECTORY_HDR_FILES}
        DESTINATION include/terralib/st/core/trajectory COMPONENT devel)

 install(FILES ${TERRALIB_LOADER_HDR_FILES}
        DESTINATION include/terralib/st/loader COMPONENT devel)

export(TARGETS terralib_mod_st APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
