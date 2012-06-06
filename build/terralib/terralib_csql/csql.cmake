# Rules for processing common module files.
# -------------------------------------------------- 
# -------------------------------------------------- 

# 3rd-party definitions.
# -------------------------------------------------- 
find_package(Boost 1.47 REQUIRED)
set(DEP_INCLUDES ${Boost_INCLUDE_DIRS})

find_package(CSQL)
if(CSQL_FOUND)
	list (APPEND DEP_LIBS ${CSQL_LIBRARY})
	list (APPEND DEP_INCLUDES ${CSQL_INCLUDE_DIR})
endif(CSQL_FOUND)

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTECSQLDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND DEP_LIBS 
    terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_geometry
		terralib_plugin
		terralib_raster
		terralib_srs
    )

#exporting module information
exportModuleInformation("csql" "${SRCDIR}" "csql")