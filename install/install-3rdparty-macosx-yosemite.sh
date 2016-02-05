#!/bin/bash
#
#  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of the TerraLib - a Framework for building GIS enabled applications.
#
#  TerraLib is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  TerraLib is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with TerraLib. See COPYING. If not, write to
#  TerraLib Team at <terralib-team@terralib.org>.
#
#
#  Description: Install all required software for TerraLib on MAC OS X Yosemite.
#
#  Author: Gilberto Ribeiro de Queiroz
#
#
#  Example:
#  $ TERRALIB_DEPENDENCIES_DIR="/Users/gribeiro/MyLibs" ./install-3rdparty-macosx-yosemite.sh
#

echo "********************************************"
echo "* TerraLib Installer for Mac OS X Yosemite *"
echo "********************************************"
echo ""
sleep 1s


#
# If first argument is false it aborts the script and
# report the message passed as second argument.
#
function valid()
{
  if [ $1 -ne 0 ]; then
    echo $2
    echo ""
    exit
  fi
}


#
# Check for terralib-3rdparty-macosx-yosemite.tar.gz
#
if [ ! -f ./terralib-3rdparty-macosx-yosemite.tar.gz ]; then
  echo "Please, make sure to have terralib-3rdparty-macosx-yosemite.tar.gz in the current directory!"
  exit
fi


#
# Extract packages
#
echo "extracting packages..."
sleep 1s

tar xzvf terralib-3rdparty-macosx-yosemite.tar.gz
valid $? "Error: could not extract 3rd party libraries (terralib-3rdparty-macosx-yosemite.tar.gz)"

echo "packages extracted!"
sleep 1s


#
# Go to 3rd party libraries dir
#
cd terralib-3rdparty-macosx-yosemite
valid $? "Error: could not enter 3rd-party libraries dir (terralib-3rdparty-macosx-yosemite)"


#
# Check installation dir
#
if [ "$TERRALIB_DEPENDENCIES_DIR" == "" ]; then
  TERRALIB_DEPENDENCIES_DIR="/opt/terralib"
fi

export PATH="$PATH:$TERRALIB_DEPENDENCIES_DIR/bin"
export LD_LIBRARY_PATH="$PATH:$TERRALIB_DEPENDENCIES_DIR/lib"

echo "installing 3rd-party libraries to '$TERRALIB_DEPENDENCIES_DIR' ..."
sleep 1s


#
# PCRE version 8.37
# Site: http://www.pcre.org
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libpcre.a" ]; then
  echo "installing PCRE..."
  sleep 1s

  tar xzvf pcre-8.37.tar.gz
  valid $? "Error: could not uncompress pcre-8.37.tar.gz!"

  cd pcre-8.37
  valid $? "Error: could not enter pcre-8.32 dir!"

  ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure PCRE!"

  make -j 4
  valid $? "Error: could not make PCRE!"

  make install
  valid $? "Error: Could not install PCRE!"

  cd ..
fi


#
# SWIG version 3.0.5
# Site: http://www.swig.org
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/bin/swig" ]; then
  echo "installing SWIG..."
  echo ""
  sleep 1s

  tar xzvf swig-3.0.5.tar.gz
  valid $? "Error: could not uncompress swig-3.0.5.tar.gz!"

  cd swig-3.0.5
  valid $? "Error: could not enter swig-3.0.5 dir!"

  ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR --with-pcre-prefix=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure SWIG!"

  make -j 4
  valid $? "Error: could not make SWIG!"

  make install
  valid $? "Error: Could not install SWIG!"

  cd ..
fi


#
# FreeXL version 1.0.1
# Site: https://www.gaia-gis.it/fossil/freexl/index
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libfreexl.dylib" ]; then
  echo "installing FreeXL..."
  sleep 1s

  tar xzvf freexl-1.0.1.tar.gz
  valid $? "Error: could not uncompress freexl-1.0.1.tar.gz!"

  cd freexl-1.0.1
  valid $? "Error: could not enter freexl-1.0.1 dir!"

  CPPFLAGS=-I$TERRALIB_DEPENDENCIES_DIR/include LDFLAGS=-L$TERRALIB_DEPENDENCIES_DIR/lib ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure FreeXL!"

  make -j 4
  valid $? "Error: could not make FreeXL!"

  make install
  valid $? "Error: Could not install FreeXL!"

  cd ..
