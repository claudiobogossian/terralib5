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

if(Qt5_FOUND)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

else() # QT4_FOUND

  include_directories(${CMAKE_CURRENT_BINARY_DIR})

  include(${QT_USE_FILE})

  include_directories(${QT_INCLUDE_DIR})

  add_definitions(${QT_DEFINITIONS})

endif()

set(TERRALIB_HDRS_TO_MOC_FILES "")

set(TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_common")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES ${QWT_LIBRARY})
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES ${QTPROPERTYBROWSER_LIBRARY})
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_classification")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_dataaccess")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_datatype")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_raster")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_geometry")
list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_maptools")

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/*.h)

source_group("Source Files"  FILES ${TERRALIB_SRC_FILES})
source_group("Header Files"  FILES ${TERRALIB_HDR_FILES})

# Canvas
file(GLOB TERRALIB_CANVAS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/*.cpp)
file(GLOB TERRALIB_CANVAS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/*.h)
file(GLOB TERRALIB_CANVAS_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/ui/*.ui)

source_group("Source Files\\canvas"  FILES ${TERRALIB_CANVAS_SRC_FILES})
source_group("Header Files\\canvas"  FILES ${TERRALIB_CANVAS_HDR_FILES})
source_group("UI Files\\canvas\\ui"  FILES ${TERRALIB_CANVAS_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/DrawLayerThread.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/EyeBirdMapDisplayWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/MapDisplay.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/MultiThreadMapDisplay.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/OverlayWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/ZoomInMapDisplayWidget.h)

# Charts
file(GLOB TERRALIB_CHARTS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/*.cpp)
file(GLOB TERRALIB_CHARTS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/*.h)
file(GLOB TERRALIB_CHARTS_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ui/*.ui)

source_group("Source Files\\charts"  FILES ${TERRALIB_CHARTS_SRC_FILES})
source_group("Header Files\\charts"  FILES ${TERRALIB_CHARTS_HDR_FILES})
source_group("UI Files\\charts\\ui"  FILES ${TERRALIB_CHARTS_UI_FILES})

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

# Classification
file(GLOB TERRALIB_CLASSIFICATION_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/*.cpp)
file(GLOB TERRALIB_CLASSIFICATION_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/*.h)
file(GLOB TERRALIB_CLASSIFICATION_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/ui/*.ui)

source_group("Source Files\\classification"  FILES ${TERRALIB_CLASSIFICATION_SRC_FILES})
source_group("Header Files\\classification"  FILES ${TERRALIB_CLASSIFICATION_HDR_FILES})
source_group("UI Files\\classification\\ui"  FILES ${TERRALIB_CLASSIFICATION_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/ROIManagerDialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/ROIManagerWidget.h)

# Color Bar
file(GLOB TERRALIB_COLORBAR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/*.cpp)
file(GLOB TERRALIB_COLORBAR_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/*.h)
file(GLOB TERRALIB_COLORBAR_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/ui/*.ui)

source_group("Source Files\\colorbar"  FILES ${TERRALIB_COLORBAR_SRC_FILES})
source_group("Header Files\\colorbar"  FILES ${TERRALIB_COLORBAR_HDR_FILES})
source_group("UI Files\\colorbar\\ui"  FILES ${TERRALIB_COLORBAR_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/ColorBar.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/ColorCatalogWidget.h)

# Dataset
file(GLOB TERRALIB_DATASET_EXPLORER_SRC_FILES    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/*.cpp)
file(GLOB TERRALIB_DATASET_EXPLORER_HDR_FILES    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/*.h)
file(GLOB TERRALIB_DATASET_SELECTOR_SRC_FILES    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/*.cpp)
file(GLOB TERRALIB_DATASET_SELECTOR_HDR_FILES    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/*.h)
file(GLOB TERRALIB_DATASET_SELECTOR_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/ui/*.ui)

source_group("Source Files\\dataset\\explorer"  FILES ${TERRALIB_DATASET_EXPLORER_SRC_FILES})
source_group("Header Files\\dataset\\explorer"  FILES ${TERRALIB_DATASET_EXPLORER_HDR_FILES})
source_group("Source Files\\dataset\\selector"  FILES ${TERRALIB_DATASET_SELECTOR_SRC_FILES})
source_group("Header Files\\dataset\\selector"  FILES ${TERRALIB_DATASET_SELECTOR_HDR_FILES})
source_group("UI Files\\dataset\\selector\\ui"  FILES ${TERRALIB_DATASET_SELECTOR_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/DataSetCategoryModel.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/DataSetExplorer.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/DataSetTreeModel.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/DataSetTreeView.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/DataSetSelectorDialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/DataSetSelectorWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/DataSetSelectorWizardPage.h)

# Datasource
file(GLOB TERRALIB_DATASOURCE_CONNECTOR_SRC_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/connector/*.cpp)
file(GLOB TERRALIB_DATASOURCE_CONNECTOR_HDR_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/connector/*.h)
file(GLOB TERRALIB_DATASOURCE_CORE_SRC_FILES       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/core/*.cpp)
file(GLOB TERRALIB_DATASOURCE_CORE_HDR_FILES       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/core/*.h)
file(GLOB TERRALIB_DATASOURCE_EXPLORER_SRC_FILES   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/*.cpp)
file(GLOB TERRALIB_DATASOURCE_EXPLORER_HDR_FILES   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/explorer/*.h)
file(GLOB TERRALIB_DATASOURCE_SELECTOR_SRC_FILES   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/selector/*.cpp)
file(GLOB TERRALIB_DATASOURCE_SELECTOR_HDR_FILES   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/selector/*.h)
file(GLOB TERRALIB_DATASOURCE_SELECTOR_UI_FILES    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/selector/ui/*.ui)

source_group("Source Files\\datasource\\connector" FILES ${TERRALIB_DATASOURCE_CONNECTOR_SRC_FILES})
source_group("Header Files\\datasource\\connector" FILES ${TERRALIB_DATASOURCE_CONNECTOR_HDR_FILES})
source_group("Source Files\\datasource\\core"      FILES ${TERRALIB_DATASOURCE_CORE_SRC_FILES})
source_group("Header Files\\datasource\\core"      FILES ${TERRALIB_DATASOURCE_CORE_HDR_FILES})
source_group("Source Files\\datasource\\core"      FILES ${TERRALIB_DATASOURCE_EXPLORER_SRC_FILES})
source_group("Header Files\\datasource\\core"      FILES ${TERRALIB_DATASOURCE_EXPLORER_HDR_FILES})
source_group("Source Files\\datasource\\selector"  FILES ${TERRALIB_DATASOURCE_SELECTOR_SRC_FILES})
source_group("Header Files\\datasource\\selector"  FILES ${TERRALIB_DATASOURCE_SELECTOR_HDR_FILES})
source_group("UI Files\\datasource\\selector\\ui"  FILES ${TERRALIB_DATASOURCE_SELECTOR_UI_FILES})

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

# Exchanger
file(GLOB TERRALIB_EXCHANGER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/*.cpp)
file(GLOB TERRALIB_EXCHANGER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/*.h)
file(GLOB TERRALIB_EXCHANGER_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/ui/*.ui)

source_group("Source Files\\exchanger"  FILES ${TERRALIB_EXCHANGER_SRC_FILES})
source_group("Header Files\\exchanger"  FILES ${TERRALIB_EXCHANGER_HDR_FILES})
source_group("UI Files\\exchanger\\ui"  FILES ${TERRALIB_EXCHANGER_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/DataExchangerWizard.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/DataExchangeSummaryWizardPage.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/DataSetOptionsWizardPage.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/DirectExchangerDialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/PostGIS2SHPDialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/SHP2ADODialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/exchanger/SHP2PostGISDialog.h)

# External Table
file(GLOB TERRALIB_EXTERNALTABLE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/externalTable/*.cpp)
file(GLOB TERRALIB_EXTERNALTABLE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/externalTable/*.h)
file(GLOB TERRALIB_EXTERNALTABLE_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/externalTable/ui/*.ui)

source_group("Source Files\\externalTable"  FILES ${TERRALIB_EXCHANGER_SRC_FILES})
source_group("Header Files\\externalTable"  FILES ${TERRALIB_EXCHANGER_HDR_FILES})
source_group("UI Files\\externalTable\\ui"  FILES ${TERRALIB_EXCHANGER_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/externalTable/DataPropertiesDialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/externalTable/DataPropertiesWidget.h)

# Help
file(GLOB TERRALIB_HELP_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/help/*.cpp)
file(GLOB TERRALIB_HELP_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/help/*.h)

source_group("Source Files\\help"  FILES ${TERRALIB_HELP_SRC_FILES})
source_group("Header Files\\help"  FILES ${TERRALIB_HELP_HDR_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/help/AssistantHelpManagerImpl.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/help/HelpPushButton.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/help/HelpToolButton .h)

# Layer
file(GLOB TERRALIB_LAYER_EXPLORER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/*.cpp)
file(GLOB TERRALIB_LAYER_EXPLORER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/*.h)

source_group("Source Files\\layer\\explorer"  FILES ${TERRALIB_LAYER_EXPLORER_SRC_FILES})
source_group("Header Files\\layer\\explorer"  FILES ${TERRALIB_LAYER_EXPLORER_HDR_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/explorer/AbstractTreeItem..h
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

file(GLOB TERRALIB_LAYER_INFO_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/info/*.cpp)
file(GLOB TERRALIB_LAYER_INFO_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/info/*.h)

source_group("Source Files\\layer\\info"  FILES ${TERRALIB_LAYER_INFO_SRC_FILES})
source_group("Header Files\\layer\\info"  FILES ${TERRALIB_LAYER_INFO_HDR_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/info/LayerPropertiesInfo.h)

file(GLOB TERRALIB_LAYER_SEARCH_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/search/*.cpp)
file(GLOB TERRALIB_LAYER_SEARCH_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/search/*.h)
file(GLOB TERRALIB_LAYER_SEARCH_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/search/ui/*.ui)

source_group("Source Files\\layer\\search"  FILES ${TERRALIB_LAYER_SEARCH_SRC_FILES})
source_group("Header Files\\layer\\search"  FILES ${TERRALIB_LAYER_SEARCH_HDR_FILES})
source_group("UI Files\\layer\\search\\ui"  FILES ${TERRALIB_LAYER_SEARCH_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/search/LayerSearchWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/search/LayerSearchWizardPage.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/search/LayerSelectorWidget.h)

file(GLOB TERRALIB_LAYER_SELECTOR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/selector/*.cpp)
file(GLOB TERRALIB_LAYER_SELECTOR_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/selector/*.h)

source_group("Source Files\\layer\\selector"  FILES ${TERRALIB_LAYER_SELECTOR_SRC_FILES})
source_group("Header Files\\layer\\selector"  FILES ${TERRALIB_LAYER_SELECTOR_HDR_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/selector/AbstractLayerSelector.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/selector/DataSetLayerSelector.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/selector/HiddenDataSetLayerSelector.h)

file(GLOB TERRALIB_LAYER_UTILS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/utils/*.cpp)
file(GLOB TERRALIB_LAYER_UTILS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layer/utils/*.h)

source_group("Source Files\\layer\\utils"  FILES ${TERRALIB_LAYER_UTILS_SRC_FILES})
source_group("Header Files\\layer\\utils"  FILES ${TERRALIB_LAYER_UTILS_HDR_FILES})

# Layout
file(GLOB TERRALIB_LAYOUT_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layout/*.cpp)
file(GLOB TERRALIB_LAYOUT_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layout/*.h)

source_group("Source Files\\layout"  FILES ${TERRALIB_LAYOUT_SRC_FILES})
source_group("Header Files\\layout"  FILES ${TERRALIB_LAYOUT_HDR_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/layout/DataFrame.h)

# Map Display
file(GLOB TERRALIB_MAPDISPLAY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/mapdisplay/*.cpp)
file(GLOB TERRALIB_MAPDISPLAY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/mapdisplay/*.h)

source_group("Source Files\\mapdisplay"  FILES ${TERRALIB_MAPDISPLAY_SRC_FILES})
source_group("Header Files\\mapdisplay"  FILES ${TERRALIB_MAPDISPLAY_HDR_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/mapdisplay/DataSetDisplay.h)

# Plugin
file(GLOB TERRALIB_PLUGIN_BUILDER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/builder/*.cpp)
file(GLOB TERRALIB_PLUGIN_BUILDER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/builder/*.h)
file(GLOB TERRALIB_PLUGIN_MANAGER_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/manager/*.cpp)
file(GLOB TERRALIB_PLUGIN_MANAGER_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/manager/*.h)

source_group("Source Files\\plugin\\builder"  FILES ${TERRALIB_PLUGIN_BUILDER_SRC_FILES})
source_group("Header Files\\plugin\\builder"  FILES ${TERRALIB_PLUGIN_BUILDER_HDR_FILES})
source_group("Source Files\\plugin\\manager"  FILES ${TERRALIB_PLUGIN_MANAGER_SRC_FILES})
source_group("Header Files\\plugin\\manager"  FILES ${TERRALIB_PLUGIN_MANAGER_HDR_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/builder/PluginBuilderWizard.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/plugin/manager/PluginManagerDialog.h)

# Progress files
file(GLOB TERRALIB_PROGRESS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/progress/*.cpp)
file(GLOB TERRALIB_PROGRESS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/progress/*.h)

source_group("Source Files\\progress"  FILES ${TERRALIB_PROGRESS_SRC_FILES})
source_group("Header Files\\progress"  FILES ${TERRALIB_PROGRESS_HDR_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/progress/ProgressViewerBar.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/progress/ProgressViewerDialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/progress/ProgressViewerWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/progress/ProgressWidgetItem.h)

# Property files
file(GLOB TERRALIB_PROPERTY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/*.cpp)
file(GLOB TERRALIB_PROPERTY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/*.h)
file(GLOB TERRALIB_PROPERTY_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/ui/*.ui)

source_group("Source Files\\property"  FILES ${TERRALIB_PROPERTY_SRC_FILES})
source_group("Header Files\\property"  FILES ${TERRALIB_PROPERTY_HDR_FILES})
source_group("UI Files\\property\\ui"  FILES ${TERRALIB_PROPERTY_UI_FILES})

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
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/.StringPropertyWidgeth
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/property/UniquekeyConstraintWidget.h)



# Property Browser files

# Query files
file(GLOB TERRALIB_QUERY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/*.cpp)
file(GLOB TERRALIB_QUERY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/*.h)
file(GLOB TERRALIB_QUERY_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/ui/*.ui)

source_group("Source Files\\query"  FILES ${TERRALIB_QUERY_SRC_FILES})
source_group("Header Files\\query"  FILES ${TERRALIB_QUERY_HDR_FILES})
source_group("UI Files\\query\\ui"  FILES ${TERRALIB_QUERY_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/DataSetWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/DataSetWizardPage.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/FieldsWizardPage.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/GroupByWizardPage.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/LayerAttributesWizardPage.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/OrderByWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/OrderByWizardPage.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/QueryBuilderWizard.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/QueryDialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/WhereClauseWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/query/WhereClauseWizardPage.h)



# Raster Processing files
# Symbology Encoding files
# Spatial Temporal files
# Table files
# Temporal files
# Tools files

# Utils files
file(GLOB TERRALIB_UTILS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/*.cpp)
file(GLOB TERRALIB_UTILS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/*.h)
file(GLOB TERRALIB_UTILS_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ui/*.ui)

source_group("Source Files\\utils"      FILES ${TERRALIB_UTILS_SRC_FILES})
source_group("Header Files\\utils"      FILES ${TERRALIB_UTILS_HDR_FILES})
source_group("Header Files\\utils\\ui"  FILES ${TERRALIB_UTILS_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ColorPickerToolButton.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/DoubleListWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/FileChooser.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/HorizontalSliderWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ListWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ParameterDialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ParameterTableWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ResourceChooser.h)


set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES})

if(Qt5_FOUND)

# uic'ing

  QT5_WRAP_UI(TERRALIB_UTILS_GEN_HDR_FILES ${TERRALIB_UTILS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_UTILS_SRC_FILES} ${TERRALIB_UTILS_HDR_FILES} ${TERRALIB_UTILS_GEN_HDR_FILES})

# Canvas
  QT5_WRAP_UI(TERRALIB_CANVAS_GEN_HDR_FILES ${TERRALIB_CANVAS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_CANVAS_SRC_FILES} ${TERRALIB_CANVAS_HDR_FILES} ${TERRALIB_CANVAS_GEN_HDR_FILES})

# Charts
  QT5_WRAP_UI(TERRALIB_CHARTS_GEN_HDR_FILES ${TERRALIB_CHARTS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_CHARTS_SRC_FILES} ${TERRALIB_CHARTS_HDR_FILES} ${TERRALIB_CHARTS_GEN_HDR_FILES})

# Classification
  QT5_WRAP_UI(TERRALIB_CLASSIFICATION_GEN_HDR_FILES ${TERRALIB_CLASSIFICATION_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_CLASSIFICATION_SRC_FILES} ${TERRALIB_CLASSIFICATION_HDR_FILES} ${TERRALIB_CLASSIFICATION_GEN_HDR_FILES})

# Color Bar
  QT5_WRAP_UI(TERRALIB_COLORBAR_GEN_HDR_FILES ${TERRALIB_COLORBAR_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_COLORBAR_SRC_FILES} ${TERRALIB_COLORBAR_HDR_FILES} ${TERRALIB_COLORBAR_GEN_HDR_FILES})

# Dataset - Explorer
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASET_EXPLORER_SRC_FILES} ${TERRALIB_DATASET_EXPLORER_HDR_FILES})

# Dataset - Selector
  QT5_WRAP_UI(TERRALIB_DATASET_SELECTOR_GEN_HDR_FILES ${TERRALIB_DATASET_SELECTOR_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASET_SELECTOR_SRC_FILES} ${TERRALIB_DATASET_SELECTOR_HDR_FILES} ${TERRALIB_DATASET_SELECTOR_GEN_HDR_FILES})

# Datasource - Connector
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_CONNECTOR_SRC_FILES} ${TERRALIB_DATASOURCE_CONNECTOR_HDR_FILES})

# Datasource - Core
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_CORE_SRC_FILES} ${TERRALIB_DATASOURCE_CORE_HDR_FILES})

# Datasource - Explorer
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_EXPLORER_SRC_FILES} ${TERRALIB_DATASOURCE_EXPLORER_HDR_FILES})

# Datasource - Selector
  QT5_WRAP_UI(TERRALIB_DATASOURCE_SELECTOR_GEN_HDR_FILES ${TERRALIB_DATASOURCE_SELECTOR_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_SELECTOR_SRC_FILES} ${TERRALIB_DATASOURCE_SELECTOR_HDR_FILES} ${TERRALIB_DATASOURCE_SELECTOR_GEN_HDR_FILES})

# Exchanger
  QT5_WRAP_UI(TERRALIB_EXCHANGER_GEN_HDR_FILES ${TERRALIB_EXCHANGER_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_EXCHANGER_SRC_FILES} ${TERRALIB_EXCHANGER_HDR_FILES} ${TERRALIB_EXCHANGER_GEN_HDR_FILES})

# External Table
  QT5_WRAP_UI(TERRALIB_EXTERNALTABLE_GEN_HDR_FILES ${TERRALIB_EXTERNALTABLE_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_EXTERNALTABLE_SRC_FILES} ${TERRALIB_EXTERNALTABLE_HDR_FILES} ${TERRALIB_EXTERNALTABLE_GEN_HDR_FILES})

# Help
  list(APPEND TERRALIB_FILES ${TERRALIB_HELP_SRC_FILES} ${TERRALIB_HELP_HDR_FILES})

# Layer - Explorer
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_EXPLORER_SRC_FILES} ${TERRALIB_LAYER_EXPLORER_HDR_FILES})

# Layer - Info
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_INFO_SRC_FILES} ${TERRALIB_LAYER_INFO_HDR_FILES})

# Layer - Search
  QT5_WRAP_UI(TERRALIB_LAYER_SEARCH_GEN_HDR_FILES ${TERRALIB_LAYER_SEARCH_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_SEARCH_SRC_FILES} ${TERRALIB_LAYER_SEARCH_HDR_FILES} ${TERRALIB_LAYER_SEARCH_GEN_HDR_FILES})

# Layer - Selector
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_SELECTOR_SRC_FILES} ${TERRALIB_LAYER_SELECTOR_HDR_FILES})

# Layer - Utils
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYER_UTILS_FILES} ${TERRALIB_LAYER_UTILS_HDR_FILES})
  
# Layout
  list(APPEND TERRALIB_FILES ${TERRALIB_LAYOUT_SRC_FILES} ${TERRALIB_LAYOUT_HDR_FILES})

# MapDisplay
  list(APPEND TERRALIB_FILES ${TERRALIB_MAPDISPLAY_SRC_FILES} ${TERRALIB_MAPDISPLAY_HDR_FILES})

# Plugin
  list(APPEND TERRALIB_FILES ${TERRALIB_PLUGIN_BUILDER_SRC_FILES} ${TERRALIB_PLUGIN_BUILDER_HDR_FILES} TERRALIB_PLUGIN_MANAGER_SRC_FILES TERRALIB_PLUGIN_MANAGER_HDR_FILES)

# Progress
  list(APPEND TERRALIB_FILES ${TERRALIB_PROGRESS_SRC_FILES} ${TERRALIB_PROGRESS_HDR_FILES})

# Property
  QT5_WRAP_UI(TERRALIB_PROPERTY_GEN_HDR_FILES ${TERRALIB_PROPERTY_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_PROPERTY_SRC_FILES} ${TERRALIB_PROPERTY_HDR_FILES} ${TERRALIB_PROPERTY_GEN_HDR_FILES})

# Query
  QT5_WRAP_UI(TERRALIB_QUERY_GEN_HDR_FILES ${TERRALIB_QUERY_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_QUERY_SRC_FILES} ${TERRALIB_QUERY_HDR_FILES} ${TERRALIB_QUERY_GEN_HDR_FILES})


  add_library(terralib_mod_qt_widgets SHARED ${TERRALIB_FILES})

  target_link_libraries(terralib_mod_qt_widgets ${TERRALIB_LIBRARIES_DEPENDENCIES})

  qt5_use_modules(terralib_mod_qt_widgets Widgets PrintSupport)

else() # Qt4

# uic'ing
  QT4_WRAP_UI(TERRALIB_UTILS_GEN_HDR_FILES ${TERRALIB_UTILS_UI_FILES})

  list(APPEND TERRALIB_FILES ${TERRALIB_UTILS_SRC_FILES} ${TERRALIB_UTILS_HDR_FILES} ${TERRALIB_UTILS_GEN_HDR_FILES})

# Canvas
  QT4_WRAP_UI(TERRALIB_CANVAS_GEN_HDR_FILES ${TERRALIB_CANVAS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_CANVAS_SRC_FILES} ${TERRALIB_CANVAS_HDR_FILES} ${TERRALIB_CANVAS_GEN_HDR_FILES})

# Charts
  QT4_WRAP_UI(TERRALIB_CHARTS_GEN_HDR_FILES ${TERRALIB_CHARTS_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_CHARTS_SRC_FILES} ${TERRALIB_CHARTS_HDR_FILES} ${TERRALIB_CHARTS_GEN_HDR_FILES})

# Classification
  QT4_WRAP_UI(TERRALIB_CLASSIFICATION_GEN_HDR_FILES ${TERRALIB_CLASSIFICATION_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_CLASSIFICATION_SRC_FILES} ${TERRALIB_CLASSIFICATION_HDR_FILES} ${TERRALIB_CLASSIFICATION_GEN_HDR_FILES})

# Color Bar
  QT4_WRAP_UI(TERRALIB_COLORBAR_GEN_HDR_FILES ${TERRALIB_COLORBAR_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_COLORBAR_SRC_FILES} ${TERRALIB_COLORBAR_HDR_FILES} ${TERRALIB_COLORBAR_GEN_HDR_FILES})

# Dataset - Explorer
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASET_EXPLORER_SRC_FILES} ${TERRALIB_DATASET_EXPLORER_HDR_FILES})

# Dataset - Selector
  QT4_WRAP_UI(TERRALIB_DATASET_SELECTOR_GEN_HDR_FILES ${TERRALIB_DATASET_SELECTOR_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASET_SELECTOR_SRC_FILES} ${TERRALIB_DATASET_SELECTOR_HDR_FILES} ${TERRALIB_DATASET_SELECTOR_GEN_HDR_FILES})

# Datasource - Connector
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_CONNECTOR_SRC_FILES} ${TERRALIB_DATASOURCE_CONNECTOR_HDR_FILES})

# Datasource - Core
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_CORE_SRC_FILES} ${TERRALIB_DATASOURCE_CORE_HDR_FILES})

# Datasource - Explorer
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_EXPLORER_SRC_FILES} ${TERRALIB_DATASOURCE_EXPLORER_HDR_FILES})

# Datasource - Selector
  QT4_WRAP_UI(TERRALIB_DATASOURCE_SELECTOR_GEN_HDR_FILES ${TERRALIB_DATASOURCE_SELECTOR_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_DATASOURCE_SELECTOR_SRC_FILES} ${TERRALIB_DATASOURCE_SELECTOR_HDR_FILES} ${TERRALIB_DATASOURCE_SELECTOR_GEN_HDR_FILES})

# Layout
list(APPEND TERRALIB_FILES ${TERRALIB_LAYOUT_SRC_FILES} ${TERRALIB_LAYOUT_HDR_FILES})

# MapDisplay
  list(APPEND TERRALIB_FILES ${TERRALIB_MAPDISPLAY_SRC_FILES} ${TERRALIB_MAPDISPLAY_HDR_FILES})

# Plugin
  list(APPEND TERRALIB_FILES ${TERRALIB_PLUGIN_BUILDER_SRC_FILES} ${TERRALIB_PLUGIN_BUILDER_HDR_FILES} TERRALIB_PLUGIN_MANAGER_SRC_FILES TERRALIB_PLUGIN_MANAGER_HDR_FILES)

# Progress
  list(APPEND TERRALIB_FILES ${TERRALIB_PROGRESS_SRC_FILES} ${TERRALIB_PROGRESS_HDR_FILES})

# Property
  QT4_WRAP_UI(TERRALIB_PROPERTY_GEN_HDR_FILES ${TERRALIB_PROPERTY_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_PROPERTY_SRC_FILES} ${TERRALIB_PROPERTY_HDR_FILES} ${TERRALIB_PROPERTY_GEN_HDR_FILES})

# Query
  QT4_WRAP_UI(TERRALIB_QUERY_GEN_HDR_FILES ${TERRALIB_QUERY_UI_FILES})
  list(APPEND TERRALIB_FILES ${TERRALIB_QUERY_SRC_FILES} ${TERRALIB_QUERY_HDR_FILES} ${TERRALIB_QUERY_GEN_HDR_FILES})

# moc'ing
  QT4_WRAP_CPP(TERRALIB_GEN_SRC_FILES ${TERRALIB_HDRS_TO_MOC_FILES})

  add_library(terralib_mod_qt_widgets SHARED ${TERRALIB_FILES})

  target_link_libraries(terralib_mod_qt_widgets ${TERRALIB_LIBRARIES_DEPENDENCIES}
                                                ${QT_LIBRARIES})

endif() 

set_target_properties(terralib_mod_qt_widgets
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

install(TARGETS terralib_mod_qt_widgets
        EXPORT terralib-targets
        RUNTIME DESTINATION bin COMPONENT runtime
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES}
        DESTINATION include/terralib/qt/widgets COMPONENT devel)

install(FILES ${TERRALIB_UTILS_HDR_FILES} ${TERRALIB_UTILS_GEN_HDR_FILES}
        DESTINATION include/terralib/qt/widgets/utils COMPONENT devel)

if(TERRALIB_MOD_MAPTOOLS_ENABLED)
  install(FILES ${TERRALIB_CANVAS_HDR_FILES} ${TERRALIB_CANVAS_GEN_HDR_FILES}
          DESTINATION include/terralib/qt/widgets/canvas COMPONENT devel)
endif()

export(TARGETS terralib_mod_qt_widgets APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
