/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file TsFilter.h

  \brief A test suit for the Filter Class.
 */

#ifndef __TERRALIB_UNITTEST_RP_FILTER_INTERNAL_H
#define __TERRALIB_UNITTEST_RP_FILTER_INTERNAL_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsFilter

  \brief A test suit for the Filter class interface.
 */
class TsFilter : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( TsFilter );

  CPPUNIT_TEST( SobelFilter );
  CPPUNIT_TEST( RobertsFilter );
  CPPUNIT_TEST( MeanFilter );
  CPPUNIT_TEST( ModeFilter );
  CPPUNIT_TEST( MedianFilter );
  CPPUNIT_TEST( DilationFilter );
  CPPUNIT_TEST( ErosionFilter );
  CPPUNIT_TEST( UserDefinedWindow );

  CPPUNIT_TEST_SUITE_END();

protected :

  void SobelFilter();
  void RobertsFilter();
  void MeanFilter();
  void ModeFilter();
  void MedianFilter();
  void DilationFilter();
  void ErosionFilter();
  void UserDefinedWindow();

};

#endif
