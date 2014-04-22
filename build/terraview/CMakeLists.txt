# This script builds the datagrid example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")  # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terraview)             # directory where the source files of the datagrid example are located
set(INCLDIR ${SRCDIR})                            # directory where the header files of the datagrid example are located

# Set the project name
set(PROJ_NAME "terraview")
project(${PROJ_NAME})         # the project name is terralib_example_datagrid

find_package(Boost ${_Boost_VERSION} REQUIRED COMPONENTS system)

if(Boost_FOUND)
  list (APPEND _dep_incs ${Boost_INCLUDE_DIRS})
  list (APPEND _dep_libs ${Boost_LIBRARIES})
endif()

find_package(Qt4 ${_Qt4_VERSION} REQUIRED COMPONENTS QtCore QtGui)

if(QT4_FOUND)
  include (${QT_USE_FILE})
  add_definitions (${QT_DEFINITIONS})

  list (APPEND _dep_incs ${QT_INCLUDE_DIR})
  list (APPEND _dep_libs ${QT_LIBRARIES})
  
endif()

list (APPEND 
  _dep_incs 
  "${CMAKE_BINARY_DIR}" 
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${ROOT}/src"
  "${INCLDIR}" 
)

include_directories (${_dep_incs})

list (APPEND 
  _dep_libs 
  terralib_common
  terralib_qt_af
  terralib_qt_widgets
)

file (GLOB SRCS ${SRCDIR}/*.cpp)
file (GLOB HDRS ${INCLDIR}/*.h)
  
  # Select the FORMS widgets     
file (GLOB FORMS ${SRCDIR}/ui/*.ui)
  
  # Select the header files for moc'ing
file (
  GLOB
  HDRS_TO_MOC 
  ${INCLDIR}/TerraView.h
)
te_moc2("${HDRS_TO_MOC}" "terraview" MOC)
  
qt4_wrap_ui (UI ${FORMS})

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${UI} ${MOC})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TE_CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${TE_CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${TE_CMAKE_LIBRARY_OUTPUT_DIRECTORY}") 

get_filename_component(TVIEW_OUT_DIR "${TVIEW_CONFIG_FILE}" PATH)

set(TVIEW_TE_LOGO "${ROOT}/resources/images/png/terralib.png")
set(TVIEW_LOGO "${ROOT}/resources/images/png/terraview.png")
set(TVIEW_ICO "${ROOT}/resources/images/png/terralib-globe-small.png")
set(TVIEW_LARGE_LOGO "${ROOT}/resources/images/png/terraview-textual.png")
set(TVIEW_SPLASH_SCREEN_PIXMAP "${ROOT}/resources/images/png/terraview-splashscreen.png")

if(TE_WIN32_GUI)
  set (GUI_TYPE WIN32)
  list (APPEND _dep_libs ${QT_QTMAIN_LIBRARY})
elseif(APPLE)
  set (GUI_TYPE MACOSX_BUNDLE)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation "/")
  if(COREFOUNDATION_LIBRARY)
    list (APPEND _dep_libs ${COREFOUNDATION_LIBRARY})
  endif()
else()
  set (GUI_TYPE)
endif()

add_definitions (-DBOOST_ALL_NO_LIB)

add_executable(${PROJ_NAME} ${GUI_TYPE} ${SRCS} ${HDRS} ${UI} ${MOC})
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(${PROJ_NAME} ${_dep_libs})

foreach(_plg ${ENABLED_PLUGINS})
  add_dependencies(${PROJ_NAME} ${_plg})
endforeach()

message ("Generating configurations files in directory \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\".")
  
set (TE_DEFAULT_PROJECT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/default_project.xml")  
set (TE_DEFAULT_DATASOURCES_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/datasources.xml") 
 
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/TerraViewConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/TerraViewConfig.h)

install(
  TARGETS ${PROJ_NAME}
  RUNTIME DESTINATION "bin" COMPONENT BINARIES
  LIBRARY DESTINATION "bin" COMPONENT BINARIES
  ARCHIVE DESTINATION "bin" COMPONENT BINARIES
  BUNDLE DESTINATION . COMPONENT BINARIES
)

if(APPLE)
  file(
    WRITE
    ${CMAKE_BINARY_DIR}/terraview.sh
    "#!/bin/sh

  BUNDLE=\"`echo \"$0\" | sed -e 's/\\/Contents\\/MacOS\\/.*//'`\"
  EXECDIR=\"$BUNDLE/Contents/MacOS\"

  export \"TERRALIB_DIR=$EXECDIR/..\"

  export \"PATH=$PATH:${BUNDLE}/Contents/Plugins\"

  export \"QT_PLUGIN_PATH=$BUNDLE/Contents/QtPlugins\"

export DYLD_PRINT_LIBRARIES=1

  $EXECDIR/tview
  "
  )

  install (
    FILES ${CMAKE_BINARY_DIR}/terraview.sh
    DESTINATION ${TE_BUNDLE_APP}/MacOS
    COMPONENT BINARIES
  )
