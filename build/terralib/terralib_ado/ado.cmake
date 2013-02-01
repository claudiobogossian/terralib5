find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  set (DEP_LIBS ${Boost_LIBRARIES})
  set (DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(ADO)
if(ADO_FOUND)
	list (APPEND DEP_INCLUDES ${ADO_INCLUDE_DIR})
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEADODLL -DBOOST_ALL_NO_LIB)
  remove_definitions( /MP )
endif(WIN32)

list (APPEND DEP_LIBS terralib_common terralib_dataaccess terralib_datatype terralib_plugin terralib_geometry terralib_memory)

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

#exporting module information
exportModuleInformation("ado" "${SRCDIR}" "ado")

set (TEPLUGIN_NAME "te.da.ado")
set (TEPLUGIN_DISPLAYNAME "ADO Data Source Driver")
set (TEPLUGIN_DESCRIPTION "This plugin enables TerraLib to access data via ADO")
set (TEPLUGIN_LIBRARY "terralib_ado")
set (TEPLUGIN_HREFDOC "http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:designimplementation:dataaccess:ado")
set (TEPLUGIN_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugin_ado_info.xml")

#Exporting plug-in information for the configuration file
set (PLG_INFO "")
getPluginInfo("${TEPLUGIN_NAME}" "${TEPLUGIN_FILE}" PLG_INFO)
set (TERRALIB_PLUGINS_DESCRIPTION "${TERRALIB_PLUGINS_DESCRIPTION}${PLG_INFO}" PARENT_SCOPE)
  
configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${TEPLUGIN_FILE})
