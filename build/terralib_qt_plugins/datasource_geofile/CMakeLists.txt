# This script builds the colorbar example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(SRCDIR ${ROOT}/datasource/geofile)                # directory where the source files of the colorbar example are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the colorbar example are located

# Set the project name
set(PROJ_NAME "terralib_qt_plugin_datasource_geofile")
project(${PROJ_NAME})         # the project name is terralib_example_colorbar
set(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_colorbar

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
  ${SRCDIR}/GeoFileConnectorDialog.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/plugins/datasource/geofile" MOC)

file ( GLOB
  FORMS ${SRCDIR}/ui/*.ui 
)

qt4_wrap_ui(UI ${FORMS})

list (APPEND DEP_INCLUDES ${TE_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
list (APPEND DEP_LIBS ${TE_LIBRARIES})

file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${INCLDIR}/*.h)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

add_definitions(-DTEQTPLUGINGEOFILEDLL)

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${MOC} ${UI})

#Exporting plug-in information for the configuration file
set (TEPLUGIN_NAME "te.qt.datasource.geofile")
set (TEPLUGIN_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/terralib_qt_plugin_datasource_geofile.xml")
set (PLG_INFO "")
getPluginInfo("${TEPLUGIN_NAME}" "${TEPLUGIN_FILE}" PLG_INFO)
set (TERRALIB_PLUGINS_DESCRIPTION "${TERRALIB_PLUGINS_DESCRIPTION}${PLG_INFO}" PARENT_SCOPE)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/plugin_info.xml.in" "${TEPLUGIN_FILE}")

include_directories( ${DEP_INCLUDES} )
add_library (${PROJ_NAME} SHARED ${HDRS} ${SRCS} ${UI} ${MOC})
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(${PROJ_NAME} ${DEP_LIBS})
                 