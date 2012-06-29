cmake_minimum_required(VERSION 2.8)

find_path(
  GDAL_INCLUDE_ALG
  gdal_alg.h
  PATH_SUFFIXES "alg" "gdal/alg" "gdal/include"
)
  
find_path(
  GDAL_INCLUDE_GCORE
  gdal.h
  PATHS /usr/local/include /usr/local/include/gdal /usr/include/gdal
  PATH_SUFFIXES "gcore" "gdal/gcore" "gdal/include"
)
          
find_path(
  GDAL_INCLUDE_OGR
  ogr_core.h
  PATH_SUFFIXES "ogr" "gdal/ogr" "gdal/include"
)
          
find_path(
  GDAL_INCLUDE_PORT
  cpl_config.h
  PATH_SUFFIXES "port" "gdal/port" "gdal/include"
)

find_path(
  GDAL_INCLUDE_OGRSF 
  ogrsf_frmts.h
  PATH_SUFFIXES "ogrsf_frmts" "gdal/ogr" "gdal/ogr/ogrsf_frmts" "gdal/include"
)
            
set(GDAL_INCLUDE_DIR
     ${GDAL_INCLUDE_ALG}
     ${GDAL_INCLUDE_GCORE}
     ${GDAL_INCLUDE_OGR}
     ${GDAL_INCLUDE_PORT}
     ${GDAL_INCLUDE_OGRSF})
       
find_library(
  GDAL_LIBRARY_RELEASE 
  NAMES gdal_i.lib gdal gdal1 gdal16
  PATHS /usr/local/lib /usr/lib
  PATH_SUFFIXES "gdal/lib"
)

find_library(
  GDAL_LIBRARY_DEBUG 
  NAMES gdal_i.lib gdal gdal1 gdal16
  PATHS /usr/local/lib /usr/lib
  PATH_SUFFIXES "gdal/lib"
)
          
checkLibNames("GDAL")
       
# Export include and library path for linking with other libraries

set(GDAL_FOUND FALSE)

if(GDAL_INCLUDE_DIR AND GDAL_LIBRARY)
  set(GDAL_FOUND TRUE)
  message("-- Found GDAL library")
else()
  if(GDAL_FIND_REQUIRED)
    message(FATAL_ERROR "-- Error: GDAL required but NOT found.")
  else()
    message(STATUS "-- Could not find GDAL.")
  endif()
endif()

MARK_AS_ADVANCED( GDAL_LIBRARY GDAL_INCLUDE_DIR)