set (MACOSX_BUNDLE TRUE)

  set (MACOSX_BUNDLE_BUNDLE_NAME "terraview")
  set (MACOSX_BUNDLE_COPYRIGHT "Copyright 2001-2013")
  set (MACOSX_BUNDLE_SIGNATURE "TerraView")
  set (MACOSX_BUNDLE_GUI_IDENTIFIER www.terraview.org)
  set (MACOSX_BUNDLE_EXECUTABLE_NAME "terraview")
  set (MACOSX_BUNDLE_INFO_STRING "System for manipulating geographical data.")
  set (MACOSX_BUNDLE_ICON_FILE images/png/terralib-globe.icns)
  set (MACOSX_BUNDLE_LONG_VERSION_STRING TerraView-${TERRALIB_STRING_VERSION})
  set (MACOSX_BUNDLE_SHORT_VERSION_STRING TerraView5)
  set (MACOSX_BUNDLE_BUNDLE_VERSION ${TERRALIB_STRING_VERSION})

  configure_file(${CMAKE_ROOT}/Modules/MacOSXBundleInfo.plist.in ${CMAKE_BINARY_DIR}/Info.plist)

  set (CPACK_PACKAGE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/../resources/images/png/terralib-globe.icns)
  set (CPACK_BUNDLE_NAME "TerraView")
  set (CPACK_BUNDLE_ICON ${CPACK_PACKAGE_ICON})

  set (CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
  set (CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${TERRALIB_STRING_VERSION}")
  set (CPACK_BUNDLE_STARTUP_COMMAND ${CMAKE_BINARY_DIR}/terraview.sh)

  installQtPlugins("qgif;qico;qjpeg;qmng;qtiff;qsvg;qsvgicon;qsqlite")

  install (SCRIPT ${CMAKE_SOURCE_DIR}/modules/FixBundle.cmake COMPONENT PLUGINS)
elseif(UNIX)

  get_filename_component(_qt_lib_path ${QT_QTCORE_LIBRARY} PATH)

  file(
    WRITE
    ${CMAKE_BINARY_DIR}/terraview.sh
    "#!/bin/bash
TE_DIR=\"$(dirname $0)/..\"
EXECDIR=\"$TE_DIR/bin\"

export \"TERRALIB_DIR=$TE_DIR\"

export \"LD_LIBRARY_PATH=$TE_DIR/lib:${TE_DEPENDENCIES_DIR}/lib:${_qt_lib_path}:$TE_DIR/plugins\"

export \"QT_PLUGIN_PATH=${QT_PLUGINS_DIR}\"
$EXECDIR/tview
"
  )

  install (
    FILES ${CMAKE_BINARY_DIR}/terraview.sh
    DESTINATION bin
    COMPONENT BINARIES
  )

  install (
    SCRIPT ${CMAKE_SOURCE_DIR}/modules/ConfigBash.cmake
    COMPONENT BINARIES
  )
endif()

# install tree for config files
include (${CMAKE_CURRENT_SOURCE_DIR}/installConfig.cmake)
set (TE_DEFAULT_PROJECT "./default_project.xml")  
set (TE_DEFAULT_DATASOURCES_FILE "./datasources.xml") 

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/TerraViewConfig.h.in ${CMAKE_BINARY_DIR}/toInstall/TerraViewConfig.h)

if(APPLE AND WITH_BUNDLE)  
  file (
    WRITE
    ${CMAKE_BINARY_DIR}/qt.conf
    "[Paths]
  Binaries = MacOS"
  )

  install (
    FILES ${CMAKE_BINARY_DIR}/qt.conf
    DESTINATION ${TE_BUNDLE_APP}/Resources
    COMPONENT BINARIES
  )

  if(NOT "${TE_HELP_DIR}" STREQUAL "")
    install (
      DIRECTORY ${QT_PLUGINS_DIR}/../Assistant.app
      DESTINATION ${TE_BUNDLE_APP}/MacOS
      COMPONENT BINARIES
    )

    install (
      FILES /usr/lib/libQtCLucene.4.8.5.dylib
      DESTINATION ${TE_BUNDLE_APP}/Lib
      COMPONENT BINARIES
    )

    install (SCRIPT ${CMAKE_SOURCE_DIR}/modules/FixAssistant.cmake COMPONENT PLUGINS)  
  endif()
elseif(WIN32)
  file (
    WRITE
    ${CMAKE_BINARY_DIR}/qt.conf
    "[Paths]
  Binaries = .
  Plugins = plugins"
  )

  install (
    FILES ${CMAKE_BINARY_DIR}/qt.conf
    DESTINATION bin
    COMPONENT BINARIES
  )
endif()

today(dateTime_)

install ( CODE
  "file (
    WRITE
    ${CMAKE_BINARY_DIR}/.generated
    ${dateTime_})"
  COMPONENT BINARIES
)

install (
  FILES ${CMAKE_BINARY_DIR}/.generated
  DESTINATION .
  COMPONENT BINARIES
)
