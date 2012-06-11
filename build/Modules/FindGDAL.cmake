cmake_minimum_required(VERSION 2.8)

if(WIN32)
  find_path(GDAL_INCLUDE_ALG
            gdal_alg.h
            PATH_SUFFIXES "alg" "gdal/alg")
    
  find_path(GDAL_INCLUDE_GCORE
            gdal.h
            PATH_SUFFIXES "gcore" "gdal/gcore" )
            
  find_path(GDAL_INCLUDE_OGR
            ogr_core.h
            PATH_SUFFIXES "ogr" "gdal/ogr")
            
  find_path(GDAL_INCLUDE_PORT
            cpl_config.h
            PATH_SUFFIXES "port" "gdal/port")

  find_path(GDAL_INCLUDE_OGRSF ogrsf_frmts.h
            PATH_SUFFIXES "ogrsf_frmts" "gdal/ogr" "gdal/ogr/ogrsf_frmts")
            
  set(GDAL_INCLUDE_DIR
       ${GDAL_INCLUDE_ALG}
       ${GDAL_INCLUDE_GCORE}
       ${GDAL_INCLUDE_OGR}
       ${GDAL_INCLUDE_PORT}
       ${GDAL_INCLUDE_OGRSF})
       
  find_library(GDAL_LIBRARY_RELEASE 
               NAMES gdal_i.lib
               )

  find_library(GDAL_LIBRARY_DEBUG 
               NAMES gdal_i.lib
               )
          
  checkLibNames("GDAL")
       
else(WIN32)
  find_path(GDAL_INCLUDE_DIR
            NAMES gdal.h
            PATHS /usr/local/include /usr/local/include/gdal /usr/include/gdal)
            
  find_library(GDAL_LIBRARY
               NAMES gdal
               /usr/local/lib /usr/lib) 
endif(WIN32)
          
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
