# Configurations for packaging using DEB
INCLUDE (InstallRequiredSystemLibraries)

set (p_deps "libstdc++ | libstdc++6 (>=4.5.2), gcc (>=4.5.2)")

GET_PROPERTY(
  deps GLOBAL
  PROPERTY ENABLED_FEATURES
)

foreach (dep ${deps})
  getLibrariesNames( ${dep} _libs )
  set (p_deps "${p_deps}, ${_libs}")
endforeach()

#set (CPACK_DEBIAN_PACKAGE_NAME ${pname})
set (CPACK_DEBIAN_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})
# CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
set (CPACK_DEBIAN_PACKAGE_DEPENDS "${p_deps}")
set (CPACK_DEBIAN_PACKAGE_MAINTAINER "Frederico A. Bede <frederico.bede@funcate.org.br>")
set (CPACK_DEBIAN_PACKAGE_DESCRIPTION "Terralib 5 package for Linux users.")
# CPACK_DEBIAN_PACKAGE_SECTION
# CPACK_DEBIAN_PACKAGE_PRIORITY
set (CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://www.dpi.inpe.br/terralib5/wiki")
set (CPACK_STRIP_FILES "bin")
set (CPACK_SOURCE_STRIP_FILES "")
# CPACK_DEBIAN_PACKAGE_DEBUG
# CPACK_DEBIAN_PACKAGE_PREDEPENDS
# CPACK_DEBIAN_PACKAGE_ENHANCES
# CPACK_DEBIAN_PACKAGE_BREAKS
# CPACK_DEBIAN_PACKAGE_CONFLICTS
# CPACK_DEBIAN_PACKAGE_PROVIDES
# CPACK_DEBIAN_PACKAGE_REPLACES

#message ("pack: ${CPACK_PACKAGE_FILE_NAME}")