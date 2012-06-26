
find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system thread REQUIRED)
if(Boost_FOUND)
  set (DEP_LIBS ${Boost_LIBRARIES})
  set (DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTERPDLL -DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)
endif()

list (APPEND DEP_LIBS
    terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_raster
    )
	
# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)
 
#exporting module information
exportModuleInformation("rp" "${SRCDIR}" "rp")