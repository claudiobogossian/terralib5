set (SAM_DIR "${ROOT}/src/terralib/sam")

configureProject("terralib_sam" "${SAM_DIR}" "${SAM_DIR}")

include (sam.cmake)

add_library(terralib_sam MODULE EXCLUDE_FROM_ALL ${HDRS} ${RTREE_HDRS})

set_target_properties(terralib_sam PROPERTIES LINK_INTERFACE_LIBRARIES "" DEBUG_POSTFIX _d LINKER_LANGUAGE C)
