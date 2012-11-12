
find_package(Qt4 ${_Qt4_VERSION} COMPONENTS QtCore QtGui REQUIRED)
if(QT4_FOUND)
  include (${QT_USE_FILE})
  add_definitions (${QT_DEFINITIONS})

  set (DEP_INCLUDES ${QT_INCLUDE_DIR})
  set (DEP_LIBS ${QT_LIBRARIES})
endif()

find_package(Boost ${_Boost_VERSION} COMPONENTS date_time filesystem system REQUIRED)
if(Boost_FOUND)
  list (APPEND DEP_LIBS ${Boost_LIBRARIES})
  list (APPEND DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

#find_package(PostgreSQL ${_PostgreSQL_VERSION})
#if(PostgreSQL_FOUND)
#	list (APPEND DEP_INCLUDES ${PostgreSQL_INCLUDE_DIR})
#endif()

find_package(Qwt ${_Qwt_VERSION} REQUIRED)
if(Qwt_FOUND)
  list (APPEND DEP_LIBS ${QWT_LIBRARY})
  list (APPEND DEP_INCLUDES ${QWT_INCLUDE_DIR})
endif()

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
#    terralib_gdal
		terralib_geometry
		terralib_maptools
		terralib_raster
		terralib_symbology
		terralib_srs
		terralib_rp
    terralib_plugin
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
  widgets/charts
  widgets/datagrid
  widgets/dataview
  widgets/dataset
#  widgets/datasource/connector/core
#  widgets/datasource/connector/gdal
#  widgets/datasource/connector/geofile
#  widgets/datasource/connector/ogr  
#  widgets/datasource/connector/postgis
#  widgets/datasource/connector/shp
#  widgets/datasource/core
#  widgets/datasource/explorer
#  widgets/datasource/selector
  widgets/layer
  widgets/progress
  widgets/property
  widgets/rp
  widgets/se
  widgets/srs
  widgets/tools
  widgets/help
  widgets/plugin/manager
  widgets/utils
)

# Files in build tree
appPrefix ("${SRCDIR}" "${_DIRS}" QT_INC_DIRS)

# Files in build tree
appPrefix ("qt" "${_DIRS}" QT_INC_INST_DIRS)

# Get files by structured by folders.
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

set (MOC "")

