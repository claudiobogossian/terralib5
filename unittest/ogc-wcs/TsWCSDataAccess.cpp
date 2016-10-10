/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/unittest/ogc-wcs/TsWCSDataAccess.cpp

  \brief A test suit for the WS OGC WCS DataAccess.

  \author Emerson Moraes
 */

// TerraLib
#include <terralib/common/Exception.h>
#include <terralib/ws/ogc/wcs/client/WCSClient.h>
#include <terralib/ws/ogc/wcs/dataaccess/Config.h>
#include <terralib/ws/ogc/wcs/dataaccess/Exception.h>
#include <terralib/ws/ogc/wcs/dataaccess/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>
#include <terralib/qt/af/ApplicationController.h>

// Boost
#include <boost/test/unit_test.hpp>

// STL
#include <map>

BOOST_AUTO_TEST_CASE(datasource_open_test)
{
  te::qt::af::AppCtrlSingleton::getInstance().initialize();

  te::da::DataSourceFactory::add(TE_OGC_WCS_DRIVER_IDENTIFIER, te::ws::ogc::wcs::da::Build);

  // Directory where temporary WCS data will be stored.
  std::string usrDataDir = te::qt::af::AppCtrlSingleton::getInstance().getUserDataDir().toUtf8().data();

  // WCS server URL.
  std::string url = "http://demo.opengeo.org/geoserver/ows";

  // Version that will be used on WCS Requests.
  std::string version = "2.0.1";

  std::string connInfo(url + "?VERSION=" + version + "&USERDATADIR=" + usrDataDir);

  // Perform connection
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("WCS2", connInfo);
  ds->open();

  BOOST_CHECK(ds->isOpened());

  return;
}

BOOST_AUTO_TEST_CASE(datasource_open_exception_test)
{
  te::qt::af::AppCtrlSingleton::getInstance().initialize();

  // Directory where temporary WCS data will be stored.
  std::string usrDataDir = te::qt::af::AppCtrlSingleton::getInstance().getUserDataDir().toUtf8().data();

  // WCS server URL.
  std::string url = "http://demo.opengeo.org/geoserver/ows";

  // Version that will be used on WCS Requests.
  std::string version = "2.0.1";

  std::string connInfo;

  // Perform connection
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("WCS2", connInfo);

  // Test without any correct connection parameter
  BOOST_CHECK_THROW(ds->open(), te::ws::ogc::wcs::da::Exception);

  connInfo = url;
  ds.reset(te::da::DataSourceFactory::make("WCS2", connInfo).release());

  // Test with URI correct connection parameter
  BOOST_CHECK_THROW(ds->open(), te::ws::ogc::wcs::da::Exception);

  connInfo = version;
  ds.reset(te::da::DataSourceFactory::make("WCS2", connInfo).release());

  // Test with VERSION correct connection parameter
  BOOST_CHECK_THROW(ds->open(), te::ws::ogc::wcs::da::Exception);

  connInfo = usrDataDir;
  ds.reset(te::da::DataSourceFactory::make("WCS2", connInfo).release());

  // Test with USERDATADIR correct connection parameter
  BOOST_CHECK_THROW(ds->open(), te::ws::ogc::wcs::da::Exception);

  connInfo = url + "?VERSION=" + version;
  ds.reset(te::da::DataSourceFactory::make("WCS2", connInfo).release());

  // Test with URI and VERSION correct connection parameters
  BOOST_CHECK_THROW(ds->open(), te::ws::ogc::wcs::da::Exception);

  connInfo = url + "?USERDATADIR=" + usrDataDir;
  ds.reset(te::da::DataSourceFactory::make("WCS2", connInfo).release());

  // Test with URI and USERDATADIR correct connection parameters
  BOOST_CHECK_THROW(ds->open(), te::ws::ogc::wcs::da::Exception);

  connInfo = "?VERSION=" + version + "&USERDATADIR=" + usrDataDir;
  ds.reset(te::da::DataSourceFactory::make("WCS2", connInfo).release());

  // Test with VERSION and USERDATADIR correct connection parameters
  BOOST_CHECK_THROW(ds->open(), te::ws::ogc::wcs::da::Exception);

  return;
}
