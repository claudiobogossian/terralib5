# ------------------------------------------------------------
# Terralib logger
# These are the defines set (The possible values are: 0 or 1):
# LOGGER_ENABLED

AS_IF(
  [test "x$tlpkgs_logger_status" == xyes],
  [
    AC_DEFINE(
      [LOGGER_ENABLED],
      [1],
      [Value 1 will enable all kind of logging in the TerraLib code]
    )  
  ],
  [
    AC_DEFINE(
      [LOGGER_ENABLED],
      [0],
      [Value 1 will enable all kind of logging in the TerraLib code]
    )
  ]  
)
# ------------------------------------------------------------
# GDAL support
# These are the defines set (The possible values are: 0 or 1):
# USE_GDAL

AS_IF(
  [test "x$tlpkgs_gdal_status" == xyes],
  [
    AC_DEFINE(
      [USE_GDAL],
      [1],
      [Value 1 will enable GDAL support, value 0 will disable it]
    )  
  ],
  [
    AC_DEFINE(
      [USE_GDAL],
      [0],
      [Value 1 will enable GDAL support, value 0 will disable it]
    )
  ]  
)

# ------------------------------------------------------------
# OGR support
# These are the defines set (The possible values are: 0 or 1):
# USE_OGR

AS_IF(
  [test "x$tlpkgs_ogr_status" == xyes],
  [
    AC_DEFINE(
      [USE_OGR],
      [1],
      [Value 1 will enable OGR support, value 0 will disable it]
    )  
  ],
  [
    AC_DEFINE(
      [USE_OGR],
      [0],
      [Value 1 will enable OGR support, value 0 will disable it]
    )
  ]  
)

# ------------------------------------------------------------
# Apache Log4CXX library support
# These are the defines set (The possible values are: 0 or 1):
# USE_APACHE_LOG4CXX

AS_IF(
  [test "x$tlpkgs_log4cxx_status" == xyes],
  [
    AC_DEFINE(
      [USE_APACHE_LOG4CXX],
      [1],
      [Value 1 will enable Apache Log4cxx log support, value 0 will disable it]
    ) 
  ],
  [
    AC_DEFINE(
      [USE_APACHE_LOG4CXX],
      [0],
      [Value 1 will enable Apache Log4cxx log support, value 0 will disable it]
    ) 
  ]  
)

# ------------------------------------------------------------
# GEOS library support
# These are the defines set (The possible values are: 0 or 1):
# USE_GEOS

AS_IF(
  [test "x$tlpkgs_geos_status" == xyes],
  [
    AC_DEFINE(
      [USE_GEOS],
      [1],
      [Value 1 will enable GEOS support, value 0 will disable it]
    ) 
  ],
  [
    AC_DEFINE(
      [USE_GEOS],
      [0],
      [Value 1 will enable GEOS support, value 0 will disable it]
    ) 
  ]  
)

# ------------------------------------------------------------
# Proj4 support
# These are the defines set (The possible values are: 0 or 1):
# USE_PROJ4

AS_IF(
  [test "x$tlpkgs_proj4_status" == xyes],
  [
    AC_DEFINE(
      [USE_PROJ4],
      [1],
      [Value 1 will enable Proj4 support, value 0 will disable it]
    )  
  ],
  [
    AC_DEFINE(
      [USE_PROJ4],
      [0],
      [Value 1 will enable Proj4 support, value 0 will disable it]
    )
  ]  
)

# ------------------------------------------------------------
# PostGIS support
# These are the defines set (The possible values are: 0 or 1):
# USE_POSTGIS

AS_IF(
  [test "x$tlpkgs_postgis_status" == xyes],
  [
    AC_DEFINE(
      [USE_PGIS],
      [1],
      [Value 1 will enable PostGIS support, value 0 will disable it]
    )  
  ],
  [
    AC_DEFINE(
      [USE_PGIS],
      [0],
      [Value 1 will enable PostGIS support, value 0 will disable it]
    )
  ]  
)

# ------------------------------------------------------------
# GRIB support
# These are the defines set (The possible values are: 0 or 1):
# USE_GRIB

AS_IF(
  [test "x$tlpkgs_grib_status" == xyes],
  [
    AC_DEFINE(
      [USE_GRIB],
      [1],
      [Value 1 will enable GRIB support, value 0 will disable it]
    )  
  ],
  [
    AC_DEFINE(
      [USE_GRIB],
      [0],
      [Value 1 will enable GRIB support, value 0 will disable it]
    )
  ]  
)