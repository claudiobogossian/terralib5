# This script builds the symbology Qt widgets example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")  # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/examples/qt/se)                   # directory where the source files of the symbology Qt widgets example are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the symbology Qt widgets example are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules")

# Set the project name
set(PROJ_NAME "terralib_example_qt_symbology")
project(${PROJ_NAME})         # the project name is terralib_example_qt_symbology
SET(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_qt_symbology

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
endif()

add_definitions(-DICON_THEME=\"${ICON_THEME}\" -DICON_THEME_PATH=\"${ICON_THEME_PATH}\")
	
set (DEP_LIBS 
    terralib_annotationtext
		terralib_color
		terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_filter
		terralib_gdal
		terralib_geometry
		terralib_gml
		terralib_maptools
		terralib_plugin
		terralib_qt_widgets
		terralib_raster
		terralib_symbology
		terralib_serialization
		terralib_srs
		terralib_xlink
		${QT_LIBRARIES}
    )  

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS ${QT_DEFINITIONS})
endif()

# Select the include directories
include_directories(
    ${TE_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${QT_INCLUDE_DIR}
)
		      
add_executable(${PROJ_NAME} ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${PROJ_NAME} ${DEP_LIBS})

copyLogFiles(${CMAKE_CURRENT_BINARY_DIR})
