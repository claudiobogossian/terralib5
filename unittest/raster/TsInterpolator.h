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
#include <boost/shared_ptr.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

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
  
  CPPUNIT_TEST(tcInterpolatorConstructor );
  
  CPPUNIT_TEST(tcGetValue );

  CPPUNIT_TEST(tcGetValues );

  CPPUNIT_TEST(tcNearestNeighborGetValue );

  CPPUNIT_TEST(tcBilinearGetValue );

  CPPUNIT_TEST(tcBicubicGetValue );

  CPPUNIT_TEST(tcInterpolationFunction );


  // add other tests (tcXxxx)

  CPPUNIT_TEST_SUITE_END();

  protected :
    
    void tcInterpolatorConstructor();

    void tcGetValue();
    
    void tcGetValues();

    void tcNearestNeighborGetValue();
   
    void tcBilinearGetValue();
   
    void tcBicubicGetValue();
   
    void tcInterpolationFunction();
    

    // add other tests...
};

#endif  // __TERRALIB_UNITTEST_RASTER_INTERNAL_INTERPOLATOR_H

