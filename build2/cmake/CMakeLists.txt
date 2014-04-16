#
#  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of the TerraLib - a Framework for building GIS enabled applications.
#
#  TerraLib is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  TerraLib is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with TerraLib. See COPYING. If not, write to
#  TerraLib Team at <terralib-team@terralib.org>.
#
#
#  Description: Main CMake script for the TerraLib build system.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

cmake_minimum_required(VERSION 2.8.7)

project(terralib)


#
# main variables for the build script
#

# variables that control the system versioning
set(TERRALIB_VERSION_MAJOR 5)
set(TERRALIB_VERSION_MINOR 0)
set(TERRALIB_VERSION_PATCH 0)
set(TERRALIB_VERSION_STATUS "beta1")
set(TERRALIB_VERSION_STRING "${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}.${TERRALIB_VERSION_PATCH}-${TERRALIB_VERSION_STATUS}")
set(TERRALIB_VERSION "0x050000")
set(TERRALIB_PKG_VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}.${TERRALIB_VERSION_PATCH})

# variables that control the install location
if(NOT DEFINED TERRALIB_DESTINATION_PLUGINS)
  set(TERRALIB_DESTINATION_PLUGINS "share/terralib/plugins")
endif()

if(NOT DEFINED TERRALIB_DESTINATION_EXAMPLES)
  set(TERRALIB_DESTINATION_EXAMPLES "share/terralib/examples")
endif()

# variables storing absolute paths
set(TERRALIB_ABSOLUTE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# variables that controls path
if(NOT DEFINED TERRALIB_DIR_VAR_NAME)
  set(TERRALIB_DIR_VAR_NAME "TERRALIB_HOME" CACHE STRING "Name of an environment variable with the base installation path of TerraLib")
endif()

if(NOT DEFINED TERRALIB_EXAMPLES_DATA_DIR)
  set(TERRALIB_EXAMPLES_DATA_DIR "${CMAKE_BINARY_DIR}/data" CACHE PATH "Path to a directory containing the data used in the examples")
endif()

# variables that controls what is built
if(NOT DEFINED TERRALIB_BUILD_EXAMPLES)
  set(TERRALIB_BUILD_EXAMPLES 1 CACHE BOOL "If on, shows a list of examples to be built")
endif()

if(NOT DEFINED TERRALIB_BUILD_UNITTEST)
  set(TERRALIB_BUILD_UNITTEST 1 CACHE BOOL "If on, shows a list of unit-tests to be built")
endif()

if(NOT DEFINED TERRALIB_BUILD_AS_BUNDLE)
  set(TERRALIB_BUILD_AS_BUNDLE 0 CACHE BOOL "If on, tells that the build will generate a bundle")
endif()

if(NOT DEFINED TERRALIB_AUTOMATIC_INITIALIZATION)
  set(TERRALIB_AUTOMATIC_INITIALIZATION 1 CACHE BOOL "Tells that TerraLib should initialize its resources automatically")
endif()


#
# global definitions and includes
#

add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)

include_directories(${CMAKE_BINARY_DIR})
include_directories(${TERRALIB_ABSOLUTE_ROOT_DIR}/src)


#
# Build will generate files into a single folder
#

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


#
# global configurations
#

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()


#
# including CMake's auxiliary files
#

include(CMakeDependentOption)


#
# first check of libraries
#

find_package(Boost 1.54.0 REQUIRED system date_time chrono timer filesystem thread log log_setup program_options unit_test_framework)

find_package(Doxygen QUIET)

find_package(GDAL)

find_package(GEOS)

find_package(GEOSC)

find_package(GNUGettext)

find_package(GNUIconv)

find_package(Log4cxx)

find_package(Lua51)

find_package(PostgreSQL)

find_package(Proj4)

find_package(Qt5 5.1 COMPONENTS Core Gui Widgets PrintSupport)

if(NOT Qt5_FOUND)
  find_package(Qt4 4.6.3 QUIET COMPONENTS QtCore QtGui)
endif()

if(Qt5_FOUND OR QT4_FOUND)
  find_package(Qwt)
  find_package(QtPropertyBrowser)
endif()

