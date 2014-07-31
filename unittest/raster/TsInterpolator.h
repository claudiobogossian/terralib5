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
  \file TsInterpolator.h
 
  \brief A test suit for the Raster Class.
 */

#ifndef __TERRALIB_UNITTEST_RASTER_INTERNAL_INTERPOLATOR_H
#define __TERRALIB_UNITTEST_RASTER_INTERNAL_INTERPOLATOR_H

#include <terralib/raster.h>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

#include <memory>

/*!
  \class TsInterpolator

  \brief It interpolates one pixel based on a selected algorithm.
         Methods currently available are Nearest Neighbor, Bilinear, and Bicubic.
  ....
  <br>
  This test suite will check the Interpolator class
  </ul>
 */
class TsInterpolator : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsInterpolator );
  
  CPPUNIT_TEST(tcNearestNeighbor );

  CPPUNIT_TEST(tcBilinear );

  CPPUNIT_TEST(tcBicubic );

  // add other tests (tcXxxx)

  CPPUNIT_TEST_SUITE_END();

  protected :
    
    void createTestRaster( unsigned int nBands, unsigned int nLines, 
      unsigned int nCols, std::auto_ptr< te::rst::Raster >& rasterPointer );
    
    void tcNearestNeighbor();
   
    void tcBilinear();
   
    void tcBicubic();    

    // add other tests...
};

#endif  // __TERRALIB_UNITTEST_RASTER_INTERNAL_INTERPOLATOR_H

