
find_package(Boost 1.47 COMPONENTS filesystem system REQUIRED)
set (DEP_LIBS ${Boost_LIBRARIES})
set (DEP_INCLUDES ${Boost_INCLUDE_DIRS})

find_package(PostgreSQL)
if(PostgreSQL_FOUND)
	list (APPEND DEP_LIBS ${PostgreSQL_LIBRARY})
	list (APPEND DEP_INCLUDES ${PostgreSQL_INCLUDE_DIR})
endif()

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEPGISDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND DEP_LIBS  
    terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_geometry
		terralib_memory
		terralib_plugin
		terralib_srs
    )
    
# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)
	
list(REMOVE_ITEM SRCS ${SRCDIR}/Module.cpp)
 
#exporting module information
exportModuleInformation("postgis" "${SRCDIR}" "postgis")