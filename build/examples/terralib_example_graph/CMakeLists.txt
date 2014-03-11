# This script builds the graph example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")  # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/examples/graph)           	  # directory where the source files of the graph example are located
set(INCLDIR ${SRCDIR})                            # directory where the header files of the graph example are located

# Set the project name
set(PROJ_NAME "terralib_example_graph")
project(${PROJ_NAME})         # the project name is terralib_example_graph
set(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_graph

find_package(Boost ${_Boost_VERSION} COMPONENTS system REQUIRED)

#Definitions for Windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB)
endif(WIN32)

set(DEP_LIBS 
    terralib_common
	terralib_dataaccess
	terralib_datatype
	terralib_gdal
	terralib_geometry
	terralib_graph
	terralib_ogr
	terralib_raster
	terralib_postgis
	terralib_srs
    terralib_maptools
	terralib_plugin
	terralib_qt_widgets
	terralib_symbology
	${QT_LIBRARIES}
	${GDAL_LIBRARY}
    ${Boost_LIBRARIES}
    )
		
# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the header files for moc'ing
file(
  GLOB HDRS_TO_MOC 
  ${INCLDIR}/DisplayWindow.h
)

qt4_wrap_cpp(MOC ${HDRS_TO_MOC})

source_group("Generated Files" FILES ${MOC})

# Select the include directories
include_directories(
    ${TE_INCLUDE_DIRS}
    ${DEP_INCLUDES}
    ${Boost_INCLUDE_DIRS}
	${GDAL_INCLUDE_DIR}
)
                          
add_executable(${PROJ_NAME} ${SRCS} ${HDRS} ${MOC})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${PROJ_NAME} ${DEP_LIBS})

copyLogFiles(${CMAKE_CURRENT_BINARY_DIR})