fi


#
# OOSP-UUID version 1.6.2
# Site: http://www.ossp.org/pkg/lib/uuid/
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libuuid.dylib" ]; then
  echo "installing OOSP-UUID..."
  sleep 1s

  tar xvf uuid-1.6.2.tar
  valid $? "Error: could not uncompress uuid-1.6.2.tar!"

  cd uuid-1.6.2
  valid $? "Error: could not enter uuid-1.6.2 dir!"

  ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR --with-cxx
  valid $? "Error: could not configure OOSP-UUID!"

  make -j 4
  valid $? "Error: could not make OOSP-UUID!"

  make install
  valid $? "Error: Could not install OOSP-UUID!"

  cd ..
fi


#
# BZIP2 verson 1.0.6
# Site: http://www.bzip.org
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libbz2.a" ]; then
  echo "installing bzip2..."
  sleep 1s

  tar xzvf bzip2-1.0.6.tar.gz
  valid $? "Error: could not uncompress bzip2-1.0.6.tar.gz!"

  cd bzip2-1.0.6
  valid $? "Error: could not enter bzip2-1.0.6 dir!"

  make
  valid $? "Error: could not make BZIP2!"

  make install PREFIX=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: Could not install BZIP2!"

  cd ..
fi


#
# Proj4 version 4.9.1 (with proj-datumgrid version 1.5)
# Site: https://trac.osgeo.org/proj/
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libproj.dylib" ]; then
  echo "installing Proj4..."
  sleep 1s

  tar xzvf proj-4.9.1.tar.gz
  valid $? "Error: could not uncompress proj-4.9.1.tar.gz!"

  cd proj-4.9.1
  valid $? "Error: could not enter proj-4.9.1 dir!"

  ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure Proj4!"

  make -j 4
  valid $? "Error: could not make Proj4!"

  make install
  valid $? "Error: Could not install Proj4!"

  cd ..
fi


#
# GEOS version 3.4.2
# Site: http://geos.osgeo.org
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libgeos.dylib" ]; then
  echo "installing GEOS..."
  sleep 1s

  tar xjvf geos-3.4.2.tar.bz2
  valid $? "Error: could not uncompress geos-3.4.2.tar.bz2!"

  cd geos-3.4.2
  valid $? "Error: could not enter geos-3.4.2 dir!"

  ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure GEOS!"

  make -j 4
  valid $? "Error: could not make GEOS!"

  make install
  valid $? "Error: Could not install GEOS!"

  cd ..
fi


#
# libPNG version 1.5.17
# Site: http://www.libpng.org/pub/png/libpng.html
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libpng.dylib" ]; then
  echo "installing libPNG..."
  sleep 1s

  tar xzvf libpng-1.5.17.tar.gz
  valid $? "Error: could not uncompress libpng-1.5.17.tar.gz!"

  cd libpng-1.5.17
  valid $? "Error: could not enter libpng-1.5.17 dir!"

  ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure libPNG!"

  make -j 4
  valid $? "Error: could not make libPNG!"

  make install
  valid $? "Error: Could not install libPNG!"

  cd ..
fi


#
# Independent JPEG Group version v9a
# Site: http://www.ijg.org
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libjpeg.dylib" ]; then
  echo "installing Independent JPEG Group Library..."
  sleep 1s

  tar xzvf jpegsrc.v9a.tar.gz
  valid $? "Error: could not uncompress jpegsrc.v9a.tar.gz!"

  cd jpeg-9a
  valid $? "Error: could not enter jpeg-9a dir!"

  ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure JPEG!"

  make -j 4
  valid $? "Error: could not make JPEG!"

  make install
  valid $? "Error: Could not install JPEG!"

  cd ..
fi


