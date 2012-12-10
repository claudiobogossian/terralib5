# - Find Qwt
# This module finds an installed Qwt package.
#
# It sets the following variables:
# QWT_FOUND           - Set to false, or undefined, if Qwt isn't found.
# QWT_INCLUDE_DIR     - The Qwt include directory.
# QWT_LIBRARY         - The Qwt library.
# QWT_LIBRARY_DEBUG   - The Qwt debug library
# QWT_LIBRARY_RELEASE - The Qwt release library

find_path(
  QWT_INCLUDE_DIR 
  NAMES qwt.h
  PATHS /usr/local/include/qwt /usr/local/include /usr/include/qwt /include /usr/include 
  PATH_SUFFIXES qwt/include include/qwt
)

find_library(
  QWT_LIBRARY_DEBUG
  NAMES qwtd
  PATHS /usr/local/qwt/lib /usr/local/lib /usr/lib/qwt /lib /usr/lib
  PATH_SUFFIXES "/qwt/lib"
)
                        
find_library(
  QWT_LIBRARY_RELEASE
  NAME qwt
  PATHS /usr/local/qwt/lib /usr/local/lib /usr/lib/qwt /lib /usr/lib
  PATH_SUFFIXES "/qwt/lib"
)
       
checkLibNames("QWT")
  
set(Qwt_FOUND FALSE)

if(QWT_INCLUDE_DIR AND QWT_LIBRARY)
   set(Qwt_FOUND TRUE)
endif()

if(Qwt_FOUND)
    message(STATUS "-- Found Qwt")
else()
  if(Qwt_FIND_REQUIRED)
    message(FATAL_ERROR "-- Error: Qwt required but NOT found.")
  else ()
    message(STATUS "-- Could not find Qwt")
  endif()
endif()

mark_as_advanced(QWT_INCLUDE_DIR QWT_LIBRARY_DEBUG QWT_LIBRARY_RELEASE)
