
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

#Definitions for Windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS
	                -D_SCL_SECURE_NO_WARNINGS
                 	-DTEVPDLL
					-DBOOST_ALL_NO_LIB
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

list (APPEND TE_DEP_LIBS
	terralib_common
	terralib_dataaccess
	terralib_datatype
	terralib_geometry
	terralib_maptools
	terralib_memory
	terralib_qt_widgets
    terralib_plugin
	terralib_statistics
	terralib_symbology
)

# Files to process.
# --------------------------------------------------
set (
  _DIRS
  .
  qt
  core
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
  ${SRCDIR}/qt/AggregationDialog.h
  ${SRCDIR}/qt/BasicGeographicOperationDialog.h
  ${SRCDIR}/qt/BufferDialog.h
  ${SRCDIR}/qt/IntersectionDialog.h
  ${SRCDIR}/qt/LayerItem.h
  ${SRCDIR}/qt/LayerTreeModel.h
  ${SRCDIR}/qt/LegendGroupItem.h
  ${SRCDIR}/qt/PolygonToLineDialog.h
  ${SRCDIR}/qt/PropertyGroupItem.h  
  ${SRCDIR}/qt/PropertyItem.h
  ${SRCDIR}/qt/SummarizationDialog.h
  ${SRCDIR}/qt/TransformationDialog.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/vp/qt" MOC)

# Select the FORMS widgets
file(
  GLOB FORMS
  ${SRCDIR}/qt/ui/*.ui
)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

qt4_wrap_ui(UI ${FORMS})

install (
  FILES ${UI}
  DESTINATION terralib/vp/ui
  COMPONENT HEADERS
)

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${MOC} ${MOC2} ${UI})

set(VP_IMAGES "${ROOT}/resources/images/svg" CACHE STRING "Path to Vector Processing images.")

# Include directory of the image files
list (APPEND QT_INC_DIRS "${CMAKE_CURRENT_BINARY_DIR}")
list (APPEND QT_INC_INST_DIRS "qt/ui")
list (APPEND TE_DEP_INCLUDES "${QT_INC_DIRS}")

set (TE_AUX_DIRS ${TE_AUX_DIRS} "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)

list (APPEND SRCS "${MOC}" "${MOC2}" "${UI}" "${qt_pb_headers_MOC}" "${qt_pb_resources_RCC}")

#exporting module information
exportModuleInformation("vp" "${QT_INC_DIRS}" "${QT_INC_INST_DIRS}")

configure_file (VectorProcessingConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/VectorProcessingConfig.h)