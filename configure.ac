m4_include(m4/tlglobals.m4)

AC_INIT([terralib],TL_VERSION_MACRO)
AC_CONFIG_AUX_DIR([.])
AM_PROG_CC_C_O
AM_INIT_AUTOMAKE([subdir-objects])
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_LANG(C++)
AC_CONFIG_MACRO_DIR([m4])

# ------------------------------------------------------------
# Getting used defined options

m4_include(m4/tloptions.m4)

# ------------------------------------------------------------
# Generating the configure scripts

AC_CONFIG_SUBDIRS([src examples unittest])

# ------------------------------------------------------------
# Generate outputs

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
