# This script builds the cppplugin_helloworld example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")      # the terralib root directory (change it if it is located in another place)
set(SRCDIR ${ROOT}/examples/cppplugin/helloworld)     # directory where the source files of the cppplugin_hellowworld example are located
set(INCLDIR ${SRCDIR})                                # directory where the header files of the cppplugin_hellowworld example are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_example_cppplugin_helloworld")
project(${PROJ_NAME})           # the project name is terralib_example_cppplugin_helloworld
set(LIB_NAME ${PROJ_NAME})      # the library name is terralib_example_cppplugin_helloworld

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

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESRSDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

#Set the TerraLib modules to be linked
if(WIN32)
  link_directories(${ROOT}/lib32/msvc2010)
else(WIN32)
  link_directories(${ROOT}/lib32)
endif(WIN32)

set(DEP_LIBS  ${DEP_LIBS}
		optimized terralib_common
		optimized terralib_plugin
		debug     terralib_common_d
		debug     terralib_plugin_d)

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the include directories
include_directories(${ROOT}/src ${DEP_INCLUDES})

link_libraries(${DEP_LIBS})

set(CMAKE_DEBUG_POSTFIX _d)

add_library(${LIB_NAME} SHARED ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
