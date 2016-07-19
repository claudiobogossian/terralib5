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
  \file terralib/unittest/ogc-wcs/TsWCSClient.cpp

  \brief A test suit for the WS OGC WCSClient.

  \author Emerson Moraes
 */

// TerraLib
#include <terralib/common/Exception.h>
#include <terralib/ws/ogc/wcs/client/WCSClient.h>
#include <terralib/ws/core/CurlWrapper.h>
#include <terralib/qt/af/ApplicationController.h>
#include "MockCurlWrapper.h"

// Boost
#include <boost/test/unit_test.hpp>

// LibCurl
#include <curl/curl.h>

#include <memory>
#include <utility>


BOOST_AUTO_TEST_CASE(getcapabilities_test)
{
  te::qt::af::AppCtrlSingleton::getInstance().initialize();

  // Directory where test WCS test data is stored.
  std::string dataDir = TERRALIB_DATA_DIR;

  // WCS server URL.
  std::string url = "http://demo.opengeo.org/geoserver/ows";

  MockCurlWrapper* mock = new MockCurlWrapper();

  // Version that will be used on WCS Requests.
  std::string version = "2.0.1";

  te::ws::ogc::WCSClient client (dataDir, url, version);

  client.setCurlWrapper(mock);

  // Makes GetCapabilities Test Request.
  BOOST_CHECK_NO_THROW(client.updateCapabilities());

  te::ws::ogc::Capabilities capabilities = client.getCapabilities();

  std::vector<std::string> coverages = capabilities.coverages;

  BOOST_CHECK(coverages.size() > 0);

  return;
}

BOOST_AUTO_TEST_CASE(getcapabilities_exception_test)
{

  te::qt::af::AppCtrlSingleton::getInstance().initialize();

  // Directory where test WCS test data is stored.
  std::string dataDir = TERRALIB_DATA_DIR;

  // A wrong WCS server URL.
  std::string url = "http://wrongaddress.org/geoserver/ows";

  // Version that will be used on WCS Requests.
  std::string version = "2.0.1";

  te::ws::ogc::WCSClient client (dataDir, url, version);

  // Makes GetCapabilities Request.
  BOOST_CHECK_THROW(client.updateCapabilities(), te::common::Exception);

  return;
}

BOOST_AUTO_TEST_CASE(not_supported_version_test)
{
  te::qt::af::AppCtrlSingleton::getInstance().initialize();

  // Directory where test WCS test data is stored.
  std::string dataDir = TERRALIB_DATA_DIR;

  // WCS server URL.
  std::string url = "http://demo.opengeo.org/geoserver/ows";

  // Version not supported yet.
  std::string version = "1.0.0";

  te::ws::ogc::WCSClient client (dataDir, url, version);

  BOOST_CHECK_THROW(client.updateCapabilities(), te::common::Exception);

  // Version not supported yet.
  version = "1.1.0";

  client = te::ws::ogc::WCSClient (dataDir, url, version);

  BOOST_CHECK_THROW(client.updateCapabilities(), te::common::Exception);

  return;
}

BOOST_AUTO_TEST_CASE(describecoverage_test)
{
  te::qt::af::AppCtrlSingleton::getInstance().initialize();

  // Directory where test WCS test data is stored.
  std::string dataDir = TERRALIB_DATA_DIR;

  // WCS server URL.
  std::string url = "http://demo.opengeo.org/geoserver/ows";

  // Version that will be used on WCS Requests.
  std::string version = "2.0.1";

  te::ws::ogc::WCSClient client (dataDir, url, version);

  std::string coverageName = "nasa__bluemarble";

  BOOST_CHECK_NO_THROW(client.describeCoverage(coverageName));

  te::ws::ogc::CoverageDescription description = client.describeCoverage(coverageName);

  BOOST_CHECK(description.coverageId == coverageName);

  return;
}

BOOST_AUTO_TEST_CASE(getcoverage_test)
{
  te::qt::af::AppCtrlSingleton::getInstance().initialize();

  // Directory where test WCS test data is stored.
  std::string dataDir = TERRALIB_DATA_DIR;

  // WCS server URL.
  std::string url = "http://demo.opengeo.org/geoserver/ows";

  // Version that will be used on WCS Requests.
  std::string version = "2.0.1";

  te::ws::ogc::WCSClient client (dataDir, url, version);

  // Coverage ID that could be retrieved on DescribeCoverage.
  std::string coverageId = "nasa__bluemarble";

  // Simple image request by id.
  te::ws::ogc::CoverageRequest request;
  request.coverageID = coverageId;

  std::string coverageDiskPath = client.getCoverage(request);

  std::string expectedResult = dataDir + std::string("/wcs/") + coverageId;

  BOOST_CHECK(coverageDiskPath == expectedResult);

  return;
}
