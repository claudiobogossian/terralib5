# Rules for processing common module files.
# -------------------------------------------------- 
# -------------------------------------------------- 

set (DEP_INCLUDES "")   #include paths needed by common module.

# 3rd-party definitions.
# -------------------------------------------------- 
find_package(Boost ${_Boost_VERSION} REQUIRED)
if(Boost_FOUND)
  set(DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

# Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEXMLDLL -DBOOST_ALL_DYN_LINK -DBOOST_ALL_NO_LIB)
endif(WIN32)

set(DEP_LIBS terralib_common)

# select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

#exporting module information
exportModuleInformation("xml" "${SRCDIR}" "xml")