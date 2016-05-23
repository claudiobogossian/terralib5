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
#  Description: Examples for TerraLib Core Runtime Module.
#
#  Author: Vinicius Campanha
#          Matheus Cavassan Zaglia
#

include_directories(${Boost_INCLUDE_DIR})
include_directories(${TERRALIB_ABSOLUTE_ROOT_DIR}/src)


add_definitions(-DBOOST_LOG_DYN_LINK)

# Examples of core/uri
file(GLOB TERRALIB_EXAMPLE_CORE_URI_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/core/uri/*.cpp)
file(GLOB TERRALIB_EXAMPLE_CORE_URI_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/core/uri/*.h)

source_group("Source Files\\uri"  FILES ${TERRALIB_EXAMPLE_CORE_URI_SRC_FILES})
source_group("Header Files\\uri"  FILES ${TERRALIB_EXAMPLE_CORE_URI_HDR_FILES})

add_executable(terralib_example_core_uri ${TERRALIB_EXAMPLE_CORE_URI_SRC_FILES} ${TERRALIB_EXAMPLE_CORE_URI_HDR_FILES})

target_link_libraries(terralib_example_core_uri terralib_mod_core)

install(FILES ${TERRALIB_EXAMPLE_CORE_URI_SRC_FILES} ${TERRALIB_EXAMPLE_CORE_URI_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_EXAMPLES}/core COMPONENT devel)


# Examples of core/encoding
file(GLOB TERRALIB_EXAMPLE_CORE_ENCODING_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/core/encoding/*.cpp)
file(GLOB TERRALIB_EXAMPLE_CORE_ENCODING_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/core/encoding/*.h)

source_group("Source Files\\encoding"  FILES ${TERRALIB_EXAMPLE_CORE_ENCODING_SRC_FILES})
source_group("Header Files\\encoding"  FILES ${TERRALIB_EXAMPLE_CORE_ENCODING_HDR_FILES})

add_executable(terralib_example_core_encoding ${TERRALIB_EXAMPLE_CORE_ENCODING_SRC_FILES} ${TERRALIB_EXAMPLE_CORE_ENCODING_HDR_FILES})

target_link_libraries(terralib_example_core_encoding terralib_mod_core)

install(FILES ${TERRALIB_EXAMPLE_CORE_ENCODING_SRC_FILES} ${TERRALIB_EXAMPLE_CORE_ENCODING_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_EXAMPLES}/encoding COMPONENT devel)

file(GLOB TERRALIB_EXAMPLE_CORE_LIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/core/lib/*.cpp)

add_executable(terralib_example_core_lib ${TERRALIB_EXAMPLE_CORE_LIB_SRC_FILES})


target_link_libraries(terralib_example_core_lib terralib_mod_core
                                                ${Boost_FILESYSTEM_LIBRARY}
                                                ${Boost_SYSTEM_LIBRARY})


#Examples of translator
file(GLOB TERRALIB_EXAMPLE_CORE_TRANSLATOR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/core/translator/*.cpp)

source_group("Source Files\\translator"  FILES ${TERRALIB_EXAMPLE_CORE_TRANSLATOR_SRC_FILES})

add_executable(terralib_example_core_translator ${TERRALIB_EXAMPLE_CORE_TRANSLATOR_SRC_FILES})

target_link_libraries(terralib_example_core_translator terralib_mod_core)


if(GETTEXT_FOUND)
  if(TERRALIB_TRANSLATOR_ENABLED)
    TERRALIB_REGISTER_TRANSLATION("terralib_example_core_translator" "pt_BR")
  endif()
endif()

#Examples of logger
#file(GLOB TERRALIB_EXAMPLE_CORE_LOGGER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/core/logger/*.cpp)

#source_group("Source Files\\logger"  FILES ${TERRALIB_EXAMPLE_CORE_LOGGER_SRC_FILES})

#add_executable(terralib_example_core_logger ${TERRALIB_EXAMPLE_CORE_LOGGER_SRC_FILES})

#target_link_libraries(terralib_example_core_logger terralib_mod_core)

# Library of functions to example lib

if(WIN32)
  add_definitions(-DTECOREEXEMPDLL)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/example)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/example)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/example)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/example)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/example)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/example)

file(GLOB HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/corelib_lib_example/*.h)
file(GLOB SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/corelib_lib_example/*.cpp)

add_library(terralib_example_core_lib_function SHARED ${HDR_FILES} ${SRC_FILES})

set_target_properties(terralib_example_core_lib_function
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

add_dependencies(terralib_example_core_lib terralib_example_core_lib_function)


