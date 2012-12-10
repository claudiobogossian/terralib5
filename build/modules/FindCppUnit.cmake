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
          PATHS /usr/local/include/cppunit /usr/include/cppunit)
                
#if(WIN32)
  find_library(CPPUNIT_LIBRARY_DEBUG
               NAMES cppunitd_dll cppunit
               PATHS /usr/local/lib /lib /usr/lib
               )
                              
  find_library(CPPUNIT_LIBRARY_RELEASE
               NAME cppunit_dll cppunit
               PATHS /usr/local/lib /lib /usr/lib
               )
               
               
  checkLibNames("CPPUNIT")
  
#else(WIN32)
 # find_library(CPPUNIT_LIBRARY
  #             NAME cppunit
   #            PATHS /usr/local/lib /lib /usr/lib)
#endif(WIN32)                

set (CPPUNIT_FOUND FALSE)

if(CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)
  set(CPPUNIT_FOUND TRUE)
  message(STATUS "-- Found CppUnit.")
else()
  if(CppUnit_FIND_REQUIRED)
    message (FATAL_ERROR "-- Error: CppUnit required but NOT found.")
  else()
    message (STATUS "-- Could NOT found CppUnit.")
  endif()
endif()

mark_as_advanced(CPPUNIT_INCLUDE_DIR CPPUNIT_LIBRARY_DEBUG CPPUNIT_LIBRARY_RELEASE)
