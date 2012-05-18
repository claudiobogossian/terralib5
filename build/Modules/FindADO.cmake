cmake_minimum_required(VERSION 2.8)

# Find path - tries to find *.h in paths hard-coded by the script
FIND_PATH (ADO_INCLUDE_DIR msado15.dll
           PATHS "C:/Program Files (x86)/Common Files/System/ado")
		   
# Export include 

IF (ADO_INCLUDE_DIR)
	SET(ADO_FOUND TRUE)
  MESSAGE("-- Found ADO")
ELSE (ADO_INCLUDE_DIR)
	MESSAGE("Could NOT find ADO")
	SET(ADO_FOUND FALSE)
ENDIF (ADO_INCLUDE_DIR)

MARK_AS_ADVANCED( ADO_INCLUDE_DIR )