if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  set (thirdParty_dlls
#    ${TE_DEPENDENCIES_DIR}/bin/asprintf.dll
    ${TE_DEPENDENCIES_DIR}/bin/boost_date_time-mt.dll
    ${TE_DEPENDENCIES_DIR}/bin/boost_chrono-mt.dll
    ${TE_DEPENDENCIES_DIR}/bin/boost_filesystem-mt.dll
    ${TE_DEPENDENCIES_DIR}/bin/boost_program_options-mt.dll
    ${TE_DEPENDENCIES_DIR}/bin/boost_system-mt.dll
    ${TE_DEPENDENCIES_DIR}/bin/boost_thread-mt.dll
    ${TE_DEPENDENCIES_DIR}/bin/charset.dll
    ${TE_DEPENDENCIES_DIR}/bin/comerr32.dll
 #   ${TE_DEPENDENCIES_DIR}/bin/csqlclient.dll
#    ${TE_DEPENDENCIES_DIR}/bin/csqljdbc.dll
#    ${TE_DEPENDENCIES_DIR}/bin/csqlodbc.dll
    ${TE_DEPENDENCIES_DIR}/bin/geos.dll
    ${TE_DEPENDENCIES_DIR}/bin/geotiff.dll
    ${TE_DEPENDENCIES_DIR}/bin/gettextlib.dll
    ${TE_DEPENDENCIES_DIR}/bin/gettextpo.dll
    ${TE_DEPENDENCIES_DIR}/bin/gettextsrc.dll
    ${TE_DEPENDENCIES_DIR}/bin/iconv.dll
    ${TE_DEPENDENCIES_DIR}/bin/intl.dll
    ${TE_DEPENDENCIES_DIR}/bin/krb5_32.dll
    ${TE_DEPENDENCIES_DIR}/bin/libeay32.dll
#    ${TE_DEPENDENCIES_DIR}/bin/libecpg.dll
    ${TE_DEPENDENCIES_DIR}/bin/libiconv-2.dll
    ${TE_DEPENDENCIES_DIR}/bin/libintl-2.dll
 #   ${TE_DEPENDENCIES_DIR}/bin/libmysql.dll
    ${TE_DEPENDENCIES_DIR}/bin/libpgtypes.dll
    ${TE_DEPENDENCIES_DIR}/bin/libpq.dll
    ${TE_DEPENDENCIES_DIR}/bin/libtiff.dll
    ${TE_DEPENDENCIES_DIR}/bin/log4cxx.dll
#    ${TE_DEPENDENCIES_DIR}/bin/mysqlcppconn.dll
    ${TE_DEPENDENCIES_DIR}/bin/proj.dll
    ${TE_DEPENDENCIES_DIR}/bin/qwt.dll
#    ${TE_DEPENDENCIES_DIR}/bin/shapelib.dll
    ${TE_DEPENDENCIES_DIR}/bin/ssleay32.dll
#    ${TE_DEPENDENCIES_DIR}/bin/v8.dll
    ${TE_DEPENDENCIES_DIR}/bin/terralib_ado4.dll
    ${TE_DEPENDENCIES_DIR}/bin/terralib4.dll
    ${TE_DEPENDENCIES_DIR}/bin/xerces-c.dll
#    ${TE_DEPENDENCIES_DIR}/bin/zlib1.dll
    ${TE_DEPENDENCIES_DIR}/gdal/win32/release/gdal110.dll
    ${TE_DEPENDENCIES_DIR}/gdal/win32/release/hdf5.dll
    ${TE_DEPENDENCIES_DIR}/gdal/win32/release/libexpat.dll
    ${TE_DEPENDENCIES_DIR}/gdal/win32/release/szip.dll
    ${TE_DEPENDENCIES_DIR}/gdal/win32/release/zlib.dll
  )
