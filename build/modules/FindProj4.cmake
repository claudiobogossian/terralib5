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
find_path(
  PROJ4_INCLUDE_DIR org_proj4_Projections.h
  PATHS /usr/local/include /usr/include 
  PATH_SUFFIXES "proj4" "proj4/include" "include/proj4" "proj" "proj/include"
)

# Find library - - tries to find *.a,*.so,*.dylib in paths hard-coded by the script
find_library(
  PROJ4_LIBRARY_RELEASE
  NAMES proj_i proj
  PATHS /usr/local/lib /lib /usr/lib
  PATH_SUFFIXES "proj4/lib" "proj/lib"
)

find_library(
  PROJ4_LIBRARY_DEBUG
  NAMES proj_i_d proj_d
  PATHS /usr/local/lib /lib /usr/lib
  PATH_SUFFIXES "proj4/lib" "proj/lib"
)

checkLibNames("PROJ4")
  
# Export include and library path for linking with other libraries
if(PROJ4_INCLUDE_DIR AND PROJ4_LIBRARY)
  set(PROJ4_FOUND TRUE)
  message(STATUS "-- Found Proj4 library")
else(PROJ4_INCLUDE_DIR AND PROJ4_LIBRARY)
  if(Proj4_FIND_REQUIRED)
    message(FATAL_ERROR "-- Error: PROJ4 required but NOT found.")
  else()
    message(STATUS "-- Could NOT find PROJ4 library.")
  endif()
endif(PROJ4_INCLUDE_DIR AND PROJ4_LIBRARY)

mark_as_advanced(PROJ4_INCLUDE_DIR PROJ4_LIBRARY_RELEASE PROJ4_LIBRARY_DEBUG)
