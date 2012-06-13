# This script builds the datagrid example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")  # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/examples/help)             # directory where the source files of the datagrid example are located
set(INCLDIR ${SRCDIR})                            # directory where the header files of the datagrid example are located

# Set the project name
set(PROJ_NAME "terralib_example_help")
project(${PROJ_NAME})         # the project name is terralib_example_datagrid
SET(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_datagrid

#Definitions for Windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS
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

set ( DEP_LIBS 
    terralib_common 
    terralib_qtwidgets
    ${QT_LIBRARIES}
)

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

file(GLOB FORMS ${SRCDIR}/*.ui)

# Select the header files for moc'ing
file(GLOB HDRS_TO_MOC 
     ${SRCDIR}/HelpTest.h
     ${SRCDIR}/HelpTest2.h
     ${SRCDIR}/MainWindow.h
     ${SRCDIR}/Plugin.h
     )
 
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS ${QT_DEFINITIONS})
endif()
	
qt4_wrap_cpp(MOC ${HDRS_TO_MOC})
qt4_wrap_ui(UI ${FORMS})

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${MOC} ${UI})

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
