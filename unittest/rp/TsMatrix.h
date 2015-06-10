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
  \file TsMatrix.h
 
  \brief A test suit for the Matrix Class.
 */

#ifndef __TERRALIB_UNITTEST_RP_Matrix_INTERNAL_H
#define __TERRALIB_UNITTEST_RP_Matrix_INTERNAL_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

// Terralib
#include <terralib/rp.h>


/*!
  \class TsMatrix

  \brief A test suit for the Matrix class interface.

  ....
  <br>
  This test suite will check the Matrix class
  </ul>
 */
class TsMatrix : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsMatrix );

  CPPUNIT_TEST( genericTests );
  CPPUNIT_TEST( rAMMemPolTest );
  CPPUNIT_TEST( diskMemPolTest );
  CPPUNIT_TEST( autoMemPolTest );

  CPPUNIT_TEST_SUITE_END();

protected :

  void genericTests();
  void rAMMemPolTest();
  void diskMemPolTest();
  void autoMemPolTest();
};

#endif
