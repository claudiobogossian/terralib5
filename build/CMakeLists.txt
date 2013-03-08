cmake_minimum_required(VERSION 2.8)

set (PROJ_NAME "all_te")
project(${PROJ_NAME})

set (TE_INCLUDE_DIRS "")
set (TE_INST_INCLUDE_DIRS "")
set (TE_MODULES "")

include (terralib/TerraLib.cmake)

set(TE_CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(TE_CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(TE_CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")  

include (terralib_qt_plugins/TerraLib_Plugins.cmake)

add_subdirectory (third-party/qt/propertybrowser)

set (ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")

option (BUILD_TERRAVIEW "Build TerraView application?" ON)

if(BUILD_TERRAVIEW)
  include (${CMAKE_CURRENT_SOURCE_DIR}/buildTree.cmake)

  if(WIN32)
    set(TVIEW_CONFIG_FILE "${TE_CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.xml" CACHE STRING "File containing main configurations of the TerraView application")
  else()
    set(TVIEW_CONFIG_FILE "${TE_CMAKE_LIBRARY_OUTPUT_DIRECTORY}/config.xml" CACHE STRING "File containing main configurations of the TerraView application")
  endif()

  set(TVIEW_RESOURCE_FILE "${CMAKE_CURRENT_BINARY_DIR}/help/help.qhc" CACHE STRING "File containing resources description of the TerraView application")
  
  include (${TE_MODULE_PATH}/GenerateQHelp.cmake)
  
  add_subdirectory (./terraview)

endif()

if(WIN32)
  message (STATUS "-- Generating run batch file.")
  if(_NOTFOUND)
    message (WARNING "Batch file may not contain paths to all searched dlls.")
  endif()
  
  # Release settings
  set (TE_GDAL_BIN ${TE_DEPENDENCIES_DIR}/gdal/win32/release)
  generateRunningBatch(${TE_MODULE_PATH}/runme.bat.in runme.bat)
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

install ( 
  FILES ${xml_config}
  DESTINATION bin
  COMPONENT BINARIES
)

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