else()
  set (thirdParty_dlls
    ${TE_DEPENDENCIES_DIR}/bin/boost_chrono-vc100-mt-1_50.dll
    ${TE_DEPENDENCIES_DIR}/bin/boost_date_time-vc100-mt-1_50.dll
    ${TE_DEPENDENCIES_DIR}/bin/boost_filesystem-vc100-mt-1_50.dll
    ${TE_DEPENDENCIES_DIR}/bin/boost_program_options-vc100-mt-1_50.dll
    ${TE_DEPENDENCIES_DIR}/bin/boost_system-vc100-mt-1_50.dll
    ${TE_DEPENDENCIES_DIR}/bin/boost_thread-vc100-mt-1_50.dll
    ${TE_DEPENDENCIES_DIR}/bin/expat.dll
    ${TE_DEPENDENCIES_DIR}/bin/gdal110.dll
    ${TE_DEPENDENCIES_DIR}/bin/geos.dll
#    ${TE_DEPENDENCIES_DIR}/bin/geotiff.dll
    ${TE_DEPENDENCIES_DIR}/bin/hdf5.dll
#    ${TE_DEPENDENCIES_DIR}/bin/libcurl.dll
    ${TE_DEPENDENCIES_DIR}/bin/libeay32.dll
    ${TE_DEPENDENCIES_DIR}/bin/libiconv-2.dll
    ${TE_DEPENDENCIES_DIR}/bin/libiconv.dll
    ${TE_DEPENDENCIES_DIR}/bin/libintl-8.dll
    ${TE_DEPENDENCIES_DIR}/bin/libintl.dll
#    ${TE_DEPENDENCIES_DIR}/bin/libpng16.dll
    ${TE_DEPENDENCIES_DIR}/bin/libpq.dll
#    ${TE_DEPENDENCIES_DIR}/bin/libtiff.dll
    ${TE_DEPENDENCIES_DIR}/bin/log4cxx.dll
    ${TE_DEPENDENCIES_DIR}/bin/proj.dll
    ${TE_DEPENDENCIES_DIR}/bin/qwt.dll
    ${TE_DEPENDENCIES_DIR}/bin/ssleay32.dll
    ${TE_DEPENDENCIES_DIR}/bin/szip.dll
    ${TE_DEPENDENCIES_DIR}/bin/xerces-c_3_1.dll
    ${TE_DEPENDENCIES_DIR}/bin/zlib.dll
  )
endif()

find_package(Qt4)
if(QT4_FOUND)
  get_filename_component (_qtbindir ${QT_DESIGNER_EXECUTABLE} PATH)

  list ( APPEND
    thirdParty_dlls 
    ${_qtbindir}/assistant.exe
    ${_qtbindir}/QtCore4.dll
    ${_qtbindir}/QtGui4.dll
    ${_qtbindir}/QtSvg4.dll
    ${_qtbindir}/QtCLucene4.dll
    ${_qtbindir}/QtHelp4.dll
    ${_qtbindir}/QtNetwork4.dll
    ${_qtbindir}/QtOpenGL4.dll
    ${_qtbindir}/QtSql4.dll
    ${_qtbindir}/QtWebKit4.dll
    ${_qtbindir}/QtXml4.dll
  )
endif()

install ( 
  FILES ${thirdParty_dlls}
  DESTINATION "bin"
  COMPONENT BINARIES
)

file (GLOB QT_IC_PLGS ${QT_PLUGINS_DIR}/iconengines/*.dll)
file (GLOB QT_IMG_PLGS ${QT_PLUGINS_DIR}/imageformats/*.dll)
file (GLOB QT_SQL_PLGS ${QT_PLUGINS_DIR}/sqldrivers/*.dll)

install ( 
  FILES 
  ${QT_IC_PLGS}
  DESTINATION "bin/plugins/iconengines"
  COMPONENT BINARIES
)

install ( 
  FILES 
  ${QT_IMG_PLGS}
  DESTINATION "bin/plugins/imageformats"
  COMPONENT BINARIES
)

install ( 
  FILES 
  ${QT_SQL_PLGS}
  DESTINATION "bin/plugins/sqldrivers"
  COMPONENT BINARIES
)