#
# TIFF version 4.0.3
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libtiff.dylib" ]; then
  echo "installing TIFF..."
  sleep 1s

  tar xzvf tiff-4.0.3.tar.gz
  valid $? "Error: could not uncompress tiff-4.0.3.tar.gz!"

  cd tiff-4.0.3
  valid $? "Error: could not enter tiff-4.0.3!"

  ./configure --enable-cxx --with-jpeg-include-dir=$TERRALIB_DEPENDENCIES_DIR/include --with-jpeg-lib-dir=$TERRALIB_DEPENDENCIES_DIR/lib --prefix=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure TIFF!"

  make -j 4
  valid $? "Error: could not make TIFF!"

  make install
  valid $? "Error: Could not install TIFF!"

  cd ..
fi


#
# GeoTIFF version 1.4.0
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libgeotiff.dylib" ]; then
  echo "installing GeoTIFF..."
  sleep 1s

  tar xzvf libgeotiff-1.4.0.tar.gz
  valid $? "Error: could not uncompress libgeotiff-1.4.0.tar.gz!"

  cd libgeotiff-1.4.0
  valid $? "Error: could not enter libgeotiff-1.4.0!"

  ./configure --with-jpeg=$TERRALIB_DEPENDENCIES_DIR --with-zlib --with-libtiff=$TERRALIB_DEPENDENCIES_DIR --with-proj=$TERRALIB_DEPENDENCIES_DIR --prefix=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure GeoTIFF!"

  make -j 4
  valid $? "Error: could not make GeoTIFF!"

  make install
  valid $? "Error: Could not install GeoTIFF!"

  cd ..
fi


#
# SZIP version 2.1
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libsz.dylib" ]; then
  echo "installing SZIP..."
  sleep 1s

  tar xzvf szip-2.1.tar.gz
  valid $? "Error: could not uncompress szip-2.1.tar.gz!"

  cd szip-2.1
  valid $? "Error: could not enter szip-2.1!"

  ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure SZIP!"

  make -j 4
  valid $? "Error: could not make SZIP!"

  make install
  valid $? "Error: Could not install SZIP!"

  cd ..
fi


#
# CURL version 7.42.1
# Site: http://curl.haxx.se/libcurl/
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libcurl.dylib" ]; then
  echo "installing CURL..."
  sleep 1s

  tar xzvf curl-7.42.1.tar.gz
  valid $? "Error: could not uncompress curl-7.42.1.tar.gz!"

  cd curl-7.42.1
  valid $? "Error: could not enter curl-7.42.1!"

  ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure CURL!"

  make -j 4
  valid $? "Error: could not make CURL!"

  make install
  valid $? "Error: Could not install CURL!"

  cd ..
fi


#
# ICU 52.1
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libicuuc.a" ]; then
  echo "installing ICU..."
  sleep 1s

  tar xzvf icu4c-52_1-src.tgz
  valid $? "Error: could not uncompress icu4c-52_1-src.tgz!"

  cd icu/source
  valid $? "Error: could not enter icu/source!"

  chmod +x runConfigureICU configure install-sh
  valid $? "Error: could not set runConfigureICU to execute mode!"

  CPPFLAGS="-DU_USING_ICU_NAMESPACE=0 -DU_CHARSET_IS_UTF8=1 -DU_NO_DEFAULT_INCLUDE_UTF_HEADERS=1" ./runConfigureICU MacOSX --prefix=$TERRALIB_DEPENDENCIES_DIR --enable-static --disable-shared --with-data-packaging=archive
  valid $? "Error: could not runConfigureICU!"

  make -j 4
  valid $? "Error: could not make ICU!"

  #make check
  #valid $? "Error: could not check ICU build!"

  make install
  valid $? "Error: Could not install ICU!"

  cd ../..
fi


#
# Xerces-c version 3.1.1
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libxerces-c.dylib" ]; then
  echo "installing Xerces..."
  sleep 1s

  tar xzvf xerces-c-3.1.1.tar.gz
  valid $? "Error: could not uncompress xerces-c-3.1.1.tar.gz!"

  cd xerces-c-3.1.1
  valid $? "Error: could not enter xerces-c-3.1.1!"

  CPPFLAGS=-I$TERRALIB_DEPENDENCIES_DIR/include LDFLAGS=-L$TERRALIB_DEPENDENCIES_DIR/lib ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR --enable-netaccessor-curl --disable-static --enable-msgloader-icu --with-icu=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure Xerces-c!"

  make -j 4
  valid $? "Error: could not make Xerces-c!"

  make install
  valid $? "Error: Could not install Xerces-c!"

  cd ..
