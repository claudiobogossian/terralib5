# - Find CppUnit
# This module finds an installed CppUnit package.
#
# It sets the following variables:
# CPPUNIT_FOUND           - Set to false, or undefined, if CppUnit isn't found.
# CPPUNIT_INCLUDE_DIR     - The CppUnit include directory.
# CPPUNIT_LIBRARY         - The CppUnit library.
# CPPUNIT_LIBRARY_DEBUG   - The CppUnit debug library
# CPPUNIT_LIBRARY_RELEASE - The CppUnit release library

find_path(CPPUNIT_INCLUDE_DIR cppunit/Test.h
          PATHS "$ENV{TERRALIB_DEP_MSVC2010}/cppunit/include"
                /usr/local/include/cppunit /usr/include/cppunit)
                
if(WIN32)
  find_library(CPPUNIT_LIBRARY_DEBUG
               NAMES cppunitd_dll
               PATHS "$ENV{TERRALIB_DEP_MSVC2010}/cppunit/src/cppunit/x86/DebugDll")
                              
  find_library(CPPUNIT_LIBRARY_RELEASE
               NAME cppunit_dll
               PATHS "$ENV{TERRALIB_DEP_MSVC2010}/cppunit/src/cppunit/x86/ReleaseDll")
               
  if(CPPUNIT_LIBRARY_DEBUG AND CPPUNIT_LIBRARY_RELEASE)
    set(CPPUNIT_LIBRARY TRUE)
  endif(CPPUNIT_LIBRARY_DEBUG AND CPPUNIT_LIBRARY_RELEASE)
else(WIN32)
  find_library(CPPUNIT_LIBRARY
               NAME cppunit
               PATHS /usr/local/lib /lib /usr/lib)
endif(WIN32)                
 
if(CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)
   set(CPPUNIT_FOUND TRUE)
endif(CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)

if(CPPUNIT_FOUND)
    message(STATUS "Found CppUnit")
else(CPPUNIT_FOUND)
    message(FATAL_ERROR "Could not find CppUnit")
endif(CPPUNIT_FOUND)

mark_as_advanced(CPPUNIT_INCLUDE_DIR CPPUNIT_LIBRARY CPPUNIT_LIBRARY_DEBUG CPPUNIT_LIBRARY_RELEASE)
