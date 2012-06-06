# Rules for processing common module files.
# -------------------------------------------------- 
# -------------------------------------------------- 

# 3rd-party definitions.
# -------------------------------------------------- 
find_package(Boost 1.47 COMPONENTS date_time REQUIRED)
set (DEP_LIBS ${Boost_LIBRARIES})
set (DEP_INCLUDES ${Boost_INCLUDE_DIRS})

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEDATATYPEDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND DEP_LIBS terralib_common)

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)
 
#exporting module information
exportModuleInformation("datatype" "${SRCDIR}" "datatype")