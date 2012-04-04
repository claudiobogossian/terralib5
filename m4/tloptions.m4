# ------------------------------------------------------------
# Debug build
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_debug_status=no

# Analysing the user options
AC_ARG_ENABLE( 
  [debug],
  [
    AS_HELP_STRING([--enable-debug],
      [Enable the Terralib debug mode build - default:disabled]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_debug_status],[yes])
    CXXFLAGS="$CXXFLAGS -g -Wall -O0"
    CFLAGS="$CFLAGS -g -Wall -O0"
    AC_MSG_NOTICE([Terralib debug mode build enabled])
  ],
  [
    CPPFLAGS="-DNDEBUG $CPPFLAGS"
    CXXFLAGS="$CXXFLAGS -w -O2"
    CFLAGS="$CFLAGS -w -O2"
    AC_MSG_NOTICE([Terralib debug mode build disabled])
  ]  
)
# ------------------------------------------------------------
# Terralib logger enabling/disabling
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_logger_status=no

# Analysing the user options
AC_ARG_ENABLE( 
  [logger],
  [
    AS_HELP_STRING([--enable-logger],
      [Enable the Terralib logger - default:disabled]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_logger_status],[yes])
    AC_MSG_NOTICE([Terralib logger enabled])
  ],
  [
    AC_MSG_NOTICE([Terralib logger disabled])
  ]  
)

