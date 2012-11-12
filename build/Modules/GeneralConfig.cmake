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
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${_binOutdir}")  
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${_binOutdir}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${_binOutdir}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${_libOutdir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${_binOutdir}")  

if(UNIX)
# On Linux-based systems, by default uses release configuration. Change CMAKE_BUILD_TYPE value to "Debug" to change to debug configuration.
  if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set (CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug 
    Release RelWithDebInfo MinSizeRel." FORCE)
  endif()
endif()
