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
  \file TsMosaic.h
 
  \brief A test suit for the Mosaic Class.
 */

#ifndef __TERRALIB_UNITTEST_RP_MOSAIC_INTERNAL_H
#define __TERRALIB_UNITTEST_RP_MOSAIC_INTERNAL_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

// Terralib
#include <terralib/rp.h>

/*!
  \class TsMosaic

  \brief A test suit for the Mosaic class interface.
 */
class TsMosaic : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsMosaic );
  
  CPPUNIT_TEST( SequenceMosaicTest );
  
  CPPUNIT_TEST( TiePointsMosaicTest2 );
 
  CPPUNIT_TEST( TiePointsMosaicTest1 );  
  
  CPPUNIT_TEST( GeoReferencedImagesMosaicTest );
  
  CPPUNIT_TEST( GeoReferencedImagesMosaicWithReprojectionTest );
  
  CPPUNIT_TEST_SUITE_END();

protected :
  
  void SequenceMosaicTest();

  void GeoReferencedImagesMosaicTest();
  
  void GeoReferencedImagesMosaicWithReprojectionTest();
  
  // All images with the same SRS
  void TiePointsMosaicTest1();
  
  // Imagines with different SRS
  void TiePointsMosaicTest2();
};

#endif
