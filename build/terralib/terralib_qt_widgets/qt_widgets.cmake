
find_package(Qt4 ${_Qt4_VERSION} COMPONENTS QtCore QtGui REQUIRED)
if(QT4_FOUND)
  include (${QT_USE_FILE})
  add_definitions (${QT_DEFINITIONS})

  set (TE_DEP_INCLUDES ${QT_INCLUDE_DIR})
  set (TE_DEP_LIBS ${QT_LIBRARIES})
endif()

find_package(Boost ${_Boost_VERSION} COMPONENTS date_time filesystem system REQUIRED)
if(Boost_FOUND)
  list (APPEND TE_DEP_LIBS ${Boost_LIBRARIES})
  list (APPEND TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(Qwt ${_Qwt_VERSION} REQUIRED)
if(Qwt_FOUND)
  list (APPEND TE_DEP_LIBS ${QWT_LIBRARY})
  list (APPEND TE_DEP_INCLUDES ${QWT_INCLUDE_DIR})
endif()

#Definitions for Windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS
	                -D_SCL_SECURE_NO_WARNINGS
                	-DTEQTWIDGETSDLL
					-DBOOST_ALL_NO_LIB
		            -DUNICODE
#		            -DQT_LARGEFILE_SUPPORT
#		            -DQT_DLL
#		            -DQT_GUI_LIB
#		            -DQT_CORE_LIB
#		            -DQT_HAVE_MMX
#		            -DQT_HAVE_3DNOW
#		            -DQT_HAVE_SSE
#		            -DQT_HAVE_MMXEXT
#		            -DQT_HAVE_SSE2
#		            -DQT_THREAD_SUPPORT
                    )
endif()

list (APPEND TE_DEP_LIBS
    terralib_annotationtext
		terralib_color
		terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_filter
#    terralib_gdal
		terralib_geometry
		terralib_maptools
		terralib_memory
		terralib_raster
		terralib_statistics
		terralib_symbology
		terralib_srs
		terralib_rp
    terralib_plugin
		terralib_xlink
		qtpropertybrowser
    )

