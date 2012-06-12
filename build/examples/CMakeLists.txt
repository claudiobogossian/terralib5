cmake_minimum_required(VERSION 2.8)

set (PROJ_NAME "all")
project(${PROJ_NAME})

find_package (terralib 5.0 COMPONENTS annotationtext color common dataaccess datatype filter 
  gdal geometry gml maptools memory ogr plugin postgis qtwidgets raster rp srs st stloader 
  symbology xerces xlink xml REQUIRED)
  
############### Get the data used for testing the example ###############
set (DEFAULT_LOGFILE_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Path to the folder that contains \"conf/te-log.conf\" file.")
set (DATA_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Path to data folder.")

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
set(TE_LOGGER_DEFAULT_CONFIGURATION_FILE "${DEFAULT_LOGFILE_DIR}/conf/te-log.conf")

add_definitions( 
  -DTE_DATA_EXAMPLE_LOCALE="${TE_DATA_EXAMPLE_LOCALE}" 
  -DTE_LOGGER_DEFAULT_CONFIGURATION_FILE="${TE_LOGGER_DEFAULT_CONFIGURATION_FILE}"
)

include (${T5_MODULEPATH}/GeneralConfig.cmake)

option (EXAMPLE_COLORBAR "Build colorbar example." ON)  
option (EXAMPLE_COMMON "Build common example." ON)  
option (EXAMPLE_PLUGIN_HELLOWORLD "Build plugin hello world example." ON)  
option (EXAMPLE_DATAACCESS "Build data access example." ON)  
option (EXAMPLE_DATAGRID "Build data grid example." ON)  
option (EXAMPLE_DATASETMANAGEMENT "Build data set management example." ON)  
option (EXAMPLE_FACTORY "Build factory example." ON)  
option (EXAMPLE_GEOMETRY "Build geometry example." ON)  
option (EXAMPLE_LAYEREXPLORER "Build layer explorer example." ON)  
option (EXAMPLE_MAPTOOLS "Build map tools example." ON)  
option (EXAMPLE_PROPERTYMANAGEMENT "Build property management example." ON)  
option (EXAMPLE_QTCANVAS "Build qt canvas example." ON)  
option (EXAMPLE_RASTER "Build raster example." ON)  
option (EXAMPLE_RP "Build rp example." ON)  
option (EXAMPLE_SAM "Build sam example." ON)  
option (EXAMPLE_SRS "Build srs example." ON)  
option (EXAMPLE_ST "Build st example." ON)  
option (EXAMPLE_TIMESERIES "Build time series example." ON)  
option (EXAMPLE_URL "Build URL example." ON)  
option (EXAMPLE_XERCES "Build xerces example." ON)  
option (EXAMPLE_HELP "Build help example." ON)  
option (EXAMPLE_QT_RP "Build qt/rp example." ON)  
option (EXAMPLE_QT_SE "Build qt/se example." ON)  
option (EXAMPLE_QT_UTILS "Build qt/utils example." ON)  
  
# Go into sub-directories
if(EXAMPLE_COLORBAR)
  add_subdirectory(terralib_example_colorbar)
endif()

# Go into sub-directories
if(EXAMPLE_COMMON)
  add_subdirectory(terralib_example_common)
endif()

if(EXAMPLE_PLUGIN_HELLOWORLD)
  add_subdirectory(terralib_example_cppplugin_helloworld)
endif()

if(EXAMPLE_DATAACCESS)
  add_subdirectory(terralib_example_dataaccess)
endif()

if(EXAMPLE_DATAGRID)
  add_subdirectory(terralib_example_datagrid)
endif()

if(EXAMPLE_DATASETMANAGEMENT)
  add_subdirectory(terralib_example_datasetmanagement)
endif()  
 
if(EXAMPLE_FACTORY)
  add_subdirectory(terralib_example_factory)
endif()

if(EXAMPLE_GEOMETRY)
  add_subdirectory(terralib_example_geometry)
endif()

if(EXAMPLE_LAYEREXPLORER)
  add_subdirectory(terralib_example_layerexplorer)
endif()

if(EXAMPLE_MAPTOOLS)
  add_subdirectory(terralib_example_maptools)
endif()

if(EXAMPLE_PROGRESS)
  add_subdirectory(terralib_example_progress)
endif()
  
if(EXAMPLE_PROPERTYMANAGENT)
  add_subdirectory(terralib_example_propertymanagement)
endif()

if(EXAMPLE_QTCANVAS)
  add_subdirectory(terralib_example_qtcanvas)
endif()

if(EXAMPLE_RASTER)
  add_subdirectory(terralib_example_raster)
endif()

if(EXAMPLE_RP)
  add_subdirectory(terralib_example_rp)
endif()

if(EXAMPLE_SAM)
  add_subdirectory(terralib_example_sam)
endif()

if(EXAMPLE_SRS)
  add_subdirectory(terralib_example_srs)
endif()

if(EXAMPLE_ST)
  add_subdirectory(terralib_example_st)
endif()

if(EXAMPLE_TIMESERIES)
  add_subdirectory(terralib_example_timeseries)
endif()

if(EXAMPLE_URL)
  add_subdirectory(terralib_example_URL)
endif()

if(EXAMPLE_XERCES)
  add_subdirectory(terralib_example_xerces)
endif()

if(EXAMPLE_HELP)
  add_subdirectory(terralib_example_help)
endif()

if(EXAMPLE_QT_RP)
  add_subdirectory(terralib_example_qt/rp)
endif()

if(EXAMPLE_QT_SE)
  add_subdirectory(terralib_example_qt/se)
endif()

if(EXAMPLE_QT_UTILS)
  add_subdirectory(terralib_example_qt/utils)
endif()

if(WIN32)
  message (STATUS "-- Generating run batch file.")
  generateRunningBatch(runme.bat)
endif()