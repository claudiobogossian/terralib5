cmake_minimum_required(VERSION 2.8)

set (PROJ_NAME "all_te")
project(${PROJ_NAME})

set (TE_INCLUDE_DIRS "")
set (TE_INST_INCLUDE_DIRS "")
set (TE_MODULES "")

option (WITH_BUNDLE "Create a bundle?" ON)

if(WITH_BUNDLE)
  set (TE_APP_NAME "terraview" CACHE STRING "Name of the generated application.")
  if(APPLE)
    set (TE_BUNDLE_APP "${TE_APP_NAME}.app/Contents")
  endif()
endif()

include (terralib/TerraLib.cmake)

set(TE_CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(TE_CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(TE_CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")  

include (terralib_qt_plugins/TerraLib_Plugins.cmake)

add_subdirectory (third-party/qt/propertybrowser)

set (ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")

option (BUILD_TERRAVIEW "Build TerraView application?" ON)

set (TE_HELP_DIR "" CACHE STRING "Location of TerraView HELP FILES.")

if(BUILD_TERRAVIEW)
  include (${CMAKE_CURRENT_SOURCE_DIR}/buildTree.cmake)

  if(WIN32)
    set(TVIEW_CONFIG_FILE "${TE_CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.xml" CACHE STRING "File containing main configurations of the TerraView application")
  else()
    set(TVIEW_CONFIG_FILE "${TE_CMAKE_LIBRARY_OUTPUT_DIRECTORY}/config.xml" CACHE STRING "File containing main configurations of the TerraView application")
  endif()

  set(TVIEW_RESOURCE_FILE "${CMAKE_CURRENT_BINARY_DIR}/help/help.qhc" CACHE STRING "File containing resources description of the TerraView application")
  
  add_subdirectory (./terraview)

  if (NOT "${TE_HELP_DIR}" STREQUAL "") 
	include (${TE_MODULE_PATH}/GenerateQHelp.cmake)
  endif()
  
  add_dependencies (terraview "Generate QHelp")

endif()

if(WIN32)
  message (STATUS "-- Generating run batch file.")
  if(_NOTFOUND)
    message (WARNING "Batch file may not contain paths to all searched dlls.")
  endif()
  
  # Release settings
  set (TE_GDAL_BIN ${TE_DEPENDENCIES_DIR}/gdal/win32/release)
  generateRunningBatch(${TE_MODULE_PATH}/runme.bat.in runme.bat)

  include (${TE_MODULE_PATH}/ThirdParty.cmake)

endif()

file(
  GLOB xml_config
  FILES ${CMAKE_BINARY_DIR}/toInstall/*.xml
)

file(
  GLOB head_config
  FILES ${CMAKE_BINARY_DIR}/toInstall/*.h
)

file(
  GLOB script_config
  FILES ${CMAKE_BINARY_DIR}/toInstall/*.cmake
)

file(
  GLOB plg_config
  FILES ${CMAKE_BINARY_DIR}/toInstall/*.teplg
)

if(APPLE AND WITH_BUNDLE)
  install ( 
    FILES ${xml_config}
    DESTINATION ${TE_BUNDLE_APP}/Plugins
    COMPONENT BINARIES
  )
else()
  install ( 
    FILES ${xml_config}
    DESTINATION bin
    COMPONENT BINARIES
  )
endif()

install ( 
  FILES ${head_config}
  DESTINATION terralib
  COMPONENT HEADERS
)

install ( 
  FILES ${script_config}
  DESTINATION modules
  COMPONENT CMAKE
)

if(APPLE AND WITH_BUNDLE)
  install ( 
    FILES ${plg_config}
    DESTINATION ${TE_BUNDLE_APP}/Plugins
    COMPONENT PLUGINS
  )
endif()

# uninstall target
if( UNIX AND NOT APPLE)
  configure_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
      "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
      IMMEDIATE @ONLY)

  add_custom_target(uninstall
      COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif()
