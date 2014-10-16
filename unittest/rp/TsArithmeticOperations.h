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
  \file TsArithmeticOperations.h

  \brief A test suit for the Arithmetic Operations algorithm.
 */

#ifndef __TERRALIB_UNITTEST_RP_ARITHMETIC_OPERATIONS_INTERNAL_H
#define __TERRALIB_UNITTEST_RP_ARITHMETIC_OPERATIONS_INTERNAL_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsArithmeticOperations

  \brief A test suit for the ArithmeticOperations algorithm.

  ....
  <br>
  This test suite will check the ArithmeticOperations algorithm
  </ul>
 */
class TsArithmeticOperations : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( TsArithmeticOperations );
  
  CPPUNIT_TEST( RasterSum );
  
  CPPUNIT_TEST( RasterSubtraction );
  
  CPPUNIT_TEST( RasterProduct );
  
  CPPUNIT_TEST( RasterDivision );

  CPPUNIT_TEST( RealSum );
  
  CPPUNIT_TEST( RealSubtraction );
  
  CPPUNIT_TEST( RealSubtractionInverse );

  CPPUNIT_TEST( RealProduct );
  
  CPPUNIT_TEST( RealDivision );
  
  CPPUNIT_TEST( RealDivisionInverse );
  
  CPPUNIT_TEST_SUITE_END();

  protected :
    
  void loadDoubleRaster( const std::string& rasterFileName, std::auto_ptr< te::rst::Raster >& rasterPtr );

  void RasterSum();
  
  void RasterSubtraction();
  
  void RasterProduct();
  
  void RasterDivision();

  void RealSum();
  
  void RealSubtraction();
  
  void RealSubtractionInverse();

  void RealProduct();
  
  void RealDivision();
  
  void RealDivisionInverse();

};

#endif  // __TERRALIB_UNITTEST_RP_ArithmeticOperations_INTERNAL_H

