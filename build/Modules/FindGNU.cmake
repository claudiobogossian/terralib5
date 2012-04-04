# - Try to find GNU Win32 library
# Once done this will define
#
#  GNU_FOUND       - the system has GNU Win32 library
#  GNU_INCLUDE_DIR - the GNU Win32 include directory
#  GNU_LIBRARY     - the GNU Win32 library

cmake_minimum_required(VERSION 2.8)

# Find path - tries to find *.h in paths hard-coded by the script
find_path(GNU_INCLUDE_DIR libintl.h
          PATHS "$ENV{TERRALIB_DEP_MSVC2010}/gnuWin32/include")

# Find library - - tries to find *.a,*.so,*.dylib in paths hard-coded by the script
find_library(GNU_GETTEXTPO_LIBRARY_RELEASE
             NAMES gettextpo.lib
             HINTS "$ENV{TERRALIB_DEP_MSVC2010}/gnuWin32/lib")
             
find_library(GNU_GETTEXTPO_LIBRARY_DEBUG
             NAMES gettextpo_d.lib
             HINTS "$ENV{TERRALIB_DEP_MSVC2010}/gnuWin32/lib")

find_library(GNU_INTL_LIBRARY_RELEASE
             NAMES intl.lib
             HINTS "$ENV{TERRALIB_DEP_MSVC2010}/gnuWin32/lib")
             
find_library(GNU_INTL_LIBRARY_DEBUG
             NAMES intl_d.lib
             HINTS "$ENV{TERRALIB_DEP_MSVC2010}/gnuWin32/lib")
             
if(GNU_GETTEXTPO_LIBRARY_RELEASE AND GNU_GETTEXTPO_LIBRARY_DEBUG AND
   GNU_INTL_LIBRARY_RELEASE AND GNU_INTL_LIBRARY_DEBUG)
  set(GNU_LIBRARY
	    optimized ${GNU_GETTEXTPO_LIBRARY_RELEASE}
	    optimized ${GNU_INTL_LIBRARY_RELEASE}
	    debug     ${GNU_GETTEXTPO_LIBRARY_DEBUG}
	    debug     ${GNU_INTL_LIBRARY_DEBUG})
endif(GNU_GETTEXTPO_LIBRARY_RELEASE AND GNU_GETTEXTPO_LIBRARY_DEBUG AND
      GNU_INTL_LIBRARY_RELEASE AND GNU_INTL_LIBRARY_DEBUG)

# Set the GNU_FOUND variable

if(GNU_INCLUDE_DIR AND GNU_LIBRARY)
	set(GNU_FOUND TRUE)
  message("-- Found the GNUWin32 library")
else(GNU_INCLUDE_DIR AND GNU_LIBRARY)
  set(GNU_FOUND FALSE)
	message("-- Could not find the GNUWin32 library")
endif(GNU_INCLUDE_DIR AND GNU_LIBRARY)

mark_as_advanced(GNU_INCLUDE_DIR GNU_LIBRARY)
