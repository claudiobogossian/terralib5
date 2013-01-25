
find_package(Boost ${_Boost_VERSION} COMPONENTS system filesystem date_time REQUIRED)
if(Boost_FOUND)
  set (DEP_LIBS ${Boost_LIBRARIES})
  set (DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(SQLite REQUIRED)
if(SQLITE_FOUND)
	list (APPEND DEP_LIBS ${SQLITE_LIBRARY})
	list (APPEND DEP_INCLUDES ${SQLITE_INCLUDE_DIR})
	set (TE_USE_SQLITE TRUE PARENT_SCOPE)
endif()

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESQLITEDLL -DTE_ENABLE_SPATIALITE -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND DEP_LIBS  
    terralib_common
		terralib_dataaccess
		terralib_datatype
		terralib_geometry
		terralib_memory
		terralib_plugin
		terralib_raster
		terralib_srs
    )

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

#exporting module information
exportModuleInformation("sqlite" "${SRCDIR}" "sqlite")