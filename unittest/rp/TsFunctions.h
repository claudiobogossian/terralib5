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
  \file TsFunctions.h
 
  \brief A test suit for raster processing Functions.
 */

#ifndef __TERRALIB_UNITTEST_RP_FUNCTIONS_INTERNAL_H
#define __TERRALIB_UNITTEST_RP_FUNCTIONS_INTERNAL_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsFunctions

  \brief A test suit for the Functions class interface.
 */
class TsFunctions : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsFunctions );

  CPPUNIT_TEST( GetStdDevValue );
  
  CPPUNIT_TEST( GetStdDevValueOptimized );
  
  CPPUNIT_TEST( PrincipalComponents );
  
  CPPUNIT_TEST( PrincipalComponentsOptimized );
  
  CPPUNIT_TEST( getMeanValue );
  
  CPPUNIT_TEST( getMeanValueOptimized );
  
  CPPUNIT_TEST( getCovarianceValue );
  
  CPPUNIT_TEST( getCovarianceValueOptimized );
  
  CPPUNIT_TEST( ihs );
  
  CPPUNIT_TEST( DecomposeBands );
  
  CPPUNIT_TEST( ComposeBandsSameSRID );
  
  CPPUNIT_TEST( ComposeBandsDifSRID );
  
  CPPUNIT_TEST( GetDetailedExtent );
  
  CPPUNIT_TEST( GetIndexedDetailedExtent );
  
  CPPUNIT_TEST( WaveletAtrous );

  CPPUNIT_TEST_SUITE_END();

protected :

  void ihs();
  
  void getMeanValue();
  
  void getMeanValueOptimized();
  
  void getCovarianceValue();
  
  void getCovarianceValueOptimized();
  
  void PrincipalComponents();
  
  void PrincipalComponentsOptimized();
  
  void GetStdDevValue();
  
  void GetStdDevValueOptimized();
  
  void DecomposeBands();
  
  void ComposeBandsSameSRID();
  
  void ComposeBandsDifSRID();
  
  void GetDetailedExtent();
  
  void GetIndexedDetailedExtent();
  
  void WaveletAtrous();
  
};

#endif