# Select the header files for moc'ing
set (
  HDRS_TO_MOC
  ${SRCDIR}/qwt/HistogramDisplay.h
  ${SRCDIR}/qwt/HistogramSelectionCursor.h
  ${SRCDIR}/qwt/Legend.h
  ${SRCDIR}/qwt/Plot.h
  ${SRCDIR}/qwt/ScatterDisplay.h
  ${SRCDIR}/qwt/ScatterSelectionCursor.h
  ${SRCDIR}/qwt/TimeSeriesDisplay.h
  ${SRCDIR}/qwt/ColorBar.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/qwt" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/canvas/DrawLayerThread.h
  ${SRCDIR}/widgets/canvas/MapDisplay.h
  ${SRCDIR}/widgets/canvas/MultiThreadMapDisplay.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/canvas" MOC)

#set (
#  HDRS_TO_MOC
#  ${SRCDIR}/widgets/datasource/connector/gdal/GDALConnectorDialog.h  
#)
#te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/datasource/connector/gdal" MOC)

#set (
#  HDRS_TO_MOC
#  ${SRCDIR}/widgets/datasource/connector/geofile/GeoFileConnectorDialog.h  
#)
#te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/datasource/connector/geofile" MOC)

#set (
#  HDRS_TO_MOC
#  ${SRCDIR}/widgets/datasource/connector/ogr/OGRConnectorDialog.h  
#)
#te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/datasource/connector/ogr" MOC)

#set (
#  HDRS_TO_MOC
#  ${SRCDIR}/widgets/datasource/connector/postgis/PostGISConnectorDialog.h  
#)
#te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/datasource/connector/postgis" MOC)

#set (
#  HDRS_TO_MOC
#  ${SRCDIR}/widgets/datasource/explorer/AbstractDataSourceTreeItem.h
#  ${SRCDIR}/widgets/datasource/explorer/DataSourceTreeView.h
#  ${SRCDIR}/widgets/datasource/explorer/CheckConstraintGroupItem.h
#  ${SRCDIR}/widgets/datasource/explorer/ForeignKeyGroupItem.h
#  ${SRCDIR}/widgets/datasource/explorer/CheckConstraintItem.h
#  ${SRCDIR}/widgets/datasource/explorer/ForeignKeyItem.h
#  ${SRCDIR}/widgets/datasource/explorer/ConstraintsItem.h
#  ${SRCDIR}/widgets/datasource/explorer/IndexGroupItem.h
#  ${SRCDIR}/widgets/datasource/explorer/DataSetGroupItem.h
#  ${SRCDIR}/widgets/datasource/explorer/IndexItem.h
#  ${SRCDIR}/widgets/datasource/explorer/DataSetItem.h
#  ${SRCDIR}/widgets/datasource/explorer/PrimaryKeyItem.h
#  ${SRCDIR}/widgets/datasource/explorer/DataSourceExplorer.h
#  ${SRCDIR}/widgets/datasource/explorer/PropertyGroupItem.h
#  ${SRCDIR}/widgets/datasource/explorer/DataSourceItem.h
#  ${SRCDIR}/widgets/datasource/explorer/PropertyItem.h
#  ${SRCDIR}/widgets/datasource/explorer/DataSourceToolBox.h
#  ${SRCDIR}/widgets/datasource/explorer/UniqueKeyGroupItem.h
#  ${SRCDIR}/widgets/datasource/explorer/DataSourceTreeModel.h
#  ${SRCDIR}/widgets/datasource/explorer/UniqueKeyItem.h
#)
#te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/datasource/explorer" MOC)

#set (
#  HDRS_TO_MOC
#  ${SRCDIR}/widgets/datasource/selector/DataSourceSelectorDialog.h
#  ${SRCDIR}/widgets/datasource/selector/DataSourceSelectorWidget.h
#  ${SRCDIR}/widgets/datasource/selector/DataSourceSelectorWizardPage.h 
#)  
#te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/datasource/selector" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/layer/AbstractTreeItem.h
  ${SRCDIR}/widgets/layer/LayerExplorer.h
  ${SRCDIR}/widgets/layer/LayerExplorerModel.h
  ${SRCDIR}/widgets/layer/LayerItem.h
  ${SRCDIR}/widgets/layer/LegendItem.h
  ${SRCDIR}/widgets/layer/Legend.h
#  ${SRCDIR}/widgets/layer/FolderLayerItem.h
)  
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/layer" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/dataset/CreateDataSet.h
  ${SRCDIR}/widgets/dataset/AddIndex.h
)  
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/dataset" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/datagrid/DataGridModel.h
  ${SRCDIR}/widgets/datagrid/DataGridView.h
  ${SRCDIR}/widgets/datagrid/HeaderView.h
)  
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/datagrid" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/property/AddProperty.h
  ${SRCDIR}/widgets/property/RemoveProperty.h
  ${SRCDIR}/widgets/property/RenameProperty.h
  ${SRCDIR}/widgets/property/UpdateProperty.h
)  
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/property" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/progress/ProgressViewerBar.h
  ${SRCDIR}/widgets/progress/ProgressViewerDialog.h
  ${SRCDIR}/widgets/progress/ProgressWidgetItem.h
  ${SRCDIR}/widgets/progress/ProgressViewerWidget.h
)  
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/progress" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/help/AssistantHelpManagerImpl.h
  ${SRCDIR}/widgets/help/HelpPushButton.h
  ${SRCDIR}/widgets/help/HelpToolButton.h
)  
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/help" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/utils/DoubleListWidget.h
  ${SRCDIR}/widgets/utils/HorizontalSliderWidget.h
  ${SRCDIR}/widgets/utils/ListWidget.h
  ${SRCDIR}/widgets/utils/ParameterDialog.h
  ${SRCDIR}/widgets/utils/ParameterTableWidget.h
  ${SRCDIR}/widgets/utils/ColorPickerToolButton.h
  ${SRCDIR}/widgets/utils/FileChooser.h
)  
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/utils" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/rp/SegmenterDialog.h
  ${SRCDIR}/widgets/rp/ContrastDialog.h
  ${SRCDIR}/widgets/rp/ClassifierDialog.h
)  
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/rp" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/se/AbstractGraphicWidget.h
  ${SRCDIR}/widgets/se/BasicFillWidget.h
  ${SRCDIR}/widgets/se/BasicStrokeWidget.h
  ${SRCDIR}/widgets/se/ChannelSelectionWidget.h
  ${SRCDIR}/widgets/se/CharMapWidget.h
  ${SRCDIR}/widgets/se/ColorMapWidget.h
  ${SRCDIR}/widgets/se/ContrastEnhancementWidget.h
  ${SRCDIR}/widgets/se/GlyphGraphicWidget.h
  ${SRCDIR}/widgets/se/GlyphMarkWidget.h
  ${SRCDIR}/widgets/se/GraphicFillWidget.h
  ${SRCDIR}/widgets/se/GraphicSelectorWidget.h
  ${SRCDIR}/widgets/se/ImageOutlineWidget.h
  ${SRCDIR}/widgets/se/LineSymbolizerWidget.h
  ${SRCDIR}/widgets/se/LocalGraphicWidget.h
  ${SRCDIR}/widgets/se/LocalImageWidget.h
  ${SRCDIR}/widgets/se/OverlapBehaviorWidget.h
  ${SRCDIR}/widgets/se/PointSymbolizerWidget.h
  ${SRCDIR}/widgets/se/PolygonSymbolizerWidget.h
  ${SRCDIR}/widgets/se/RasterSymbolizerDialog.h
  ${SRCDIR}/widgets/se/RasterVisualWidget.h
  ${SRCDIR}/widgets/se/SelectedChannelWidget.h
  ${SRCDIR}/widgets/se/ShadedReliefWidget.h
  ${SRCDIR}/widgets/se/SymbolEditorWidget.h
  ${SRCDIR}/widgets/se/SymbolizerTableWidget.h
  ${SRCDIR}/widgets/se/WellKnownGraphicWidget.h
  ${SRCDIR}/widgets/se/WellKnownMarkWidget.h
)  
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/se" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/srs/SRSDialog.h
  ${SRCDIR}/widgets/srs/SRSManagerDialog.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/srs" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/dataview/TabularViewer.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/dataview" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/tools/CoordTracking.h
  ${SRCDIR}/widgets/tools/Measure.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/tools" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/plugin/manager/PluginManagerDialog.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/plugin/manager" MOC)

