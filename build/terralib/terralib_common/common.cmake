# Rules for processing common module files.
# -------------------------------------------------- 
# -------------------------------------------------- 

set (TE_DEP_INCLUDES "")   #include paths needed by common module.

# 3rd-party definitions.
# -------------------------------------------------- 
find_package(Log4Cxx REQUIRED)
if(Log4Cxx_FOUND)
  set (TE_DEP_INCLUDES ${LOG4CXX_INCLUDE_DIR})
  set (TE_DEP_LIBS ${LOG4CXX_LIBRARY})
  set (TE_USE_APACHE_LOG4CXX TRUE PARENT_SCOPE)
endif()

find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system locale REQUIRED)
if(Boost_FOUND)
  list (APPEND TE_DEP_LIBS ${Boost_LIBRARIES})
  list (APPEND TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

if(WIN32)
  find_package(GNU)
  if(GNU_FOUND)
    list (APPEND TE_DEP_LIBS ${GNU_LIBRARY})
    list (APPEND TE_DEP_INCLUDES ${GNU_INCLUDE_DIR})
    set (TE_USE_GNU_TEXT_UTILITIES TRUE PARENT_SCOPE)
    set (TE_USE_GNU_ICONV TRUE PARENT_SCOPE)
  endif()
  
  #Add Ws2_32.lib for Windows
  list (APPEND TE_DEP_LIBS Ws2_32.lib)

  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTECOMMONDLL -DBOOST_ALL_NO_LIB)
endif()

if(UNIX)
  find_package(DL)
  if(DL_FOUND)
    list (APPEND TE_DEP_LIBS ${DL_LIBRARY})
    list (APPEND TE_DEP_INCLUDES ${DL_INCLUDE_DIR})
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
appPrefix (${SRCDIR} "${_DIRS}" COMMON_INC_DIRS)

# Files in build tree
appPrefix ("common" "${_DIRS}" COMMON_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

list (APPEND TE_DEP_INCLUDES "${COMMON_INC_DIRS}")

#exporting module information
exportModuleInformation("common" "${COMMON_INC_DIRS}" "${COMMON_INC_INST_DIRS}")