find_package(Qt4 ${_Qt4_VERSION} COMPONENTS QtCore QtGui REQUIRED)

if(QT4_FOUND)
  include (${QT_USE_FILE})
  add_definitions (${QT_DEFINITIONS})

  set (TE_DEP_INCLUDES ${QT_INCLUDE_DIR})
  set (TE_DEP_LIBS ${QT_LIBRARIES})
endif()

find_package(Boost ${_Boost_VERSION} COMPONENTS system filesystem REQUIRED)
if(Boost_FOUND)
  list (APPEND TE_DEP_LIBS ${Boost_LIBRARIES})
  list (APPEND TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(Xerces ${_Xerces_VERSION} REQUIRED)
if(XERCES_FOUND)
  list (APPEND TE_DEP_LIBS ${XERCES_LIBRARIES})
  list (APPEND TE_DEP_INCLUDES ${XERCES_INCLUDE_DIRS})
endif()


#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DTEQTAFDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

add_definitions (-DTE_SCHEMA_LOCATION=\"${TE_SCHEMA_LOCATION}\")

list ( APPEND
  TE_DEP_LIBS 
  terralib_common 
  terralib_dataaccess 
  terralib_datatype 
  terralib_plugin 
  terralib_geometry 
  terralib_memory
  terralib_qt_widgets
  terralib_maptools
  terralib_serialization
  terralib_srs
  terralib_symbology
  terralib_xerces
  terralib_xml
)

set (
  _DIRS 
  .
  connectors
  events
  settings
)

set (
  HDRS_TO_MOC
  ${SRCDIR}/ApplicationController.h
  ${SRCDIR}/ApplicationPlugins.h
  ${SRCDIR}/BaseApplication.h
  ${SRCDIR}/ProjectInfoDialog.h
)

te_moc2("${HDRS_TO_MOC}" "terralib/qt/af" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/connectors/ChartDisplayDockWidget.h
  ${SRCDIR}/connectors/LayoutEditor.h  
  ${SRCDIR}/connectors/DataSetTableDockWidget.h
  ${SRCDIR}/connectors/InterfaceController.h  
  ${SRCDIR}/connectors/LayerExplorer.h
  ${SRCDIR}/connectors/MapDisplay.h  
  ${SRCDIR}/connectors/StyleExplorer.h
)

te_moc2("${HDRS_TO_MOC}" "terralib/qt/af/connectors" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/settings/AbstractSettingWidget.h
  ${SRCDIR}/settings/DisplayWidget.h
  ${SRCDIR}/settings/MenuBarModel.h
  ${SRCDIR}/settings/ProjectWidget.h
  ${SRCDIR}/settings/SettingsDialog.h
  ${SRCDIR}/settings/TableWidget.h
  ${SRCDIR}/settings/ToolbarsWidget.h
  ${SRCDIR}/settings/GeneralConfigWidget.h
)

te_moc2("${HDRS_TO_MOC}" "terralib/qt/af/settings" MOC)

# Select the FORMS widgets
file(
  GLOB FORMS
  ${SRCDIR}/ui/*.ui
  ${SRCDIR}/settings/ui/*.ui
)

qt4_wrap_ui(UI ${FORMS})

install (
  FILES ${UI}
  DESTINATION terralib/qt/af/ui
  COMPONENT HEADERS
)

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${MOC} ${UI})

# Files in build tree
appPrefix ("${SRCDIR}" "${_DIRS}" QT_AF_INC_DIRS)

# Files in build tree
appPrefix ("qt/af" "${_DIRS}" QT_AF_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

list (APPEND SRCS "${MOC}" "${UI}")

# Include directory of the image files
list (APPEND QT_AF_INC_DIRS "${CMAKE_CURRENT_BINARY_DIR}")
list (APPEND TE_DEP_INCLUDES "${QT_AF_INC_DIRS}")

#exporting module information
exportModuleInformation ("qt_af" "${QT_AF_INC_DIRS}" "${QT_AF_INC_INST_DIRS}")
