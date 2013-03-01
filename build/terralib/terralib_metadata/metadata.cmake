# Rules for processing metadata module files.
# -------------------------------------------------- 

set (TE_DEP_INCLUDES "")   #include paths needed by metadata module.

# 3rd-party definitions.

find_package(Boost ${_Boost_VERSION} COMPONENTS date_time REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_LIBS ${Boost_LIBRARIES})
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEMDDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND TE_DEP_LIBS 
  terralib_common 
  terralib_dataaccess
  terralib_datatype 
  terralib_geometry
  terralib_xml
  )

set (
  _DIRS 
  .
  iso19115
  serialization
)

# Files in build tree
appPrefix (${SRCDIR} "${_DIRS}" METADATA_INC_DIRS)

# Files in build tree
appPrefix ("metadata" "${_DIRS}" METADATA_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

list (APPEND TE_DEP_INCLUDES "${METADATA_INC_DIRS}")

#exporting module information
exportModuleInformation("metadata"" ${METADATA_INC_DIRS}" "${METADATA_INC_INST_DIRS}")