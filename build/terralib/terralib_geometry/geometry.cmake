
find_package(Boost ${_Boost_VERSION} REQUIRED)
if(Boost_FOUND)
  set(DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(Geos ${_Geos_VERSION})
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