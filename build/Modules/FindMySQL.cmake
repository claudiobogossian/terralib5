cmake_minimum_required(VERSION 2.8)

# Find path - tries to find *.h in paths hard-coded by the script
FIND_PATH (MYSQL_INCLUDE_DIR driver/mysql_public_iface.h
           PATHS "$ENV{TERRALIB_DEP_MSVC2010}/mysql/include"
           /usr/local/include/mysql /usr/include/mysql /usr/local/mysql/include)

# Find library - - tries to find *.a,*.so,*.dylib in paths hard-coded by the script

FIND_LIBRARY(MYSQL_LIBRARY
             NAMES libmysqlcppconn mysqlcppconn
             PATHS "$ENV{TERRALIB_DEP_MSVC2010}/mysql/lib32"
             /usr/local/lib /usr/lib /usr/local/mysql/lib)

# Export include and library path for linking with other libraries

SET(MYSQL_FOUND FALSE)

IF (MYSQL_INCLUDE_DIR  AND MYSQL_LIBRARY)
	SET(MYSQL_FOUND TRUE)
  MESSAGE("-- Found MySQL library")
ELSE (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
  if(MySQL_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "-- Error: MySQL required but NOT found.")
  else()
    MESSAGE(STATUS "-- Could NOT find MySQL library")
  endif()
ENDIF (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)

MARK_AS_ADVANCED( MYSQL_LIBRARY MYSQL_INCLUDE_DIR )