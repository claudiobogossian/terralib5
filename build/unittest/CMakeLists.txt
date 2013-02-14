cmake_minimum_required(VERSION 2.8)

set (PROJ_NAME "unittests")
project(${PROJ_NAME})

if(NOT "${TE_DEPENDENCIES_DIR}" STREQUAL "")
  set (CMAKE_FIND_ROOT_PATH ${TE_DEPENDENCIES_DIR})
endif()

find_package (terralib 5.0 COMPONENTS annotationtext color common dataaccess datatype filter 
  gdal geometry gml maptools memory ogr plugin postgis qt_widgets raster rp srs st stloader 
  symbology xerces xlink xml REQUIRED)

find_package (CppUnit REQUIRED)

set (TE_DEFAULT_LOGFILE_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Path to the folder that contains \"conf/te-log.conf\" file.")
set (TE_OUTPUT_REPORT_DIR "${CMAKE_CURRENT_BINARY_DIR}/reports" CACHE PATH "Path where the report files will be generated.")

############### Get the data used for testing the examples ###############
set (TE_DATA_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Path to data folder.")

if(NOT EXISTS ${TE_DATA_DIR}/data/report.xsl)
  message (STATUS "-- Installing data for examples.")
  file(DOWNLOAD "http://www.dpi.inpe.br/terralib5/data.zip" ${TE_DATA_DIR}/data.zip)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${TE_DATA_DIR}/data.zip WORKING_DIRECTORY ${TE_DATA_DIR})
  file (REMOVE ${TE_DATA_DIR}/data.zip)
  message (STATUS "-- Data files installed.")
endif()

set(TE_DATA_LOCALE "${TE_DATA_DIR}")
set(TE_DATA_UNITTEST_LOCALE ${TE_OUTPUT_REPORT_DIR})

if(NOT EXISTS ${TE_DATA_UNITTEST_LOCALE})
  file (MAKE_DIRECTORY ${TE_DATA_UNITTEST_LOCALE})
endif()

configure_file(${TE_DATA_DIR}/data/report.xsl ${TE_DATA_UNITTEST_LOCALE}/report.xsl COPYONLY)

add_definitions( 
  -DTE_DATA_LOCALE="${TE_DATA_LOCALE}" 
  -DTE_DATA_UNITTEST_LOCALE="${TE_DATA_UNITTEST_LOCALE}" 
)

include (${TE_MODULE_PATH}/GeneralConfig.cmake)

add_definitions(-DTE_PLUGINS_PATH=\"${TE_BIN_DIR}\")

# Go into sub-directories

add_subdirectory(terralib_unittest_common)
add_subdirectory(terralib_unittest_dataaccess)
add_subdirectory(terralib_unittest_datatype)
add_subdirectory(terralib_unittest_geometry)
add_subdirectory(terralib_unittest_memory)
add_subdirectory(terralib_unittest_qt_widgets)
add_subdirectory(terralib_unittest_rp)
add_subdirectory(terralib_unittest_raster)
add_subdirectory(terralib_unittest_sam)
add_subdirectory(terralib_unittest_srs)

# Verifying if some packag was not found.
get_cmake_property (_NOTFOUND PACKAGES_NOT_FOUND)

if(_NOTFOUND)
  message (WARNING "Set TE_DEPENDENCIES_DIR to search, for the 3rd-party libraries, first in this path.")
endif()

find_path ( 
  GDAL_DATADIR NAME compdcs.csv 
  PATH ${TE_DEPENDENCIES_DIR} 
  PATH_SUFFIXES gdal/data 
  DOC "Path to where the gdal data directory is located."
  )
  
if(WIN32)
  message (STATUS "-- Generating run batch file.")
  if(_NOTFOUND)
    message (WARNING "Batch file may not contain paths to all searched dlls.")
  endif()
  
  # Release settings
  set (TE_GDAL_BIN ${TE_DEPENDENCIES_DIR}/gdal/win32/release)
  generateRunningBatch(${TE_MODULE_PATH}/runme.bat.in runme.bat)
endif()