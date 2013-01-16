
find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  set (DEP_LIBS ${Boost_LIBRARIES})
  set (DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(PostgreSQL ${_PostgrerSQL_VERSION} REQUIRED)
if(PostgreSQL_FOUND)
  list (APPEND DEP_LIBS ${PostgreSQL_LIBRARY})
  list (APPEND DEP_INCLUDES ${PostgreSQL_INCLUDE_DIR})
  set (TE_USE_PGIS TRUE PARENT_SCOPE)
endif()

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEPGISDLL -DBOOST_ALL_NO_LIB)
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
exportModuleInformation("postgis" "${SRCDIR}" "postgis")

set (TEPLUGIN_NAME "te.da.pgis")
set (TEPLUGIN_DISPLAYNAME "PostGIS Data Source Driver")
set (TEPLUGIN_DESCRIPTION "This plugin enables TerraLib to access data via PostGIS")
set (TEPLUGIN_LIBRARY "terralib_postgis")
set (TEPLUGIN_HREFDOC "http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:designimplementation:dataaccess:pgis")

configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugin_pgis_info.xml)
