# ------------------------------------------------------------
# Solving Terralib basic mandatory modules depencencies

AM_LDFLAGS="-lterralib_datatype"

AM_LDFLAGS="-lterralib_common $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_color $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_xml $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_dataaccess $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_memory $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_gml $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_fe $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_se $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_maptools $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_geometry $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_annotationtext $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_plugin $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_raster $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_srs $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_xlink $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_rp $AM_LDFLAGS"

# ------------------------------------------------------------
# Solving Terralib optional modules depencencies

# QT widgets module
AS_IF(
  [test "x$tlpkgs_qt_status" == xyes],
  [
    AM_LDFLAGS="-lterralib_qtwidgets $AM_LDFLAGS"
  ]  
)

# ------------------------------------------------------------
# Configuring the output variable used when building and executing binaries

AM_LDFLAGS="-L../bin $AM_LDFLAGS"
AC_SUBST(AM_LDFLAGS,[$AM_LDFLAGS])

AM_CPPFLAGS="-I$srcdir/../src"
AC_SUBST(AM_CPPFLAGS,[$AM_CPPFLAGS])
