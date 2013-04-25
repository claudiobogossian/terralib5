# This script builds the dataaccess unit test 
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")    # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/unittest/dataaccess)             # directory where the source files of the dataaccess unit test are located
set(INCLDIR ${SRCDIR})                              # directory where the header files of the dataaccess unit test are located

# Set the project name
set(PROJ_NAME "terralib_unittest_dataaccess")
project(${PROJ_NAME})           # the project name is terralib_unittest_dataaccess
set(LIB_NAME ${PROJ_NAME})      # the library name is terralib_unittest_dataaccess

set (DEP_LIBS ${Boost_LIBRARIES})
set (DEP_INCLUDES 
  ${Boost_INCLUDE_DIRS}
  ${TE_INCLUDE_DIRS}
  )

if(CPPUNIT_FOUND)
  list (APPEND DEP_INCLUDES ${CPPUNIT_INCLUDE_DIR})
  list (APPEND DEP_LIBS ${CPPUNIT_LIBRARY})	
endif()

if(GDAL_FOUND)
	list (APPEND DEP_LIBS ${GDAL_LIBRARY})
	list (APPEND DEP_INCLUDES ${GDAL_INCLUDE_DIR})
endif()

if(PostgreSQL_FOUND)
	list (APPEND DEP_LIBS ${PostgreSQL_LIBRARY})
	list (APPEND DEP_INCLUDES ${PostgreSQL_INCLUDE_DIR})
endif()


find_package(Boost ${_Boost_VERSION} COMPONENTS system REQUIRED)

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB)
endif()

list (
  APPEND DEP_LIBS  
  terralib_common
	terralib_dataaccess
	terralib_datatype
	terralib_geometry
	terralib_plugin
	terralib_raster
	terralib_srs
    ${Boost_LIBRARIES}	
  )

# Select the source files
file(GLOB DATAACCESS_SRCS ${SRCDIR}/*.cpp)
#file(GLOB GDAL_SRCS       ${SRCDIR}/../gdal/*.cpp)
#file(GLOB PGIS_SRCS       ${SRCDIR}/../postgis/*.cpp)

#Select the header files
file(GLOB DATAACCESS_HDRS ${SRCDIR}/*.h)
#file(GLOB GDAL_HDRS       ${SRCDIR}/../gdal/*.h)
#file(GLOB PGIS_HDRS       ${SRCDIR}/../postgis/*.h)

# Remove some files from the source and header files selected
list(REMOVE_ITEM DATAACCESS_HDRS ${SRCDIR}/Config.h)   
#list(REMOVE_ITEM GDAL_SRCS ${SRCDIR}/../gdal/main.cpp)
#list(REMOVE_ITEM PGIS_SRCS ${SRCDIR}/../postgis/main.cpp ${SRCDIR}/../postgis/TsPostGISInfo.cpp)
#list(REMOVE_ITEM PGIS_HDRS ${SRCDIR}/../postgis/TsPostGISInfo.h)

# Group the source and header files
source_group("dataaccess\\Source Files" FILES ${DATAACCESS_SRCS})
#source_group("gdal\\Source Files"       FILES ${GDAL_SRCS})
#source_group("postgis\\Source Files"    FILES ${PGIS_SRCS})

source_group("dataaccess\\Header Files" FILES ${DATAACCESS_HDRS})
#source_group("gdal\\Header Files"       FILES ${GDAL_HDRS})
#source_group("postgis\\Header Files"    FILES ${PGIS_HDRS})

# Select the include directories
include_directories(${ROOT}/src ${DEP_INCLUDES} )

add_executable(${PROJ_NAME} ${DATAACCESS_SRCS} ${DATAACCESS_HDRS} )
 #              ${GDAL_SRCS} ${GDAL_HDRS}
 #              ${PGIS_SRCS} ${PGIS_HDRS})
               
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries (${PROJ_NAME} ${DEP_LIBS})