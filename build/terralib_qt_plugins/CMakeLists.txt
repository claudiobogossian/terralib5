cmake_minimum_required(VERSION 2.8)

set (PROJ_NAME "terralib_qt_plugins")
project(${PROJ_NAME})

find_package(terralib 5.0 REQUIRED COMPONENTS common dataaccess plugin qt_widgets qt_af)
if(terralib_FOUND)

  include (${T5_MODULEPATH}/GeneralConfig.cmake)

  if(WIN32)
    include (${T5_MODULEPATH}/WinConfig.cmake)
  endif()

  option (QT_PLUGIN_DATASOURCE_GDAL "Build GDAL datasource datasource plugin for Qt." ON)
  option (QT_PLUGIN_DATASOURCE_GEOFILE "Build GeoFile datasource plugin for Qt." ON)
  option (QT_PLUGIN_DATASOURCE_OGR "Build OGR datasource plugin for Qt." ON)
  option (QT_PLUGIN_DATASOURCE_PGIS "Build PostGIS datasource plugin for Qt." ON)
  option (QT_PLUGIN_DATASOURCE_SHP "Build Shapefile datasource plugin for Qt." ON)
  option (QT_PLUGIN_DATASOURCE_WCS "Build WCS datasource plugin for Qt." ON)  
  option (QT_PLUGIN_DATASOURCE_WFS "Build WFS datasource plugin for Qt." ON)
  option (QT_PLUGIN_DATASOURCE_WMS "Build WMS datasource plugin for Qt." ON)  


  set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../src/terralib/qt/plugins")  # change this if the terralib root directory is somewhere else
    
  # Go into sub-directories
  if(QT_PLUGIN_DATASOURCE_OGR)
    add_subdirectory(datasource_ogr)
  endif()
  
  if(QT_PLUGIN_DATASOURCE_GDAL)
    add_subdirectory(datasource_gdal)
  endif()
  
  if(QT_PLUGIN_DATASOURCE_PGIS)
    add_subdirectory(datasource_pgis)
  endif()
  
  if(QT_PLUGIN_DATASOURCE_GEOFILE)
    add_subdirectory(datasource_geofile)
  endif()
  
  if(QT_PLUGIN_DATASOURCE_SHP)
    add_subdirectory(datasource_shp)
  endif()
  
  if(QT_PLUGIN_DATASOURCE_WCS)
    add_subdirectory(datasource_wcs)
  endif()
  
  if(QT_PLUGIN_DATASOURCE_WMS)
    add_subdirectory(datasource_wms)
  endif()
  
  if(QT_PLUGIN_DATASOURCE_WFS)
    add_subdirectory(datasource_wfs)
  endif()
endif()