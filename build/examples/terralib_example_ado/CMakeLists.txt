# This script builds the ADO example
# The variable ROOT contains the root directory where terralib is installed

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")     # the terralib root directory (change it if it is in a different place)
set(SRCDIR ${ROOT}/examples/ado)                     # directory where the source files of the ADO example are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the ADO example are located

# Set the project name
set(PROJ_NAME "terralib_example_ado")
project(${PROJ_NAME})         # the project name is terralib_example_ado
SET(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_ado

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
		              -DQT_THREAD_SUPPORT
                  )
endif()

set(DEP_LIBS
    terralib_ado  
    terralib_annotationtext
		terralib_color
		terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_filter
		terralib_geometry
		terralib_gml
		terralib_maptools
		terralib_plugin
		terralib_postgis
		terralib_qt_widgets
		terralib_raster
		terralib_srs
		terralib_xlink
    ${QT_LIBRARIES}
    )
 	
# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the header files for moc'ing
#file(GLOB HDRS_TO_MOC 
#     ${SRCDIR}/PropertyManagement.h)
 
# Select the FORMS widgets     
#file(GLOB FORMS
#	   ${SRCDIR}/PropertyManagement.ui)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS ${QT_DEFINITIONS})
endif()
	
#qt4_wrap_cpp(MOC ${HDRS_TO_MOC})

#qt4_wrap_ui(UI ${FORMS})

#source_group("Form Files" FILES ${FORMS})
#source_group("Generated Files" FILES ${MOC} ${UI})

# Select the include directories
include_directories(
    ${TE_INCLUDE_DIRS}
    "${CMAKE_CURRENT_BINARY_DIR}"
    ${DEP_INCLUDES}
    ${Boost_INCLUDE_DIRS}
    ${QT_INCLUDE_DIR}
)

add_executable(${PROJ_NAME} ${SRCS} ${HDRS} ${UI} ${MOC})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${PROJ_NAME} ${DEP_LIBS})

copyLogFiles(${CMAKE_CURRENT_BINARY_DIR})
                  