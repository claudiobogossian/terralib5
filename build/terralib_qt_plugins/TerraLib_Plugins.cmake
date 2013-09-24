if(WIN32)
  option (BUILD_QT_PLUGIN_DATASOURCE_ADO "Build ADO datasource datasource plugin for Qt." ON)
endif()

option (BUILD_QT_PLUGIN_DATASOURCE_GDAL "Build GDAL datasource datasource plugin for Qt." ON)
option (BUILD_QT_PLUGIN_DATASOURCE_GEOFILE "Build GeoFile datasource plugin for Qt." OFF)
option (BUILD_QT_PLUGIN_DATASOURCE_MYSQL "Build PostGIS datasource plugin for Qt." OFF)
option (BUILD_QT_PLUGIN_DATASOURCE_OGR "Build OGR datasource plugin for Qt." ON)
option (BUILD_QT_PLUGIN_DATASOURCE_PGIS "Build PostGIS datasource plugin for Qt." ON)
option (BUILD_QT_PLUGIN_DATASOURCE_SHP "Build Shapefile datasource plugin for Qt." ON)
option (BUILD_QT_PLUGIN_DATASOURCE_SQLITE "Build SQLite datasource plugin for Qt." OFF)
option (BUILD_QT_PLUGIN_DATASOURCE_WCS "Build WCS datasource plugin for Qt." OFF)  
option (BUILD_QT_PLUGIN_DATASOURCE_WFS "Build WFS datasource plugin for Qt." OFF)
option (BUILD_QT_PLUGIN_DATASOURCE_WMS "Build WMS datasource plugin for Qt." OFF)  
option (BUILD_QT_PLUGIN_LAYOUT "Build LAYOUT plugin for Qt." ON)
option (BUILD_QT_PLUGIN_RP "Build RP plugin for Qt." OFF)
option (BUILD_QT_PLUGIN_VP "Build VP plugin for Qt." OFF) 

set (
  TE_INCLUDE_DIRS
  "${CMAKE_CURRENT_BINARY_DIR}" 
  "${CMAKE_CURRENT_SOURCE_DIR}/../src"
)

set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../src/terralib/qt/plugins")  # change this if the terralib root directory is somewhere else
 
if(WIN32)
  # Go into sub-directories
  if(BUILD_QT_PLUGIN_DATASOURCE_ADO)
    add_subdirectory(terralib_qt_plugins/datasource_ado)
  endif()
endif()

if(BUILD_QT_PLUGIN_DATASOURCE_GDAL)
  add_subdirectory(terralib_qt_plugins/datasource_gdal)
endif()

if(BUILD_QT_PLUGIN_DATASOURCE_GEOFILE)
  add_subdirectory(terralib_qt_plugins/datasource_geofile)
endif()

if(BUILD_QT_PLUGIN_DATASOURCE_MYSQL)
  add_subdirectory(terralib_qt_plugins/datasource_mysql)
endif()

if(BUILD_QT_PLUGIN_DATASOURCE_OGR)
  add_subdirectory(terralib_qt_plugins/datasource_ogr)
endif()

if(BUILD_QT_PLUGIN_DATASOURCE_PGIS)
  add_subdirectory(terralib_qt_plugins/datasource_pgis)
endif()

if(BUILD_QT_PLUGIN_DATASOURCE_SHP)
  add_subdirectory(terralib_qt_plugins/datasource_shp)
endif()

if(BUILD_QT_PLUGIN_DATASOURCE_SQLITE)
  add_subdirectory(terralib_qt_plugins/datasource_sqlite)
endif()

if(BUILD_QT_PLUGIN_DATASOURCE_WCS)
  add_subdirectory(terralib_qt_plugins/datasource_wcs)
endif()

if(BUILD_QT_PLUGIN_DATASOURCE_WMS)
  add_subdirectory(terralib_qt_plugins/datasource_wms)
endif()

if(BUILD_QT_PLUGIN_DATASOURCE_WFS)
  add_subdirectory(terralib_qt_plugins/datasource_wfs)
endif()

if(BUILD_QT_PLUGIN_LAYOUT)
  add_subdirectory(terralib_qt_plugins/layout)
endif()

if(BUILD_QT_PLUGIN_RP)
  add_subdirectory(terralib_qt_plugins/rp)
endif()

if(BUILD_QT_PLUGIN_VP)
  add_subdirectory(terralib_qt_plugins/vp)
endif()