find_package(Xerces)


#
# some variables needed in config file
#

if(Boost_FOUND)
  set(TERRALIB_BOOST_ENABLED ON)
endif()

if(GDAL_FOUND)
  set(TERRALIB_GDAL_ENABLED ON)
endif()

if(GEOS_FOUND)
  set(TERRALIB_GEOS_ENABLED ON)
endif()

if(GEOSC_FOUND)
  set(TERRALIB_GEOSC_ENABLED ON)
endif()

if(GNUGETTEXT_FOUND)

  set(TERRALIB_GNU_GETTEXT_ENABLED ON)

  if(NOT DEFINED TERRALIB_TRANSLATOR_ENABLED)
    set(TERRALIB_TRANSLATOR_ENABLED 1 CACHE BOOL "Enable translator support?")
  endif()

else()

  if(DEFINED TERRALIB_TRANSLATOR_ENABLED)
    set(TERRALIB_TRANSLATOR_ENABLED 0 CACHE BOOL "Enable translator support?" FORCE)
  endif()
  
endif()

if(GNUICONV_FOUND)

  set(TERRALIB_GNU_ICONV_ENABLED ON)

  if(NOT DEFINED TERRALIB_CHARENCODING_ENABLED)
    set(TERRALIB_CHARENCODING_ENABLED 1 CACHE BOOL "Enable character encoding support?")
  endif()

else()

  if(DEFINED TERRALIB_CHARENCODING_ENABLED)
    set(TERRALIB_CHARENCODING_ENABLED 0 CACHE BOOL "Enable character encoding support?" FORCE)
  endif()

endif()

if(LUA51_FOUND)
  set(TERRALIB_LUA_ENABLED ON)
endif()

if(LOG4CXX_FOUND)
  set(TERRALIB_APACHE_LOG4CXX_ENABLED ON)

  if(NOT DEFINED TERRALIB_LOGGER_ENABLED)
    set(TERRALIB_LOGGER_ENABLED 1 CACHE BOOL "Enable logger?")
  endif()

  if(TERRALIB_LOGGER_ENABLED AND NOT DEFINED TERRALIB_LOGGER_DO_AUTOMATIC_INITIALIZATION)

    set(TERRALIB_LOGGER_DO_AUTOMATIC_INITIALIZATION 1 CACHE BOOL "Automatic initialize logger support?")

  else()

    if(NOT TERRALIB_LOGGER_ENABLED AND DEFINED TERRALIB_LOGGER_DO_AUTOMATIC_INITIALIZATION)
      set(TERRALIB_LOGGER_DO_AUTOMATIC_INITIALIZATION 0 CACHE BOOL "Automatic initialize logger support?" FORCE)
    endif()

  endif()
  
  if(TERRALIB_LOGGER_ENABLED AND NOT DEFINED TERRALIB_LOGGER_DO_STATIC_INITIALIZATION)

    set(TERRALIB_LOGGER_DO_STATIC_INITIALIZATION 1 CACHE BOOL "Perform logger initialization support during static's initialization?")

  else()

    if(NOT TERRALIB_LOGGER_ENABLED AND DEFINED TERRALIB_LOGGER_DO_STATIC_INITIALIZATION)
      set(TERRALIB_LOGGER_DO_STATIC_INITIALIZATION 0 CACHE BOOL "Perform logger initialization support during static's initialization?" FORCE)
    endif()

  endif()

else()

  if(DEFINED TERRALIB_LOGGER_ENABLED)
    set(TERRALIB_LOGGER_ENABLED 0 CACHE BOOL "Enable logger?" FORCE)
  endif()

endif()

if(PostgreSQL_INCLUDE_DIR AND PostgreSQL_LIBRARY_DIR)
  set(TERRALIB_LIBPQ_ENABLED ON)
endif()

if(PROJ4_FOUND)
  set(TERRALIB_PROJ4_ENABLED ON)
endif()

if(QT4_FOUND)
  set(TERRALIB_QT4_ENABLED ON)
endif()

if(Qt5_FOUND)
  set(TERRALIB_QT5_ENABLED ON)
endif()

