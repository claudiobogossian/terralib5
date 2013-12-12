
find_package(Boost ${_Boost_VERSION} COMPONENTS thread date_time system REQUIRED)
if(Boost_FOUND)
  list (APPEND TE_DEP_LIBS ${Boost_LIBRARIES})
  list (APPEND TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(Geos ${_Geos_VERSION})
if(GEOS_FOUND)
	list (APPEND TE_DEP_INCLUDES ${GEOS_INCLUDE_DIR})
  list (APPEND TE_DEP_LIBS ${GEOS_LIBRARY})
  set (TE_USE_GEOS TRUE PARENT_SCOPE)
ENDIF(GEOS_FOUND)

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DTEGEOMDLL -DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)
endif()

list (APPEND TE_DEP_LIBS  terralib_common
                          terralib_datatype
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
appPrefix (${SRCDIR} "${_DIRS}" GM_INC_DIRS)

# Files in build tree
appPrefix ("common" "${_DIRS}" GM_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

#exporting module information
exportModuleInformation("geometry" "${GM_INC_DIRS}" "${GM_INC_INST_DIRS}")