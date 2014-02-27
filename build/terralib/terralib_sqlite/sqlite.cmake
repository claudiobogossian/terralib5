
find_package(Boost ${_Boost_VERSION} COMPONENTS system filesystem date_time REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_LIBS ${Boost_LIBRARIES})
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(SQLite REQUIRED)
if(SQLITE_FOUND)
	list (APPEND TE_DEP_LIBS ${SQLITE_LIBRARY})
	list (APPEND TE_DEP_INCLUDES ${SQLITE_INCLUDE_DIR})
	set (TE_USE_SQLITE TRUE PARENT_SCOPE)
endif()

if(SPATIALITE_FOUND)
	list (APPEND TE_DEP_LIBS ${SPATIALITE_LIBRARY})
	list (APPEND TE_DEP_INCLUDES ${SPATIALITE_INCLUDE_DIR})
	set (TE_USE_SPATIALITE TRUE PARENT_SCOPE)
    add_definitions(-DTE_ENABLE_SPATIALITE)
endif()

if(RASTERLITE_FOUND)
	list (APPEND TE_DEP_LIBS ${RASTERLITE_LIBRARY})
	list (APPEND TE_DEP_INCLUDES ${RASTERLITE_INCLUDE_DIR})
	set (TE_USE_RASTERLITE TRUE PARENT_SCOPE)
	add_definitions(-DTE_ENABLE_RASTERLITE)
endif()

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESQLITEDLL -DBOOST_ALL_NO_LIB)
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
exportModuleInformation("sqlite" "${SRCDIR}" "sqlite")

set (TE_PLUGIN_NAME "te.da.sqlite")
set (TE_PLUGIN_DISPLAYNAME "SQLite Data Source Driver")
set (TE_PLUGIN_DESCRIPTION "This plugin enables TerraLib to access data via SQLite")
set (TE_PLUGIN_LIBRARY "terralib_sqlite")
set (TE_PLUGIN_HREFDOC "http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:designimplementation:dataaccess:sqlite")
set (TE_PLUGIN_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TE_PLUGIN_NAME}.teplg")
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
configure_file(${CMAKE_SOURCE_DIR}/../resources/xml/spatialite-capabilities.xml.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spatialite-capabilities.xml)

#Exporting plugin information for the configuration file in install tree
set (TE_SCHEMA_LOCATION "../schemas/terralib")
set (TE_PLUGIN_REQUIREMENTS "")

set (TE_PLG_INFO "")
getPluginInfo("${TE_PLUGIN_NAME}" "./plugin_sqlite_info.xml" TE_PLG_INFO)
set (TE_INST_PLUGINS_DESCRIPTION "${TE_INST_PLUGINS_DESCRIPTION}${TE_PLG_INFO}" PARENT_SCOPE)
  
configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${CMAKE_BINARY_DIR}/toInstall/${TE_PLUGIN_NAME}.teplg)

set (TE_ENABLED_PLUGINS "${TE_ENABLED_PLUGINS}\n    <Plugin>${TE_PLUGIN_NAME}</Plugin>" PARENT_SCOPE)
