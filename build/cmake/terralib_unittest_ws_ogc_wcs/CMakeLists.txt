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
#  Description: Build the Unit Test for the WS OGC Web Coverage Service.
#
#  Author: Vinicius Campanha <vinicius.campanha@funcate.org.br>
#  Author: Emerson Moraes <emerson.moraes@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR}
                    ${CURL_INCLUDE_DIRS}
                    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src)

include_directories(${GMOCK_INCLUDE_DIRS})

add_definitions(-DBOOST_TEST_DYN_LINK)

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/ogc-wcs/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/ogc-wcs/*.h)

add_executable(terralib_unittest_ws_ogc_wcs ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES})

target_link_libraries(terralib_unittest_ws_ogc_wcs terralib_mod_ws_ogc_wcs_client
                                                    terralib_mod_ws_core
                                                    terralib_mod_ws_ogc_wcs_dataaccess
                                                    terralib_mod_qt_apf
                                                    terralib_mod_qt_widgets
                                                    terralib_mod_plugin
                                                    ${CURL_LIBRARIES}
                                                    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                                                    ${GMOCK_BOTH_LIBRARIES})

install(FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_UNITTEST}/ogc-wcs COMPONENT devel)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER MATCHES ".*clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

add_test(NAME terralib_unittest_ws_ogc_wcs COMMAND terralib_unittest_ws_ogc_wcs
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
