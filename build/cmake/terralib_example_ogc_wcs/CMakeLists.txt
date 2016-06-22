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
#  Description: The WS OGC WCS examples
#
#  Author: Vinicius Campanha <vinicius.campanha@funcate.org.br>
#

include_directories(${TERRALIB_ABSOLUTE_ROOT_DIR}/src)

file(GLOB TERRALIB_EXAMPLE_OGC_WCS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/ogc-wcs/*.cpp)
file(GLOB TERRALIB_EXAMPLE_OGC_WCS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/ogc-wcs/*.h)

add_executable(terralib_example_ws_ogc_wcs ${TERRALIB_EXAMPLE_OGC_WCS_SRC_FILES} ${TERRALIB_EXAMPLE_OGC_WCS_HDR_FILES})

target_link_libraries(terralib_example_ws_ogc_wcs terralib_mod_ws_ogc_wcs_client
                                                   terralib_mod_ws_ogc_wcs_dataaccess
                                                   terralib_mod_plugin)

install(FILES ${TERRALIB_EXAMPLE_OGC_WCS_SRC_FILES} ${TERRALIB_EXAMPLE_OGC_WCS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_EXAMPLES}/ogc-wcs COMPONENT devel)
