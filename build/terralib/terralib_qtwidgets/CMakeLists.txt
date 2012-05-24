# This script builds the terralib qtwidgets module
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations

set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")     # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terralib/qt)                  # directory where the source files of the terralib qtwidgets module are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the terralib qtwidgets module are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_qtwidgets")
project(${PROJ_NAME})         # the project name is terralib_qtwidgets
set(LIB_NAME ${PROJ_NAME})    # the library name is terralib_qtwidgets

if(WIN32)
	if(MSVC)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG     "${ROOT}/bin32/msvc2010")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE   "${ROOT}/bin32/msvc2010")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG     "${ROOT}/lib32/msvc2010")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE   "${ROOT}/lib32/msvc2010")
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
	
	set(QT_LIBRARY ${QT_QTCORE_LIBRARY})
	set(QT_LIBRARY ${QT_LIBRARY} ${QT_QTMAIN_LIBRARY})
	set(QT_LIBRARY ${QT_LIBRARY} ${QT_QTGUI_LIBRARY})
	set(QT_LIBRARY ${QT_LIBRARY} ${QT_QTSVG_LIBRARY})
	set(DEP_LIBS   ${DEP_LIBS} ${QT_LIBRARY})	
endif(QT4_FOUND)

find_package(Boost 1.44 COMPONENTS date_time REQUIRED)
if(Boost_FOUND)
  if(WIN32)
	  set(BOOST_LIBRARY
		  optimized ${Boost_DATE_TIME_LIBRARY_RELEASE}
		  debug     ${Boost_DATE_TIME_LIBRARY_DEBUG})
    set (DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIR})
    set (DEP_LIBS ${DEP_LIBS} ${BOOST_LIBRARY})
  else(WIN32)
    set (DEP_LIBS ${DEP_LIBS} ${Boost_LIBRARIES})
    set (DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
  endif(WIN32)    
endif(Boost_FOUND)

find_package(Postgresql)
if(POSTGRESQL_FOUND)
	set(DEP_INCLUDES ${DEP_INCLUDES} ${POSTGRESQL_INCLUDE_DIR})
endif(POSTGRESQL_FOUND)

find_package(Qwt)
if(QWT_FOUND)
  if(WIN32)
    set(QWT_LIBRARY
      optimized ${QWT_LIBRARY_RELEASE}
      debug     ${QWT_LIBRARY_DEBUG})
    set(DEP_INCLUDES ${DEP_INCLUDES} ${QWT_INCLUDE_DIR})
    set(DEP_LIBS     ${DEP_LIBS} ${QWT_LIBRARY})
  else(WIN32)
    set(DEP_INCLUDES ${DEP_INCLUDES} ${QWT_INCLUDE_DIR})
    set(DEP_LIBS     ${DEP_LIBS} ${QWT_LIBRARY})
  endif(WIN32)    
endif(QWT_FOUND)

#Definitions for Windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEQTWIDGETSDLL -DBOOST_ALL_NO_LIB
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

#Set the TerraLib modules to be linked
if(WIN32)
  link_directories(${ROOT}/lib32/msvc2010)
else(WIN32)
  link_directories(${ROOT}/lib32)
endif(WIN32)

set(DEP_LIBS  ${DEP_LIBS}
		optimized terralib_annotationtext
		optimized terralib_color
		optimized terralib_common
		optimized terralib_dataaccess
		optimized terralib_datatype
		optimized terralib_filter
		optimized terralib_geometry
		optimized terralib_maptools
		optimized terralib_raster
		optimized terralib_se
		optimized terralib_srs
		optimized terralib_xlink
		debug     terralib_annotationtext_d
		debug     terralib_color_d
		debug     terralib_common_d
		debug     terralib_dataaccess_d
		debug     terralib_datatype_d
		debug     terralib_filter_d
		debug     terralib_geometry_d
		debug     terralib_maptools_d
		debug     terralib_raster_d
		debug     terralib_se_d
		debug     terralib_srs_d
		debug     terralib_xlink_d)
	
link_libraries(${DEP_LIBS})

# Select the source files
file(GLOB QWT_SRCS      ${SRCDIR}/qwt/*.cpp)
file(GLOB WIDGETS_SRCS  ${SRCDIR}/widgets/*.cpp)
file(GLOB CANVAS_SRCS   ${SRCDIR}/widgets/canvas/*.cpp)
file(GLOB DATAGRID_SRCS ${SRCDIR}/widgets/datagrid/*.cpp)
file(GLOB DATASET_SRCS  ${SRCDIR}/widgets/dataset/*.cpp)
file(GLOB LAYER_SRCS    ${SRCDIR}/widgets/layer/*.cpp)
file(GLOB PROGRESS_SRCS ${SRCDIR}/widgets/progress/*.cpp)
file(GLOB PROPERTY_SRCS ${SRCDIR}/widgets/property/*.cpp)
file(GLOB SE_SRCS  		${SRCDIR}/widgets/se/*.cpp)
file(GLOB HELP_SRCS 	${SRCDIR}/widgets/help/*.cpp)
file(GLOB UTILS_SRCS 	${SRCDIR}/widgets/utils/*.cpp)

# Select the header files
file(GLOB QWT_HDRS      ${SRCDIR}/qwt/*.h)
file(GLOB WIDGETS_HDRS  ${SRCDIR}/widgets/*.h)
file(GLOB CANVAS_HDRS   ${SRCDIR}/widgets/canvas/*.h)
file(GLOB DATAGRID_HDRS ${SRCDIR}/widgets/datagrid/*.h)
file(GLOB DATASET_HDRS  ${SRCDIR}/widgets/dataset/*.h)
file(GLOB LAYER_HDRS    ${SRCDIR}/widgets/layer/*.h)
file(GLOB PROGRESS_HDRS ${SRCDIR}/widgets/progress/*.h)
file(GLOB PROPERTY_HDRS ${SRCDIR}/widgets/property/*.h)
file(GLOB SE_HDRS       ${SRCDIR}/widgets/se/*.h)
file(GLOB HELP_HDRS 	${SRCDIR}/widgets/help/*.h)
file(GLOB UTILS_HDRS 	${SRCDIR}/widgets/utils/*.h)

# Group the source and header files
source_group("qwt\\Source Files"               FILES ${QWT_SRCS})
source_group("widgets\\Source Files"           FILES ${WIDGETS_SRCS})
source_group("widgets\\canvas\\Source Files"   FILES ${CANVAS_SRCS})
source_group("widgets\\datagrid\\Source Files" FILES ${DATAGRID_SRCS})
source_group("widgets\\dataset\\Source Files"  FILES ${DATASET_SRCS})
source_group("widgets\\layer\\Source Files"    FILES ${LAYER_SRCS})
source_group("widgets\\progress\\Source Files" FILES ${PROGRESS_SRCS})
source_group("widgets\\property\\Source Files" FILES ${PROPERTY_SRCS})
source_group("widgets\\se\\Source Files"       FILES ${SE_SRCS})
source_group("widgets\\help\\Source Files"	   FILES ${HELP_SRCS})
source_group("widgets\\utils\\Source Files"	   FILES ${UTILS_SRCS})
source_group("qwt\\Header Files"               FILES ${QWT_HDRS})
source_group("widgets\\Header Files"           FILES ${WIDGETS_HDRS})
source_group("widgets\\canvas\\Header Files"   FILES ${CANVAS_HDRS})
source_group("widgets\\datagrid\\Header Files" FILES ${DATAGRID_HDRS})
source_group("widgets\\dataset\\Header Files"  FILES ${DATASET_HDRS})
source_group("widgets\\layer\\Header Files"    FILES ${LAYER_HDRS})
source_group("widgets\\progress\\Header Files" FILES ${PROGRESS_HDRS})
source_group("widgets\\property\\Header Files" FILES ${PROPERTY_HDRS})
source_group("widgets\\se\\Header Files"       FILES ${SE_HDRS})
source_group("widgets\\help\\Header Files" 	   FILES ${HELP_HDRS})
source_group("widgets\\utils\\Header Files"    FILES ${UTILS_HDRS})

# Select the header files for moc'ing
file(GLOB HDRS_TO_MOC
	 ${SRCDIR}/qwt/HistogramDisplay.h
	 ${SRCDIR}/qwt/HistogramSelectionCursor.h
	 ${SRCDIR}/qwt/Legend.h
	 ${SRCDIR}/qwt/Plot.h
	 ${SRCDIR}/qwt/ScatterDisplay.h
	 ${SRCDIR}/qwt/ScatterSelectionCursor.h
	 ${SRCDIR}/qwt/TimeSeriesDisplay.h
	 ${SRCDIR}/widgets/layer/AbstractLayerItem.h
	 ${SRCDIR}/widgets/layer/LayerExplorer.h
	 ${SRCDIR}/widgets/layer/LayerExplorerModel.h
	 ${SRCDIR}/widgets/layer/LayerItem.h
	 ${SRCDIR}/widgets/layer/LegendItem.h
	 ${SRCDIR}/widgets/layer/FolderLayerItem.h	
	 ${SRCDIR}/widgets/dataset/CreateDataSet.h
	 ${SRCDIR}/widgets/dataset/AddIndex.h	
	 ${SRCDIR}/widgets/datagrid/DataGridModel.h
	 ${SRCDIR}/widgets/datagrid/DataGridView.h	
	 ${SRCDIR}/widgets/datagrid/HeaderView.h	
	 ${SRCDIR}/widgets/property/AddProperty.h
	 ${SRCDIR}/widgets/property/RemoveProperty.h
	 ${SRCDIR}/widgets/property/RenameProperty.h
	 ${SRCDIR}/widgets/property/UpdateProperty.h	
	 ${SRCDIR}/widgets/canvas/MapDisplay.h
	 ${SRCDIR}/widgets/canvas/ZoomAndPan.h
	 ${SRCDIR}/widgets/progress/ProgressViewerBar.h
	 ${SRCDIR}/widgets/progress/ProgressViewerDialog.h
	 ${SRCDIR}/widgets/progress/ProgressWidgetItem.h
	 ${SRCDIR}/widgets/progress/ProgressViewerWidget.h
     ${SRCDIR}/widgets/help/AssistantHelpManagerImpl.h
     ${SRCDIR}/widgets/help/HelpPushButton.h
	 ${SRCDIR}/widgets/help/HelpToolButton.h
	 ${SRCDIR}/widgets/utils/DoubleListWidget.h
	 ${SRCDIR}/widgets/utils/ListWidget.h
	 ${SRCDIR}/widgets/utils/ParameterDialog.h
	 ${SRCDIR}/widgets/utils/ParameterTableWidget.h
	 ${SRCDIR}/widgets/utils/ScopedCursor.h
	 ${SRCDIR}/widgets/se/BasicFillWidget.h
	 ${SRCDIR}/widgets/se/BasicStrokeDialog.h
	 ${SRCDIR}/widgets/se/BasicStrokeWidget.h
	 ${SRCDIR}/widgets/se/MarkDialog.h
	 ${SRCDIR}/widgets/se/MarkWidget.h)


# Select the FORMS widgets     
file(GLOB FORMS
	 ${SRCDIR}/widgets/*.ui
     ${SRCDIR}/widgets/canvas/*.ui
     ${SRCDIR}/widgets/datagrid/*.ui
     ${SRCDIR}/widgets/dataset/*.ui
     ${SRCDIR}/widgets/layer/*.ui
     ${SRCDIR}/widgets/property/*.ui
	 ${SRCDIR}/widgets/utils/ui/*.ui
	 ${SRCDIR}/widgets/se/ui/*.ui)

list(REMOVE_ITEM WIDGETS_HDRS ${SRCDIR}/postgis.h ${SRCDIR}/widgets.h)
	
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS ${QT_DEFINITIONS} -DQWT_DLL)
endif(WIN32)
	
qt4_wrap_cpp(MOC ${HDRS_TO_MOC})

qt4_wrap_ui(UI ${FORMS})

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${MOC} ${UI})

# Include directory of the image files
set(DEP_INCLUDES ${DEP_INCLUDES} ${ROOT}/images)

# Select the include directories
include_directories(${ROOT}/src
                    "${CMAKE_CURRENT_BINARY_DIR}"
                    ${DEP_INCLUDES})
		
add_library(${LIB_NAME} SHARED
            ${QWT_SRCS}      ${QWT_HDRS}
            ${WIDGETS_SRCS}  ${WIDGETS_HDRS}
            ${CANVAS_SRCS}   ${CANVAS_HDRS}
            ${DATAGRID_SRCS} ${DATAGRID_HDRS}
            ${DATASET_SRCS}  ${DATASET_HDRS}
            ${LAYER_SRCS}    ${LAYER_HDRS}
			${PROGRESS_SRCS} ${PROGRESS_HDRS}
            ${PROPERTY_SRCS} ${PROPERTY_HDRS}
			${SE_SRCS}  	 ${SE_HDRS}
            ${HELP_SRCS}     ${HELP_HDRS}
			${UTILS_SRCS}    ${UTILS_HDRS}
            ${UI} ${MOC})
            
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
         
add_dependencies(${PROJ_NAME}
 	               terralib_annotationtext
	               terralib_color
				   terralib_common 
	               terralib_dataaccess
	               terralib_datatype
				   terralib_filter
	               terralib_geometry
	               terralib_maptools
	               terralib_raster
				   terralib_se
                   terralib_srs
	               terralib_xlink)
                 
if(WIN32)
  set(CompilerVersion vc100)
  set(terralib-3rdparty-libs $ENV{TERRALIB_DEP_MSVC2010})
  string(REPLACE "\\" "/" terralib-3rdparty-libs ${terralib-3rdparty-libs})
  
  file(COPY
       "${terralib-3rdparty-libs}/qt/bin/QtCore4.dll"
       "${terralib-3rdparty-libs}/qt/bin/QtGui4.dll"
       "${terralib-3rdparty-libs}/qt/bin/QtSvg4.dll"
       "${terralib-3rdparty-libs}/qt/bin/QtCored4.dll"
       "${terralib-3rdparty-libs}/qt/bin/QtGuid4.dll"
       "${terralib-3rdparty-libs}/qt/bin/QtSvgd4.dll"
       "${terralib-3rdparty-libs}/qwt/bin/win32/qwt.dll"
       "${terralib-3rdparty-libs}/qwt/bin/win32/qwtd.dll"
       DESTINATION "${ROOT}/bin32/msvc2010") 

endif(WIN32)

                