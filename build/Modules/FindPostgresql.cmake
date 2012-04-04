cmake_minimum_required(VERSION 2.8)

# Find path - tries to find *.h in paths hard-coded by the script
FIND_PATH (POSTGRESQL_INCLUDE_DIR postgres_ext.h
           PATHS "$ENV{TERRALIB_DEP_MSVC2010}/postgresql/include"
           /usr/local/include/postgresql /usr/include/postgresql /usr/local/postgresql/include)

# Find library - - tries to find *.a,*.so,*.dylib in paths hard-coded by the script

FIND_LIBRARY(POSTGRESQL_LIBRARY
             NAMES libpq pq
             PATHS "$ENV{TERRALIB_DEP_MSVC2010}/postgresql/lib/win32"
             /usr/local/lib /usr/lib /usr/local/postgresql/lib)

# Export include and library path for linking with other libraries

IF (POSTGRESQL_INCLUDE_DIR  AND POSTGRESQL_LIBRARY)
	SET(POSTGRESQL_FOUND TRUE)
  MESSAGE("-- Found PostgreSQL library")
ELSE (POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARY)
	MESSAGE("Could NOT find PostgreSQL library")
	SET(POSTGRESQL_FOUND FALSE)
ENDIF (POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARY)

set ( POSTGRESQL_LIBS ${POSTGRESQL_LIBRARY} )

MARK_AS_ADVANCED( POSTGRESQL_LIBRARY POSTGRESQL_INCLUDE_DIR )