if(QTPROPERTYBROWSER_FOUND)
  set(TERRALIB_QTPROPERTYBROWSER_ENABLED ON)
endif()

if(QWT_FOUND)
  set(TERRALIB_QWT_ENABLED ON)
endif()

if(XERCES_FOUND)
  set(TERRALIB_XERCES_ENABLED ON)
endif()


#
# build options
#
# warning: please, do not change the order below!
#

if(DOXYGEN_FOUND)
  option(TERRALIB_DOXYGEN_ENABLED  "Enable API documentation build?" ON)
endif()

option(TERRALIB_MOD_COLOR_ENABLED "Build Color module?" ON)

option(TERRALIB_MOD_DATATYPE_ENABLED "Build Data Type module?" ON)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_GEOMETRY_ENABLED "Build Geometry module?" ON "TERRALIB_MOD_DATATYPE_ENABLED" OFF)

option(TERRALIB_MOD_SRS_ENABLED "Build Spatial Reference System module?" ON)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_RASTER_ENABLED "Build Raster module?" ON "TERRALIB_MOD_DATATYPE_ENABLED;TERRALIB_MOD_SRS_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_DATAACCESS_ENABLED "Build Data Access module?" ON "TERRALIB_MOD_DATATYPE_ENABLED;TERRALIB_MOD_RASTER_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED" OFF)

option(TERRALIB_MOD_XLINK_ENABLED "Build XLink module?" ON)

option(TERRALIB_MOD_XML_ENABLED "Build XML module?" ON)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_PLUGIN_ENABLED "Build Plugin module?" ON "TERRALIB_MOD_XML_ENABLED" OFF)

if(XERCES_FOUND)
  CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_XERCES_ENABLED "Build Xerces support?" ON "TERRALIB_MOD_XML_ENABLED;TERRALIB_MOD_PLUGIN_ENABLED" OFF)
endif()

if(PostgreSQL_INCLUDE_DIR AND PostgreSQL_LIBRARY_DIR)
  CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_POSTGIS_ENABLED "Build PostGIS support?" ON "TERRALIB_MOD_XML_ENABLED;TERRALIB_MOD_DATAACCESS_ENABLED;TERRALIB_MOD_DATATYPE_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_PLUGIN_ENABLED" OFF)
endif()

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_MEMORY_ENABLED "Build Memory support?" ON "TERRALIB_MOD_DATAACCESS_ENABLED;TERRALIB_MOD_DATATYPE_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_RASTER_ENABLED" OFF)

if(GDAL_FOUND)
  CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_OGR_ENABLED "Build OGR support?" ON "TERRALIB_MOD_DATAACCESS_ENABLED;TERRALIB_MOD_DATATYPE_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_PLUGIN_ENABLED" OFF)

  CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_GDAL_ENABLED "Build GDAL support?" ON "TERRALIB_MOD_OGR_ENABLED;TERRALIB_MOD_DATAACCESS_ENABLED;TERRALIB_MOD_DATATYPE_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_PLUGIN_ENABLED" OFF)
