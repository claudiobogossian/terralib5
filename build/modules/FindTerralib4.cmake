cmake_minimum_required(VERSION 2.8)

# Find path - tries to find *.h in paths hard-coded by the script
FIND_PATH (TERRALIB4_INCLUDE_TERRALIB
           NAMES teConfig.h
		   PATHS /usr/local/lib /lib /usr/lib $ENV{TE_DEPENDENCIES_DIR}/include/terralib4/terralib)
		   
FIND_PATH (TERRALIB4_INCLUDE_ADO
           NAMES TeAdoDB.h
		   PATHS /usr/local/lib /lib /usr/lib $ENV{TE_DEPENDENCIES_DIR}/include/terralib4/terralib/drivers/ado)
		   
FIND_PATH (TERRALIB4_INCLUDE_KERNEL
           NAMES TeDatabase.h
		   PATHS /usr/local/lib /lib /usr/lib $ENV{TE_DEPENDENCIES_DIR}/include/terralib4/terralib/kernel)
		   
FIND_PATH (TERRALIB4_INCLUDE_UTILS
           NAMES TeUpdateDBVersion.h
		   PATHS /usr/local/lib /lib /usr/lib $ENV{TE_DEPENDENCIES_DIR}/include/terralib4/terralib/utils)
		   
FIND_PATH (TERRALIB4_INCLUDE
           NAMES terralib/teConfig.h
		   PATHS /usr/local/lib /lib /usr/lib $ENV{TE_DEPENDENCIES_DIR}/include/terralib4)

set (TERRALIB4_INCLUDE_DIR
     ${TERRALIB4_INCLUDE_TERRALIB}
	 ${TERRALIB4_INCLUDE_ADO}
	 ${TERRALIB4_INCLUDE_KERNEL}
	 ${TERRALIB4_INCLUDE_UTILS}
	 ${TERRALIB4_INCLUDE})

# Find library - - tries to find *.a,*.so,*.dylib in paths hard-coded by the script

FIND_LIBRARY(TERRALIB4_LIBRARY_DEBUG
             NAMES terralib4d
             PATHS /usr/local/lib /lib /usr/lib $ENV{TE_DEPENDENCIES_DIR}/lib)
			 
FIND_LIBRARY(TERRALIB4_ADO_LIBRARY_DEBUG
             NAMES terralib4_adod
             PATHS /usr/local/lib /lib /usr/lib $ENV{TE_DEPENDENCIES_DIR}/lib)
			 
FIND_LIBRARY(TERRALIB4_LIBRARY_RELEASE
             NAMES terralib4
             PATHS /usr/local/lib /lib /usr/lib $ENV{TE_DEPENDENCIES_DIR}/lib)
			 
FIND_LIBRARY(TERRALIB4_ADO_LIBRARY_RELEASE
             NAMES terralib4_ado
             PATHS /usr/local/lib /lib /usr/lib $ENV{TE_DEPENDENCIES_DIR}/lib)

# Export include and library path for linking with other libraries

set (TERRALIB4_LIBRARY debug;${TERRALIB4_LIBRARY_DEBUG};optimized;${TERRALIB4_LIBRARY_RELEASE})
set (TERRALIB4_ADO_LIBRARY debug;${TERRALIB4_ADO_LIBRARY_DEBUG};optimized;${TERRALIB4_ADO_LIBRARY_RELEASE})

set (TERRALIB4_LIBRARY ${TERRALIB4_LIBRARY} ${TERRALIB4_ADO_LIBRARY})

SET(TERRALIB4_FOUND FALSE)

IF (TERRALIB4_INCLUDE_DIR AND TERRALIB4_LIBRARY)
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