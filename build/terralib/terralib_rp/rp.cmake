
find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem date_time system thread REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_LIBS ${Boost_LIBRARIES})
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DTERPDLL -DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)
endif()

list (APPEND TE_DEP_LIBS
    terralib_classification
    terralib_common
    terralib_dataaccess
    terralib_datatype
    terralib_memory
    terralib_geometry
    terralib_raster
    terralib_srs
    terralib_statistics
    )

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)
 
#exporting module information
exportModuleInformation("rp" "${SRCDIR}" "rp")