# This script builds the xerces example

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")  # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/examples/xerces)               # directory where the source files of the xerces example are located
set(INCLDIR ${SRCDIR})                            # directory where the header files of the xerces example are located

# Set the project name
set(PROJ_NAME "terralib_example_xerces")
project(${PROJ_NAME})         # the project name is terralib_example_xerces
SET(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_xerces

#Definitions for Windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB)
endif()
	
set(DEP_LIBS 
	terralib_common
	terralib_plugin
	terralib_xml
    )  

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the include directories
include_directories(
    ${TE_INCLUDE_DIRS}
	${DEP_INCLUDES}
    ${Boost_INCLUDE_DIRS}
)
		      
add_executable(${PROJ_NAME} ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${PROJ_NAME} ${DEP_LIBS})

copyLogFiles(${CMAKE_CURRENT_BINARY_DIR})