fi


#
# libxml2
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libxml2.dylib" ]; then
  echo "installing libxml2..."
  sleep 1s

  tar xzvf libxml2-2.9.1.tar.gz
  valid $? "Error: could not uncompress libxml2-2.9.1.tar.gz!"

  cd libxml2-2.9.1
  valid $? "Error: could not enter libxml2-2.9.1!"

  CPPFLAGS=-I$TERRALIB_DEPENDENCIES_DIR/include LDFLAGS=-L$TERRALIB_DEPENDENCIES_DIR/lib ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR --with-icu --without-python
  valid $? "Error: could not configure libxml2!"

  make -j 4
  valid $? "Error: could not make libxml2"

  make install
  valid $? "Error: Could not install libxml2"

  cd ..
fi


#
# libxslt
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libxslt.dylib" ]; then
  echo "installing libxslt..."
  sleep 1s

  tar xzvf libxslt-1.1.28.tar.gz
  valid $? "Error: could not uncompress libxslt-1.1.28.tar.gz!"

  cd libxslt-1.1.28
  valid $? "Error: could not enter libxslt-1.1.28!"

  CPPFLAGS=-I$TERRALIB_DEPENDENCIES_DIR/include LDFLAGS=-L$TERRALIB_DEPENDENCIES_DIR/lib ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR --with-libxml-prefix=$TERRALIB_DEPENDENCIES_DIR --without-debug
  valid $? "Error: could not configure libxslt!"

  make -j 4
  valid $? "Error: could not make libxslt"

  make install
  valid $? "Error: Could not install libxslt"

  cd ..
fi


#
# Boost 1.58
# Site: http://www.boost.org
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libboost_thread.dylib" ]; then
  echo "installing boost..."
  sleep 1s

  tar xzvf boost_1_58_0.tar.gz
  valid $? "Error: could not uncompress boost_1_58_0.tar.gz!"

  cd boost_1_58_0
  valid $? "Error: could not enter boost_1_58_0!"

  ./bootstrap.sh
  valid $? "Error: could not configure Boost!"

  ./b2 runtime-link=shared link=shared variant=release threading=multi --prefix=$TERRALIB_DEPENDENCIES_DIR -sICU_PATH=$TERRALIB_DEPENDENCIES_DIR -sICONV_PATH=$TERRALIB_DEPENDENCIES_DIR -sBZIP2_INCLUDE=$TERRALIB_DEPENDENCIES_DIR/include -sBZIP2_LIBPATH=$TERRALIB_DEPENDENCIES_DIR/lib install
  valid $? "Error: could not make boost"

  cd ..
fi


#
# PostgreSQL version 9.4.1
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/pgsql/lib/libpq.dylib" ]; then
  echo "installing PostgreSQL..."
  sleep 1s

  tar xjvf postgresql-9.4.1.tar.bz2
  valid $? "Error: could not uncompress postgresql-9.4.1.tar.bz2!"

  cd postgresql-9.4.1
  valid $? "Error: could not enter postgresql-9.4.1!"

  CPPFLAGS="-I$TERRALIB_DEPENDENCIES_DIR/include -I$TERRALIB_DEPENDENCIES_DIR/include/libxml2" LDFLAGS="-lstdc++ -L$TERRALIB_DEPENDENCIES_DIR/lib" ./configure --with-libxml --with-libxslt --with-openssl --with-uuid=e2fs --prefix=$TERRALIB_DEPENDENCIES_DIR/pgsql --with-includes=$TERRALIB_DEPENDENCIES_DIR/include --with-libraries=$TERRALIB_DEPENDENCIES_DIR/lib
  valid $? "Error: could not configure postgresql!"

  make -j 4
  valid $? "Error: could not make postgresql"

  make install
  valid $? "Error: Could not install postgresql"

  cd contrib/uuid-ossp
  valid $? "Error: could not enter postgresql-9.4.1/contrib/uuid-ossp!"

  make
  valid $? "Error: could not make postgresql-9.4.1/contrib/uuid-ossp"

  make install
  valid $? "Error: Could not install postgresql-9.4.1/contrib/uuid-ossp"

  cd ../../..