endif()

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_ANNOTATIONTEXT_ENABLED "Build AnnotationText module?" ON "TERRALIB_MOD_GEOMETRY_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_GML_ENABLED "Build GML module?" ON "TERRALIB_MOD_GEOMETRY_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_FILTER_ENABLED "Build Filter module?" ON "TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_GML_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_SYMBOLOGY_ENABLED "Build Symbology module?" ON "TERRALIB_MOD_COLOR_ENABLED;TERRALIB_MOD_FILTER_ENABLED;TERRALIB_MOD_XLINK_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_MAPTOOLS_ENABLED "Build MapTools module?" ON "TERRALIB_MOD_ANNOTATIONTEXT_ENABLED;TERRALIB_MOD_COLOR_ENABLED;TERRALIB_MOD_DATAACCESS_ENABLED;TERRALIB_MOD_DATATYPE_ENABLED;TERRALIB_MOD_FILTER_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_GML_ENABLED;TERRALIB_MOD_MEMORY_ENABLED;TERRALIB_MOD_RASTER_ENABLED;TERRALIB_MOD_SRS_ENABLED;TERRALIB_MOD_SYMBOLOGY_ENABLED;TERRALIB_MOD_XLINK_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_CLASSIFICATION_ENABLED "Build Classification module?" ON "TERRALIB_MOD_DATAACCESS_ENABLED;TERRALIB_MOD_DATATYPE_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_MEMORY_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_METADATA_ENABLED "Build Metadata module?" ON "TERRALIB_MOD_DATAACCESS_ENABLED;TERRALIB_MOD_DATATYPE_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_GRAPH_ENABLED "Build Graph module?" ON "TERRALIB_MOD_DATAACCESS_ENABLED;TERRALIB_MOD_DATATYPE_ENABLED;TERRALIB_MOD_RASTER_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_MAPTOOLS_ENABLED;TERRALIB_MOD_MEMORY_ENABLED;TERRALIB_MOD_PLUGIN_ENABLED;TERRALIB_MOD_SYMBOLOGY_ENABLED;TERRALIB_MOD_SRS_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_STATISTICS_CORE_ENABLED "Build Statistics Core module?" ON "TERRALIB_MOD_RASTER_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_DATAACCESS_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_VP_CORE_ENABLED "Build Vector Processing Core module?" ON "TERRALIB_MOD_STATISTICS_CORE_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_DATAACCESS_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_RP_ENABLED "Build Raster Processing module?" ON "TERRALIB_MOD_RASTER_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_STATISTICS_CORE_ENABLED;TERRALIB_MOD_MEMORY_ENABLED;TERRALIB_MOD_SRS_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_STMEMORY_ENABLED "Build ST Memory module?" ON "TERRALIB_MOD_MEMORY_ENABLED;TERRALIB_MOD_RASTER_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_DATAACCESS_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_ST_ENABLED "Build ST module?" ON "TERRALIB_MOD_STMEMORY_ENABLED;TERRALIB_MOD_RASTER_ENABLED;TERRALIB_MOD_GEOMETRY_ENABLED;TERRALIB_MOD_DATAACCESS_ENABLED" OFF)

if(QT4_FOUND OR Qt5_FOUND)

  CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_STATISTICS_QT_ENABLED "Build Qt support for Statistics module?" ON "TERRALIB_MOD_STATISTICS_CORE_ENABLED" OFF)

  if(QWT_FOUND AND QTPROPERTYBROWSER_FOUND)
    CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_QT_WIDGETS_ENABLED "Build Qt Widgets?" ON "TERRALIB_MOD_MAPTOOLS_ENABLED;TERRALIB_MOD_DATAACCESS_ENABLED;TERRALIB_MOD_RP_ENABLED;TERRALIB_MOD_ST_ENABLED" OFF)
  endif()

  CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_VP_QT_ENABLED "Build Vector Processing Qt module?" ON "TERRALIB_MOD_VP_CORE_ENABLED;TERRALIB_MOD_QT_WIDGETS_ENABLED" OFF)

  CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_QT_APF_ENABLED "Build Qt Appllication Framework?" ON "TERRALIB_MOD_QT_WIDGETS_ENABLED" OFF)
endif()

option(TERRALIB_MOD_XSD_ENABLED "Build XSD module?" ON)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_GDAL_ENABLED "Build GDAL Driver Qt plugin?" ON "TERRALIB_MOD_GDAL_ENABLED;TERRALIB_MOD_QT_APF_ENABLED;TERRALIB_MOD_QT_WIDGETS_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_OGR_ENABLED "Build OGR Driver Qt plugin?" ON "TERRALIB_MOD_OGR_ENABLED;TERRALIB_MOD_QT_APF_ENABLED;TERRALIB_MOD_QT_WIDGETS_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_POSTGIS_ENABLED "Build PostGIS Driver Qt plugin?" ON "TERRALIB_MOD_POSTGIS_ENABLED;TERRALIB_MOD_QT_APF_ENABLED;TERRALIB_MOD_QT_WIDGETS_ENABLED" OFF)

CMAKE_DEPENDENT_OPTION(TERRALIB_TERRAVIEW_ENABLED "Build TerraView?" ON "TERRALIB_MOD_QT_APF_ENABLED" OFF)

