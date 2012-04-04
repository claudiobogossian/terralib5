# This script builds the terralib st module for spatiotemporal data handling
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations

set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")     	# the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terralib/st)               		# directory where the source files of the terralib st module are located
set(INCLDIR ${SRCDIR})                               	# directory where the header files of the terralib st module are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_st")
project(${PROJ_NAME})           # the project name is terralib_st
set(LIB_NAME ${PROJ_NAME})      # the library name is terralib_st

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

find_package(Boost 1.44 COMPONENTS date_time REQUIRED)
if(Boost_FOUND)
  if(WIN32)
	  set(BOOST_LIBRARY
		  optimized ${Boost_DATE_TIME_LIBRARY_RELEASE}
		  debug     ${Boost_DATE_TIME_LIBRARY_DEBUG})
    set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIR})
    set(DEP_LIBS ${DEP_LIBS} ${BOOST_LIBRARY})
  else(WIN32)
    set(DEP_LIBS ${DEP_LIBS} ${Boost_LIBRARIES})
    set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
  endif(WIN32)    
endif(Boost_FOUND)

# Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESTDLL -DBOOST_ALL_NO_LIB)
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
		optimized terralib_datatype
		optimized terralib_geometry
		optimized terralib_raster
		debug     terralib_common_d
		debug     terralib_datatype_d
		debug     terralib_geometry_d
		debug     terralib_raster_d)
    
link_libraries(${DEP_LIBS})

# Select the source files
file(GLOB SRCS            		${SRCDIR}/*.cpp)
file(GLOB COVERAGE_SRCS 		${SRCDIR}/coverage/*.cpp)
file(GLOB INTERPOLATOR_SRCS    	${SRCDIR}/interpolator/*.cpp)
file(GLOB MOVINGOBJECT_SRCS    	${SRCDIR}/movingobject/*.cpp)
file(GLOB OBSERVATION_SRCS      ${SRCDIR}/observation/*.cpp)
file(GLOB TIMESERIES_SRCS   	${SRCDIR}/timeseries/*.cpp)

# Select the header files
file(GLOB HDRS            		${SRCDIR}/*.h)
file(GLOB COVERAGE_HDRS 		${SRCDIR}/coverage/*.h)
file(GLOB INTERPOLATOR_HDRS    	${SRCDIR}/interpolator/*.h)
file(GLOB MOVINGOBJECT_HDRS    	${SRCDIR}/movingobject/*.h)
file(GLOB OBSERVATION_HDRS      ${SRCDIR}/observation/*.h)
file(GLOB TIMESERIES_HDRS   	${SRCDIR}/timeseries/*.h)

# Group the source and header files
source_group("Source Files"             	FILES ${SRCS})
source_group("coverage\\Source Files"    	FILES ${COVERAGE_SRCS})
source_group("interpolator\\Source Files" 	FILES ${INTERPOLATOR_SRCS})
source_group("movingobject\\Source Files"   FILES ${MOVINGOBJECT_SRCS})
source_group("observation\\Source Files"    FILES ${OBSERVATION_SRCS})
source_group("timeseries\\Source Files"    	FILES ${TIMESERIES_SRCS})
source_group("Header Files"             	FILES ${HDRS})
source_group("coverage\\Header Files"    	FILES ${COVERAGE_HDRS})
source_group("interpolator\\Header Files" 	FILES ${INTERPOLATOR_HDRS})
source_group("movingobject\\Header Files"   FILES ${MOVINGOBJECT_HDRS})
source_group("observation\\Header Files"   	FILES ${OBSERVATION_HDRS})
source_group("timeseries\\Header Files"    	FILES ${TIMESERIES_HDRS})

# Select the include directories
include_directories(  ${SRCDIR}/coverage
	                  ${SRCDIR}/interpolator
	                  ${SRCDIR}/movingobject
	                  ${SRCDIR}/observation
					  ${SRCDIR}/timeseries
	                  ${DEP_INCLUDES})
					  
add_library(${LIB_NAME} SHARED
            ${SRCS} ${HDRS}
            ${COVERAGE_SRCS} ${COVERAGE_HDRS}
            ${INTERPOLATOR_SRCS} ${INTERPOLATOR_HDRS}
            ${MOVINGOBJECT_SRCS} ${MOVINGOBJECT_HDRS} 
			${OBSERVATION_SRCS} ${OBSERVATION_HDRS} 
            ${TIMESERIES_SRCS} ${TIMESERIES_HDRS})					  
					  
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)	  
					  
add_dependencies(${PROJ_NAME}
                 terralib_common
	             terralib_datatype
				 terralib_raster
	             terralib_geometry)
