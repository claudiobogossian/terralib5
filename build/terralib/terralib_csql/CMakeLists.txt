# This script builds the terralib csql module
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")      # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terralib/csql)                 # directory where the source files of the terralib csql module are located
set(INCLDIR ${SRCDIR})                                # directory where the header files of the terralib csql module are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_csql")
project(${PROJ_NAME})           # the project name is terralib_csql
set(LIB_NAME ${PROJ_NAME})      # the library name is terralib_csql

if(WIN32)
	if(MSVC)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ROOT}/bin32/msvc2010")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ROOT}/bin32/msvc2010")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${ROOT}/lib32/msvc2010")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${ROOT}/lib32/msvc2010")
	endif(MSVC)
else(WIN32)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ROOT}/lib32")
endif(WIN32)

find_package(Boost 1.44 REQUIRED)
if(Boost_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

find_package(CSQL)
if(CSQL_FOUND)
	set(DEP_LIBS ${DEP_LIBS} ${CSQL_LIBRARY})
	set(DEP_INCLUDES ${DEP_INCLUDES} ${CSQL_INCLUDE_DIR})
endif(CSQL_FOUND)

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTECSQLDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

#Set the TerraLib modules to be linked
if(WIN32)
  link_directories(${ROOT}/lib32/msvc2010)
else(WIN32)
  link_directories(${ROOT}/lib32)
endif(WIN32)

set(DEP_LIBS  ${DEP_LIBS}
		optimized terralib_common
		optimized terralib_dataaccess
		optimized terralib_datatype
		optimized terralib_geometry
		optimized terralib_plugin
		optimized terralib_raster
		optimized terralib_srs
		debug     terralib_common_d
		debug     terralib_dataaccess_d
		debug     terralib_datatype_d
		debug     terralib_geometry_d
		debug     terralib_plugin_d
		debug     terralib_raster_d
		debug     terralib_srs_d)

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

#list(REMOVE_ITEM HDRS ${INCLDIR}/Module.h)
#list(REMOVE_ITEM SRCS ${SRCDIR}/Module.cpp)

# Select the include directories
include_directories(${ROOT}/src ${DEP_INCLUDES})

link_libraries(${DEP_LIBS})
		
add_library(${LIB_NAME} SHARED ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

add_dependencies(${PROJ_NAME}
                 terralib_common
			           terralib_dataaccess
		             terralib_datatype
		             terralib_geometry
		             terralib_plugin
		             terralib_raster
                 terralib_srs)

if(WIN32)
  set(terralib-3rdparty-libs $ENV{TERRALIB_DEP_MSVC2010})
  string(REPLACE "\\" "/" terralib-3rdparty-libs ${terralib-3rdparty-libs})
  
  file(COPY
       "${terralib-3rdparty-libs}/csql"
       DESTINATION "${ROOT}/bin32/msvc2010"
       FILES_MATCHING PATTERN "*.dll")
endif(WIN32)
 