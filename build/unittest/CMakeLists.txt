cmake_minimum_required(VERSION 2.8)

set (PROJ_NAME "all")
project(${PROJ_NAME})

find_package (terralib 5.0 COMPONENTS annotationtext color common dataaccess datatype filter 
  gdal geometry gml maptools memory ogr plugin postgis qtwidgets raster rp srs st stloader 
  symbology xerces xlink xml REQUIRED)
  
find_package (CppUnit REQUIRED)

############### Get the data used for testing the example ###############
set (DATA_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Path to data folder.")
set (DEFAULT_LOGFILE_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Path to the folder that contains \"conf/te-log.conf\" file.")
set (TE_OUTPUT_REPORT_DIR "${CMAKE_CURRENT_BINARY_DIR}/reports" CACHE PATH "Path where the report files will be generated.")

if(NOT EXISTS ${DATA_DIR}/data/report.xsl)
  message (STATUS "-- Installing data for examples.")
  
  file(DOWNLOAD "http://www.dpi.inpe.br/terralib5/data.zip" ${DATA_DIR}/data.zip)
                  
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf data.zip
                  WORKING_DIRECTORY ${DATA_DIR})

  file (REMOVE ${DATA_DIR}/data.zip)
  message (STATUS "-- Data files installed.")
endif()

if(NOT EXISTS ${DEFAULT_LOGFILE_DIR}/conf/te-log.conf)
  message (STATUS "-- Copying ${DEFAULT_LOGCONF_FILE}.")
  configure_file(${DEFAULT_LOGCONF_FILE} "${DEFAULT_LOGFILE_DIR}/conf/te-log.conf" COPYONLY)
  message (STATUS "-- te-log.conf copied in \"${DEFAULT_LOGFILE_DIR}/conf\".")
endif()

set(TE_DATA_EXAMPLE_LOCALE "${DATA_DIR}/data")
set(TE_DATA_UNITTEST_LOCALE ${TE_OUTPUT_REPORT_DIR})

if(NOT EXISTS ${TE_DATA_UNITTEST_LOCALE})
  file (MAKE_DIRECTORY ${TE_DATA_UNITTEST_LOCALE})
endif()

add_definitions( 
  -DTE_DATA_EXAMPLE_LOCALE="${TE_DATA_EXAMPLE_LOCALE}" 
  -DTE_DATA_UNITTEST_LOCALE="${TE_DATA_UNITTEST_LOCALE}" 
  -DTE_LOGGER_DEFAULT_CONFIGURATION_FILE="${DEFAULT_LOGFILE_DIR}/conf/te-log.conf"
)

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