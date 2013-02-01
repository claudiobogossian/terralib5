
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

if(SPATIALITE_FOUND)
	list (APPEND DEP_LIBS ${SPATIALITE_LIBRARY})
	list (APPEND DEP_INCLUDES ${SPATIALITE_INCLUDE_DIR})
	set (TE_USE_SPATIALITE TRUE PARENT_SCOPE)
	if(WIN32)
		add_definitions(-DTE_ENABLE_SPATIALITE)
	endif(WIN32)	
endif()

if(RASTERLITE_FOUND)
	list (APPEND DEP_LIBS ${RASTERLITE_LIBRARY})
	list (APPEND DEP_INCLUDES ${RASTERLITE_INCLUDE_DIR})
	set (TE_USE_RASTERLITE TRUE PARENT_SCOPE)
	if(WIN32)
		add_definitions(-DTE_ENABLE_RASTERLITE)
	endif(WIN32)
endif()

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESQLITEDLL -DBOOST_ALL_NO_LIB)
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

set (TEPLUGIN_NAME "te.da.sqlite")
set (TEPLUGIN_DISPLAYNAME "SQLite Data Source Driver")
set (TEPLUGIN_DESCRIPTION "This plugin enables TerraLib to access data via SQLite")
set (TEPLUGIN_LIBRARY "terralib_sqlite")
set (TEPLUGIN_HREFDOC "http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:designimplementation:dataaccess:sqlite")
set (TEPLUGIN_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugin_sqlite_info.xml")
set (TEPLUGIN_REQUIREMENTS "")

set (RSC_LINK "")
getPluginResources("SharedLibraryName" "${TEPLUGIN_LIBRARY}" RSC_LINK)
set (TEPLUGIN_RESOURCES 
  "\n  <Resources>${RSC_LINK}  </Resources>\n"
)

#Exporting plug-in information for the configuration file
set (PLG_INFO "")
getPluginInfo("${TEPLUGIN_NAME}" "${TEPLUGIN_FILE}" PLG_INFO)
set (TERRALIB_PLUGINS_DESCRIPTION "${TERRALIB_PLUGINS_DESCRIPTION}${PLG_INFO}" PARENT_SCOPE)

configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${TEPLUGIN_FILE})
