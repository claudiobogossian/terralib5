cmake_minimum_required(VERSION 2.8)

set (PROJ_NAME "terralib")
project(${PROJ_NAME})

set (TE_VERSION "5.0.0")
set (T5_MODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Modules")

list (APPEND CMAKE_MODULE_PATH "${T5_MODULES_PATH}" )

set (TE_DEPENDENCIES_DIR "$ENV{TE_DEPENDENCIES_DIR}" CACHE PATH "Find the directory containing terralib5 3rd-party dependencies.")

include (${T5_MODULES_PATH}/GeneralConfig.cmake)
include (${T5_MODULES_PATH}/UtilFunctions.cmake)

if(WIN32)
  include (${T5_MODULES_PATH}/WinConfig.cmake)
endif()

add_definitions (-DBOOST_FILESYSTEM_VERSION=3)

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

if(WIN32)
  option (BUILD_ADO "Build ADO module?" ON)
endif()

IF(BUILD_ANNOTATIONTEXT)
	add_subdirectory(terralib_annotationtext)
endif()

IF(BUILD_COLOR)
	add_subdirectory(terralib_color)
endif()

IF(BUILD_COMMON)
	add_subdirectory(terralib_common)
endif()

IF(BUILD_DATAACCESS)
	add_subdirectory(terralib_dataaccess)
endif()

IF(BUILD_DATATYPE)
	add_subdirectory(terralib_datatype)
endif()

IF(BUILD_FILTER)
	add_subdirectory(terralib_filter)
endif()

IF(BUILD_GDAL)
	add_subdirectory(terralib_gdal)
endif()

IF(BUILD_GEOMETRY)
	add_subdirectory(terralib_geometry)
endif()

IF(BUILD_GML)
	add_subdirectory(terralib_gml)
endif()

IF(BUILD_MAPTOOLS)
	add_subdirectory(terralib_maptools)
endif()

IF(BUILD_MEMORY)
	add_subdirectory(terralib_memory)
endif()

IF(BUILD_OGR)
	add_subdirectory(terralib_ogr)
endif()

IF(BUILD_PLUGIN)
	add_subdirectory(terralib_plugin)
endif()

IF(BUILD_POSTGIS)
	add_subdirectory(terralib_postgis)
endif()

IF(BUILD_QTWIDGETS)
	add_subdirectory(terralib_qtwidgets)
endif()

IF(BUILD_RASTER)
	add_subdirectory(terralib_raster)
endif()

IF(BUILD_RP)
	add_subdirectory(terralib_rp)
endif()

IF(BUILD_SAM)
	add_subdirectory(terralib_sam)
endif()

IF(BUILD_SRS)
	add_subdirectory(terralib_srs)
endif()

IF(BUILD_ST)
	add_subdirectory(terralib_st)
endif()

IF(BUILD_STLOADER)
	add_subdirectory(terralib_stloader)
endif()

IF(BUILD_SYMBOLOGY)
	add_subdirectory(terralib_symbology)
endif()

IF(BUILD_XERCES)
	add_subdirectory(terralib_xerces)
endif()

IF(BUILD_XLINK)
	add_subdirectory(terralib_xlink)
endif()

IF(BUILD_XML)
	add_subdirectory(terralib_xml)
endif()

IF(BUILD_GRIB)
  add_subdirectory(terralib_grib)
endif()

IF(BUILD_ADO)
  add_subdirectory(terralib_ado)
endif()

# Verifying if some packag was not found.
get_cmake_property (_NOTFOUND PACKAGES_NOT_FOUND)

if(_NOTFOUND)
  message (WARNING "Set TE_DEPENDENCIES_DIR to search, for the 3rd-party libraries, first in this path.")
endif()

# Generating cmake configuration files
message(STATUS "-- Generating configuration files.")

get_cmake_property(TE_3DS PACKAGES_FOUND)
set (_TE_INCLUDE_DIRS ${ROOT}/src "${_TE_INCLUDE_DIRS}")

configure_file (terralibConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/terralibConfig.cmake @ONLY)
configure_file (terralibConfigVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/terralibConfigVersion.cmake @ONLY)
configure_file (teBuildTreeSettings.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/teBuildTreeSettings.cmake @ONLY)

# Installing CMake files
# Exporting targets.
export( TARGETS ${_TE_LIBRARIES} FILE "${PROJECT_BINARY_DIR}/teDepends.cmake")
export( PACKAGE terralib )

install (
  EXPORT teDepends 
  DESTINATION Modules
  COMPONENT CMAKE
)
  
# CMake util files
FILE ( GLOB cmake_files "${CMAKE_CURRENT_BINARY_DIR}/terralibConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/terralibConfigVersion.cmake")
install ( 
  FILES ${cmake_files}
  DESTINATION Modules
  COMPONENT CMAKE
)

install ( 
  DIRECTORY ${T5_MODULES_PATH}
  DESTINATION .
  COMPONENT CMAKE
  PATTERN "*Package*" EXCLUDE
)

install ( 
  DIRECTORY ${ROOT}/log
  DESTINATION .
  COMPONENT HEADERS
)

# Installing TerraLib remaining header files
FILE ( GLOB h_files ${ROOT}/src/*.h )

install ( 
  FILES ${h_files}
  DESTINATION .
  COMPONENT HEADERS
)

FILE ( GLOB h_files ${ROOT}/src/terralib/*.h )

install ( 
  FILES ${h_files}
  DESTINATION terralib
  COMPONENT HEADERS
)

include (${T5_MODULES_PATH}/Package.cmake)
