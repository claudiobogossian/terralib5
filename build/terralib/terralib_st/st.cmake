
find_package(Boost 1.47 COMPONENTS date_time REQUIRED)
set (DEP_LIBS ${Boost_LIBRARIES})
set (DEP_INCLUDES ${Boost_INCLUDE_DIRS})

# Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESTDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND DEP_LIBS 
    terralib_common
		terralib_datatype
		terralib_geometry
		terralib_raster
    )
    

# Files to process.
# -------------------------------------------------- 
set (
  _DIRS 
  .
  coverage
  interpolator
  movingobject
  observation
  timeseries
)

# Files in build tree
appPrefix (${SRCDIR} "${_DIRS}" ST_INC_DIRS)

# Files in build tree
appPrefix ("st" "${_DIRS}" ST_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

#exporting module information
exportModuleInformation("st" "${ST_INC_DIRS}" "${ST_INC_INST_DIRS}")
