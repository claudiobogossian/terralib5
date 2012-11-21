# This script builds the colorbar example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(SRCDIR ${ROOT}/shp)                # directory where the source files of the colorbar example are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the colorbar example are located

# Set the project name
set(PROJ_NAME "plugin_shp")
project(${PROJ_NAME})         # the project name is terralib_example_colorbar
set(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_colorbar

find_package(Qt4 ${_Qt4_VERSION} REQUIRED COMPONENTS QtCore QtGui)
if(QT4_FOUND)
  include (${QT_USE_FILE})
  add_definitions (${QT_DEFINITIONS})
  set (DEP_INCLUDES ${QT_INCLUDE_DIR})
  set (DEP_LIBS ${QT_LIBRARIES})
endif()

find_package(Boost ${_Boost_VERSION} REQUIRED COMPONENTS system filesystem)
if(Boost_FOUND)
  list (APPEND DEP_INCLUDES ${Boost_INCLUDE_DIRS})
  list (APPEND DEP_LIBS ${Boost_LIBRARIES})
  add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)
endif()

list (APPEND DEP_INCLUDES ${TE_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
list (APPEND DEP_LIBS ${TE_LIBRARIES})

file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${INCLDIR}/*.h)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

set (AF_PLUGIN_FILE "qt_shp.xml")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/plugin_info.xml.in" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugin_qt_shp_info.xml")

include_directories( ${DEP_INCLUDES} )
add_library (${PROJ_NAME} SHARED ${HDRS} ${SRCS})
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(${PROJ_NAME} ${DEP_LIBS})
                 