
find_package(Boost 1.46 REQUIRED)
set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEGMLDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

set(DEP_LIBS 
    terralib_common
		terralib_geometry
		terralib_srs
    )

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

#exporting module information
exportModuleInformation("gml" "${SRCDIR}" "gml")