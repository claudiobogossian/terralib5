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
          /usr/local/include /usr/include)
          
# Find library - - tries to find *.a,*.so,*.dylib in paths hard-coded by the script
if(WIN32)
  find_library(XERCESC_LIBRARY_RELEASE
               NAMES xerces-c
               PATHS "$ENV{TERRALIB_DEP_MSVC2010}/xerces/lib/win32")
               
  find_library(XERCESC_LIBRARY_DEBUG
               NAMES xerces-c_d
               PATHS "$ENV{TERRALIB_DEP_MSVC2010}/xerces/lib/win32")
               
  if(XERCESC_LIBRARY_RELEASE AND XERCESC_LIBRARY_DEBUG)
    set(XERCESC_LIBRARY
      optimized ${XERCESC_LIBRARY_RELEASE}
      debug     ${XERCESC_LIBRARY_DEBUG})
  endif(XERCESC_LIBRARY_RELEASE AND XERCESC_LIBRARY_DEBUG)
else(WIN32)
  find_library(XERCESC_LIBRARY
               NAMES xerces-c
               /usr/local/lib /usr/lib)  
endif(WIN32)

# Set the XERCESC_FOUND variable
if(XERCESC_INCLUDE_DIR AND XERCESC_LIBRARY)
	set(XERCESC_FOUND TRUE)
  message("-- Found Xerces library")
else(XERCESC_INCLUDE_DIR AND XERCESC_LIBRARY)
	message("Could NOT find Xerces library")
	set(XERCESC_FOUND FALSE)
endif(XERCESC_INCLUDE_DIR  AND XERCESC_LIBRARY)

mark_as_advanced(XERCESC_INCLUDE_DIR XERCESC_LIBRARY)
