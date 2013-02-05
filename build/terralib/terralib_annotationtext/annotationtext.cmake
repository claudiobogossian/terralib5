# Rules for processing annotation text module files.
# -------------------------------------------------- 

# 3rd-party definitions.
find_package(Boost ${_Boost_VERSION} REQUIRED)
if(Boost_FOUND)
  set(TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEANNOTATIONTEXTDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

set(TE_DEP_LIBS  
    terralib_common
		terralib_datatype
		terralib_geometry
		terralib_srs
    )

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)
 
#exporting module information
exportModuleInformation("annotationtext" "${SRCDIR}" "annotationtext")