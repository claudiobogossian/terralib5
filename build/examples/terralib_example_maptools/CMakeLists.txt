# This script builds the maptools example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")  # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/examples/maptools)             # directory where the source files of the maptools example are located
set(INCLDIR ${SRCDIR})                            # directory where the header files of the maptools example are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_example_maptools")
project(${PROJ_NAME})         # the project name is terralib_example_maptools
SET(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_maptools

if(WIN32)
	if(MSVC)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${ROOT}/bin32/msvc2010")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ROOT}/bin32/msvc2010")
	endif(MSVC)
else(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT}/bin32")
endif(WIN32)

find_package(Qt4)
if(QT4_FOUND)
	set(QT_INCLUDE_DIR "${QT_QTCORE_INCLUDE_DIR}/..")                     # qt/include	
	set(QT_INCLUDE_DIR ${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR})        # qt/include/QtCore
	set(QT_INCLUDE_DIR ${QT_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})         # qt/include/QtGui
	set(DEP_INCLUDES   ${DEP_INCLUDES} ${QT_INCLUDE_DIR})
	
	set(QT_LIBRARY ${QT_LIBRARY} ${QT_QTMAIN_LIBRARY})
	set(QT_LIBRARY ${QT_LIBRARY} ${QT_QTCORE_LIBRARY})
	set(QT_LIBRARY ${QT_LIBRARY} ${QT_QTGUI_LIBRARY})
	set(DEP_LIBS   ${DEP_LIBS} ${QT_LIBRARY})	
endif(QT4_FOUND)

find_package(Boost 1.44 REQUIRED)
if(Boost_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

#Definitions for Windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB
		              -DUNICODE
		              -DQT_LARGEFILE_SUPPORT
		              -DQT_DLL
		              -DQT_GUI_LIB
		              -DQT_CORE_LIB
		              -DQT_HAVE_MMX
		              -DQT_HAVE_3DNOW
		              -DQT_HAVE_SSE
		              -DQT_HAVE_MMXEXT
		              -DQT_HAVE_SSE2
		              -DQT_THREAD_SUPPORT)
endif(WIN32)
	
#Set the TerraLib modules to be linked
if(WIN32)
  link_directories(${ROOT}/lib32/msvc2010)
else(WIN32)
  link_directories(${ROOT}/lib32)
endif(WIN32)

set(DEP_LIBS  ${DEP_LIBS}
		optimized terralib_annotationtext
		optimized terralib_color
		optimized terralib_common
		optimized terralib_dataaccess
		optimized terralib_datatype
		optimized terralib_filter
		optimized terralib_geometry
		optimized terralib_gml
		optimized terralib_maptools
		optimized terralib_plugin
		optimized terralib_qtwidgets
		optimized terralib_raster
		optimized terralib_se
		optimized terralib_srs
		optimized terralib_xlink
		debug     terralib_annotationtext_d
		debug     terralib_color_d
		debug     terralib_common_d
		debug     terralib_dataaccess_d
		debug     terralib_datatype_d
		debug     terralib_filter_d
		debug     terralib_geometry_d
		debug     terralib_gml_d
		debug     terralib_maptools_d
		debug     terralib_plugin_d
		debug     terralib_qtwidgets_d
		debug     terralib_raster_d
		debug     terralib_se_d
		debug     terralib_srs_d
		debug     terralib_xlink_d)  

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS ${QT_DEFINITIONS})
endif(WIN32)

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
            