cmake_minimum_required(VERSION 2.8)

project(all)

set (T5_MODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Modules")

list (APPEND CMAKE_MODULE_PATH "${T5_MODULES_PATH}")

include (${T5_MODULES_PATH}/GeneralConfig.cmake)
include (${T5_MODULES_PATH}/UtilFunctions.cmake)

if(WIN32)
  include (${T5_MODULES_PATH}/WinConfig.cmake)
endif()

#-----------------------------------------------------------------------
option (BUILD_ANNOTATIONTEXT "Build annotation text module?" ON)
option (BUILD_COLOR "Build color module?" ON)
option (BUILD_COMMON "Build common module?" ON)
option (BUILD_DATAACCESS "Build data access module?" ON)
option (BUILD_DATATYPE "Build annotation text module?" ON)
option (BUILD_FILTER "Build filter module?" ON)
option (BUILD_GDAL "Build GDAL module?" ON)
option (BUILD_GEOMETRY "Build geometry module?" ON)
option (BUILD_GML "Build GML module?" ON)
option (BUILD_MAPTOOLS "Build map tools module?" ON)
option (BUILD_MEMORY "Build memory module?" ON)
option (BUILD_OGR "Build OGR module?" ON)
option (BUILD_PLUGIN "Build plug-in module?" ON)
option (BUILD_POSTGIS "Build PostGIS module?" ON)
option (BUILD_QTWIDGETS "Build Qt widgets module?" ON)
option (BUILD_RASTER "Build raster module?" ON)
option (BUILD_RP "Build raster processing module?" ON)
option (BUILD_SAM "Build SAM module?" ON)
option (BUILD_SRS "Build spatial reference system module?" ON)
option (BUILD_ST "Build spatial-temporal module?" ON)
option (BUILD_STLOADER "Build ST loader module?" ON)
option (BUILD_SYMBOLOGY "Build symbology module?" ON)
option (BUILD_XERCES "Build Xerces module?" ON)
option (BUILD_XLINK "Build XLink module?" ON)
option (BUILD_XML "Build XML module?" ON)

if(UNIX)
  option (BUILD_GRIB "Build GRIB module?" ON)
endif()

IF(BUILD_ANNOTATIONTEXT)
	add_subdirectory(terralib_annotationtext)
endif(BUILD_ANNOTATIONTEXT)

IF(BUILD_COLOR)
	add_subdirectory(terralib_color)
endif(BUILD_COLOR)

IF(BUILD_COMMON)
	add_subdirectory(terralib_common)
endif(BUILD_COMMON)

IF(BUILD_DATAACCESS)
	add_subdirectory(terralib_dataaccess)
endif(BUILD_DATAACCESS)

IF(BUILD_DATATYPE)
	add_subdirectory(terralib_datatype)
endif(BUILD_DATATYPE)

IF(BUILD_FILTER)
	add_subdirectory(terralib_filter)
endif(BUILD_FILTER)

IF(BUILD_GDAL)
	add_subdirectory(terralib_gdal)
endif(BUILD_GDAL)

IF(BUILD_GEOMETRY)
	add_subdirectory(terralib_geometry)
endif(BUILD_GEOMETRY)

IF(BUILD_GML)
	add_subdirectory(terralib_gml)
endif(BUILD_GML)

IF(BUILD_MAPTOOLS)
	add_subdirectory(terralib_maptools)
endif(BUILD_MAPTOOLS)

IF(BUILD_MEMORY)
	add_subdirectory(terralib_memory)
endif(BUILD_MEMORY)

IF(BUILD_OGR)
	add_subdirectory(terralib_ogr)
endif(BUILD_OGR)

IF(BUILD_PLUGIN)
	add_subdirectory(terralib_plugin)
endif(BUILD_PLUGIN)

IF(BUILD_POSTGIS)
	add_subdirectory(terralib_postgis)
endif(BUILD_POSTGIS)

IF(BUILD_QTWIDGETS)
	add_subdirectory(terralib_qtwidgets)
endif(BUILD_QTWIDGETS)

IF(BUILD_RASTER)
	add_subdirectory(terralib_raster)
endif(BUILD_RASTER)

IF(BUILD_RP)
	add_subdirectory(terralib_rp)
endif(BUILD_RP)

IF(BUILD_SAM)
	add_subdirectory(terralib_sam)
endif(BUILD_SAM)

IF(BUILD_SRS)
	add_subdirectory(terralib_srs)
endif(BUILD_SRS)

IF(BUILD_ST)
	add_subdirectory(terralib_st)
endif(BUILD_ST)

IF(BUILD_STLOADER)
	add_subdirectory(terralib_stloader)
endif(BUILD_STLOADER)

IF(BUILD_SYMBOLOGY)
	add_subdirectory(terralib_symbology)
endif(BUILD_SYMBOLOGY)

IF(BUILD_XERCES)
	add_subdirectory(terralib_xerces)
endif(BUILD_XERCES)

IF(BUILD_XLINK)
	add_subdirectory(terralib_xlink)
endif(BUILD_XLINK)

IF(BUILD_XML)
	add_subdirectory(terralib_xml)
endif(BUILD_XML)

IF(BUILD_GRIB)
  add_subdirectory(terralib_grib)
endif(BUILD_GRIB)
