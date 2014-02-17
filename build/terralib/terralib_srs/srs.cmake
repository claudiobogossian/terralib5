
find_package(Boost ${_Boost_VERSION} REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(Proj4 ${_Proj4_VERSION})
if(PROJ4_FOUND)
	set(TE_DEP_LIBS ${PROJ4_LIBRARY})
	list (APPEND TE_DEP_INCLUDES ${PROJ4_INCLUDE_DIR})
  set (TE_USE_PROJ4 TRUE PARENT_SCOPE)
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESRSDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND TE_DEP_LIBS terralib_common)

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

list(APPEND TE_DEP_INCLUDES "${CMAKE_CURRENT_BINARY_DIR}")

#exporting module information
exportModuleInformation("srs" "${SRCDIR}" "srs")