cmake_minimum_required(VERSION 2.8)

# Find path - tries to find *.h in paths hard-coded by the script
FIND_PATH (TERRALIB4_INCLUDE_TERRALIB terralib/teConfig.h)
FIND_PATH (TERRALIB4_INCLUDE_ADO terralib/drivers/ado/TeAdoDB.h)
FIND_PATH (TERRALIB4_INCLUDE_KERNEL terralib/kernel/TeDatabase.h)
FIND_PATH (TERRALIB4_INCLUDE_UTILS terralib/utils/TeUpdateDBVersion.h)

set (TERRALIB4_INCLUDE_DIR
     ${TERRALIB4_INCLUDE_TERRALIB}
	 ${TERRALIB4_INCLUDE_ADO}
	 ${TERRALIB4_INCLUDE_KERNEL}
	 ${TERRALIB4_INCLUDE_UTILS})

# Find library - - tries to find *.a,*.so,*.dylib in paths hard-coded by the script

FIND_LIBRARY(TERRALIB4_LIBRARY_DEBUG
             NAMES terralib
             PATHS /usr/local/lib /lib /usr/lib)
			 
FIND_LIBRARY(TERRALIB4_ADO_LIBRARY_DEBUG
             NAMES terralib_ado
             PATHS /usr/local/lib /lib /usr/lib)

# Export include and library path for linking with other libraries

set(TERRALIB4_LIBRARY
     ${TERRALIB4_LIBRARY_DEBUG}
     ${TERRALIB4_ADO_LIBRARY_DEBUG})

SET(TERRALIB4_FOUND FALSE)

IF (TERRALIB4_INCLUDE_DIR  AND TERRALIB4_LIBRARY)
	SET(TERRALIB4_FOUND TRUE)
  MESSAGE("-- Found TERRALIB4 library")
ELSE ()
  if(TERRALIB4_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "-- Error: Terralib 4 required but NOT found.")
  else()
    MESSAGE(STATUS "-- Could NOT find Terralib4 library")
  endif()
ENDIF ()

MARK_AS_ADVANCED( TERRALIB4_LIBRARY TERRALIB4_INCLUDE_DIR )