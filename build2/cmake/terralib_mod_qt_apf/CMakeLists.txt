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
#  Description: Build configuration for the Qt Application Framework Module.
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
set(TERRALIB_LIBRARIES_DEPENDENCIES "terralib_mod_qt_widgets")

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/*.h)
file(GLOB TERRALIB_UI_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/ui/*.ui)

source_group("Source Files"  FILES ${TERRALIB_SRC_FILES})
source_group("Header Files"  FILES ${TERRALIB_HDR_FILES})
source_group("UI Files"  FILES ${TERRALIB_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/ApplicationController.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/ApplicationPlugins.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/BaseApplication.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/ProjectInfoDialog.h)

#connectors
file(GLOB TERRALIB_CONNECTORS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/connectors/*.cpp)
file(GLOB TERRALIB_CONNECTORS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/connectors/*.h)

source_group("Source Files\\connectors"  FILES ${TERRALIB_CONNECTORS_SRC_FILES})
source_group("Header Files\\connectors"  FILES ${TERRALIB_CONNECTORS_HDR_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/connectors/ChartDisplayDockWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/connectors/DataSetTableDockWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/connectors/InterfaceController.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/connectors/LayerExplorer.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/connectors/LayoutEditor.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/connectors/MapDisplay.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/connectors/StyleExplorer.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/connectors/TimeSliderWidget.h)

#events
file(GLOB TERRALIB_EVENTS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/events/*.h)

source_group("Header Files\\events"  FILES ${TERRALIB_EVENTS_HDR_FILES})

#settings
file(GLOB TERRALIB_SETTINGS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/settings/*.cpp)
file(GLOB TERRALIB_SETTINGS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/settings/*.h)
file(GLOB TERRALIB_SETTINGS_UI_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/settings/ui/*.ui)

source_group("Source Files\\settings"  FILES ${TERRALIB_SETTINGS_SRC_FILES})
source_group("Header Files\\settings"  FILES ${TERRALIB_SETTINGS_HDR_FILES})
source_group("UI Files\\settings"  FILES ${TERRALIB_SETTINGS_UI_FILES})

list(APPEND TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/settings/AbstractSettingWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/settings/DisplayWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/settings/GeneralConfigWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/settings/MenuBarModel.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/settings/ProjectWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/settings/SettingsDialog.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/settings/TableWidget.h
                                       ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af/settings/ToolbarsWidget.h)

if(Qt5_FOUND)

# uic'ing
  QT5_WRAP_UI(TERRALIB_GEN_HDR_FILES ${TERRALIB_UI_FILES})
  QT5_WRAP_UI(TERRALIB_SETTINGS_GEN_HDR_FILES ${TERRALIB_SETTINGS_UI_FILES})

  set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES} ${TERRALIB_GEN_HDR_FILES}
                     ${TERRALIB_CONNECTORS_SRC_FILES} ${TERRALIB_CONNECTORS_HDR_FILES}
                     ${TERRALIB_EVENTS_HDR_FILES}
                     ${TERRALIB_SETTINGS_SRC_FILES} ${TERRALIB_SETTINGS_HDR_FILES} ${TERRALIB_SETTINGS_GEN_HDR_FILES})

  add_library(terralib_mod_qt_apf SHARED ${TERRALIB_FILES})

  target_link_libraries(terralib_mod_qt_apf ${TERRALIB_LIBRARIES_DEPENDENCIES})

  qt5_use_modules(terralib_mod_qt_apf Widgets)

else()

# uic'ing
  QT4_WRAP_UI(TERRALIB_GEN_HDR_FILES ${TERRALIB_UI_FILES})
  QT4_WRAP_UI(TERRALIB_SETTINGS_GEN_HDR_FILES ${TERRALIB_SETTINGS_UI_FILES})

# moc'ing
  QT4_WRAP_CPP(TERRALIB_GEN_SRC_FILES ${TERRALIB_HDRS_TO_MOC_FILES})

  set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES} ${TERRALIB_GEN_HDR_FILES}
                     ${TERRALIB_CONNECTORS_SRC_FILES} ${TERRALIB_CONNECTORS_HDR_FILES}
                     ${TERRALIB_EVENTS_HDR_FILES}
                     ${TERRALIB_SETTINGS_SRC_FILES} ${TERRALIB_SETTINGS_HDR_FILES} ${TERRALIB_SETTINGS_GEN_HDR_FILES}
                     ${TERRALIB_GEN_SRC_FILES})

  add_library(terralib_mod_qt_apf SHARED ${TERRALIB_FILES})

  target_link_libraries(terralib_mod_qt_apf ${TERRALIB_LIBRARIES_DEPENDENCIES}
                                            ${QT_LIBRARIES})

endif() 

set_target_properties(terralib_mod_qt_apf
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

install(TARGETS terralib_mod_qt_apf
        EXPORT terralib-targets
        RUNTIME DESTINATION bin COMPONENT runtime
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES} ${TERRALIB_GEN_HDR_FILES}
        DESTINATION include/terralib/qt/af COMPONENT devel)

export(TARGETS terralib_mod_qt_apf APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
