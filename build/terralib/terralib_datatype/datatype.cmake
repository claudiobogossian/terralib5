# 3rd-party definitions.
# -------------------------------------------------- 
find_package(Boost ${_Boost_VERSION} COMPONENTS date_time system REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_LIBS ${Boost_LIBRARIES})
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEDATATYPEDLL -DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)
endif(WIN32)

list (APPEND TE_DEP_LIBS terralib_common)

# Files to process.
# -------------------------------------------------- 
set (
  _DIRS 
  .
  serialization/xml
)

# Files in build tree
appPrefix (${SRCDIR} "${_DIRS}" DT_INC_DIRS)

# Files in build tree
appPrefix ("common" "${_DIRS}" DT_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

#exporting module information
exportModuleInformation("datatype" "${DT_INC_DIRS}" "${DT_INC_INST_DIRS}")