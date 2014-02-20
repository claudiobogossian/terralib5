
find_package(Boost ${_Boost_VERSION} COMPONENTS date_time system filesystem REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_LIBS ${Boost_LIBRARIES})
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

# Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESERIALIZATIONDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND TE_DEP_LIBS 
		terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_filter
		terralib_geometry
		terralib_maptools
		terralib_raster
		terralib_symbology
		terralib_xlink
		terralib_xml
    )

# Files to process.
set (
  _DIRS 
  .
  dataaccess
  datatype
  fe
  geometry
  maptools
  qt
  qt/widgets
  raster
  se
  xlink
)

# Files in build tree
appPrefix (${SRCDIR} "${_DIRS}" SER_INC_DIRS)

# Files in build tree
appPrefix ("serialization" "${_DIRS}" SER_INC_INST_DIRS)

# Get files structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

#exporting module information
exportModuleInformation("serialization" "${SER_INC_DIRS}" "${SER_INC_INST_DIRS}")  
