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
#  Description: Build configuration for the TerraLib Edit Qt Module.
#
#  Author: Douglas Uba <douglas@dpi.inpe.br>
#

include_directories(${Boost_INCLUDE_DIR})

if(WIN32)
  add_definitions(-DTEEDITQTDLL)
endif()

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/*.h)

source_group("Source Files"  FILES ${TERRALIB_SRC_FILES})
source_group("Header Files"  FILES ${TERRALIB_HDR_FILES})

file(GLOB TERRALIB_TOOLS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/tools/*.cpp)
file(GLOB TERRALIB_TOOLS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/tools/*.h)

source_group("Source Files\\tools"  FILES ${TERRALIB_TOOLS_SRC_FILES})
source_group("Header Files\\tools"  FILES ${TERRALIB_TOOLS_HDR_FILES})

file(GLOB TERRALIB_UI_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/ui/*.ui)

file(GLOB TERRALIB_CORE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/core/*.cpp)
file(GLOB TERRALIB_CORE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/core/*.h)

source_group("Source Files\\core"  FILES ${TERRALIB_CORE_SRC_FILES})
source_group("Header Files\\core"  FILES ${TERRALIB_CORE_HDR_FILES})

file(GLOB TERRALIB_CORE_COMMAND_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/core/command/*.cpp)
file(GLOB TERRALIB_CORE_COMMAND_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/core/command/*.h)

source_group("Source Files\\core\\command"  FILES ${TERRALIB_CORE_COMMAND_SRC_FILES})
source_group("Header Files\\core\\command"  FILES ${TERRALIB_CORE_COMMAND_HDR_FILES})

if(Qt5_FOUND)

# uic'ing
  QT5_WRAP_UI(TERRALIB_GEN_HDR_FILES ${TERRALIB_UI_FILES})

  set(CMAKE_AUTOMOC ON)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES}
                     ${TERRALIB_TOOLS_SRC_FILES} ${TERRALIB_TOOLS_HDR_FILES}
                     ${TERRALIB_GEN_HDR_FILES} 
					 ${TERRALIB_CORE_SRC_FILES} ${TERRALIB_CORE_HDR_FILES}
					 ${TERRALIB_CORE_COMMAND_SRC_FILES} ${TERRALIB_CORE_COMMAND_HDR_FILES})

  add_library(terralib_mod_edit_qt SHARED ${TERRALIB_FILES})

  target_link_libraries(terralib_mod_edit_qt terralib_mod_edit_core
                                            terralib_mod_qt_widgets)

  qt5_use_modules(terralib_mod_edit_qt Widgets)

else() # QT4_FOUND

# uic'ing
  QT4_WRAP_UI(TERRALIB_GEN_HDR_FILES ${TERRALIB_UI_FILES})

  include_directories(${CMAKE_CURRENT_BINARY_DIR})

  include(${QT_USE_FILE})

  include_directories(${QT_INCLUDE_DIR})

  add_definitions(${QT_DEFINITIONS})

  # moc'ing
    set(TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/tools/AggregateAreaTool.h
								   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/tools/DeleteGeometryTool.h
								   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/tools/MergeGeometriesTool.h
								   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/tools/SubtractAreaTool.h
								   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/tools/VertexTool.h
                                   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/tools/CreatePolygonTool.h
                                   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/tools/CreateLineTool.h
                                   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/tools/MoveGeometryTool.h
                                   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/SnapOptionsDialog.h
								   ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/edit/qt/FeatureAttributesDialog.h)

    QT4_WRAP_CPP(TERRALIB_GEN_SRC_FILES ${TERRALIB_HDRS_TO_MOC_FILES})

   set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES}
                     ${TERRALIB_TOOLS_SRC_FILES} ${TERRALIB_TOOLS_HDR_FILES}
                     ${TERRALIB_GEN_HDR_FILES} ${TERRALIB_GEN_SRC_FILES}
					 ${TERRALIB_CORE_SRC_FILES} ${TERRALIB_CORE_HDR_FILES}
					 ${TERRALIB_CORE_COMMAND_SRC_FILES} ${TERRALIB_CORE_COMMAND_HDR_FILES})

    add_library(terralib_mod_edit_qt SHARED ${TERRALIB_FILES})

    target_link_libraries(terralib_mod_edit_qt terralib_mod_edit_core
                                              terralib_mod_qt_widgets
                                              ${QT_LIBRARIES})

endif()

set_target_properties(terralib_mod_edit_qt
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 INSTALL_NAME_DIR "@executable_path/../lib")

install(TARGETS terralib_mod_edit_qt
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES} ${TERRALIB_GEN_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/edit/qt COMPONENT devel)

install(FILES ${TERRALIB_TOOLS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/edit/qt/tools COMPONENT devel)

export(TARGETS terralib_mod_edit_qt APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
