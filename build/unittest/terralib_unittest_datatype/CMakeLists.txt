# This script builds the datatype unit test 
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")        # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/unittest/datatype)                   # directory where the source files of the datatype unit test are located
set(INCLDIR ${SRCDIR})                                  # directory where the header files of the datatype unit test are located

# Set the project name
set(PROJ_NAME "terralib_unittest_datatype")
project(${PROJ_NAME})           # the project name is terralib_unittest_datatype
set(LIB_NAME ${PROJ_NAME})      # the library name is terralib_unittest_datatype

find_package(Boost ${_Boost_VERSION} COMPONENTS system REQUIRED)

set (DEP_INCLUDES 
  ${Boost_INCLUDE_DIRS} 
  ${CPPUNIT_INCLUDE_DIR}
  ${TE_INCLUDE_DIRS}
  ) 
set (DEP_LIBS ${CPPUNIT_LIBRARY} ${Boost_LIBRARIES})	

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (
  APPEND DEP_LIBS  
  terralib_common
	terralib_datatype
	terralib_geometry
	terralib_srs
  )

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

list(REMOVE_ITEM SRCS ${SRCDIR}/TsDateTime.cpp)
list(REMOVE_ITEM HDRS ${INCLDIR}/TsDateTime.h)

# Select the include directories
include_directories(${ROOT}/src ${DEP_INCLUDES})

add_executable(${PROJ_NAME} ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries (${PROJ_NAME} ${DEP_LIBS})
               