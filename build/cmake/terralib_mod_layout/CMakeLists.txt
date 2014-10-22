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
#  Description: Build configuration for the Print Layout Module.
#
#  Author: Juliana Hohara de Souza Coelho <juliana.coelho@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})
include_directories(${QTPROPERTYBROWSER_INCLUDE_DIR})

if(WIN32)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -DTELAYOUTDLL)
endif()

if(Qt5_FOUND)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

else() # QT4_FOUND

  include_directories(${CMAKE_CURRENT_BINARY_DIR})

  include(${QT_USE_FILE})

  include_directories(${QT_INCLUDE_DIR})

  add_definitions(${QT_DEFINITIONS})

endif()

file(GLOB TERRALIB_LAYOUT_CORE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_ENUM_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/enum/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_ENUM_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/enum/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_VISITOR_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/derivativevisitor/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_VISITOR_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/derivativevisitor/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_FACTORY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/factory/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_FACTORY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/factory/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_MVC_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/mvc/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_MVC_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/mvc/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_PROXY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/proxy/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_PROXY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/proxy/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_SINGLETON_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/singleton/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_SINGLETON_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/pattern/singleton/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_PROPERTY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/property/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_PROPERTY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/property/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_SERIALIZATION_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/serialization/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_SERIALIZATION_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/serialization/*.h)
file(GLOB TERRALIB_LAYOUT_CORE_TEMPLATE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/template/*.cpp)
file(GLOB TERRALIB_LAYOUT_CORE_TEMPLATE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/core/template/*.h)
file(GLOB TERRALIB_LAYOUT_ITEM_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/item/*.cpp)
file(GLOB TERRALIB_LAYOUT_ITEM_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/item/*.h)
file(GLOB TERRALIB_LAYOUT_OUTSIDE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/outside/*.cpp)
file(GLOB TERRALIB_LAYOUT_OUTSIDE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/outside/*.h)
file(GLOB TERRALIB_LAYOUT_QT_CORE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_CORE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/*.h)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_FACTORY_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/factory/*.h)
file(GLOB TERRALIB_LAYOUT_QT_COMMAND_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/command/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_COMMAND_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/pattern/command/*.h)
file(GLOB TERRALIB_LAYOUT_QT_TOOLS_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/tools/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_TOOLS_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/tools/*.h)
file(GLOB TERRALIB_LAYOUT_QT_ITEM_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/item/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_ITEM_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/item/*.h)
file(GLOB TERRALIB_LAYOUT_QT_OUTSIDE_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/*.cpp)
file(GLOB TERRALIB_LAYOUT_QT_OUTSIDE_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/*.h)
file(GLOB TERRALIB_LAYOUT_UI_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/ui/*.ui)

# datasource
source_group("Source Files\\core" FILES ${TERRALIB_LAYOUT_CORE_SRC_FILES})
source_group("Header Files\\core" FILES ${TERRALIB_LAYOUT_CORE_HDR_FILES})
source_group("Source Files\\core\\enum" FILES ${TERRALIB_LAYOUT_CORE_ENUM_SRC_FILES})
source_group("Header Files\\core\\enum" FILES ${TERRALIB_LAYOUT_CORE_ENUM_HDR_FILES})
source_group("Source Files\\core\\pattern\\visitor" FILES ${TERRALIB_LAYOUT_CORE_VISITOR_SRC_FILES})
source_group("Header Files\\core\\pattern\\visitor" FILES ${TERRALIB_LAYOUT_CORE_VISITOR_HDR_FILES})
source_group("Source Files\\core\\pattern\\factory" FILES ${TERRALIB_LAYOUT_CORE_FACTORY_SRC_FILES})
source_group("Header Files\\core\\pattern\\factory" FILES ${TERRALIB_LAYOUT_CORE_FACTORY_HDR_FILES})
source_group("Source Files\\core\\pattern\\mvc" FILES ${TERRALIB_LAYOUT_CORE_MVC_SRC_FILES})
source_group("Header Files\\core\\pattern\\mvc" FILES ${TERRALIB_LAYOUT_CORE_MVC_HDR_FILES})
source_group("Source Files\\core\\pattern\\proxy" FILES ${TERRALIB_LAYOUT_CORE_PROXY_SRC_FILES})
source_group("Header Files\\core\\pattern\\proxy" FILES ${TERRALIB_LAYOUT_CORE_PROXY_HDR_FILES})
source_group("Source Files\\core\\pattern\\singleton" FILES ${TERRALIB_LAYOUT_CORE_SINGLETON_SRC_FILES})
source_group("Header Files\\core\\pattern\\singleton" FILES ${TERRALIB_LAYOUT_CORE_SINGLETON_HDR_FILES})
source_group("Source Files\\core\\property" FILES ${TERRALIB_LAYOUT_CORE_PROPERTY_SRC_FILES})
source_group("Header Files\\core\\property" FILES ${TERRALIB_LAYOUT_CORE_PROPERTY_HDR_FILES})
source_group("Source Files\\core\\serialization" FILES ${TERRALIB_LAYOUT_CORE_SERIALIZATION_SRC_FILES})
source_group("Header Files\\core\\serialization" FILES ${TERRALIB_LAYOUT_CORE_SERIALIZATION_HDR_FILES})
source_group("Source Files\\core\\template" FILES ${TERRALIB_LAYOUT_CORE_TEMPLATE_SRC_FILES})
source_group("Header Files\\core\\template" FILES ${TERRALIB_LAYOUT_CORE_TEMPLATE_HDR_FILES})
source_group("Source Files\\item"   FILES ${TERRALIB_LAYOUT_ITEM_SRC_FILES})
source_group("Header Files\\item"   FILES ${TERRALIB_LAYOUT_ITEM_HDR_FILES})
source_group("Source Files\\outside"  FILES ${TERRALIB_LAYOUT_OUTSIDE_SRC_FILES})
source_group("Header Files\\outside"  FILES ${TERRALIB_LAYOUT_OUTSIDE_HDR_FILES})
source_group("Source Files\\qt\\core"  FILES ${TERRALIB_LAYOUT_QT_CORE_SRC_FILES})
source_group("Header Files\\qt\\core"  FILES ${TERRALIB_LAYOUT_QT_CORE_HDR_FILES})
source_group("Source Files\\qt\\core\\pattern\\factory"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_SRC_FILES})
source_group("Header Files\\qt\\core\\pattern\\factory"  FILES ${TERRALIB_LAYOUT_QT_FACTORY_HDR_FILES})
source_group("Source Files\\qt\\core\\pattern\\command"  FILES ${TERRALIB_LAYOUT_QT_COMMAND_SRC_FILES})
source_group("Header Files\\qt\\core\\pattern\\command"  FILES ${TERRALIB_LAYOUT_QT_COMMAND_HDR_FILES})
source_group("Source Files\\qt\\core\\tools"  FILES ${TERRALIB_LAYOUT_QT_TOOLS_SRC_FILES})
source_group("Header Files\\qt\\core\\tools"  FILES ${TERRALIB_LAYOUT_QT_TOOLS_HDR_FILES})
source_group("Source Files\\qt\\item"  FILES ${TERRALIB_LAYOUT_QT_ITEM_SRC_FILES})
source_group("Header Files\\qt\\item"  FILES ${TERRALIB_LAYOUT_QT_ITEM_HDR_FILES})
source_group("Source Files\\qt\\outside"  FILES ${TERRALIB_LAYOUT_QT_OUTSIDE_SRC_FILES})
source_group("Header Files\\qt\\outside"  FILES ${TERRALIB_LAYOUT_QT_OUTSIDE_HDR_FILES})
source_group("UI Files\\"  FILES ${TERRALIB_LAYOUT_UI_HDR_FILES})

set(TERRALIB_FILES ${TERRALIB_LAYOUT_CORE_SRC_FILES} ${TERRALIB_LAYOUT_CORE_HDR_FILES} ${TERRALIB_LAYOUT_CORE_ENUM_SRC_FILES} ${TERRALIB_LAYOUT_CORE_ENUM_HDR_FILES} ${TERRALIB_LAYOUT_CORE_VISITOR_SRC_FILES} ${TERRALIB_LAYOUT_CORE_VISITOR_HDR_FILES} ${TERRALIB_LAYOUT_CORE_FACTORY_SRC_FILES} ${TERRALIB_LAYOUT_CORE_FACTORY_HDR_FILES} ${TERRALIB_LAYOUT_CORE_MVC_SRC_FILES} ${TERRALIB_LAYOUT_CORE_MVC_HDR_FILES} ${TERRALIB_LAYOUT_CORE_PROXY_SRC_FILES} ${TERRALIB_LAYOUT_CORE_PROXY_HDR_FILES} ${TERRALIB_LAYOUT_CORE_SINGLETON_SRC_FILES} ${TERRALIB_LAYOUT_CORE_SINGLETON_HDR_FILES} ${TERRALIB_LAYOUT_CORE_PROPERTY_SRC_FILES} ${TERRALIB_LAYOUT_CORE_PROPERTY_HDR_FILES} ${TERRALIB_LAYOUT_CORE_SERIALIZATION_SRC_FILES} ${TERRALIB_LAYOUT_CORE_SERIALIZATION_HDR_FILES} ${TERRALIB_LAYOUT_CORE_TEMPLATE_SRC_FILES} ${TERRALIB_LAYOUT_CORE_TEMPLATE_HDR_FILES} ${TERRALIB_LAYOUT_ITEM_SRC_FILES} ${TERRALIB_LAYOUT_ITEM_HDR_FILES} ${TERRALIB_LAYOUT_OUTSIDE_SRC_FILES} ${TERRALIB_LAYOUT_OUTSIDE_HDR_FILES} ${TERRALIB_LAYOUT_QT_CORE_SRC_FILES} ${TERRALIB_LAYOUT_QT_CORE_HDR_FILES} ${TERRALIB_LAYOUT_QT_FACTORY_SRC_FILES} ${TERRALIB_LAYOUT_QT_FACTORY_HDR_FILES} ${TERRALIB_LAYOUT_QT_COMMAND_SRC_FILES} ${TERRALIB_LAYOUT_QT_COMMAND_HDR_FILES}  ${TERRALIB_LAYOUT_QT_TOOLS_SRC_FILES} ${TERRALIB_LAYOUT_QT_TOOLS_HDR_FILES} ${TERRALIB_LAYOUT_QT_ITEM_SRC_FILES} ${TERRALIB_LAYOUT_QT_ITEM_HDR_FILES} ${TERRALIB_LAYOUT_QT_OUTSIDE_SRC_FILES} ${TERRALIB_LAYOUT_QT_OUTSIDE_HDR_FILES})

if(Qt5_FOUND)

# uic'ing
  QT5_WRAP_UI(TERRALIB_LAYOUT_GEN_HDR_FILES ${TERRALIB_LAYOUT_UI_HDR_FILES})

  list(APPEND TERRALIB_FILES ${TERRALIB_LAYOUT_GEN_HDR_FILES})

  add_library(terralib_mod_layout SHARED ${TERRALIB_FILES})

  qt5_use_modules(terralib_mod_layout Widgets)

else()

# uic'ing
  QT4_WRAP_UI(TERRALIB_LAYOUT_GEN_HDR_FILES ${TERRALIB_LAYOUT_UI_HDR_FILES})

# moc'ing
  set(TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/Scene.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/PropertiesOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/ObjectInspectorOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/ToolbarOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/View.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/item/ObjectItem.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/item/MapItem.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/PropertyBrowser.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/VisualizationArea.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/item/MapGridItem.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/GridSettingsOutside.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/ObjectInspectorPropertyBrowser.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/PropertiesItemPropertyBrowser.h
                                 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/PageSetupOutside.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/SystematicScaleOutside.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/EditTemplateOutside.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/core/MenuItem.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/item/DefaultTextItem.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/item/TextItem.h
								 ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/layout/qt/outside/TextGridSettingsOutside.h)

  QT4_WRAP_CPP(TERRALIB_GEN_SRC_FILES ${TERRALIB_HDRS_TO_MOC_FILES} OPTIONS -DBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)

  list(APPEND TERRALIB_FILES ${TERRALIB_LAYOUT_GEN_HDR_FILES} ${TERRALIB_GEN_SRC_FILES})

  add_library(terralib_mod_layout SHARED ${TERRALIB_FILES})

  target_link_libraries(terralib_mod_layout ${QT_LIBRARIES})

endif() 

target_link_libraries(terralib_mod_layout  terralib_mod_color
                                           terralib_mod_common
                                           terralib_mod_dataaccess
                                           terralib_mod_datatype
                                           terralib_mod_geometry
                                           terralib_mod_maptools
                                           terralib_mod_memory
                                           terralib_mod_plugin
                                           terralib_mod_qt_widgets
                                           terralib_mod_srs
                                           terralib_mod_symbology)

set_target_properties(terralib_mod_layout
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 INSTALL_NAME_DIR "@executable_path/../lib")

install(TARGETS terralib_mod_layout
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_LAYOUT_CORE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_ENUM_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/enum COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_VISITOR_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/pattern/derivativevisitor COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_FACTORY_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/pattern/factory COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_MVC_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/pattern/mvc COMPONENT devel)
		
install(FILES ${TERRALIB_LAYOUT_CORE_PROXY_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/pattern/proxy COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_SINGLETON_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/pattern/singleton COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_PROPERTY_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/property COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_SERIALIZATION_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/serialization COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_CORE_TEMPLATE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/core/template COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_ITEM_HDR_FILES}        
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/item COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_OUTSIDE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/outside COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_CORE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_FACTORY_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/pattern/factory COMPONENT devel)
		
install(FILES ${TERRALIB_LAYOUT_QT_COMMAND_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/pattern/command COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_TOOLS_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/core/tools COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_ITEM_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/item COMPONENT devel)

install(FILES ${TERRALIB_LAYOUT_QT_OUTSIDE_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/layout/qt/outside COMPONENT devel)

export(TARGETS terralib_mod_layout APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
