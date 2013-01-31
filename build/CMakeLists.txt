cmake_minimum_required(VERSION 2.8)

set (PROJ_NAME "all_te")
project(${PROJ_NAME})

include (./terralib/TerraLib.cmake)
include (./terralib_qt_plugins/TerraLib_Plugins.cmake)

set(TE_CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(TE_CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(TE_CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")  

set (ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")

option (BUILD_TERRAVIEW "Build TerraView application?" ON)

if(BUILD_TERRAVIEW)
  set(TERRAVIEW_SPLASH_SCREEN_PIXMAP "${ROOT}/resources/images/terralib_logo.svg" CACHE STRING "File containing logo of the TerraLib")
  
  if(WIN32)
    set(TERRAVIEW_CONFIG_FILE "${TE_CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.xml" CACHE STRING "File containing main configurations of the TerraView application")
  else()
    set(TERRAVIEW_CONFIG_FILE "${TE_CMAKE_LIBRARY_OUTPUT_DIRECTORY}/config.xml" CACHE STRING "File containing main configurations of the TerraView application")
  endif()

  set(TERRAVIEW_RESOURCE_FILE "terraview.rcc" CACHE STRING "File containing resources description of the TerraView application")
  
  add_subdirectory (./terraview)
endif()

if(WIN32)
  message (STATUS "-- Generating run batch file.")
  if(_NOTFOUND)
    message (WARNING "Batch file may not contain paths to all searched dlls.")
  endif()
  
  # Release settings
  set (GDAL_BIN ${TE_DEPENDENCIES_DIR}/gdal/win32/release)
  generateRunningBatch(${T5_MODULES_PATH}/runme.bat.in runme.bat)
endif()  
