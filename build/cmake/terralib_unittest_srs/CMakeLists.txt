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
#  Description: Build the Unit Test for the SRS module.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#


include_directories(${Boost_INCLUDE_DIR}
                    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src)

add_definitions(-DBOOST_TEST_DYN_LINK)

file(GLOB TERRALIB_UNITTEST_SRS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/srs/*.h)
file(GLOB TERRALIB_UNITTEST_SRS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/srs/*.cpp)

source_group("Header Files" FILES ${TERRALIB_UNITTEST_SRS_HDR_FILES})
source_group("Source Files" FILES ${TERRALIB_UNITTEST_SRS_SRC_FILES})

add_executable(terralib_unittest_srs ${TERRALIB_UNITTEST_SRS_HDR_FILES}
                                     ${TERRALIB_UNITTEST_SRS_SRC_FILES})

target_link_libraries(terralib_unittest_srs terralib_mod_srs
                                            ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(NAME terralib_unittest_srs
         COMMAND terralib_unittest_srs
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

install(FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_UNITTEST}/srs COMPONENT devel)
