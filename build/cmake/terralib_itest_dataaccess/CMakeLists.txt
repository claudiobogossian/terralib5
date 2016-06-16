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
#  Description: Integration Test for TerraLib Data Access Module.
#
#  Author: Carolina Galvão dos Santos
#

include_directories(${TERRALIB_ABSOLUTE_ROOT_DIR}/src)
include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})

file(GLOB TERRALIB_ITEST_DATAACCESS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/itest/dataaccess/*.cpp)
file(GLOB TERRALIB_ITEST_DATAACCESS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/itest/dataaccess/*.h)

source_group("Source Files" FILES ${TERRALIB_ITEST_DATAACCESS_SRC_FILES})
source_group("Header Files" FILES ${TERRALIB_ITEST_DATAACCESS_HDR_FILES})

add_executable(terralib_itest_dataaccess ${TERRALIB_ITEST_DATAACCESS_SRC_FILES} ${TERRALIB_ITEST_DATAACCESS_HDR_FILES})

target_link_libraries(terralib_itest_dataaccess terralib_mod_dataaccess
                                                terralib_mod_plugin
                                                ${GTEST_BOTH_LIBRARIES}
                                                ${GMOCK_BOTH_LIBRARIES})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER MATCHES ".*clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

add_test(NAME terralib_itest_dataaccess
         COMMAND terralib_itest_dataaccess
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
