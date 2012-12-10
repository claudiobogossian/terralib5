# This script builds the legend example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")     # the terralib root directory (change it if it is in a different place)
set(SRCDIR ${ROOT}/examples/legend)                  # directory where the source files of the legend example are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the legend example are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../modules")

# Set the project name
set(PROJ_NAME "terralib_example_legend")
project(${PROJ_NAME})         # the project name is terralib_example_legend
SET(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_legend


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



set(DEP_LIBS
	 terralib_color
         terralib_common
	terralib_dataaccess
	terralib_datatype
	terralib_geometry
	terralib_maptools
        terralib_plugin
	terralib_postgis
	terralib_qtwidgets
	terralib_xlink
	${QT_LIBRARIES}
	${POSTGRESQL_LIBRARIES})
 	
# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the header files for moc'ing
file(GLOB HDRS_TO_MOC 
     ${SRCDIR}/Legend.h)
 
# Select the FORMS widgets     
#file(GLOB FORMS
#	   ${SRCDIR}/LegendEdition.ui)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS ${QT_DEFINITIONS})
endif(WIN32)
	
qt4_wrap_cpp(MOC ${HDRS_TO_MOC})

#qt4_wrap_ui(UI ${FORMS})

#source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${MOC})
	 
# Select the include directories
include_directories(
    ${TE_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${QT_INCLUDE_DIR}
    ${QWT_INCLUDE_DIR}
	  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(${PROJ_NAME} ${SRCS} ${HDRS} ${MOC})
	 
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${PROJ_NAME} ${DEP_LIBS})
                  