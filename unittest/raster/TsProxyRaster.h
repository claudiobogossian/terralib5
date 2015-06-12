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
  \file TsProxyRaster.h
 
  \brief A test suit for the ProxyRaster Class.
 */

#ifndef __TERRALIB_UNITTEST_RASTER_INTERNAL_PROXYRASTER_H
#define __TERRALIB_UNITTEST_RASTER_INTERNAL_PROXYRASTER_H

#include <terralib/raster.h>
#include <boost/shared_ptr.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsProxyRaster

  \brief A test suit for the ProxyRaster class interface.

  ....
  <br>
  This test suite will check the ProxyRaster class
  </ul>
 */
class TsProxyRaster : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsProxyRaster );
  
  CPPUNIT_TEST(tcProxyRasterConstructor );
  
  CPPUNIT_TEST(tcSwap );

  CPPUNIT_TEST(tcOpen );

  CPPUNIT_TEST(tcGetInfo );

  CPPUNIT_TEST(tcGetNumberOfBands );

  CPPUNIT_TEST(tcGetBandDataType);

  CPPUNIT_TEST(tcGetBandI );
 
  CPPUNIT_TEST(tcGetBand );

  CPPUNIT_TEST(tcBandOperatorI );

  CPPUNIT_TEST(tcBandOperator );

  CPPUNIT_TEST(tcBandClone );


  // add other tests (tcXxxx)

  CPPUNIT_TEST_SUITE_END();

  protected :
    
    void tcProxyRasterConstructor();

    void tcSwap();
    
    void tcOpen();
    
    void tcGetInfo();
    
    void tcGetNumberOfBands();
    
    void tcGetBandDataType();
    
    void tcGetBandI();
    
    void tcGetBand();
    
    void tcBandOperatorI();
    
    void tcBandOperator();
    
    void tcBandClone();


    // add other tests...
};

#endif  // __TERRALIB_UNITTEST_RASTER_INTERNAL_PROXYRASTER_H

