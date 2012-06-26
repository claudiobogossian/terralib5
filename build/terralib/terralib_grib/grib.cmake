
find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  set (DEP_LIBS ${DEP_LIBS} ${Boost_LIBRARIES})
  set (DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
endif()

find_package(Grib)
if(GRIB_FOUND)
  list (APPEND DEP_LIBS ${GRIB_LIBRARY})
  list (APPEND DEP_INCLUDES ${GRIB_INCLUDE_DIR})
endif()

list (APPEND DEP_LIBS 
    terralib_common
    terralib_dataaccess
    terralib_datatype
    terralib_geometry
    terralib_plugin
    terralib_raster
    terralib_srs
    )

# Select the source files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

#exporting module information
exportModuleInformation("grib" "${SRCDIR}" "grib")