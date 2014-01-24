find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_LIBS ${Boost_LIBRARIES})
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(GDAL ${_GDAL_VERSION})
if(GDAL_FOUND)
	list (APPEND TE_DEP_LIBS ${GDAL_LIBRARY})
	list (APPEND TE_DEP_INCLUDES ${GDAL_INCLUDE_DIR})
  set (TE_USE_WMS TRUE PARENT_SCOPE)
endif()

find_path ( 
  GDAL_DATADIR NAME compdcs.csv 
  PATH ${TE_DEPENDENCIES_DIR} 
  PATH_SUFFIXES gdal/data 
  DOC "Path to where the gdal data directory is located."
  )
  
find_package(Qt4 ${_Qt4_VERSION} COMPONENTS QtCore QtGui REQUIRED)
if(QT4_FOUND)
  include (${QT_USE_FILE})
  add_definitions (${QT_DEFINITIONS})

  list (APPEND TE_DEP_LIBS ${QT_LIBRARIES})
  list (APPEND TE_DEP_INCLUDES ${QT_INCLUDE_DIR})
endif()

# Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DTEWMSDLL -DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)
endif(WIN32)

list (APPEND TE_DEP_LIBS 
		terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_geometry
		terralib_gdal
		terralib_maptools
		terralib_plugin
		terralib_qt_widgets
		terralib_raster
		terralib_symbology
    )

# Files to process.
# --------------------------------------------------
set (
  _DIRS
  .
  qt
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
  ${SRCDIR}/qt/WMSLayerItem.h
)
te_moc2("${HDRS_TO_MOC}" "terralib/wms/qt" MOC)

source_group("Generated Files" FILES ${MOC} ${MOC2} ${UI})

list (APPEND SRCS "${MOC}" "${MOC2}")

#exporting module information
exportModuleInformation("wms" "${SRCDIR}" "wms")

set (TE_PLUGIN_NAME "te.da.wms")
set (TE_PLUGIN_DISPLAYNAME "WMS Data Source Driver")
set (TE_PLUGIN_DESCRIPTION "This plugin enables TerraLib to access data via WMS servers")
set (TE_PLUGIN_LIBRARY "terralib_wms")
set (TE_PLUGIN_HREFDOC "http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:designimplementation:dataaccess:wms")
set (TE_PLUGIN_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TE_PLUGIN_NAME}.teplg")
set (TE_PLUGIN_REQUIREMENTS "")

set (RSC_LINK "")
getPluginResources("SharedLibraryName" "${TE_PLUGIN_LIBRARY}" RSC_LINK)
set (TE_PLUGIN_RESOURCES 
  "\n  <Resources>${RSC_LINK}\n  </Resources>\n"
)

#Exporting plug-in information for the configuration file
set (TE_PLG_INFO "")
getPluginInfo("${TE_PLUGIN_NAME}" "${TE_PLUGIN_FILE}" TE_PLG_INFO)
set (TE_PLUGINS_DESCRIPTION "${TE_PLUGINS_DESCRIPTION}${TE_PLG_INFO}" PARENT_SCOPE)

configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${TE_PLUGIN_FILE})

#Exporting plugin information for the configuration file in install tree
set (TE_SCHEMA_LOCATION "../schemas/terralib")
set (TE_PLUGIN_REQUIREMENTS "")

set (TE_PLG_INFO "")
getPluginInfo("${TE_PLUGIN_NAME}" "./${TE_PLUGIN_NAME}.teplg" TE_PLG_INFO)
set (TE_INST_PLUGINS_DESCRIPTION "${TE_INST_PLUGINS_DESCRIPTION}${TE_PLG_INFO}" PARENT_SCOPE)
  
configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${CMAKE_BINARY_DIR}/toInstall/${TE_PLUGIN_NAME}.teplg)

set (TE_ENABLED_PLUGINS "${TE_ENABLED_PLUGINS}\n    <Plugin>${TE_PLUGIN_NAME}</Plugin>" PARENT_SCOPE)