fi


#
# HDF4 version 4.2.9
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libmfhdf.a" ]; then
  echo "installing HDF4..."
  sleep 1s

  tar xzvf hdf-4.2.9.tar.gz
  valid $? "Error: could not uncompress hdf-4.2.9.tar.gz!"

  cd hdf-4.2.9
  valid $? "Error: could not enter hdf-4.2.9!"

  ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR --with-szlib=$TERRALIB_DEPENDENCIES_DIR --with-zlib --with-jpeg=$TERRALIB_DEPENDENCIES_DIR --enable-netcdf --disable-fortran
  valid $? "Error: could not configure hdf-4!"

  make -j 4
  valid $? "Error: could not make hdf-4"

  make install
  valid $? "Error: Could not install hdf-4"

  cd ..
fi


#
# SQLite version 3.8.10.1
# Site: https://www.sqlite.org
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libsqlite3.dylib" ]; then
  echo "installing SQLite..."
  sleep 1s

  tar xzvf sqlite-autoconf-3081001.tar.gz
  valid $? "Error: could not uncompress sqlite-autoconf-3081001.tar.gz!"

  cd sqlite-autoconf-3081001
  valid $? "Error: could not enter sqlite-autoconf-3081001!"

  CFLAGS="-Os -DSQLITE_ENABLE_COLUMN_METADATA -DSQLITE_ENABLE_FTS4 -DSQLITE_ENABLE_RTREE -DSQLITE_SOUNDEX -DSQLITE_OMIT_AUTOINIT" ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not configure sqlite!"

  make -j 4
  valid $? "Error: could not make sqlite"

  make install
  valid $? "Error: Could not install sqlite"

  cd ..
fi


#
# SpatiaLite version 4.2.0
# Site: https://www.gaia-gis.it/fossil/libspatialite
# Obs.: In the future we should turn on libxml2 support.
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libspatialite.dylib" ]; then
  echo "installing SpatiaLite..."
  sleep 1s

  tar xzvf libspatialite-4.2.0.tar.gz
  valid $? "Error: could not uncompress libspatialite-4.2.0.tar.gz!"

  cd libspatialite-4.2.0
  valid $? "Error: could not enter libspatialite-4.2.0!"

  CPPFLAGS="-I$TERRALIB_DEPENDENCIES_DIR -I$TERRALIB_DEPENDENCIES_DIR/include -I$TERRALIB_DEPENDENCIES_DIR/include/libxml2 -I$TERRALIB_DEPENDENCIES_DIR/include/libxml2/libxml" LDFLAGS="-L$TERRALIB_DEPENDENCIES_DIR/lib"  ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR --enable-proj --enable-geos --enable-geosadvanced --enable-iconv --enable-freexl --enable-geocallbacks --enable-epsg --enable-gcov --enable-mathsql --enable-libxml2=no --enable-geopackage --with-geosconfig=$TERRALIB_DEPENDENCIES_DIR/bin/geos-config
  valid $? "Error: could not configure libspatialite!"

  make -j 4
  valid $? "Error: could not make libspatialite"

  make install
  valid $? "Error: Could not install libspatialite"

  cd ..
fi


