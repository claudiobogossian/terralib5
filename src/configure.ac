m4_include(../m4/tlglobals.m4)

AC_INIT([terralib_src],TL_VERSION_MACRO)
AC_CONFIG_AUX_DIR([.])
AM_PROG_CC_C_O
AM_INIT_AUTOMAKE([subdir-objects])
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_LANG(C++)
AC_CONFIG_MACRO_DIR([m4])

# ------------------------------------------------------------
# Getting used defined options

m4_include(../m4/tloptions.m4)
m4_include(m4/tldefines.m4)

# ------------------------------------------------------------
# Generating config file

AC_CONFIG_HEADERS([config.h:config.h.in],[])

m4_include([m4/ax_prefix_config_h.m4])
AX_PREFIX_CONFIG_H($srcdir/terralib/linuxplatformdefines.h,TE,config.h)

# ------------------------------------------------------------
# Optional modules

AM_CONDITIONAL([TE_BUILD_OGR_MODULE],[test "x$tlpkgs_ogr_status" == xyes])

AM_CONDITIONAL([TE_BUILD_GDAL_MODULE],[test "x$tlpkgs_gdal_status" == xyes])

AM_CONDITIONAL([TE_BUILD_POSTGIS_MODULE],[test "x$tlpkgs_postgis_status" == xyes])

AM_CONDITIONAL([TE_BUILD_QT_MODULE],[test "x$tlpkgs_qt_status" == xyes])

AM_CONDITIONAL([TE_BUILD_GRIB_MODULE],[test "x$tlpkgs_grib_status" == xyes])

# ------------------------------------------------------------
# Generate outputs

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
