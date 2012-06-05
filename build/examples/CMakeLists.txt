cmake_minimum_required(VERSION 2.8)

project(all)

############### Get the data used for testing the example ###############

set(BUILD_DIR_DATA_EXISTS FALSE)
if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/data)
  set(BUILD_DIR_DATA_EXISTS TRUE)
endif(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/data)

if(NOT BUILD_DIR_DATA_EXISTS)
  file(DOWNLOAD "http://www.dpi.inpe.br/terralib5/data.tar.gz"  ${CMAKE_CURRENT_BINARY_DIR}/data.tar.gz)
                  
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf data.tar.gz
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif(NOT BUILD_DIR_DATA_EXISTS)

set(TE_DATA_EXAMPLE_LOCALE "${CMAKE_CURRENT_BINARY_DIR}/data")
ADD_DEFINITIONS( -DTE_DATA_EXAMPLE_LOCALE="${TE_DATA_EXAMPLE_LOCALE}/" )

# Go into sub-directories

add_subdirectory(terralib_example_colorbar)
add_subdirectory(terralib_example_cppplugin_helloworld)
add_subdirectory(terralib_example_dataaccess)
add_subdirectory(terralib_example_datagrid)
add_subdirectory(terralib_example_datasetmanagement)
add_subdirectory(terralib_example_factory)
add_subdirectory(terralib_example_geometry)
add_subdirectory(terralib_example_layerexplorer)
add_subdirectory(terralib_example_maptools)
add_subdirectory(terralib_example_myviewer)
add_subdirectory(terralib_example_progress)
add_subdirectory(terralib_example_propertymanagement)
add_subdirectory(terralib_example_qtcanvas)
add_subdirectory(terralib_example_raster)
add_subdirectory(terralib_example_rp)
add_subdirectory(terralib_example_sam)
add_subdirectory(terralib_example_srs)
add_subdirectory(terralib_example_st)
add_subdirectory(terralib_example_timeseries)
add_subdirectory(terralib_example_URL)
add_subdirectory(terralib_example_xerces)
add_subdirectory(terralib_example_help)
add_subdirectory(terralib_example_qt/rp)
add_subdirectory(terralib_example_qt/se)
add_subdirectory(terralib_example_qt/utils)
