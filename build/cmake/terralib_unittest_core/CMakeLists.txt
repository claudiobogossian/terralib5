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
#

include_directories(${Boost_INCLUDE_DIR})
include_directories(${TERRALIB_ABSOLUTE_ROOT_DIR}/src)

file(GLOB TERRALIB_UNITTEST_CORE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/*.cpp)
file(GLOB TERRALIB_UNITTEST_CORE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/*.h)

file(GLOB TERRALIB_UNITTEST_CORE_URI_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/uri/*.cpp)
file(GLOB TERRALIB_UNITTEST_CORE_URI_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core/uri/*.h)

source_group("Source Files"       FILES ${TERRALIB_UNITTEST_CORE_SRC_FILES})
source_group("Header Files"       FILES ${TERRALIB_UNITTEST_CORE_HDR_FILES})
source_group("Source Files\\uri"  FILES ${TERRALIB_UNITTEST_CORE_URI_SRC_FILES})
source_group("Header Files\\uri"  FILES ${TERRALIB_UNITTEST_CORE_URI_HDR_FILES})

add_executable(terralib_unittest_core ${TERRALIB_UNITTEST_CORE_SRC_FILES} ${TERRALIB_UNITTEST_CORE_HDR_FILES}
                                      ${TERRALIB_UNITTEST_CORE_URI_SRC_FILES} ${TERRALIB_UNITTEST_CORE_URI_HDR_FILES})

target_link_libraries(terralib_unittest_core terralib_mod_core)
