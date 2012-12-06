# - Find CSQL
# This module finds an installed CSQL package.
#
# It sets the following variables:
# CSQL_FOUND           - Set to false, or undefined, if CSQL isn't found.
# CSQL_INCLUDE_DIR     - The CSQL include directory.
# CSQL_LIBRARY         - The CSQL library.


find_path(CSQL_INCLUDE_DIR sql.h
          PATHS /usr/local/include/csql /usr/include/csql)
                
find_library(CSQL_LIBRARY
             NAMES csqlsql csqlsqlnw
             PATHS /usr/local/lib/csql /lib /usr/lib)
                              
set(CSQL_FOUND FALSE)

if(CSQL_INCLUDE_DIR AND CSQL_LIBRARY)
  set(CSQL_FOUND TRUE)
  message(STATUS "-- Found CSQL.")
else()
  if(CSQL_FIND_REQUIRED)
    message(FATAL_ERROR "-- Error: CSQL required but NOT found.")
  else()
    message(STATUS "-- Could not find CSQL")
  endif()
endif()

mark_as_advanced(CSQL_INCLUDE_DIR CSQL_LIBRARY)
