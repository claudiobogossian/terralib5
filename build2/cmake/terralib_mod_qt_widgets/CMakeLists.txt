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

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/*.h)

source_group("Source Files"  FILES ${TERRALIB_SRC_FILES})
source_group("Header Files"  FILES ${TERRALIB_HDR_FILES})

# Canvas files
if(TERRALIB_MOD_MAPTOOLS_ENABLED)
  file(GLOB TERRALIB_CANVAS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/*.cpp)
  file(GLOB TERRALIB_CANVAS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/*.h)
  file(GLOB TERRALIB_CANVAS_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/ui/*.ui)

  source_group("Source Files\\canvas"      FILES ${TERRALIB_CANVAS_SRC_FILES})
  source_group("Header Files\\canvas"      FILES ${TERRALIB_CANVAS_HDR_FILES})
  source_group("Header Files\\canvas\\ui"  FILES ${TERRALIB_CANVAS_UI_FILES})

  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/DrawLayerThread.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/EyeBirdMapDisplayWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/MapDisplay.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/MultiThreadMapDisplay.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/OverlayWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/ZoomInMapDisplayWidget.h)
endif()

# Charts files
file(GLOB TERRALIB_CHARTS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/*.cpp)
file(GLOB TERRALIB_CHARTS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/*.h)
file(GLOB TERRALIB_CHARTS_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/ui/*.ui)

source_group("Source Files\\charts"      FILES ${TERRALIB_CHARTS_SRC_FILES})
source_group("Header Files\\charts"      FILES ${TERRALIB_CHARTS_HDR_FILES})
source_group("Header Files\\charts\\ui"  FILES ${TERRALIB_CHARTS_UI_FILES})

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

# Classification Files
file(GLOB TERRALIB_CLASSIFICATION_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/*.cpp)
file(GLOB TERRALIB_CLASSIFICATION_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/*.h)
file(GLOB TERRALIB_CLASSIFICATION_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/ui/*.ui)

source_group("Source Files\\classification"      FILES ${TERRALIB_CLASSIFICATION_SRC_FILES})
source_group("Header Files\\classification"      FILES ${TERRALIB_CLASSIFICATION_HDR_FILES})
source_group("Header Files\\classification\\ui"  FILES ${TERRALIB_CLASSIFICATION_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/ROIManagerDialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/ROIManagerWidget.h)


# Color Bar Files
file(GLOB TERRALIB_COLORBAR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/*.cpp)
file(GLOB TERRALIB_COLORBAR_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/*.h)
file(GLOB TERRALIB_COLORBAR_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/ui/*.ui)

source_group("Source Files\\classification"      FILES ${TERRALIB_COLORBAR_SRC_FILES})
source_group("Header Files\\classification"      FILES ${TERRALIB_COLORBAR_HDR_FILES})
source_group("Header Files\\classification\\ui"  FILES ${TERRALIB_COLORBAR_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/ColorBar.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/ColorCatalogWidget.h)

# Dataset files
file(GLOB TERRALIB_DATASET_EXPLORER_SRC_FILES    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/*.cpp)
file(GLOB TERRALIB_DATASET_EXPLORER_HDR_FILES    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/*.h)
file(GLOB TERRALIB_DATASET_SELECTOR_SRC_FILES    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/*.cpp)
file(GLOB TERRALIB_DATASET_SELECTOR_HDR_FILES    ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/*.h)
file(GLOB TERRALIB_DATASET_SELECTOR_UI_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/ui/*.ui)

source_group("Source Files\\dataset\\explorer"      FILES ${TERRALIB_DATASET_EXPLORER_SRC_FILES})
source_group("Header Files\\dataset\\explorer"      FILES ${TERRALIB_DATASET_EXPLORER_HDR_FILES})
source_group("Header Files\\dataset\\selector"      FILES ${TERRALIB_DATASET_SELECTOR_SRC_FILES})
source_group("Header Files\\dataset\\selector"      FILES ${TERRALIB_DATASET_SELECTOR_HDR_FILES})
source_group("Header Files\\dataset\\selector\\ui"  FILES ${TERRALIB_DATASET_SELECTOR_UI_HDR_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/DataSetCategoryModel.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/DataSetExplorer.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/DataSetTreeModel.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/explorer/DataSetTreeView.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/DataSetSelectorDialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/DataSetSelectorWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/selector/DataSetSelectorWizardPage.h)

# Datasource files
# Exchanger files
# External Table files
# Help files
# Layer files
# Layout files
# Map Display files
# Plugin files
# Progress files
# Property files
# Property Browser files
# Query files
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

  if(TERRALIB_MOD_MAPTOOLS_ENABLED)
    QT5_WRAP_UI(TERRALIB_CANVAS_GEN_HDR_FILES ${TERRALIB_CANVAS_UI_FILES})

    list(APPEND TERRALIB_FILES ${TERRALIB_CANVAS_SRC_FILES} ${TERRALIB_CANVAS_HDR_FILES} ${TERRALIB_CANVAS_GEN_HDR_FILES})

    list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_maptools")
  endif()

  add_library(terralib_mod_qt_widgets SHARED ${TERRALIB_FILES})

  target_link_libraries(terralib_mod_qt_widgets ${TERRALIB_LIBRARIES_DEPENDENCIES})

  qt5_use_modules(terralib_mod_qt_widgets Widgets PrintSupport)

else() # Qt4

# uic'ing
  QT4_WRAP_UI(TERRALIB_UTILS_GEN_HDR_FILES ${TERRALIB_UTILS_UI_FILES})

  list(APPEND TERRALIB_FILES ${TERRALIB_UTILS_SRC_FILES} ${TERRALIB_UTILS_HDR_FILES} ${TERRALIB_UTILS_GEN_HDR_FILES})
  
  if(TERRALIB_MOD_MAPTOOLS_ENABLED)
    QT4_WRAP_UI(TERRALIB_CANVAS_GEN_HDR_FILES ${TERRALIB_CANVAS_UI_FILES})

    list(APPEND TERRALIB_FILES ${TERRALIB_CANVAS_SRC_FILES} ${TERRALIB_CANVAS_HDR_FILES} ${TERRALIB_CANVAS_GEN_HDR_FILES})

    list(APPEND TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_maptools")
  endif()

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
