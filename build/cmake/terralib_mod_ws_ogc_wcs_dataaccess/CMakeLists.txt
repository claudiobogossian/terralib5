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
#  Description: Build configuration OGC WCS Data Access Driver.
#
#  Author: Vinicius Campanha
#

include_directories(${Boost_INCLUDE_DIR}
                    ${GDAL_INCLUDE_DIR})

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/ws/ogc/wcs/dataaccess/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/ws/ogc/wcs/dataaccess/*.h)

source_group("Source Files"  FILES ${TERRALIB_SRC_FILES})
source_group("Header Files"  FILES ${TERRALIB_HDR_FILES})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(terralib_mod_ws_ogc_wcs_dataacess ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES})


target_link_libraries(terralib_mod_ws_ogc_wcs_dataacess terralib_mod_ws_ogc_wcs_client
                                                          terralib_mod_gdal
                                                          terralib_mod_dataaccess
                                                          terralib_mod_datatype)
