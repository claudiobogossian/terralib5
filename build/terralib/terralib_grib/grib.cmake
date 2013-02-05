
find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_LIBS ${DEP_LIBS} ${Boost_LIBRARIES})
  set (TE_DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
endif()

find_package(Grib REQUIRED)
if(GRIB_FOUND)
  list (APPEND TE_DEP_LIBS ${GRIB_LIBRARY})
  list (APPEND TE_DEP_INCLUDES ${GRIB_INCLUDE_DIR})
endif()

list (APPEND TE_DEP_LIBS 
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