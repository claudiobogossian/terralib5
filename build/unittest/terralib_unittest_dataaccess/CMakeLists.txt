# This script builds the dataaccess unit test 
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")    # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/unittest/dataaccess)             # directory where the source files of the dataaccess unit test are located
set(INCLDIR ${SRCDIR})                              # directory where the header files of the dataaccess unit test are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_unittest_dataaccess")
project(${PROJ_NAME})           # the project name is terralib_unittest_dataaccess
set(LIB_NAME ${PROJ_NAME})      # the library name is terralib_unittest_dataaccess

if(WIN32)
	if(MSVC)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ROOT}/bin32/msvc2010")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ROOT}/bin32/msvc2010")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${ROOT}/lib32/msvc2010")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${ROOT}/lib32/msvc2010")
	endif(MSVC)
else(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT}/bin32")
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

find_package(CppUnit)
if(CPPUNIT_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${CPPUNIT_INCLUDE_DIR})

  if(WIN32)
    set(CPPUNIT_LIBRARY
      optimized ${CPPUNIT_LIBRARY_RELEASE}
      debug     ${CPPUNIT_LIBRARY_DEBUG})
  else(WIN32)
    set(CPPUNIT_LIBRARY ${CPPUNIT_LIBRARY})
  endif(WIN32)
  
  set(DEP_LIBS ${DEP_LIBS} ${CPPUNIT_LIBRARY})	
endif(CPPUNIT_FOUND)

find_package(GDAL)
if(GDAL_FOUND)
	set(DEP_LIBS ${DEP_LIBS} ${GDAL_LIBRARY})
	set(DEP_INCLUDES ${DEP_INCLUDES} ${GDAL_INCLUDE_DIR})
endif(GDAL_FOUND)

find_package(Postgresql)
if(POSTGRESQL_FOUND)
	set(DEP_LIBS ${DEP_LIBS} ${POSTGRESQL_LIBRARY})
	set(DEP_INCLUDES ${DEP_INCLUDES} ${POSTGRESQL_INCLUDE_DIR})
endif(POSTGRESQL_FOUND)

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB)
endif(WIN32)

#Set the TerraLib modules to be linked
if(WIN32)
  link_directories(${ROOT}/lib32/msvc2010)
elseif(APPLE)
  link_directories(${ROOT}/lib32/Release ${ROOT}/lib32/Debug)
elseif(UNIX)
  link_directories(${ROOT}/lib32)
endif(WIN32)

set(DEP_LIBS  ${DEP_LIBS}
		optimized terralib_common
		optimized terralib_dataaccess
		optimized terralib_datatype
		optimized terralib_gdal
		optimized terralib_geometry
		optimized terralib_ogr
		optimized terralib_plugin
		optimized terralib_postgis
		optimized terralib_raster
		optimized terralib_srs
		debug     terralib_common_d
		debug     terralib_dataaccess_d
		debug     terralib_datatype_d
		debug     terralib_gdal_d
		debug     terralib_geometry_d
		debug     terralib_ogr_d
		debug     terralib_plugin_d
		debug     terralib_postgis_d
		debug     terralib_raster_d
		debug     terralib_srs_d)
    
# Select the source files
file(GLOB DATAACCESS_SRCS ${SRCDIR}/*.cpp)
file(GLOB GDAL_SRCS       ${SRCDIR}/../gdal/*.cpp)
file(GLOB PGIS_SRCS       ${SRCDIR}/../postgis/*.cpp)

#Select the header files
file(GLOB DATAACCESS_HDRS ${SRCDIR}/*.h)
file(GLOB GDAL_HDRS       ${SRCDIR}/../gdal/*.h)
file(GLOB PGIS_HDRS       ${SRCDIR}/../postgis/*.h)

# Remove some files from the source and header files selected
list(REMOVE_ITEM DATAACCESS_HDRS ${SRCDIR}/Config.h)   
list(REMOVE_ITEM GDAL_SRCS ${SRCDIR}/../gdal/main.cpp)
list(REMOVE_ITEM PGIS_SRCS ${SRCDIR}/../postgis/main.cpp ${SRCDIR}/../postgis/TsPostGISInfo.cpp)
list(REMOVE_ITEM PGIS_HDRS ${SRCDIR}/../postgis/TsPostGISInfo.h)

# Group the source and header files
source_group("dataaccess\\Source Files" FILES ${DATAACCESS_SRCS})
source_group("gdal\\Source Files"       FILES ${GDAL_SRCS})
source_group("postgis\\Source Files"    FILES ${PGIS_SRCS})

source_group("dataaccess\\Header Files" FILES ${DATAACCESS_HDRS})
source_group("gdal\\Header Files"       FILES ${GDAL_HDRS})
source_group("postgis\\Header Files"    FILES ${PGIS_HDRS})

# Select the include directories
include_directories(${ROOT}/src ${DEP_INCLUDES})

link_libraries(${DEP_LIBS})
		
add_executable(${PROJ_NAME} ${DATAACCESS_SRCS} ${DATAACCESS_HDRS}
               ${GDAL_SRCS} ${GDAL_HDRS}
               ${PGIS_SRCS} ${PGIS_HDRS})
               
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

if(WIN32)
  set(terralib-3rdparty-libs $ENV{TERRALIB_DEP_MSVC2010})
  string(REPLACE "\\" "/" terralib-3rdparty-libs ${terralib-3rdparty-libs})
  
  file(COPY
       "${terralib-3rdparty-libs}/cppunit/src/cppunit/x86/ReleaseDll/cppunit_dll.dll"
       "${terralib-3rdparty-libs}/cppunit/src/cppunit/x86/DebugDll/cppunitd_dll.dll"
       "${ROOT}/unittest/report.xsl"
       DESTINATION "${ROOT}/bin32/msvc2010")
else(WIN32)
  file(COPY
       "${ROOT}/unittest/report.xsl"
       DESTINATION "${ROOT}/bin32")      
endif(WIN32)

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
 