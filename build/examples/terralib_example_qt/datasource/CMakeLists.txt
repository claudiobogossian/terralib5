# This script builds the DataSource Qt component example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")  # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/examples/qt/datasource)                # directory where the source files of the chart Qt widgets example are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the chart Qt widgets example are located

# Set the project name
set(PROJ_NAME "terralib_example_qt_datasource")
project(${PROJ_NAME})         # the project name is terralib_example_qt_datasource
SET(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_qt_datasource

set(DEP_LIBS 
    terralib_common
    terralib_plugin
    terralib_qt_widgets
    terralib_dataaccess
    ${QWT_LIBRARY}
    ${QT_LIBRARIES}
    )  

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the header files for moc'ing
file(
  GLOB HDRS_TO_MOC 
  ${INCLDIR}/MainWindow.h
)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS ${QT_DEFINITIONS} -DBOOST_ALL_NO_LIB)
endif()

qt4_wrap_cpp(MOC ${HDRS_TO_MOC})

source_group("Generated Files" FILES ${MOC})

# Select the include directories
include_directories(
    ${TE_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${QT_INCLUDE_DIR}
    ${QWT_INCLUDE_DIR}
)
		      
add_executable(${PROJ_NAME} ${SRCS} ${HDRS} ${MOC})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${PROJ_NAME} ${DEP_LIBS})

copyLogFiles(${CMAKE_CURRENT_BINARY_DIR})
