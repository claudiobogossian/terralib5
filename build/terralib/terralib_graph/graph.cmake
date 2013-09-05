#Definitions for windows compiling
if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEGRAPHDLL)
endif(WIN32)

list (APPEND TE_DEP_LIBS 
    terralib_common
	terralib_dataaccess
	terralib_datatype
	terralib_raster
	terralib_geometry
	terralib_maptools
	terralib_memory
	terralib_plugin
	terralib_symbology
	terralib_srs
    )
	
# Files to process.
# -------------------------------------------------- 
set (
  _DIRS 
  .
  builder
  cache
  core
  drivers
  drivers/database
  functions
  graphs
  iterator
  layer
  loader
)

# Files in build tree
appPrefix (${SRCDIR} "${_DIRS}" GRAPH_INC_DIRS)

# Files in build tree
appPrefix ("graph" "${_DIRS}" GRAPH_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" SRCS "")

#exporting module information
exportModuleInformation("graph" "${GRAPH_INC_DIRS}" "${GRAPH_INC_INST_DIRS}")