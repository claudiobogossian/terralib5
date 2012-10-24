find_package(IDL)
if(IDL_FOUND)
  list (APPEND DEP_LIBS ${IDL_LIBRARY})
  list (APPEND DEP_INCLUDES ${IDL_INCLUDE_DIR})
endif(IDL_FOUND)

#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEIDLDLL -DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3)
endif()

list (APPEND DEP_LIBS
    terralib_common
    terralib_dataaccess
    terralib_datatype
    terralib_geometry
    terralib_memory
    terralib_ogr
    terralib_plugin
    terralib_raster
    terralib_srs
    terralib_rp
    )

# Files to process.
# -------------------------------------------------- 
set (
  _DIRS 
  .
  rp
)

# Files in build tree
appPrefix (${SRCDIR} "${_DIRS}" IDL_INC_DIRS)

# Files in build tree
appPrefix ("idl" "${_DIRS}" IDL_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

#exporting module information
exportModuleInformation("idl" "${IDL_INC_DIRS}" "${IDL_INC_INST_DIRS}")

# copying the DLM file to binaries path

IF(WIN32)
  configure_file( ${SRCDIR}/terralib_idl.dlm ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}/terralib_idl.dlm COPYONLY )
  configure_file( ${SRCDIR}/terralib_idl.dlm ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}/terralib_idl.dlm COPYONLY )
else()
  configure_file( ${SRCDIR}/terralib_idl.dlm ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}/libterralib_idl.dlm COPYONLY )
  configure_file( ${SRCDIR}/terralib_idl.dlm ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}/libterralib_idl.dlm COPYONLY )
endif()

