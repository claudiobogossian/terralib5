cmake_minimum_required(VERSION 2.8)

if(WIN32)
  find_path(GDAL_INCLUDE_ALG
            gdal_alg.h
            PATHS "$ENV{TERRALIB_DEP_MSVC2010}/gdal"
            PATH_SUFFIXES alg)
    
  find_path(GDAL_INCLUDE_GCORE
            gdal.h
            PATHS "$ENV{TERRALIB_DEP_MSVC2010}/gdal"
            PATH_SUFFIXES gcore)
            
  find_path(GDAL_INCLUDE_OGR
            ogr_core.h
            PATHS "$ENV{TERRALIB_DEP_MSVC2010}/gdal"
            PATH_SUFFIXES ogr)
            
  find_path(GDAL_INCLUDE_PORT
            cpl_config.h
            PATHS "$ENV{TERRALIB_DEP_MSVC2010}/gdal"
            PATH_SUFFIXES port)

  find_path(GDAL_INCLUDE_OGRSF ogrsf_frmts.h
            PATHS "$ENV{TERRALIB_DEP_MSVC2010}/gdal/ogr"
            PATH_SUFFIXES ogrsf_frmts)
            
  set(GDAL_INCLUDE_DIR
       ${GDAL_INCLUDE_ALG}
       ${GDAL_INCLUDE_GCORE}
       ${GDAL_INCLUDE_OGR}
       ${GDAL_INCLUDE_PORT}
       ${GDAL_INCLUDE_OGRSF})
       
  find_library(GDAL_LIBRARY_RELEASE 
               NAMES gdal_i.lib
               HINTS "$ENV{TERRALIB_DEP_MSVC2010}/gdal/win32/release")

  find_library(GDAL_LIBRARY_DEBUG 
               NAMES gdal_i.lib 
               HINTS "$ENV{TERRALIB_DEP_MSVC2010}/gdal/win32/debug")

  set(GDAL_LIBRARY
      optimized ${GDAL_LIBRARY_RELEASE}
      debug     ${GDAL_LIBRARY_DEBUG})
       
else(WIN32)
  find_path(GDAL_INCLUDE_DIR
            NAMES gdal.h
            PATHS /usr/local/include /usr/local/include/gdal /usr/include/gdal)
            
  find_library(GDAL_LIBRARY
               NAMES gdal
               /usr/local/lib /usr/lib) 
endif(WIN32)
          
# Export include and library path for linking with other libraries

if(GDAL_INCLUDE_DIR AND GDAL_LIBRARY)
  set(GDAL_FOUND TRUE)
  message("-- Found GDAL library")
else(GDAL_INCLUDE_DIR AND GDAL_LIBRARY)
	message("Could NOT find GDAL library")
	set(GDAL_FOUND FALSE)
endif(GDAL_INCLUDE_DIR AND GDAL_LIBRARY)

set(GDAL_LIBS ${GDAL_LIBRARY} )
	
MARK_AS_ADVANCED( GDAL_LIBRARY GDAL_INCLUDE_DIR)
