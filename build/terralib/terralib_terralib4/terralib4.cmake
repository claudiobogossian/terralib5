find_package(Boost ${_Boost_VERSION} COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_LIBS ${Boost_LIBRARIES})
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

find_package(Terralib4 REQUIRED)
if(TERRALIB4_FOUND)
  set (TE_DEP_LIBS ${TE_DEP_LIBS} ${TERRALIB4_LIBRARY})
  set (TE_DEP_INCLUDES ${TE_DEP_INCLUDES} ${TERRALIB4_INCLUDE_DIR})
endif()

if(WIN32)
  find_package(ADOTL4 REQUIRED)
  if(ADOTL4_FOUND)
    set (TE_DEP_INCLUDES ${TE_DEP_INCLUDES} ${ADOTL4_INCLUDE_DIR})
  endif()
endif()

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTETERRALIB4DLL -DBOOST_ALL_NO_LIB)
	remove_definitions ( /MP )
endif(WIN32)

list(APPEND TE_DEP_LIBS  terralib_dataaccess
                         terralib_geometry
                         terralib_raster
                         terralib_datatype
						 terralib_memory
                         terralib_srs
                         terralib_plugin
                         terralib_common)
						 
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

exportModuleInformation("terralib4" "${SRCDIR}" "terralib4")

set (TE_PLUGIN_NAME "te.da.terralib4")
set (TE_PLUGIN_DISPLAYNAME "TerraLib 4.X Data Access Driver")
set (TE_PLUGIN_DESCRIPTION "This plugin enables TerraLib 5 applications to access data stored in TerraLib 4.X format")
set (TE_PLUGIN_LIBRARY "terralib_terralib4")
set (TE_PLUGIN_HREFDOC "http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:designimplementation:dataaccess:terralib4")
set (TE_PLUGIN_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TE_PLUGIN_NAME}.teplg")
set (TE_PLUGIN_REQUIREMENTS "")

set(RSC_LINK "")
getPluginResources("SharedLibraryName" "${TE_PLUGIN_LIBRARY}" RSC_LINK)
set(TE_PLUGIN_RESOURCES "\n  <Resources>${RSC_LINK}\n  </Resources>\n")

set(TE_PLG_INFO "")
getPluginInfo("${TE_PLUGIN_NAME}" "${TE_PLUGIN_FILE}" TE_PLG_INFO)
set(TE_PLUGINS_DESCRIPTION "${TE_PLUGINS_DESCRIPTION}${TE_PLG_INFO}" PARENT_SCOPE)

configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${TE_PLUGIN_FILE})

set(TE_SCHEMA_LOCATION "../schemas/terralib")
set(TE_PLUGIN_REQUIREMENTS "")

set(TE_PLG_INFO "")
getPluginInfo("${TE_PLUGIN_NAME}" "./plugin_terralib4_info.xml" TE_PLG_INFO)
set(TE_INST_PLUGINS_DESCRIPTION "${TE_INST_PLUGINS_DESCRIPTION}${TE_PLG_INFO}" PARENT_SCOPE)

configure_file(${CMAKE_SOURCE_DIR}/default_plugin_info.xml.in ${CMAKE_BINARY_DIR}/toInstall/${TE_PLUGIN_NAME}.teplg)

set(TE_ENABLED_PLUGINS "${TE_ENABLED_PLUGINS}\n    <Plugin>${TE_PLUGIN_NAME}</Plugin>" PARENT_SCOPE)
