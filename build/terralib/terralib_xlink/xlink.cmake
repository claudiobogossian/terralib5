
find_package(Boost ${_Boost_VERSION} REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEXLDLL -DBOOST_ALL_NO_LIB)
endif()

set (TE_DEP_LIBS terralib_common)

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

#exporting module information
exportModuleInformation("xlink" "${SRCDIR}" "xlink")