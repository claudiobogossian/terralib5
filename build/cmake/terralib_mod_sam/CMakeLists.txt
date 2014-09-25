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
#  Description: TerraLib Spatial Access Methods (SAM).
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Emiliano Ferreira Castejon <castejon@dpi.inpe.br>
#

include_directories(${Boost_INCLUDE_DIR})

file(GLOB TERRALIB_KDTREE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/sam/kdtree/*.h)
file(GLOB TERRALIB_RTREE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/sam/rtree/*.h)

source_group("Header Files\\kdtree"  FILES ${TERRALIB_KDTREE_HDR_FILES})
source_group("Header Files\\rtree"  FILES ${TERRALIB_RTREE_HDR_FILES})

add_library(terralib_mod_sam STATIC EXCLUDE_FROM_ALL ${TERRALIB_KDTREE_HDR_FILES} ${TERRALIB_RTREE_HDR_FILES})

set_target_properties(terralib_mod_sam PROPERTIES LINKER_LANGUAGE CXX)

install(FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/sam/kdtree.h
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/sam COMPONENT devel)

install(FILES ${TERRALIB_KDTREE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/sam/kdtree COMPONENT devel)

install(FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/sam/rtree.h
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/sam COMPONENT devel)

install(FILES ${TERRALIB_RTREE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/sam/rtree COMPONENT devel)
