#Finding dumpbin - to discover libraries dependencies

set (paths_to_search "")

if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  set (path_prefix "$ENV{ProgramFiles(x86)}")
else()
  set (path_prefix "$ENV{ProgramFiles}")
endif()

set (DPS_DIR_COMMENT "Find the directory containing terralib5 3rd-party dependencies.")

if(DEFINED ENV{TERRALIB_DEP_MSVC2010})
  set (TE_DEPENDENCIES_DIR "$ENV{TERRALIB_DEP_MSVC2010}" CACHE PATH ${DPS_DIR_COMMENT})
else()
  set (TE_DEPENDENCIES_DIR "" CACHE PATH ${DPS_DIR_COMMENT})
endif()

if(NOT "${TE_DEPENDENCIES_DIR}" STREQUAL "")
  set (CMAKE_FIND_ROOT_PATH ${TE_DEPENDENCIES_DIR})
else ()
  message (WARNING "If not set TE_DEPENDENCIES_DIR variable, CMake may not find Terralib 5 dependencies.")
endif()

#if(MSVC)
#  set (
#    paths_2_search
#    "${path_prefix}/Microsoft Visual Studio 8/VC/bin"
#    "${path_prefix}/Microsoft Visual Studio 9.0/VC/bin"
#    "${path_prefix}/Microsoft Visual Studio 10.0/VC/bin"
#  )

#  find_path (_DUMPBIN_PATH dumpbin.exe ${paths_2_search} DOC "Dumpbin is used to discover terralib dependencies.")
  
#  if(_DUMPBIN_PATH)
#    message ("-- Dumpbin found at: \"${_DUMPBIN_PATH}\".")
#    list (FIND CMAKE_FIND_ROOT_PATH ${_DUMPBIN_PATH} _onSchPath)
    
#    if(_onSchPath EQUAL -1)
#      list (APPEND CMAKE_FIND_ROOT_PATH ${_DUMPBIN_PATH})
#      message ("-- Dumpbin path added to search path.")
#    endif()
#  else()
#    message ("-- Could not found dumpbin.exe")
#  endif()
#endif()

include (${CMAKE_ROOT}/Modules/GetPrerequisites.cmake)