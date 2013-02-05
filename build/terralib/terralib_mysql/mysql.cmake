
find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_LIBS ${Boost_LIBRARIES})
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(MySQL REQUIRED)
if(MYSQL_FOUND)
	list (APPEND TE_DEP_LIBS ${MYSQL_LIBRARY})
	list (APPEND TE_DEP_INCLUDES ${MYSQL_INCLUDE_DIR})
	set (TE_USE_MYSQL TRUE PARENT_SCOPE)
endif()

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEMYSQLDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

list (APPEND TE_DEP_LIBS  
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
exportModuleInformation("mysql" "${SRCDIR}" "mysql")

set (TE_PLUGIN_NAME "te.da.mysql")
set (TE_PLUGIN_DISPLAYNAME "MySQL Data Source Driver")
set (TE_PLUGIN_DESCRIPTION "This plugin enables TerraLib to access data via MySQL")
set (TE_PLUGIN_LIBRARY "terralib_mysql")
set (TE_PLUGIN_HREFDOC "http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:designimplementation:dataaccess:mysql")
set (TE_PLUGIN_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugin_mysql_info.xml")
set (TE_PLUGIN_REQUIREMENTS "")

set (RSC_LINK "")
getPluginResources("SharedLibraryName" "${TE_PLUGIN_LIBRARY}" RSC_LINK)
set (TE_PLUGIN_RESOURCES 
  "\n  <Resources>${RSC_LINK}\n  </Resources>\n"
)

#Exporting plug-in information for the configuration file
set (TE_PLG_INFO "")
getPluginInfo("${TE_PLUGIN_NAME}" "${TE_PLUGIN_FILE}" TE_PLG_INFO)
set (TE_PLUGINS_DESCRIPTION "${TE_PLUGINS_DESCRIPTION}${TE_PLG_INFO}" PARENT_SCOPE)

configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${TE_PLUGIN_FILE})

configure_file(${CMAKE_SOURCE_DIR}/../resources/sql/mysql-ogc-sfs.sql ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mysql-ogc-sfs.sql COPYONLY)
configure_file(${TE_JSON_FILES_LOCATION}/srs.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/srs.json COPYONLY)