if(TERRALIB_BUILD_EXAMPLES)
  option(TERRALIB_EXAMPLE_COMMON_ENABLED "Build examples for the Common runtime module?" ON)
  option(TERRALIB_EXAMPLE_FACTORY_ENABLED "Build the TerraLib Factory example?" ON)

  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_ADO_ENABLED "Build ADO examples?" ON "TERRALIB_MOD_DATAACCESS_ENABLED" OFF)
  CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_COLORBAR_ENABLED "Build color bar example?" ON "TERRALIB_MOD_QT_WIDGETS_ENABLED" OFF)
  CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_CPPPLUGIN_ENABLED "Build C++ plugins example" ON "TERRALIB_MOD_PLUGIN_ENABLED" OFF)
  CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_DATAACCESS_ENABLED "Build examples for the Data Access module?" ON "TERRALIB_MOD_DATAACCESS_ENABLED" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_DATASETMANAGEMENT_ENABLED "Build examples for the Dataset  Management support?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_DATAVIEW_ENABLED "Build examples for the Data View support?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_GEOMETRY_ENABLED "Build examples for the Geometry module?" ON "TERRALIB_MOD_GEOMETRY_ENABLED" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_GRAPH_ENABLED "Build examples for the Graph module?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_HELP_ENABLED "Build examples for the TerraLib Help support?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_LAYEREXPLORER_ENABLED "Build the Layer Explorer example?" ON "TERRALIB_MOD_QT_WIDGETS_ENABLED" OFF)
  CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_MAPTOOLS_ENABLED "Build examples for the MapTools module?" ON "TERRALIB_MOD_MAPTOOLS_ENABLED" OFF)
  CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_PROGRESS_ENABLED "Build examples for the TerraLib Progress support?" ON "TERRALIB_MOD_QT_WIDGETS_ENABLED" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_PROPERTYMANAGEMENT_ENABLED "Build examples for Property Management support?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_QT_CANVAS_ENABLED "Build examples for Canvas support?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_QT_CHARTS_ENABLED "Build examples for Charts support" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_QT_DATASOURCE_ENABLED "Build examples for the Qt Data Source support?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_QT_QUERY_ENABLED "Build examples for the Qt Query support?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_QT_RASTER_VISUAL_ENABLED "Build examples for the Qt Raster Visual support?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_QT_RP_ENABLED "Build examples for the Qt Raster Processing module?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_QT_SE_ENABLED "Build examples for the Qt Symbology Encoding module?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_QT_SRS_ENABLED "Build examples for the Qt SRS module?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_QT_TOOLS_ENABLED "Build examples for the Qt Tools support?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_QT_UTILS_ENABLED "Build examples for the Qt Utils support?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_RASTER_ENABLED "Build examples for the Raster module?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_RP_ENABLED "Build examples for the Raster Processing module?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_SAM_ENABLED "Build examples for the SAM module?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_SERIALIZATION_ENABLED "Build examples for the Serialization support?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_SRS_ENABLED "Build examples for the SRS module?" ON "TERRALIB_MOD_SRS_ENABLED" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_ST_ENABLED "Build examples for the Spatial Temporal?" ON "TERRALIB_MOD_DATAACCESS" OFF)
  CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_VP_ENABLED "Build examples for the Vector Processing module?" ON "TERRALIB_MOD_VP_CORE_ENABLED" OFF)
  #CMAKE_DEPENDENT_OPTION(TERRALIB_EXAMPLE_XERCES_ENABLED "Build examples for the Data Access module?" ON "TERRALIB_MOD_DATAACCESS" OFF)

