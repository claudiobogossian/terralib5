
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
include (${QT_USE_FILE})
add_definitions (${QT_DEFINITIONS})

set (DEP_INCLUDES ${QT_INCLUDE_DIR})
set (DEP_LIBS ${QT_LIBRARIES})

find_package(Boost 1.46 COMPONENTS date_time REQUIRED)
list (APPEND DEP_LIBS ${Boost_LIBRARIES})
list (APPEND DEP_INCLUDES ${Boost_INCLUDE_DIRS})

find_package(PostgreSQL)
if(PostgreSQL_FOUND)
	list (APPEND DEP_INCLUDES ${PostgreSQL_INCLUDE_DIR})
endif()

find_package(Qwt REQUIRED )
list (APPEND DEP_LIBS ${QWT_LIBRARY})
list (APPEND DEP_INCLUDES ${QWT_INCLUDE_DIR})

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
		              -DQT_THREAD_SUPPORT
                  )
endif()

list (APPEND DEP_LIBS
    terralib_annotationtext
		terralib_color
		terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_filter
		terralib_geometry
		terralib_maptools
		terralib_raster
		terralib_symbology
		terralib_srs
		terralib_rp
		terralib_xlink
    )
	
# Files to process.
# -------------------------------------------------- 
set (
  _DIRS 
  .
  qwt
  widgets
  widgets/canvas
  widgets/datagrid
  widgets/dataset
  widgets/layer
  widgets/progress
  widgets/property
  widgets/rp
  widgets/se
  widgets/help
  widgets/utils
)

# Files in build tree
appPrefix ("${SRCDIR}" "${_DIRS}" QT_INC_DIRS)

# Files in build tree
appPrefix ("qt" "${_DIRS}" QT_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

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
     ${SRCDIR}/widgets/utils/ColorPickerToolButton.h
     ${SRCDIR}/widgets/rp/SegmenterDialog.h
     ${SRCDIR}/widgets/rp/ContrastDialog.h
	 ${SRCDIR}/widgets/se/BasicFillWidget.h
	 ${SRCDIR}/widgets/se/BasicStrokeWidget.h
	 ${SRCDIR}/widgets/se/ChannelSelectionWidget.h
	 ${SRCDIR}/widgets/se/CharMapWidget.h
	 ${SRCDIR}/widgets/se/ContrastEnhancementWidget.h
	 ${SRCDIR}/widgets/se/GlyphMarkWidget.h
	 ${SRCDIR}/widgets/se/ImageOutlineWidget.h
	 ${SRCDIR}/widgets/se/OverlapBehaviorWidget.h
	 ${SRCDIR}/widgets/se/RasterSymbolizerDialog.h
	 ${SRCDIR}/widgets/se/SelectedChannelWidget.h
	 ${SRCDIR}/widgets/se/ShadedReliefWidget.h
	 ${SRCDIR}/widgets/se/WellKnownMarkWidget.h)

# Select the FORMS widgets     
file(GLOB FORMS
	 ${SRCDIR}/widgets/*.ui
     ${SRCDIR}/widgets/canvas/*.ui
     ${SRCDIR}/widgets/datagrid/*.ui
     ${SRCDIR}/widgets/dataset/*.ui
     ${SRCDIR}/widgets/layer/*.ui
     ${SRCDIR}/widgets/property/*.ui
	 ${SRCDIR}/widgets/utils/ui/*.ui
	 ${SRCDIR}/widgets/rp/ui/*.ui
	 ${SRCDIR}/widgets/se/ui/*.ui)

list(REMOVE_ITEM SRCS ${SRCDIR}/postgis.h ${SRCDIR}/widgets.h)
	
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DQWT_DLL)
endif()
	
qt4_wrap_cpp(MOC ${HDRS_TO_MOC})
qt4_wrap_ui(UI ${FORMS})

install (
  FILES ${UI}
  DESTINATION terralib/qt/ui
  COMPONENT HEADERS
)

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${MOC} ${UI})

# Include directory of the image files
list (APPEND QT_INC_DIRS "${CMAKE_CURRENT_BINARY_DIR}" "${ROOT}/images")
list (APPEND QT_INC_INST_DIRS "qt/ui")
list (APPEND DEP_INCLUDES "${QT_INC_DIRS}")

list (APPEND SRCS "${MOC}" "${UI}")

#exporting module information
exportModuleInformation("qtwidgets" "${QT_INC_DIRS}" "${QT_INC_INST_DIRS}")
  
                