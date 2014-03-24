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

# variables storing absolute paths
set(TERRALIB_ABSOLUTE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# variables that controls path
set(TERRALIB_DIR_VAR_NAME "TERRALIB_HOME")

#
# global definitions and includes
#

add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)

include_directories(${CMAKE_BINARY_DIR})


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

find_package(Qt5 5.1 COMPONENTS Core Gui Widgets)

if(Qt5_FOUND)

  #set(CMAKE_AUTOMOC ON)
  #set(CMAKE_INCLUDE_CURRENT_DIR ON)

else()

  find_package(Qt4 4.6.3 QUIET COMPONENTS QtCore QtGui)

  if(QT4_FOUND)

    #include(${QT_USE_FILE})

    #include_directories(${QT_INCLUDE_DIR})

    #add_definitions(${QT_DEFINITIONS})

  endif()

endif()


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

if(PostgreSQL_FOUND OR (PostgreSQL_INCLUDE_DIR AND PostgreSQL_LIBRARY_DIR))
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


#
# build options
#

if(DOXYGEN_FOUND)
  option(TERRALIB_DOXYGEN_ENABLED  "Enable API documentation build?" ON)
endif()

option(TERRALIB_MOD_COLOR_ENABLED "Build Color module?" ON)

option(TERRALIB_MOD_DATATYPE_ENABLED "Build Data Type module?" ON)

if(PROJ4_FOUND)
  option(TERRALIB_MOD_SRS_ENABLED "Build Spatial Reference System module?" ON)
endif()

option(TERRALIB_MOD_XLINK_ENABLED "Build XLink module?" ON)

option(TERRALIB_MOD_XML_ENABLED "Build XML module?" ON)


#
# process TERRALIB configuration files
#

configure_file(${terralib_SOURCE_DIR}/terralib-config-pkg.cmake.in
               ${terralib_BINARY_DIR}/pkg/terralib-config.cmake @ONLY)

configure_file(${terralib_SOURCE_DIR}/terralib-config-version.cmake.in
               ${terralib_BINARY_DIR}/terralib-config-version.cmake @ONLY)

configure_file(${terralib_SOURCE_DIR}/terralib-config.cmake.in
               ${terralib_BINARY_DIR}/terralib-config.cmake @ONLY)

configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/Config.h.in
               ${CMAKE_BINARY_DIR}/terralib_config.h @ONLY)

configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/Defines.h.in
               ${CMAKE_BINARY_DIR}/terralib_defines.h @ONLY)

configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/Version.h.in
               ${CMAKE_BINARY_DIR}/terralib_version.h @ONLY)


#
# build modules
#

#if(TERRALIB_DOXYGEN_ENABLED)
#  add_subdirectory(terralib_doc_doxygen)
#endif()

if(TERRALIB_MOD_COLOR_ENABLED)
  add_subdirectory(terralib_mod_color)
endif()


add_subdirectory(terralib_mod_common)

if(TERRALIB_MOD_DATATYPE_ENABLED)
  add_subdirectory(terralib_mod_datatype)
endif()

if(TERRALIB_MOD_SRS_ENABLED)
  add_subdirectory(terralib_mod_srs)
endif()

if(TERRALIB_MOD_XLINK_ENABLED)
  add_subdirectory(terralib_mod_xlink)
endif()


if(TERRALIB_MOD_XML_ENABLED)
  add_subdirectory(terralib_mod_xml)
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

install(FILES ${terralib_BINARY_DIR}/pkg/terralib-config.cmake
              ${terralib_BINARY_DIR}/terralib-config-version.cmake
        DESTINATION lib/cmake/terralib-${TERRALIB_PKG_VERSION} COMPONENT devel)

install(EXPORT terralib-targets
        DESTINATION lib/cmake/terralib-${TERRALIB_PKG_VERSION}
        FILE terralib-exports.cmake
        COMPONENT devel)

#install(DIRECTORY ${TERRALIB_ABSOLUTE_ROOT_DIR}/resources/icons 
#        DESTINATION share/terralib COMPONENT runtime)


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

