set (SE_DIR "${ROOT}/src/terralib/se")
makeProject("symbology" "${SE_DIR}" "${SE_DIR}")

set_target_properties(terralib_symbology PROPERTIES OUTPUT_NAME "terralib_se")
