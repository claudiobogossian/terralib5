# - Try to find Xerces-C
# Once done this will define
#
#  XERCESC_FOUND       - the system has Xerces-C
#  XERCESC_INCLUDE_DIR - the Xerces-C include directory
#  XERCESC_LIBRARY     - the Xerces-C library

cmake_minimum_required(VERSION 2.8)

# Find path - tries to find *.h in paths hard-coded by the script
find_path(XERCESC_INCLUDE_DIR xercesc/util/XercesVersion.hpp
          PATHS "$ENV{TERRALIB_DEP_MSVC2010}/xerces/include"
          /usr/local/include /usr/include
          PATH_SUFFIXES "xerces")
          
# Find library - - tries to find *.a,*.so,*.dylib in paths hard-coded by the script
if(WIN32)
  find_library(XERCESC_LIBRARY_RELEASE
               NAMES xerces-c
               PATHS "$ENV{TERRALIB_DEP_MSVC2010}/xerces/lib/win32")
               
  find_library(XERCESC_LIBRARY_DEBUG
               NAMES xerces-c_d
               PATHS "$ENV{TERRALIB_DEP_MSVC2010}/xerces/lib/win32")
               
  checkLibNames("XERCESC")

else(WIN32)
  find_library(XERCESC_LIBRARY
               NAMES xerces-c
               /usr/local/lib /usr/lib)  
endif(WIN32)

set(XERCESC_FOUND FALSE)

# Set the XERCESC_FOUND variable
if(XERCESC_INCLUDE_DIR AND XERCESC_LIBRARY)
	set(XERCESC_FOUND TRUE)
  message("-- Found Xerces library")
else()
  if(Xerces_FIND_REQUIRED)
    message(FATAL_ERROR "-- Error: Xerces required but NOT found.")
  else()
    message(STATUS "-- Could NOT find Xerces library")
  endif()
endif()

mark_as_advanced(XERCESC_INCLUDE_DIR XERCESC_LIBRARY_DEBUG XERCESC_LIBRARY_RELEASE)
