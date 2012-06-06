# Rules for processing common module files.
# -------------------------------------------------- 
# -------------------------------------------------- 

set (DEP_INCLUDES "")   #include paths needed by common module.

# 3rd-party definitions.
# -------------------------------------------------- 
find_package(Log4Cxx)
if(Log4cxx_FOUND)
  set(DEP_INCLUDES ${LOG4CXX_INCLUDE_DIR})
  set(DEP_LIBS ${DEP_LIBS} ${LOG4CXX_LIBRARY})	
endif()

find_package(Boost 1.47 COMPONENTS filesystem system REQUIRED)
list (APPEND DEP_LIBS ${Boost_LIBRARIES})
list (APPEND DEP_INCLUDES ${Boost_INCLUDE_DIRS})

if(WIN32)
  find_package(GNU)
  if(GNU_FOUND)
	  list (APPEND DEP_LIBS ${GNU_LIBRARY})
	  list (APPEND DEP_INCLUDES ${GNU_INCLUDE_DIR})
  endif()
  
  #Add Ws2_32.lib for Windows
  list (APPEND DEP_LIBS Ws2_32.lib)

  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTECOMMONDLL -DBOOST_ALL_NO_LIB)
endif()

if(UNIX)
  find_package(DL)
  if(DL_FOUND)
	  list (APPEND DEP_LIBS ${DL_LIBRARY})
	  list (APPEND DEP_INCLUDES ${DL_INCLUDE_DIR})
  endif(DL_FOUND)
endif(UNIX)

# Files to process.
# -------------------------------------------------- 
set (
  _DIRS 
  .
  URI
  progress
)

# Files in build tree
appPrefix (${SRCDIR} ${_DIRS} COMMON_INC_DIRS)

# Files in build tree
appPrefix ("common" ${_DIRS} COMMON_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

#exporting module information
exportModuleInformation("common" "${COMMON_INC_DIRS}" "${COMMON_INC_INST_DIRS}")