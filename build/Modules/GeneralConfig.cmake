if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  set( _arc "32" )
else()
  set( _arc "64" )
endif()

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")     # the terralib root directory (change it if it is in another directory)

set ( _binOutdir "${CMAKE_BINARY_DIR}/bin${_arc}" )
set ( _libOutdir "${CMAKE_BINARY_DIR}/lib${_arc}" )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${_binOutdir}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${_binOutdir}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${_libOutdir}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${_libOutdir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${_binOutdir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${_binOutdir}")  

if(NOT "${TE_DEPENDENCIES_DIR}" STREQUAL "")
  set (CMAKE_FIND_ROOT_PATH ${TE_DEPENDENCIES_DIR})
endif()