# ------------------------------------------------------------
# GRIB support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_grib_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_grib_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [grib],
  [
    AS_HELP_STRING([--with-grib<=PATH>],
      [Enable (default:disabled) the Terralib GRIB support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_grib_status],[check])
    AS_VAR_SET([tlpkgs_grib_arg],[$withval])
    LDFLAGS="-lgrib_api $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval/include -I$withval $CPPFLAGS"
        LDFLAGS="-L$withval/lib -L$withval $LDFLAGS"
      ]  
    )
  ],
  [
    AS_VAR_SET([tlpkgs_grib_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_grib_status" == xcheck],
  [
    AC_CHECK_LIB(
      [grib_api],
      [main],
      [
        AS_VAR_SET([tlpkgs_grib_status],[yes])
        AC_MSG_NOTICE([Terralib GRIB support enabled])
      ],
      [AC_MSG_FAILURE(GRIB API not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib GRIB support disabled])
  ]
)

# ------------------------------------------------------------
# OGR support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_ogr_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_ogr_arg=""
# NOTE: SINCE GDAL USES OGR: GDAL WILL BE USED FOR OGR SUPPORT
# Analysing the user options
AC_ARG_WITH( 
  [ogr],
  [
    AS_HELP_STRING([--with-ogr<=PATH>],
      [Enable (default:disabled) the Terralib OGR support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_ogr_status],[check])
    AS_VAR_SET([tlpkgs_ogr_arg],[$withval])
    LDFLAGS="-lgdal $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval/include -I$withval $CPPFLAGS"
        LDFLAGS="-L$withval/lib -L$withval $LDFLAGS"
      ]  
    )  
  ],
  [
    AS_VAR_SET([tlpkgs_ogr_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_ogr_status" == xcheck],
  [
    AC_CHECK_LIB(
      [gdal],
      [main],
      [
        AS_VAR_SET([tlpkgs_ogr_status],[yes])
        AC_MSG_NOTICE([Terralib OGR support enabled])
      ],
      [AC_MSG_FAILURE(GDAL not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib OGR support disabled])
  ]
)

# ------------------------------------------------------------
# GDAL support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_gdal_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_gdal_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [gdal],
  [
    AS_HELP_STRING([--with-gdal<=PATH>],
      [Enable (default:disabled) the Terralib GDAL support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_gdal_status],[check])
    AS_VAR_SET([tlpkgs_gdal_arg],[$withval])
    LDFLAGS="-lgdal $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval/include -I$withval $CPPFLAGS"
        LDFLAGS="-L$withval/lib -L$withval $LDFLAGS"
      ]  
    )
  ],
  [
    AS_VAR_SET([tlpkgs_gdal_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_gdal_status" == xcheck],
  [
    AS_IF(
      [test "x$tlpkgs_ogr_status" == xyes],
      [
        AC_CHECK_LIB(
          [gdal],
          [main],
          [
            AS_VAR_SET([tlpkgs_gdal_status],[yes])
            AC_MSG_NOTICE([Terralib GDAL support enabled])
          ],
          [AC_MSG_FAILURE(GDAL not found)]
        )
      ],
      [AC_MSG_FAILURE(Terralib OGR support must be enabled to use the GDAL support)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib GDAL support disabled])
  ]
)

# ------------------------------------------------------------
# Apache Log4CXX library support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_log4cxx_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_log4cxx_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [log4cxx],
  [
    AS_HELP_STRING([--with-log4cxx<=PATH>],
      [Enable (default:disabled) the Terralib Apache Log4CXX log support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_log4cxx_status],[check])
    AS_VAR_SET([tlpkgs_log4cxx_arg],[$withval])
    LDFLAGS="-llog4cxx $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval/include -I$withval $CPPFLAGS"
        LDFLAGS="-L$withval/lib -L$withval $LDFLAGS"
      ]  
    )  
  ],
  [
    AS_VAR_SET([tlpkgs_log4cxx_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_log4cxx_status" == xcheck],
  [
    AC_CHECK_LIB(
      [log4cxx],
      [open],
      [
        AS_VAR_SET([tlpkgs_log4cxx_status],[yes])
        AC_MSG_NOTICE([Terralib Apache Log4CXX log support enabled])
      ],
      [AC_MSG_FAILURE(Apache log4cxx not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib Apache Log4CXX log support disabled])
  ]
)

# ------------------------------------------------------------
# GEOS library support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_geos_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_geos_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [geos],
  [
    AS_HELP_STRING([--with-geos<=PATH>],
      [Enable (default:disabled) the Terralib GEOS support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_geos_status],[check])
    AS_VAR_SET([tlpkgs_geos_arg],[$withval])
    LDFLAGS="-lgeos $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval/include -I$withval $CPPFLAGS"
        LDFLAGS="-L$withval/lib -L$withval $LDFLAGS"
      ]  
    )  
  ],
  [
    AS_VAR_SET([tlpkgs_geos_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_geos_status" == xcheck],
  [
    AC_CHECK_LIB(
      [geos],
      [open],
      [
        AS_VAR_SET([tlpkgs_geos_status],[yes])
        AC_MSG_NOTICE([Terralib GEOS support enabled])
      ],
      [AC_MSG_FAILURE(GEOS not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib GEOS support disabled])
  ]
)

# ------------------------------------------------------------
# Proj4 library support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_proj4_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_proj4_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [proj4],
  [
    AS_HELP_STRING([--with-proj4<=PATH>],
      [Enable (default:disabled) the Terralib Proj4 support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_proj4_status],[check])
    AS_VAR_SET([tlpkgs_proj4_arg],[$withval])
    LDFLAGS="-lproj $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval/include -I$withval $CPPFLAGS"
        LDFLAGS="-L$withval/lib -L$withval $LDFLAGS"
      ]  
    )  
  ],
  [
    AS_VAR_SET([tlpkgs_proj4_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_proj4_status" == xcheck],
  [
    AC_CHECK_LIB(
      [proj],
      [pj_init_plus],
      [
        AS_VAR_SET([tlpkgs_proj4_status],[yes])
        AC_MSG_NOTICE([Terralib Proj4 support enabled])
      ],
      [AC_MSG_FAILURE(Proj4 not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib Proj4 support disabled])
  ]
)

# ------------------------------------------------------------
# PostGIS support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_postgis_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_postgis_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [postgis],
  [
    AS_HELP_STRING([--with-postgis<=PATH>],
      [Enable (default:disabled) the Terralib PostGIS support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_postgis_status],[check])
    AS_VAR_SET([tlpkgs_postgis_arg],[$withval])
    LDFLAGS="-lpq $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval/include -I$withval $CPPFLAGS"
        LDFLAGS="-L$withval/lib -L$withval $LDFLAGS"
      ]  
    )    
  ],
  [
    AS_VAR_SET([tlpkgs_postgis_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_postgis_status" == xcheck],
  [
    AC_CHECK_LIB(
      [pq],
      [PQexec],
      [
        AS_VAR_SET([tlpkgs_postgis_status],[yes])
        AC_MSG_NOTICE([Terralib PostGIS support enabled])
      ],
      [AC_MSG_FAILURE(PostGIS not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib PostGIS support disabled])
  ]
)

# ------------------------------------------------------------
# CPPunit support options
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_cppunit_status=no
# Possible values: empty(default), the user given argument
tlpkgs_cppunit_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [cppunit],
  [
    AS_HELP_STRING([--with-cppunit<=PATH>],
      [Enable (default:disabled) the Terralib CPPUnit support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_cppunit_status],[check])
    AS_VAR_SET([tlpkgs_cppunit_arg],[$withval])
    LDFLAGS="-lcppunit $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval/include -I$withval $CPPFLAGS"
        LDFLAGS="-L$withval/lib -L$withval $LDFLAGS"
      ]  
    ) 
  ],
  [
    AS_VAR_SET([tlpkgs_cppunit_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_cppunit_status" == xcheck],
  [
    AC_CHECK_HEADERS(
      [cppunit/Outputter.h],
      [
        AS_VAR_SET([tlpkgs_cppunit_status],[yes])
        AC_MSG_NOTICE([Terralib CPPUnit support enabled])
      ],
      [AC_MSG_FAILURE(CPPUnit not found)],
      []
    )
  ],
  [
    AC_MSG_NOTICE([Terralib CPPUnit support disabled])
  ]
)

# ------------------------------------------------------------
# QT support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_qt_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_qt_arg=""
# QT variables set (also exported to Makefile.am): 
#  QT_PATH - path to QT installed directory
#  MOC 
#  UIC 
#  RCC 

# Analysing the user options
AC_ARG_WITH( 
  [qt],
  [
    AS_HELP_STRING([--with-qt<=PATH>],
      [Enable (default:disabled) the Terralib QT support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_qt_status],[check])
    AS_VAR_SET([tlpkgs_qt_arg],[$withval])
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
        QT_PATH="$withval"
        AC_MSG_NOTICE(Using QT path:$QT_PATH)
      ],
      [
        AS_IF(
          [test -d "$QTDIR"],
          [
            AC_MSG_NOTICE(Using QTDIR variable:$QTDIR)
            CPPFLAGS="-I$QTDIR/include $CPPFLAGS"
            LDFLAGS="-L$QTDIR/lib $LDFLAGS"
            QT_PATH="$QTDIR"
          ],
          [
            AC_MSG_NOTICE(QTDIR environment variable not present. The system default paths will be used.)
          ]
        )
      ]
    )    
  ],
  [
    AS_VAR_SET([tlpkgs_qt_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_qt_status" == xcheck],
  [
    AC_CHECK_LIB(
      [QtCore],
      [main],
      [
        # ----------------------------------------------------
        # QT flags
        CPPFLAGS="-DQT_SHARED -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -D_REENTRANT $CPPFLAGS"
        LDFLAGS="-lQtCore -lQtGui $LDFLAGS"
        # ----------------------------------------------------
        # Find moc (Meta Object Compiler).
        AC_ARG_VAR([MOC], [Qt Meta Object Compiler command])
        AC_PATH_PROGS([MOC], [moc moc-qt4 moc-qt3], [missing],
                      [$QT_PATH:$QT_PATH/bin]:$PATH)
        if test x"$MOC" = xmissing; then
          AC_MSG_ERROR([Cannot find moc (Meta Object Compiler) in your PATH. Try using --with-qt.])
        fi
        # ----------------------------------------------------
        # Find uic (User Interface Compiler).
        AC_ARG_VAR([UIC], [Qt User Interface Compiler command])
        AC_PATH_PROGS([UIC], [uic uic-qt4 uic-qt3 uic3], [missing],
                      [$QT_PATH:$QT_PATH/bin:$PATH])
        if test x"$UIC" = xmissing; then
          AC_MSG_ERROR([Cannot find uic (User Interface Compiler) in your PATH. Try using --with-qt.])
        fi
        # ----------------------------------------------------
        # Find rcc (Qt Resource Compiler).
        AC_ARG_VAR([RCC], [Qt Resource Compiler command])
        AC_PATH_PROGS([RCC], [rcc], [false], [$QT_PATH:$QT_PATH/bin:$PATH])
        if test x"$UIC" = xfalse; then
          AC_MSG_WARN([Cannot find rcc (Qt Resource Compiler) in your PATH. Try using --with-qt.])
        fi    
        # ----------------------------------------------------       
        AS_VAR_SET([tlpkgs_qt_status],[yes])
        AC_MSG_NOTICE([Terralib QT support enabled])
      ],
      [AC_MSG_FAILURE(QT not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib QT support disabled])
  ]
)

# ------------------------------------------------------------
# Boost support (mandatory)
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_boost_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_boost_arg=""

CPPFLAGS="-DBOOST_FILESYSTEM_VERSION=3 $CPPFLAGS"

LDFLAGS="-lboost_date_time $LDFLAGS"
LDFLAGS="-lboost_filesystem $LDFLAGS"
# LDFLAGS="-lboost_graph $LDFLAGS"
LDFLAGS="-lboost_iostreams $LDFLAGS"
# LDFLAGS="-lboost_math_c99f $LDFLAGS"
# LDFLAGS="-lboost_math_c99l $LDFLAGS"
# LDFLAGS="-lboost_math_c99 $LDFLAGS"
# LDFLAGS="-lboost_math_tr1f $LDFLAGS"
# LDFLAGS="-lboost_math_tr1l $LDFLAGS"
# LDFLAGS="-lboost_math_tr1 $LDFLAGS"
# LDFLAGS="-lboost_prg_exec_monitor $LDFLAGS"
# LDFLAGS="-lboost_program_options $LDFLAGS"
# LDFLAGS="-lboost_random $LDFLAGS"
# LDFLAGS="-lboost_regex $LDFLAGS"
# LDFLAGS="-lboost_serialization $LDFLAGS"
# LDFLAGS="-lboost_signals $LDFLAGS"
LDFLAGS="-lboost_system $LDFLAGS"
LDFLAGS="-lboost_thread $LDFLAGS"
# LDFLAGS="-lboost_unit_test_framework $LDFLAGS"
# LDFLAGS="-lboost_wave $LDFLAGS"
# LDFLAGS="-lboost_wserialization $LDFLAGS"

# Analysing the user options
AC_ARG_WITH( 
  [boost],
  [
    AS_HELP_STRING([--with-boost=PATH],
      [Change the Terralib Boost support to use a specific given path - default:The default system path]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_boost_arg],[$withval])
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    )    
  ]
)

AC_CHECK_HEADERS(
  [boost/version.hpp],
  [
    AS_VAR_SET([tlpkgs_boost_status],[yes])
    AC_MSG_NOTICE([Terralib Boost support enabled])
  ],
  [
    AC_MSG_FAILURE(Boost not found)
  ]
)

# ------------------------------------------------------------
# Examples build
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_examples_status=no

# Analysing the user options
AC_ARG_ENABLE( 
  [examples],
  [
    AS_HELP_STRING([--enable-examples],
      [Enable the Terralib examples build - default:disabled]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_examples_status],[yes])
    AC_MSG_NOTICE([Terralib examples build enabled])
  ],
  [
    AC_MSG_NOTICE([Terralib examples build disabled])
  ]  
)


