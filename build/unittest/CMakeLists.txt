cmake_minimum_required(VERSION 2.8)

set (PROJ_NAME "all")
project(${PROJ_NAME})

############### Get the data used for testing the example ###############
set (DATA_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Path to data folder.")

if(NOT EXISTS ${DATA_DIR}/data/report.xsl)
  message (STATUS "-- Installing data for examples.")
  
  file(DOWNLOAD "http://www.dpi.inpe.br/terralib5/data.tar.gz" ${DATA_DIR}/data.tar.gz)
                  
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf data.tar.gz
                  WORKING_DIRECTORY ${DATA_DIR})

  message (STATUS "-- Data files installed.")
endif()

set(TE_DATA_EXAMPLE_LOCALE "${DATA_DIR}/data")

add_definitions( -DTE_DATA_EXAMPLE_LOCALE="${TE_DATA_EXAMPLE_LOCALE}" )

find_package (terralib 5.0 COMPONENTS annotationtext color common dataaccess datatype filter 
  gdal geometry gml maptools memory ogr plugin postgis qtwidgets raster rp srs st stloader 
  symbology xerces xlink xml REQUIRED)
  
find_package (CppUnit REQUIRED)

include (${T5_MODULEPATH}/GeneralConfig.cmake)

# Go into sub-directories

add_subdirectory(terralib_unittest_common)
add_subdirectory(terralib_unittest_dataaccess)
add_subdirectory(terralib_unittest_datatype)
add_subdirectory(terralib_unittest_geometry)
add_subdirectory(terralib_unittest_qtwidgets)
add_subdirectory(terralib_unittest_rp)
add_subdirectory(terralib_unittest_sam)
add_subdirectory(terralib_unittest_srs)

if(WIN32)
  message (STATUS "-- Generating run batch file.")
  generateRunningBatch(runme.bat)
endif()