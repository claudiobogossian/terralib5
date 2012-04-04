m4_include(../m4/tlglobals.m4)

AC_INIT([terralib_examples],TL_VERSION_MACRO)
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

# ------------------------------------------------------------
# Global examples settings

AC_DEFINE_UNQUOTED(
  [TE_DATA_EXAMPLE_LOCALE],
  ["../../"],
  [This flag tells where is the base folder for the data used in the examples]
)

# ------------------------------------------------------------
# Generating config file

AH_TOP([#ifndef __TERRALIB_INTERNAL_LINUXEXAMPLESPLATFORMDEFINES_H 
#define __TERRALIB_INTERNAL_LINUXEXAMPLESPLATFORMDEFINES_H])

AH_BOTTOM([#endif /*__TERRALIB_INTERNAL_LINUXEXAMPLESPLATFORMDEFINES_H */])

AC_CONFIG_HEADERS([$srcdir/linuxexamplesplatformdefines.h:linuxexamplesplatformdefines.h.in],[])

# ------------------------------------------------------------
# Optional examples

AM_CONDITIONAL([TE_BUILD_EXAMPLE],[test x$tlpkgs_examples_status = xyes])

AM_CONDITIONAL([TE_BUILD_GDAL_BASED_EXAMPLE],[
test x$tlpkgs_examples_status = xyes && test x$tlpkgs_gdal_status = xyes])

AM_CONDITIONAL([TE_BUILD_OGR_BASED_EXAMPLE],[
test x$tlpkgs_examples_status = xyes && test x$tlpkgs_ogr_status = xyes])

AM_CONDITIONAL([TE_BUILD_GEOS_BASED_EXAMPLE],[
test x$tlpkgs_examples_status = xyes && test x$tlpkgs_geos_status = xyes])

AM_CONDITIONAL([TE_BUILD_PROJ4_BASED_EXAMPLE],[
test x$tlpkgs_examples_status = xyes && test x$tlpkgs_proj4_status = xyes])

AM_CONDITIONAL([TE_BUILD_POSTGIS_BASED_EXAMPLE],[
test x$tlpkgs_examples_status = xyes && test x$tlpkgs_postgis_status = xyes])

AM_CONDITIONAL([TE_BUILD_QT_BASED_EXAMPLE],[
test x$tlpkgs_examples_status = xyes && test x$tlpkgs_qt_status = xyes])

# ------------------------------------------------------------
# Generate outputs

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
