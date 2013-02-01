# This script builds the WCS Plugin
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(SRCDIR ${ROOT}/datasource/wcs)                # directory where the source files of the wcs plugin are located
set(INCLDIR ${SRCDIR})                 # directory where the header files of the wcs plugin are located

# Set the project name
set(PROJ_NAME "terralib_qt_plugin_datasource_wcs")
project(${PROJ_NAME})         # the project name is plugin_wcs
set(LIB_NAME ${PROJ_NAME})    # the library name is plugin_wcs

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
  ${SRCDIR}/WCSConnectorDialog.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/plugins/datasource/wcs" MOC)

file ( GLOB
  FORMS ${SRCDIR}/ui/*.ui 
)

qt4_wrap_ui(UI ${FORMS})

list (APPEND DEP_INCLUDES ${TE_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
list (APPEND 
  DEP_LIBS 
  terralib_common
  terralib_dataaccess
  terralib_plugin
  terralib_qt_widgets
)

file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${INCLDIR}/*.h)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

add_definitions(-DTEQTPLUGINWCSDLL)

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${MOC} ${UI})

include_directories( ${DEP_INCLUDES} )
add_library (${PROJ_NAME} SHARED ${HDRS} ${SRCS} ${UI} ${MOC})
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(${PROJ_NAME} ${DEP_LIBS})

#Exporting plug-in file
set (TEPLUGIN_NAME "te.qt.datasource.wcs")
set (TEPLUGIN_DISPLAYNAME "WCS Data Source Qt-Widgets")
set (TEPLUGIN_DESCRIPTION "This plugin enables widgets to create or open data managed by WCS driver.")
set (TEPLUGIN_LIBRARY "terralib_qt_plugin_datasource_gdal")
set (TEPLUGIN_HREFDOC "http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:designimplementation:dataaccess:wcs")
set (TEPLUGIN_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/terralib_qt_plugin_datasource_gdal.xml")
getPluginResources("SharedLibraryName" "${TEPLUGIN_LIBRARY}" RSC_LINK)
getPluginResources("LangFile" "pt.br" RSC_LINK2)
set (TEPLUGIN_RESOURCES 
  "  <Resources>${RSC_LINK}  ${RSC_LINK2}\n  </Resources>\n"
)
getPluginRequirements("te.da.wcs" REQ)
set (TEPLUGIN_REQUIREMENTS 
  "\n  <RequiredPlugins>${REQ}  </RequiredPlugins>\n"
)

#Exporting plug-in information for the configuration file
set (PLG_INFO "")
getPluginInfo("${TEPLUGIN_NAME}" "${TEPLUGIN_FILE}" PLG_INFO)
set (TERRALIB_PLUGINS_DESCRIPTION "${TERRALIB_PLUGINS_DESCRIPTION}${PLG_INFO}" PARENT_SCOPE)
  
configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${TEPLUGIN_FILE})