else()
  if(defined TERRALIB_EXAMPLE_COMMON)
    set(TERRALIB_EXAMPLE_COMMON 0 CACHE BOOL "Build examples for the Common runtime module?" FORCE)
  endif()

  if(defined TERRALIB_EXAMPLE_COLORBAR)
    set(TERRALIB_EXAMPLE_COLORBAR 0 CACHE BOOL "Build examples for the Color Bar runtime module?" FORCE)
  endif()

  if(defined TERRALIB_EXAMPLE_CPPPLUGIN)
    set(TERRALIB_EXAMPLE_CPPPLUGIN 0 CACHE BOOL "Build C++ plugins example?" FORCE)
  endif()

  if(defined TERRALIB_EXAMPLE_DATAACCESS)
    set(TERRALIB_EXAMPLE_DATAACCESS 0 CACHE BOOL "Build examples for the Data Access module?" FORCE)
  endif()

  if(defined TERRALIB_EXAMPLE_FACTORY)
    set(TERRALIB_EXAMPLE_FACTORY 0 CACHE BOOL "Build the TerraLib Factory support example?" FORCE)
  endif()

  if(defined TERRALIB_EXAMPLE_GEOMETRY)
    set(TERRALIB_EXAMPLE_GEOMETRY 0 CACHE BOOL "Build examples for the the Geometry module?" FORCE)
  endif()

  if(defined TERRALIB_EXAMPLE_LAYEREXPLORER)
    set(TERRALIB_EXAMPLE_LAYEREXPLORER 0 CACHE BOOL "Build the TerraLib Layer Explorer support example?" FORCE)
  endif()

  if(defined TERRALIB_EXAMPLE_MAPTOOLS)
    set(TERRALIB_EXAMPLE_MAPTOOLS 0 CACHE BOOL "Build examples for the the Maptools module?" FORCE)
  endif()

  if(defined TERRALIB_EXAMPLE_PROGRESS)
    set(TERRALIB_EXAMPLE_PROGRESS 0 CACHE BOOL "Build example for the TerraLib Progress support?" FORCE)
  endif()

  if(defined TERRALIB_EXAMPLE_SRS)
    set(TERRALIB_EXAMPLE_SRS 0 CACHE BOOL "Build examples for the the SRS module?" FORCE)
  endif()

  if(defined TERRALIB_EXAMPLE_VP)
    set(TERRALIB_EXAMPLE_VP 0 CACHE BOOL "Build examples for the the Vector Processing module?" FORCE)
  endif()

endif()


#
# process TERRALIB configuration files
#

configure_file(${CMAKE_SOURCE_DIR}/terralib-config-pkg.cmake.in
               ${CMAKE_BINARY_DIR}/pkg/terralib-config.cmake @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/terralib-config-version.cmake.in
               ${CMAKE_BINARY_DIR}/terralib-config-version.cmake @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/terralib-config.cmake.in
               ${CMAKE_BINARY_DIR}/terralib-config.cmake @ONLY)

configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/Config.h.in
               ${CMAKE_BINARY_DIR}/terralib_config.h @ONLY)

configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/Defines.h.in
               ${CMAKE_BINARY_DIR}/terralib_defines.h @ONLY)

configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/Version.h.in
               ${CMAKE_BINARY_DIR}/terralib_version.h @ONLY)

configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/Config.h.in
               ${CMAKE_BINARY_DIR}/terralib_examples_config.h @ONLY)


#
# build modules
#

#if(TERRALIB_DOXYGEN_ENABLED)
#  add_subdirectory(terralib_doc_doxygen)
#endif()

if(TERRALIB_MOD_ANNOTATIONTEXT_ENABLED)
  add_subdirectory(terralib_mod_annotationtext)
endif()

if(TERRALIB_MOD_CLASSIFICATION_ENABLED)
  add_subdirectory(terralib_mod_classification)
endif()

if(TERRALIB_MOD_COLOR_ENABLED)
  add_subdirectory(terralib_mod_color)
endif()

add_subdirectory(terralib_mod_common)

if(TERRALIB_MOD_DATAACCESS_ENABLED)
  add_subdirectory(terralib_mod_dataaccess)
endif()

if(TERRALIB_MOD_DATATYPE_ENABLED)
  add_subdirectory(terralib_mod_datatype)
endif()

if(TERRALIB_MOD_FILTER_ENABLED)
  add_subdirectory(terralib_mod_filter)
endif()

if(TERRALIB_MOD_GDAL_ENABLED)
  add_subdirectory(terralib_mod_gdal)
endif()

if(TERRALIB_MOD_GEOMETRY_ENABLED)
  add_subdirectory(terralib_mod_geometry)
endif()

