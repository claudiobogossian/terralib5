# - Find DL
# This module finds an installed DL package.
#
# It sets the following variables:
# DL_FOUND           - Set to false, or undefined, if DL isn't found.
# DL_INCLUDE_DIR     - The DL include directory.
# DL_LIBRARY         - The DL library.

# Find path - tries to find *.h in paths hard-coded by the script

if(UNIX)
	find_path(DL_INCLUDE_DIR dlfcn.h
		PATHS "/usr/local/include" "/usr/include")

	find_library(DL_LIBRARY
				 NAMES dl libdl
				 HINTS /usr/lib /usr/lib/i386-linux-gnu/)
		  
  set(DL_FOUND FALSE)
  
# Export include and library path for linking with other libraries
	if(DL_INCLUDE_DIR AND DL_LIBRARY)
		message(STATUS "-- Found DL library")
	else()
    if(DL_FIND_REQUIRED)
      message(FATAL_ERROR "-- Error: DL required but NOT found.")
    else()
      message(STATUS "-- Could NOT find DL library")
    endif()
	endif()

	mark_as_advanced(DL_INCLUDE_DIR DL_LIBRARY)
	
endif(UNIX)	
