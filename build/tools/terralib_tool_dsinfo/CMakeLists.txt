# This script builds the terralib library
# (c) Felipe V. Siqueira, september 2011
#
# To use it, you should adjust ROOT 
# change ROOT to the directory where terralib source is located

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..") 				# change this if terralib is somewhere else
set(SRCDIR ${ROOT}/src/terralib/tools/dsinfo) 						# this is the directory where terralib sources will be located
set(INCLDIR ${SRCDIR}) 												# this is the directory where terralib header files will be located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_tool_dsinfo")
project(${PROJ_NAME}) # the project name is terralib_tool_dsinfo

if(WIN32)
	if(MSVC)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ROOT}/bin32/msvc2010")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ROOT}/bin32/msvc2010")
	endif(MSVC)
endif(WIN32)

find_package(Boost 1.44.0 COMPONENTS program_options REQUIRED)
if(Boost_FOUND)
  if(WIN32)
    set(BOOST_LIBRARY
      optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE}
      debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG})
    set (DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIR})
    set (DEP_LIBS ${DEP_LIBS} ${BOOST_LIBRARY})
  else(WIN32)
    set (DEP_LIBS ${DEP_LIBS} ${Boost_LIBRARIES})
    set (DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
  endif(WIN32)
endif(Boost_FOUND)

#Add Ws2_32.lib for Windows
if(WIN32)
  set (DEP_LIBS ${DEP_LIBS} Ws2_32.lib)
endif(WIN32)

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB -DBOOST_PROGRAM_OPTIONS_DYN_LINK)
endif(WIN32)

if(WIN32)
	set(DEP_LIBS ${DEP_LIBS}
		optimized	"${ROOT}/lib32/msvc2010/terralib_common.lib"
		optimized	"${ROOT}/lib32/msvc2010/terralib_datatype.lib"
		optimized	"${ROOT}/lib32/msvc2010/terralib_dataaccess.lib"	
		optimized	"${ROOT}/lib32/msvc2010/terralib_plugin.lib"	
		optimized	"${ROOT}/lib32/msvc2010/terralib_postgis.lib"	
		optimized	"${ROOT}/lib32/msvc2010/terralib_gdal.lib"		
		debug		"${ROOT}/lib32/msvc2010/terralib_common_d.lib"
		debug     	"${ROOT}/lib32/msvc2010/terralib_datatype_d.lib"
		debug 		"${ROOT}/lib32/msvc2010/terralib_dataaccess_d.lib"	
		debug 		"${ROOT}/lib32/msvc2010/terralib_plugin_d.lib"	
		debug 		"${ROOT}/lib32/msvc2010/terralib_postgis_d.lib"	
		debug 		"${ROOT}/lib32/msvc2010/terralib_gdal_d.lib")
endif(WIN32)  



# Select the header files
file(GLOB HDRS
	${SRCDIR}/console/*.h
	${SRCDIR}/core/*.h
	)
	
# Select the source files
file(GLOB SRCS 
	${SRCDIR}/console/*.cpp
	${SRCDIR}/core/*.cpp
	)

# Select the include files
include_directories(
	${ROOT}/src
	${SRCDIR}/console
	${SRCDIR}/core
	${DEP_INCLUDES}
	)

link_libraries(${DEP_LIBS})

add_executable(${PROJ_NAME} ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES OUTPUT_NAME "te_dsinfo" DEBUG_POSTFIX _d)

if(WIN32)
  set (CompilerVersion vc100)
  set (terralib-3rdparty-libs $ENV{TERRALIB_DEP_MSVC2010})
  string(REPLACE "\\" "/" terralib-3rdparty-libs ${terralib-3rdparty-libs})
  
  file(MAKE_DIRECTORY "${ROOT}/bin32/msvc2010")

  file(COPY
       "${terralib-3rdparty-libs}/boost/lib/win32/boost_program_options-${CompilerVersion}-mt-gd-${Boost_LIB_VERSION}.dll"
       "${terralib-3rdparty-libs}/boost/lib/win32/boost_program_options-${CompilerVersion}-mt-${Boost_LIB_VERSION}.dll"
       DESTINATION "${ROOT}/bin32/msvc2010"
	   )                    
endif(WIN32)
