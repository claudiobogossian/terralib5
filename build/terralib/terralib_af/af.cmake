find_package(Qt4 ${_Qt4_VERSION} COMPONENTS QtCore QtGui REQUIRED)
if(QT4_FOUND)
  include (${QT_USE_FILE})
  add_definitions (${QT_DEFINITIONS})

  set (DEP_INCLUDES ${QT_INCLUDE_DIR})
  set (DEP_LIBS ${QT_LIBRARIES})
endif()

find_package(Boost ${_Boost_VERSION} COMPONENTS system filesystem REQUIRED)
if(Boost_FOUND)
  list (APPEND DEP_LIBS ${Boost_LIBRARIES})
  list (APPEND DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEQTAFDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list ( APPEND
  DEP_LIBS 
  terralib_common 
  terralib_dataaccess 
  terralib_datatype 
  terralib_plugin 
  terralib_geometry 
  terralib_memory
  terralib_qtwidgets
  terralib_maptools
)

set (
  _DIRS 
  .
  events
  connectors
)

file (
  GLOB HDRS_TO_MOC
  ${SRCDIR}/CoreApplication.h
  ${SRCDIR}/connectors/MapDisplay.h
  ${SRCDIR}/connectors/TabularViewer.h
  ${SRCDIR}/connectors/LayerExplorer.h
)

set (
  HDRS_TO_MOC
  ${SRCDIR}/CoreApplication.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/af" MOC)

set (
  HDRS_TO_MOC
  ${SRCDIR}/connectors/MapDisplay.h
  ${SRCDIR}/connectors/TabularViewer.h
  ${SRCDIR}/connectors/LayerExplorer.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/qt/af/connectors" MOC)

source_group("Generated Files" FILES ${MOC})

# Files in build tree
appPrefix ("${SRCDIR}" "${_DIRS}" AF_INC_DIRS)

# Files in build tree
appPrefix ("qt/af" "${_DIRS}" AF_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

list (APPEND SRCS "${MOC}")

# Include directory of the image files
list (APPEND AF_INC_DIRS "${CMAKE_CURRENT_BINARY_DIR}")
list (APPEND DEP_INCLUDES "${AF_INC_DIRS}")

#exporting module information
exportModuleInformation ("af" "${AF_INC_DIRS}" "${AF_INC_INST_DIRS}")
