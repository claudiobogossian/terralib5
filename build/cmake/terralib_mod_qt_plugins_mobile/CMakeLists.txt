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
#  Description: Build configuration for the Raster Processing Qt plugin.
#
#  Author: Eric Silva Abreu <eric@dpi.inpe.br>
#          Andre Gomes de Oliveira <andre.oliveira@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})
include_directories(${GDAL_INCLUDE_DIR})

if(WIN32)
  add_definitions(-DMOBILEDLL)
endif()

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/mobile/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/mobile/*.h)

source_group("Source Files"  FILES ${TERRALIB_SRC_FILES})
source_group("Header Files"  FILES ${TERRALIB_HDR_FILES})

file(GLOB TERRALIB_CORE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/mobile/core/*.cpp)
file(GLOB TERRALIB_CORE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/mobile/core/*.h)

source_group("Source Files\\core"  FILES ${TERRALIB_CORE_SRC_FILES})
source_group("Header Files\\core"  FILES ${TERRALIB_CORE_HDR_FILES})

file(GLOB TERRALIB_CORE_FORM_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/mobile/core/form/*.cpp)
file(GLOB TERRALIB_CORE_FORM_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/mobile/core/form/*.h)

source_group("Source Files\\core\\form"  FILES ${TERRALIB_CORE_FORM_SRC_FILES})
source_group("Header Files\\core\\form"  FILES ${TERRALIB_CORE_FORM_HDR_FILES})

file(GLOB TERRALIB_QT_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/mobile/qt/*.cpp)
file(GLOB TERRALIB_QT_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/mobile/qt/*.h)

source_group("Source Files\\qt"  FILES ${TERRALIB_QT_SRC_FILES})
source_group("Header Files\\qt"  FILES ${TERRALIB_QT_HDR_FILES})

file(GLOB TERRALIB_UI_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/mobile/qt/ui/*.ui)

source_group("UI Files"  FILES ${TERRALIB_UI_FILES})

if(Qt5_FOUND)
  # uic'ing
  QT5_WRAP_UI(TERRALIB_GEN_HDR_FILES ${TERRALIB_UI_FILES})

  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  set(TERRALIB_FILES     ${TERRALIB_SRC_FILES} 
				         ${TERRALIB_HDR_FILES} 
				         ${TERRALIB_CORE_SRC_FILES} 
				         ${TERRALIB_CORE_HDR_FILES}
                         ${TERRALIB_CORE_FORM_SRC_FILES} 
				         ${TERRALIB_CORE_FORM_HDR_FILES}
				         ${TERRALIB_QT_SRC_FILES} 
				         ${TERRALIB_QT_HDR_FILES}
				         ${TERRALIB_GEN_HDR_FILES})

  add_library(terralib_mod_qt_plugins_mobile SHARED ${TERRALIB_FILES})

  target_link_libraries(terralib_mod_qt_plugins_mobile terralib_mod_plugin terralib_mod_qt_apf terralib_mod_gdal ${GDAL_LIBRARY})

  qt5_use_modules(terralib_mod_qt_plugins_mobile Widgets)
endif()

set_target_properties(terralib_mod_qt_plugins_mobile
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 INSTALL_NAME_DIR "@executable_path/../lib")

install(TARGETS terralib_mod_qt_plugins_mobile
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES} ${TERRALIB_CORE_HDR_FILES} ${TERRALIB_QT_HDR_FILES} ${TERRALIB_GEN_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/qt/plugins/mobile COMPONENT devel)

install(FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/plugins/te.qt.mobile.teplg
        DESTINATION ${TERRALIB_DESTINATION_PLUGINS} COMPONENT runtime)

export(TARGETS terralib_mod_qt_plugins_mobile APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