#
# GDAL/OGR version 1.11.2
# Site: http://www.gdal.org
# Obs.: in the future add a flag --with-libkml --with-php --with-python
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/gdal1/lib/libgdal.dylib" ]; then
  echo "installing GDAL/OGR 1.11.2..."
  sleep 1s

  tar xzvf gdal-1.11.2.tar.gz
  valid $? "Error: could not uncompress gdal-1.11.2.tar.gz!"

  cd gdal-1.11.2
  valid $? "Error: could not enter gdal-1.11.2!"

  CPPFLAGS="-I$TERRALIB_DEPENDENCIES_DIR/include -I$TERRALIB_DEPENDENCIES_DIR/include/libxml2" LDFLAGS=-L$TERRALIB_DEPENDENCIES_DIR/lib ./configure --with-pg=$TERRALIB_DEPENDENCIES_DIR/pgsql/bin/pg_config --with-png=$TERRALIB_DEPENDENCIES_DIR --with-libtiff=$TERRALIB_DEPENDENCIES_DIR --with-geotiff=$TERRALIB_DEPENDENCIES_DIR --with-jpeg=$TERRALIB_DEPENDENCIES_DIR  --with-gif --with-ecw=yes --with-xerces=$TERRALIB_DEPENDENCIES_DIR --with-expat=yes --with-curl=$TERRALIB_DEPENDENCIES_DIR/bin/curl-config --with-sqlite3=$TERRALIB_DEPENDENCIES_DIR --with-geos=$TERRALIB_DEPENDENCIES_DIR/bin/geos-config --with-threads --with-spatialite=$TERRALIB_DEPENDENCIES_DIR --with-hdf4=$TERRALIB_DEPENDENCIES_DIR --with-freexl=$TERRALIB_DEPENDENCIES_DIR --prefix=$TERRALIB_DEPENDENCIES_DIR/gdal1 --with-xml2=$TERRALIB_DEPENDENCIES_DIR/bin/xml2-config --without-python
  valid $? "Error: could not configure gdal 1.11.2!"

  make -j 4
  valid $? "Error: could not make gdal 1.11.2"

  make install
  valid $? "Error: Could not install gdal 1.11.2"

  cd ..
fi

#
# GDAL/OGR version 2.0.1
# Site: http://www.gdal.org
# Obs.: in the future add a flag --with-libkml --with-php --with-python
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/gdal2/lib/libgdal.dylib" ]; then
  echo "installing GDAL/OGR 2.0.1..."
  sleep 1s

  tar xzvf gdal-2.0.1.tar.gz
  valid $? "Error: could not uncompress gdal-2.0.1.tar.gz!"

  cd gdal-2.0.1
  valid $? "Error: could not enter gdal-2.0.1!"

  CPPFLAGS="-I$TERRALIB_DEPENDENCIES_DIR/include -I$TERRALIB_DEPENDENCIES_DIR/include/libxml2" LDFLAGS=-L$TERRALIB_DEPENDENCIES_DIR/lib ./configure --with-pg=$TERRALIB_DEPENDENCIES_DIR/pgsql/bin/pg_config --with-png=$TERRALIB_DEPENDENCIES_DIR --with-libtiff=$TERRALIB_DEPENDENCIES_DIR --with-geotiff=$TERRALIB_DEPENDENCIES_DIR --with-jpeg=$TERRALIB_DEPENDENCIES_DIR  --with-gif --with-ecw=yes --with-xerces=$TERRALIB_DEPENDENCIES_DIR --with-expat=yes --with-curl=$TERRALIB_DEPENDENCIES_DIR/bin/curl-config --with-sqlite3=$TERRALIB_DEPENDENCIES_DIR --with-geos=$TERRALIB_DEPENDENCIES_DIR/bin/geos-config --with-threads --with-spatialite=$TERRALIB_DEPENDENCIES_DIR --with-hdf4=$TERRALIB_DEPENDENCIES_DIR --with-freexl=$TERRALIB_DEPENDENCIES_DIR --prefix=$TERRALIB_DEPENDENCIES_DIR/gdal2 --with-xml2=$TERRALIB_DEPENDENCIES_DIR/bin/xml2-config  --without-python
  valid $? "Error: could not configure gdal 2.0.1!"

  make -j 4
  valid $? "Error: could not make gdal 2.0.1"

  make install
  valid $? "Error: Could not install gdal 2.0.1"

  cd ..
fi


#
# CppUnit version 1.12.1
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libcppunit.dylib" ]; then
  echo "installing CppUnit.."
  sleep 1s

  tar xzvf cppunit-1.12.1.tar.gz
  valid $? "Error: could not uncompress cppunit-1.12.1.tar.gz!"

  cd cppunit-1.12.1
  valid $? "Error: could not enter cppunit-1.12.1!"

  #LDFLAGS="-ldl" ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR
  ./configure --prefix=$TERRALIB_DEPENDENCIES_DIR
#  valid $? "Error: could not configure cppunit!"

  make -j 4
  valid $? "Error: could not make cppunit"

  make install
  valid $? "Error: Could not install cppunit"

  cd ..
fi


