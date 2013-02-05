list (APPEND TE_DEP_INCLUDES "${CMAKE_CURRENT_BINARY_DIR}")

set (TE_MODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

list (APPEND CMAKE_MODULE_PATH "${TE_MODULES_PATH}" )

set (TE_DEPENDENCIES_DIR "$ENV{TE_DEPENDENCIES_DIR}" CACHE PATH "Find the directory containing terralib5 3rd-party dependencies.")

include (${TE_MODULES_PATH}/GeneralConfig.cmake)
include (${TE_MODULES_PATH}/UtilFunctions.cmake)
include (${TE_MODULES_PATH}/Deps_versions.cmake)

if(WIN32)
  include (${TE_MODULES_PATH}/WinConfig.cmake)
endif()

add_definitions (-DBOOST_FILESYSTEM_VERSION=3 -DBOOST_UBLAS_TYPE_CHECK=0)

if(NOT "${TE_DEPENDENCIES_DIR}" STREQUAL "")
  set (CMAKE_FIND_ROOT_PATH "${TE_DEPENDENCIES_DIR}" "${TE_DEPENDENCIES_DIR}/gdal")
  list (APPEND CMAKE_PREFIX_PATH "${TE_DEPENDENCIES_DIR}" "${TE_DEPENDENCIES_DIR}/gdal")
endif()

# TerraLib configurations
# =========================
set (TE_MAJOR_VERSION 5)
set (TE_MINOR_VERSION 0)
set (TE_PATCH_VERSION 0)
set (TE_RELEASE_STATUS "alpha.1")
set (TE_BUILD_DATE "20130301")
set (TE_STRING_BUILD_DATE "2013-03-01")
set (TE_STRING_VERSION "5.0.0-alpha.1")
set (TE_INT_VERSION "050000")
set (TE_VERSION "${TE_MAJOR_VERSION}.${TE_MINOR_VERSION}.${TE_PATCH_VERSION}")
set (TE_DIR_ENVIRONMENT_VARIABLE "TE_BIN_DIR" CACHE STRING "Name of the variable containg TerraLib binaries.") 
set (TE_SCHEMA_LOCATION "${ROOT}/schemas/terralib" CACHE STRING "Location of the plugin_info.xsd file.")
set (TE_LOGO "${ROOT}/resources/themes/terralib/128x128/terralib_logo_128x128.png" CACHE STRING "Location of the TerraLib logo image.")
set (TE_JSON_FILES_LOCATION "${ROOT}/resources/json" CACHE STRING "Location of the TerraLib initialization files.")
set (TE_SQL_FILES_LOCATION "${ROOT}/resources/sql" CACHE STRING "Location of the TerraLib initialization files.")
set (TE_XML_FILES_LOCATION "${ROOT}/resources/xml" CACHE STRING "Location of the TerraLib initialization files.")
set (TE_ABOUT_LOGO "${ROOT}/resources/themes/terralib/128x128/terralib_logo_128x128.png" CACHE STRING "Location of the about logo image.")
set (TE_ICONS_THEME_PATH "${ROOT}/resources/themes" CACHE STRING "Location of the theme of icons available in TerraLib.")
set (TE_ICONS_THEME "terralib" CACHE STRING "Name of the theme of icons (by default we will use terralib theme).")
set (TE_ICONS_THEME "terralib" CACHE STRING "Name of the theme of icons (by default we will use terralib theme).")
if(WIN32)
  set (TE_BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
else()
  set (TE_BIN_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endif()
# =========================

option (BUILD_ANNOTATIONTEXT "Build Annotation Text module?" ON)
option (BUILD_COLOR "Build Color module?" ON)
option (BUILD_COMMON "Build Common Runtime module?" ON)
option (BUILD_DATAACCESS "Build Data Access module?" ON)
option (BUILD_DATATYPE "Build Data Type module?" ON)
option (BUILD_FILTER "Build OGC Filter module?" ON)
option (BUILD_GDAL "Build GDAL module?" ON)
option (BUILD_GEOMETRY "Build Geometry module?" ON)
option (BUILD_GML "Build GML module?" ON)
option (BUILD_IDL "Build IDL module?" OFF)
option (BUILD_MAPTOOLS "Build Map Tools module?" ON)
option (BUILD_MEMORY "Build Memory module?" ON)
option (BUILD_METADATA "Build Metadata module?" OFF)
option (BUILD_MYSQL "Build MySQL module?" OFF)
option (BUILD_OGR "Build OGR module?" ON)
option (BUILD_PLUGIN "Build Plugin module?" ON)
option (BUILD_POSTGIS "Build PostGIS module?" ON)
option (BUILD_QT_AF "Build Qt Application Framework module?" ON)
option (BUILD_QT_WIDGETS "Build Qt Widgets module?" ON)
option (BUILD_RASTER "Build Raster module?" ON)
option (BUILD_RP "Build Raster Processing module?" ON)
option (BUILD_SAM "Build Spatial Access Methods module?" ON)
option (BUILD_SERIALIZATION "Build Serialization module?" ON)
option (BUILD_SQLITE "Build SQLite module?" OFF)
option (BUILD_SRS "Build Spatial Reference System module?" ON)
option (BUILD_ST "Build Spatial-Temporal module?" ON)
option (BUILD_STLOADER "Build Spatial-Temporal Loader module?" ON)
option (BUILD_SYMBOLOGY "Build Symbology module?" ON)
option (BUILD_XERCES "Build Xerces module?" ON)
option (BUILD_XLINK "Build XLink module?" ON)
option (BUILD_XML "Build XML module?" ON)
option (BUILD_XSD "Build XSD module?" ON)

option (TE_STATIC_BUILD "Build all modules as static libraries?" OFF)
option (TE_LOGGER_ENABLED "Logger enabled?" ON)
option (TE_LOGGER_DO_AUTOMATIC_INITIALIZATION "Initialize automatically logger?" ON)
option (TE_LOGGER_DO_STATIC_INITIALIZATION "Initialize logger while static initialization?" OFF)
option (TE_TRANSLATOR_ENABLED "Enable translate support on TerraLib?" ON)
option (TE_CHARENCODING_ENABLED "Enable character encoding support on TerraLib?" ON)

if(UNIX)
  option (BUILD_GRIB "Build GRIB module?" ON)
endif()

if(WIN32)
  option (BUILD_ADO "Build ADO module?" ON)
endif()

IF(BUILD_ANNOTATIONTEXT)
	add_subdirectory(terralib/terralib_annotationtext)
endif()

IF(BUILD_COLOR)
	add_subdirectory(terralib/terralib_color)
endif()

IF(BUILD_COMMON)
	add_subdirectory(terralib/terralib_common)
endif()

IF(BUILD_DATAACCESS)
	add_subdirectory(terralib/terralib_dataaccess)
endif()

IF(BUILD_DATATYPE)
	add_subdirectory(terralib/terralib_datatype)
endif()

IF(BUILD_FILTER)
	add_subdirectory(terralib/terralib_filter)
endif()

IF(BUILD_GDAL)
	add_subdirectory(terralib/terralib_gdal)
endif()

IF(BUILD_GEOMETRY)
	add_subdirectory(terralib/terralib_geometry)
endif()

IF(BUILD_GML)
	add_subdirectory(terralib/terralib_gml)
endif()

IF(BUILD_IDL)
  add_subdirectory(terralib/terralib_idl)
endif()

IF(BUILD_MAPTOOLS)
	add_subdirectory(terralib/terralib_maptools)
endif()

IF(BUILD_MEMORY)
	add_subdirectory(terralib/terralib_memory)
endif()

IF(BUILD_METADATA)
	add_subdirectory(terralib/terralib_metadata)
endif()

IF(BUILD_MYSQL)
	add_subdirectory(terralib/terralib_mysql)
endif()

IF(BUILD_OGR)
	add_subdirectory(terralib/terralib_ogr)
endif()

IF(BUILD_PLUGIN)
	add_subdirectory(terralib/terralib_plugin)
endif()

IF(BUILD_POSTGIS)
	add_subdirectory(terralib/terralib_postgis)
endif()

IF(BUILD_QT_AF)
	add_subdirectory(terralib/terralib_qt_af)
endif()

IF(BUILD_QT_WIDGETS)
	add_subdirectory(terralib/terralib_qt_widgets)
endif()

IF(BUILD_RASTER)
	add_subdirectory(terralib/terralib_raster)
endif()

IF(BUILD_RP)
	add_subdirectory(terralib/terralib_rp)
endif()

IF(BUILD_SAM)
	add_subdirectory(terralib/terralib_sam)
endif()

IF(BUILD_SERIALIZATION)
	add_subdirectory(terralib/terralib_serialization)
endif()

IF(BUILD_SQLITE)
	add_subdirectory(terralib/terralib_sqlite)
endif()

IF(BUILD_SRS)
	add_subdirectory(terralib/terralib_srs)
endif()

IF(BUILD_ST)
	add_subdirectory(terralib/terralib_st)
endif()

IF(BUILD_STLOADER)
	add_subdirectory(terralib/terralib_stloader)
endif()

IF(BUILD_SYMBOLOGY)
	add_subdirectory(terralib/terralib_symbology)
endif()

IF(BUILD_XERCES)
	add_subdirectory(terralib/terralib_xerces)
endif()

IF(BUILD_XLINK)
	add_subdirectory(terralib/terralib_xlink)
endif()

IF(BUILD_XML)
	add_subdirectory(terralib/terralib_xml)
endif()

IF(BUILD_XSD)
	add_subdirectory(terralib/terralib_xsd)
endif()

IF(BUILD_GRIB)
  add_subdirectory(terralib/terralib_grib)
endif()

IF(BUILD_ADO)
  add_subdirectory(terralib/terralib_ado)
endif()

# Generating cmake configuration files
message(STATUS "-- Generating configuration files.")

get_cmake_property(TE_3DS PACKAGES_FOUND)

list (APPEND TE_INCLUDE_DIRS "${ROOT}/src")

configure_file (terralibConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/terralibConfig.cmake @ONLY)
configure_file (terralibConfigVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/terralibConfigVersion.cmake @ONLY)
configure_file (teBuildTreeSettings.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/teBuildTreeSettings.cmake @ONLY)
configure_file (${ROOT}/src/terralib/TerraLibConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/TerraLibConfig.h)

# Installing CMake files
# Exporting targets.
export( TARGETS ${TE_LIBRARIES} FILE "${PROJECT_BINARY_DIR}/teDepends.cmake")
export( PACKAGE terralib )

install (
  EXPORT teDepends 
  DESTINATION modules
  COMPONENT CMAKE
)
  
# CMake util files
FILE ( GLOB cmake_files "${CMAKE_CURRENT_BINARY_DIR}/terralibConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/terralibConfigVersion.cmake")
install ( 
  FILES ${cmake_files}
  DESTINATION modules
  COMPONENT CMAKE
)

install ( 
  DIRECTORY ${T5_MODULES_PATH}
  DESTINATION .
  COMPONENT CMAKE
  PATTERN "*Package*" EXCLUDE
  PATTERN "GenerateDox.cmake" EXCLUDE
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

install (
  DIRECTORY ${ROOT}/schemas
  DESTINATION .
  COMPONENT HEADERS
  FILES_MATCHING PATTERN "*.xsd"
)

install (
  DIRECTORY ${ROOT}/resources
  DESTINATION .
  COMPONENT HEADERS
)

install ( 
  FILES ${CMAKE_CURRENT_BINARY_DIR}/TerraLibConfig.h
  DESTINATION terralib
  COMPONENT HEADERS
)

include (${TE_MODULES_PATH}/Package.cmake)
include (${TE_MODULES_PATH}/GenerateDox.cmake)

#set (TE_LIBRARIES "${_TE_LIBRARIES}")
