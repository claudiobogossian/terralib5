# This script builds the datagrid example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")  # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/examples/dataview)             # directory where the source files of the datagrid example are located
set(INCLDIR ${SRCDIR})                            # directory where the header files of the datagrid example are located

set (ICON_THEME "terralib" CACHE STRING "Defines the default theme to be used.")
set (ICON_THEME_PATH "C:/icons" CACHE STRING "Lists of directories that contains icon themes.")

# Set the project name
set(PROJ_NAME "terralib_example_dataview")
project(${PROJ_NAME})         # the project name is terralib_example_datagrid
SET(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_datagrid


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

add_definitions(-DICON_THEME=\"${ICON_THEME}\" -DICON_THEME_PATH=\"${ICON_THEME_PATH}\")

set(DEP_LIBS 
		terralib_color
		terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_maptools
		terralib_plugin
		terralib_qtwidgets
    ${QT_LIBRARIES}
    )

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the header files for moc'ing
file(
  GLOB HDRS_TO_MOC 
  ${INCLDIR}/TabularViewerEx.h
  ${INCLDIR}/SelectPKey.h
  ${INCLDIR}/HighlightedInfo.h
)
 
# Select the FORMS widgets     
file( GLOB FORMS ${SRCDIR}/ui/*.ui )

if(WIN32)
	add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif()

add_definitions(${QT_DEFINITIONS})

qt4_wrap_cpp(MOC ${HDRS_TO_MOC})

qt4_wrap_ui(UI ${FORMS})

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${MOC} ${UI})

# Select the include directories
include_directories(
    ${TE_INCLUDE_DIRS}
    ${DEP_INCLUDES}
    ${Boost_INCLUDE_DIRS}
    ${INCLDIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)
       
add_executable(${PROJ_NAME} ${SRCS} ${HDRS} ${MOC} ${UI})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${PROJ_NAME} ${DEP_LIBS})

copyLogFiles(${CMAKE_CURRENT_BINARY_DIR})
                  