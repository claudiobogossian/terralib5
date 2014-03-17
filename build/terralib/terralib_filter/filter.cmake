find_package(Boost ${_Boost_VERSION} REQUIRED)
if(Boost_FOUND)
  list( APPEND TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEFEDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

set(TE_DEP_LIBS 
    terralib_common
    terralib_dataaccess
    terralib_datatype
    terralib_geometry
    terralib_gml
    terralib_srs
    )

# Files to process.
# -------------------------------------------------- 
set (
  _DIRS 
  .
  serialization
)

# Files in build tree
appPrefix (${SRCDIR} "${_DIRS}" FE_INC_DIRS)

# Files in build tree
appPrefix ("common" "${_DIRS}" FE_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

#exporting module information
exportModuleInformation("filter" "${FE_INC_DIRS}" "${FE_INC_INST_DIRS}")
