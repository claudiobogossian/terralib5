/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

// Unit-Test TerraLib
#include "TsMatrixUtils.h"

#include <terralib/common/MatrixUtils.h>

#include <cstdlib>

CPPUNIT_TEST_SUITE_REGISTRATION( TsMatrixUtils );

void TsMatrixUtils::setUp()
{
}

void TsMatrixUtils::tearDown()
{
}

void TsMatrixUtils::getInverseMatrix()
{
  boost::numeric::ublas::matrix< double > inputMatrix( 3, 3 );
  inputMatrix( 0 , 0 ) = 1;
  inputMatrix( 0 , 1 ) = 2;
  inputMatrix( 0 , 2 ) = 3;
  inputMatrix( 1 , 0 ) = 4;
  inputMatrix( 1 , 1 ) = 5;
  inputMatrix( 1 , 2 ) = 6;
  inputMatrix( 2 , 0 ) = 7;
  inputMatrix( 2 , 1 ) = 8;
  inputMatrix( 2 , 2 ) = 1;
  
  boost::numeric::ublas::matrix< double > outputMatrix( 3, 3 );
  CPPUNIT_ASSERT( te::common::GetInverseMatrix( inputMatrix, outputMatrix ) );
  
  CPPUNIT_ASSERT( std::abs( outputMatrix( 0 , 0 ) + 1.791667 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 0 , 1 ) - 0.916667 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 0 , 2 ) + 0.125000 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 1 , 0 ) - 1.583333 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 1 , 1 ) + 0.833333 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 1 , 2 ) - 0.250000 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 2 , 0 ) + 0.125000 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 2 , 1 ) - 0.250000 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 2 , 2 ) + 0.125000 ) < 0.000001 );
}

void TsMatrixUtils::getPseudoInverseMatrix()
{
  boost::numeric::ublas::matrix< double > inputMatrix( 3, 3 );
  inputMatrix( 0 , 0 ) = 1;
  inputMatrix( 0 , 1 ) = 2;
  inputMatrix( 0 , 2 ) = 3;
  inputMatrix( 1 , 0 ) = 4;
  inputMatrix( 1 , 1 ) = 5;
  inputMatrix( 1 , 2 ) = 6;
  inputMatrix( 2 , 0 ) = 7;
  inputMatrix( 2 , 1 ) = 8;
  inputMatrix( 2 , 2 ) = 1;
  
  boost::numeric::ublas::matrix< double > outputMatrix( 3, 3 );
  CPPUNIT_ASSERT( te::common::GetPseudoInverseMatrix( inputMatrix, outputMatrix ) );
  
  CPPUNIT_ASSERT( std::abs( outputMatrix( 0 , 0 ) + 1.791667 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 0 , 1 ) - 0.916667 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 0 , 2 ) + 0.125000 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 1 , 0 ) - 1.583333 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 1 , 1 ) + 0.833333 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 1 , 2 ) - 0.250000 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 2 , 0 ) + 0.125000 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 2 , 1 ) - 0.250000 ) < 0.000001 );
  CPPUNIT_ASSERT( std::abs( outputMatrix( 2 , 2 ) + 0.125000 ) < 0.000001 );
}

void TsMatrixUtils::GetDeterminant()
{
  {
    boost::numeric::ublas::matrix< double > inputMatrix( 1, 1 );
    inputMatrix( 0 , 0 ) = 1;
    double determinant = 0;
    CPPUNIT_ASSERT( te::common::GetDeterminant( inputMatrix, determinant ) );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( determinant, 1.0, 0.0000000001 );
  }

  {
    boost::numeric::ublas::matrix< double > inputMatrix( 2, 2 );
    inputMatrix( 0 , 0 ) = 5;
    inputMatrix( 0 , 1 ) = 3;
    inputMatrix( 1 , 0 ) = 2;
    inputMatrix( 1 , 1 ) = 4;
    double determinant = 0;
    CPPUNIT_ASSERT( te::common::GetDeterminant( inputMatrix, determinant ) );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( determinant, 14, 0.0000000001 );
  }
  
  {
    boost::numeric::ublas::matrix< double > inputMatrix( 3, 3 );
    inputMatrix( 0 , 0 ) = 1;
    inputMatrix( 0 , 1 ) = 2;
    inputMatrix( 0 , 2 ) = 3;
    inputMatrix( 1 , 0 ) = 4;
    inputMatrix( 1 , 1 ) = 5;
    inputMatrix( 1 , 2 ) = 6;
    inputMatrix( 2 , 0 ) = 7;
    inputMatrix( 2 , 1 ) = 8;
    inputMatrix( 2 , 2 ) = 10;
    
    double determinant = 0;
    CPPUNIT_ASSERT( te::common::GetDeterminant( inputMatrix, determinant ) );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( determinant, -3.0, 0.0000000001 );
  }  

}

