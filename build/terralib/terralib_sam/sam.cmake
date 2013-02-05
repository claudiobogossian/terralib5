
if(MSVC)
	string(REPLACE "INCREMENTAL:YES" "INCREMENTAL:NO" replacementFlags
	${CMAKE_EXE_LINKER_FLAGS_DEBUG})
	set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/INCREMENTAL:NO ${replacementFlags}")
endif(MSVC)

find_package(Boost ${_Boost_VERSION} REQUIRED)
if(Boost_FOUND)
  set (TE_DEP_INCLUDES ${Boost_INCLUDE_DIRS})
endif()

set (TE_DEP_LIBS terralib_common)

# Files to process.
# -------------------------------------------------- 
set (
  _DIRS 
  .
  rtree
)

# Files in build tree
appPrefix (${SRCDIR} "${_DIRS}" SAM_INC_DIRS)

# Files in build tree
appPrefix ("sam" "${_DIRS}" SAM_INC_INST_DIRS)

# Get files by structured by folders. 
getFfiles(${SRCDIR} "${_DIRS}" HDRS "")

set (TE_INCLUDE_DIRS "${TE_INCLUDE_DIRS}" "${SRCDIR}" "${SRCDIR}/rtree" PARENT_SCOPE)
set (TE_INST_INCLUDE_DIRS "${TE_INST_INCLUDE_DIRS}" "sam" "sam/rtree" PARENT_SCOPE)
set (TE_MODULES "${TE_MODULES}" "sam" PARENT_SCOPE)

installFiles (${SRCDIR} "terralib" "HEADERS" "*.h*")