if(TERRALIB_MOD_GML_ENABLED)
  add_subdirectory(terralib_mod_gml)
endif()

if(TERRALIB_MOD_GRAPH_ENABLED)
  add_subdirectory(terralib_mod_graph)
endif()

if(TERRALIB_MOD_MAPTOOLS_ENABLED)
  add_subdirectory(terralib_mod_maptools)
endif()

if(TERRALIB_MOD_METADATA_ENABLED)
  add_subdirectory(terralib_mod_metadata)
endif()

if(TERRALIB_MOD_MEMORY_ENABLED)
  add_subdirectory(terralib_mod_memory)
endif()

if(TERRALIB_MOD_OGR_ENABLED)
  add_subdirectory(terralib_mod_ogr)
endif()

if(TERRALIB_MOD_PLUGIN_ENABLED)
  add_subdirectory(terralib_mod_plugin)
endif()

if(TERRALIB_MOD_POSTGIS_ENABLED)
  add_subdirectory(terralib_mod_postgis)
endif()

if(TERRALIB_MOD_QT_APF_ENABLED)
  add_subdirectory(terralib_mod_qt_apf)
endif()

if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_GDAL_ENABLED)
  add_subdirectory(terralib_mod_qt_plugins_datasource_gdal)
endif()

if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_OGR_ENABLED)
  add_subdirectory(terralib_mod_qt_plugins_datasource_ogr)
endif()

if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_POSTGIS_ENABLED)
  add_subdirectory(terralib_mod_qt_plugins_datasource_pgis)
endif()

if(TERRALIB_MOD_QT_WIDGETS_ENABLED)
  add_subdirectory(terralib_mod_qt_widgets)
endif()

if(TERRALIB_MOD_RASTER_ENABLED)
  add_subdirectory(terralib_mod_raster)
endif()

if(TERRALIB_MOD_RP_ENABLED)
  add_subdirectory(terralib_mod_rp)
endif()

if(TERRALIB_MOD_SRS_ENABLED)
  add_subdirectory(terralib_mod_srs)
endif()

if(TERRALIB_MOD_ST_ENABLED)
  add_subdirectory(terralib_mod_st)
endif()

if(TERRALIB_MOD_STATISTICS_CORE_ENABLED)
  add_subdirectory(terralib_mod_statistics_core)
endif()

if(TERRALIB_MOD_STMEMORY_ENABLED)
  add_subdirectory(terralib_mod_stmemory)
endif()

if(TERRALIB_MOD_STATISTICS_QT_ENABLED)
  add_subdirectory(terralib_mod_statistics_qt)
endif()

if(TERRALIB_MOD_SYMBOLOGY_ENABLED)
  add_subdirectory(terralib_mod_symbology)
endif()

if(TERRALIB_MOD_VP_CORE_ENABLED)
  add_subdirectory(terralib_mod_vp_core)
endif()

if(TERRALIB_MOD_VP_QT_ENABLED)
  add_subdirectory(terralib_mod_vp_qt)
endif()

if(TERRALIB_MOD_XERCES_ENABLED)
  add_subdirectory(terralib_mod_xerces)
endif()

if(TERRALIB_MOD_XLINK_ENABLED)
  add_subdirectory(terralib_mod_xlink)
endif()

if(TERRALIB_MOD_XML_ENABLED)
  add_subdirectory(terralib_mod_xml)
endif()

if(TERRALIB_MOD_XSD_ENABLED)
  add_subdirectory(terralib_mod_xsd)
endif()

if(TERRALIB_TERRAVIEW_ENABLED)
  add_subdirectory(terraview)
endif()

