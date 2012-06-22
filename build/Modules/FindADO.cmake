cmake_minimum_required(VERSION 2.8)

if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  if("$ENV{CommonProgramFiles(x86)}" EQUAL "")
    set (path_prefix "$ENV{CommonProgramFiles}")
  else()
    set (path_prefix "$ENV{CommonProgramFiles(x86)}")
  endif()
else()
  set (path_prefix "$ENV{CommonProgramFiles}")
endif()

# Find path - tries to find *.h in paths hard-coded by the script
FIND_PATH (ADO_INCLUDE_DIR msado15.dll
           PATHS ${path_prefix}"/System/ado")
		   
# Export include 

IF (ADO_INCLUDE_DIR)
	SET(ADO_FOUND TRUE)
  MESSAGE("-- Found ADO")
ELSE (ADO_INCLUDE_DIR)
	MESSAGE("Could NOT find ADO")
	SET(ADO_FOUND FALSE)
ENDIF (ADO_INCLUDE_DIR)

MARK_AS_ADVANCED( ADO_INCLUDE_DIR )