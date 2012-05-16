# This script builds the terralib geometry module
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")     # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terralib/geometry)            # directory where the source files of the terralib geometry module are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the terralib geometry module are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_geometry")
project(${PROJ_NAME})               # the project name is terralib_geometry
set(LIB_NAME ${PROJ_NAME})          # the library name is terralib_geometry

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

find_package(Boost 1.44.0 REQUIRED)
if(Boost_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

find_package(Geos)
if(GEOS_FOUND)
	set(DEP_INCLUDES ${DEP_INCLUDES} ${GEOS_INCLUDE_DIR})
	set(DEP_LIBS ${DEP_LIBS} ${GEOS_LIBRARY})
ENDIF(GEOS_FOUND)

#Definitions for windows compiling
add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEGEOMDLL -DBOOST_ALL_NO_LIB)

#Set the TerraLib modules to be linked
if(WIN32)
  link_directories(${ROOT}/lib32/msvc2010)
else(WIN32)
  link_directories(${ROOT}/lib32)
endif(WIN32)

set(DEP_LIBS  ${DEP_LIBS}
		optimized terralib_common
		optimized terralib_datatype
		optimized terralib_srs
		debug     terralib_common_d
		debug     terralib_datatype_d
		debug     terralib_srs_d)

file(GLOB SRCS ${SRCDIR}/*.cpp)

file(GLOB HDRS ${SRCDIR}/*.h)
 
# Select the include directories
include_directories(${ROOT}/src ${DEP_INCLUDES})

link_libraries(${DEP_LIBS})
		
add_library(${LIB_NAME} SHARED ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

add_dependencies(${PROJ_NAME} terralib_common terralib_datatype terralib_srs)

if(WIN32)
  set(terralib-3rdparty-libs $ENV{TERRALIB_DEP_MSVC2010})
  string(REPLACE "\\" "/" terralib-3rdparty-libs ${terralib-3rdparty-libs})
  
  file(COPY
       "${terralib-3rdparty-libs}/geos/bin/win32/"
       DESTINATION "${ROOT}/bin32/msvc2010"
       FILES_MATCHING PATTERN "*.dll")
endif(WIN32)        
