# This script builds the rp unit test
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")     # the terralib root directory (change it if it is in a different directory)
set(SRCDIR ${ROOT}/unittest/rp)                         # directory where the source files of the rp unit test are located
set(INCLDIR ${SRCDIR})                                  # directory where the header files of the rp unit test are located

# Set the project name
set(PROJ_NAME "terralib_unittest_rp_classifier")
project(${PROJ_NAME})           # the project name is terralib_unittest_rp_classifier
set(LIB_NAME ${PROJ_NAME})      # the library name is terralib_unittest_rp_classifier

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB)
endif(WIN32)

find_package(GDAL ${_GDAL_VERSION})
if(GDAL_FOUND)
  list (APPEND DEP_LIBS ${GDAL_LIBRARY})
  list (APPEND DEP_INCLUDES ${GDAL_INCLUDE_DIR})
endif(GDAL_FOUND)

set (
  DEP_LIBS
  terralib_common
  terralib_dataaccess
  terralib_gdal
  terralib_geometry
  terralib_ogr
  terralib_plugin
  terralib_raster
  terralib_rp
  ${CPPUNIT_LIBRARY}
  ${GDAL_LIBRARY}
  )

# Select the source and header files
set(SRCS ${SRCDIR}/TsClassifier.cpp ${SRCDIR}/TsClassifierMain.cpp)
set(HDRS ${SRCDIR}/Config.h ${SRCDIR}/LoadModules.h ${SRCDIR}/TsClassifier.h)

# Select the include directories
include_directories(
  ${ROOT}/src
  ${DEP_INCLUDES}
  ${CPPUNIT_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  )

add_executable(${PROJ_NAME} ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries (${PROJ_NAME} ${DEP_LIBS})
