
find_package(Boost ${_Boost_VERSION} REQUIRED)
if(Boost_FOUND)
  set (DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(Proj4 ${_Proj4_VERSION})
if(PROJ4_FOUND)
	set(DEP_LIBS ${PROJ4_LIBRARY})
	list (APPEND DEP_INCLUDES ${PROJ4_INCLUDE_DIR})
  set (TE_USE_PROJ4 TRUE PARENT_SCOPE)
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESRSDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

add_definitions (-DTERRALIB_JSON_FILES_LOCATION=\"${TERRALIB_JSON_FILES_LOCATION}\")

list (APPEND DEP_LIBS terralib_common)

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

#exporting module information
exportModuleInformation("srs" "${SRCDIR}" "srs")