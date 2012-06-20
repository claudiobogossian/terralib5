
find_package(Boost 1.46 REQUIRED)
set(DEP_INCLUDES ${Boost_INCLUDE_DIRS})

find_package(Geos)
if(GEOS_FOUND)
	list (APPEND DEP_INCLUDES ${GEOS_INCLUDE_DIR})
	set(DEP_LIBS ${GEOS_LIBRARY})
ENDIF(GEOS_FOUND)

#Definitions for windows compiling
add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEGEOMDLL -DBOOST_ALL_NO_LIB)

list (APPEND DEP_LIBS  
    terralib_common
		terralib_datatype
		terralib_srs
    )

file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)
 
 #exporting module information
exportModuleInformation("geometry" "${SRCDIR}" "geometry")