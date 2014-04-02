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

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/*.h)
set(TERRALIB_HDRS_TO_MOC_FILES "")

set(TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_common")

file(GLOB TERRALIB_UTILS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/*.cpp)
file(GLOB TERRALIB_UTILS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/*.h)
file(GLOB TERRALIB_UTILS_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ui/*.ui)

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ColorPickerToolButton.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/DoubleListWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/FileChooser.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/HorizontalSliderWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ListWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ParameterDialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ParameterTableWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/utils/ResourceChooser.h)

if(TERRALIB_MOD_MAPTOOLS_ENABLED)
  file(GLOB TERRALIB_CANVAS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/*.cpp)
  file(GLOB TERRALIB_CANVAS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/*.h)
  file(GLOB TERRALIB_CANVAS_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/ui/*.ui)

  list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/DrawLayerThread.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/EyeBirdMapDisplayWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/MapDisplay.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/MultiThreadMapDisplay.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/OverlayWidget.h
                                         ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/canvas/ZoomInMapDisplayWidget.h)
endif()

#file(GLOB TERRALIB_CANVAS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/*.cpp)
#file(GLOB TERRALIB_CANVAS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/charts/*.h)

#file(GLOB TERRALIB_CANVAS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/*.cpp)
#file(GLOB TERRALIB_CANVAS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/classification/*.h)

#file(GLOB TERRALIB_CANVAS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/*.cpp)
#file(GLOB TERRALIB_CANVAS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/colorbar/*.h)

#file(GLOB TERRALIB_CANVAS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/*.cpp)
#file(GLOB TERRALIB_CANVAS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/dataset/*.h)

#file(GLOB TERRALIB_CANVAS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/*.cpp)
#file(GLOB TERRALIB_CANVAS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets/datasource/*.h)

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