# Files to process.
# --------------------------------------------------
set (
  _DIRS
  .
  widgets
  widgets/canvas
  widgets/charts
  widgets/colorbar
 # widgets/datagrid
 # widgets/dataview
  widgets/dataset
  widgets/dataset/explorer
  widgets/dataset/selector
  widgets/datasource/core
  widgets/datasource/explorer
  widgets/datasource/selector
  widgets/datasource/connector
  widgets/exchanger
  widgets/externalTable
  widgets/layer/explorer
  widgets/layer/search
  widgets/layer/selector
  widgets/layer/utils
  widgets/layer/info
  widgets/layout
  widgets/mapdisplay
  widgets/progress
  widgets/property
  widgets/propertybrowser
  widgets/propertybrowser/qt
  widgets/rp
  widgets/se
  widgets/slider
  widgets/srs
  widgets/table
  widgets/temporal
  widgets/tools
  widgets/help
  widgets/plugin/builder
  widgets/plugin/manager
  widgets/query
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
  ${SRCDIR}/widgets/canvas/DrawLayerThread.h
  ${SRCDIR}/widgets/canvas/EyeBirdMapDisplayWidget.h
  ${SRCDIR}/widgets/canvas/MapDisplay.h
  ${SRCDIR}/widgets/canvas/MultiThreadMapDisplay.h
  ${SRCDIR}/widgets/canvas/OverlayWidget.h
  ${SRCDIR}/widgets/canvas/ZoomInMapDisplayWidget.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/canvas" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/colorbar/ColorBar.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/colorbar" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/charts/ChartDisplay.h
  ${SRCDIR}/widgets/charts/ChartDisplayWidget.h
  ${SRCDIR}/widgets/charts/ChartLayerDialog.h
  ${SRCDIR}/widgets/charts/ChartLayerWidget.h
  ${SRCDIR}/widgets/charts/ChartStyleFrame.h
  ${SRCDIR}/widgets/charts/ChartStyleWidget.h
  ${SRCDIR}/widgets/charts/HistogramDataWidget.h
  ${SRCDIR}/widgets/charts/HistogramStyleWidget.h
  ${SRCDIR}/widgets/charts/HistogramDialog.h
  ${SRCDIR}/widgets/charts/ChartProperties.h
  ${SRCDIR}/widgets/charts/HistogramFrame.h
  ${SRCDIR}/widgets/charts/ScatterFrame.h
  ${SRCDIR}/widgets/charts/ScatterDataWidget.h
  ${SRCDIR}/widgets/charts/ScatterStyleWidget.h
  ${SRCDIR}/widgets/charts/ScatterDialog.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/charts" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/datasource/explorer/AbstractDataSourceTreeItem.h
  ${SRCDIR}/widgets/datasource/explorer/DataSourceTreeView.h
  ${SRCDIR}/widgets/datasource/explorer/CheckConstraintItem.h
  ${SRCDIR}/widgets/datasource/explorer/ForeignKeyItem.h
  ${SRCDIR}/widgets/datasource/explorer/ConstraintsItem.h
  ${SRCDIR}/widgets/datasource/explorer/IndexGroupItem.h
  ${SRCDIR}/widgets/datasource/explorer/DataSetCategoryGroupItem.h
  ${SRCDIR}/widgets/datasource/explorer/DataSetCategoryItem.h
  ${SRCDIR}/widgets/datasource/explorer/DataSetGroupItem.h
  ${SRCDIR}/widgets/datasource/explorer/IndexItem.h
  ${SRCDIR}/widgets/datasource/explorer/DataSetItem.h
  ${SRCDIR}/widgets/datasource/explorer/PrimaryKeyItem.h
  ${SRCDIR}/widgets/datasource/explorer/DataSourceExplorer.h
  ${SRCDIR}/widgets/datasource/explorer/PropertyGroupItem.h
  ${SRCDIR}/widgets/datasource/explorer/DataSourceItem.h
  ${SRCDIR}/widgets/datasource/explorer/PropertyItem.h
  ${SRCDIR}/widgets/datasource/explorer/DataSourceToolBox.h
  ${SRCDIR}/widgets/datasource/explorer/DataSourceTreeModel.h
  ${SRCDIR}/widgets/datasource/explorer/UniqueKeyItem.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/datasource/explorer" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/datasource/selector/DataSourceExplorerDialog.h
  ${SRCDIR}/widgets/datasource/selector/DataSourceSelectorDialog.h
  ${SRCDIR}/widgets/datasource/selector/DataSourceSelectorWidget.h
  ${SRCDIR}/widgets/datasource/selector/DataSourceSelectorWizardPage.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/datasource/selector" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/exchanger/DataExchangerWizard.h
  ${SRCDIR}/widgets/exchanger/DataExchangeSummaryWizardPage.h
  ${SRCDIR}/widgets/exchanger/DataSetOptionsWizardPage.h
  ${SRCDIR}/widgets/exchanger/DirectExchangerDialog.h
  ${SRCDIR}/widgets/exchanger/SHP2ADODialog.h
  ${SRCDIR}/widgets/exchanger/SHP2PostGISDialog.h
  ${SRCDIR}/widgets/exchanger/PostGIS2SHPDialog.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/exchanger" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/externalTable/DataPropertiesWidget.h
  ${SRCDIR}/widgets/externalTable/DataPropertiesDialog.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/externalTable" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/layer/explorer/AbstractTreeItem.h
  ${SRCDIR}/widgets/layer/explorer/ChartItem.h
  ${SRCDIR}/widgets/layer/explorer/ChartSliceItem.h
  ${SRCDIR}/widgets/layer/explorer/ColorMapItem.h
  ${SRCDIR}/widgets/layer/explorer/ColorMapSliceItem.h
  ${SRCDIR}/widgets/layer/explorer/DataSetLayerItem.h
  ${SRCDIR}/widgets/layer/explorer/DataSetAdapterLayerItem.h
  ${SRCDIR}/widgets/layer/explorer/FolderLayerItem.h
  ${SRCDIR}/widgets/layer/explorer/GroupingItem.h
  ${SRCDIR}/widgets/layer/explorer/GroupingSliceItem.h
  ${SRCDIR}/widgets/layer/explorer/LayerExplorer.h
  ${SRCDIR}/widgets/layer/explorer/LayerTreeModel.h
  ${SRCDIR}/widgets/layer/explorer/LayerTreeView.h
  ${SRCDIR}/widgets/layer/explorer/LegendItem.h
  ${SRCDIR}/widgets/layer/explorer/QueryLayerItem.h
  ${SRCDIR}/widgets/layer/explorer/RasterLayerItem.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/layer/explorer" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/layer/info/LayerPropertiesInfo.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/layer/info" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/layer/search/LayerSearchWidget.h
  ${SRCDIR}/widgets/layer/search/LayerSearchWizardPage.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/layer/search" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/layer/selector/AbstractLayerSelector.h
  ${SRCDIR}/widgets/layer/selector/DataSetLayerSelector.h
  ${SRCDIR}/widgets/layer/selector/HiddenDataSetLayerSelector.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/layer/selector" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/layout/DataFrame.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/layout" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/mapdisplay/DataSetDisplay.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/mapdisplay" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/dataset/CreateDataSet.h
  ${SRCDIR}/widgets/dataset/AddIndex.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/dataset" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/dataset/selector/DataSetSelectorDialog.h
  ${SRCDIR}/widgets/dataset/selector/DataSetSelectorWidget.h
  ${SRCDIR}/widgets/dataset/selector/DataSetSelectorWizardPage.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/dataset/selector" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/dataset/explorer/DataSetExplorer.h
  ${SRCDIR}/widgets/dataset/explorer/DataSetCategoryModel.h
  ${SRCDIR}/widgets/dataset/explorer/DataSetTreeModel.h
  ${SRCDIR}/widgets/dataset/explorer/DataSetTreeView.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/dataset/explorer" MOC)

#set (
#  HDRS_TO_MOC
#  ${SRCDIR}/widgets/datagrid/DataGridModel.h
#  ${SRCDIR}/widgets/datagrid/DataGridView.h
#  ${SRCDIR}/widgets/datagrid/HeaderView.h
#)
#te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/datagrid" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/property/AddProperty.h
  ${SRCDIR}/widgets/property/ConstraintsIndexesListWidget.h
  ${SRCDIR}/widgets/property/ConstraintsIndexesPropertyDialog.h
  ${SRCDIR}/widgets/property/ConstraintWidget.h
  ${SRCDIR}/widgets/property/DataSetAdapterWidget.h
  ${SRCDIR}/widgets/property/DateTimePropertyWidget.h
  ${SRCDIR}/widgets/property/GeometryPropertyWidget.h
  ${SRCDIR}/widgets/property/IndexWidget.h
  ${SRCDIR}/widgets/property/NewPropertyDialog.h
  ${SRCDIR}/widgets/property/NewPropertyWidget.h
  ${SRCDIR}/widgets/property/NumericPropertyWidget.h
  ${SRCDIR}/widgets/property/PrimaryKeyConstraintWidget.h
  ${SRCDIR}/widgets/property/PropertyConverterDialog.h
  ${SRCDIR}/widgets/property/RemoveProperty.h
  ${SRCDIR}/widgets/property/RenameProperty.h
  ${SRCDIR}/widgets/property/SimplePropertyWidget.h
  ${SRCDIR}/widgets/property/StringPropertyWidget.h
  ${SRCDIR}/widgets/property/UniquekeyConstraintWidget.h
  ${SRCDIR}/widgets/property/UpdateProperty.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/property" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/propertybrowser/AbstractPropertyItem.h
  ${SRCDIR}/widgets/propertybrowser/AbstractPropertyManager.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/propertybrowser" MOC)

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
  ${SRCDIR}/widgets/utils/ResourceChooser.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/utils" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/rp/ClassifierDialog.h
  ${SRCDIR}/widgets/rp/ClassifierWizardPage.h
  ${SRCDIR}/widgets/rp/ContrastDialog.h
  ${SRCDIR}/widgets/rp/ContrastWizardPage.h
  ${SRCDIR}/widgets/rp/MixtureModelDialog.h
  ${SRCDIR}/widgets/rp/MixtureModelWizardPage.h
  ${SRCDIR}/widgets/rp/RasterHistogramDialog.h
  ${SRCDIR}/widgets/rp/RasterHistogramWidget.h
  ${SRCDIR}/widgets/rp/RasterInfoWidget.h
  ${SRCDIR}/widgets/rp/RasterInfoWizardPage.h
  ${SRCDIR}/widgets/rp/RasterNavigatorDialog.h
  ${SRCDIR}/widgets/rp/RasterNavigatorWidget.h
  ${SRCDIR}/widgets/rp/RasterNavigatorWizardPage.h
  ${SRCDIR}/widgets/rp/SegmenterDialog.h
  ${SRCDIR}/widgets/rp/SegmenterWizardPage.h
  ${SRCDIR}/widgets/rp/TiePointsLocatorAdvancedDialog.h
  ${SRCDIR}/widgets/rp/TiePointsLocatorDialog.h
  ${SRCDIR}/widgets/rp/TiePointLocatorDialog.h
  ${SRCDIR}/widgets/rp/TiePointLocatorWidget.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/rp" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/se/AbstractGraphicWidget.h
  ${SRCDIR}/widgets/se/BasicFillPropertyItem.h
  ${SRCDIR}/widgets/se/BasicFillWidget.h
  ${SRCDIR}/widgets/se/BasicStrokePropertyItem.h
  ${SRCDIR}/widgets/se/BasicStrokeWidget.h
  ${SRCDIR}/widgets/se/ChannelSelectionWidget.h
  ${SRCDIR}/widgets/se/CharMapWidget.h
  ${SRCDIR}/widgets/se/ColorMapWidget.h
  ${SRCDIR}/widgets/se/ContrastEnhancementWidget.h
  ${SRCDIR}/widgets/se/GlyphGraphicWidget.h
  ${SRCDIR}/widgets/se/GlyphMarkPropertyItem.h
  ${SRCDIR}/widgets/se/GlyphMarkWidget.h
  ${SRCDIR}/widgets/se/GraphicProperty.h
  ${SRCDIR}/widgets/se/GraphicPropertyItem.h
  ${SRCDIR}/widgets/se/GraphicFillWidget.h
  ${SRCDIR}/widgets/se/GraphicSelectorWidget.h
  ${SRCDIR}/widgets/se/GroupingWidget.h
  ${SRCDIR}/widgets/se/GroupingDialog.h
  ${SRCDIR}/widgets/se/ImageOutlineWidget.h
  ${SRCDIR}/widgets/se/LineSymbolizerProperty.h
  ${SRCDIR}/widgets/se/LineSymbolizerWidget.h
  ${SRCDIR}/widgets/se/LocalGraphicWidget.h
  ${SRCDIR}/widgets/se/LocalImagePropertyItem.h
  ${SRCDIR}/widgets/se/LocalImageWidget.h
  ${SRCDIR}/widgets/se/OverlapBehaviorWidget.h
  ${SRCDIR}/widgets/se/PointSymbolizerProperty.h
  ${SRCDIR}/widgets/se/PointSymbolizerWidget.h
  ${SRCDIR}/widgets/se/PolygonSymbolizerProperty.h
  ${SRCDIR}/widgets/se/PolygonSymbolizerWidget.h
  ${SRCDIR}/widgets/se/RasterSymbolizerDialog.h
  ${SRCDIR}/widgets/se/RasterSymbolizerWidget.h
  ${SRCDIR}/widgets/se/SelectedChannelWidget.h
  ${SRCDIR}/widgets/se/ShadedReliefWidget.h
  ${SRCDIR}/widgets/se/StyleControllerWidget.h
  ${SRCDIR}/widgets/se/StyleDockWidget.h
  ${SRCDIR}/widgets/se/StyleExplorer.h
  ${SRCDIR}/widgets/se/SymbolEditorWidget.h
  ${SRCDIR}/widgets/se/SymbolSelectorDialog.h
  ${SRCDIR}/widgets/se/SymbolTableWidget.h
  ${SRCDIR}/widgets/se/WellKnownGraphicWidget.h
  ${SRCDIR}/widgets/se/WellKnownMarkPropertyItem.h
  ${SRCDIR}/widgets/se/WellKnownMarkWidget.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/se" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/slider/TimePropertiesDialog.h
  ${SRCDIR}/widgets/slider/TimeSliderWidget.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/slider" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/srs/SRSManagerDialog.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/srs" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/table/DataSetTableView.h
  ${SRCDIR}/widgets/table/DataSetTableVerticalHeader.h
  ${SRCDIR}/widgets/table/AddColumnDialog.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/table" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/temporal/ObservationDialog.h
  ${SRCDIR}/widgets/temporal/TemporalPropertiesWidget.h
  ${SRCDIR}/widgets/temporal/TimeSeriesDialog.h
  ${SRCDIR}/widgets/temporal/TimeSeriesPropertiesWidget.h
  ${SRCDIR}/widgets/temporal/TrajectoryDialog.h
  ${SRCDIR}/widgets/temporal/TrajectoryPropertiesWidget.h
		  
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/temporal" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/tools/CoordTracking.h
  ${SRCDIR}/widgets/tools/Info.h
  ${SRCDIR}/widgets/tools/Measure.h
  ${SRCDIR}/widgets/tools/PanExtent.h
  ${SRCDIR}/widgets/tools/PointPicker.h
  ${SRCDIR}/widgets/tools/PolygonAcquire.h
  ${SRCDIR}/widgets/tools/ReadPixelTool.h
  ${SRCDIR}/widgets/tools/Selection.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/tools" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/plugin/builder/PluginBuilderWizard.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/plugin/builder" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/plugin/manager/PluginManagerDialog.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/plugin/manager" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/widgets/query/DataSetWidget.h
  ${SRCDIR}/widgets/query/DataSetWizardPage.h
  ${SRCDIR}/widgets/query/FieldsWizardPage.h
  ${SRCDIR}/widgets/query/GroupByWizardPage.h
  ${SRCDIR}/widgets/query/LayerAttributesWizardPage.h
  ${SRCDIR}/widgets/query/OrderByWidget.h
  ${SRCDIR}/widgets/query/OrderByWizardPage.h
  ${SRCDIR}/widgets/query/QueryBuilderWizard.h
  ${SRCDIR}/widgets/query/QueryDialog.h
  ${SRCDIR}/widgets/query/WhereClauseWidget.h
  ${SRCDIR}/widgets/query/WhereClauseWizardPage.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/widgets/query" MOC)

# Select the FORMS widgets
file(
  GLOB FORMS
  ${SRCDIR}/widgets/*.ui
  ${SRCDIR}/widgets/canvas/ui/*.ui
  ${SRCDIR}/widgets/charts/ui/*.ui
 # ${SRCDIR}/widgets/datagrid/*.ui
  ${SRCDIR}/widgets/dataset/*.ui
  ${SRCDIR}/widgets/dataset/selector/ui/*.ui
  ${SRCDIR}/widgets/datasource/selector/ui/*.ui
  ${SRCDIR}/widgets/exchanger/ui/*.ui
  ${SRCDIR}/widgets/externalTable/ui/*.ui
  ${SRCDIR}/widgets/layer/search/ui/*.ui
  ${SRCDIR}/widgets/property/*.ui
  ${SRCDIR}/widgets/property/ui/*.ui
  ${SRCDIR}/widgets/utils/ui/*.ui
  ${SRCDIR}/widgets/rp/ui/*.ui
  ${SRCDIR}/widgets/se/ui/*.ui
  ${SRCDIR}/widgets/slider/ui/*.ui
  ${SRCDIR}/widgets/srs/ui/*.ui
  ${SRCDIR}/widgets/table/ui/*.ui
  ${SRCDIR}/widgets/temporal/ui/*.ui
  ${SRCDIR}/widgets/plugin/builder/ui/*.ui
  ${SRCDIR}/widgets/plugin/manager/ui/*.ui
  ${SRCDIR}/widgets/query/ui/*.ui
)

list(REMOVE_ITEM SRCS ${SRCDIR}/postgis.h ${SRCDIR}/widgets.h)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DQWT_DLL)
endif()

set (MOC2 ${CMAKE_CURRENT_BINARY_DIR}/moc_TablePopupFilter.cpp)
QT4_CREATE_MOC_COMMAND("${SRCDIR}/widgets/table/DataSetTableView.cpp" "${MOC2}" "" "-fterralib/qt/widgets/table/DataSetTableView.cpp" )

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
list (APPEND TE_DEP_INCLUDES "${QT_INC_DIRS}")

set (TE_AUX_DIRS ${TE_AUX_DIRS} "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)

list (APPEND SRCS "${MOC}" "${MOC2}" "${UI}" "${qt_pb_headers_MOC}" "${qt_pb_resources_RCC}")

list (REMOVE_ITEM SRCS ${SRCDIR}/widgets/table/DataSetTableView.cpp)

#exporting module information
exportModuleInformation("qt_widgets" "${QT_INC_DIRS}" "${QT_INC_INST_DIRS}")

