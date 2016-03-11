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
  \file TsWCS.h

  \brief A test suit for the WS OGC WCS.
 */

// TerraLib
#include <terralib/common/Exception.h>
#include <terralib/ws/ogc/wcs/client/WCS.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/ws/ogc/wcs/dataaccess/Transactor.h>

// TerraLib Test
#include "TsWCS.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TsWCS );

void TsWCS::tsGetCapabilities()
{
  // http://flanche.net:9090/rasdaman/ows?SERVICE=WCS&VERSION=2.0.1&REQUEST=GetCapabilities
  //  te::ws::ogc::WCS clientWCS("http://flanche.net:9090/rasdaman/ows", "2.0.1");

  // http://sedac.ciesin.columbia.edu/geoserver/wcs?service=WCS&request=GetCapabilities&version=2.0.1
  te::ws::ogc::WCS clientWCS("http://sedac.ciesin.columbia.edu/geoserver/wcs", "2.0.1");

  CPPUNIT_ASSERT_NO_THROW_MESSAGE("Error at getCapabilities().", clientWCS.updateCapabilities());
}

void TsWCS::tsGetCapabilitiesException()
{
  te::ws::ogc::WCS clientWCS("http://wrongadress.net:9090", "2.0.1");

  CPPUNIT_ASSERT_THROW(clientWCS.updateCapabilities(), te::common::Exception);
}

void TsWCS::tsDescribeCoverage()
{
  try
  {
    // http://flanche.net:9090/rasdaman/ows?SERVICE=WCS&VERSION=2.0.1&REQUEST=DescribeCoverage&COVERAGEID=NIR
    //    te::ws::ogc::WCS clientWCS("http://flanche.net:9090/rasdaman/ows", "2.0.1");
    //    QXmlStreamReader* coverageDescribe = clientWCS.describeCoverage("NIR");

    // http://sedac.ciesin.columbia.edu/geoserver/wcs?service=WCS&request=DescribeCoverage&coverageid=other__wcmc-world-database-of-protected-areas&version=2.0.1
    te::ws::ogc::WCS clientWCS("http://sedac.ciesin.columbia.edu/geoserver/wcs", "2.0.1");
    std::string coverage = "other__wcmc-world-database-of-protected-areas";
    te::ws::ogc::CoverageDescription coverageDescription;

    CPPUNIT_ASSERT_NO_THROW_MESSAGE("Error at describeCoverage().", coverageDescription = clientWCS.describeCoverage(coverage));

    if(coverageDescription.coverageId.compare(coverage) != 0)
    {
      CPPUNIT_FAIL("Error: Is not the requested coverage!");
    }

  }
  catch(...)
  {
    CPPUNIT_FAIL("Error in describeCoverage()!");
  }
}

void TsWCS::tsDescribeCoverageException()
{
  te::ws::ogc::WCS clientWCS("http://flanche.net:9090/rasdaman/ows", "2.0.1");

  // Requiring the description of a non-existent coverage
  CPPUNIT_ASSERT_THROW(clientWCS.describeCoverage("NONE"), te::common::Exception);
}


void TsWCS::tsDataSource()
{
  try
  {
    te::da::DataSourcePtr ds(te::da::DataSourceFactory::make("OGC_WCS").release());

    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = "http://sedac.ciesin.columbia.edu/geoserver/wcs";
    connInfo["VERSION"] = "2.0.1";

    ds->setConnectionInfo(connInfo);

    ds->open();

    if(!ds->isOpened())
      CPPUNIT_FAIL("Error!");

    if(!ds->isValid())
      CPPUNIT_FAIL("Error!");

    te::ws::ogc::wcs::da::Transactor* transactor = dynamic_cast< te::ws::ogc::wcs::da::Transactor* > (ds->getTransactor().release());

//    std::auto_ptr < te::ws::ogc::wcs::da::Transactor > transactor = ds->getTransactor();

    transactor->getNumberOfDataSets();

    std::vector<std::string> dataSetNames = transactor ->getDataSetNames();

    if(dataSetNames.empty())
      CPPUNIT_FAIL("Error!");

    std::string dataSetName = dataSetNames.at(0);

    if(!transactor->dataSetExists(dataSetName))
      CPPUNIT_FAIL("Error!");

    te::ws::ogc::CoverageDescription coverageDescription = transactor->coverageDescription(dataSetName);
    te::ws::ogc::CoverageRequest coverageRequest;
    coverageRequest.coverageID = dataSetName;
    coverageRequest.format = coverageDescription.serviceParameters.nativeFormat;

    te::ws::ogc::Coordinate co= coverageDescription.boundedBy.coordinate.at(0);

    co.coord.at(0) = "0";
    co.coord.at(1) = "0";

    coverageDescription.boundedBy.coordinate.at(0) = co;

    co.coord.at(0) = "10";
    co.coord.at(1) = "10";
    coverageDescription.boundedBy.coordinate.at(1) = co;

    coverageRequest.boundedBy = coverageDescription.boundedBy;
    coverageRequest.subSet = coverageDescription.domainSet.subSet;

    transactor->setCoverageRequest(coverageRequest);

    std::auto_ptr<te::da::DataSet> dataSet = transactor->getDataSet(dataSetName);

    if(!dataSet.get())
      CPPUNIT_FAIL("Error!");
  }
  catch(te::common::Exception& e)
  {
    CPPUNIT_FAIL(e.what());
  }
  catch(...)
  {
    CPPUNIT_FAIL("Unknow error!");
  }
}
