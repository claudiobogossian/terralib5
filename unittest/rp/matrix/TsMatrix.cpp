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
  \file terralib/unittest/rp/matrix/TsMatrix.cpp

  \brief A test suit for the Matrix interface.
*/

// TerraLib
#include "../Config.h"
#include <terralib/rp.h>
#include <terralib/common/PlatformUtils.h>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (matrix_tests)

BOOST_AUTO_TEST_CASE(generic_tests)
{
  /* Checking basic matrix creation and access */

  te::rp::Matrix< double > m1;
  BOOST_CHECK( m1.reset( 2, 2,
    te::rp::Matrix< double >::RAMMemPol ) );
  BOOST_CHECK( ( ! m1.isEmpty() ) );
  BOOST_CHECK( m1.getMemPolicy() == te::rp::Matrix< double >::RAMMemPol );

  m1( 0 , 0 ) = 0.0;
  m1( 0 , 1 ) = 1.0;
  m1( 1 , 0 ) = 2.0;
  m1( 1 , 1 ) = 3.0;

  BOOST_CHECK( m1.getLinesNumber() == 2 );
  BOOST_CHECK( m1.getColumnsNumber() == 2 );

  BOOST_CHECK( m1( 0 , 0 ) == 0.0 );
  BOOST_CHECK( m1( 0 , 1 ) == 1.0 );
  BOOST_CHECK( m1( 1 , 0 ) == 2.0 );
  BOOST_CHECK( m1( 1 , 1 ) == 3.0 );

  BOOST_CHECK( m1[ 0 ][ 0 ] == 0.0 );
  BOOST_CHECK( m1[ 0 ][ 1 ] == 1.0 );
  BOOST_CHECK( m1[ 1 ][ 0 ] == 2.0 );
  BOOST_CHECK( m1[ 1 ][ 1 ] == 3.0 );

  /* Reset test */

  BOOST_CHECK( m1.reset( 2, 2 ) );
  BOOST_CHECK( m1.getMemPolicy() == te::rp::Matrix< double >::RAMMemPol );

  m1( 0 , 0 ) = 0.0;
  m1( 0 , 1 ) = 1.0;
  m1( 1 , 0 ) = 2.0;
  m1( 1 , 1 ) = 3.0;

  BOOST_CHECK( m1.getLinesNumber() == 2 );
  BOOST_CHECK( m1.getColumnsNumber() == 2 );

  BOOST_CHECK( m1( 0 , 0 ) == 0.0 );
  BOOST_CHECK( m1( 0 , 1 ) == 1.0 );
  BOOST_CHECK( m1( 1 , 0 ) == 2.0 );
  BOOST_CHECK( m1( 1 , 1 ) == 3.0 );

  /* Checking copy operator */

  te::rp::Matrix< double > m2;
  BOOST_CHECK( m2.isEmpty() );
  BOOST_CHECK( m2.getMemPolicy() == te::rp::Matrix< double >::RAMMemPol );

  BOOST_CHECK( m2.getLinesNumber() == 0 );
  BOOST_CHECK( m2.getColumnsNumber() == 0 );

  m2 = m1;

  BOOST_CHECK( m2.getLinesNumber() == 2 );
  BOOST_CHECK( m2.getColumnsNumber() == 2 );

  BOOST_CHECK( m2( 0 , 0 ) == 0.0 );
  BOOST_CHECK( m2( 0 , 1 ) == 1.0 );
  BOOST_CHECK( m2( 1 , 0 ) == 2.0 );
  BOOST_CHECK( m2( 1 , 1 ) == 3.0 );

  /* Checking copy constructor */

  te::rp::Matrix< double > m3( m1 );
  BOOST_CHECK( m3.getMemPolicy() == te::rp::Matrix< double >::RAMMemPol );

  BOOST_CHECK( m3.getLinesNumber() == 2 );
  BOOST_CHECK( m3.getColumnsNumber() == 2 );

  BOOST_CHECK( m3( 0 , 0 ) == 0.0 );
  BOOST_CHECK( m3( 0 , 1 ) == 1.0 );
  BOOST_CHECK( m3( 1 , 0 ) == 2.0 );
  BOOST_CHECK( m3( 1 , 1 ) == 3.0 );
}

