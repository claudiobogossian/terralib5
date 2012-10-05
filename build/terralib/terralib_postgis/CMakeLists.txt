set (PGIS_DIR "${ROOT}/src/terralib/postgis")
makeProject("postgis" "${PGIS_DIR}" "${PGIS_DIR}")

set (PGIS_ENABLED TRUE PARENT_SCOPE)