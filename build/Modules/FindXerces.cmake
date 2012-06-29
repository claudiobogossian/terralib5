# - Try to find Xerces-C
# Once done this will define
#
#  XERCES_FOUND       - the system has Xerces-C
#  XERCES_INCLUDE_DIR - the Xerces-C include directory
#  XERCES_LIBRARY     - the Xerces-C library

cmake_minimum_required(VERSION 2.8)

# Find path - tries to find *.h in paths hard-coded by the script
find_path(
  XERCES_INCLUDE_DIR xercesc/util/XercesVersion.hpp
  PATHS /usr/local/include /usr/include
  PATH_SUFFIXES "xerces" "xerces/include" "include/xerces"
)
          
# Find library - - tries to find *.a,*.so,*.dylib in paths hard-coded by the script
find_library(
  XERCES_LIBRARY_RELEASE
  NAMES xerces-c
  PATHS /usr/local/lib /usr/lib
  PATH_SUFFIXES "xerces/lib"
)
             
find_library(
  XERCES_LIBRARY_DEBUG
  NAMES xerces-c_d
  PATHS /usr/local/lib /usr/lib
  PATH_SUFFIXES "xerces/lib"
)
             
checkLibNames("XERCES")

set(Xerces_FOUND FALSE)

# Set the XERCESC_FOUND variable
if(XERCES_INCLUDE_DIR AND XERCES_LIBRARY)
	set(Xerces_FOUND TRUE)
  message("-- Found Xerces library")
else()
  if(Xerces_FIND_REQUIRED)
    message(FATAL_ERROR "-- Error: Xerces required but NOT found.")
  else()
    message(STATUS "-- Could NOT find Xerces library")
  endif()
endif()

mark_as_advanced(XERCES_INCLUDE_DIR XERCES_LIBRARY_DEBUG XERCES_LIBRARY_RELEASE)