# Select the FORMS widgets
file(
  GLOB FORMS
  ${SRCDIR}/widgets/*.ui
  ${SRCDIR}/widgets/canvas/*.ui
  ${SRCDIR}/widgets/charts/*.ui
  ${SRCDIR}/widgets/datagrid/*.ui
  ${SRCDIR}/widgets/dataset/*.ui
#  ${SRCDIR}/widgets/datasource/connector/gdal/ui/*.ui
#  ${SRCDIR}/widgets/datasource/connector/geofile/ui/*.ui
#  ${SRCDIR}/widgets/datasource/connector/ogr/ui/*.ui
#  ${SRCDIR}/widgets/datasource/connector/postgis/ui/*.ui
#  ${SRCDIR}/widgets/datasource/selector/ui/*.ui  
  ${SRCDIR}/widgets/layer/ui/*.ui
  ${SRCDIR}/widgets/property/*.ui
  ${SRCDIR}/widgets/utils/ui/*.ui
  ${SRCDIR}/widgets/rp/ui/*.ui
  ${SRCDIR}/widgets/se/ui/*.ui
  ${SRCDIR}/widgets/srs/ui/*.ui
  ${SRCDIR}/widgets/plugin/manager/ui/*.ui
)

list(REMOVE_ITEM SRCS ${SRCDIR}/postgis.h ${SRCDIR}/widgets.h)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DQWT_DLL)
endif()

set (MOC2 ${CMAKE_CURRENT_BINARY_DIR}/moc_DataViewPopupFilter.cpp)
QT4_CREATE_MOC_COMMAND("${SRCDIR}/widgets/dataview/TabularViewer.cpp" "${MOC2}" "" "-fterralib/qt/widgets/dataview/TabularViewer.cpp" )

qt4_wrap_ui(UI ${FORMS})

install (
  FILES ${UI}
  DESTINATION terralib/qt/ui
  COMPONENT HEADERS
)

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${MOC} ${MOC2} ${UI})

# Include directory of the image files
list (APPEND QT_INC_DIRS "${CMAKE_CURRENT_BINARY_DIR}")
list (APPEND QT_INC_INST_DIRS "qt/ui")
list (APPEND DEP_INCLUDES "${QT_INC_DIRS}")

list (APPEND SRCS "${MOC}" "${MOC2}" "${UI}")

list (REMOVE_ITEM SRCS ${SRCDIR}/widgets/dataview/TabularViewer.cpp)

#exporting module information
exportModuleInformation("qtwidgets" "${QT_INC_DIRS}" "${QT_INC_INST_DIRS}")

