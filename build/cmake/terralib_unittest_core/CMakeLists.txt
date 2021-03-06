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
#  Description: Unit Tests for TerraLib Core Runtime Module.
#
#  Author: Vinicius Campanha
#          Matheus Cavassan Zaglia
#          Frederico Bedê
#          Gilberto Ribeiro de Queiroz
#

include_directories(${TERRALIB_ABSOLUTE_ROOT_DIR}/src)

add_definitions(-DBOOST_TEST_DYN_LINK)

file(GLOB TERRALIB_UNITTEST_CORE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/*.cpp)
file(GLOB TERRALIB_UNITTEST_CORE_ENCODING_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/encoding/*.cpp)
file(GLOB TERRALIB_UNITTEST_CORE_FILESYSTEM_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/filesystem/*.cpp)
file(GLOB TERRALIB_UNITTEST_CORE_LIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/lib/*.cpp)
file(GLOB TERRALIB_UNITTEST_CORE_LOGGER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/logger/*.cpp)
file(GLOB TERRALIB_UNITTEST_CORE_PLUGIN_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/plugin/*.cpp)
file(GLOB TERRALIB_UNITTEST_CORE_TRANSLATOR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/translator/*.cpp)
file(GLOB TERRALIB_UNITTEST_CORE_URI_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/uri/*.cpp)
file(GLOB TERRALIB_UNITTEST_CORE_UTILS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/utils/*.cpp)



source_group("Source Files"              FILES ${TERRALIB_UNITTEST_CORE_SRC_FILES})
source_group("Source Files\\encoding"    FILES ${TERRALIB_UNITTEST_CORE_ENCODING_SRC_FILES})
source_group("Source Files\\filesystem"  FILES ${TERRALIB_UNITTEST_CORE_FILESYSTEM_SRC_FILES})
source_group("Source Files\\lib"         FILES ${TERRALIB_UNITTEST_CORE_LIB_SRC_FILES})
source_group("Source Files\\logger"      FILES ${TERRALIB_UNITTEST_CORE_LOGGER_SRC_FILES})
source_group("Source Files\\plugin"      FILES ${TERRALIB_UNITTEST_CORE_PLUGIN_SRC_FILES})
source_group("Source Files\\translator"  FILES ${TERRALIB_UNITTEST_CORE_TRANSLATOR_SRC_FILES})
source_group("Source Files\\uri"         FILES ${TERRALIB_UNITTEST_CORE_URI_SRC_FILES})
source_group("Source Files\\utils"       FILES ${TERRALIB_UNITTEST_CORE_UTILS_SRC_FILES})


add_executable(terralib_unittest_core ${TERRALIB_UNITTEST_CORE_SRC_FILES}
                                      ${TERRALIB_UNITTEST_CORE_ENCODING_SRC_FILES}
                                      ${TERRALIB_UNITTEST_CORE_FILESYSTEM_SRC_FILES}
                                      ${TERRALIB_UNITTEST_CORE_LIB_SRC_FILES}
                                      ${TERRALIB_UNITTEST_CORE_LOGGER_SRC_FILES}
                                      ${TERRALIB_UNITTEST_CORE_PLUGIN_SRC_FILES}
                                      ${TERRALIB_UNITTEST_CORE_TRANSLATOR_SRC_FILES}
                                      ${TERRALIB_UNITTEST_CORE_URI_SRC_FILES}
                                      ${TERRALIB_UNITTEST_CORE_UTILS_SRC_FILES})

target_link_libraries(terralib_unittest_core terralib_mod_core ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                                                               ${Boost_FILESYSTEM_LIBRARY}
                                                               ${Boost_SYSTEM_LIBRARY})


TERRALIB_REGISTER_TRANSLATION("terralib_unittest_core" "pt_BR"
                              "${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations"
                              "${CMAKE_BINARY_DIR}/share/terralib/translations")

add_test(NAME terralib_unittest_core COMMAND terralib_unittest_core
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Plugin for core/plugin

## plugin 1
file(GLOB TERRALIB_UNITTEST_CORE_PLUGIN1_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/plugin/te_unittest_core_plugin1/*.cpp)

add_library(terralib_unittest_core_plugin1 SHARED ${TERRALIB_UNITTEST_CORE_PLUGIN1_SRC_FILES})
target_link_libraries(terralib_unittest_core_plugin1 terralib_mod_core)

file(COPY ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/plugin/te_unittest_core_plugin1/plugin1.teplg.json
    DESTINATION ${CMAKE_BINARY_DIR}/unittest/plugins)

## plugin 2
file(GLOB TERRALIB_UNITTEST_CORE_PLUGIN2_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/plugin/te_unittest_core_plugin2/*.cpp)

add_library(terralib_unittest_core_plugin2 SHARED ${TERRALIB_UNITTEST_CORE_PLUGIN2_SRC_FILES})
target_link_libraries(terralib_unittest_core_plugin2 terralib_mod_core)

file(COPY ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/plugin/te_unittest_core_plugin2/plugin2.teplg.json
    DESTINATION ${CMAKE_BINARY_DIR}/unittest/plugins)

## plugin 3
file(GLOB TERRALIB_UNITTEST_CORE_PLUGIN3_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/plugin/te_unittest_core_plugin3/*.cpp)

add_library(terralib_unittest_core_plugin3 SHARED ${TERRALIB_UNITTEST_CORE_PLUGIN3_SRC_FILES})
target_link_libraries(terralib_unittest_core_plugin3 terralib_mod_core)

file(COPY ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/plugin/te_unittest_core_plugin3/plugin3.teplg.json
    DESTINATION ${CMAKE_BINARY_DIR}/unittest/plugins)

## plugin 4
file(GLOB TERRALIB_UNITTEST_CORE_PLUGIN4_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/plugin/te_unittest_core_plugin4/*.cpp)

add_library(terralib_unittest_core_plugin4 SHARED ${TERRALIB_UNITTEST_CORE_PLUGIN4_SRC_FILES})
target_link_libraries(terralib_unittest_core_plugin4 terralib_mod_core)

file(COPY ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/plugin/te_unittest_core_plugin4/plugin4.teplg.json
    DESTINATION ${CMAKE_BINARY_DIR}/unittest/plugins)

add_dependencies(terralib_unittest_core terralib_unittest_core_plugin1
                                        terralib_unittest_core_plugin2
                                        terralib_unittest_core_plugin3
                                        terralib_unittest_core_plugin4)


if(WIN32)
  add_definitions(-DTECOREEXEMPDLL)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/example)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/example)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/example)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/example)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/example)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/example)

file(GLOB HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/core/lib/lib_example/*.h)
file(GLOB SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/core/lib/lib_example/*.cpp)

add_library(terralib_unittest_core_lib_function SHARED ${HDR_FILES} ${SRC_FILES})

set_target_properties(terralib_unittest_core_lib_function
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

add_dependencies(terralib_unittest_core terralib_unittest_core_lib_function)


