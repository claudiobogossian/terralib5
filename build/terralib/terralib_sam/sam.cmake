
if(MSVC)
	string(REPLACE "INCREMENTAL:YES" "INCREMENTAL:NO" replacementFlags
	${CMAKE_EXE_LINKER_FLAGS_DEBUG})
	set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/INCREMENTAL:NO ${replacementFlags}")
endif(MSVC)

find_package(Boost 1.47 REQUIRED)
set (DEP_INCLUDES ${Boost_INCLUDE_DIRS})

set (DEP_LIBS terralib_common)

# Files to process.
# -------------------------------------------------- 
set (
  _DIRS 
  .
  rtree
)

# Files in build tree
appPrefix (${SRCDIR} ${_DIRS} SAM_INC_DIRS)

# Files in build tree
appPrefix ("sam" ${_DIRS} SAM_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" HDRS "")

list (APPEND _TE_INCLUDE_DIRS "${SRCDIR}" "${SRCDIR}/rtree")
list (APPEND _TE_INST_INCLUDE_DIRS "sam" "sam/rtree")

set (_TE_INCLUDE_DIRS ${_TE_INCLUDE_DIRS} PARENT_SCOPE)
set (_TE_INST_INCLUDE_DIRS ${_TE_INST_INCLUDE_DIRS} PARENT_SCOPE)

installFiles (${SRCDIR} "terralib" "HEADERS" "*.h*")
