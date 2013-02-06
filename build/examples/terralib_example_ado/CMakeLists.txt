# This script builds the ADO example
# The variable ROOT contains the root directory where terralib is installed

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")     # the terralib root directory (change it if it is in a different place)
set(SRCDIR ${ROOT}/examples/ado)                     # directory where the source files of the ADO example are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the ADO example are located

# Set the project name
set(PROJ_NAME "terralib_example_ado")
project(${PROJ_NAME})         # the project name is terralib_example_ado
SET(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_ado

find_package(Boost ${_Boost_VERSION} COMPONENTS system REQUIRED)

#Definitions for Windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB -DUNICODE)
endif()

set(DEP_LIBS
    terralib_ado  
	terralib_common
	terralib_dataaccess
	terralib_datatype
	terralib_geometry
	terralib_plugin
	terralib_raster
	terralib_srs
	${Boost_LIBRARIES}
    )
 	
# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the include directories
include_directories(
    ${TE_INCLUDE_DIRS}
    "${CMAKE_CURRENT_BINARY_DIR}"
    ${DEP_INCLUDES}
    ${Boost_INCLUDE_DIRS}
)

add_executable(${PROJ_NAME} ${SRCS} ${HDRS} ${UI} ${MOC})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${PROJ_NAME} ${DEP_LIBS})

copyLogFiles(${CMAKE_CURRENT_BINARY_DIR})
                  