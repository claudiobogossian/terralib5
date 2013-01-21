cmake_minimum_required(VERSION 2.8)

set (PROJ_NAME "all_te")
project(${PROJ_NAME})

include (./terralib/TerraLib.cmake)
include (./terralib_qt_plugins/TerraLib_Plugins.cmake)

set(TE_CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(TE_CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(TE_CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")  

set (ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")

message ("root: ${ROOT}")

# Configurations to be used for applications based on TerraLib
set (TE_SCHEMA_LOCATION "${ROOT}/schemas/terralib" CACHE STRING "Location of the plugin_info.xsd file.")
set (TE_LOGO "${ROOT}/resources/themes/terralib/scalable/terralib_logo.svg" CACHE STRING "Location of the TerraLib logo image.")
set (TE_ABOUT_LOGO "${ROOT}/resources/themes/terralib/128x128/terralib_logo_128x128.png" CACHE STRING "Location of the about logo image.")
set (TERRALIB_INIT_FILES_LOCATION "${ROOT}/resources/init_files" CACHE STRING "Location of the TerraLib initialization files.")
set (TE_ICONS_THEME_PATH "${ROOT}/resources/themes" CACHE STRING "Location of the theme of icons available in TerraLib.")
set (TE_ICONS_THEME "terralib" CACHE STRING "Name of the theme of icons (by default we will use terralib theme).")
#-----------------------------------------------------------------------

add_subdirectory (./terraview)

if(WIN32)
  message (STATUS "-- Generating run batch file.")
  if(_NOTFOUND)
    message (WARNING "Batch file may not contain paths to all searched dlls.")
  endif()
  
  # Release settings
  set (GDAL_BIN ${TE_DEPENDENCIES_DIR}/gdal/win32/release)
  generateRunningBatch(${T5_MODULES_PATH}/runme.bat.in runme.bat)
endif()  
