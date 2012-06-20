
find_package(Boost 1.46 REQUIRED)
set (DEP_INCLUDES ${Boost_INCLUDE_DIRS})

find_package(Proj4)
if(PROJ4_FOUND)
	set(DEP_LIBS ${PROJ4_LIBRARY})
	list (APPEND DEP_INCLUDES ${PROJ4_INCLUDE_DIR})
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESRSDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND DEP_LIBS terralib_common)

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

#exporting module information
exportModuleInformation("srs" "${SRCDIR}" "srs")