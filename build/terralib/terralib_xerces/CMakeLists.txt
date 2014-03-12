set (XERCES_DIR "${ROOT}/src/terralib/xerces")
makeProject("xerces" "${XERCES_DIR}" "${XERCES_DIR}")

if("${ENABLED_PLUGINS}" STREQUAL "")
	set (ENABLED_PLUGINS terralib_xerces PARENT_SCOPE)
else()
	set (ENABLED_PLUGINS "${ENABLED_PLUGINS}" terralib_xerces PARENT_SCOPE)
endif()
