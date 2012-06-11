
find_package(Boost 1.47 COMPONENTS filesystem system REQUIRED)
set (DEP_LIBS ${Boost_LIBRARIES})
set (DEP_INCLUDES ${Boost_INCLUDE_DIRS})

find_package(GDAL)
if(GDAL_FOUND)
	list (APPEND DEP_LIBS ${GDAL_LIBRARY})
	list (APPEND DEP_INCLUDES ${GDAL_INCLUDE_DIR})
endif()

# Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEMAPDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND DEP_LIBS 
    terralib_annotationtext
		terralib_color
		terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_filter
		terralib_geometry
		terralib_gml
		terralib_symbology
		terralib_xlink
    )

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)	
file(GLOB HDRS ${SRCDIR}/*.h)
 
#exporting module information
exportModuleInformation("maptools" "${SRCDIR}" "maptools")