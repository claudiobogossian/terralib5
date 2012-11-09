
find_package(Boost ${_Boost_VERSION} COMPONENTS thread REQUIRED)
if(Boost_FOUND)
  list (APPEND DEP_LIBS ${Boost_LIBRARIES})
  list (APPEND DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(Geos ${_Geos_VERSION})
if(GEOS_FOUND)
	list (APPEND DEP_INCLUDES ${GEOS_INCLUDE_DIR})
  list (APPEND DEP_LIBS ${GEOS_LIBRARY})
ENDIF(GEOS_FOUND)

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEGEOMDLL -DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)
endif()

list (APPEND DEP_LIBS  
    terralib_common
		terralib_datatype
		terralib_srs
    )

file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)
 
 #exporting module information
exportModuleInformation("geometry" "${SRCDIR}" "geometry")