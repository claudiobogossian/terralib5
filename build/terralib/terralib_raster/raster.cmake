
find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  set(DEP_LIBS ${Boost_LIBRARIES})
  set(DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(Proj4 ${_Proj4_VERSION})
if(PROJ4_FOUND)	
	list (APPEND DEP_INCLUDES ${PROJ4_INCLUDE_DIR})
endif()

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTERSTDLL -DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)
endif()

list (APPEND DEP_LIBS 
    terralib_common
		terralib_datatype
		terralib_geometry
		terralib_srs
    )

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)
 
#exporting module information
exportModuleInformation("raster" "${SRCDIR}" "raster")