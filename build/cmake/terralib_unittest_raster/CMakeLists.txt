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
#  Description: Build the Unit Test for the Raster Processing Blender.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

add_definitions(-DBOOST_TEST_DYN_LINK)

include_directories(${Boost_INCLUDE_DIR}
                    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src)


file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/*.h)
file(GLOB TERRALIB_UNITTEST_RASTER_BAND_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/band/*.cpp)
file(GLOB TERRALIB_UNITTEST_RASTER_BLOCK_UTILS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/block_utils/*.cpp)
file(GLOB TERRALIB_UNITTEST_RASTER_GRID_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/grid/*.cpp)
file(GLOB TERRALIB_UNITTEST_RASTER_INTERPOLATOR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/interpolator/*.cpp)
file(GLOB TERRALIB_UNITTEST_RASTER_ITERATOR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/iterator/*.cpp)
file(GLOB TERRALIB_UNITTEST_RASTER_PROXY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/proxy/*.cpp)
file(GLOB TERRALIB_UNITTEST_RASTER_RASTER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/raster/*.cpp)
file(GLOB TERRALIB_UNITTEST_RASTER_REPROJECTION_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/reprojection/*.cpp)
file(GLOB TERRALIB_UNITTEST_RASTER_SUMMARY_MANAGER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/summary_manager/*.cpp)
file(GLOB TERRALIB_UNITTEST_RASTER_SYNCHRONIZED_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/synchronized/*.cpp)
file(GLOB TERRALIB_UNITTEST_RASTER_VECTORIZER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/raster/vectorizer/*.cpp)


source_group("Source Files\\band"            FILES ${TERRALIB_UNITTEST_RASTER_BAND_SRC_FILES})
source_group("Source Files\\block_utils"     FILES ${TERRALIB_UNITTEST_RASTER_BLOCK_UTILS_SRC_FILES})
source_group("Source Files\\grid"            FILES ${TERRALIB_UNITTEST_RASTER_GRID_SRC_FILES})
source_group("Source Files\\interpolator"    FILES ${TERRALIB_UNITTEST_RASTER_INTERPOLATOR_SRC_FILES})
source_group("Source Files\\iterator"        FILES ${TERRALIB_UNITTEST_RASTER_ITERATOR_SRC_FILES})
source_group("Source Files\\proxy"           FILES ${TERRALIB_UNITTEST_RASTER_PROXY_SRC_FILES})
source_group("Source Files\\raster"          FILES ${TERRALIB_UNITTEST_RASTER_RASTER_SRC_FILES})
source_group("Source Files\\reprojection"    FILES ${TERRALIB_UNITTEST_RASTER_REPROJECTION_SRC_FILES})
source_group("Source Files\\summary_manager" FILES ${TERRALIB_UNITTEST_RASTER_SUMMARY_MANAGER_SRC_FILES})
source_group("Source Files\\synchronized"    FILES ${TERRALIB_UNITTEST_RASTER_SYNCHRONIZED_SRC_FILES})
source_group("Source Files\\vectorizer"      FILES ${TERRALIB_UNITTEST_RASTER_VECTORIZER_SRC_FILES})


add_executable(terralib_unittest_raster ${TERRALIB_SRC_FILES}
                                        ${TERRALIB_HDR_FILES}
                                        ${TERRALIB_UNITTEST_RASTER_BAND_SRC_FILES}
                                        ${TERRALIB_UNITTEST_RASTER_BLOCK_UTILS_SRC_FILES}
                                        ${TERRALIB_UNITTEST_RASTER_GRID_SRC_FILES}
                                        ${TERRALIB_UNITTEST_RASTER_INTERPOLATOR_SRC_FILES}
                                        ${TERRALIB_UNITTEST_RASTER_ITERATOR_SRC_FILES}
                                        ${TERRALIB_UNITTEST_RASTER_PROXY_SRC_FILES}
                                        ${TERRALIB_UNITTEST_RASTER_RASTER_SRC_FILES}
                                        ${TERRALIB_UNITTEST_RASTER_REPROJECTION_SRC_FILES}
                                        ${TERRALIB_UNITTEST_RASTER_SUMMARY_MANAGER_SRC_FILES}
                                        ${TERRALIB_UNITTEST_RASTER_SYNCHRONIZED_SRC_FILES}
                                        ${TERRALIB_UNITTEST_RASTER_VECTORIZER_SRC_FILES})

target_link_libraries(terralib_unittest_raster terralib_mod_dataaccess
                                               terralib_mod_geometry
                                               terralib_mod_memory
                                                
                                               terralib_mod_raster
                                               terralib_mod_rp
                                               ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(NAME terralib_unittest_raster
         COMMAND terralib_unittest_raster
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
