cmake_minimum_required(VERSION 2.8)

set (PROJ_NAME "qt_af_plugins")
project(${PROJ_NAME})

find_package(terralib 5.0 REQUIRED COMPONENTS common dataaccess plugin qtwidgets af)
if(terralib_FOUND)
  add_definitions(-DTEAFPLUGINDLL)

  include (${T5_MODULEPATH}/GeneralConfig.cmake)

  if(WIN32)
    include (${T5_MODULEPATH}/WinConfig.cmake)
  endif()

  option (PLUGIN_OGR "Build OGR plug-in." ON)  
  option (PLUGIN_GDAL "Build GDAL plug-in." ON)  
  option (PLUGIN_PGIS "Build PostGIS plug-in." ON)  
  option (PLUGIN_GEOFILE "Build GeoFile plug-in." ON)  
  option (PLUGIN_SHP "Build Shapefile plug-in." ON)  
  option (PLUGIN_WCS "Build WCS plug-in." ON)  
  option (PLUGIN_WMS "Build WMS plug-in." ON)  
  option (PLUGIN_WFS "Build WFS plug-in." ON)  

  set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../src/qt_af_plugins")  # change this if the terralib root directory is somewhere else
    
  # Go into sub-directories
  if(PLUGIN_OGR)
    add_subdirectory(ogr)
  endif()
  
  if(PLUGIN_GDAL)
    add_subdirectory(gdal)
  endif()
  
  if(PLUGIN_PGIS)
    add_subdirectory(pgis)
  endif()
  
  if(PLUGIN_GEOFILE)
    add_subdirectory(geofile)
  endif()
  
  if(PLUGIN_SHP)
    add_subdirectory(shp)
  endif()
  
  if(PLUGIN_WCS)
    add_subdirectory(wcs)
  endif()
  
  if(PLUGIN_WMS)
    add_subdirectory(wms)
  endif()
  
  if(PLUGIN_WFS)
    add_subdirectory(wfs)
  endif()
endif()