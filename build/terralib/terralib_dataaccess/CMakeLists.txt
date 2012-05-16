# This script builds the terralib dataaccess module
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations

set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")     # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terralib/dataaccess)          # directory where the source files of the terralib dataaccess module are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the terralib dataaccess module are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Sets the project name
set (PROJ_NAME "terralib_dataaccess")
project(${PROJ_NAME})       # the project name is terralib_dataaccess
set(LIB_NAME ${PROJ_NAME})  # the library name is terralib_dataaccess

if(WIN32)
	if(MSVC)
		set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ROOT}/bin32/msvc2010")
		set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ROOT}/bin32/msvc2010")
		set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${ROOT}/lib32/msvc2010")
		set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${ROOT}/lib32/msvc2010")
	endif(MSVC)
else(WIN32)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ROOT}/lib32")
endif(WIN32)

find_package(Boost 1.44.0 COMPONENTS thread REQUIRED)
if(Boost_FOUND)
  if(WIN32)
    set(BOOST_LIBRARY
      optimized ${Boost_THREAD_LIBRARY_RELEASE}
      debug     ${Boost_THREAD_LIBRARY_DEBUG})     
    set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIR})
    set(DEP_LIBS ${DEP_LIBS} ${BOOST_LIBRARY})
  else(WIN32)
    set(DEP_LIBS ${DEP_LIBS} ${Boost_LIBRARIES})
    set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
  endif(WIN32)
endif(Boost_FOUND)

# Definitions for windows compiling
if(WIN32)
add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEDATAACCESSDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

#Set the TerraLib modules to be linked
if(WIN32)
  link_directories(${ROOT}/lib32/msvc2010)
else(WIN32)
  link_directories(${ROOT}/lib32)
endif(WIN32)

set(DEP_LIBS  ${DEP_LIBS}
		optimized terralib_common
		optimized terralib_datatype
		optimized terralib_geometry
		optimized terralib_raster
		optimized terralib_srs
		debug     terralib_common_d
		debug     terralib_datatype_d
		debug     terralib_geometry_d
		debug     terralib_raster_d
		debug     terralib_srs_d)

link_libraries(${DEP_LIBS})

# Select the source files
file(GLOB SRCS            ${SRCDIR}/*.cpp)
file(GLOB DATASET_SRCS    ${SRCDIR}/dataset/*.cpp)
file(GLOB DATASOURCE_SRCS ${SRCDIR}/datasource/*.cpp)
file(GLOB QUERY_SRCS      ${SRCDIR}/query/*.cpp)

# Select the header files
file(GLOB HDRS            ${SRCDIR}/*.h)
file(GLOB DATASET_HDRS    ${SRCDIR}/dataset/*.h)
file(GLOB DATASOURCE_HDRS ${SRCDIR}/datasource/*.h)
file(GLOB QUERY_HDRS      ${SRCDIR}/query/*.h)

# Group the source and header files
source_group("Source Files"             FILES ${SRCS})
source_group("dataset\\Source Files"    FILES ${DATASET_SRCS})
source_group("datasource\\Source Files" FILES ${DATASOURCE_SRCS})
source_group("query\\Source Files"      FILES ${QUERY_SRCS})
source_group("Header Files"             FILES ${HDRS})
source_group("dataset\\Header Files"    FILES ${DATASET_HDRS})
source_group("datasource\\Header Files" FILES ${DATASOURCE_HDRS})
source_group("query\\Header Files"      FILES ${QUERY_HDRS})
 
# Select the include directories
include_directories(${SRCDIR}/dataset
	                  ${SRCDIR}/datasource
	                  ${SRCDIR}/query
	                  ${DEP_INCLUDES})
		
add_library(${LIB_NAME} SHARED
            ${SRCS} ${HDRS}
            ${DATASET_SRCS} ${DATASET_HDRS}
            ${DATASOURCE_SRCS} ${DATASOURCE_HDRS}
            ${QUERY_SRCS} ${QUERY_HDRS})
            
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

add_dependencies(${PROJ_NAME}
                 terralib_common 
	               terralib_datatype
	               terralib_geometry
	               terralib_raster
	               terralib_srs)
