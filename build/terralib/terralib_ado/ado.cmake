find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_LIBS ${Boost_LIBRARIES})
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(ADO)
if(ADO_FOUND)
	list (APPEND TE_DEP_INCLUDES ${ADO_INCLUDE_DIR})
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEADODLL -DBOOST_ALL_NO_LIB)
  remove_definitions( /MP )
endif(WIN32)

list (APPEND TE_DEP_LIBS terralib_common terralib_dataaccess terralib_datatype terralib_plugin terralib_geometry terralib_memory)

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

#exporting module information
exportModuleInformation("ado" "${SRCDIR}" "ado")

#Exporting plug-in information for the configuration file
set (TE_PLUGIN_NAME "te.da.ado")
set (TE_PLUGIN_DISPLAYNAME "ADO Data Source Driver")
set (TE_PLUGIN_DESCRIPTION "This plugin enables TerraLib to access data via ADO")
set (TE_PLUGIN_LIBRARY "terralib_ado")
set (TE_PLUGIN_HREFDOC "http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:designimplementation:dataaccess:ado")
set (TE_PLUGIN_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugin_ado_info.xml")
set (TE_PLUGIN_REQUIREMENTS "")

set (RSC_LINK "")
getPluginResources("SharedLibraryName" "${TE_PLUGIN_LIBRARY}" RSC_LINK)
set (TE_PLUGIN_RESOURCES 
  "\n  <Resources>${RSC_LINK}\n  </Resources>\n"
)

set (TE_PLG_INFO "")
getPluginInfo("${TE_PLUGIN_NAME}" "${TE_PLUGIN_FILE}" TE_PLG_INFO)
set (TE_PLUGINS_DESCRIPTION "${TE_PLUGINS_DESCRIPTION}${TE_PLG_INFO}" PARENT_SCOPE)
  
configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${TE_PLUGIN_FILE})
