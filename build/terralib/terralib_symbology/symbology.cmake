
find_package(Boost ${_Boost_VERSION} REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESEDLL -DBOOST_ALL_NO_LIB)
endif()

set (TE_DEP_LIBS 
    terralib_color
		terralib_common
		terralib_raster
		terralib_filter
		terralib_xlink
    )

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)
 
#exporting module information
exportModuleInformation("symbology" "${SRCDIR}" "se")