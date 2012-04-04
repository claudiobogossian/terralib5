# This script builds the terralib java binding
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../..")   # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terralib/binding/java/jni)           # directory where the source files of the terralib java binding are located
set(INCLDIR ${SRCDIR})                                      # directory where the header files of the terralib java binding are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Modules")

# Set the project name
set(PROJ_NAME "terralib_binding_java")
project(${PROJ_NAME})           # the project name is terralib_binding_java
set(LIB_NAME ${PROJ_NAME})      # the library name is terralib_binding_java

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

find_package(Qt4)
if(QT4_FOUND)
	set(QT_INCLUDE_DIR "${QT_QTCORE_INCLUDE_DIR}/..")                     #qt/include	
	set(QT_INCLUDE_DIR ${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR})        #qt/include/QtCore
	set(QT_INCLUDE_DIR ${QT_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})         #qt/include/QtGui
	set(QT_INCLUDE_DIR ${QT_INCLUDE_DIR} ${QT_QAXCONTAINER_INCLUDE_DIR})  #qt/include/ActiveQt
	set(DEP_INCLUDES   ${DEP_INCLUDES} ${QT_INCLUDE_DIR})
endif(QT4_FOUND)

find_package(Boost 1.44 REQUIRED)
if(Boost_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

find_package(JNI)
if(JNI_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${JAVA_INCLUDE_PATH})
  set(DEP_INCLUDES ${DEP_INCLUDES} ${JAVA_INCLUDE_PATH2})
endif(JNI_FOUND)

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEJAVACOMMONDLL -DBOOST_ALL_DYN_LINK -DBOOST_ALL_NO_LIB)
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
		optimized terralib_binding_java_common
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
		optimized terralib_srs
		optimized terralib_se
		optimized terralib_xlink
		debug     terralib_annotationtext_d
		debug     terralib_binding_java_common_d
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
		debug     terralib_srs_d
		debug     terralib_se_d
		debug     terralib_xlink_d)

link_libraries(${DEP_LIBS})

# Select the source files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB DATAACCESS_SRCS ${SRCDIR}/dataaccess/*.cpp)
file(GLOB DATATYPE_SRCS ${SRCDIR}/datatype/*.cpp)
file(GLOB GEOMETRY_SRCS ${SRCDIR}/geometry/*.cpp)
file(GLOB PLATFORM_SRCS ${SRCDIR}/platform/*.cpp)
file(GLOB PLUGIN_SRCS ${SRCDIR}/plugin/*.cpp)
file(GLOB QT_SRCS ${SRCDIR}/qt/*.cpp)
file(GLOB RASTER_SRCS ${SRCDIR}/raster/*.cpp)
file(GLOB SRS_SRCS ${SRCDIR}/srs/*.cpp)

# Select the header files
file(GLOB HDRS ${SRCDIR}/*.h)
file(GLOB DATAACCESS_HDRS ${SRCDIR}/dataaccess/*.h)
file(GLOB DATATYPE_HDRS ${SRCDIR}/datatype/*.h)
file(GLOB GEOMETRY_HDRS ${SRCDIR}/geometry/*.h)
file(GLOB PLATFORM_HDRS ${SRCDIR}/platform/*.h)
file(GLOB PLUGIN_HDRS ${SRCDIR}/plugin/*.h)
file(GLOB QT_HDRS ${SRCDIR}/qt/*.h)
file(GLOB RASTER_HDRS ${SRCDIR}/raster/*.h)
file(GLOB SRS_HDRS ${SRCDIR}/srs/*.h)

# Group the source and header files
source_group("Source Files" FILES ${SRCDIR})
source_group("Source Files\\dataaccess" FILES ${DATAACCESS_SRCS})
source_group("Source Files\\datatype" FILES ${DATATYPE_SRCS})
source_group("Source Files\\geometry" FILES ${GEOMETRY_SRCS})
source_group("Source Files\\platform" FILES ${PLATFORM_SRCS})
source_group("Source Files\\plugin" FILES ${PLUGIN_SRCS})
source_group("Source Files\\qt" FILES ${QT_SRCS})
source_group("Source Files\\raster" FILES ${RASTER_SRCS})
source_group("Source Files\\srs" FILES ${SRS_SRCS})
source_group("Header Files" FILES ${SRCDIR})
source_group("Header Files\\dataaccess" FILES ${DATAACCESS_HDRS})
source_group("Header Files\\datatype" FILES ${DATATYPE_HDRS})
source_group("Header Files\\geometry" FILES ${GEOMETRY_HDRS})
source_group("Header Files\\platform" FILES ${PLATFORM_HDRS})
source_group("Header Files\\plugin" FILES ${PLUGIN_HDRS})
source_group("Header Files\\qt" FILES ${QT_HDRS})
source_group("Header Files\\raster" FILES ${RASTER_HDRS})
source_group("Header Files\\srs" FILES ${SRS_HDRS})

# Select the include directories
include_directories(${SRCDIR}/dataaccess
	                  ${SRCDIR}/datatype
	                  ${SRCDIR}/geometry
	                  ${SRCDIR}/platform
	                  ${SRCDIR}/plugin
					  ${SRCDIR}/qt
					  ${SRCDIR}/raster
					  ${SRCDIR}/srs
	                  ${DEP_INCLUDES})
		
add_library(${LIB_NAME} SHARED
			${SRCS} ${HDRS}
			${DATAACCESS_SRCS} ${DATAACCESS_HDRS}
			${DATATYPE_SRCS} ${DATATYPE_HDRS}
			${GEOMETRY_SRCS} ${GEOMETRY_HDRS}
			${PLATFORM_SRCS} ${PLATFORM_HDRS}
			${PLUGIN_SRCS} ${PLUGIN_HDRS}
			${QT_SRCS} ${QT_HDRS}
			${RASTER_SRCS} ${RASTER_HDRS}
			${SRS_SRCS} ${SRS_HDRS})
      
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

add_dependencies(${PROJ_NAME} terralib_common
	terralib_datatype
	terralib_srs
	terralib_geometry
	terralib_dataaccess
	terralib_plugin
	terralib_color
	terralib_xlink
	terralib_gml
	terralib_filter
	terralib_symbology
	terralib_annotationtext
	terralib_maptools
	terralib_qtwidgets
	terralib_binding_java_common)