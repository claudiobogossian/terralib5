cmake_minimum_required(VERSION 2.8)

if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  if("$ENV{ProgramFiles(x86)}")
	set (path_prefix "$ENV{ProgramFiles(x86)}")
  else()
    set (path_prefix "$ENV{ProgramFiles}")
  endif()
else()
  set (path_prefix "$ENV{ProgramFiles}")
endif()

# Find path - tries to find *.h in paths hard-coded by the script
FIND_PATH (ADO_INCLUDE_DIR msado15.dll
           PATHS ${path_prefix}"/Common Files/System/ado")
		   
# Export include 

IF (ADO_INCLUDE_DIR)
	SET(ADO_FOUND TRUE)
  MESSAGE("-- Found ADO")
ELSE (ADO_INCLUDE_DIR)
	MESSAGE("Could NOT find ADO")
	SET(ADO_FOUND FALSE)
ENDIF (ADO_INCLUDE_DIR)

MARK_AS_ADVANCED( ADO_INCLUDE_DIR )