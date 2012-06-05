# - Try to find GNU Win32 library
# Once done this will define
#
#  GNU_FOUND       - the system has GNU Win32 library
#  GNU_INCLUDE_DIR - the GNU Win32 include directory
#  GNU_LIBRARY     - the GNU Win32 library

cmake_minimum_required(VERSION 2.8)

# Find path - tries to find *.h in paths hard-coded by the script
find_path( GNU_INCLUDE_DIR NAMES libintl.h 
        PATH_SUFFIXES gnuWin32
        )

# Find library - - tries to find *.a,*.so,*.dylib in paths hard-coded by the script
find_library(GNU_GETTEXTPO_LIBRARY_RELEASE
             NAMES gettextpo.lib 
             )
#             HINTS "$ENV{TERRALIB_DEP_MSVC2010}/gnuWin32/lib")
             
find_library(GNU_GETTEXTPO_LIBRARY_DEBUG
             NAMES gettextpo_d.lib 
             )
#             HINTS "$ENV{TERRALIB_DEP_MSVC2010}/gnuWin32/lib")

find_library(GNU_INTL_LIBRARY_RELEASE
             NAMES intl.lib
             )
#             HINTS "$ENV{TERRALIB_DEP_MSVC2010}/gnuWin32/lib")
             
find_library(GNU_INTL_LIBRARY_DEBUG
             NAMES intl_d.lib )

checkLibNames("GNU_GETTEXTPO")
checkLibNames("GNU_INTL")
             
set (GNU_LIBRARY ${GNU_GETTEXTPO_LIBRARY} ${GNU_INTL_LIBRARY})             

set(GNU_FOUND FALSE)

if(GNU_INCLUDE_DIR AND GNU_LIBRARY)
	set(GNU_FOUND TRUE)
  message("-- Found GNUWin32 library")
else()
  if(GNU_FIND_REQUIRED)
    message(FATAL_ERROR "-- Gnu required but NOT find.")
  else()
    message(STATUS "-- Could NOT find the Gnu.")
  endif()
endif()

mark_as_advanced(GNU_INCLUDE_DIR GNU_LIBRARY)
