
find_package(Boost ${_Boost_VERSION} COMPONENTS date_time system REQUIRED)
if(Boost_FOUND)
  set(TE_DEP_LIBS ${Boost_LIBRARIES})
  set(TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

# Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESTDLL -DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)
endif(WIN32)

list (APPEND TE_DEP_LIBS 
		terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_geometry
		terralib_maptools
		terralib_raster
		terralib_stmemory
		terralib_plugin
    )
    

# Files to process.
# -------------------------------------------------- 
set (
  _DIRS 
  .
  core/coverage
  core/event
  core/interpolator
  core/observation
  core/timeseries
  core/trajectory
  loader
  maptools
  serialization
)

# Files in build tree
appPrefix (${SRCDIR} "${_DIRS}" ST_INC_DIRS)

# Files in build tree
appPrefix ("st" "${_DIRS}" ST_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

#exporting module information
exportModuleInformation("st" "${ST_INC_DIRS}" "${ST_INC_INST_DIRS}")

