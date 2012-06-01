# This script builds the dataaccess example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")  # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/examples/dataaccess)           # directory where the source files of the dataaccess example are located
set(INCLDIR ${SRCDIR})                            # directory where the header files of the dataaccess example are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_example_dataaccess")
project(${PROJ_NAME})         # the project name is terralib_example_dataaccess
set(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_dataaccess

if(WIN32)
	if(MSVC)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${ROOT}/bin32/msvc2010")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ROOT}/bin32/msvc2010")
	endif(MSVC)
else(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT}/bin32")
endif(WIN32)

find_package(Boost 1.44 REQUIRED)
if(Boost_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

find_package(Proj4)
if(PROJ4_FOUND)
	set(DEP_INCLUDES ${DEP_INCLUDES} ${PROJ4_INCLUDE_DIR})
endif(PROJ4_FOUND)

#Definitions for Windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB)
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
		optimized terralib_memory
		optimized terralib_plugin
		optimized terralib_raster
		optimized terralib_srs
		debug     terralib_common_d
		debug     terralib_dataaccess_d
		debug     terralib_datatype_d
		debug     terralib_geometry_d
		debug     terralib_memory_d
		debug     terralib_plugin_d
		debug     terralib_raster_d
		debug     terralib_srs_d)
		
# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the include directories
include_directories(${ROOT}/src
                    ${DEP_INCLUDES})
                          
add_executable(${PROJ_NAME} ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${PROJ_NAME} ${DEP_LIBS})

############### Get the data used for testing the example ###############

# Set the directory where the executable of
# this example is generated (EXEC_DIR)
if(WIN32)
  set(EXEC_DIR "${ROOT}/bin32/msvc2010")
else(WIN32)
  set(EXEC_DIR "${ROOT}/bin32")
endif(WIN32)

# Check if there is data in the EXEC_DIR
set(EXEC_DIR_DATA_EXISTS FALSE)
if(EXISTS ${EXEC_DIR}/data)
  set(EXEC_DIR_DATA_EXISTS TRUE)
endif(EXISTS ${EXEC_DIR}/data)

# Check if there is data in the current
# build directory (CURRENT_BINARY_DIR)
set(BUILD_DIR_DATA_EXISTS FALSE)
if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/data)
  set(BUILD_DIR_DATA_EXISTS TRUE)
endif(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/data)

if(EXEC_DIR_DATA_EXISTS)
  if(NOT BUILD_DIR_DATA_EXISTS)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/data")
    file(COPY "${EXEC_DIR}/data/"
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/data")
  endif(NOT BUILD_DIR_DATA_EXISTS)
else(EXEC_DIR_DATA_EXISTS)
  if(BUILD_DIR_DATA_EXISTS)
    file(MAKE_DIRECTORY "${EXEC_DIR}/data")
    file(COPY "${CMAKE_CURRENT_BINARY_DIR}/data/"
         DESTINATION "${EXEC_DIR}/data")
  else(BUILD_DIR_DATA_EXISTS)
    file(DOWNLOAD "http://www.dpi.inpe.br/terralib5/data.tar.gz"  ${CMAKE_CURRENT_BINARY_DIR}/data.tar.gz)
                    
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf data.tar.gz
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
                    
    file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/data.tar.gz)

    # Copy the data into the EXEC_DIR
    file(MAKE_DIRECTORY "${EXEC_DIR}/data")
    file(COPY "${CMAKE_CURRENT_BINARY_DIR}/data/"
         DESTINATION "${EXEC_DIR}/data")    
  endif(BUILD_DIR_DATA_EXISTS) 
endif(EXEC_DIR_DATA_EXISTS)
