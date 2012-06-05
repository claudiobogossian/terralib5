cmake_minimum_required(VERSION 2.8)

if(UNIX)

	find_path(GRIB_INCLUDE_DIR
		NAMES grib.h
		PATHS /usr/local/include /usr/local/include/grib /usr/include/grib "$ENV{TERRALIB_DEP_MSVC2010}/grib/src")
				
	find_library(GRIB_LIBRARY
		NAMES grib.a
		HINTS /usr/local/lib /usr/lib "$ENV{TERRALIB_DEP_MSVC2010}/grib/src")
			  
		set(GRIB_FOUND FALSE)
    
	# Export include and library path for linking with other libraries
	if(GRIB_INCLUDE_DIR AND GRIB_LIBRARY)
	  set(GRIB_FOUND TRUE)
	  message("-- Found GRIB library")
	else()
    if(Grib_FIND_REQUIRED)
      message(FATAL_ERROR "-- Error: Grib required but NOT found.")
    else()
      message(STATUS "-- Could NOT find GRIB library")
    endif()
	endif()
		
	MARK_AS_ADVANCED(GRIB_LIBRARY GRIB_INCLUDE_DIR)
	
 endif(UNIX)