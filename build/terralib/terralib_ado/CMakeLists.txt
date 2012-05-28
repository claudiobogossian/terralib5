# This script builds the terralib postgis module
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")     # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terralib/ado)             # directory where the source files of the terralib postgis module are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the terralib postgis module are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_ado")
project(${PROJ_NAME})      # the project name is terralib_postgis
set(LIB_NAME ${PROJ_NAME}) # the library name is terralib_postgis

if(MSVC)
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ROOT}/bin32/msvc2010")
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ROOT}/bin32/msvc2010")
	set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${ROOT}/lib32/msvc2010")
	set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${ROOT}/lib32/msvc2010")
endif(MSVC)

find_package(Boost 1.44.0 COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
	set(BOOST_LIBRARY
		optimized ${Boost_THREAD_LIBRARY_RELEASE}
		debug     ${Boost_THREAD_LIBRARY_DEBUG})
	set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIR})
	set(DEP_LIBS ${DEP_LIBS} ${BOOST_LIBRARY})
endif(Boost_FOUND)

find_package(ADO)
if(ADO_FOUND)
	set(DEP_INCLUDES ${DEP_INCLUDES} ${ADO_INCLUDE_DIR})
endif(ADO_FOUND)

#Definitions for windows compiling
add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEADODLL -DBOOST_ALL_NO_LIB)

link_directories(${ROOT}/lib32/msvc2010)

set(DEP_LIBS  ${DEP_LIBS}
		optimized terralib_common
		optimized terralib_dataaccess
		optimized terralib_datatype
		optimized terralib_plugin
		debug     terralib_common_d
		debug     terralib_dataaccess_d
		debug     terralib_datatype_d
		debug 	  terralib_plugin_d)
    
link_libraries(${DEP_LIBS})

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)
	
list(REMOVE_ITEM SRCS ${SRCDIR}/Module.cpp)
 
# Select the include directories
include_directories(${ROOT}/src ${DEP_INCLUDES})
		
add_library(${LIB_NAME} SHARED ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

add_dependencies(${PROJ_NAME}
                 terralib_common 
                 terralib_dataaccess
                 terralib_datatype
				 terralib_plugin)