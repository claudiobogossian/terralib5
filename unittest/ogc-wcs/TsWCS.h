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


#ifndef __TERRALIB_UNITTEST_WCS_H
#define __TERRALIB_UNITTEST_WCS_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>


class TsWCS : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsWCS );

// It registers the class methods as Test Cases belonging to the suit
//  CPPUNIT_TEST( tsGetCapabilities );

//  CPPUNIT_TEST( tsGetCapabilitiesException );

//  CPPUNIT_TEST( tsDescribeCoverage );

//  CPPUNIT_TEST( tsDescribeCoverageException );

  CPPUNIT_TEST( tsDataSource );

  CPPUNIT_TEST_SUITE_END();

protected:

  void tsGetCapabilities();

  void tsGetCapabilitiesException();

  void tsDescribeCoverage();

  void tsDescribeCoverageException();

  void tsDataSource();

};

#endif // __TERRALIB_UNITTEST_WCS_H
