# - Try to find the Geos library
# Once done this will define
#
#  GEOS_FOUND       - the system has the Geos librart
#  GEOS_INCLUDE_DIR - the Geos include directory
#  GEOS_LIBRARY     - the Geos library

cmake_minimum_required(VERSION 2.8)

# Find path - tries to find *.h in paths hard-coded by the script
find_path(
  GEOS_INCLUDE_DIR 
  NAMES geos.h geos_c.h
  PATHS /usr/local/include /usr/include
  PATH_SUFFIXES "geos" "geos/include"
)

find_library(
  GEOS_LIBRARY_RELEASE
  NAMES geos geos_i
  PATHS /usr/local/lib /usr/lib  
  PATH_SUFFIXES "geos/lib"
)
             
find_library(
  GEOS_LIBRARY_DEBUG
  NAMES geos_d geos_i_d libgeos_d
  PATHS /usr/local/lib /usr/lib  
  PATH_SUFFIXES "geos/lib"
)
         
checkLibNames("GEOS")
  
set(GEOS_FOUND FALSE)

# Set the GEOS_FOUND variable
if(GEOS_INCLUDE_DIR AND GEOS_LIBRARY)
	set(GEOS_FOUND TRUE)
  message("-- Found GEOS library")
else()
  if(GEOS_FIND_REQUIRED)
    message(FATAL_ERROR "-- GEOS required but NOT found.")
  else()
    message("-- Could NOT find GEOS library")
  endif()
endif()

mark_as_advanced(GEOS_INCLUDE_DIR GEOS_LIBRARY)
