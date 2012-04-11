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
		  
# Export include and library path for linking with other libraries
	if(DL_INCLUDE_DIR AND DL_LIBRARY)
		set(DL_FOUND TRUE)
		message(STATUS "Found DL library")
	else(DL_INCLUDE_DIR AND DL_LIBRARY)
		message(FATAL_ERROR "Could NOT find DL library")
	endif(DL_INCLUDE_DIR AND DL_LIBRARY)

	mark_as_advanced(DL_FOUND DL_INCLUDE_DIR DL_LIBRARY)
	
endif(UNIX)	
