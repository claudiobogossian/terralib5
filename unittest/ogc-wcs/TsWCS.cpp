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
#include <terralib/ws/ogc/wcs-client/ClientWCS.h>

// TerraLib Test
#include "TsWCS.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TsWCS );

void TsWCS::tsGetCapabilities()
{
  // http://flanche.net:9090/rasdaman/ows?SERVICE=WCS&VERSION=2.0.1&REQUEST=GetCapabilities
  te::ws::ogc::ClientWCS clientWCS("http://flanche.net:9090/rasdaman/ows", "2.0.1");

  CPPUNIT_ASSERT_NO_THROW_MESSAGE("Error at getCapabilities().", clientWCS.getCapabilities());
}

void TsWCS::tsGetCapabilitiesException()
{
  te::ws::ogc::ClientWCS clientWCS("http://wrongadress.net:9090", "2.0.1");

  CPPUNIT_ASSERT_THROW(clientWCS.getCapabilities(), te::common::Exception);
}

void TsWCS::tsDescribeCoverage()
{
  try
  {
    // http://flanche.net:9090/rasdaman/ows?SERVICE=WCS&VERSION=2.0.1&REQUEST=DescribeCoverage&COVERAGEID=NIR

    te::ws::ogc::ClientWCS clientWCS("http://flanche.net:9090/rasdaman/ows", "2.0.1");

    QXmlStreamReader* coverageDescribe = clientWCS.describeCoverage("NIR");

    CPPUNIT_ASSERT(coverageDescribe);
  }
  catch(...)
  {
    CPPUNIT_FAIL("Error in describeCoverage()!");
  }
}

void TsWCS::tsDescribeCoverageException()
{
    te::ws::ogc::ClientWCS clientWCS("http://flanche.net:9090/rasdaman/ows", "2.0.1");

    // Requiring the description of a non-existent coverage
    CPPUNIT_ASSERT_THROW(clientWCS.describeCoverage("NONE"), te::common::Exception);
}
