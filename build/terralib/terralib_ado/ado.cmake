
find_package(Boost 1.47 REQUIRED)
set (DEP_INCLUDES ${Boost_INCLUDE_DIRS})

find_package(ADO)
if(ADO_FOUND)
	list (APPEND DEP_INCLUDES ${ADO_INCLUDE_DIR})
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEADODLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND DEP_LIBS terralib_ado)

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

#exporting module information
exportModuleInformation("ado" "${SRCDIR}" "ado")