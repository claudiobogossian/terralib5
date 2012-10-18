cmake_minimum_required(VERSION 2.8)

set (PROJ_NAME "examples")
project(${PROJ_NAME})

find_package (terralib 5.0 COMPONENTS annotationtext color common dataaccess datatype filter 
  gdal geometry gml maptools memory ogr plugin postgis qtwidgets raster rp srs st stloader 
  symbology xerces xlink xml REQUIRED)
 
if(NOT "${TE_DEPENDENCIES_DIR}" STREQUAL "")
  set (CMAKE_FIND_ROOT_PATH ${TE_DEPENDENCIES_DIR})
endif()

include (${T5_MODULEPATH}/GeneralConfig.cmake)

############### Get the data used for testing the example ###############
set (DATA_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Path to data folder.")

if(NOT EXISTS ${DATA_DIR}/data/report.xsl)
  message (STATUS "-- Installing data for examples.")
  
  file(DOWNLOAD "http://www.dpi.inpe.br/terralib5/data.zip" ${DATA_DIR}/data.zip)
                  
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf data.zip
                  WORKING_DIRECTORY ${DATA_DIR})

  file (REMOVE ${DATA_DIR}/data.zip)
  message (STATUS "-- Data files installed.")
endif()

set(TE_DATA_EXAMPLE_LOCALE "${DATA_DIR}")

add_definitions( 
  -DTE_DATA_EXAMPLE_LOCALE="${TE_DATA_EXAMPLE_LOCALE}" 
)

set (ICON_THEME "terralib" CACHE STRING "Defines the default theme to be used.")
set (ICON_THEME_PATH "" CACHE STRING "Lists of directories that contains icon themes.")

option (EXAMPLE_COLORBAR "Build colorbar example." ON)  
option (EXAMPLE_COMMON "Build common example." ON)  
option (EXAMPLE_PLUGIN_HELLOWORLD "Build plugin hello world example." ON)  
option (EXAMPLE_DATAACCESS "Build data access example." ON)  
option (EXAMPLE_DATAGRID "Build data grid example." ON)  
option (EXAMPLE_DATAVIEW "Build data view example." ON)  
option (EXAMPLE_DATASETMANAGEMENT "Build data set management example." ON)  
option (EXAMPLE_FACTORY "Build factory example." ON)  
option (EXAMPLE_GEOMETRY "Build geometry example." ON)  
option (EXAMPLE_LAYEREXPLORER "Build layer explorer example." ON)  
option (EXAMPLE_LEGEND "Build legend example." ON)  
option (EXAMPLE_MAPTOOLS "Build map tools example." ON)  
option (EXAMPLE_PROGRESS "Build task progress example." ON)  
option (EXAMPLE_PROPERTYMANAGEMENT "Build property management example." ON)  
option (EXAMPLE_RASTER "Build raster example." ON)  
option (EXAMPLE_RP "Build rp example." ON)  
option (EXAMPLE_SAM "Build sam example." ON)  
option (EXAMPLE_SRS "Build srs example." ON)  
option (EXAMPLE_ST "Build st example." ON)  
option (EXAMPLE_MYVIEWER "Build my viewer example." ON)  
option (EXAMPLE_URL "Build URL example." ON)  
option (EXAMPLE_XERCES "Build xerces example." ON)  
option (EXAMPLE_HELP "Build help example." ON)  
option (EXAMPLE_QT_RP "Build qt/rp example." ON)  
option (EXAMPLE_QT_SE "Build qt/se example." ON)  
option (EXAMPLE_QT_SRS "Build qt/srs example." ON)  
option (EXAMPLE_QT_UTILS "Build qt/utils example." ON)  
option (EXAMPLE_QT_CHARTS "Build qt/charts example." ON)  
option (EXAMPLE_QT_CANVAS "Build qt/canvas example." ON)
option (EXAMPLE_QT_TOOLS "Build qt/tools example." ON)
option (EXAMPLE_QT_DATASOURCE "Build qt/datasource example." ON)
  
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

if(EXAMPLE_DATAVIEW)
  add_subdirectory(terralib_example_dataview)
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

if(EXAMPLE_LEGEND)
  add_subdirectory(terralib_example_legend)
endif()

if(EXAMPLE_MAPTOOLS)
  add_subdirectory(terralib_example_maptools)
endif()

if(EXAMPLE_PROGRESS)
  add_subdirectory(terralib_example_progress)
endif()
  
if(EXAMPLE_PROPERTYMANAGEMENT)
  add_subdirectory(terralib_example_propertymanagement)
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

if(EXAMPLE_MYVIEWER)
  add_subdirectory(terralib_example_myviewer)
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

if(EXAMPLE_QT_SRS)
  add_subdirectory(terralib_example_qt/srs)
endif()

if(EXAMPLE_QT_UTILS)
  add_subdirectory(terralib_example_qt/utils)
endif()

if(EXAMPLE_QT_CHARTS)
  add_subdirectory(terralib_example_qt/charts)
endif()

if(EXAMPLE_QT_CANVAS)
  add_subdirectory(terralib_example_qt/canvas)
endif()

if(EXAMPLE_QT_TOOLS)
  add_subdirectory(terralib_example_qt/tools)
endif()

if(EXAMPLE_QT_DATASOURCE)
  add_subdirectory(terralib_example_qt/datasource)
endif()


# Verifying if some packag was not found.
get_cmake_property (_NOTFOUND PACKAGES_NOT_FOUND)

if(_NOTFOUND)
  message (WARNING "${_NOTFOUND}. Set TE_DEPENDENCIES_DIR to search, for the 3rd-party libraries, first in this path.")
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
  
  # Debug settings
  set (GDAL_BIN ${TE_DEPENDENCIES_DIR}/gdal/win32/debug)
  generateRunningBatch(${T5_MODULEPATH}/runme.bat.in runme.debug.bat)

  # Release settings
  set (GDAL_BIN ${TE_DEPENDENCIES_DIR}/gdal/win32/release)
  generateRunningBatch(${T5_MODULEPATH}/runme.bat.in runme.release.bat)
endif()