BOOST_AUTO_TEST_CASE(rAMMemPol_test)
{
  const unsigned int max_ram  = (unsigned int)
    ( 0.20 * (double)MIN( ( te::common::GetTotalVirtualMemory() / 2 ) - 
    te::common::GetUsedVirtualMemory(), te::common::GetTotalPhysicalMemory() ) );

  unsigned long int lines = (unsigned long int)(
    ( (double)( max_ram ) ) /
    ( ( (double)sizeof( double ) ) * 1000.00 ) );
  unsigned long int cols = 1000;

  te::rp::Matrix< double > m1;
  BOOST_CHECK( m1.reset( lines, cols, te::rp::Matrix< double >::RAMMemPol ) );

  BOOST_CHECK( m1.getMemPolicy() == te::rp::Matrix< double >::RAMMemPol );

  double counter = 0.0;

  unsigned int line = 0;
  unsigned int col = 0;

  for( line = 0 ; line < lines ; ++line ) {
    for( col = 0 ; col < cols ; ++col ) {
      m1( line , col ) = counter;

      ++counter;
    }
  }

  counter = 0.0;

  for( line = 0 ; line < lines ; ++line ) {
    for( col = 0 ; col < cols ; ++col ) {
      BOOST_CHECK( ( m1( line , col ) == counter ) );

      ++counter;
    }
  }
  
  m1.reset();

  return;
}

BOOST_AUTO_TEST_CASE(autoMemPol_test)
{
  const unsigned long long int max_ram  = te::common::GetTotalPhysicalMemory() / 10;

  unsigned long int lines = (unsigned long int)
    (
      ( 2 * max_ram ) 
      /
      ( sizeof( double ) * 1000 ) 
    );
  unsigned long int cols = 1000;

  te::rp::Matrix< double > m1;
  BOOST_CHECK( m1.reset( lines, cols, te::rp::Matrix< double >::AutoMemPol, 10 ) );

  BOOST_CHECK( m1.getMemPolicy() == te::rp::Matrix< double >::AutoMemPol );

  double counter = 0.0;

  unsigned int line = 0;
  unsigned int col = 0;

  for( line = 0 ; line < lines ; ++line ) {
    for( col = 0 ; col < cols ; ++col ) {
      m1( line , col ) = counter;

      ++counter;
    }
  }

  counter = 0.0;

  for( line = 0 ; line < lines ; ++line ) {
    for( col = 0 ; col < cols ; ++col ) {
      BOOST_CHECK( ( m1( line , col ) == counter ) );

      ++counter;
    }
  }
  
  m1.reset();

  return;
}

BOOST_AUTO_TEST_CASE(diskMemPol_test)
{
  unsigned long int lines = 3000;
  unsigned long int cols = 4000;

  te::rp::Matrix< double > m1;
  BOOST_CHECK( m1.reset( lines, cols,
    te::rp::Matrix< double >::DiskMemPol ) );

  BOOST_CHECK( m1.getMemPolicy() == te::rp::Matrix< double >::DiskMemPol );
  BOOST_CHECK( m1.getLinesNumber() == lines );
  BOOST_CHECK( m1.getColumnsNumber() == cols );

  unsigned int line = 0;
  unsigned int col = 0;
  double counter = 0.0;

  for( line = 0 ; line < lines ; ++line ) {
    for( col = 0 ; col < cols ; ++col ) {
      m1( line , col ) = counter;
      ++counter;
    }
  }

  counter = 0.0;

  for( line = 0 ; line < lines ; ++line ) {
    for( col = 0 ; col < cols ; ++col ) {
      BOOST_CHECK( ( m1( line , col ) == counter ) );
      ++counter;
    }
  }
  
  m1.reset();
  
  return;
}

BOOST_AUTO_TEST_SUITE_END()
