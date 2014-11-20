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
  \file TsClassifier.h

  \brief A test suit for the Classifier Class.
 */

#ifndef __TERRALIB_UNITTEST_RP_CLASSIFIER_INTERNAL_H
#define __TERRALIB_UNITTEST_RP_CLASSIFIER_INTERNAL_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

// Terralib
#include <terralib/rp.h>


/*!
  \class TsClassifier

  \brief A test suit for the Classifier class interface.

  ....
  <br>
  This test suite will check the Classifier class
  </ul>
 */
class TsClassifier : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( TsClassifier );

  CPPUNIT_TEST( SAM );

  CPPUNIT_TEST( MAP );

  CPPUNIT_TEST( ISOSeg );

  CPPUNIT_TEST( EM );

  CPPUNIT_TEST( KMeans );

  CPPUNIT_TEST_SUITE_END();

  protected :

    void ISOSeg();

    void MAP();

    void EM();
    
    void SAM();

    void KMeans();

};

#endif  // __TERRALIB_UNITTEST_RP_CLASSIFIER_INTERNAL_H