#
# Qt Property Browser
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libqt_property_browser.dylib" ]; then
  echo "installing Qt Property Browser..."
  sleep 1s

  tar xzvf qtpropertybrowser.tar.gz
  valid $? "Error: could not uncompress qtpropertybrowser.tar.gz!"

  cd qtpropertybrowser
  valid $? "Error: could not change dir to qtpropertybrowser!"

  qmake "TERRALIB_DIR=$TERRALIB_DEPENDENCIES_DIR"
  valid $? "Error: could not run qmake for qt-property-browser!"

  make -j 4
  valid $? "Error: could not make qt-property-browser!"

  make install
  valid $? "Error: Could not install qt-property-browser!"

  cd ..
fi


#
# QScintilla
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libqscintilla2.dylib" ]; then
  echo "installing QScintilla..."
  sleep 1s

  tar xzvf QScintilla-gpl-2.8.tar.gz
  valid $? "Error: could not uncompress QScintilla-gpl-2.8.tar.gz!"

  cd QScintilla-gpl-2.8/Qt4Qt5
  valid $? "Error: could not change dir to QScintilla-gpl-2.8/Qt4Qt5!"

  qmake "TERRALIB_DIR=$TERRALIB_DEPENDENCIES_DIR"
  valid $? "Error: could not prepare QScintilla build with qmake!"

  make -j 4
  valid $? "Error: could not make QScintilla!"

  make install
  valid $? "Error: Could not install QScintilla!"

  cd ../..
fi


#
# Lua
#
if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/liblua.a" ]; then
  echo "installing Lua..."
  sleep 1s

  tar xzvf lua-5.2.2.tar.gz
  valid $? "Error: could not uncompress lua-5.2.2.tar.gz!"

  cd lua-5.2.2
  valid $? "Error: could not change dir to lua-5.2.2!"

  make macosx INSTALL_TOP=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not make Lua!"

  make linux install INSTALL_TOP=$TERRALIB_DEPENDENCIES_DIR
  valid $? "Error: could not install Lua!"

  cd ..
fi
 
 
#
# QtLua
#
# if [ ! -f "$TERRALIB_DEPENDENCIES_DIR/lib/libqtlua.dylib" ]; then
#   echo "installing QtLua..."
#   sleep 1s
#  
#   #tar xzvf libqtlua-2.0.tar.gz
#   #valid $? "Error: could not uncompress libqtlua-2.0.tar.gz!"
# 
#   cd libqtlua-2.0
#   valid $? "Error: could not change dir to libqtlua-2.0!"
# 
#   LDFLAGS=-L$TERRALIB_DEPENDENCIES_DIR/lib ./configure --enable-qtrelease --disable-examples --enable-shared --with-lua-inc-dir=$TERRALIB_DEPENDENCIES_DIR/include --prefix=$TERRALIB_DEPENDENCIES_DIR --with-lua-lib=lua
#   valid $? "Error: could not configure QtLua!"
# 
#   make -j 4
#   valid $? "Error: could not make QtLua!"
# 
#   make install
#   valid $? "Error: could not install QtLua!"
# 
#   cp -r src/internal $TERRALIB_DEPENDENCIES_DIR/include/QtLua/
#   valid $? "Error: could not copy QtLua internal folder!"
# 
#   cd ..
# fi


#
# Qwt version 
# Site: http://qwt.sourceforge.net
#
if [ ! -d "$TERRALIB_DEPENDENCIES_DIR/lib/qwt.framework" ]; then
  echo "installing Qwt..."
  sleep 1s

  tar xjvf qwt-6.1.2.tar.bz2
  valid $? "Error: could not uncompress qwt-6.1.2.tar.bz2!"

  cd qwt-6.1.2
  valid $? "Error: could not change dir to qwt-6.1.2!"

  qmake qwt.pro "QWT_INSTALL_PREFIX_TARGET=$TERRALIB_DEPENDENCIES_DIR"
  valid $? "Error: could not configure Qwt!"

  make -j 4
  valid $? "Error: could not make Qwt!"

  make install
  valid $? "Error: could not copy Qwt internal folder!"

  cd ..
fi

#
# Finished!
#
clear
echo "********************************************"
echo "* TerraLib Installer for Mac OS X Yosemite *"
echo "********************************************"
echo ""
echo "finished successfully!"
