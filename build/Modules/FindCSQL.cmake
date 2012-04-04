# - Find CSQL
# This module finds an installed CSQL package.
#
# It sets the following variables:
# CSQL_FOUND           - Set to false, or undefined, if CSQL isn't found.
# CSQL_INCLUDE_DIR     - The CSQL include directory.
# CSQL_LIBRARY         - The CSQL library.


find_path(CSQL_INCLUDE_DIR sql.h
          PATHS "$ENV{TERRALIB_DEP_MSVC2010}/csql"
                /usr/local/include/csql /usr/include/csql)
                
find_library(CSQL_LIBRARY
             NAMES csqlsql csqlsqlnw
             PATHS "$ENV{TERRALIB_DEP_MSVC2010}/csql"
             /usr/local/lib/csql /lib /usr/lib)
                              
if(CSQL_INCLUDE_DIR AND CSQL_LIBRARY)
   set(CSQL_FOUND TRUE)
endif(CSQL_INCLUDE_DIR AND CSQL_LIBRARY)

if(CSQL_FOUND)
    message(STATUS "Found CSQL")
else(CSQL_FOUND)
    message(FATAL_ERROR "Could not find CSQL")
endif(CSQL_FOUND)

mark_as_advanced(CSQL_INCLUDE_DIR CSQL_LIBRARY)
