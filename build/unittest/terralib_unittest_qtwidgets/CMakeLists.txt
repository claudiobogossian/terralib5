# This script builds the qtwidgets unit test 
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")    # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/unittest/qtwidgets)              # directory where the source files of the qtwidgets unit test are located
set(INCLDIR ${SRCDIR})                              # directory where the header files of the qtwidgets unit test are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_unittest_qtwidgets")
project(${PROJ_NAME})           # the project name is terralib_unittest_qtwidgets
set(LIB_NAME ${PROJ_NAME})      # the library name is terralib_unittest_qtwidgets

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

find_package(Boost 1.44 REQUIRED)
if(Boost_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
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

find_package(Postgresql)
if(POSTGRESQL_FOUND)
	set(DEP_LIBS ${DEP_LIBS} ${POSTGRESQL_LIBRARY})
	set(DEP_INCLUDES ${DEP_INCLUDES} ${POSTGRESQL_INCLUDE_DIR})
endif(POSTGRESQL_FOUND)

find_package(Qt4)
if(QT4_FOUND)
	set(QT_INCLUDE_DIR "${QT_QTCORE_INCLUDE_DIR}/..")                     #qt/include	
	set(QT_INCLUDE_DIR ${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR})        #qt/include/QtCore
	set(QT_INCLUDE_DIR ${QT_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})         #qt/include/QtGui
	set(QT_INCLUDE_DIR ${QT_INCLUDE_DIR} ${QT_QAXCONTAINER_INCLUDE_DIR})  #qt/include/ActiveQt
	set(DEP_INCLUDES   ${DEP_INCLUDES} ${QT_INCLUDE_DIR})
	
	set(QT_LIBRARY ${QT_QTCORE_LIBRARY})
	set(QT_LIBRARY ${QT_LIBRARY} ${QT_QTMAIN_LIBRARY})
	set(QT_LIBRARY ${QT_LIBRARY} ${QT_QTGUI_LIBRARY})
	set(QT_LIBRARY ${QT_LIBRARY} ${QT_QTSVG_LIBRARY})
	set(DEP_LIBS   ${DEP_LIBS} ${QT_LIBRARY})	
endif(QT4_FOUND)

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
		optimized terralib_postgis
		optimized terralib_qtwidgets
		optimized terralib_raster
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
		debug     terralib_postgis_d
		debug     terralib_qtwidgets_d
		debug     terralib_raster_d
		debug     terralib_srs_d
		debug     terralib_xlink_d)
    
# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the include directories
include_directories(${ROOT}/src ${DEP_INCLUDES})

link_libraries(${DEP_LIBS})
		
add_executable(${PROJ_NAME} ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

if(WIN32)
  set(terralib-3rdparty-libs $ENV{TERRALIB_DEP_MSVC2010})
  string(REPLACE "\\" "/" terralib-3rdparty-libs ${terralib-3rdparty-libs})
  
  file(COPY
       "${terralib-3rdparty-libs}/cppunit/src/cppunit/x86/ReleaseDll/cppunit_dll.dll"
       "${terralib-3rdparty-libs}/cppunit/src/cppunit/x86/DebugDll/cppunitd_dll.dll"
       DESTINATION "${ROOT}/bin32/msvc2010")
endif(WIN32)
 