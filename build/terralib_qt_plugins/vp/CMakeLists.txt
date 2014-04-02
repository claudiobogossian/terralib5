# This script builds the VP Qt Plugin
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(SRCDIR ${ROOT}/vp)                		# directory where the source files of the vp qt plugin are located
set(INCLDIR ${SRCDIR})                      # directory where the header files of the vp qt plugin are located

# Set the project name
set(PROJ_NAME "terralib_qt_plugin_vp")
project(${PROJ_NAME})         # the project name is terralib_qt_plugin_vp
set(LIB_NAME ${PROJ_NAME})    # the library name is terralib_qt_plugin_vp

find_package(Qt4 ${_Qt4_VERSION} REQUIRED COMPONENTS QtCore QtGui)
if(QT4_FOUND)
  include (${QT_USE_FILE})
  add_definitions (${QT_DEFINITIONS})
  set (DEP_INCLUDES ${QT_INCLUDE_DIR})
  set (DEP_LIBS ${QT_LIBRARIES})
endif()

find_package(Boost ${_Boost_VERSION} REQUIRED COMPONENTS system filesystem)
if(Boost_FOUND)
  list (APPEND DEP_INCLUDES ${Boost_INCLUDE_DIRS})
  list (APPEND DEP_LIBS ${Boost_LIBRARIES})
  add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)
endif()

# Select the header files for moc'ing
set (
  HDRS_TO_MOC
  ${SRCDIR}/AbstractAction.h
  ${SRCDIR}/AggregationAction.h
  ${SRCDIR}/BasicGeographicOpAction.h
  ${SRCDIR}/BufferAction.h
  ${SRCDIR}/IntersectionAction.h
  ${SRCDIR}/PolygonToLineAction.h
  ${SRCDIR}/SummarizationAction.h
  ${SRCDIR}/TransformationAction.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/plugins/vp" MOC)

list (APPEND DEP_INCLUDES ${TE_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
list (APPEND 
  DEP_LIBS 
  terralib_common
  terralib_dataaccess
  terralib_maptools
  terralib_plugin
  terralib_vp
  terralib_qt_af
)

file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${INCLDIR}/*.h)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

add_definitions(-DTEQTPLUGINVPDLL)

source_group("Generated Files" FILES ${MOC})

include_directories( ${DEP_INCLUDES} )
add_library (${PROJ_NAME} SHARED ${HDRS} ${SRCS} ${MOC})
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(${PROJ_NAME} ${DEP_LIBS})

#Exporting plug-in file
set (TE_PLUGIN_NAME "te.qt.vp")
set (TE_PLUGIN_DISPLAYNAME "Vector Processing")
set (TE_PLUGIN_DESCRIPTION "This plugin enables the terralib vector processing operations.")
set (TE_PLUGIN_LIBRARY "terralib_qt_plugin_vp")
set (TE_PLUGIN_HREFDOC "http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:designimplementation")
set (TE_PLUGIN_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TE_PLUGIN_NAME}.teplg")
getPluginResources("SharedLibraryName" "${TE_PLUGIN_LIBRARY}" RSC_LINK)
getPluginResources("LangFile" "pt.br" RSC_LINK2)
set (TE_PLUGIN_RESOURCES 
  "  <Resources>${RSC_LINK}  ${RSC_LINK2}\n  </Resources>\n"
)

#Exporting plugin information for the configuration file
set (TE_SCHEMA_LOCATION "${CMAKE_SOURCE_DIR}/../schemas/terralib")

set (TE_PLG_INFO "")
getPluginInfo("${TE_PLUGIN_NAME}" "${TE_PLUGIN_FILE}" TE_PLG_INFO)
set (TE_PLUGINS_DESCRIPTION "${TE_PLUGINS_DESCRIPTION}${TE_PLG_INFO}" PARENT_SCOPE)
  
configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${TE_PLUGIN_FILE})

#Exporting plugin information for the configuration file in install tree
set (TE_SCHEMA_LOCATION "../schemas/terralib")

set (TE_PLG_INFO "")
getPluginInfo("${TE_PLUGIN_NAME}" "./${TE_PLUGIN_NAME}.teplg" TE_PLG_INFO)
set (TE_INST_PLUGINS_DESCRIPTION "${TE_INST_PLUGINS_DESCRIPTION}${TE_PLG_INFO}" PARENT_SCOPE)
  
configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${CMAKE_BINARY_DIR}/toInstall/${TE_PLUGIN_NAME}.teplg)

if(WIN32)
  install(
    TARGETS ${PROJ_NAME}
    RUNTIME DESTINATION "bin" 
    COMPONENT PLUGINS
  )
else()
  install(
    TARGETS ${PROJ_NAME}
    ARCHIVE DESTINATION "lib" COMPONENT PLUGINS
    LIBRARY DESTINATION "lib" COMPONENT PLUGINS
  )
endif()

set (TE_ENABLED_PLUGINS "${TE_ENABLED_PLUGINS}\n    <Plugin>${TE_PLUGIN_NAME}</Plugin>" PARENT_SCOPE)
