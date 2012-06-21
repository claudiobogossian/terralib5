# - Find Qwt
# This module finds an installed Qwt package.
#
# It sets the following variables:
# QWT_FOUND           - Set to false, or undefined, if Qwt isn't found.
# QWT_INCLUDE_DIR     - The Qwt include directory.
# QWT_LIBRARY         - The Qwt library.
# QWT_LIBRARY_DEBUG   - The Qwt debug library
# QWT_LIBRARY_RELEASE - The Qwt release library

find_path(QWT_INCLUDE_DIR qwt.h
          PATHS /usr/local/qwt/include /usr/local/include
                /usr/include/qwt /usr/include
          PATH_SUFFIXES "qwt"
          )
                
if(WIN32)
  find_library(QWT_LIBRARY_DEBUG
               NAMES qwtd
               PATHS 
               )
                              
  find_library(QWT_LIBRARY_RELEASE
               NAME qwt
               )
       
  checkLibNames("QWT")
  
else(WIN32)
  find_library(QWT_LIBRARY
               NAME qwt
               PATHS /usr/local/qwt/lib /usr/local/lib
                     /usr/lib/qwt /lib /usr/lib)
endif(WIN32)                
 
if(QWT_INCLUDE_DIR AND QWT_LIBRARY)
   set(QWT_FOUND TRUE)
endif(QWT_INCLUDE_DIR AND QWT_LIBRARY)

if(QWT_FOUND)
    message(STATUS "-- Found Qwt")
else(QWT_FOUND)
  if(QWT_find_REQUIRED)
    message(FATAL_ERROR "-- Error: Qwt required but NOT found.")
  else ()
    message(STATUS "-- Could not find Qwt")
  endif()
endif(QWT_FOUND)

mark_as_advanced(QWT_INCLUDE_DIR QWT_LIBRARY_DEBUG QWT_LIBRARY_RELEASE)
