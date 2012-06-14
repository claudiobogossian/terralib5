cmake_minimum_required(VERSION 2.8)

if(UNIX)

	find_path( GRIB_INCLUDE_DIR
		NAMES grib.h grib_api.h
		PATHS /usr/local/include /usr/local/include/grib /usr/include/grib /usr/include)
				
	find_library(GRIB_LIBRARY
		NAMES grib.a libgrib_api.so
		HINTS /usr/local/lib /usr/lib 
  )
			  
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
