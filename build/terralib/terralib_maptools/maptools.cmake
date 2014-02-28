
find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_LIBS ${Boost_LIBRARIES})
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(GDAL ${_GDAL_VERSION})
if(GDAL_FOUND)
	list (APPEND TE_DEP_LIBS ${GDAL_LIBRARY})
	list (APPEND TE_DEP_INCLUDES ${GDAL_INCLUDE_DIR})
endif()

# Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEMAPDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND TE_DEP_LIBS 
    terralib_annotationtext
		terralib_color
		terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_filter
		terralib_geometry
		terralib_gml
		terralib_memory
		terralib_raster
		terralib_srs
		terralib_symbology
		terralib_xlink
    )
	
# Files to process.
# -------------------------------------------------- 
set (
  _DIRS 
  .
  serialization
)

# Files in build tree
appPrefix (${SRCDIR} "${_DIRS}" MAPTOOLS_INC_DIRS)

# Files in build tree
appPrefix ("common" "${_DIRS}" MAPTOOLS_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

#exporting module information
exportModuleInformation("maptools" "${MAPTOOLS_INC_DIRS}" "${MAPTOOLS_INC_INST_DIRS}")
