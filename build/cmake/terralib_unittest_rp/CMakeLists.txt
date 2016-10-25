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

if(WIN32)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif()

file(GLOB TERRALIB_UNITTEST_RP_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_ARITHMETIC_OPERATIONS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/arithmetic_operations/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_BLENDER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/blender/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_CLASSIFIER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/classifier/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_CONTRAST_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/contrast/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_FILTER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/filter/*.cpp)
file(GLOB TERRALIB_UNITTEST_RP_FUNCTIONS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/functions/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_FUSION_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/fusion/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_MATRIX_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/matrix/*.cpp)
file(GLOB TERRALIB_UNITTEST_RP_MIXTURE_MODEL_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/mixture_model/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_MOSAIC_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/mosaic/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_RADAR_FUNCTIONS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/radar_functions/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_REGISTER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/register/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_SEGMENTER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/segmenter/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_SKELETON_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/skeleton/*.cpp)
#file(GLOB TERRALIB_UNITTEST_RP_TEXTURE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/texture/*.cpp)
file(GLOB TERRALIB_UNITTEST_RP_TIE_POINTS_LOCATOR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/rp/tie_points_locator/*.cpp)

source_group("Source Files"                         FILES ${TERRALIB_UNITTEST_RP_SRC_FILES})
#source_group("Source Files\\arithmetic_operations"  FILES ${TERRALIB_UNITTEST_RP_ARITHMETIC_OPERATIONS_SRC_FILES})
#source_group("Source Files\\blender"                FILES ${TERRALIB_UNITTEST_RP_BLENDER_SRC_FILES})
#source_group("Source Files\\classifier"             FILES ${TERRALIB_UNITTEST_RP_CLASSIFIER_SRC_FILES})
#source_group("Source Files\\contrast"               FILES ${TERRALIB_UNITTEST_RP_CONTRAST_SRC_FILES})
#source_group("Source Files\\filter"                 FILES ${TERRALIB_UNITTEST_RP_FILTER_SRC_FILES})
source_group("Source Files\\functions"              FILES ${TERRALIB_UNITTEST_RP_FUNCTIONS_SRC_FILES})
#source_group("Source Files\\fusion"                 FILES ${TERRALIB_UNITTEST_RP_FUSION_SRC_FILES})
#source_group("Source Files\\matrix"                 FILES ${TERRALIB_UNITTEST_RP_MATRIX_SRC_FILES})
source_group("Source Files\\mixture_model"          FILES ${TERRALIB_UNITTEST_RP_MIXTURE_MODEL_SRC_FILES})
#source_group("Source Files\\mosaic"                 FILES ${TERRALIB_UNITTEST_RP_MOSAIC_SRC_FILES})
#source_group("Source Files\\radar_functions"        FILES ${TERRALIB_UNITTEST_RP_RADAR_FUNCTIONS_SRC_FILES})
#source_group("Source Files\\register"               FILES ${TERRALIB_UNITTEST_RP_REGISTER_SRC_FILES})
#source_group("Source Files\\segmenter"              FILES ${TERRALIB_UNITTEST_RP_SEGMENTER_SRC_FILES})
#source_group("Source Files\\skeleton"               FILES ${TERRALIB_UNITTEST_RP_SKELETON_SRC_FILES})
#source_group("Source Files\\texture"                FILES ${TERRALIB_UNITTEST_RP_TEXTURE_SRC_FILES})
source_group("Source Files\\tie_points_locator"     FILES ${TERRALIB_UNITTEST_RP_TIE_POINTS_LOCATOR_SRC_FILES})


include_directories(${Boost_INCLUDE_DIR}
                    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src)

include_directories(${GDAL_INCLUDE_DIR})

add_executable(terralib_unittest_rp ${TERRALIB_UNITTEST_RP_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_ARITHMETIC_OPERATIONS_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_BLENDER_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_CLASSIFIER_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_CONTRAST_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_FILTER_SRC_FILES}
                                    ${TERRALIB_UNITTEST_RP_FUNCTIONS_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_FUSION_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_MATRIX_SRC_FILES}
                                    ${TERRALIB_UNITTEST_RP_MIXTURE_MODEL_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_MOSAIC_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_RADAR_FUNCTIONS_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_REGISTER_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_SEGMENTER_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_SKELETON_SRC_FILES}
#                                    ${TERRALIB_UNITTEST_RP_TEXTURE_SRC_FILES}
                                    ${TERRALIB_UNITTEST_RP_TIE_POINTS_LOCATOR_SRC_FILES})

target_link_libraries(terralib_unittest_rp terralib_mod_geometry
                                           terralib_mod_plugin
                                           terralib_mod_raster
                                           terralib_mod_rp
                                           ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(NAME terralib_unittest_rp
         COMMAND terralib_unittest_rp
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
