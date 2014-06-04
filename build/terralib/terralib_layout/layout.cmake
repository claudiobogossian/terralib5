
find_package(Qt4 ${_Qt4_VERSION} COMPONENTS QtCore QtGui REQUIRED)
if(QT4_FOUND)
  include (${QT_USE_FILE})
  add_definitions (${QT_DEFINITIONS})

  set (TE_DEP_INCLUDES ${QT_INCLUDE_DIR})
  set (TE_DEP_LIBS ${QT_LIBRARIES})
endif()

find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  list (APPEND TE_DEP_LIBS ${Boost_LIBRARIES})
  list (APPEND TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

#Definitions for Windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTELAYOUTDLL -DBOOST_ALL_NO_LIB
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
	terralib_plugin
	terralib_qt_widgets
	terralib_color
	terralib_srs
	terralib_symbology
	qtpropertybrowser
)

# Files to process.
# --------------------------------------------------
set (
  _DIRS
  .
  core
  include
  include/core
  include/core/enum
  include/core/pattern
  include/core/pattern/derivativevisitor
  include/core/pattern/factory
  include/core/pattern/mvc
  include/core/pattern/singleton
  include/core/serialization
  include/core/property
  include/core/template
  include/item
  include/outside
  qt
  qt/ui
  qt/include/core
  qt/include/item
  qt/include/main
  qt/include/moc
  qt/include/outside
  qt/include/tools
  qt/include/utils
  qt/include/core/pattern/factory
  qt/src/core
  qt/src/item
  qt/src/main
  qt/src/outside
  qt/src/tools
  qt/src/utils
  qt/src/core/pattern/factory
  src
  src/core
  src/core/enum
  src/core/pattern
  src/core/pattern/derivativevisitor
  src/core/pattern/factory
  src/core/pattern/mvc
  src/core/pattern/singleton
  src/core/serialization
  src/core/property
  src/core/template
  src/item
  src/outside
)

# Files in build tree
appPrefix ("${SRCDIR}" "${_DIRS}" QT_INC_DIRS)

# Files in build tree
appPrefix ("core" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("include" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("include/core" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("include/core/enum" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("include/core/pattern/derivativevisitor" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("include/core/pattern/factory" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("include/core/pattern/mvc" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("include/core/pattern/singleton" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("include/core/serialization" "${_DIRS}" QT_INC_INST_DIRS)  

# Files in build tree
appPrefix ("include/core/property" "${_DIRS}" QT_INC_INST_DIRS) 

# Files in build tree
appPrefix ("include/core/template" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("include/item" "${_DIRS}" QT_INC_INST_DIRS)  

# Files in build tree
appPrefix ("include/outside" "${_DIRS}" QT_INC_INST_DIRS)  

# Files in build tree
appPrefix ("qt" "${_DIRS}" QT_INC_INST_DIRS)  

# Files in build tree
appPrefix ("qt/ui" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("qt/include/core" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("qt/include/core/pattern/factory" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("qt/include/item" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("qt/include/main" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("qt/include/moc" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("qt/include/outside" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("qt/include/tools" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("qt/include/utils" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("qt/src/core" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("qt/src/core/pattern/factory" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("qt/src/item" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("qt/src/main" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("qt/src/outside" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("qt/src/tools" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("qt/src/utils" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("src" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("src/core" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("src/pattern/derivativevisitor" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("src/pattern/factory" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("src/pattern/mvc" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("src/pattern/singleton" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("src/core/serialization" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("src/core/property" "${_DIRS}" QT_INC_INST_DIRS)

# Files in build tree
appPrefix ("src/core/template" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("src/item" "${_DIRS}" QT_INC_INST_DIRS)
  
# Files in build tree
appPrefix ("src/outside " "${_DIRS}" QT_INC_INST_DIRS)
  
# Get files by structured by folders.
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

set (MOC "")

# Select the header files for moc'ing
set (
  HDRS_TO_MOC
  ${SRCDIR}/qt/include/moc/Scene.h
  ${SRCDIR}/qt/include/moc/PropertiesOutside.h
  ${SRCDIR}/qt/include/moc/DisplayOutside.h
  ${SRCDIR}/qt/include/moc/ObjectInspectorOutside.h
  ${SRCDIR}/qt/include/moc/ToolbarOutside.h
  ${SRCDIR}/qt/include/moc/View.h
  ${SRCDIR}/qt/include/moc/ObjectItem.h
  ${SRCDIR}/qt/include/moc/MapItem.h
  ${SRCDIR}/qt/include/moc/PropertyBrowser.h
  ${SRCDIR}/qt/include/moc/VisualizationArea.h
  ${SRCDIR}/qt/include/moc/OutsideArea.h
  ${SRCDIR}/qt/include/moc/MapGridItem.h
  ${SRCDIR}/qt/include/moc/GridSettingsOutside.h
  ${SRCDIR}/qt/include/moc/ObjectInspectorPropertyBrowser.h
  ${SRCDIR}/qt/include/moc/PropertiesItemPropertyBrowser.h
  ${SRCDIR}/qt/include/moc/PageSetupOutside.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/layout/qt/include/moc" MOC)

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
  DESTINATION terralib/layout/ui
  COMPONENT HEADERS
)

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${MOC} ${MOC2} ${UI})

set(LAYOUT_IMAGES_PNG "${ROOT}/resources/images/png/terralib_layout" CACHE STRING "Path to layout images.")

set(LAYOUT_IMAGES_SVG "${ROOT}/resources/images/svg" CACHE STRING "Path to layout images.")

set(LAYOUT_IMAGES_ICO "${ROOT}/resources/images/ico" CACHE STRING "Path to layout images.")

# Include directory of the image files
list (APPEND QT_INC_DIRS "${CMAKE_CURRENT_BINARY_DIR}")
list (APPEND QT_INC_INST_DIRS "qt/ui")
list (APPEND TE_DEP_INCLUDES "${QT_INC_DIRS}")

set (TE_AUX_DIRS ${TE_AUX_DIRS} "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)

list (APPEND SRCS "${MOC}" "${MOC2}" "${UI}" "${qt_pb_headers_MOC}" "${qt_pb_resources_RCC}")

#exporting module information
exportModuleInformation("layout" "${QT_INC_DIRS}" "${QT_INC_INST_DIRS}")

configure_file (LayoutConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/LayoutConfig.h)