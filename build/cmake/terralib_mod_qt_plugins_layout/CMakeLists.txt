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
#  Description: Build configuration for the Layout Plugin.
#
#  Author: Juliana Hohara de Souza Coelho <juliana.coelho@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})
include_directories(${QTPROPERTYBROWSER_INCLUDE_DIR})

if(WIN32)
  add_definitions(-DTEQTPLUGINLAYOUTDLL)
endif()

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/layout/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/layout/*.h)

if(Qt5_FOUND)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES})

  add_library(terralib_mod_qt_plugins_layout SHARED ${TERRALIB_FILES})

  target_link_libraries(terralib_mod_qt_plugins_layout terralib_mod_layout
                                                       terralib_mod_plugin
                                                       terralib_mod_qt_apf
                                                       terralib_mod_color
                                                       terralib_mod_common
                                                       terralib_mod_dataaccess
                                                       terralib_mod_datatype
                                                       terralib_mod_geometry
                                                       terralib_mod_maptools
                                                       terralib_mod_qt_widgets
													   ${QTPROPERTYBROWSER_LIBRARY})

  qt5_use_modules(terralib_mod_qt_plugins_layout Widgets)

else()
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  include(${QT_USE_FILE})
  include_directories(${QT_INCLUDE_DIR})
  add_definitions(${QT_DEFINITIONS})

# moc'ing
  set(TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/layout/LayoutEditorAction.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/layout/AbstractAction.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/layout/OutsideArea.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/layout/DisplayDock.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/layout/ObjectInspectorDock.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/layout/PropertiesDock.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/layout/EditTemplateDock.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/layout/ProxyProject.h)

  QT4_WRAP_CPP(TERRALIB_GEN_SRC_FILES ${TERRALIB_HDRS_TO_MOC_FILES} OPTIONS -DBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)

  set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES} ${TERRALIB_GEN_SRC_FILES})

  add_library(terralib_mod_qt_plugins_layout SHARED ${TERRALIB_FILES})

  target_link_libraries(terralib_mod_qt_plugins_layout terralib_mod_layout
                                                       terralib_mod_plugin
                                                       terralib_mod_qt_apf
                                                       ${QT_LIBRARIES}
													   ${QTPROPERTYBROWSER_LIBRARY})

endif()

set_target_properties(terralib_mod_qt_plugins_layout
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 INSTALL_NAME_DIR "@executable_path/../lib")

install(TARGETS terralib_mod_qt_plugins_layout
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/plugins/layout COMPONENT devel)

install(FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/plugins/te.qt.layout.teplg
        DESTINATION ${TERRALIB_DESTINATION_PLUGINS} COMPONENT runtime)

export(TARGETS terralib_mod_qt_plugins_layout APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