if(TERRALIB_BUILD_EXAMPLES)
  if(TERRALIB_EXAMPLE_COMMON_ENABLED)
    add_subdirectory(terralib_example_common)
  endif()
  
  if(TERRALIB_EXAMPLE_COLORBAR_ENABLED)
    add_subdirectory(terralib_example_colorbar)
  endif()

  if(TERRALIB_EXAMPLE_CPPPLUGIN_ENABLED)
    add_subdirectory(terralib_example_cppplugin)
  endif()

  if(TERRALIB_EXAMPLE_DATAACCESS_ENABLED)
    add_subdirectory(terralib_example_dataaccess)
  endif()

  if(TERRALIB_EXAMPLE_FACTORY_ENABLED)
    add_subdirectory(terralib_example_factory)
  endif()

  if(TERRALIB_EXAMPLE_GEOMETRY_ENABLED)
    add_subdirectory(terralib_example_geometry)
  endif()

  if(TERRALIB_EXAMPLE_LAYEREXPLORER_ENABLED)
    add_subdirectory(terralib_example_layerexplorer)
  endif()

  if(TERRALIB_EXAMPLE_MAPTOOLS_ENABLED)
    add_subdirectory(terralib_example_maptools)
  endif()

  if(TERRALIB_EXAMPLE_PROGRESS_ENABLED)
    add_subdirectory(terralib_example_progress)
  endif()

  if(TERRALIB_EXAMPLE_SRS_ENABLED)
    add_subdirectory(terralib_example_srs)
  endif()

  if(TERRALIB_EXAMPLE_VP_ENABLED)
    add_subdirectory(terralib_example_vp)
  endif()

endif()


#
# install and targets export
#

install(FILES ${CMAKE_BINARY_DIR}/terralib_config.h
              ${CMAKE_BINARY_DIR}/terralib_defines.h
              ${CMAKE_BINARY_DIR}/terralib_version.h
              ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/Config.h
              ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/Defines.h
              ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/Version.h
        DESTINATION include/terralib COMPONENT devel)

install(FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib.h
        DESTINATION include COMPONENT devel)

install(FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/Config.h.in
              ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/Config.h
        DESTINATION ${TERRALIB_DESTINATION_EXAMPLES} COMPONENT devel)

install(FILES ${terralib_BINARY_DIR}/pkg/terralib-config.cmake
              ${terralib_BINARY_DIR}/terralib-config-version.cmake
        DESTINATION lib/cmake/terralib-${TERRALIB_PKG_VERSION} COMPONENT devel)

install(EXPORT terralib-targets
        DESTINATION lib/cmake/terralib-${TERRALIB_PKG_VERSION}
        FILE terralib-exports.cmake
        COMPONENT devel)

install(DIRECTORY ${TERRALIB_ABSOLUTE_ROOT_DIR}/share
        DESTINATION share COMPONENT runtime)


#
# Track dependencies
#

if(${TERRALIB_TRACK_DEPENDENCIES})

  #set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP ON)

  include(InstallRequiredSystemLibraries)

  install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
          DESTINATION bin
          COMPONENT runtime)

  add_subdirectory(fix_bundle)
endif()


#
# CPack
#

set(CPACK_SOURCE_INSTALLED_DIRECTORIES "${TERRALIB_ABSOLUTE_ROOT_DIR};/")

configure_file(${terralib_SOURCE_DIR}/terralib-cpack-options.cmake.in
               ${terralib_BINARY_DIR}/terralibCPackOptions.cmake @ONLY)

set(CPACK_PROJECT_CONFIG_FILE ${terralib_BINARY_DIR}/terralibCPackOptions.cmake)

include(CPack)

# defining installation types, components and groups
cpack_add_install_type(full DISPLAY_NAME "Complete")
cpack_add_install_type(minimalistic DISPLAY_NAME "Minimalistic")

cpack_add_component_group(general_group
                          DISPLAY_NAME "Applications"
                          DESCRIPTION "TerraLib ..."
                          EXPANDED)

cpack_add_component_group(devel_group
                          DISPLAY_NAME "Development Tools"
                          DESCRIPTION "Development Tools: header files, libraries and cmake stuffs"
                          EXPANDED)

cpack_add_component(runtime
                    DISPLAY_NAME "Runtime Libraries and Executables"
                    DESCRIPTION "Install only necessary shared libraries (or DLLs) and executables"
                    GROUP general_group
                    INSTALL_TYPES full minimalistic)

cpack_add_component(devel
                    DISPLAY_NAME "Development package"
                    DESCRIPTION "Install header files, cmake stuffs and additional shared libraries"
                    GROUP devel_group
                    INSTALL_TYPES full minimalistic)

