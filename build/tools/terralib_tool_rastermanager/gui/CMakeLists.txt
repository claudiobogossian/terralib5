# This script builds the terralib library
# (c) Felipe V. Siqueira, september 2011
#
# To use it, you should adjust ROOT 
# change ROOT to the directory where terralib source is located

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../..") 				# change this if terralib is somewhere else
set(SRCDIR ${ROOT}/src/terralib/tools/rastermanager/gui) 						# this is the directory where terralib sources will be located
set(INCLDIR ${SRCDIR}) 												# this is the directory where terralib header files will be located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules")

# Set the project name
set(PROJ_NAME "terralib_tool_rastermanager_gui")
project(${PROJ_NAME}) # the project name is terralib_tool_dsinfo

if(WIN32)
	if(MSVC)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ROOT}/bin32/msvc2010")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ROOT}/bin32/msvc2010")
	endif(MSVC)
endif(WIN32)

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

find_package(Boost 1.44.0 COMPONENTS system filesystem REQUIRED)
if(Boost_FOUND)
  if(WIN32)
    set(BOOST_LIBRARY
      optimized ${Boost_SYSTEM_LIBRARY_RELEASE}
	  optimized ${Boost_FILESYSTEM_LIBRARY_RELEASE}
	  debug ${Boost_SYSTEM_LIBRARY_DEBUG}
	  debug ${Boost_FILESYSTEM_LIBRARY_DEBUG})
    set (DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIR})
    set (DEP_LIBS ${DEP_LIBS} ${BOOST_LIBRARY})
  else(WIN32)
    set (DEP_LIBS ${DEP_LIBS} ${Boost_LIBRARIES})
    set (DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
  endif(WIN32)
endif(Boost_FOUND)

find_package(GDAL REQUIRED)
if(GDAL_FOUND)
  set (DEP_INCLUDES ${DEP_INCLUDES} ${GDAL_INCLUDE_DIR})
  set (DEP_LIBS ${DEP_LIBS} ${GDAL_LIBRARY})
endif(GDAL_FOUND)

#Add Ws2_32.lib for Windows
if(WIN32)
  set (DEP_LIBS ${DEP_LIBS} Ws2_32.lib)
endif(WIN32)

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB
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

if(WIN32)
	set(DEP_LIBS ${DEP_LIBS}
		optimized	"${ROOT}/lib32/msvc2010/terralib_common.lib"
		optimized	"${ROOT}/lib32/msvc2010/terralib_plugin.lib"
		optimized	"${ROOT}/lib32/msvc2010/terralib_gdal.lib"
		optimized	"${ROOT}/lib32/msvc2010/terralib_raster.lib"
		optimized	"${ROOT}/lib32/msvc2010/terralib_geometry.lib"	
		debug		"${ROOT}/lib32/msvc2010/terralib_common_d.lib"	
		debug 		"${ROOT}/lib32/msvc2010/terralib_plugin_d.lib"
		debug 		"${ROOT}/lib32/msvc2010/terralib_gdal_d.lib"
		debug	"${ROOT}/lib32/msvc2010/terralib_raster_d.lib"
		debug	"${ROOT}/lib32/msvc2010/terralib_geometry_d.lib")
endif(WIN32)  

link_libraries(${DEP_LIBS})

# Select the header files
file(GLOB HDRS
	${SRCDIR}/../core/*.h
	${SRCDIR}/../qt/*.h
	)
	
# Select the source files
file(GLOB SRCS 
	${SRCDIR}/../core/*.cpp
	${SRCDIR}/../qt/*.cpp
	)

# Select the header files for moc'ing
file(GLOB HDRS_TO_MOC
	 ${SRCDIR}/../qt/*.h)
	
# Select the FORMS widgets     
file(GLOB FORMS
	   ${SRCDIR}/../qt/*.ui
	   )

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS ${QT_DEFINITIONS} -DQWT_DLL)
endif(WIN32)

qt4_wrap_cpp(MOC ${HDRS_TO_MOC})

qt4_wrap_ui(UI ${FORMS})

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${MOC} ${UI})

# Select the include files
include_directories(
	${ROOT}/src
	${SRCDIR}/gui
	${SRCDIR}/../qt
	${SRCDIR}/../core
	"${CMAKE_CURRENT_BINARY_DIR}"
	${DEP_INCLUDES}
	)

add_executable(${PROJ_NAME} ${SRCS} ${HDRS} ${UI} ${MOC})

set_target_properties(${PROJ_NAME} PROPERTIES OUTPUT_NAME "te_rastermanager_gui" DEBUG_POSTFIX _d)