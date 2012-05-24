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

set(TE_DATA_UNITTEST_LOCALE "${CMAKE_CURRENT_BINARY_DIR}/data")
ADD_DEFINITIONS( -DTE_DATA_UNITTEST_LOCALE="${TE_DATA_UNITTEST_LOCALE}/" )

# Go into sub-directories

add_subdirectory(terralib_unittest_common)
add_subdirectory(terralib_unittest_dataaccess)
add_subdirectory(terralib_unittest_datatype)
add_subdirectory(terralib_unittest_geometry)
add_subdirectory(terralib_unittest_qtwidgets)
add_subdirectory(terralib_unittest_rp)
add_subdirectory(terralib_unittest_sam)
add_subdirectory(terralib_unittest_srs)
