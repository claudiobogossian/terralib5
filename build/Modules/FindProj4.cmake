# - Find Proj4
# This module finds an installed Proj4 package.
#
# It sets the following variables:
# PROJ4_FOUND           - Set to false, or undefined, if Proj4 isn't found.
# PROJ4_INCLUDE_DIR     - The Proj4 include directory.
# PROJ4_LIBRARY         - The Proj4 library.
# PROJ4_LIBRARY_DEBUG   - The Proj4 debug library
# PROJ4_LIBRARY_RELEASE - The Proj4 release library

# Find path - tries to find *.h in paths hard-coded by the script
find_path(PROJ4_INCLUDE_DIR org_proj4_Projections.h
          PATHS "$ENV{TERRALIB_DEP_MSVC2010}/proj4/include"
          "/usr/local/include" "/usr/include")

# Find library - - tries to find *.a,*.so,*.dylib in paths hard-coded by the script
if(WIN32)
  find_library(PROJ4_LIBRARY_RELEASE
               NAMES proj_i
               PATHS "$ENV{TERRALIB_DEP_MSVC2010}/proj4/lib/win32/release")

  find_library(PROJ4_LIBRARY_DEBUG
               NAMES proj_i
               PATHS "$ENV{TERRALIB_DEP_MSVC2010}/proj4/lib/win32/debug")       

  if(PROJ4_LIBRARY_RELEASE AND PROJ4_LIBRARY_DEBUG)
    set(PROJ4_LIBRARY 
	      optimized ${PROJ4_LIBRARY_RELEASE}	
	      debug     ${PROJ4_LIBRARY_DEBUG})
  endif(PROJ4_LIBRARY_RELEASE AND PROJ4_LIBRARY_DEBUG)
else(WIN32)
  find_library(PROJ4_LIBRARY
               NAME proj
               PATHS /usr/local/lib /lib /usr/lib)
endif(WIN32)
             
# Export include and library path for linking with other libraries
if(PROJ4_INCLUDE_DIR AND PROJ4_LIBRARY)
  set(PROJ4_FOUND TRUE)
  message(STATUS "Found Proj4 library")
else(PROJ4_INCLUDE_DIR AND PROJ4_LIBRARY)
	message(FATAL_ERROR "Could NOT find PROJ4 library")
endif(PROJ4_INCLUDE_DIR AND PROJ4_LIBRARY)

mark_as_advanced(PROJ4_FOUND PROJ4_INCLUDE_DIR PROJ4_LIBRARY PROJ4_LIBRARY_RELEASE PROJ4_LIBRARY_DEBUG)
