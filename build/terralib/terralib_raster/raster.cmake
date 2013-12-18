
find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system thread REQUIRED)
if(Boost_FOUND)
  set(TE_DEP_LIBS ${Boost_LIBRARIES})
  set(TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(Proj4 ${_Proj4_VERSION})
if(PROJ4_FOUND)	
	list (APPEND TE_DEP_INCLUDES ${PROJ4_INCLUDE_DIR})
endif()

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTERSTDLL -DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)
endif()

list (APPEND TE_DEP_LIBS terralib_common
                         terralib_datatype
                         terralib_geometry
                         terralib_srs
                         terralib_xml)

# Files to process.
# -------------------------------------------------- 
set (
  _DIRS 
  .
  serialization/xml
)

# Files in build tree
appPrefix (${SRCDIR} "${_DIRS}" RST_INC_DIRS)

# Files in build tree
appPrefix ("common" "${_DIRS}" RST_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

#exporting module information
exportModuleInformation("raster" "${RST_INC_DIRS}" "${RST_INC_INST_DIRS}")