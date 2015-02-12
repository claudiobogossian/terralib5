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
#  Description: Build configuration for the Qt Widgets Module.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})
include_directories(${QWT_INCLUDE_DIR})
include_directories(${QTPROPERTYBROWSER_INCLUDE_DIR})

add_definitions(-D_SCL_SECURE_NO_WARNINGS -DTEQTWIDGETSDLL -DUNICODE -DQWT_DLL)

#
# Set the source and ui files
#

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/*.h)

# canvas
file(GLOB TERRALIB_CANVAS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/*.cpp)
file(GLOB TERRALIB_CANVAS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/*.h)
file(GLOB TERRALIB_CANVAS_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/ui/*.ui)

# charts
file(GLOB TERRALIB_CHARTS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/*.cpp)
file(GLOB TERRALIB_CHARTS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/*.h)
file(GLOB TERRALIB_CHARTS_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ui/*.ui)

# classification
file(GLOB TERRALIB_CLASSIFICATION_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/*.cpp)
file(GLOB TERRALIB_CLASSIFICATION_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/*.h)
file(GLOB TERRALIB_CLASSIFICATION_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/ui/*.ui)

# colorbar
file(GLOB TERRALIB_COLORBAR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/*.cpp)
file(GLOB TERRALIB_COLORBAR_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/*.h)
file(GLOB TERRALIB_COLORBAR_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/ui/*.ui)

# dataset
file(GLOB TERRALIB_DATASET_EXPLORER_SRC_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/*.cpp)
file(GLOB TERRALIB_DATASET_EXPLORER_HDR_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/*.h)
file(GLOB TERRALIB_DATASET_SELECTOR_SRC_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/*.cpp)
file(GLOB TERRALIB_DATASET_SELECTOR_HDR_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/*.h)
file(GLOB TERRALIB_DATASET_SELECTOR_UI_FILES   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/ui/*.ui)

# datasource
file(GLOB TERRALIB_DATASOURCE_CONNECTOR_SRC_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/connector/*.cpp)
file(GLOB TERRALIB_DATASOURCE_CONNECTOR_HDR_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/connector/*.h)
file(GLOB TERRALIB_DATASOURCE_CORE_SRC_FILES       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/core/*.cpp)
file(GLOB TERRALIB_DATASOURCE_CORE_HDR_FILES       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/core/*.h)
file(GLOB TERRALIB_DATASOURCE_EXPLORER_SRC_FILES   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/*.cpp)
file(GLOB TERRALIB_DATASOURCE_EXPLORER_HDR_FILES   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/*.h)
file(GLOB TERRALIB_DATASOURCE_SELECTOR_SRC_FILES   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/selector/*.cpp)
file(GLOB TERRALIB_DATASOURCE_SELECTOR_HDR_FILES   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/selector/*.h)
file(GLOB TERRALIB_DATASOURCE_SELECTOR_UI_FILES    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/selector/ui/*.ui)

# exchanger
file(GLOB TERRALIB_EXCHANGER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/*.cpp)
file(GLOB TERRALIB_EXCHANGER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/*.h)
file(GLOB TERRALIB_EXCHANGER_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/ui/*.ui)

# externaltable
file(GLOB TERRALIB_EXTERNALTABLE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/externalTable/*.cpp)
file(GLOB TERRALIB_EXTERNALTABLE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/externalTable/*.h)
file(GLOB TERRALIB_EXTERNALTABLE_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/externalTable/ui/*.ui)

# help
file(GLOB TERRALIB_HELP_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/help/*.cpp)
file(GLOB TERRALIB_HELP_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/help/*.h)

# layer
file(GLOB TERRALIB_LAYER_EXPLORER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/*.cpp)
file(GLOB TERRALIB_LAYER_EXPLORER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/*.h)

file(GLOB TERRALIB_LAYER_INFO_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/info/*.cpp)
file(GLOB TERRALIB_LAYER_INFO_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/info/*.h)

file(GLOB TERRALIB_LAYER_SEARCH_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/search/*.cpp)
file(GLOB TERRALIB_LAYER_SEARCH_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/search/*.h)

file(GLOB TERRALIB_LAYER_SEARCH_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/search/ui/*.ui)

file(GLOB TERRALIB_LAYER_SELECTOR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/selector/*.cpp)
file(GLOB TERRALIB_LAYER_SELECTOR_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/selector/*.h)

file(GLOB TERRALIB_LAYER_UTILS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/utils/*.cpp)
file(GLOB TERRALIB_LAYER_UTILS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/utils/*.h)
file(GLOB TERRALIB_LAYER_UTILS_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/utils/ui/*.ui)

# mapdisplay
file(GLOB TERRALIB_MAPDISPLAY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/mapdisplay/*.cpp)
file(GLOB TERRALIB_MAPDISPLAY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/mapdisplay/*.h)

# plugin
file(GLOB TERRALIB_PLUGIN_BUILDER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/builder/*.cpp)
file(GLOB TERRALIB_PLUGIN_BUILDER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/builder/*.h)

file(GLOB TERRALIB_PLUGIN_BUILDER_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/builder/ui/*.ui)

file(GLOB TERRALIB_PLUGIN_MANAGER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/manager/*.cpp)
file(GLOB TERRALIB_PLUGIN_MANAGER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/manager/*.h)

file(GLOB TERRALIB_PLUGIN_MANAGER_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/manager/ui/*.ui)

# progress
file(GLOB TERRALIB_PROGRESS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/progress/*.cpp)
file(GLOB TERRALIB_PROGRESS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/progress/*.h)

# property
file(GLOB TERRALIB_PROPERTY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/*.cpp)
file(GLOB TERRALIB_PROPERTY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/*.h)
file(GLOB TERRALIB_PROPERTY_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/ui/*.ui)

# propertybrowser
file(GLOB TERRALIB_PROPERTY_BROWSER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/propertybrowser/*.cpp)
file(GLOB TERRALIB_PROPERTY_BROWSER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/propertybrowser/*.h)

# query
file(GLOB TERRALIB_QUERY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/*.cpp)
file(GLOB TERRALIB_QUERY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/*.h)
file(GLOB TERRALIB_QUERY_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/ui/*.ui)

# rp
file(GLOB TERRALIB_RASTER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/raster/*.cpp)
file(GLOB TERRALIB_RASTER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/raster/*.h)
file(GLOB TERRALIB_RASTER_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/raster/ui/*.ui)

# rp
file(GLOB TERRALIB_RP_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/*.cpp)
file(GLOB TERRALIB_RP_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/*.h)
file(GLOB TERRALIB_RP_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/ui/*.ui)

# se
file(GLOB TERRALIB_SE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/*.cpp)
file(GLOB TERRALIB_SE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/*.h)

file(GLOB TERRALIB_SE_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/ui/*.ui)

# srs
file(GLOB TERRALIB_SRS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/srs/*.cpp)
file(GLOB TERRALIB_SRS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/srs/*.h)
file(GLOB TERRALIB_SRS_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/srs/ui/*.ui)

# st
file(GLOB TERRALIB_ST_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/*.cpp)
file(GLOB TERRALIB_ST_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/*.h)

file(GLOB TERRALIB_ST_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/ui/*.ui)

# table
file(GLOB TERRALIB_TABLE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/table/*.cpp)
file(GLOB TERRALIB_TABLE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/table/*.h)
file(GLOB TERRALIB_TABLE_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/table/ui/*.ui)

# tools
file(GLOB TERRALIB_TOOLS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/tools/*.cpp)
file(GLOB TERRALIB_TOOLS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/tools/*.h)

# utils
file(GLOB TERRALIB_UTILS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/*.cpp)
file(GLOB TERRALIB_UTILS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/*.h)

file(GLOB TERRALIB_UTILS_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ui/*.ui)

#
# Grouping the source  and ui files
#
source_group("Source Files"  FILES ${TERRALIB_SRC_FILES})
source_group("Header Files"  FILES ${TERRALIB_HDR_FILES})

# canvas
source_group("Source Files\\canvas"  FILES ${TERRALIB_CANVAS_SRC_FILES})
source_group("Header Files\\canvas"  FILES ${TERRALIB_CANVAS_HDR_FILES})
source_group("UI Files\\canvas"      FILES ${TERRALIB_CANVAS_UI_FILES})

# charts
source_group("Source Files\\charts"  FILES ${TERRALIB_CHARTS_SRC_FILES})
source_group("Header Files\\charts"  FILES ${TERRALIB_CHARTS_HDR_FILES})
source_group("UI Files\\charts"      FILES ${TERRALIB_CHARTS_UI_FILES})

# classification
source_group("Source Files\\classification"  FILES ${TERRALIB_CLASSIFICATION_SRC_FILES})
source_group("Header Files\\classification"  FILES ${TERRALIB_CLASSIFICATION_HDR_FILES})
source_group("UI Files\\classification"      FILES ${TERRALIB_CLASSIFICATION_UI_FILES})

# colorbar
source_group("Source Files\\colorbar"  FILES ${TERRALIB_COLORBAR_SRC_FILES})
source_group("Header Files\\colorbar"  FILES ${TERRALIB_COLORBAR_HDR_FILES})
source_group("UI Files\\colorbar"      FILES ${TERRALIB_COLORBAR_UI_FILES})

# explorer
source_group("Source Files\\dataset\\explorer"  FILES ${TERRALIB_DATASET_EXPLORER_SRC_FILES})
source_group("Header Files\\dataset\\explorer"  FILES ${TERRALIB_DATASET_EXPLORER_HDR_FILES})
source_group("Source Files\\dataset\\selector"  FILES ${TERRALIB_DATASET_SELECTOR_SRC_FILES})
source_group("Header Files\\dataset\\selector"  FILES ${TERRALIB_DATASET_SELECTOR_HDR_FILES})
source_group("UI Files\\dataset\\selector"      FILES ${TERRALIB_DATASET_SELECTOR_UI_FILES})

# datasource
source_group("Source Files\\datasource\\connector" FILES ${TERRALIB_DATASOURCE_CONNECTOR_SRC_FILES})
source_group("Header Files\\datasource\\connector" FILES ${TERRALIB_DATASOURCE_CONNECTOR_HDR_FILES})
source_group("Source Files\\datasource\\core"      FILES ${TERRALIB_DATASOURCE_CORE_SRC_FILES})
source_group("Header Files\\datasource\\core"      FILES ${TERRALIB_DATASOURCE_CORE_HDR_FILES})
source_group("Source Files\\datasource\\explorer"  FILES ${TERRALIB_DATASOURCE_EXPLORER_SRC_FILES})
source_group("Header Files\\datasource\\explorer"  FILES ${TERRALIB_DATASOURCE_EXPLORER_HDR_FILES})
source_group("Source Files\\datasource\\selector"  FILES ${TERRALIB_DATASOURCE_SELECTOR_SRC_FILES})
source_group("Header Files\\datasource\\selector"  FILES ${TERRALIB_DATASOURCE_SELECTOR_HDR_FILES})
source_group("UI Files\\datasource\\selector"      FILES ${TERRALIB_DATASOURCE_SELECTOR_UI_FILES})

# exchanger
source_group("Source Files\\exchanger"  FILES ${TERRALIB_EXCHANGER_SRC_FILES})
source_group("Header Files\\exchanger"  FILES ${TERRALIB_EXCHANGER_HDR_FILES})
source_group("UI Files\\exchanger"      FILES ${TERRALIB_EXCHANGER_UI_FILES})

# externaltable
source_group("Source Files\\externaltable"  FILES ${TERRALIB_EXTERNALTABLE_SRC_FILES})
source_group("Header Files\\externaltable"  FILES ${TERRALIB_EXTERNALTABLE_HDR_FILES})
source_group("UI Files\\externaltable"      FILES ${TERRALIB_EXTERNALTABLE_UI_FILES})

# help
source_group("Source Files\\help"  FILES ${TERRALIB_HELP_SRC_FILES})
source_group("Header Files\\help"  FILES ${TERRALIB_HELP_HDR_FILES})

# layer
source_group("Source Files\\layer\\explorer"  FILES ${TERRALIB_LAYER_EXPLORER_SRC_FILES})
source_group("Header Files\\layer\\explorer"  FILES ${TERRALIB_LAYER_EXPLORER_HDR_FILES})

source_group("Source Files\\layer\\info"  FILES ${TERRALIB_LAYER_INFO_SRC_FILES})
source_group("Header Files\\layer\\info"  FILES ${TERRALIB_LAYER_INFO_HDR_FILES})

source_group("Source Files\\layer\\search"  FILES ${TERRALIB_LAYER_SEARCH_SRC_FILES})
source_group("Header Files\\layer\\search"  FILES ${TERRALIB_LAYER_SEARCH_HDR_FILES})
source_group("UI Files\\layer\\search"      FILES ${TERRALIB_LAYER_SEARCH_UI_FILES})

source_group("Source Files\\layer\\selector"  FILES ${TERRALIB_LAYER_SELECTOR_SRC_FILES})
source_group("Header Files\\layer\\selector"  FILES ${TERRALIB_LAYER_SELECTOR_HDR_FILES})

source_group("Source Files\\layer\\utils"  FILES ${TERRALIB_LAYER_UTILS_SRC_FILES})
source_group("Header Files\\layer\\utils"  FILES ${TERRALIB_LAYER_UTILS_HDR_FILES})
source_group("UI Files\\layer\\utils"      FILES ${TERRALIB_LAYER_UTILS_UI_FILES})

# mapdisplay
source_group("Source Files\\mapdisplay"  FILES ${TERRALIB_MAPDISPLAY_SRC_FILES})
source_group("Header Files\\mapdisplay"  FILES ${TERRALIB_MAPDISPLAY_HDR_FILES})

# plugin
source_group("Source Files\\plugin\\builder"  FILES ${TERRALIB_PLUGIN_BUILDER_SRC_FILES})
source_group("Header Files\\plugin\\builder"  FILES ${TERRALIB_PLUGIN_BUILDER_HDR_FILES})
source_group("UI Files\\plugin\\builder"      FILES ${TERRALIB_PLUGIN_BUILDER_UI_FILES})
source_group("Source Files\\plugin\\manager"  FILES ${TERRALIB_PLUGIN_MANAGER_SRC_FILES})
source_group("Header Files\\plugin\\manager"  FILES ${TERRALIB_PLUGIN_MANAGER_HDR_FILES})
source_group("UI Files\\plugin\\manager"      FILES ${TERRALIB_PLUGIN_MANAGER_UI_FILES})

# progress
source_group("Source Files\\progress"  FILES ${TERRALIB_PROGRESS_SRC_FILES})
source_group("Header Files\\progress"  FILES ${TERRALIB_PROGRESS_HDR_FILES})

# property
source_group("Source Files\\property"  FILES ${TERRALIB_PROPERTY_SRC_FILES})
source_group("Header Files\\property"  FILES ${TERRALIB_PROPERTY_HDR_FILES})
source_group("UI Files\\property"      FILES ${TERRALIB_PROPERTY_UI_FILES})

# propertybrowser
source_group("Source Files\\propertybrowser"  FILES ${TERRALIB_PROPERTY_BROWSER_SRC_FILES})
source_group("Header Files\\propertybrowser"  FILES ${TERRALIB_PROPERTY_BROWSER_HDR_FILES})

# query
source_group("Source Files\\query"  FILES ${TERRALIB_QUERY_SRC_FILES})
source_group("Header Files\\query"  FILES ${TERRALIB_QUERY_HDR_FILES})
source_group("UI Files\\query"      FILES ${TERRALIB_QUERY_UI_FILES})

# raster
source_group("Source Files\\raster"  FILES ${TERRALIB_RASTER_SRC_FILES})
source_group("Header Files\\raster"  FILES ${TERRALIB_RASTER_HDR_FILES})
source_group("UI Files\\raster"      FILES ${TERRALIB_RASTER_UI_FILES})

# rp
source_group("Source Files\\rp"  FILES ${TERRALIB_RP_SRC_FILES})
source_group("Header Files\\rp"  FILES ${TERRALIB_RP_HDR_FILES})
source_group("UI Files\\rp"      FILES ${TERRALIB_RP_UI_FILES})

# se
source_group("Source Files\\se"  FILES ${TERRALIB_SE_SRC_FILES})
source_group("Header Files\\se"  FILES ${TERRALIB_SE_HDR_FILES})
source_group("UI Files\\se"      FILES ${TERRALIB_SE_UI_FILES})

# srs
source_group("Source Files\\srs"  FILES ${TERRALIB_SRS_SRC_FILES})
source_group("Header Files\\srs"  FILES ${TERRALIB_SRS_HDR_FILES})
source_group("UI Files\\srs"      FILES ${TERRALIB_SRS_UI_FILES})

# st
source_group("Source Files\\st"  FILES ${TERRALIB_ST_SRC_FILES})
source_group("Header Files\\st"  FILES ${TERRALIB_ST_HDR_FILES})
source_group("UI Files\\st"      FILES ${TERRALIB_ST_UI_FILES})

# table
source_group("Source Files\\table"  FILES ${TERRALIB_TABLE_SRC_FILES})
source_group("Header Files\\table"  FILES ${TERRALIB_TABLE_HDR_FILES})
source_group("UI Files\\table"      FILES ${TERRALIB_TABLE_UI_FILES})

# temporal
source_group("Source Files\\temporal"  FILES ${TERRALIB_TEMPORAL_SRC_FILES})
source_group("Header Files\\temporal"  FILES ${TERRALIB_TEMPORAL_HDR_FILES})
source_group("UI Files\\temporal"      FILES ${TERRALIB_TEMPORAL_UI_FILES})

# tools
source_group("Source Files\\tools"  FILES ${TERRALIB_TOOLS_SRC_FILES})
source_group("Header Files\\tools"  FILES ${TERRALIB_TOOLS_HDR_FILES})

# utils
source_group("Source Files\\utils"  FILES ${TERRALIB_UTILS_SRC_FILES})
source_group("Header Files\\utils"  FILES ${TERRALIB_UTILS_HDR_FILES})
source_group("Header Files\\utils"  FILES ${TERRALIB_UTILS_UI_FILES})

#
# Set the dependency libraries
#

set(TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_common")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_st")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_stmemory")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_statistics_core")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_statistics_qt")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_classification")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_dataaccess")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_datatype")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_raster")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_geometry")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_maptools")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_rp")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_plugin")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES ${QWT_LIBRARY})
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES ${QTPROPERTYBROWSER_LIBRARY})

# Set the initial value of TERRALIB_FILES (valid for Qt4 and Qt5)
list(APPEND TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES})


if(Qt5_FOUND)
  set(CMAKE_AUTOMOC ON)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  #
  # Qt5 uic'ing
  #

  # canvas
  QT5_WRAP_UI(TERRALIB_CANVAS_GEN_HDR_FILES ${TERRALIB_CANVAS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_CANVAS_SRC_FILES} ${TERRALIB_CANVAS_HDR_FILES} ${TERRALIB_CANVAS_GEN_HDR_FILES})

  # charts
  QT5_WRAP_UI(TERRALIB_CHARTS_GEN_HDR_FILES ${TERRALIB_CHARTS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_CHARTS_SRC_FILES} ${TERRALIB_CHARTS_HDR_FILES} ${TERRALIB_CHARTS_GEN_HDR_FILES})

  # classification
  QT5_WRAP_UI(TERRALIB_CLASSIFICATION_GEN_HDR_FILES ${TERRALIB_CLASSIFICATION_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_CLASSIFICATION_SRC_FILES} ${TERRALIB_CLASSIFICATION_HDR_FILES} ${TERRALIB_CLASSIFICATION_GEN_HDR_FILES})

  # colorbar
  QT5_WRAP_UI(TERRALIB_COLORBAR_GEN_HDR_FILES ${TERRALIB_COLORBAR_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_COLORBAR_SRC_FILES} ${TERRALIB_COLORBAR_HDR_FILES} ${TERRALIB_COLORBAR_GEN_HDR_FILES})

  # dataset - explorer
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASET_EXPLORER_SRC_FILES} ${TERRALIB_DATASET_EXPLORER_HDR_FILES})

  # dataset - selector
  QT5_WRAP_UI(TERRALIB_DATASET_SELECTOR_GEN_HDR_FILES ${TERRALIB_DATASET_SELECTOR_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASET_SELECTOR_SRC_FILES} ${TERRALIB_DATASET_SELECTOR_HDR_FILES} ${TERRALIB_DATASET_SELECTOR_GEN_HDR_FILES})

  # datasource - connector
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_CONNECTOR_SRC_FILES} ${TERRALIB_DATASOURCE_CONNECTOR_HDR_FILES})

  # datasource - core
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_CORE_SRC_FILES} ${TERRALIB_DATASOURCE_CORE_HDR_FILES})

  # datasource - explorer
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_EXPLORER_SRC_FILES} ${TERRALIB_DATASOURCE_EXPLORER_HDR_FILES})

  # datasource - selector
  QT5_WRAP_UI(TERRALIB_DATASOURCE_SELECTOR_GEN_HDR_FILES ${TERRALIB_DATASOURCE_SELECTOR_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_SELECTOR_SRC_FILES} ${TERRALIB_DATASOURCE_SELECTOR_HDR_FILES} ${TERRALIB_DATASOURCE_SELECTOR_GEN_HDR_FILES})

  # exchanger
  QT5_WRAP_UI(TERRALIB_EXCHANGER_GEN_HDR_FILES ${TERRALIB_EXCHANGER_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_EXCHANGER_SRC_FILES} ${TERRALIB_EXCHANGER_HDR_FILES} ${TERRALIB_EXCHANGER_GEN_HDR_FILES})

  # externaltable
  QT5_WRAP_UI(TERRALIB_EXTERNALTABLE_GEN_HDR_FILES ${TERRALIB_EXTERNALTABLE_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_EXTERNALTABLE_SRC_FILES} ${TERRALIB_EXTERNALTABLE_HDR_FILES} ${TERRALIB_EXTERNALTABLE_GEN_HDR_FILES})

  # help
  list(APPEND TERRALIB_FILES ${TERRALIB_HELP_SRC_FILES} ${TERRALIB_HELP_HDR_FILES})

  # layer - explorer
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_EXPLORER_SRC_FILES} ${TERRALIB_LAYER_EXPLORER_HDR_FILES})

  # layer - info
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_INFO_SRC_FILES} ${TERRALIB_LAYER_INFO_HDR_FILES})

  # layer - search
  QT5_WRAP_UI(TERRALIB_LAYER_SEARCH_GEN_HDR_FILES ${TERRALIB_LAYER_SEARCH_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_SEARCH_SRC_FILES} ${TERRALIB_LAYER_SEARCH_HDR_FILES} ${TERRALIB_LAYER_SEARCH_GEN_HDR_FILES})

  # layer - selector
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_SELECTOR_SRC_FILES} ${TERRALIB_LAYER_SELECTOR_HDR_FILES})

  # layer - utils
  QT5_WRAP_UI(TERRALIB_LAYER_UTILS_GEN_HDR_FILES ${TERRALIB_LAYER_UTILS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_UTILS_SRC_FILES} ${TERRALIB_LAYER_UTILS_HDR_FILES} ${TERRALIB_LAYER_UTILS_GEN_HDR_FILES})

  # mapdisplay
  list(APPEND TERRALIB_FILES ${TERRALIB_MAPDISPLAY_SRC_FILES} ${TERRALIB_MAPDISPLAY_HDR_FILES})

  # plugin - builder
  QT5_WRAP_UI(TERRALIB_PLUGIN_BUILDER_GEN_HDR_FILES ${TERRALIB_PLUGIN_BUILDER_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_PLUGIN_BUILDER_SRC_FILES} ${TERRALIB_PLUGIN_BUILDER_HDR_FILES} ${TERRALIB_PLUGIN_BUILDER_GEN_HDR_FILES})

  # plugin - manager
  QT5_WRAP_UI(TERRALIB_PLUGIN_MANAGER_GEN_HDR_FILES ${TERRALIB_PLUGIN_MANAGER_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_PLUGIN_MANAGER_SRC_FILES} ${TERRALIB_PLUGIN_MANAGER_HDR_FILES} ${TERRALIB_PLUGIN_MANAGER_GEN_HDR_FILES})

  # progress
  list(APPEND TERRALIB_FILES ${TERRALIB_PROGRESS_SRC_FILES} ${TERRALIB_PROGRESS_HDR_FILES})

  # property
  QT5_WRAP_UI(TERRALIB_PROPERTY_GEN_HDR_FILES ${TERRALIB_PROPERTY_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_PROPERTY_SRC_FILES} ${TERRALIB_PROPERTY_HDR_FILES} ${TERRALIB_PROPERTY_GEN_HDR_FILES})

  # propertybrowser
  list(APPEND TERRALIB_FILES ${TERRALIB_PROPERTY_BROWSER_SRC_FILES} ${TERRALIB_PROPERTY_BROWSER_HDR_FILES})

  # query
  QT5_WRAP_UI(TERRALIB_QUERY_GEN_HDR_FILES ${TERRALIB_QUERY_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_QUERY_SRC_FILES} ${TERRALIB_QUERY_HDR_FILES} ${TERRALIB_QUERY_GEN_HDR_FILES})

  # raster
  QT5_WRAP_UI(TERRALIB_RASTER_GEN_HDR_FILES ${TERRALIB_RASTER_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_RASTER_SRC_FILES} ${TERRALIB_RASTER_HDR_FILES} ${TERRALIB_RASTER_GEN_HDR_FILES})

  # raster Processing
  QT5_WRAP_UI(TERRALIB_RP_GEN_HDR_FILES ${TERRALIB_RP_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_RP_SRC_FILES} ${TERRALIB_RP_HDR_FILES} ${TERRALIB_RP_GEN_HDR_FILES})

  # symbology Encoding
  QT5_WRAP_UI(TERRALIB_SE_GEN_HDR_FILES ${TERRALIB_SE_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_SE_SRC_FILES} ${TERRALIB_SE_HDR_FILES} ${TERRALIB_SE_GEN_HDR_FILES})

  # srs
  QT5_WRAP_UI(TERRALIB_SRS_GEN_HDR_FILES ${TERRALIB_SRS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_SRS_SRC_FILES} ${TERRALIB_SRS_HDR_FILES} ${TERRALIB_SRS_GEN_HDR_FILES})

  # st
  QT5_WRAP_UI(TERRALIB_ST_GEN_HDR_FILES ${TERRALIB_ST_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_ST_SRC_FILES} ${TERRALIB_ST_HDR_FILES} ${TERRALIB_ST_GEN_HDR_FILES})

  # table
  QT5_WRAP_UI(TERRALIB_TABLE_GEN_HDR_FILES ${TERRALIB_TABLE_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_TABLE_SRC_FILES} ${TERRALIB_TABLE_HDR_FILES} ${TERRALIB_TABLE_GEN_HDR_FILES})

  # temporal
  QT5_WRAP_UI(TERRALIB_TEMPORAL_GEN_HDR_FILES ${TERRALIB_TEMPORAL_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_TEMPORAL_SRC_FILES} ${TERRALIB_TEMPORAL_HDR_FILES} ${TERRALIB_TEMPORAL_GEN_HDR_FILES})

  # tools
  list(APPEND TERRALIB_FILES ${TERRALIB_TOOLS_SRC_FILES} ${TERRALIB_TOOLS_HDR_FILES})

  # utils
  QT5_WRAP_UI(TERRALIB_UTILS_GEN_HDR_FILES ${TERRALIB_UTILS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_UTILS_SRC_FILES} ${TERRALIB_UTILS_HDR_FILES} ${TERRALIB_UTILS_GEN_HDR_FILES})

  # end of Qt5 uic'ing

  add_library(terralib_mod_qt_widgets SHARED ${TERRALIB_FILES})

  target_link_libraries(terralib_mod_qt_widgets ${TERRALIB_LIBRARIES_DEPENDENCIES})

  qt5_use_modules(terralib_mod_qt_widgets Widgets PrintSupport)

else() # QT4_FOUND

  include_directories(${CMAKE_CURRENT_BINARY_DIR})

  include(${QT_USE_FILE})

  include_directories(${QT_INCLUDE_DIR})

  add_definitions(${QT_DEFINITIONS})

  #
  # Qt4 uic'ing
  #

  # canvas
  QT4_WRAP_UI(TERRALIB_CANVAS_GEN_HDR_FILES ${TERRALIB_CANVAS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_CANVAS_SRC_FILES} ${TERRALIB_CANVAS_HDR_FILES} ${TERRALIB_CANVAS_GEN_HDR_FILES})

  # charts
  QT4_WRAP_UI(TERRALIB_CHARTS_GEN_HDR_FILES ${TERRALIB_CHARTS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_CHARTS_SRC_FILES} ${TERRALIB_CHARTS_HDR_FILES} ${TERRALIB_CHARTS_GEN_HDR_FILES})

  # classification
  QT4_WRAP_UI(TERRALIB_CLASSIFICATION_GEN_HDR_FILES ${TERRALIB_CLASSIFICATION_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_CLASSIFICATION_SRC_FILES} ${TERRALIB_CLASSIFICATION_HDR_FILES} ${TERRALIB_CLASSIFICATION_GEN_HDR_FILES})

  # colorbar
  QT4_WRAP_UI(TERRALIB_COLORBAR_GEN_HDR_FILES ${TERRALIB_COLORBAR_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_COLORBAR_SRC_FILES} ${TERRALIB_COLORBAR_HDR_FILES} ${TERRALIB_COLORBAR_GEN_HDR_FILES})

  # dataset - explorer
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASET_EXPLORER_SRC_FILES} ${TERRALIB_DATASET_EXPLORER_HDR_FILES})

  # dataset - selector
  QT4_WRAP_UI(TERRALIB_DATASET_SELECTOR_GEN_HDR_FILES ${TERRALIB_DATASET_SELECTOR_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASET_SELECTOR_SRC_FILES} ${TERRALIB_DATASET_SELECTOR_HDR_FILES} ${TERRALIB_DATASET_SELECTOR_GEN_HDR_FILES})

  # datasource - connector
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_CONNECTOR_SRC_FILES} ${TERRALIB_DATASOURCE_CONNECTOR_HDR_FILES})

  # datasource - core
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_CORE_SRC_FILES} ${TERRALIB_DATASOURCE_CORE_HDR_FILES})

  # datasource - explorer
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_EXPLORER_SRC_FILES} ${TERRALIB_DATASOURCE_EXPLORER_HDR_FILES})

  # datasource - selector
  QT4_WRAP_UI(TERRALIB_DATASOURCE_SELECTOR_GEN_HDR_FILES ${TERRALIB_DATASOURCE_SELECTOR_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_SELECTOR_SRC_FILES} ${TERRALIB_DATASOURCE_SELECTOR_HDR_FILES} ${TERRALIB_DATASOURCE_SELECTOR_GEN_HDR_FILES})

  # exchanger
  QT4_WRAP_UI(TERRALIB_EXCHANGER_GEN_HDR_FILES ${TERRALIB_EXCHANGER_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_EXCHANGER_SRC_FILES} ${TERRALIB_EXCHANGER_HDR_FILES} ${TERRALIB_EXCHANGER_GEN_HDR_FILES})

  # externaltable
  QT4_WRAP_UI(TERRALIB_EXTERNALTABLE_GEN_HDR_FILES ${TERRALIB_EXTERNALTABLE_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_EXTERNALTABLE_SRC_FILES} ${TERRALIB_EXTERNALTABLE_HDR_FILES} ${TERRALIB_EXTERNALTABLE_GEN_HDR_FILES})

  # help
  list(APPEND TERRALIB_FILES ${TERRALIB_HELP_SRC_FILES} ${TERRALIB_HELP_HDR_FILES})

  # layer - explorer
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_EXPLORER_SRC_FILES} ${TERRALIB_LAYER_EXPLORER_HDR_FILES})

  # layer - info
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_INFO_SRC_FILES} ${TERRALIB_LAYER_INFO_HDR_FILES})

  # layer - search
  QT4_WRAP_UI(TERRALIB_LAYER_SEARCH_GEN_HDR_FILES ${TERRALIB_LAYER_SEARCH_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_SEARCH_SRC_FILES} ${TERRALIB_LAYER_SEARCH_HDR_FILES} ${TERRALIB_LAYER_SEARCH_GEN_HDR_FILES})

  # layer - selector
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_SELECTOR_SRC_FILES} ${TERRALIB_LAYER_SELECTOR_HDR_FILES})

  # layer - utils
  QT4_WRAP_UI(TERRALIB_LAYER_UTILS_GEN_HDR_FILES ${TERRALIB_LAYER_UTILS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_UTILS_SRC_FILES} ${TERRALIB_LAYER_UTILS_HDR_FILES} ${TERRALIB_LAYER_UTILS_GEN_HDR_FILES})

  # mapdisplay
  list(APPEND TERRALIB_FILES ${TERRALIB_MAPDISPLAY_SRC_FILES} ${TERRALIB_MAPDISPLAY_HDR_FILES})

  # plugin - builder
  QT4_WRAP_UI(TERRALIB_PLUGIN_BUILDER_GEN_HDR_FILES ${TERRALIB_PLUGIN_BUILDER_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_PLUGIN_BUILDER_SRC_FILES} ${TERRALIB_PLUGIN_BUILDER_HDR_FILES} ${TERRALIB_PLUGIN_BUILDER_GEN_HDR_FILES})

  # plugin - manager
  QT4_WRAP_UI(TERRALIB_PLUGIN_MANAGER_GEN_HDR_FILES ${TERRALIB_PLUGIN_MANAGER_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_PLUGIN_MANAGER_SRC_FILES} ${TERRALIB_PLUGIN_MANAGER_HDR_FILES} ${TERRALIB_PLUGIN_MANAGER_GEN_HDR_FILES})

  # progress
  list(APPEND TERRALIB_FILES ${TERRALIB_PROGRESS_SRC_FILES} ${TERRALIB_PROGRESS_HDR_FILES})

  # property
  QT4_WRAP_UI(TERRALIB_PROPERTY_GEN_HDR_FILES ${TERRALIB_PROPERTY_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_PROPERTY_SRC_FILES} ${TERRALIB_PROPERTY_HDR_FILES} ${TERRALIB_PROPERTY_GEN_HDR_FILES})
  
  # propertybrowser
  list(APPEND TERRALIB_FILES ${TERRALIB_PROPERTY_BROWSER_SRC_FILES} ${TERRALIB_PROPERTY_BROWSER_HDR_FILES})

  # query
  QT4_WRAP_UI(TERRALIB_QUERY_GEN_HDR_FILES ${TERRALIB_QUERY_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_QUERY_SRC_FILES} ${TERRALIB_QUERY_HDR_FILES} ${TERRALIB_QUERY_GEN_HDR_FILES})

  # raster
  QT4_WRAP_UI(TERRALIB_RASTER_GEN_HDR_FILES ${TERRALIB_RASTER_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_RASTER_SRC_FILES} ${TERRALIB_RASTER_HDR_FILES} ${TERRALIB_RASTER_GEN_HDR_FILES})
  
  # rp
  QT4_WRAP_UI(TERRALIB_RP_GEN_HDR_FILES ${TERRALIB_RP_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_RP_SRC_FILES} ${TERRALIB_RP_HDR_FILES} ${TERRALIB_RP_GEN_HDR_FILES})

  # se
  QT4_WRAP_UI(TERRALIB_SE_GEN_HDR_FILES ${TERRALIB_SE_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_SE_SRC_FILES} ${TERRALIB_SE_HDR_FILES} ${TERRALIB_SE_GEN_HDR_FILES})

  # srs
  QT4_WRAP_UI(TERRALIB_SRS_GEN_HDR_FILES ${TERRALIB_SRS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_SRS_SRC_FILES} ${TERRALIB_SRS_HDR_FILES} ${TERRALIB_SRS_GEN_HDR_FILES})

  # st
  QT4_WRAP_UI(TERRALIB_ST_GEN_HDR_FILES ${TERRALIB_ST_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_ST_SRC_FILES} ${TERRALIB_ST_HDR_FILES} ${TERRALIB_ST_GEN_HDR_FILES})

  # table
  QT4_WRAP_UI(TERRALIB_TABLE_GEN_HDR_FILES ${TERRALIB_TABLE_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_TABLE_SRC_FILES} ${TERRALIB_TABLE_HDR_FILES} ${TERRALIB_TABLE_GEN_HDR_FILES})

  # temporal
  QT4_WRAP_UI(TERRALIB_TEMPORAL_GEN_HDR_FILES ${TERRALIB_TEMPORAL_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_TEMPORAL_SRC_FILES} ${TERRALIB_TEMPORAL_HDR_FILES} ${TERRALIB_TEMPORAL_GEN_HDR_FILES})

  # tools
  list(APPEND TERRALIB_FILES ${TERRALIB_TOOLS_SRC_FILES} ${TERRALIB_TOOLS_HDR_FILES})

  # utils
  QT4_WRAP_UI(TERRALIB_UTILS_GEN_HDR_FILES ${TERRALIB_UTILS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_UTILS_SRC_FILES} ${TERRALIB_UTILS_HDR_FILES} ${TERRALIB_UTILS_GEN_HDR_FILES})

  #
  # Qt4 moc'ing
  #

  # canvas
  set(TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/DrawLayerThread.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/EyeBirdMapDisplayWidget.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/MapDisplay.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/MultiThreadMapDisplay.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/OverlayWidget.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/ZoomInMapDisplayWidget.h)

  # charts
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ChartDisplay.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ChartDisplayWidget.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ChartLayerDialog.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ChartLayerWidget.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ChartProperties.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ChartStyleFrame.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ChartStyleWidget.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/HistogramDataWidget.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/HistogramDialog.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/HistogramFrame.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/HistogramStyleWidget.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ScatterDataWidget.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ScatterDialog.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ScatterFrame.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ScatterStyleWidget.h)

  # classification
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/ROIManagerDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/ROIManagerWidget.h)

  # colorbar
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/ColorBar.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/ColorCatalogWidget.h)

  # dataset
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/DataSetCategoryModel.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/DataSetExplorer.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/DataSetTreeModel.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/DataSetTreeView.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/DataSetSelectorDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/DataSetSelectorWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/DataSetSelectorWizardPage.h)

  # datasource
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/AbstractDataSourceTreeItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/CheckConstraintItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/ConstraintsItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/DataSetCategoryGroupItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/DataSetCategoryItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/DataSetGroupItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/DataSetItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/DataSourceExplorer.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/DataSourceItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/DataSourceToolBox.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/DataSourceTreeModel.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/DataSourceTreeView.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/ForeignKeyItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/IndexGroupItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/IndexItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/PrimaryKeyItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/PropertyGroupItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/PropertyItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/UniqueKeyItem.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/selector/DataSourceExplorerDialog.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/selector/DataSourceSelectorDialog.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/selector/DataSourceSelectorWidget.h
                                          ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/selector/DataSourceSelectorWizardPage.h)

  # exchanger
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/DataExchangerWizard.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/DataExchangeSummaryWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/DataSetOptionsWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/DirectExchangerDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/PostGIS2SHPDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/SHP2ADODialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/SHP2PostGISDialog.h)

  # externaltable
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/externalTable/DataPropertiesDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/externalTable/DataPropertiesWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/externalTable/FieldsDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/externalTable/TableLinkDialog.h)

  # help
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/help/AssistantHelpManagerImpl.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/help/HelpPushButton.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/help/HelpToolButton.h)

  # layer
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/AbstractTreeItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/ChartItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/ChartSliceItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/ColorMapItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/ColorMapSliceItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/DataSetAdapterLayerItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/DataSetLayerItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/FolderLayerItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/GroupingItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/GroupingSliceItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/LayerExplorer.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/LayerTreeModel.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/LayerTreeView.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/LegendItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/QueryLayerItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/RasterLayerItem.h)

  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/info/LayerPropertiesInfo.h)

  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/search/LayerSearchWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/search/LayerSearchWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/search/LayerSelectorWidget.h)

  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/selector/AbstractLayerSelector.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/selector/DataSetLayerSelector.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/selector/HiddenDataSetLayerSelector.h)
										 
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/utils/CompositionModeDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/utils/CompositionModeMenuWidget.h)

  # mapdisplay
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/mapdisplay/DataSetDisplay.h)

  # plugin
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/builder/PluginBuilderWizard.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/manager/PluginManagerDialog.h)

  # progress
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/progress/ProgressViewerBar.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/progress/ProgressViewerDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/progress/ProgressViewerWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/progress/ProgressWidgetItem.h)

  # property
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/ConstraintsIndexesListWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/ConstraintsIndexesPropertyDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/ConstraintWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/DataSetAdapterWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/DateTimePropertyWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/GeometryPropertyWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/IndexWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/NewPropertyDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/NewPropertyWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/NumericPropertyWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/PrimaryKeyConstraintWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/PropertyConverterDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/StringPropertyWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/UniquekeyConstraintWidget.h)

  # propertybrowser
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/propertybrowser/AbstractPropertyItem.h)

  # query
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/DataSetWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/DataSetWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/FieldsWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/GroupByWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/LayerAttributesWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/OrderByWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/OrderByWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/QueryBuilderWizard.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/QueryDataSourceDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/QueryDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/WhereClauseWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/WhereClauseWizardPage.h)

  # rp
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/raster/MultiResolutionDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/raster/MultiResolutionWidget.h)
  
  # rp
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/ArithmeticOpWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/ClassifierDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/ClassifierWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/ClippingWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/ColorTransformWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/ComposeBandsWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/ContrastDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/ContrastWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/FilterWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/FusionWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/MaskDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/MixtureModelDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/MixtureModelDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/MixtureModelWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/MosaicWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/OutputRasterParametersWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/RasterHistogramDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/RasterHistogramWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/RasterInfoWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/RasterInfoWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/RasterNavigatorDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/RasterNavigatorWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/RasterNavigatorWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/SegmenterDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/SegmenterWizardPage.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/TiePointLocatorDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/TiePointLocatorWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/TiePointLocatorParametersWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/TiePointsLocatorAdvancedDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/TiePointsLocatorDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/TiePointsLocatorDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/rp/VectorizationWizardPage.h)

  # se
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/AbstractGraphicWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/AbstractGraphicWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/BasicFillPropertyItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/BasicFillWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/BasicStrokePropertyItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/BasicStrokeWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/ChannelSelectionWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/CharMapWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/ColorMapWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/ContrastEnhancementWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/GlyphGraphicWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/GlyphMarkPropertyItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/GlyphMarkWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/GraphicFillWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/GraphicProperty.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/GraphicPropertyItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/GraphicSelectorWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/GroupingDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/GroupingWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/ImageOutlineWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/LineSymbolizerProperty.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/LineSymbolizerWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/LocalGraphicWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/LocalImagePropertyItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/LocalImageWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/OverlapBehaviorWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/PointSymbolizerProperty.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/PointSymbolizerWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/PolygonSymbolizerProperty.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/PolygonSymbolizerWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/RasterSymbolizerDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/RasterSymbolizerWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/SelectedChannelWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/ShadedReliefWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/StyleControllerWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/StyleDockWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/StyleExplorer.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/SymbolEditorWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/SymbolSelectorDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/SymbolTableWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/WellKnownGraphicWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/WellKnownMarkPropertyItem.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/se/WellKnownMarkWidget.h)

  # srs
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/srs/SRSManagerDialog.h)

  # st
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/Animation.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/AnimationItem.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/AnimationScene.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/AnimationView.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/PixmapItem.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/SliderPropertiesDialog.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/TrajectoryItem.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/TimeSliderWidget.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/TemporalPropertiesWidget.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/TimeSeriesPropertiesWidget.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/TimeSeriesPropertiesWizardPage.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/TimeSeriesWizard.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/TrajectoryPropertiesWidget.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/TrajectoryPropertiesWizardPage.h
                                           ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/st/TrajectoryWizard.h)


  # table
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/table/AddColumnDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/table/AlterDataDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/table/DataSetTableHorizontalHeader.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/table/DataSetTableVerticalHeader.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/table/DataSetTableView.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/table/RenameColumnDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/table/RetypeColumnDialog.h)

  # tools
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/tools/CoordTracking.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/tools/ExtentAcquire.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/tools/Info.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/tools/Measure.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/tools/PanExtent.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/tools/PointPicker.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/tools/PolygonAcquire.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/tools/ReadPixelTool.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/tools/Selection.h)

  # utils
  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ColorPickerToolButton.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/DoubleListWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/FileChooser.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/HorizontalSliderWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ListWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ParameterDialog.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ParameterTableWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ResourceChooser.h)

  # Moc'ing DataSetTableView.cpp
  QT4_GENERATE_MOC(${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/table/DataSetTableView.cpp DataSetTableView.moc)

  QT4_WRAP_CPP(TERRALIB_GEN_SRC_FILES ${TERRALIB_HDRS_TO_MOC_FILES} OPTIONS -DBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)

  add_library(terralib_mod_qt_widgets SHARED ${TERRALIB_FILES} ${TERRALIB_GEN_SRC_FILES} DataSetTableView.moc)

  target_link_libraries(terralib_mod_qt_widgets ${TERRALIB_LIBRARIES_DEPENDENCIES}
                                                ${QT_LIBRARIES})

endif()

set_target_properties(terralib_mod_qt_widgets
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 INSTALL_NAME_DIR "@executable_path/../lib")

install(TARGETS terralib_mod_qt_widgets
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets COMPONENT devel)

install(FILES ${TERRALIB_CANVAS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/canvas COMPONENT devel)

install(FILES ${TERRALIB_CHARTS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/charts COMPONENT devel)

install(FILES ${TERRALIB_CLASSIFICATION_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/classification COMPONENT devel)

install(FILES ${TERRALIB_COLORBAR_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/colorbar COMPONENT devel)

install(FILES ${TERRALIB_DATASET_EXPLORER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/dataset/explorer COMPONENT devel)

install(FILES ${TERRALIB_DATASET_SELECTOR_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/dataset/selector COMPONENT devel)

install(FILES ${TERRALIB_DATASOURCE_CONNECTOR_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/datasource/connector COMPONENT devel)

install(FILES ${TERRALIB_DATASOURCE_CORE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/datasource/core COMPONENT devel)

install(FILES ${TERRALIB_DATASOURCE_EXPLORER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/datasource/explorer COMPONENT devel)

install(FILES ${TERRALIB_DATASOURCE_SELECTOR_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/datasource/selector COMPONENT devel)

install(FILES ${TERRALIB_EXCHANGER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/exchanger COMPONENT devel)

install(FILES ${TERRALIB_EXTERNALTABLE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/externaltable COMPONENT devel)

install(FILES ${TERRALIB_HELP_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/help COMPONENT devel)

install(FILES ${TERRALIB_LAYER_EXPLORER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/layer/explorer COMPONENT devel)

install(FILES ${TERRALIB_LAYER_INFO_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/layer/info COMPONENT devel)

install(FILES ${TERRALIB_LAYER_SEARCH_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/layer/search COMPONENT devel)

install(FILES ${TERRALIB_LAYER_SELECTOR_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/layer/selector COMPONENT devel)

install(FILES ${TERRALIB_LAYER_UTILS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/layer/utils COMPONENT devel)

install(FILES ${TERRALIB_MAPDISPLAY_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/mapdisplay COMPONENT devel)

install(FILES ${TERRALIB_PLUGIN_BUILDER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/plugin/builder COMPONENT devel)

install(FILES ${TERRALIB_PLUGIN_MANAGER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/plugin/manager COMPONENT devel)

install(FILES ${TERRALIB_PROGRESS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/progress COMPONENT devel)

install(FILES ${TERRALIB_PROPERTY_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/property COMPONENT devel)

install(FILES ${TERRALIB_PROPERTY_BROWSER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/propertybrowser COMPONENT devel)

install(FILES ${TERRALIB_QUERY_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/query COMPONENT devel)

install(FILES ${TERRALIB_RASTER_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/raster COMPONENT devel)

install(FILES ${TERRALIB_RP_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/rp COMPONENT devel)

install(FILES ${TERRALIB_SE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/se COMPONENT devel)

install(FILES ${TERRALIB_SRS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/srs COMPONENT devel)

install(FILES ${TERRALIB_ST_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/st COMPONENT devel)

install(FILES ${TERRALIB_TABLE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/table COMPONENT devel)

install(FILES ${TERRALIB_TEMPORAL_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/temporal COMPONENT devel)

install(FILES ${TERRALIB_TOOLS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/tools COMPONENT devel)

install(FILES ${TERRALIB_UTILS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/widgets/utils COMPONENT devel)

export(TARGETS terralib_mod_qt_widgets APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
